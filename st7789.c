///////////////////////////////////////////////
// ST7789 制御関数
// st7789.c
///////////////////////////////////////////////

#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "station_define.h"
#include "st7789.h"

#include <stdio.h>
#include <stdarg.h> // 変数引数を扱うために必要
#include <string.h>

#include "font.h" // フォントデータのインクルード

//=============
// Globals
//=============

uint8_t gSpi_colordata[1200];	// 色データスタック用配列
uint16_t gSpidatanum = 0;		// スタックした色データの配列位置保存用

uint32_t gLCD_printf_ColorF = LCD_WHT;	// 液晶printf文字色
uint32_t gLCD_printf_ColorB = LCD_BLK;	// 液晶printf文字背景色
uint32_t gLCD_printf_PosX = 0;			// 液晶printf文字表示位置X（ドット指定）
uint32_t gLCD_printf_PosY = 0;			// 液晶printf文字表示位置Y（ドット指定）

uint16_t offsetx = 0;					// 画面回転のためのオフセットX軸
uint16_t offsety = 0;					// 画面回転のためのオフセットy軸

// 解説
// ST7780は横320x縦240ピクセルのデータを管理しているが、
// 実際の液晶は240x240しかないため、回転表示させると、画面外になってしまう。
// これを補正するために、オフセット値を設定する。

/// @brief 色データをデータ配列にスタックする。：
/// 処理時間を短くするため、ドットの色データをグローバル変数にスタックする。
/// @param color 色データ
void stackColorData(uint16_t color) {
    gSpi_colordata[gSpidatanum] = (color >> 8);
    gSpidatanum++;
    gSpi_colordata[gSpidatanum] = (color & 0xFF);
    gSpidatanum++;
}

/// @brief スタックされた色データをSPIで出力する。
void sendColorData(){
    if (gSpidatanum == 0)
        return;
    else {
        gpio_put(PIN_DC, 1); // DC high, dataモード設定

		// SPIデータブロッキング転送
        spi_write_blocking(SPI_PORT, &gSpi_colordata[0], gSpidatanum);
		
		// データ数クリア（配列位置保存用をクリア）
        gSpidatanum = 0;
    }
}

// printfSt7789の表示位置を文字数で設定する

/// @brief 液晶printf文字表示位置設定関数
/// @param x 表示文字位置（文字数換算）
/// @param y 表示文字位置（文字数換算）
void locateLcdPrintf(uint8_t x, uint8_t y){
    gLCD_printf_PosX = x * FONT_WIDTH * 2;
    gLCD_printf_PosY = y * FONT_HEIGHT * 2;
}

// printfSt7789の表示色を設定する

/// @brief 液晶printfの表示色設定関数
/// @param colorf 液晶printf文字色
/// @param colorb 液晶printf文字背景色
void setColorLcdPrintf(uint16_t colorf, uint16_t colorb){
    gLCD_printf_ColorF = colorf;
    gLCD_printf_ColorB = colorb;
}

/// @brief SPIコマンド書き込み関数
/// @param cmd コマンドデータ
void writeCommandSt7789(uint8_t cmd) {
    gpio_put(PIN_DC, 0); // DC low, commandモード設定
    spi_write_blocking(SPI_PORT, &cmd, 1);
}

/// @brief SPI データ書き込み関数
/// @param data データモード用データ
void writeDataSt7789(uint8_t data) {
    gpio_put(PIN_DC, 1); // DC high, dataモード設定
    spi_write_blocking(SPI_PORT, &data, 1);
}

/// @brief 液晶コントローラST7789初期化関数
void initializeSt7789() {
    // Reset the display
    gpio_put(PIN_RST, 1);
    sleep_ms(50);
    gpio_put(PIN_RST, 0);
    sleep_ms(50);
    gpio_put(PIN_RST, 1);
    sleep_ms(50);

    // Send initialization commands
    writeCommandSt7789(0x11); // Sleep out
    sleep_ms(120);
    writeCommandSt7789(0x36); // MADCTL: Memory data access control
    writeDataSt7789(0x00);
    writeCommandSt7789(0x3A); // COLMOD: Interface pixel format
    writeDataSt7789(0x05); // 16 bits/pixel
    
    writeCommandSt7789(0xB2); // PORCTRL: Porch control
    writeDataSt7789(0x0C);
    writeDataSt7789(0x0C);
    writeDataSt7789(0x00);
    writeDataSt7789(0x33);
    writeDataSt7789(0x33);
    
    writeCommandSt7789(0xB7); // GCTRL: Gate control
    writeDataSt7789(0x35);
    
    writeCommandSt7789(0xBB); // VCOMS: VCOM setting
    writeDataSt7789(0x19);
    
    writeCommandSt7789(0xC0); // LCMCTRL: LCM control
    writeDataSt7789(0x2C);
    
    writeCommandSt7789(0xC2); // VDVVRHEN: VDV and VRH command enable
    writeDataSt7789(0x01);
    
    writeCommandSt7789(0xC3); // VRHS: VRH set
    writeDataSt7789(0x12);
    
    writeCommandSt7789(0xC4); // VDVS: VDV set
    writeDataSt7789(0x20);
    
    writeCommandSt7789(0xC6); // FRCTRL2: Frame rate control in normal mode
    writeDataSt7789(0x0F);
    
    writeCommandSt7789(0xD0); // PWCTRL1: Power control
    writeDataSt7789(0xA4);
    writeDataSt7789(0xA1);
    
    writeCommandSt7789(0xE0); // PVGAMCTRL: Positive voltage gamma control
    writeDataSt7789(0xD0);
    writeDataSt7789(0x04);
    writeDataSt7789(0x0D);
    writeDataSt7789(0x11);
    writeDataSt7789(0x13);
    writeDataSt7789(0x2B);
    writeDataSt7789(0x3F);
    writeDataSt7789(0x54);
    writeDataSt7789(0x4C);
    writeDataSt7789(0x18);
    writeDataSt7789(0x0D);
    writeDataSt7789(0x0B);
    writeDataSt7789(0x1F);
    writeDataSt7789(0x23);
    
    writeCommandSt7789(0xE1); // NVGAMCTRL: Negative voltage gamma control
    writeDataSt7789(0xD0);
    writeDataSt7789(0x04);
    writeDataSt7789(0x0C);
    writeDataSt7789(0x11);
    writeDataSt7789(0x13);
    writeDataSt7789(0x2C);
    writeDataSt7789(0x3F);
    writeDataSt7789(0x44);
    writeDataSt7789(0x51);
    writeDataSt7789(0x2F);
    writeDataSt7789(0x1F);
    writeDataSt7789(0x1F);
    writeDataSt7789(0x20);
    writeDataSt7789(0x23);
    
    writeCommandSt7789(0x21); // INVON: Display inversion on
    writeCommandSt7789(0x29); // Display on
}

/// @brief ST7789 液晶ウィンドウアドレス設定関数：
/// 液晶を書き換えるエリアを設定する。SPIで送信する。
/// @param x0 左上のX座標
/// @param y0 左上のY座標
/// @param x1 右下のX座標
/// @param y1 右下のY座標
void setAddressWindowSt7789(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	// 回転のためのオフセット値を足し算
	x0 += offsetx;
	y0 += offsety;

	x1 += offsetx;
	y1 += offsety;

    writeCommandSt7789(0x2A); // CASET: Column address set
    writeDataSt7789(x0 >> 8);
    writeDataSt7789(x0 & 0xFF);
    writeDataSt7789(x1 >> 8);
    writeDataSt7789(x1 & 0xFF);

    writeCommandSt7789(0x2B); // RASET: Row address set
    writeDataSt7789(y0 >> 8);
    writeDataSt7789(y0 & 0xFF);
    writeDataSt7789(y1 >> 8);
    writeDataSt7789(y1 & 0xFF);

    writeCommandSt7789(0x2C); // RAMWR: Memory write
}

/// @brief 1ピクセル描画関数
/// @param x 書き換えるX座標
/// @param y 書き換えるY座標
/// @param color 書き込む色データ
void draw1PixelSt7789(uint16_t x, uint16_t y, uint16_t color) {
	// 書き込むウィンドウを設定する。1ピクセル分
    setAddressWindowSt7789(x, y, x, y);

	// 色データを送る。２バイト分。
    writeDataSt7789(color >> 8);
    writeDataSt7789(color & 0xFF);
}

// 四角塗りつぶし関数

/// @brief 四角塗りつぶし関数：
/// 指定した四角形の色を指定色で塗りつぶす。
/// @param x 左上のX座標
/// @param y 左上のY座標
/// @param w 四角形の横幅
/// @param h 四角形の縦高さ
/// @param color 塗りつぶす色データ
void fillRectangleSt7789(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    setAddressWindowSt7789(x, y, x + w - 1, y + h - 1);
    uint8_t data[2] = {color >> 8, color & 0xFF};
    for (uint32_t i = 0; i < w * h; i++) {
        writeDataSt7789(data[0]);
        writeDataSt7789(data[1]);
    }
}

/// @brief 画面塗りつぶし関数
/// @param color 塗りつぶす色データ
void fillScreenSt7789(uint16_t color) {
	// 液晶サイズ240x240ピクセル
    fillRectangleSt7789(0, 0, LCD_XSIZE, LCD_YSIZE, color);
}

/// @brief ピクセル色書き込み関数：
/// 色データを１ピクセル分書き込む関数２バイトに分割して、ブロック転送する。
/// @param color 書き込む色データ
void send1PixelColor(uint16_t color){
    uint8_t data[2];
    data[0] = (color >> 8);
    data[1] = (color & 0xFF);

    gpio_put(PIN_DC, 1); // DC high, data
    spi_write_blocking(SPI_PORT, &data[0], 2);

    //writeDataSt7789(color >> 8);
    //writeDataSt7789(color & 0xFF);
}

/// @brief ST7789 液晶画面回転関数：
/// 回転角度（0, 90, 180, 270度）に応じて、適切な値をMADCTLレジスタに書き込む。
/// @param rotation 設定角度　0:0度 1:90度 2:180度 3:270度
void setRotationSt7789(uint8_t rotation) {
    writeCommandSt7789(0x36); // MADCTL: Memory data access control
    switch (rotation) {
        case 0:
            writeDataSt7789(0x00); // No rotation
			offsetx = 0;
			offsety = 0;
            break;
        case 1:
            writeDataSt7789(0x60); // 90 degrees
			offsetx = 0;
			offsety = 0;
            break;
        case 2:
            writeDataSt7789(0xC0); // 180 degrees
 			offsetx = 0;
			offsety = 320-LCD_YSIZE;
			break;
        case 3:
            writeDataSt7789(0xA0); // 270 degrees
			offsetx = 320-LCD_XSIZE;
			offsety = 0;
			break;
        default:
            writeDataSt7789(0x00); // Default: No rotation
			offsetx = 0;
			offsety = 0;
            break;
    }
}

/// @brief LCD 文字表示関数(for printf)：
/// ASCII 32~126に含まれる文字を表示。それ以外は@にて表示。
/// 8ドットフォントは小さすぎるので、4ピクセル1ドットで表示する。
/// @param x 表示するX座標
/// @param y 表示するy座標
/// @param c 表示する文字コード(ASCII)
/// @param color 文字色
/// @param bgcolor 背景色
void draw8x8Charx2(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bgcolor) {
    // LFコードは表示しない
    if (c == 0x0a) return;

 	// ASCII 32~126に含まれる文字以外は @ で表示
    if (c < 32 || c > 126) {
	    c = 64;
    }
    const uint8_t *bitmap = font_data[c - 32]; // フォントデータ参照
    
    // ウインドウ設定
    setAddressWindowSt7789(x, y, x + FONT_WIDTH * 2 - 1, y + FONT_HEIGHT * 2 - 1) ;
    for (int8_t i = 0; i < FONT_HEIGHT; i++) {
	uint8_t line = bitmap[i];
	uint8_t line2 = bitmap[i];
        for (int8_t j = 0; j < FONT_WIDTH; j++) {
		    if (line & 0x80) {
                stackColorData(color);      // 色データを配列にスタック
                stackColorData(color);      // 色データを配列にスタック
		    } else {  
                stackColorData(bgcolor);    // 色データを配列にスタック
                stackColorData(bgcolor);    // 色データを配列にスタック
		    }
		    line <<= 1;	// 左シフト
	    }
        for (int8_t j = 0; j < FONT_WIDTH; j++) {
		    if (line2 & 0x80) {
                stackColorData(color);      // 色データを配列にスタック
                stackColorData(color);      // 色データを配列にスタック
		    } else {  
                stackColorData(bgcolor);    // 色データを配列にスタック
                stackColorData(bgcolor);    // 色データを配列にスタック
		    }
		    line2 <<= 1;	// 左シフト
	    }
    }
    // スタックされた色データをSPIでブロック転送する
    sendColorData();
}

/// @brief ST7789用printf関数：
/// 半角英数字記号のみ対応。 \nなどの制御コードは非対応。usage:printfSt7789("Hello, %s!", "World");
/// @param fmt 文字列。可変長引数に対応する。通常のprintfと同様のフォーマット指定を行う。
/// @param  
void printfSt7789(const char *fmt, ...) {
    char buffer[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    drawStringSt7789(gLCD_printf_PosX, gLCD_printf_PosY, buffer, gLCD_printf_ColorF, gLCD_printf_ColorB);
}

/// @brief 文字列表示関数：
/// 指定された文字列を表示する。
/// @param x 表示するX座標
/// @param y 表示するY座標
/// @param str 文字列データ
/// @param color 文字色
/// @param bgcolor 背景色
void drawStringSt7789(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bgcolor) {
    
    uint16_t i = 0;
    uint16_t str_length = strlen(str);
    for(i = 0; i < str_length; i++){
        draw8x8Charx2(x, y, str[i], color, bgcolor);
        x+=(FONT_WIDTH * 2);
    }
}


// 文字表示関数
void draw16x16Char(const uint16_t ch_num16x16[], uint32_t posx, uint32_t posy, uint32_t color_f, uint32_t color_b)
{
//座標は0-127まで表示される
//X座標は16から128-16までなら正しく表示され、それ以外は欠けます。
	uint32_t data[8];			// SPI送信データ
	uint16_t xsize;			// フォントのX軸方法のサイズ
	uint16_t left_loss_size = 16;		// 左欠時のX軸方法のサイズ
	uint16_t x, y;				// 汎用レジスタ
	uint16_t pixel;			// ピクセルチェック用レジスタ
	uint32_t color;			// 565色データ変数
	//
	uint32_t col0, col1, row0, row1;	// LCD書き込み位置保存用

	// X方向、Y軸方向がサイズアウトしていたら描画しない
	if ((posx > 143) || (posy > 112)){
		return;
	}
	
	//X軸開始位置設定
	if (posx >= 16){
		col0 = posx - 16;		//  16と設定したら  0から 15まで 16ビット描画
								//  15と設定したら  0から 14まで 15ビット描画
								//   1と設定したら  0から  0まで  1ビット描画
								// 128と設定したら112から127まで 16ビット描画
								// 129と設定したら113から127まで 15ビット描画
								// 143と設定したら127から127まで  1ビット描画
		xsize = 16;				// X軸方向サイズを保存
	}else{
		col0 = 0;					// 値が小さければ左側が欠ける
		xsize = posx;				// X軸方向サイズを保存
		left_loss_size = posx;
	}

	//X軸終了位置設定
	if (posx <= 128){			// 
		col1 = posx - 1;		//  16と設定したら 15まで描画
								//   1と設定したら  0まで描画
								// 128と設定したら127まで描画
								// 129と設定したら127まで描画
								// 143と設定したら127まで描画
	}else{
		col1 = 127;					// 値が大きければ右側が欠ける
		xsize = 144 - posx;			// X軸方向サイズを保存
	}
	//Y軸開始位置設定
	row0 = posy;
	//Y軸終了位置設定
	row1 = posy + 16 - 1;

	// col,rowをLCDへ送る
	setAddressWindowSt7789(col0, row0, col1, row1);

	// フォントデータ描画
	// フォントデータは引数で渡されている *ch_num16x16
	for (y = 0; y < 16; y++)
	{
		pixel = *ch_num16x16;					// フォントデータ1行ロード
		pixel = pixel << (16 - left_loss_size);	// 左側欠け対応

		for (x = 0; x < xsize; x++)
		{
			color = (pixel & 0x8000)? color_f : color_b;
			//LCD_Send_Pixel_s(color);	// ドット描画
			send1PixelColor(color);	// ドット描画
			pixel = pixel << 1;	// 次のドットに移行
		}
		ch_num16x16++;			// インクリメント 次の行
	}
}

// 文字表示関数
void draw16x16Charx2(const uint16_t ch_num16x16[], uint32_t posx, uint32_t posy, uint32_t color_f, uint32_t color_b)
{
//座標は0-119まで表示される
//X座標は16から120-16までなら正しく表示され、それ以外は欠けます。
	uint32_t data[8];			// SPI送信データ
	uint16_t xsize;			// フォントのX軸方法のサイズ
	uint16_t left_loss_size = 16;		// 左欠時のX軸方法のサイズ
	uint16_t x, y;				// 汎用レジスタ
	uint16_t pixel;			// ピクセルチェック用レジスタ
	uint32_t color;			// 565色データ変数
	//
	uint32_t col0, col1, row0, row1;	// LCD書き込み位置保存用

	// X方向、Y軸方向がサイズアウトしていたら描画しない
	if ((posx > 135) || (posy > 104)){
		return;
	}
	
	//X軸開始位置設定
	if (posx >= 16){
		col0 = posx - 16;		//  16と設定したら  0から 15まで 16ビット描画
								//  15と設定したら  0から 14まで 15ビット描画
								//   1と設定したら  0から  0まで  1ビット描画
								// 128と設定したら112から127まで 16ビット描画
								// 129と設定したら113から127まで 15ビット描画
								// 143と設定したら127から127まで  1ビット描画
		xsize = 16;				// X軸方向サイズを保存
	}else{
		col0 = 0;					// 値が小さければ左側が欠ける
		xsize = posx;				// X軸方向サイズを保存
		left_loss_size = posx;
	}

	//X軸終了位置設定
	if (posx <= 120){			// 
		col1 = posx - 1;		//  16と設定したら 15まで描画
								//   1と設定したら  0まで描画
								// 128と設定したら127まで描画
								// 129と設定したら127まで描画
								// 143と設定したら127まで描画
	}else{
		col1 = 119;					// 値が大きければ右側が欠ける
		xsize = 136 - posx;			// X軸方向サイズを保存
	}
	//Y軸開始位置設定
	row0 = posy;
	//Y軸終了位置設定
	row1 = posy + 16 - 1;

	// col,rowをLCDへ送る
	setAddressWindowSt7789(col0 * 2, row0 * 2, col1*2 + 1, row1*2 + 1);

	// フォントデータ描画
	// フォントデータは引数で渡されている *ch_num16x16
	for (y = 0; y < 16; y++)
	{
		//同じものを2列書きます
		pixel = *ch_num16x16;					// フォントデータ1行ロード
		pixel = pixel << (16 - left_loss_size);	// 左側欠け対応

		for (x = 0; x < xsize; x++)
		{
			color = (pixel & 0x8000)? color_f : color_b;
			//LCD_Send_Pixel_s(color);	// ドット描画
			send1PixelColor(color);	// ドット描画
			send1PixelColor(color);	// ドット描画	2ドット目
			pixel = pixel << 1;	// 次のドットに移行
		}
		
		// 同じものを2列書きます
		pixel = *ch_num16x16;					// フォントデータ1行ロード
		pixel = pixel << (16 - left_loss_size);	// 左側欠け対応

		for (x = 0; x < xsize; x++)
		{
			color = (pixel & 0x8000)? color_f : color_b;
			//LCD_Send_Pixel_s(color);	// ドット描画
			send1PixelColor(color);	// ドット描画
			send1PixelColor(color);	// ドット描画	2ドット目
			pixel = pixel << 1;	// 次のドットに移行
		}
		ch_num16x16++;			// インクリメント 次の行
	}
}

/// @brief 24ピクセル文字表示関数
/// @param ch_num24x24 表示文字フォントデータ
/// @param posx 表示するX座標(24から240までなら文字全体が表示される。0から23までは左側が欠ける。240-23以上であれば右側が欠ける。)
/// @param posy 表示するY座標
/// @param color_f 文字色
/// @param color_b 背景色
void draw24x24Char(const uint32_t ch_num24x24[], uint32_t posx, uint32_t posy, uint32_t color_f, uint32_t color_b)
{
//座標は0-127まで表示される
//X座標は16から128-16までなら正しく表示され、それ以外は欠けます。

	uint16_t fontsize =24;				// フォントピクセルサイズ

	uint32_t data[8];					// SPI送信データ
	uint16_t xsize;						// フォントのX軸方法のサイズ
	uint16_t left_loss_size = fontsize;	// 左欠時のX軸方法のサイズ
	uint16_t x, y;						// 汎用レジスタ
	uint32_t pixel;						// ピクセルチェック用レジスタ
	uint32_t bitmask;					// ビットマスク位置
	uint32_t color;						// 565色データ変数
	
	uint32_t col0, col1, row0, row1;	// LCD書き込み位置保存用
	
	// マスクビット位置
	bitmask = 1 << (fontsize-1);
	

	// X方向、Y軸方向がサイズアウトしていたら描画しない
	if ((posx > LCD_XSIZE + fontsize - 1) || (posy > LCD_YSIZE - fontsize)){
		return;
	}
	
	//X軸開始位置設定
	if (posx >= fontsize){
		col0 = posx - fontsize;	//  24と設定したら  0から 23まで 24ビット描画
								//  23と設定したら  0から 22まで 23ビット描画
								//   1と設定したら  0から  0まで  1ビット描画
								// 240と設定したら216から239まで 24ビット描画
								// 241と設定したら217から239まで 23ビット描画
								// 263と設定したら239から239まで  1ビット描画
		xsize = fontsize;		// X軸方向サイズを保存
	}else{
		col0 = 0;				// 値が小さければ左側が欠ける
		xsize = posx;			// X軸方向サイズを保存
		left_loss_size = posx;
	}

	//X軸終了位置設定
	if (posx <= LCD_XSIZE){		// 
		col1 = posx - 1;		//  24と設定したら 23まで描画
								//   1と設定したら  0まで描画
								// 240と設定したら239まで描画
								// 241と設定したら239まで描画
								// 263と設定したら239まで描画
	}else{
		col1 = LCD_XSIZE - 1;					// 値が大きければ右側が欠ける
		xsize = LCD_XSIZE + fontsize - posx;	// X軸方向サイズを保存
	}
	//Y軸開始位置設定
	row0 = posy;
	//Y軸終了位置設定
	row1 = posy + fontsize - 1;

	// col,rowをLCDへ送る
	setAddressWindowSt7789(col0, row0, col1, row1);

	// フォントデータ描画
	// フォントデータは引数で渡されている *ch_num24x24
    //printf("xsize=%d\n",xsize);
	for (y = 0; y < fontsize; y++)
	{
		pixel = *ch_num24x24;					// フォントデータ1行ロード
		pixel = pixel << (fontsize - left_loss_size);	// 左側欠け対応

		for (x = 0; x < xsize; x++)
		{
			color = (pixel & bitmask)? color_f : color_b;
			//send1PixelColor(color);	// ドット描画
			stackColorData(color);			// 高速処理のためスタックする
			pixel = pixel << 1;	// 次のドットに移行
		}
		ch_num24x24++;			// インクリメント 次の行
	}

	// スタックした色データをブロック転送する。
	sendColorData();
}

/// @brief 16x24フォンとデータ表示関数（縦長文字表示。時刻用数値フォント表示用）
/// @param ch_num16x24 フォントデータ
/// @param posx 表示するX座標(24から240までなら文字全体が表示される。0から23までは左側が欠ける。240-23以上であれば右側が欠ける。)
/// @param posy 表示するY座標
/// @param color_f 文字色
/// @param color_b 背景色
void draw16x24Char(const uint16_t ch_num16x24[], uint32_t posx, uint32_t posy, uint32_t color_f, uint32_t color_b)
{
	uint16_t fontsizex =16;
	uint16_t fontsizey =24;
	uint32_t data[8];		// SPI送信データ
	uint16_t xsize;			// フォントのX軸方法のサイズ
	uint16_t left_loss_size = fontsizex;		// 左欠時のX軸方法のサイズ
	uint16_t x, y;				// 汎用レジスタ
	uint16_t pixel;			// ピクセルチェック用レジスタ
	uint16_t bitmask;		// ビットマスク位置
	uint32_t color;			// 565色データ変数
	//
	uint32_t col0, col1, row0, row1;	// LCD書き込み位置保存用
	
	// マスクビット位置
	bitmask = 1 << (fontsizex-1);
	

	// X方向、Y軸方向がサイズアウトしていたら描画しない
	if ((posx > LCD_XSIZE + fontsizex - 1) || (posy > LCD_YSIZE - fontsizey)){
		return;
	}
	
	//X軸開始位置設定
	if (posx >= fontsizex){
		col0 = posx - fontsizex;//  16と設定したら  0から 15まで 16ビット描画
								//  15と設定したら  0から 14まで 15ビット描画
								//   1と設定したら  0から  0まで  1ビット描画
								// 240と設定したら224から239まで 16ビット描画
								// 241と設定したら225から239まで 15ビット描画
								// 256と設定したら239から239まで  1ビット描画
		xsize = fontsizex;		// X軸方向サイズを保存
	}else{
		col0 = 0;					// 値が小さければ左側が欠ける
		xsize = posx;				// X軸方向サイズを保存
		left_loss_size = posx;
	}

	//X軸終了位置設定
	if (posx <= LCD_XSIZE){			// 
		col1 = posx - 1;		//  16と設定したら 15まで描画
								//   1と設定したら  0まで描画
								// 240と設定したら239まで描画
								// 241と設定したら239まで描画
								// 256と設定したら239まで描画
	}else{
		col1 = LCD_XSIZE - 1;					// 値が大きければ右側が欠ける
		xsize = LCD_XSIZE + fontsizex - posx;	// X軸方向サイズを保存
	}
	//Y軸開始位置設定
	row0 = posy;
	//Y軸終了位置設定
	row1 = posy + fontsizey - 1;

	// col,rowをLCDへ送る
	setAddressWindowSt7789(col0, row0, col1, row1);

	// フォントデータ描画
	// フォントデータは引数で渡されている *ch_num24x24
    //printf("xsize=%d\n",xsize);
	for (y = 0; y < fontsizey; y++)
	{
		pixel = *ch_num16x24;					// フォントデータ1行ロード
		pixel = pixel << (fontsizex - left_loss_size);	// 左側欠け対応

		for (x = 0; x < xsize; x++)
		{
			color = (pixel & bitmask)? color_f : color_b;
			send1PixelColor(color);	// ドット描画
			pixel = pixel << 1;	// 次のドットに移行
		}
		ch_num16x24++;			// インクリメント 次の行
	}
}


/// @brief 16色パレット画像表示：
/// 16色bmpデータ表示用
/// @param color_plt16 カラーパレット16色分
/// @param dot_data ピクチャーデータ（左上から右下への連続データ)
/// @param posx 描画X座標
/// @param posy 描画Y座標
/// @param xsize 描画幅サイズ
/// @param ysize 描画高さサイズ
void drawBMP_A(const uint16_t *color_plt16, const uint8_t *dot_data, uint16_t posx, uint16_t posy, uint16_t xsize, uint16_t ysize)
{
	uint32_t oled[8];
	uint32_t data_size;
	uint32_t pixel;
	uint32_t color;
	uint32_t i;
	//
	uint32_t col0, col1, row0, row1;

	// 表示位置とサイズから座標を計算
	col0 = posx;
	col1 = posx + xsize - 1;
	row0 = posy; 
	row1 = posy + ysize - 1;
	
	// 座標データをLCDへ送信
	setAddressWindowSt7789(col0, row0, col1, row1);
	
    // ピクセルデータ送付 1バイトに2ピクセル格納されている
    data_size = xsize * ysize / 2;
    for (i = 0; i <data_size; i++)
    {
            pixel = dot_data[i];
            pixel = (pixel >> 4) & 0x0F;
            color = color_plt16[pixel];

			send1PixelColor(color);
            
			pixel = dot_data[i];
            pixel = pixel & 0x0F;
			color = color_plt16[pixel];
			
			send1PixelColor(color);
	}
}

/// @brief 16色パレット画像表示(上下反転バージョン）：
/// 16色bmpデータ表示用。bmpファイルは左下から右上へのデータであるため、そのデータをそのまま表示できるように下から１ラインずつ表示する。
/// @param color_plt16 カラーパレット16色分
/// @param dot_data ピクチャーデータ（左上から右下への連続データ)
/// @param posx 描画X座標
/// @param posy 描画Y座標
/// @param xsize 描画幅サイズ
/// @param ysize 描画高さサイズ
void drawBMP_B(const uint16_t *color_plt16, const uint8_t *dot_data, uint16_t posx, uint16_t posy, uint16_t xsize, uint16_t ysize)
{
	uint32_t oled[8];
	uint32_t data_size;
	uint32_t pixel;
	uint32_t color;
	uint32_t i,j;
	//
	uint32_t col0, col1, row0, row1;

	uint32_t y;
	uint32_t num;		// 配列の要素変数
	// 表示位置とサイズから座標を計算
	col0 = posx;
	col1 = posx + xsize - 1;
	row0 = posy; 
	row1 = posy + ysize - 1;
	
	// bmpデータフォーマットは左下から右上に向かったデータであるため、下の列から１ラインずつ表示する
	num = 0;		// 配列要素数初期化

	for (y = row1, j = 0; j < ysize; y--, j++){
		// 座標データをLCDへ送信
		//printf("y = %4d J = %4d ysize = %4d row0 = %4d\n",y,j,ysize,row0);
		setAddressWindowSt7789(col0, y, col1, y);
		
		// １列分のデータサイズを計算 1バイトに2ピクセル格納されている
		data_size = xsize / 2;

		// １列分のピクセル色情報を書き込む
		for (i = 0; i < data_size; i++, num++){
            pixel = dot_data[num];
            pixel = (pixel >> 4) & 0x0F;
            color = color_plt16[pixel];

			send1PixelColor(color);
            
			pixel = dot_data[num];
            pixel = pixel & 0x0F;
			color = color_plt16[pixel];
			
			send1PixelColor(color);
		}		
	}
}