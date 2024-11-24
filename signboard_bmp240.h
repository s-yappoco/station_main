///////////////////////////////////////////////
// 看板データ宣言
// signboard_bmp240.h
///////////////////////////////////////////////

// 看板ビットマップデータ
// 16色ビットマップとして記録する。
// 横240×縦48
// このビットマップデータは左下から右上に向かって作成している

// 色パレット 16色

#ifndef _SIGNBOARD_BMP240_H
#define _SIGNBOARD_BMP240_H

#include "pico/stdlib.h"
#include <stdio.h>

#define SIGNBOARDXSIZE (240)		// 看板のXピクセルサイズ
#define SIGNBOARDYSIZE (48)			// 看板のYピクセルサイズ



// ビットマップ色パレット(1番線/2番線)
extern const uint16_t BMP_PLT_UP[16];
extern const uint16_t BMP_PLT_DOWN[16];

// ビットマップデータ(1番線/2番線)
extern const uint8_t BMP_DAT_UP[];
extern const uint8_t BMP_DAT_DOWN[];

#endif // _SIGNBOARD_BMP240_H