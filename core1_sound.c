///////////////////////////////////////////////
// マルチコア処理　core1
// core1_sound
///////////////////////////////////////////////

// include
//#include "pico/stdlib.h"
//#include "pico/multicore.h"
#include "hardware/pwm.h"

#include "core1_sound.h"
#include "station_define.h"
#include "sounddata.h"
#include "st7789.h"


// グローバル変数


uint32_t muteTimer = 0;     // ミュート制御用のタイマー

// 音声データ指定用構造体
struct sound_sts{
    const uint8_t  *sounddata;     // 8bitサウンドデータ指定用ポインタ
    bool  isStopedSound;           // 音声が停止しているとき、trueとなる
};

// 音声鳴動ステータスの定義
struct sound_sts lineSoundsts[2];  // 0:1番線　1:2番線

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

    // 音声停止中初期値設定
    lineSoundsts[0].isStopedSound = true;  // 音声停止ステータス
    lineSoundsts[0].sounddata = SILENT_SOUNDDATA;  // 1番線用音声データ 無音
    lineSoundsts[1].isStopedSound = true;  // 音声停止ステータス
    lineSoundsts[1].sounddata = SILENT_SOUNDDATA;  // 2番線用音声データ 無音


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

        locateLcdPrintf(0,0);
        setColorLcdPrintf(LCD_RED,LCD_BLK);
        printfSt7789("c1IRQ NG");

        while(true);  //停止
    }

    // デバッグ
    locateLcdPrintf(0,0);
    setColorLcdPrintf(LCD_WHT,LCD_BLK);
    printfSt7789("c1IRQ OK");
    // ここまで

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
    // 6kHzごとに呼び出されるため、重い処理やwaitなどは禁止する
    // 音声データを読み出して、PWM出力設定したら、すぐにreturnすること。

    // ★前回の状態を記憶しておくため、static（静的ローカル変数）に変更します。
    // 初期値として、起動直後は安全のため無音・停止状態にしておきます。
    static struct sound_sts soundsts[2] = {
        { SILENT_SOUNDDATA, true },
        { SILENT_SOUNDDATA, true }
    };

    // PWMへ渡す音声レベル
    uint8_t audio_duty = 0;

    // ★セマフォの取得を試みる（ブロックしない）
    if (sem_try_acquire(&sem)) {
        // セマフォが取れた場合のみ、最新の共有メモリからデータをコピー
        soundsts[0] = lineSoundsts[0];
        soundsts[1] = lineSoundsts[1];
        
        // コピーしたら「すぐに」セマフォを解放する
        sem_release(&sem);
    } else {
        // ★Busyだった場合はここに入ります。
        // 何もしないことで、前回この関数が終わった時点の soundsts[2] の中身（ポインタ位置など）が
        // そのまま維持され、今回の処理に使い回されます。
    }

    // サウンドデータ配列からデータを取得し、音声レベルを設定する。
    // 2車線分取得し、データを半分にして足し算する。

    // 2車線分の生データを一時的に足し合わせるための変数（int型など大きめの型にする）
    uint16_t mixed_raw = 0;

    for(uint8_t i=0; i<2; i++){
        if (*soundsts[i].sounddata == 0xff){       // 最終データの時
            soundsts[i].isStopedSound = true;      // 音声停止ステータス
            mixed_raw += (RESOLUTION / 2);         // 無音時は、波形の中央値（128）
        } else {
            soundsts[i].isStopedSound = false;     // 再生中ステータス
            mixed_raw += *soundsts[i].sounddata;   // 再生中は、生データをそのまま（割らずに）足す
            soundsts[i].sounddata++;               // 配列要素を一つ進める
        }
    }

    // ★最後にまとめて2で割る（これで小数点以下の切り捨てによるノイズを防ぐ）
    audio_duty = (uint8_t)(mixed_raw / 2);

    // for(uint8_t i=0; i<2; i++){
    //     if (*soundsts[i].sounddata == 0xff){       // 最終データの時
    //         soundsts[i].isStopedSound = true;      // 音声停止ステータス
    //         audio_duty += RESOLUTION / 4;          // 音声を中央値で出力(中央値のさらに半分)
    //     } else {
    //         audio_duty += *soundsts[i].sounddata / 2;  // 音声データを変数にインクリメント
    //         soundsts[i].sounddata++;               // 配列要素を一つ進める
    //         soundsts[i].isStopedSound = false;     // 音声停止ステータス
    //     }
    // }

    // ★再生位置を進めた結果（sounddataのポインタなど）をグローバル側に書き戻す
    if (sem_try_acquire(&sem)) {
        // セマフォが取れた場合のみ、最新の状態を書き戻す
        lineSoundsts[0] = soundsts[0];
        lineSoundsts[1] = soundsts[1];
        
        sem_release(&sem);
    } else {
        // ★書き戻し時にBusyだった場合：
        // 今回のポインタのインクリメント結果は lineSoundsts には即座に反映されませんが、
        // 次回の割り込み時に「Core0がセマフォを離していれば」最初の if (sem_try_acquire) の中で
        // 前回の残像ではなく、Core0側のデータと安全に同期されるため問題ありません。
    }

    // pwm出力を実行
    pwm_set_chan_level(slice_num, PWM_CHAN_A, audio_duty);

    // ミュート処理
    if (soundsts[0].isStopedSound == true && soundsts[1].isStopedSound == true){
        muteSound();            // ミュート
    } else {
        unMuteSound();          // ミュート解除
    }

    // タイマー割り込みを継続
    return(true);
}

/// @brief 電車の接近アナウンスの放送を開始します。
/// @param line_no 番線番号　0 ～　1
void announceTrainApproach(uint8_t line_no){

    // セマフォから許可を要求。許可が得られるまで待機
    sem_acquire_blocking(&sem);
    // sem_try_acquire(&sem);

    if (line_no == 0){                  // 1番線処理
        lineSoundsts[0].sounddata = APPROACH_SOUNDDATA1;
        lineSoundsts[0].isStopedSound = false;
    } else {                            // 2番線処理
        lineSoundsts[1].sounddata = APPROACH_SOUNDDATA2;
        lineSoundsts[1].isStopedSound = false;       
    }

    // セマフォの許可を解除
    sem_release(&sem);
}

/// @brief 電車の発車ベルの放送を開始します。
/// @param line_no 番線番号　0 ～　1
void playDepartureMelody(uint8_t line_no){

    // セマフォから許可を要求。許可が得られるまで待機
    sem_acquire_blocking(&sem);
    // sem_try_acquire(&sem);

    if (line_no == 0){                  // 1番線処理
        lineSoundsts[0].sounddata = BELLDATA2;
        lineSoundsts[0].isStopedSound = false;
    } else {                            // 2番線処理
        lineSoundsts[1].sounddata = BELLDATA1;
        lineSoundsts[1].isStopedSound = false;       
    }

    // セマフォの許可を解除
    sem_release(&sem);

}

/// @brief ドア閉アナウンスの放送を開始します。
/// @param line_no 番線番号　0 ～　1
void announceDoorCloseing(uint8_t line_no){

    // セマフォから許可を要求。許可が得られるまで待機
    sem_acquire_blocking(&sem);
    // sem_try_acquire(&sem);

    if (line_no == 0){                  // 1番線処理
        lineSoundsts[0].sounddata = DOOR_CLOSE_SOUNDDATA1;
        lineSoundsts[0].isStopedSound = false;
    } else {                            // 2番線処理
        lineSoundsts[1].sounddata = DOOR_CLOSE_SOUNDDATA2;
        lineSoundsts[1].isStopedSound = false;       
    }

    // セマフォの許可を解除
    sem_release(&sem);
}

/// @brief ジングルサウンド鳴動
void playJingleSound(){
    // セマフォから許可を要求。許可が得られるまで待機
    sem_acquire_blocking(&sem);
    // sem_try_acquire(&sem);

    // ジングルサウンド鳴動開始
    lineSoundsts[0].sounddata = JINGLE_SOUND_DATA;
    lineSoundsts[0].isStopedSound = false;

    // セマフォの許可を解除
    sem_release(&sem);
}


/// @brief 放送が停止しているかどうかを確認する
/// @param line_no 番線番号　0 ～　1
/// @return 放送が停止していればtrue;
bool isSoundStop(uint8_t line_no){

    bool isStopedSound;         // 停止しているかどうか

    // セマフォから許可を要求。許可が得られるまで待機
    sem_acquire_blocking(&sem);
    // sem_try_acquire(&sem);

    // サウンドが停止しているかどうかを確認
    if (line_no < 2) {
        isStopedSound = lineSoundsts[line_no].isStopedSound;       
    } else {
        isStopedSound = true;           // 2以上はプログラム異常
    }

    // セマフォの許可を解除
    sem_release(&sem);

    return (isStopedSound);
}

/// @brief AUDIO　ICへミュート出力を行う
void muteSound(){
    gpio_put(PIN_AUDIOMUTE,1);      // ミュート出力
    // デバッグ
    // locateLcdPrintf(0,1);
    // setColorLcdPrintf(LCD_WHT,LCD_BLK);
    // printfSt7789("MUTE   ");

}

/// @brief AUDIO ICのミュートを解除する
void unMuteSound(){
    gpio_put(PIN_AUDIOMUTE,0);      // ミュート解除

    // デバッグ
    // locateLcdPrintf(0,1);
    // setColorLcdPrintf(LCD_YEL,LCD_BLK);
    // printfSt7789("PLAYING");
}


/// @brief 音声を停止する
/// @param line_no 
void stopSound(uint8_t line_no){
    // セマフォから許可を要求。許可が得られるまで待機
    sem_acquire_blocking(&sem);
    // sem_try_acquire(&sem);

    if (line_no == 0){                  // 1番線処理
        lineSoundsts[0].sounddata = SILENT_SOUNDDATA;
        lineSoundsts[0].isStopedSound = true;
    } else {                            // 2番線処理
        lineSoundsts[1].sounddata = SILENT_SOUNDDATA;
        lineSoundsts[1].isStopedSound = true;       
    }

    // セマフォの許可を解除
    sem_release(&sem);
}
            