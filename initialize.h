///////////////////////////////////////////////
// 初期化処理
// initialize.h
///////////////////////////////////////////////

#ifndef _INITIALIZE_H
#define _INITIALIZE_H

///@brief 初期化関数
void initializeSettings();

/// @brief SPIの初期化を行う。
void spi_initialize();

/// @brief GPIO初期化関数
/// GPIOの初期化を行う。
void gpio_initialize();

/// @brief ディスプレイ初期化
void display_initialize();

/// @brief タイトル表示関数
void showTitle();

/// @brief ATOSモード切替
void setAtosMode();

/// @brief core1イニシャル処理
/// core1を起動する
void core1_initialize();

#endif // _INITIALIZE_H

