///////////////////////////////////////////////
// ステートマシン制御
// stateCtrl.h
///////////////////////////////////////////////

#ifndef _STATECTRL_H
#define _STATECTRL_H

#include "station_define.h"

/// @brief 車線ステータス初期化関数
void initializeLineState();

/// @brief 電光掲示板表示内容制御ステートマシン
/// @param st 車線ステートマシン（参照渡し）
/// @param tb 時刻表テーブル（参照渡し：定数）
void stationStateCtrl(struct line_st *st, const struct timetable tb[]);

/// @brief 電車が近づいているかどうかを判定する
/// @param st 車線ステータス
/// @param tb 時刻表テーブル
/// @return 接近アナウンス放送へ移行するときtrue
bool isTrainApproaching(struct line_st *st,const struct timetable tb[]);

#endif // _STATECTRL_H