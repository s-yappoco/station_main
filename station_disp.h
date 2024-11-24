///////////////////////////////////////////////
// 駅電光掲示板表示関連関数
// station_disp.h
///////////////////////////////////////////////
#ifndef _STATION_DISP_H
#define _STATION_DISP_H

#include "pico/stdlib.h"
#include <stdio.h>
//==============
// Prototypes
//==============

/// @brief 駅名表示関数：
/// 指定した駅番号に従って駅名を表示する。24ドットフォント３文字表示を行う。
/// @param stationNumber 表示したい駅名番号
/// @param posy 表示したい座標位置（LCD絶対位置指定）
/// @param color_f 文字色
/// @param color_b 背景色
void drawStationName(uint16_t stationNumber, uint32_t posy, uint32_t color_f, uint32_t color_b);


/// @brief 電車種別（山手線・各駅停車・快速）表示関数
/// @param trainType 種別指定：山手線 0: 各駅停車 1: 快速 2
/// @param isJapanese 日本語か英語かを選択するフラグ trueで日本語表示
/// @param posy 表示したい座標位置（LCD絶対位置指定）
void drawTrainType(uint16_t trainType, bool isJapanese, uint32_t posy);

/// @brief 行き先駅表示関数：
/// 駅名を４文字で表示する
/// @param stationNumber 表示駅番号
/// @param isJapanese 日本語か英語かを選択するフラグ trueで日本語表示
/// @param posy 表示したい座標位置（LCD絶対位置指定）
void drawDestination(uint16_t stationNumber, bool isJapanese, uint32_t posy);

/// @brief 時刻or残り時間表示関数：
/// 電車か車での残り分数、もしくは到着予定時刻を表示する関数。
/// @param isDrawLeftTime 時刻を表示するか、残り分数を表示するか　tureで残り分数表示
/// @param time 出発時刻　hms構造体
/// @param min 残り分数
/// @param isJapanese 日本語か英語かを選択するフラグ trueで日本語表示
/// @param posy 表示したい座標位置（LCD絶対位置指定）
void drawTime(bool isDrawLeftTime,struct time_hms time, uint16_t min, bool isJapanese, uint32_t posy);

/// @brief 24ドット文字スクロール表示関数：
/// 渡された文字列をスクロール表示する。
/// 文字列データは最初のワードに色情報＋最終文字フラグを付けた17ワードを１文字データとする。
/// 0x0000 白
/// 0x0001 緑
/// 0x0002 赤
/// 0x0003 橙
/// 0x0010 が付いていたら、点滅文字とする。
/// 0x0100 が付いていたら、最終文字とする。
/// @param char_line 表示する文字列。ポインタ渡し
/// @param posy 表示したい座標位置（LCD絶対位置指定）
/// @param start_frg 表示を開始するフラグ。true指定で最初から表示を行う。
/// @param scrl_pos スクロールポジション。どこまで表示が進んでいるかを指定する。
/// @return スクロールが終了したら1を返す
uint16_t drawScroll24x24Char(const uint32_t *char_line ,uint32_t posy, uint16_t start_frg, uint16_t *scrl_pos);

/// @brief 線路地図を描画する。
/// @param posy 描画したい座標位置（LCD絶対位置指定）
void drawRailway(uint32_t posy);

/// @brief 車両を描画する
/// @param posx 描画したい座標位置　横軸
/// @param posy 描画したい座標位置　縦軸
void drawTrain(uint32_t posx, uint32_t posy);

/// @brief マップ情報表示関数：
/// 線路を描画し、車両位置情報に従って車両を描画する。
/// @param posy 描画したい座標位置　縦軸
/// @param no1train １両目車両の到着までの分数 ６分以内から表示される
/// @param no2train ２両目車両の到着までの分数 ６分以内から表示される
void drawMapInfo(uint32_t posy,uint16_t no1train, uint16_t no2train);

/// @brief 電車接近情報表示「電車がまいります」「Train approaching」
/// @param posy 描画したい座標位置　縦軸
/// @param isJapanese 日本語か英語かを選択するフラグ trueで日本語表示
void drawApproaching(uint32_t posy,bool isJapanese);


#endif //_STATION_DISP_H