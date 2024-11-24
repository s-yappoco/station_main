///////////////////////////////////////////////
// 電光掲示板 文字列等定義
// station_signboard.h
///////////////////////////////////////////////

#ifndef _STATION_SIGNBOARD_H
#define _STATION_SIGNBOARD_H

#include "pico/stdlib.h"
#include <stdio.h>

// 文字列データ1
//駅構内や列車内でのお忘れ物には十分ご注意ください。
extern const uint32_t SIGNBOARD24_1[];

// 文字列データ2
//【遅延】湘南新宿ラインは、宇都宮線内での人身事故の影響で、宇都宮線・横須賀線方面の一部列車に遅れと運休が出ています。
extern const uint32_t SIGNBOARD24_2[];

// 文字列データ3
//★発車間際の駆け込み乗車は大変危険です。ゆとりあるご乗車をお願い致します。
extern const uint32_t SIGNBOARD24_3[];

#endif  // _STATION_SIGNBOARD_H
