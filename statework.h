///////////////////////////////////////////////
// メイン定義ファイル
// statework.h
///////////////////////////////////////////////

#ifndef _STATE_WORK
#define _STATE_WORK

#include "station_define.h"

/// @brief ステートマシンの状態に従って実行する関数を呼び出す
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doStateWork(struct line_st *st, const struct timetable tb[]);

/// @brief 上段下段に日本語で行き先と時刻を表示する
/// @param st 車線ステータス
/// @param tb 時刻表構造体配列
void doJpDualDisplay(struct line_st *st, const struct timetable tb[]);

/// @brief 上段下段に日本語表示の終了待ち
/// @param st 車線ステータス
/// @param tb 時刻表構造体配列
void doJPDualWait(struct line_st *st, const struct timetable tb[]);

/// @brief 上段下段に英語で行き先と時刻を表示する
/// @param st 車線ステータス
/// @param tb 時刻表構造体配列
void doEnDualDisplay(struct line_st *st, const struct timetable tb[]);

/// @brief 上段下段に英語表示の終了待ち
/// @param st 車線ステータス
/// @param tb 時刻表構造体配列
void doEnDualWait(struct line_st *st, const struct timetable tb[]);

/// @brief スクロール案内表示を開始する
/// @param st 車線ステータス
/// @param tb 時刻表構造体配列
void doGuidanceInit(struct line_st *st, const struct timetable tb[]);

/// @brief スクロール案内表示中（上段に日本語表示上書き）
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doJpGuidanceDisplay(struct line_st *st, const struct timetable tb[]);

/// @brief 上段日本語表示下段スクロール中ウエイト
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doJpGuidanceWait(struct line_st *st, const struct timetable tb[]);

/// @brief スクロール案内表示中（上段に英語表示上書き）
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doEnGuidanceDisplay(struct line_st *st, const struct timetable tb[]);

/// @brief 上段英語表示下段スクロール中ウエイト
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doEnGuidanceWait(struct line_st *st, const struct timetable tb[]);

/// @brief 上段日本語表示 下段地図表示
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doJpMapDisplay(struct line_st *st, const struct timetable tb[]);

/// @brief 上段日本語表示 下段地図表示中ウエイト
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doJpMapWait(struct line_st *st, const struct timetable tb[]);

/// @brief 上段英語表示 下段地図表示
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doEnMapDisplay(struct line_st *st, const struct timetable tb[]);

/// @brief 上段英語表示 下段地図表示中ウエイト
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doEnMapWait(struct line_st *st, const struct timetable tb[]);

/// @brief 電車接近アナウンス放送開始
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doApproachingMelodyStart(struct line_st *st, const struct timetable tb[]);

/// @brief 電車接近表示（日本語）
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doJpApproachingDisplay(struct line_st *st, const struct timetable tb[]);

/// @brief 電車がまいります。表示中
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doJpApproachingWait(struct line_st *st, const struct timetable tb[]);

/// @brief 電車接近表示（英語）
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doEnApproachingDisplay(struct line_st *st, const struct timetable tb[]);

/// @brief train approach 表示中
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doEnApproachingWait(struct line_st *st, const struct timetable tb[]);

/// @brief 上段のみ日本語表示（電車発車前表示）
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doJpSingleLineDisplay(struct line_st *st, const struct timetable tb[]);

/// @brief 上段のみ日本語表示 発車メロディ鳴動待ち
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doJpSingleLineWait(struct line_st *st, const struct timetable tb[]);

/// @brief 発車メロディ鳴動開始
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doDepatureMelodyStart(struct line_st *st, const struct timetable tb[]);

/// @brief 発車メロディ鳴動中
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doDepatureMelodyPlaying(struct line_st *st, const struct timetable tb[]);

/// @brief ドア締めアナウンス鳴動開始
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doDoorCloseAnnounceStart(struct line_st *st, const struct timetable tb[]);

/// @brief ドア締めアナウンス鳴動終了待ち
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doDoorCloseAnnouncePlaying(struct line_st *st, const struct timetable tb[]);

/// @brief 電車発車 次のデータ準備
/// @param st 車線ステータス構造体
/// @param tb 時刻表構造体配列
void doSetNextTrain(struct line_st *st, const struct timetable tb[]);

#endif // _STATE_WORK