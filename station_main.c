///////////////////////////////////////////////
// メイン関数
// station_main
///////////////////////////////////////////////

// include
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdio.h>

#include "station_define.h"
#include "st7789.h"
#include "timer_ctrl.h"
#include "station_disp.h"
#include "signboard_bmp240.h"
#include "station_signboard.h"
#include "trainTimetable.h"
#include "stname.h"

//プロトタイプ宣言

/// @brief 車線ステータス各種タイマー制御関数
/// 車線ステータスを制御する各種タイマーをインクリメントする。
/// 到着までの残り分数を計算する。
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列（値渡し
void timer_ctrl(struct line_st *st, const struct timetable tb[]);

/// @brief ステートマシン（フェーズ）制御関数：
/// 列車案内、スクロール文字表示、地図表示、列車接近案内など各種状態を制御する
/// @param st 車線ステータス構造体（参照渡し）
/// @param tb 時刻表構造体配列（値渡し）
void setDisplayMode(struct line_st *st, const struct timetable tb[]);

/// @brief ステートマシン(ステート）制御関数：
/// 日本語表示、英語表示を切り替えるステートマシン
/// @param st 車線ステータス構造体（参照渡し）
void switchLanguage(struct line_st *st);

/// @brief ステートマシンに基づく表示実行関数
/// @param st 車線ステータス構造体（参照渡し）
/// @param tb 時刻表構造体配列（値渡し）
void drawLcdDisplay(struct line_st *st, const struct timetable tb[]);

/// @brief スクロール文字制御関数：
/// スクロール文字を表示する。
/// @param st 車線ステータス構造体（参照渡し）
void drawScrollText(struct line_st *st);


// グローバル変数
uint8_t		isBlinkOn = 1;		// 文字列点滅用
bool        isChangeByTime;		// 次の電車に移行する手段を時間にするか、ボタンにするかのフラグ

///////////////////////////////////////////////
// メイン関数
///////////////////////////////////////////////
int main() {

    ///////////////////////////////////////////
    // 初期設定
    ///////////////////////////////////////////
    // SPI初期化
    ///////////////////////////////////////////
    //ピンの接続:
    //SPI SCK (Clock) を GP2 (ピン4)
    //SPI TX (MOSI) を GP3 (ピン5)
    //SPI CS をGP5(ピン7)->採用しているディスプレイにはCSピンがないので、未接続
    //ディスプレイのRST ピンを GP12 (ピン16)
    //ディスプレイのDC ピンを GP8 (ピン11)

    // 通信速度設定
    //spi_init(SPI_PORT, 1000 * 1000); // 1 MHz
    spi_init(SPI_PORT, 1000 * 1000 * 20); // 20 MHz
    // ピン設定
    gpio_set_function(PIN_SPI_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SPI_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS, GPIO_FUNC_SPI);

    spi_set_format(
        SPI_PORT, // SPIポート
	    8,        // データビット数
        SPI_CPOL_1, // クロック極性（CPOL）
        SPI_CPHA_1, // クロック位相（CPHA）
        SPI_MSB_FIRST// MSBファースト
	); 
    ///////////////////////////////////////////
    // GPIO設定
    ///////////////////////////////////////////
    // オンボードLEDピン番号設定
    const uint8_t LED_PIN = PICO_DEFAULT_LED_PIN;
    // 各種ピン設定
    gpio_init(PIN_RST);             // SPI リセットピン初期化
    gpio_init(PIN_DC);              // DCピン初期化
    gpio_init(LED_PIN);
    gpio_init(PIN_APPBTN1);         // 1番線用電車接近ボタン
    gpio_init(PIN_APPBTN2);         // 2番線用電車接近ボタン

    //入出力方向設定
    gpio_set_dir(PIN_RST, GPIO_OUT);    // RSTピン出力
    gpio_set_dir(PIN_DC, GPIO_OUT);     // DCピン出力
    gpio_set_dir(LED_PIN, GPIO_OUT);    // オンボードLED出力
    gpio_set_dir(PIN_APPBTN1, GPIO_IN); // ボタン入力
    gpio_set_dir(PIN_APPBTN2, GPIO_IN); // ボタン入力
    //プルアップ設定
    gpio_pull_up(PIN_APPBTN1);      // ボタン入力ピンをpull up設定
    gpio_pull_up(PIN_APPBTN2);      // ボタン入力ピンをpull up設定

    // スタンダードIO初期設定
    stdio_init_all();

    ///////////////////////////////////////////
    // ST7789ディスプレイ初期設定
    ///////////////////////////////////////////
    // Initialize the display
    initializeSt7789();

    // 画面回転設定
    // 設定角度 0:0度 1:90度 2:180度 3:270度
    setRotationSt7789(2) ;

    // Draw a red rectangle in the center
    //fillRectangleSt7789(60, 60, 120, 120, LCD_RED); // Red

    // Example: Draw pixels of different colors
    //draw1PixelSt7789(120, 120, 0x07E0); // Green

    // 時刻初期化
    initializeClock();

    ///////////////////////////////////////////
    // 初期画面表示
    ///////////////////////////////////////////  
    // 画面を黒で塗りつぶす
    fillScreenSt7789(LCD_BLK); // BLACK

    // 初期文字列表示
    locateLcdPrintf(0,0);
    setColorLcdPrintf(LCD_GRN,LCD_BLK);
    printfSt7789("===============");

    locateLcdPrintf(0,1);
    setColorLcdPrintf(LCD_YEL,LCD_GRY);
    printfSt7789("Train Info     ");
    locateLcdPrintf(0,2);
    printfSt7789(" Display system");

    locateLcdPrintf(0,3);
    setColorLcdPrintf(LCD_GRN,LCD_BLK);
    printfSt7789("===============");


    locateLcdPrintf(0,5);
    setColorLcdPrintf(LCD_CYN,LCD_BLK);
    printfSt7789("ver 1.00a"); 

    locateLcdPrintf(0,10);
    setColorLcdPrintf(LCD_WHT,LCD_BLK);
    printfSt7789("@2024  yamapy"); 

    locateLcdPrintf(0,14);
    setColorLcdPrintf(LCD_WHT,LCD_BLK);
    printfSt7789("0123456789ABCDE"); 

    // ATOSモード切替
    // ボタンを押下していなかったら、時間切り替えモードとする
    if(gpio_get(PIN_APPBTN1)==0 || gpio_get(PIN_APPBTN2)==0)
        isChangeByTime = false;
    else
        isChangeByTime = true;
	
    locateLcdPrintf(0,6);
    if (isChangeByTime){
        setColorLcdPrintf(LCD_MAG,LCD_BLK);
        printfSt7789("Time chg mode"); 
    }else{
        setColorLcdPrintf(LCD_RED,LCD_BLK);
		printfSt7789("Button chg mode"); 		
    }

    // 初期画面表示時間中、LEDを点滅させます
    uint16_t i;
    for (i=0; i<10; i++){
        sleep_ms(200);
        gpio_put(LED_PIN,1);
        sleep_ms(200);
        gpio_put(LED_PIN,0);
    }

    // 画面を黒で塗りつぶす
    fillScreenSt7789(LCD_BLK); // BLACK

    ///////////////////////////////////////////
    // 変数定義
    ///////////////////////////////////////////
    // 現在時刻構造体
    struct time_struct time_disp;
    struct time_hms end_time;

    // 現在時刻取得（初期値リード)
    time_disp = getClock();

    // 終了時刻設定
    end_time.hh = END_HH;
    end_time.mm = END_MM;
    end_time.ss = END_SS;

    // 1番線2番線構造体をそれぞれ設定
    struct line_st line1;
    struct line_st line2;
 
    // タスク管理用
    station_prosess station_prosess = DO_DEBUG;
    
    // 構造体値初期化
    line1.posy = LINE1POSY;                     // 表示高さ設定
    line1.current_state = NEXT_J_ST;            // ステートマシン初期値
    line1.current_phase = NEXT_A_P;             // ステートフェーズ初期値
    line1.updatestate = true;                   // ステートマシン更新フラグ
    line1.info_no = 0;                          // スクロール文字種別番号初期値
    line1.start_flg = false;                    // スクロール文字開始フラグ
    line1.end_flg = true;                       // スクロール文字終了フラグ
    line1.scrl_pos = 0;                         // スクロール文字ポジション
    line1.button_gpio = PIN_APPBTN1;            // 電車接近のボタンのGIO番号
    line1.phase_timer = 0;                      // フェーズ切り替え用タイマー
    line1.language_timer = 0;                   // 言語切り替え用タイマー

    line1.dec_min_timer = 0;                    // 残り分数減算タイマー
    line1.timetableNo = 0;                      // 次の電車の時刻表は前から何番目か

    if(isChangeByTime){
        // 時刻表から最初の分数の値を計算して初期値とする
        line1.nexttrainmin = getTimeDifference(line1TB[line1.timetableNo].departure_time, time_disp.timehms);          // 次の電車までの分数初期値
        line1.nextnexttrainmin = getTimeDifference(line1TB[line1.timetableNo+1].departure_time, time_disp.timehms);  // 次の次の電車までの分数　初期値
    }else{
        line1.nexttrainmin = NEXTTRAININI;          // 次の電車までの分数初期値
        line1.nextnexttrainmin = NEXTNEXTTRAININI;  // 次の次の電車までの分数　初期値
    }

    line2.posy = LINE2POSY;                     // 表示高さ設定
    line2.current_state = NEXT_J_ST;            // ステートマシン初期値
    line2.current_phase = NEXT_A_P;             // ステートフェーズ初期値
    line2.updatestate = true;                   // ステートマシン更新フラグ
    line2.info_no = 0;                          // スクロール文字種別番号初期値
    line2.start_flg = false;                    // スクロール文字開始フラグ
    line2.end_flg = true;                       // スクロール文字終了フラグ
    line2.scrl_pos = 0;                         // スクロール文字ポジション
    line2.button_gpio = PIN_APPBTN2;            // 電車接近のボタンのGIO番号
    line2.phase_timer = 20*1;                   // フェーズ切り替え用タイマー（初期値を1番線のタイマーから1秒ずらす）
    line2.language_timer = 20*1;                // 言語切り替え用タイマー

    line2.dec_min_timer = 20*1;                 // 残り分数減算タイマー
    line2.timetableNo = 0;                      // 次の電車の時刻表は前から何番目か

    if(isChangeByTime){
        // 時刻表から最初の分数の値を計算して初期値とする
        line2.nexttrainmin = getTimeDifference(line2TB[line2.timetableNo].departure_time, time_disp.timehms);          // 次の電車までの分数初期値
        line2.nextnexttrainmin = getTimeDifference(line2TB[line2.timetableNo+1].departure_time, time_disp.timehms);  // 次の次の電車までの分数　初期値
    }else{
        line2.nexttrainmin = NEXTTRAININI;          // 次の電車までの分数初期値
        line2.nextnexttrainmin = NEXTNEXTTRAININI;  // 次の次の電車までの分数　初期値
    }

    ///////////////////////////////////////////
    // タイマー割り込み
    ///////////////////////////////////////////
    // 繰り返しタイマーを設定
    struct repeating_timer timer;
    // 10ミリ秒ごとに割り込みを実行
    bool result = add_repeating_timer_ms(10, repeating_timer_callback, NULL, &timer);
    if (!result) {
        while(1);  //停止
    }
    
    // タイマーを0にリセット
    clear100msecTimer();

    
    ///////////////////////////////////////////
    // 画面初期表示
    ///////////////////////////////////////////
    // 看板表示(車線1)
	drawBMP_B(&BMP_PLT_UP[0], &BMP_DAT_UP[0], 0 ,line1.posy, SIGNBOARDXSIZE , SIGNBOARDYSIZE);
    // 看板表示(車線2)
	drawBMP_B(&BMP_PLT_DOWN[0], &BMP_DAT_DOWN[0], 0 ,line2.posy, SIGNBOARDXSIZE , SIGNBOARDYSIZE);

    ///////////////////////////////////////////
    // メインループ
    ///////////////////////////////////////////
    while (true) {

        //スクロール文字点滅フラグ設定
        isBlinkOn = brinktimer();

        // *****タスク振り分け*****
		// 説明
		// タイマー割り込み関数でフラグが立ったら(10msec毎に発生する)、
		// 実行するべきタスクを指定する
        if (chk10msec()){
            switch(station_prosess){
				// ***** 車線1のステートを制御するタスク
                case    DO_STATE1:
                    // タイマーの設定
                    timer_ctrl(&line1, line1TB);
                    // ステートマシン(フェーズ)
                    setDisplayMode(&line1, line1TB);
                    // ステートマシン（ステート）
                    switchLanguage(&line1);
                    // ステート実行
                    drawLcdDisplay(&line1, line1TB);

                    station_prosess = DO_SCRL1;

                    break;
				
				// ***** 車線1のスクロール文字列を表示するタスク
                case    DO_SCRL1:
                    // インフォメーションスクロール
                    drawScrollText(&line1);

                    station_prosess = DO_STATE2;
                    break;
                    
				// ***** 車線2のステートを制御するタスク
                case    DO_STATE2:
                     // タイマーの設定
                    timer_ctrl(&line2, line2TB);
                    // ステートマシン(フェーズ)
                    setDisplayMode(&line2, line2TB);
                    // ステートマシン（ステート）
                    switchLanguage(&line2);
                    // ステート実行
                    drawLcdDisplay(&line2,line2TB);

                    station_prosess = DO_SCRL2;

                    break;
				
				// ***** 車線2のスクロール文字列を表示するタスク
                case    DO_SCRL2:
                    // インフォメーションスクロール
                    drawScrollText(&line2);

                    station_prosess = DO_DEBUG;
                    break;

				// ***** その他(デバッグ用表示)タスク
                case    DO_DEBUG:
                    // 時刻がすぎたら、最初に戻る
                    if(isBefore(end_time, time_disp.timehms)){
                        // 初期化実行
                        initializeClock();               // 時刻初期値
                        line1.timetableNo = 0;      // 時刻表位置初期化
                        line2.timetableNo = 0;      // 時刻表位置初期化

                        // 残り分数初期化
                        if(isChangeByTime){
                            // 時刻表から最初の分数の値を計算して初期値とする
                            line1.nexttrainmin = getTimeDifference(line1TB[line1.timetableNo].departure_time, time_disp.timehms);          // 次の電車までの分数初期値
                            line1.nextnexttrainmin = getTimeDifference(line1TB[line1.timetableNo+1].departure_time, time_disp.timehms);  // 次の次の電車までの分数　初期値
                        }else{
                            line1.nexttrainmin = NEXTTRAININI;          // 次の電車までの分数初期値
                            line1.nextnexttrainmin = NEXTNEXTTRAININI;  // 次の次の電車までの分数　初期値
                        }

                        if(isChangeByTime){
                            // 時刻表から最初の分数の値を計算して初期値とする
                            line2.nexttrainmin = getTimeDifference(line2TB[line2.timetableNo].departure_time, time_disp.timehms);          // 次の電車までの分数初期値
                            line2.nextnexttrainmin = getTimeDifference(line2TB[line2.timetableNo+1].departure_time, time_disp.timehms);  // 次の次の電車までの分数　初期値
                        }else{
                            line2.nexttrainmin = NEXTTRAININI;          // 次の電車までの分数初期値
                            line2.nextnexttrainmin = NEXTNEXTTRAININI;  // 次の次の電車までの分数　初期値
                        }
                    }
				
					// デバッグ：車線1,2のステータス表示
                    // locateLcdPrintf(0,6);
                    // setColorLcdPrintf(LCD_WHT,LCD_BLK);
                    // printfSt7789("%2d:%2d/%2d:%2d",line1.nexttrainmin, line1.nextnexttrainmin ,line2.nexttrainmin, line2.nextnexttrainmin);

                    // デバッグ：現在時刻表示
                    time_disp = getClock();
                    locateLcdPrintf(0,14);
                    setColorLcdPrintf(LCD_YEL, LCD_BLK);    
                    printfSt7789("[%02d:%02d:%02d] [%02d]", time_disp.timehms.hh, time_disp.timehms.mm, time_disp.timehms.ss, time_disp.time_msec);

                    //デバッグ：ボタンを押下したらLED点灯
                    //green LED
                    if(gpio_get(PIN_APPBTN1)==0 || gpio_get(PIN_APPBTN2)==0){
                        gpio_put(LED_PIN,1);
                    }else{
                        gpio_put(LED_PIN,0);
                    }
                    // デバッグここまで

                    station_prosess = DO_STATE1;
                    break;
                default :
                    station_prosess = DO_STATE1;                   
                    break;
            }//switch
        } //if(timer10msec)   
     }//while(true)
}//main


/// @brief 車線ステータス各種タイマー制御関数
/// 車線ステータスを制御する各種タイマーをインクリメントする。
/// 到着までの残り分数を計算する。
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列（値渡し）
void timer_ctrl(struct line_st *st, const struct timetable tb[]){

    // タイマー設定
    // 50msecごとに呼び出されると想定
    st->phase_timer++;
    st->language_timer++;
    st->dec_min_timer++;

    struct time_struct now_time;
    now_time = getClock();

    // フラグが立ったら残り分数を減算する。
    if(isChangeByTime){
        int16_t min;
        // 時間で切り替わるモード：時刻表と現在時刻から計算する。
        // 次の電車までの分数計算
        min = getTimeDifference(tb[st->timetableNo].departure_time, now_time.timehms);
        if (min > 0)
            st->nexttrainmin = min;         // 次の電車までの分数初期値 １分以下にはならないように制御
        else
            st->nexttrainmin = 1;

        // 次の次の電車までの分数計算 
        min = getTimeDifference(tb[st->timetableNo+1].departure_time, now_time.timehms);
        if (min > 0)
            st->nextnexttrainmin = min;     // 次の次の電車までの分数　初期値
        // ３分以下にはならないように制御
        if (st->nextnexttrainmin < 3)
            st->nextnexttrainmin = 3;
    }else{
        // ボタンで切り替わるモード：設定時間が過ぎたら減算する。
        if(st->dec_min_timer > DECTIME){
            // タイマークリア
            st->dec_min_timer = 0;

            // 分数を減算
            if(st->nexttrainmin > 1)            // 1分以下にはならないように
                st->nexttrainmin --;
            if(st->nextnexttrainmin > 3)        // 3分以下にはならないように
                st->nextnexttrainmin --;
        }
    }
    
}//timer_ctrl


/// @brief ステートマシン（フェーズ）制御関数：
/// 列車案内、スクロール文字表示、地図表示、列車接近案内など各種状態を制御する
/// @param st 車線ステータス構造体（参照渡し）
/// @param tb 時刻表構造体配列（値渡し）
void setDisplayMode(struct line_st *st, const struct timetable tb[]){
	
	// 次の列車に移行するべきかどうかのフラグ
	bool nextTrainFlg;

	// 現時刻を取得し、構造体に格納する
	struct time_struct now_time;

	
	now_time = getClock();
		
		
	if (isChangeByTime){
		// 時間切り替えモード
		// 時刻表が現在時刻より過去だったら、移行フラグ設定
		if(isBefore(tb[st->timetableNo].departure_time, now_time.timehms)){
			nextTrainFlg = true;
		} else {
			nextTrainFlg = false;
		}
		
	}else{
		// ボタン切り替えモード
		// GPIO状態取得
		if (gpio_get(st->button_gpio) == 0){          // GPIOはボタン押下時にlow
			nextTrainFlg = true;
		} else {
			nextTrainFlg = false;
		}	
	
	}
	

    // ステートマシン記述（フェーズ）
    switch(st->current_phase){
        case    NEXT_A_P:       // 通常フェーズA
            if (nextTrainFlg){
                st->current_phase = APPROACH_P;     // インフォメーションフェーズへ移行
                st->current_state = APPROACH_J_ST;  // 日本語表示
                st->end_flg = true;             // スクロール終了エンドフラグをセット
                st->start_flg = false;           // スクロール開始フラグセット（スクロールスタート）
                st->phase_timer = 0;            // フェーズ移行タイマークリア
                st->language_timer = 0;                 // 英語日本語切り替えタイマークリア
                st->updatestate = true;         // ステートマシン更新フラグ                   
            }else if (st->phase_timer > PHASETIME_A){
                st->current_phase = INFO_P;     // インフォメーションフェーズへ移行
                st->current_state = INFO1_J_ST; // 
                st->end_flg = false;            // スクロール終了エンドフラグをクリア
                st->start_flg = true;           // スクロール開始フラグセット（スクロールスタート）
                st->phase_timer = 0;             // フェーズ移行タイマークリア
                st->language_timer = 0;                // 英語日本語切り替えタイマークリア
                st->updatestate = true;         // ステートマシン更新フラグ
            }
            break;
        case    INFO_P:         // インフォメーションフェーズ
            if (nextTrainFlg){
                st->current_phase = APPROACH_P;     // インフォメーションフェーズへ移行
                st->current_state = APPROACH_J_ST;  // 日本語表示
                st->end_flg = true;             // スクロール終了エンドフラグをセット
                st->start_flg = false;          // スクロール開始フラグセット（スクロールスタート）
                st->phase_timer = 0;             // フェーズ移行タイマークリア
                st->language_timer = 0;                // 英語日本語切り替えタイマークリア
                st->updatestate = true;         // ステートマシン更新フラグ                   
            }else if(st->end_flg){                    // インフォメーション表示終了したら
                st->current_phase = NEXT_B_P;   // インフォメーションフェーズへ移行
                st->current_state = NEXT_J_ST;  // 日本語表示
                st->phase_timer = 0;             // フェーズ移行タイマークリア
                st->language_timer = 0;                // 英語日本語切り替えタイマークリア 
                st->updatestate = true;         // ステートマシン更新フラグ        
            }
            break;
        case    NEXT_B_P:       // 通常フェーズB
            if (nextTrainFlg){
                st->current_phase = APPROACH_P;     // インフォメーションフェーズへ移行
                st->current_state = APPROACH_J_ST;  // 日本語表示
                st->end_flg = true;             // スクロール終了エンドフラグをセット
                st->start_flg = false;           // スクロール開始フラグセット（スクロールスタート）
                st->phase_timer = 0;           // フェーズ移行タイマークリア
                st->language_timer = 0;              // 英語日本語切り替えタイマークリア
                st->updatestate = true;         // ステートマシン更新フラグ                   
            }else if (st->phase_timer > PHASETIME_A){
                st->current_phase = MAP_P;      // マップ表示フェーズへ移行
                st->current_state = MAP_J_ST;   // 地図日本語表示
                st->phase_timer = 0;            // フェーズ移行タイマークリア
                st->language_timer = 0;                 // 英語日本語切り替えタイマークリア
                st->updatestate = true;         // ステートマシン更新フラグ
            }
            break;
        case    MAP_P:          // 地図表示フェーズ
            if (nextTrainFlg){
                st->current_phase = APPROACH_P;     // インフォメーションフェーズへ移行
                st->current_state = APPROACH_J_ST;  // 日本語表示
                st->end_flg = true;             // スクロール終了エンドフラグをセット
                st->start_flg = false;           // スクロール開始フラグセット（スクロールスタート）
                st->phase_timer = 0;           // フェーズ移行タイマークリア
                st->language_timer = 0;                 // 英語日本語切り替えタイマークリア
                st->updatestate = true;         // ステートマシン更新フラグ                   
            }else if (st->phase_timer > PHASETIME_A){
                st->current_phase = NEXT_A_P;   // マップ表示フェーズへ移行
                st->current_state = NEXT_J_ST;  // 日本語表示
                st->phase_timer = 0;            // フェーズ移行タイマークリア
                st->language_timer = 0;                 // 英語日本語切り替えタイマークリア
                st->updatestate = true;         // ステートマシン更新フラグ
            }
            break;
            case   APPROACH_P:     // 電車接近フェーズ
            if (st->phase_timer > PHASETIME_B){
                st->current_phase = NEXT_ST_P;   // 電車発車フェースへ移行
                st->current_state = NEXT_STATION_ST; 
                st->phase_timer = 0;            // フェーズ移行タイマークリア
                st->language_timer = 0;                 // 英語日本語切り替えタイマークリア
                st->updatestate = true;         // ステートマシン更新フラグ
            }

            break;
        case    NEXT_ST_P:      // 電車発車フェーズ
            if (st->phase_timer > PHASETIME_C){
                // 電車出発　次の電車までの到着分数を再設定
                st->nexttrainmin = st->nextnexttrainmin;        // 次の電車の値をコピーする
                if (st->nexttrainmin > 5)                       // 6だと次の電車にくっついてしまうので、5分未満とする
                    st->nexttrainmin = 5;
				
                st->timetableNo++;                              // 次の時刻に移行する

				// 時刻分数差分を取得
				int16_t minutes;
				// 次の時刻表までの差分を計算する
				minutes = getTimeDifference(tb[st->timetableNo+1].departure_time, now_time.timehms);
				
				// 時間切り替えモードならちゃんと計算、ボタンモードなら固定値
				if (isChangeByTime){
					// 時間切り替えモード
					if(minutes > st->nexttrainmin)
						st->nextnexttrainmin = minutes;		// 次の電車より時間が長いなら、そのまま設定
					else
						st->nextnexttrainmin = NEXTNEXTTRAININI;
				} else {
					// ボタン切り替えモード
					st->nextnexttrainmin = NEXTNEXTTRAININI;    // 次の次の電車は7分					
				}

                st->current_phase = NEXT_A_P;                   // マップ表示フェーズへ移行
                st->current_state = NEXT_J_ST;                  // 日本語表示

                st->phase_timer = 0;                            // フェーズ移行タイマークリア
                st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
                st->updatestate = true;                         // ステートマシン更新フラグ
            }

            break;
        default:
            break;
    }//switch(current_phase)   
}//setDisplayMode

/// @brief ステートマシン(ステート）制御関数：
/// 日本語表示、英語表示を切り替えるステートマシン
/// @param st 車線ステータス構造体（参照渡し）
void switchLanguage(struct line_st *st){
    // ステートマシン記述
    switch(st->current_state){
        case    NEXT_J_ST:        // 通常の日本語表示
            if (st->language_timer > LANGUAGETIME){            // 5秒たったら、英語へ切り替え
                st->current_state = NEXT_E_ST;
                st->updatestate = true;         // ステートマシン更新フラグ
                st->language_timer = 0;
            }

            break;
        case    NEXT_E_ST:        // 通常の英語表示
            if (st->language_timer > LANGUAGETIME){            // 5秒たったら、日本語へ切り替え
                st->current_state = NEXT_J_ST;
                st->updatestate = true;         // ステートマシン更新フラグ
                st->language_timer = 0;
            }

            break;
        case    INFO1_J_ST:         // インフォメーション1 日本語
            if(st->language_timer > LANGUAGETIME){
                st->current_state = INFO1_E_ST;  
                st->updatestate = true;         // ステートマシン更新フラグ 
                st->language_timer = 0;
            }

            break;
        case    INFO1_E_ST:         // インフォメーション1 英語
            if(st->language_timer > LANGUAGETIME){
                st->current_state = INFO1_J_ST;  
                st->updatestate = true;         // ステートマシン更新フラグ 
                st->language_timer = 0;
            }
            break;
        case    INFO2_J_ST:         // インフォメーション2 日本語


            break;
        case    INFO2_E_ST:         // インフォメーション2 英語


            break;
        case    MAP_J_ST:          // 地図表示 日本語
            if(st->language_timer > LANGUAGETIME){
                st->current_state = MAP_E_ST;  
                st->updatestate = true;         // ステートマシン更新フラグ 
                st->language_timer = 0;
           }

            break;
        case    MAP_E_ST:          // 地図表示 英語
            if(st->language_timer > LANGUAGETIME){
                st->current_state = MAP_J_ST;  
                st->updatestate = true;         // ステートマシン更新フラグ 
                st->language_timer = 0;
            }

            break;             
        case    APPROACH_J_ST:    // 電車がまいります
            if(st->language_timer > LANGUAGETIME){
                st->current_state = APPROACH_E_ST;  
                st->updatestate = true;         // ステートマシン更新フラグ 
                st->language_timer = 0;
           }

            break;
        case    APPROACH_E_ST:    // Train approaching
            if(st->language_timer > LANGUAGETIME){
                st->current_state = APPROACH_J_ST;  
                st->updatestate = true;         // ステートマシン更新フラグ 
                st->language_timer = 0;
            }

            break;
        case    NEXT_STATION_ST:  // 日本語表示のみ　切り替えなし

            break;
        default:
        
            break;

    }// switch(current_state)
}//switchLanguage


/// @brief ステートマシンに基づく表示実行関数
/// @param st 車線ステータス構造体（参照渡し）
/// @param tb 時刻表構造体配列（値渡し）
void drawLcdDisplay(struct line_st *st, const struct timetable tb[]){
    // ステートマシン実行タスク（日本語 英語切り替え
    switch(st->current_state){
        case    NEXT_J_ST:        // 通常の日本語表示
            if (st->updatestate){
                st->updatestate = false;         // ステートマシン更新フラグ
                // 上段日本語表示
                drawTrainType(tb[st->timetableNo].train_type,true,st->posy + 24*2);
                printf("JA%2d B%2d T1=%2d T2=%2d\n",tb[st->timetableNo].destination,tb[st->timetableNo+1].destination,tb[st->timetableNo].train_type,tb[st->timetableNo+1].train_type);
                drawDestination(tb[st->timetableNo].destination,true,st->posy + 24*2);
                drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,true,st->posy +24*2);
                // 下段日本語表示
                drawTrainType(tb[st->timetableNo+1].train_type,true,st->posy + 24*3);
                drawDestination(tb[st->timetableNo+1].destination,true,st->posy + 24*3);
                drawTime(tb[st->timetableNo+1].isDrawLeftTime,tb[st->timetableNo+1].departure_time,st->nextnexttrainmin,true,st->posy +24*3);                  
            }

            break;
        case    NEXT_E_ST:        // 通常の英語表示
            if (st->updatestate){
                st->updatestate = false;         // ステートマシン更新フラグ
                // 上段英語表示
                drawTrainType(tb[st->timetableNo].train_type,false,st->posy + 24*2);      
                drawDestination(tb[st->timetableNo].destination,false,st->posy + 24*2);
                drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,false,st->posy +24*2); 

                // 下段英語表示
                //LCD_Draw_YAMANOTE(false,st->posy + 24*3, LCD_STATION_ORANGE, LCD_BLK);
                drawTrainType(tb[st->timetableNo+1].train_type,false,st->posy + 24*3);      
                drawDestination(tb[st->timetableNo+1].destination,false,st->posy + 24*3);
                drawTime(tb[st->timetableNo+1].isDrawLeftTime,tb[st->timetableNo+1].departure_time,st->nextnexttrainmin,false,st->posy +24*3);       
                printf("EA%2d B%2d T1=%2d T2=%2d\n",tb[st->timetableNo].destination,tb[st->timetableNo+1].destination,tb[st->timetableNo].train_type,tb[st->timetableNo+1].train_type);
            }

            break;
        case    INFO1_J_ST:         // インフォメーション1
            if (st->updatestate){
                st->updatestate = false;         // ステートマシン更新フラグ
                // 上段日本語表示
                drawTrainType(tb[st->timetableNo].train_type,true,st->posy + 24*2);
                drawDestination(tb[st->timetableNo].destination,true,st->posy + 24*2);
                drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,true,st->posy +24*2);
            }

            break;
        case    INFO1_E_ST:         // インフォメーション1
            if (st->updatestate){
                st->updatestate = false;         // ステートマシン更新フラグ
                // 上段英語表示
                drawTrainType(tb[st->timetableNo].train_type,false,st->posy + 24*2);      
                drawDestination(tb[st->timetableNo].destination,false,st->posy + 24*2);                
                drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,false,st->posy +24*2);
            }
            break;
        case    INFO2_J_ST:         // インフォメーション2


            break;
        case    INFO2_E_ST:         // インフォメーション2


            break;
        case    MAP_J_ST:         // インフォメーション2
            if (st->updatestate){
                st->updatestate = false;         // ステートマシン更新フラグ
                // 上段日本語表示
                drawTrainType(tb[st->timetableNo].train_type,true,st->posy + 24*2);      
                drawDestination(tb[st->timetableNo].destination,true,st->posy + 24*2);
                drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,true,st->posy +24*2);
                // 下段地図表示
                drawMapInfo(st->posy + 24*3,st->nexttrainmin, st->nextnexttrainmin);

            }
            break;
        case    MAP_E_ST:         // インフォメーション2
            if (st->updatestate){
                st->updatestate = false;         // ステートマシン更新フラグ
                // 上段英語表示
                drawTrainType(tb[st->timetableNo].train_type,false,st->posy + 24*2);      
                drawDestination(tb[st->timetableNo].destination,false,st->posy + 24*2);
                drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,false,st->posy +24*2);
                // 下段地図表示
                drawMapInfo(st->posy + 24*3,st->nexttrainmin, st->nextnexttrainmin);
            }

            break;
            
        case    APPROACH_J_ST:    // 電車がまいります
            if (st->updatestate){
                st->updatestate = false;         // ステートマシン更新フラグ
                // 上段日本語表示
                drawTrainType(tb[st->timetableNo].train_type,true,st->posy + 24*2);      
                drawDestination(tb[st->timetableNo].destination,true,st->posy + 24*2);
                if (tb[st->timetableNo].isDrawLeftTime)
                    fillRectangleSt7789(24*3, st->posy + 24*2, 24*3, 24, LCD_BLK); // 残り時間非表示 
                else
                    drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,true,st->posy +24*2);               

                // 下段クリア
                fillRectangleSt7789(0, st->posy + 24*3, LCD_XSIZE, 24, LCD_BLK); // black                   
            }
            drawApproaching(st->posy + 24*3,true);       // 電車がまいります。日本語表示


            break;
        case    APPROACH_E_ST:    // Train approaching
            if (st->updatestate){
                st->updatestate = false;         // ステートマシン更新フラグ
                // 上段英語表示
                drawTrainType(tb[st->timetableNo].train_type,false,st->posy + 24*2);      
                drawDestination(tb[st->timetableNo].destination,false,st->posy + 24*2);
                if (tb[st->timetableNo].isDrawLeftTime)
                    fillRectangleSt7789(24*3, st->posy + 24*2, 24*3, 24, LCD_BLK); // 残り時間非表示 
                else
                    drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,false,st->posy +24*2);

                // 下段クリア
                fillRectangleSt7789(0, st->posy + 24*3, LCD_XSIZE, 24, LCD_BLK); // black                   
            }
            drawApproaching(st->posy + 24*3,false);      // train approach 英語表示

            break;
        case    NEXT_STATION_ST:  // 次は鶯谷駅に停車します
            if (st->updatestate){
                st->updatestate = false;         // ステートマシン更新フラグ
                // 上段日本語表示
                drawTrainType(tb[st->timetableNo].train_type,true,st->posy + 24*2);      
                drawDestination(tb[st->timetableNo].destination,true,st->posy + 24*2);
                if (tb[st->timetableNo].isDrawLeftTime)
                    fillRectangleSt7789(24*3, st->posy + 24*2, 24*3, 24, LCD_BLK); // 残り時間非表示 
                else
                    drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,true,st->posy +24*2);

                // 下段日本語表示
                drawTrainType(tb[st->timetableNo+1].train_type,true,st->posy + 24*3);      
                drawDestination(tb[st->timetableNo+1].destination,true,st->posy + 24*3);
                drawTime(tb[st->timetableNo+1].isDrawLeftTime,tb[st->timetableNo+1].departure_time,st->nextnexttrainmin,true,st->posy +24*3);
            }
            break;
        default:
        
            break;
    }//  switch(current_state)
}//do_state


/// @brief スクロール文字制御関数：
/// スクロール文字を表示する。
/// @param st 車線ステータス構造体（参照渡し）
void drawScrollText(struct line_st *st){

    // インフォメーション先頭アドレス指定用
    const uint32_t *infodata;
    // スクロール処理

    if(!st->end_flg){               // エンドフラグが立っているなら表示はしない
        // スクロール情報表示開始
        if(st->start_flg){
            // 下段クリア
            fillRectangleSt7789(0, st->posy + 24*3, LCD_XSIZE, 24, LCD_BLK); // black

            // 表示するインフォメーションを選択
            st->info_no++;
            if (st->info_no > 2)
                st->info_no = 0;
            
            // printf("infono=%2d %3d\n",st->info_no,st->posy);
        } 

        switch (st->info_no)
        {
            case (uint16_t)0:
                infodata = &SIGNBOARD24_1[0];
                break;
            case (uint16_t)1:
                infodata = &SIGNBOARD24_2[0];
                break;
            case (uint16_t)2:
                infodata = &SIGNBOARD24_3[0];
                break;
            
            default:
                infodata = &SIGNBOARD24_1[0];
                break;
        }
        // スクロール表示関数呼び出し        
        st->end_flg = drawScroll24x24Char(infodata ,st->posy + 24*3, st->start_flg, &st->scrl_pos);
//      st->end_flg = drawScroll24x24Char(&SIGNBOARD24_1[0] ,st->posy + 24*3, st->start_flg, &st->scrl_pos);
        st->start_flg = 0;

        // 最後の文字まで終了している場合に言語切り替えタイマーを初期化する。
        if (st->end_flg)
            st->language_timer = 0;
    }//(!st->end_flg)
}//drawScrollText
