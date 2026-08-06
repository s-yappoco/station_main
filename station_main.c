///////////////////////////////////////////////
// メイン関数
// station_main
///////////////////////////////////////////////

// include
//#include "pico/stdlib.h"
//#include "hardware/spi.h"
#include "pico/multicore.h"
//#include "hardware/pwm.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "station_define.h"
#include "initialize.h"
#include "st7789.h"
#include "timer_ctrl.h"
#include "station_disp.h"
#include "signboard_bmp240.h"
#include "station_signboard.h"
#include "trainTimetable.h"
//#include "stname.h"
#include "core1_sound.h"
//#include "sounddata.h"
#include "button.h"
#include "stateCtrl.h"
#include "statework.h"


//プロトタイプ宣言

/// @brief 車線ステータス各種タイマー制御関数
/// 車線ステータスを制御する各種タイマーをインクリメントする。
/// 到着までの残り分数を計算する。
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列（値渡し
void timer_ctrl(struct line_st *st, const struct timetable tb[]);

/*
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
*/

/// @brief スクロール文字制御関数：
/// スクロール文字を表示する。
/// @param st 車線ステータス構造体（参照渡し）
void drawScrollText(struct line_st *st);

/// @brief 車線1処理実行
void doState1();

/// @brief 車線1スクロール処理
void doScrl1();

/// @brief 車線2処理実行
void doState2();

/// @brief 車線2スクロール処理
void doScrl2();

/// @brief 全体処理
void doAnyProcess();


// グローバル変数
bool    isBlinkOn =true;		// 文字列点滅用
bool    isChangeByTime;		// 次の電車に移行する手段を時間にするか、ボタンにするかのフラグ

// オンボードLEDピン番号設定
const uint8_t LED_PIN = PICO_DEFAULT_LED_PIN;

// 現在時刻構造体定義
struct time_struct time_disp;
// 最終時刻構造体定義
struct time_hms end_time;

// 1番線2番線構造体をそれぞれ設定
struct line_st line1;
struct line_st line2;

// 排他処理用のセマフォ
semaphore_t sem;
// 音声鳴動ステータスの定義
//struct sound_sts line1Soundsts;  // 1番線用
//struct sound_sts line2Soundsts;  // 2番専用

///////////////////////////////////////////////
// メイン関数
///////////////////////////////////////////////
int main() {

    // 初期化処理
    initializeSettings();


    ///////////////////////////////////////////
    // 変数定義
    ///////////////////////////////////////////


    // 現在時刻取得（初期値リード)
    time_disp = getClock();


    // 車線状態初期化
    initializeLineState();

    // タスク管理用
    station_prosess station_prosess = DO_ANYPROCESS;
    

    ///////////////////////////////////////////
    // 音声データ初期値指定
    ///////////////////////////////////////////
    //line1Soundsts.sounddata = SOUND_NO1;

    ///////////////////////////////////////////
    // タイマー割り込み
    ///////////////////////////////////////////
    // locateLcdPrintf(0,4);
    // setColorLcdPrintf(LCD_WHT,LCD_BLK);
    // printfSt7789("1234");

    
    // 繰り返しタイマーを設定
    struct repeating_timer timer;
    // 10ミリ秒ごとに割り込みを実行
    bool result = add_repeating_timer_ms(10, repeating_timer_callback, NULL, &timer);
    if (!result) {
        while(1);  //停止
    }
    

    // core1を起動
    //core1_initialize();


    // //ジングルサウンド鳴動（車線0でなる）
    // playJingleSound();

    // sleep_ms(5000);


    // locateLcdPrintf(0,5);
    // printfSt7789("B");
    // タイマーを0にリセット
    // clear100msecTimer();

    // locateLcdPrintf(0,5);
    // printfSt7789("C");  


    ///////////////////////////////////////////
    // マルチタスク処理実行
    ///////////////////////////////////////////
    // // セマフォを初期化
    // sem_init(&sem, 1, 1);
    // // セマフォの許可を解除
    // sem_release(&sem);

    // core1で動作させる関数を実行する。
    // multicore_launch_core1(core1_main);

    // locateLcdPrintf(0,5);
    // printfSt7789("D");

    // sleep_ms(200);
    // //ジングルサウンド鳴動（車線0でなる）
    // playJingleSound();
    // //playDepartureMelody(0);

    // sleep_ms(2000);

    // ためしに車線1で音を鳴らす。
    //announceDoorCloseing(1);
    //announceTrainApproach(1);
    //playDepartureMelody(1);

    // locateLcdPrintf(0,5);
    // printfSt7789("E");




    ///////////////////////////////////////////
    // 画面初期表示
    ///////////////////////////////////////////
    // 看板表示(車線1)
	drawBMP_B(&BMP_PLT_UP[0], &BMP_DAT_UP[0], 0 ,line1.posy, SIGNBOARDXSIZE , SIGNBOARDYSIZE);
    // 看板表示(車線2)
	drawBMP_B(&BMP_PLT_DOWN[0], &BMP_DAT_DOWN[0], 0 ,line2.posy, SIGNBOARDXSIZE , SIGNBOARDYSIZE);



    // デバッグ
    // locateLcdPrintf(0,12);
    // printfSt7789("outWhile");

    // デバッグ
    // while(true){
    //     // 例: LEDを点滅させる
    //     gpio_put(LED_PIN, 1);
    //     sleep_ms(50);
    //     gpio_put(LED_PIN, 0);
    //     sleep_ms(50);
    // }

    // debug
    // ボタンの処理が正しいかどうか確認する
    /*
    while(true){

        if (chk10msec()){

            check_buttons(); // ボタンチェック関数呼び出し


            locateLcdPrintf(0,6);
            setColorLcdPrintf(LCD_WHT,LCD_BLK);
            printfSt7789("Button check ");

            if (is_button_released_flag_lineno(0)){
                // 車線1のボタンが離された
                locateLcdPrintf(0,7);
                setColorLcdPrintf(LCD_WHT,LCD_BLK);
                printfSt7789("Button 1 ");
                clear_button_released_flag_lineno(0); // フラグをクリア

            }else{
                locateLcdPrintf(0,7);
                setColorLcdPrintf(LCD_WHT,LCD_BLK);
                printfSt7789("         ");
            }

            if (is_button_released_flag_lineno(1)){
                // 車線2のボタンが離された
                locateLcdPrintf(0,8);
                setColorLcdPrintf(LCD_WHT,LCD_BLK);
                printfSt7789("Button 2 ");
                clear_button_released_flag_lineno(1); // フラグをクリア
            }else{
                locateLcdPrintf(0,8);
                setColorLcdPrintf(LCD_WHT,LCD_BLK);
                printfSt7789("         ");
            }
        }
    }
    */

    //デバッグ
    // 音声が鳴るかどうか、確認する
    /*
    uint8_t debugsts[] = {0,0};
    uint8_t line_no_debug = 0;
    while(true){
        if (chk10msec()){

            check_buttons(); // ボタンチェック関数呼び出し

            for(line_no_debug=0;line_no_debug<2;line_no_debug++){
                if (is_button_released_flag_lineno(line_no_debug)){
                    locateLcdPrintf(0,7);
                    setColorLcdPrintf(LCD_WHT,LCD_BLK);
                    printfSt7789("line %d ",line_no_debug+1);
                    switch (debugsts[line_no_debug]) {
                        case 0:
                            debugsts[line_no_debug] = 1;
                            announceTrainApproach(line_no_debug);
                            break;
                        case 1:
                            debugsts[line_no_debug] = 2;
                            playDepartureMelody(line_no_debug);
                            break;
                        case 2:
                            debugsts[line_no_debug] = 0;
                            announceDoorCloseing(line_no_debug);
                            break;
                        default:
                            debugsts[line_no_debug] = 0;
                    }
                    clear_button_released_flag_lineno(line_no_debug); // フラグをクリア
                }
    
            }
            // デバッグ
            locateLcdPrintf(0,3);
            setColorLcdPrintf(LCD_WHT,LCD_BLK);
            //printfSt7789("%1d / %1d", isSoundStop(0), isSoundStop(1));
 
        }
    }
    */


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
                    //timer_ctrl(&line1, line1TB);
                    // ステートマシン(フェーズ)
                    //setDisplayMode(&line1, line1TB);
                    // ステートマシン（ステート）
                    //switchLanguage(&line1);
                    // ステート実行
                    //drawLcdDisplay(&line1, line1TB);


                    // ステートマシン
                    //stationStateCtrl(&line1, line1TB);

                    // ステートマシンにしたがい、処理実行
                    //doStateWork(&line1, line1TB);

                    doState1();
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
                    //timer_ctrl(&line2, line2TB);
                    // ステートマシン(フェーズ)
                    //setDisplayMode(&line2, line2TB);
                    // ステートマシン（ステート）
                    //switchLanguage(&line2);
                    // ステート実行
                    //drawLcdDisplay(&line2,line2TB);


                    // ステートマシン
                    //stationStateCtrl(&line2, line2TB);

                    // ステートマシンにしたがい、処理実行
                    //doStateWork(&line2, line2TB);

                    doState2();
                    station_prosess = DO_SCRL2;

                    break;
				
				// ***** 車線2のスクロール文字列を表示するタスク
                case    DO_SCRL2:
                    // インフォメーションスクロール
                    drawScrollText(&line2);

                    station_prosess = DO_ANYPROCESS;
                    break;

				// ***** その他(デバッグ用表示)タスク
                case    DO_ANYPROCESS:

                    doAnyProcess();

                    /*
                    // ボタンチェック関数呼び出し。
                    // 50msec毎に呼び出す
                    check_buttons();
                    
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
                    //if(gpio_get(PIN_APPBTN1)==0 || gpio_get(PIN_APPBTN2)==0){
                    //    gpio_put(LED_PIN,1);
                    //}else{
                    //    gpio_put(LED_PIN,0);
                    //}
                    
                    if(gpio_get(PIN_APPBTN1)==0 || gpio_get(PIN_APPBTN2)==0){
                        // line1Soundsts.sounddata = SOUND_NO1;
                    }

                    
                    // デバッグここまで                  
                    */
                    
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
    st->next_st_timer++;

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

/*
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
		// ボタン状態取得
		if (is_button_released_flag_lineno(st->line_no)){
            clear_button_released_flag_lineno(st->line_no);
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
                st->current_state = APPROACH_SOUND_ST;  // 接近アナウンス放送
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
                st->current_state = APPROACH_SOUND_ST;  // 接近アナウンス放送
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
                st->current_state = APPROACH_SOUND_ST;  // 接近アナウンス放送
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
                st->current_state = APPROACH_SOUND_ST;  // 接近アナウンス放送
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


*/


/*
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
        case    APPROACH_SOUND_ST:    //接近アナウンス放送開始
            if (!st->updatestate){
                st->current_state = APPROACH_J_ST;  
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
            st->current_state = NEXT_ST_SILENT1_ST;
            st->next_st_timer = 0;
            st->updatestate = true;         // ステートマシン更新フラグ 
            clear_button_released_flag_lineno(st->line_no);
            break;

        case    NEXT_ST_SILENT1_ST:     // 電車到着 ベル鳴動前
            if (isChangeByTime){        // 時間切り替えモード
                if(st->next_st_timer > SILENT1TIME){
                    st->current_state = NEXT_ST_BELL;
                    st->next_st_timer = 0;
                    st->updatestate = true;         // ステートマシン更新フラグ 
                    clear_button_released_flag_lineno(st->line_no);
                }
            } else {                    // ボタン切り替えモード
                if(is_button_released_flag_lineno(st->line_no)){
                    st->current_state = NEXT_ST_BELL;
                    st->next_st_timer = 0;
                    st->updatestate = true;         // ステートマシン更新フラグ 
                    clear_button_released_flag_lineno(st->line_no);             
                }    
            }
            break;
        
        case    NEXT_ST_BELL:           // チャイム鳴動中
            if (isChangeByTime){        // 時間切り替えモード
                if(isSoundStop(st->line_no)){                   // チャイムが鳴り終わったら
                    st->current_state = NEXT_ST_ANNOUNCE;
                    st->next_st_timer = 0;
                    st->updatestate = true;         // ステートマシン更新フラグ
                    clear_button_released_flag_lineno(st->line_no);                  
                }
            } else {                    // ボタン切り替えモード
                if(isSoundStop(st->line_no) || is_button_released_flag_lineno(st->line_no)){                   // チャイムが鳴り終わったら
                    st->current_state = NEXT_ST_ANNOUNCE;
                    st->next_st_timer = 0;
                    st->updatestate = true;         // ステートマシン更新フラグ         
                    clear_button_released_flag_lineno(st->line_no);     
                }         
            }
            break;

        case    NEXT_ST_ANNOUNCE:       // 1番線、ドアが閉まります。
            if (isSoundStop(st->line_no)){
                st->current_state = NEXT_ST_SILENT2_ST;
                st->next_st_timer = 0;
                st->updatestate = true;         // ステートマシン更新フラグ                        
            }

            break;
        case    NEXT_ST_SILENT2_ST:     // 音声無音状態2（扉が閉まって、発車する状態を想定）  
            if (st->next_st_timer > SILENT2TIME){

                
            }       
            break;
        
        default:
        

            break;

    }// switch(current_state)
}//switchLanguage

*/

/*

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

        case    APPROACH_SOUND_ST:    // 接近アナウンス放送開始
            if (st->updatestate){
                st->updatestate = false;         // ステートマシン更新フラグ
                announceTrainApproach(st->line_no);  // 接近アナウンス放送開始

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
        
        case    NEXT_ST_SILENT1_ST:  // 音声無音状態1（扉が開いた状態を想定）
            if (st->updatestate){
                st->updatestate = false;         // ステートマシン更新フラグ
                                                    // ドアが開いた音を入れるなら、ここ
            }
            break;

        case    NEXT_ST_BELL:         // チャイム鳴動中
            if (st->updatestate){
                st->updatestate = false;         // ステートマシン更新フラグ
                playDepartureMelody(st->line_no);
            }

            break;

        case    NEXT_ST_ANNOUNCE:     // 1番線、ドアが閉まります。
            if (st->updatestate){
                st->updatestate = false;         // ステートマシン更新フラグ
                announceDoorCloseing(st->line_no);
                
            }
            break;

        case    NEXT_ST_SILENT2_ST:   // 音声無音状態2（扉が閉まって、発車する状態を想定）
            if (st->updatestate){
                st->updatestate = false;         // ステートマシン更新フラグ
                                                // ドアが閉まる音を入れるなら、ここ
             }
            break;
        default:
        
            break;
    }//  switch(current_state)
}//do_state

*/

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


/// @brief 車線1処理実行
void doState1(){
    // タイマーの設定
    timer_ctrl(&line1, line1TB);

    // ステートマシン
    stationStateCtrl(&line1, line1TB);

    // ステートマシンにしたがい、処理実行
    doStateWork(&line1, line1TB);


}

/// @brief 車線1スクロール処理
void doScrl1(){
    // インフォメーションスクロール
    drawScrollText(&line1);

}

/// @brief 車線2処理実行
void doState2(){

    // タイマーの設定
    timer_ctrl(&line2, line2TB);

    // ステートマシン
    stationStateCtrl(&line2, line2TB);

    // ステートマシンにしたがい、処理実行
    doStateWork(&line2, line2TB);

}

/// @brief 車線2スクロール処理
void doScrl2(){
    // インフォメーションスクロール
    drawScrollText(&line2);
}

/// @brief 全体処理
void doAnyProcess(){
    // ボタンチェック関数呼び出し。
    // 50msec毎に呼び出す
    check_buttons();
    
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

    // // デバッグ：車線1,2のステータス表示
    // locateLcdPrintf(0,7);
    // setColorLcdPrintf(LCD_WHT,LCD_BLK);
    // printfSt7789("%2d:%2d/%4d/%4d",1,line1.lineState, line1.phase_timer,line1.language_timer);
    // locateLcdPrintf(0,8);
    // setColorLcdPrintf(LCD_WHT,LCD_BLK);
    // printfSt7789("%2d:%2d/%4d/%4d",2,line2.lineState, line2.phase_timer,line2.language_timer);

    // デバッグ：現在時刻表示
    time_disp = getClock();
    locateLcdPrintf(0,14);
    setColorLcdPrintf(LCD_YEL, LCD_BLK);    
    printfSt7789("[%02d:%02d:%02d] [%02d]", time_disp.timehms.hh, time_disp.timehms.mm, time_disp.timehms.ss, time_disp.time_msec);

    //デバッグ：ボタンを押下したらLED点灯
    //green LED
    //if(gpio_get(PIN_APPBTN1)==0 || gpio_get(PIN_APPBTN2)==0){
    //    gpio_put(LED_PIN,1);
    //}else{
    //    gpio_put(LED_PIN,0);
    //}
    
    if(gpio_get(PIN_APPBTN1)==0 || gpio_get(PIN_APPBTN2)==0){
        // line1Soundsts.sounddata = SOUND_NO1;
    }

    
    // デバッグここまで                  
                    

}