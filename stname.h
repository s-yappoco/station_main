///////////////////////////////////////////////
// 目的地等駅名フォント 数値フォント宣言
// stname.h
///////////////////////////////////////////////
#ifndef _STNAME_H
#define _STNAME_H

#include "pico/stdlib.h"
#include <stdio.h>

// 駅名データ　24ビットフォント4文字　日本語 英語の順
// 行き先駅順番
// 0	上野東京方面
// 1	池袋新宿方面
// 2	大船
// 3	磯子
// 4	蒲田
// 5	桜木町
// 6	鶴見
// 7	東神奈川
extern const uint32_t STNAME_FONT24[];

// 電車種別表示用　山手線 各駅停車 快速のフォントデータ
// 24ビットフォント3文字　日本語　英語の順
extern const uint32_t trainTypeFONT24[];

// 残り分数表示フォントデータ（日本語）「約　分後」
extern const uint32_t MINUTE_LEFT_J[];

// 残り分数表示フォントデータ（英語）「min.」
extern const uint32_t MINUTE_LEFT_E[];

// 残り分数表示用　数値フォントデータ(0-9)
extern const uint16_t MINUTE_NUM[];

// 列車到着時刻用　数値フォントデータ(0-9 :)
extern const uint16_t MINUTE_NUM_B[];

// 地図表示用レールフォント
extern const uint32_t RAILWAY_A[];

// 地図表示用レールフォント当駅ビットマップ色データ
extern uint16_t BMP_RAILWAY_B[16];

// 地図表示用レールフォント当駅ビットマップ
extern const uint8_t RAILWAY_B[];

// 電車表示(24dot x 9dot)
extern const uint32_t TRAIN_MODEL[];

// 「電車がまいります」 フォントデータ
extern const uint32_t SIGNBOARD24APP_J[];

// 「Train approaching」フォントデータ
extern const uint32_t SIGNBOARD24APP_E[];

#endif //_STNAME_H