///////////////////////////////////////////////
// タイマー割り込み関連
// trainTimetable.c
///////////////////////////////////////////////

// include
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdio.h>

#include "station_define.h"
//#include "stname.h"
#include "st7789.h"
//#include "station_disp.h"

//=============
// Globals
//=============
bool is100msecTiming = false;      // 100msecタイミングフラグ
bool is10msecTiming = false;      // 10msecタイミングフラグ

// 現在時刻構造体
struct time_struct time_now;    // 現在時刻構造体定義

//100msecタイマーを作るためのカウンタ
uint16_t count10 = 0;

// 100m秒タイマー
uint16_t timer100msec = 0;

// ステートマシンフェーズ移行用タイマー
uint16_t phasetimer = 0;

// 点滅タイマー
uint16_t brinktimer100msec = 0;

// scrollタイマー
uint16_t scrolltimer10msec = 0;

// スクロールタイミング
bool isScrollTiming1 = false;        // 車線１用スクロールタイミング
bool isScrollTiming2 = false;        // 車線２用スクロールタイミング

// 山手線がくるまでの分数（最大7分、最小1分）
uint16_t min_yamanote1 = 3;
uint16_t min_yamanote2 = 7;

/// @brief 現在時刻初期化関数
void initializeClock(){
    time_now.timehms.hh = START_HH;		// 時
    time_now.timehms.mm = START_MM;		// 分
    time_now.timehms.ss = START_SS;		// 秒
    time_now.time_msec = 00;			// 100msec毎にカウントアップ 10数えたら1秒経過
}

/// @brief 時計動作関数
void tickClock(){
        // 現在時刻の設定
    time_now.time_msec++;
    if (time_now.time_msec >= ONESECCOUNT){
        time_now.time_msec = 0;
        time_now.timehms.ss++;					// 秒

        if (time_now.timehms.ss >= 59){
            time_now.timehms.ss = 0;
            time_now.timehms.mm++;				// 分
            if (time_now.timehms.mm >= 59) {
                time_now.timehms.mm = 0;
                time_now.timehms.hh ++;			// 時
                if (time_now.timehms.hh > 24){
                    time_now.timehms.hh = 1;
                }
            }
        }
    }
}

/// @brief 現在時刻を現在時刻を戻り値とする関数
/// @return 現在時刻構造体
struct time_struct getClock(){
    return time_now;
}


/// @brief タイマー割り込みハンドラー
/// @param t 割り込み用タイマーポインタ
/// @return 繰り返し実行できる正常な状態であればtrue
bool repeating_timer_callback(struct repeating_timer *t) {
    
    //10msecタイマー割り込み発生フラグセット
    is10msecTiming = true;

    //100msecタイマー
    count10++;
    if (count10 >= 10){
        count10 = 0;
    
        // 100msecタイマーフラグセット
        is100msecTiming = true;

        //100msecタイマーインクリメント
        timer100msec++;
        phasetimer++;
        //点滅用タイマーインクリメント
        brinktimer100msec++;
        if (brinktimer100msec > 10)
            brinktimer100msec = 0;

        /*
        locateLcdPrintf(0,13);
        setColorLcdPrintf(LCD_WHT,LCD_BLK);
        printfSt7789("t=%d",timer100msec);
        */
        //時計
        tickClock();

        return true;  // 繰り返し実行するためにtrueを返す

    }

    // スクロールタイミングを生成するための10msecカウンタ
    scrolltimer10msec++;
    // 上半分のスクロール用
    if (scrolltimer10msec == 1){
        isScrollTiming1 = true;
    
    // 下半分のスクロール用
    }else if (scrolltimer10msec == 2) {
        isScrollTiming2 = true;
    }
    // 30msecごとにループ
    if (scrolltimer10msec > 2){
        scrolltimer10msec = 0;
    }
    
}

/// @brief 100msec過ぎているかをチェックする関数
/// @return 100msec過ぎていたらtrue
bool check100msecTimer(){
    if (is100msecTiming == true){
        is100msecTiming = false;
        return true;
    } else {
        return false;
    }
    
}


/// @brief 指定した時間を経過しているかどうかを判定する関数。100msecタイマーベース
/// @param secvalue 指定時間。単位は秒
/// @return タイマーが指定時間を過ぎていたらtureとなる
bool hasTimePassed(uint16_t secvalue){
    if(timer100msec >= secvalue*10)
        return (true);
    else
        return (false);

}

// 100msecタイマーリセット関数


/// @brief 100msecタイマー初期化関数
void clear100msecTimer(void){
    timer100msec = 0;
}

/// @brief 100msecフェーズ移行用タイマーチェック関数
/// @param secvalue 指定時間。単位は秒
/// @return タイマーが指定時間を過ぎていたらtureとなる
bool checkPheseTimer(uint16_t secvalue){
    if(phasetimer >= secvalue*10)
        return (true);
    else
        return (false);

}

/// @brief フェーズタイマー初期化関数
void clearPhaseTimer(void){
    phasetimer = 0;
}

/// @brief  LED点滅タイミング生成関数
/// @return 点灯させるときtrue　消灯させるときfalse
bool brinktimer(void){
    if(brinktimer100msec < 8){
        return(true);
    } else {
        return(false);
    }
}



/// @brief タイマー割り込み発生フラグチェック
/// @return 10msec割り込みが発生したら、trueを返す。（内部フラグは自動でクリアする）
bool chk10msec(void){
    if(is10msecTiming){
        is10msecTiming = false;
        return(true);
    }else{
        return(false);
    }
}


/// @brief スクロールタイミング生成
/// @return スクロールタイミングでtrueとなる。
bool checkScrollTiming1(void){
    if (isScrollTiming1 == true){
        isScrollTiming1 = false;
        return true;
    } else {
        return false;
    } 
}


/// @brief スクロールタイミング生成
/// @return スクロールタイミングでtrueとなる。
bool chkisScrollTiming2(void){
    if (isScrollTiming2 == true){
        isScrollTiming2 = false;
        return true;
    } else {
        return false;
    } 
}

/// @brief 二つの時刻を比較する
/// @param AT 時刻構造体　A
/// @param BT 時刻構造体　B
/// @return AがBより過去だったら、true、そうでなければfalse
bool isBefore(struct time_hms AT,struct time_hms BT){
    // 時を比較する
    if(AT.hh < BT.hh){
        return(true);
    }else if (AT.hh > BT.hh){
        return(false);
    }

    // 分を比較する
    if(AT.mm < BT.mm){
        return(true);
    }else if (AT.mm > BT.mm){
        return(false);
    }

    // 秒を比較する
    if(AT.ss < BT.ss){
        return(true);
    }else{
        return(false);
    }

}


/// @brief 二つの時刻の差を返す
/// @param AT 時刻構造体　A
/// @param BT 時刻構造体　B
/// @return 差分のmitute数を返す。
int16_t getTimeDifference(struct time_hms AT,struct time_hms BT){
	
	int32_t timeA,timeB;
    // 秒単位で計算する
	timeA = AT.hh * 60 * 60 + AT.mm * 60 + AT.ss;
	timeB = BT.hh * 60 * 60 + BT.mm * 60 + BT.ss;

    // 分単位で返す。
    //１分未満が残り１分になるように、１を足しておく
	return((timeA - timeB)/60 + 1);

}

