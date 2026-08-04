///////////////////////////////////////////////
// 電光掲示板 サウンドデータ
// sounddata.h
///////////////////////////////////////////////

#ifndef _SOUNDDATA_H
#define _SOUNDDATA_H

#include "pico/stdlib.h"
#include <stdio.h>

extern const uint8_t SOUND_NO1[];
extern const uint8_t SOUND_NO2[];

extern const uint8_t SILENT_SOUNDDATA[];

// 電車接近アナウンス音声データ
extern const uint8_t APPROACH_SOUNDDATA1[];
extern const uint8_t APPROACH_SOUNDDATA2[];

// 発車ベル音声データ
extern const uint8_t BELLDATA1[];
extern const uint8_t BELLDATA2[];

// ドアが閉まります。アナウンス音声データ
extern const uint8_t DOOR_CLOSE_SOUNDDATA1[];
extern const uint8_t DOOR_CLOSE_SOUNDDATA2[];

// ジングルサウンドデータ
extern const uint8_t JINGLE_SOUND_DATA[];
#endif // _SOUNDDATA_H
