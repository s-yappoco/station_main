///////////////////////////////////////////////
// マルチコア処理　core1
// core1_sound
///////////////////////////////////////////////

// include
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/pwm.h"

#include "core1_sound.h"
#include "station_define.h"
#include "sounddata.h"


// グローバル変数
extern semaphore_t sem;     // 排他処理用のセマフォ

uint32_t muteTimer = 0;     // ミュート制御用のタイマー

// 音声鳴動ステータスの定義
extern struct sound_sts line1Soundsts;  // 1番線用
extern struct sound_sts line2Soundsts;  // 2番専用

//スライス番号保存用
uint slice_num;

void core1_main(){

    // オンボードLEDピン番号設定
    const uint8_t LED_PIN = PICO_DEFAULT_LED_PIN;
    //const uint8_t TIMER_ALARM_NUM = 1;    // CORE1で使用するタイマーアラーム番号（おそらく0～3までのどれか）
    //const uint8_t MAX_TIMERS = 200;           // CORE1で使用するタイマーの最大値 
    //const uint32_t TIMER_USEC = 1000000/6000; // タイマーを呼び出す間隔(usec単位) 音声ファイル6khzサンプリング

    uint8_t audio_duty;

    ///////////////////////////////////////////
    // PWM設定関連
    ///////////////////////////////////////////
    // 対応ピン初期化
    gpio_init(PIN_AUDIOOUT);        // 音声出力ピン(PWM出力)

    // 出力設定
    gpio_set_dir(PIN_AUDIOOUT,GPIO_OUT);    // 音声出力ピン(PWM出力)

    // 該当GPをPWMに設定する
    gpio_set_function(PIN_AUDIOOUT, GPIO_FUNC_PWM);
    //スライス番号を取得する
    slice_num = pwm_gpio_to_slice_num(PIN_AUDIOOUT);

    // PWMの周波数を100kHzに設定
    float frequency = PWM_FREQ;
    float divider = (float)CLOCK_SPEED / frequency/10;
    // pwm_set_clkdiv(slice_num, divider);

    // PWMのカウンタを255に設定
    // このカウンタ値になると出力pinはhighになり、カウンタ値が0に戻る
    pwm_set_wrap(slice_num, RESOLUTION - 1);

    // PWMのデューティー比設定
    audio_duty = RESOLUTION / 2;        // 中央値を出力
    // 注意：picoのPWMの各スライスのチャンネルについて
    // GP偶数番のピンはchan_a、奇数はchan_bと決まっています。
    // https://lipoyang.hatenablog.com/entry/2021/12/12/201432
    // スライス番号もピン番号によって決まっています。
    // https://www.route55go.com/self-study/pico/class/s028_pwm/
    pwm_set_chan_level(slice_num, PWM_CHAN_A, audio_duty);

    // pwm出力有効
    pwm_set_enabled(slice_num, true);


    // タイマー割り込み関連準備
    // 別タイマーでのalarm pool変数準備
    //alarm_pool_t* core1_alarm;
    // 準備した変数の初期設定
    //core1_alarm = alarm_pool_create(TIMER_ALARM_NUM,MAX_TIMERS);
    // 繰り返しタイマーを設定
    struct repeating_timer timer;
    // タイマーによる割り込み関数を設定し、タイマー割り込みを実行する
    //alarm_pool_add_repeating_timer_ms (core1_alarm, TIMER_MSEC, call_back_core1, NULL, &timer);
    
    // core1にて割り込みを実行
    // core1で割り込み設定すれば、登録されたcall back関数もcore1で実行されるようです。
    bool result = add_repeating_timer_us(TIMER_USEC, call_back_core1, NULL, &timer);
    if (!result) {
        while(true);  //停止
    }


    while(true);

    // while (true)
    // {
    //     /* code */
        
    //     // セマフォから許可を要求。許可が得られるまで待機
    //     sem_acquire_blocking(&sem);

    //     // 共用メモリへのアクセス
    //     // すぐに抜けないとデッドロックする。

    //     // セマフォの許可を解除
    //     sem_release(&sem);


    //     // 例: LEDを点滅させる
    //     gpio_put(LED_PIN, 1);
    //     sleep_ms(500);
    //     gpio_put(LED_PIN, 0);
    //     sleep_ms(500);

    // }
    
}

/// @brief core1内タイマーのコールバック関数
/// @return 割り込みを継続する場合はtrue
bool call_back_core1(){
    // 6kHzことに呼び出されるため、重い処理やwaitなどは禁止する
    // 音声データを読み出して、PWM出力設定したら、すぐにreturnすること。



    // オンボードLEDピン番号設定
    const uint8_t LED_PIN = PICO_DEFAULT_LED_PIN;

    struct sound_sts lineSoundsts;

    uint8_t audio_duty;

    //     /* 割り込み毎にトグルする */
    // if ( gpio_get( LED_PIN ) != 0 )
    // {
    //     gpio_put( LED_PIN, 0 );
    // }
    // else
    // {
    //     gpio_put( LED_PIN, 1 );
    // }

    // セマフォから許可を要求。許可が得られるまで待機
    sem_acquire_blocking(&sem);

    // 共用メモリへのアクセス
    // すぐに抜けないとデッドロックする。

    lineSoundsts = line1Soundsts;

    // セマフォの許可を解除
    sem_release(&sem);

    // サウンドデータ配列からデータを取得し、音声レベルを設定する。
    if (*lineSoundsts.sounddata == 0xff){       // 最終データの時
        lineSoundsts.isStopedSound = true;      // 音声停止ステータス
        audio_duty = RESOLUTION / 2;            // 音声を中央値で出力

        // mute制御
        // ここの処理が必要かどうか・・・・要するに音声が鳴り終わってから何秒後にミュートにするか・・・
        // 無音時間の制御にはよいかもしれないね。
        if (muteTimer <= 6000*5){
            muteTimer ++;
            if (muteTimer >= 6000*5){
                gpio_put(PIN_AUDIOMUTE,1);      // ミュート出力
            }
        }
        //gpio_put(PIN_AUDIOMUTE,1);              // ミュート出力
        // debug
        //gpio_put( LED_PIN, 1 );
        // ここまで
    } else {
        audio_duty = *lineSoundsts.sounddata;   // 音声データを変数に格納
        lineSoundsts.sounddata++;               // 配列要素を一つ進める
        lineSoundsts.isStopedSound = false;     // 音声停止ステータス
        gpio_put(PIN_AUDIOMUTE,0);              // ミュート解除
        muteTimer = 0;

        //debug
        //gpio_put( LED_PIN, 0 );
        // ここまで      
    }
    /* 500us待機 */
    //busy_wait_us_32( 500 );

    // pwm出力を実行
    // if (lineSoundsts.isStopedSound == true){
    //     audio_duty = RESOLUTION / 2;    // 出力値固定
    // } 
    pwm_set_chan_level(slice_num, PWM_CHAN_A, audio_duty);

    // セマフォから許可を要求。許可が得られるまで待機
    sem_acquire_blocking(&sem);

    // 共用メモリへのアクセス
    // すぐに抜けないとデッドロックする。

    line1Soundsts = lineSoundsts;

    // セマフォの許可を解除
    sem_release(&sem);

    // タイマー割り込みを継続
    return(true);

}
