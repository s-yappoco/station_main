///////////////////////////////////////////////
// ST7789 制御関数
// st7789.h
///////////////////////////////////////////////


#ifndef _ST7789_H
#define _ST7789_H

#include "pico/stdlib.h"
#include <stdio.h>

#define LCD_XSIZE (240)		// LCDサイズ
#define LCD_YSIZE (240)		// LCDサイズ

//===============
// Define Colors
//===============
#define LCD_RED_MAX 0x1f
#define LCD_GRN_MAX 0x3f
#define LCD_BLU_MAX 0x1f
#define LCD_RED_MIN 0x00
#define LCD_GRN_MIN 0x00
#define LCD_BLU_MIN 0x00
#define LCD_RED_MID 0x10
#define LCD_GRN_MID 0x20
#define LCD_BLU_MID 0x10
//
#define LCD_RED ((LCD_RED_MAX << 11) + (LCD_GRN_MIN << 5) + (LCD_BLU_MIN << 0))
#define LCD_GRN ((LCD_RED_MIN << 11) + (LCD_GRN_MAX << 5) + (LCD_BLU_MIN << 0))
#define LCD_BLU ((LCD_RED_MIN << 11) + (LCD_GRN_MIN << 5) + (LCD_BLU_MAX << 0))
#define LCD_BLK ((LCD_RED_MIN << 11) + (LCD_GRN_MIN << 5) + (LCD_BLU_MIN << 0))
#define LCD_WHT ((LCD_RED_MAX << 11) + (LCD_GRN_MAX << 5) + (LCD_BLU_MAX << 0))
#define LCD_YEL ((LCD_RED_MAX << 11) + (LCD_GRN_MAX << 5) + (LCD_BLU_MIN << 0))
#define LCD_CYN ((LCD_RED_MIN << 11) + (LCD_GRN_MAX << 5) + (LCD_BLU_MAX << 0))
#define LCD_MAG ((LCD_RED_MAX << 11) + (LCD_GRN_MIN << 5) + (LCD_BLU_MAX << 0))
#define LCD_GRY ((LCD_RED_MID << 11) + (LCD_GRN_MID << 5) + (LCD_BLU_MID << 0))

// 色番号定義
//#define OLED_color_GREEN	0x07E0
#define LCD_STATION_GREEN	0x27E4
#define LCD_STATION_ORANGE	0xFF00


//==============
// Prototypes
//==============

// 色データをデータ配列にスタックする
void stackColorData(uint16_t color);

// スタックされた色データをSPIで出力する
void sendColorData();

// printfSt7789の表示位置を文字数で設定する
void locateLcdPrintf(uint8_t x, uint8_t y);

// printfSt7789の表示色を設定する
void setColorLcdPrintf(uint16_t colorf, uint16_t colorb);

// SPI コマンド書き込み関数
void writeCommandSt7789(uint8_t cmd);

// SPI データ書き込み関数
void writeDataSt7789(uint8_t data);

// SPI 初期化関数
void initializeSt7789();

// ST7789 ウインドウアドレス設定関数
void setAddressWindowSt7789(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

// ピクセル書き込み関数
void draw1PixelSt7789(uint16_t x, uint16_t y, uint16_t color);

// 四角塗りつぶし関数
void fillRectangleSt7789(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

// 画面塗りつぶし関数
void fillScreenSt7789(uint16_t color);

// ピクセル色書き込み関数
// ウインドウアドレスを設定した後、必要な分だけピクセルデータを
// 連続で書き込む
void send1PixelColor(uint16_t color);

// ウインドウ回転関数
//回転角度（0, 90, 180, 270度）に応じて、適切な値をMADCTLレジスタに書き込みます。
// 引数 0:0度 1:90度 2:180度 3:270度
void setRotationSt7789(uint8_t rotation);

// LCD 文字表示関数(for printf)
void draw8x8Charx2(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bgcolor);


// ST7789用printf関数
void printfSt7789(const char *fmt, ...);

void drawStringSt7789(uint16_t x, uint16_t y, const char *buffer, uint16_t color, uint16_t bgcolor) ;


// 文字表示関数
void draw16x16Char(const uint16_t ch_num16x16[], uint32_t posx, uint32_t posy, uint32_t color_f, uint32_t color_b);
void draw24x24Char(const uint32_t ch_num24x24[], uint32_t posx, uint32_t posy, uint32_t color_f, uint32_t color_b);
void draw16x24Char(const uint16_t ch_num16x24[], uint32_t posx, uint32_t posy, uint32_t color_f, uint32_t color_b);
void draw16x16Charx2(const uint16_t ch_num16x16[], uint32_t posx, uint32_t posy, uint32_t color_f, uint32_t color_b);

// 16色パレット画像表示
void drawBMP_A(const uint16_t *color_plt16, const uint8_t *dot_data, uint16_t posx, uint16_t posy, uint16_t xsize, uint16_t ysize);
// 16色パレット画像表示　上下反転
void drawBMP_B(const uint16_t *color_plt16, const uint8_t *dot_data, uint16_t posx, uint16_t posy, uint16_t xsize, uint16_t ysize);



#endif // _ST7789_H
