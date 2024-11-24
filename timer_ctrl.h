#ifndef _TIMER_CTRL_H
#define _TIMER_CTRL_H

#include "pico/stdlib.h"
#include <stdio.h>

// 時刻初期化
void initializeClock();

// 時計動作関数
void tickClock();

/// @brief 現在時刻を現在時刻を戻り値とする関数
/// @return 現在時刻構造体
struct time_struct getClock();

// タイマー割り込みハンドラ
bool repeating_timer_callback(struct repeating_timer *t) ;

// 100msec過ぎているかをチェックする関数
bool check100msecTimer();

// タイマーが指定時間を過ぎていたらtureとなる
// 100msecタイマーと100倍した値で比較する
bool hasTimePassed(uint16_t secvalue);

// 100msecタイマーリセット関数
void clear100msecTimer(void);

// 100msecフェーズ移行用タイマー
bool checkPheseTimer(uint16_t secvalue);
// 100msecタイマーリセット関数
void clearPhaseTimer(void);

// 点滅タイミング生成関数
// 2秒 true、1秒 false
bool brinktimer(void);

// スクロールタイミング生成departedTran
bool checkScrollTiming1(void);
bool chkisScrollTiming2(void);

// タイマー割り込み発生フラグチェック
bool chk10msec(void);

/// @brief 二つの時刻を比較する
/// @param AT 時刻構造体　A
/// @param BT 時刻構造体　B
/// @return AがBより過去だったら、true、そうでなければfalse
bool isBefore(struct time_hms AT,struct time_hms BT);

/// @brief 二つの時刻の差を返す
/// @param AT 時刻構造体　A
/// @param BT 時刻構造体　B
/// @return 差分のmitute数を返す。
int16_t getTimeDifference(struct time_hms AT,struct time_hms BT);

#endif  // _TIMER_CTRL_H

