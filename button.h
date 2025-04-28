///////////////////////////////////////////////
// 押しボタン関連
// button.h
///////////////////////////////////////////////

#ifndef _BUTTON_H
#define _BUTTON_H


// ボタンに関する設定
#define MAX_BUTTONS 4               // 最大ボタン数
#define DEBOUNCE_DELAY 3            // デバウンスチェックの遅延（50ms × 3 = 150ms）


/// @brief ボタンの状態を全てチェックする関数
/// 50msec毎に実行すると想定
void check_buttons();

/// @brief ボタンのフラグを取得する関数
/// @param button_id 何番目のボタンをチェックするかを指定する 0～MAX_BUTTONS-1
/// @return ボタンが放されたときにtrue
bool is_button_released(uint8_t button_id);

/// @brief ボタンのフラグをクリアする関数
/// @param button_id 何番目のボタンをチェックするかを指定する 0～MAX_BUTTONS-1
void clear_button_released_flag(uint8_t button_id);

/// @brief 車線番号対応のボタンが押されたどうかを取得する
/// @param line_no 車線番号　0 または　1
/// @return どちらかのボタンが離されたときにtrue
bool is_button_released_flag_lineno(uint8_t line_no);

/// @brief 車線番号対応のボタンフラグをクリアする関数
/// @param line_no  車線番号　0 または　1
void clear_button_released_flag_lineno(uint8_t line_no);

#endif // _BUTTON_H
