///////////////////////////////////////////////
// 押しボタン関連
// button.c
///////////////////////////////////////////////

#include "pico/stdlib.h"
#include <stdio.h>

#include "station_define.h"
#include "button.h"

typedef struct {
    uint gpio_pin;                  // GPIOピン番号
    bool current_state;             // 現在の状態（安定状態） trueでボタン離す
    bool prev_state;                // 前回の安定状態        trueでボタン離す
    uint8_t debounce_counter;       // デバウンス確認用カウンタ
    bool button_released_flag;      // ボタンが放されたときのフラグ
} Button;

// ボタン状態を管理する配列（最大ボタン数分）
static Button buttons[MAX_BUTTONS] = {
    {PIN_APPBTN1, true, true, 0, false},    // ボタン0（GPIO0）
    {PIN_APPBTN2, true, true, 0, false},    // ボタン1（GPIO1）
    {PIN_APPBTN3, true, true, 0, false},    // ボタン2（GPIO10）
    {PIN_APPBTN4, true, true, 0, false}     // ボタン3（GPIO11）
};

/// @brief ボタンの状態を全てチェックする関数
/// 50msec毎に実行すると想定
void check_buttons() {
    for (uint8_t i = 0; i < MAX_BUTTONS; i++) {
        // 現在のピン状態を読み取る false ボタン押下　　true ボタン離す
        bool current_pin_state = gpio_get(buttons[i].gpio_pin);

        // デバウンス処理
        if (current_pin_state == buttons[i].current_state) {
            // 状態が変わっていなければカウンタをリセット
            buttons[i].debounce_counter = 0;
        } else {
            // 状態が変わっていればカウントをインクリメント
            buttons[i].debounce_counter++;
            if (buttons[i].debounce_counter >= DEBOUNCE_DELAY) {
                // 一定回数状態が変わっていたら状態を更新
                buttons[i].debounce_counter = 0;
                buttons[i].prev_state = buttons[i].current_state;
                buttons[i].current_state = current_pin_state;

                // 前回の状態が押された状態で、現在が離された状態ならフラグを立てる
                if (buttons[i].prev_state == false && buttons[i].current_state == true) {
                // if (buttons[i].prev_state == true && buttons[i].current_state == false) {
                    buttons[i].button_released_flag = true;
                
                    // デバッグ
                    // if(i == 0)
                    //     gpio_put(LED_PIN,1);
                    // else
                    //     gpio_put(LED_PIN,0);


                }
            }
        }
    }
}

/// @brief ボタンのフラグを取得する関数
/// @param button_id 何番目のボタンをチェックするかを指定する 0～MAX_BUTTONS-1
/// @return ボタンが放されたときにtrue
bool is_button_released(uint8_t button_id) {
    if (button_id < MAX_BUTTONS) {
        return buttons[button_id].button_released_flag;
    }
    return false;
}

/// @brief ボタンのフラグをクリアする関数
/// @param button_id 何番目のボタンをチェックするかを指定する 0～MAX_BUTTONS-1
void clear_button_released_flag(uint8_t button_id) {
    if (button_id < MAX_BUTTONS) {
        buttons[button_id].button_released_flag = false;
    }
}

/// @brief 車線番号対応のボタンが押されたどうかを取得する
/// @param line_no 車線番号　0 または　1
/// @return どちらかのボタンが離されたときにtrue
bool is_button_released_flag_lineno(uint8_t line_no){
    if(line_no == 0){
        // デバッグ
        if(buttons[0].button_released_flag || buttons[2].button_released_flag){
            gpio_put(LED_PIN,1);        // LED ON
        }

        // 車線1対応のボタンが離された
        return (buttons[0].button_released_flag || buttons[2].button_released_flag);
    } else {
        // デバッグ
        if(buttons[1].button_released_flag || buttons[3].button_released_flag){
            gpio_put(LED_PIN,0);        // LED OFF
        }        
        // 車線2対応のボタンが離された
        return (buttons[1].button_released_flag || buttons[3].button_released_flag);
    }
}

/// @brief 車線番号対応のボタンフラグをクリアする関数
/// @param line_no  車線番号　0 または　1
void clear_button_released_flag_lineno(uint8_t line_no) {
    if (line_no == 0) {
        // 車線1用ボタンフラグクリア
        buttons[0].button_released_flag = false;
        buttons[2].button_released_flag = false;
    } else {
        // 車線2用ボタンフラグクリア
        buttons[1].button_released_flag = false;
        buttons[3].button_released_flag = false;
    }
}

/*
int main() {
    stdio_init_all();
    init_buttons();

    while (true) {
        // 50ms間隔でボタン状態をチェック
        check_buttons();

        // 各ボタンの状態を確認・処理（例: ボタンが離された場合に表示）
        for (int i = 0; i < MAX_BUTTONS; i++) {
            if (is_button_released(i)) {
                printf("Button %d was released!\n", i);
                clear_button_released_flag(i); // フラグをクリア
            }
        }

        sleep_ms(50); // 50ms待機
    }

    return 0;
}

*/