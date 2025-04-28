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

/// @brief 電車の接近アナウンスの放送を開始します。
/// @param line_no 番線番号　0 ～　1
void announceTrainApproach(uint8_t line_no);

/// @brief 電車の発車ベルの放送を開始します。
/// @param line_no 番線番号　0 ～　1
void playDepartureMelody(uint8_t line_no);

/// @brief ドア閉アナウンスの放送を開始します。
/// @param line_no 番線番号　0 ～　1
void announceDoorCloseing(uint8_t line_no);

/// @brief 放送が停止しているかどうかを確認する
/// @param line_no 番線番号　0 ～　1
/// @return 放送が停止していればtrue;
bool isSoundStop(uint8_t line_no);

/// @brief AUDIO　ICへミュート出力を行う
void muteSound();

/// @brief AUDIO ICのミュートを解除する
void unMuteSound();

#endif // _CORE1_SOUND_H
