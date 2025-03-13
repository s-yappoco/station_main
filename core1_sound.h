///////////////////////////////////////////////
// マルチコア制御
// core1_sound.h
///////////////////////////////////////////////

#ifndef _CORE1_SOUND_H
#define _CORE1_SOUND_H


/// @brief マルチコアテスト関数
void core1_main();

/// @brief core1から呼び出されるタイマー割り込み関数
bool call_back_core1();

// 音声データ指定用構造体
struct sound_sts{
  const uint8_t  *sounddata;     // 8bitサウンドデータ指定用ポインタ
  bool  isStopedSound;           // 音声が停止しているとき、trueとなる
};


#endif // _CORE1_SOUND_H
