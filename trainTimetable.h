///////////////////////////////////////////////
// 時刻表（1番線　2番線）宣言
// trainTimetable.h
///////////////////////////////////////////////

#ifndef _TRAINTIMETABLE_H
#define _TRAINTIMETABLE_H

#include "pico/stdlib.h"
#include <stdio.h>

#include "station_define.h"

// 行き先定数定義
// 0	上野東京方面
// 1	池袋新宿方面
// 2	大船
// 3	磯子
// 4	蒲田
// 5	桜木町
// 6	鶴見
// 7	東神奈川

#define TOKYOUENO           (0)
#define IKEBUKUROSINJUKU    (1)
#define OFUNA               (2)
#define ISOGO               (3)
#define KAMATA              (4)
#define SAKURAGICHO         (5)
#define TSURUMI             (6)
#define HIGASHIKANAGAWA     (7)

// 電車種別定義
#define YAMANOTELINE        (0)
#define LOCAL               (1)
#define RAPID               (2)

// 1番線時刻表
extern const struct timetable line1TB[];

// 2番線時刻表
extern const struct timetable line2TB[];

#endif  // _TRAINTIMETABLE_H

