///////////////////////////////////////////////
// 駅電光掲示板表示関連関数
// station_disp.c
///////////////////////////////////////////////

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdio.h>

#include "station_define.h"
#include "stname.h"
#include "st7789.h"
#include "station_disp.h"


/// @brief 駅名表示関数：
/// 指定した駅番号に従って駅名を表示する。24ドットフォント３文字表示を行う。
/// @param stationNumber 表示したい駅名番号
/// @param posy 表示したい座標位置（LCD絶対位置指定）
/// @param color_f 文字色
/// @param color_b 背景色
void drawStationName(uint16_t stationNumber, uint32_t posy, uint32_t color_f, uint32_t color_b)
{
	uint16_t i;					// 汎用レジスタ

	// 駅名の最初の配列指定
	i = stationNumber * 3 * 16;
	draw24x24Char(&STNAME_FONT24[i],	  7*24+24, posy, color_f, color_b);
	draw24x24Char(&STNAME_FONT24[i+24], 8*24+24, posy, color_f, color_b);
	draw24x24Char(&STNAME_FONT24[i+48], 9*24+24, posy, color_f, color_b);

}


/// @brief 電車種別（山手線・各駅停車・快速）表示関数
/// @param trainType 種別指定：山手線 0: 各駅停車 1: 快速 2
/// @param isJapanese 日本語か英語かを選択するフラグ trueで日本語表示
/// @param posy 表示したい座標位置（LCD絶対位置指定）
void drawTrainType(uint16_t trainType, bool isJapanese, uint32_t posy)
//
// isJapanese がtrueの場合は漢字で、falseの場合はアルファベットで表示する
// posy	表示したい座標位置（LCD絶対位置指定）
{
	uint16_t charnum;
	uint32_t color_f,color_b;

	// 表示文字色
	color_f = LCD_STATION_ORANGE;
	color_b = LCD_BLK;

	// フォントデータの先頭文字数の計算
	charnum = trainType * 3 * 2;	// 1種類あたり、3文字の日本語 英語1パターン

	// 英語だったら、後半3文字を表示
	if(isJapanese){
		charnum += 0;
	}else{
		charnum += 3;
	}

	// 文字表示
	draw24x24Char(&trainTypeFONT24[24*charnum],24+24*0,posy,color_f,color_b);
	draw24x24Char(&trainTypeFONT24[24*(charnum+1)],24+24*1,posy,color_f,color_b);
	draw24x24Char(&trainTypeFONT24[24*(charnum+2)],24+24*2,posy,color_f,color_b);

}

/// @brief 行き先駅表示関数：
/// 駅名を４文字で表示する
/// @param stationNumber 表示駅番号
/// @param isJapanese 日本語か英語かを選択するフラグ trueで日本語表示
/// @param posy 表示したい座標位置（LCD絶対位置指定）
void drawDestination(uint16_t stationNumber, bool isJapanese, uint32_t posy)
{
	uint16_t charnum;
	uint32_t color_f,color_b;

	// 表示文字色
	color_f = LCD_STATION_ORANGE;
	color_b = LCD_BLK;

	// 英語だったら、後半4文字を表示
	if(isJapanese){
		charnum = 0;
	}else{
		charnum = 4;
	}

	// 駅は0～9までで定義されている
	if (stationNumber > 7){
		stationNumber = 0;		// 強制的に大船とする
	}

	// 目的駅表示 4文字で表現
	// 1駅名あたり、4文字x2バージョンで8文字分のデータとなる
	draw24x24Char(&STNAME_FONT24[24*(stationNumber*8 + charnum + 0)],24+24*6+24*0,posy,color_f,color_b);
	draw24x24Char(&STNAME_FONT24[24*(stationNumber*8 + charnum + 1)],24+24*6+24*1,posy,color_f,color_b);
	draw24x24Char(&STNAME_FONT24[24*(stationNumber*8 + charnum + 2)],24+24*6+24*2,posy,color_f,color_b);
	draw24x24Char(&STNAME_FONT24[24*(stationNumber*8 + charnum + 3)],24+24*6+24*3,posy,color_f,color_b);
}


/// @brief 時刻or残り時間表示関数：
/// 電車か車での残り分数、もしくは到着予定時刻を表示する関数。
/// @param isDrawLeftTime 時刻を表示するか、残り分数を表示するか　tureで残り分数表示
/// @param time 出発時刻　hms構造体
/// @param min 残り分数
/// @param isJapanese 日本語か英語かを選択するフラグ trueで日本語表示
/// @param posy 表示したい座標位置（LCD絶対位置指定）
void drawTime(bool isDrawLeftTime,struct time_hms time, uint16_t min, bool isJapanese, uint32_t posy)
{
	uint32_t color_f = LCD_STATION_GREEN;
	uint32_t color_b = LCD_BLK;
	uint16_t x_dot = 3 * 24;			// 何ドット目から標示するか

	// 1から9間でしか表示しないようにガードをかける
	if (min > 9)
		min = 9;
	else if(min < 1)
		min = 1;
	
	if (isDrawLeftTime){
		if(isJapanese){
			// 残り分数標示(日本語表示)
			draw24x24Char(&MINUTE_LEFT_J[24*0],24+x_dot+24*0,posy,color_f,color_b);
			draw24x24Char(&MINUTE_LEFT_J[24*1],24+x_dot+24*1,posy,color_f,color_b);
			draw24x24Char(&MINUTE_LEFT_J[24*2],24+x_dot+24*2,posy,color_f,color_b);
			draw16x24Char(&MINUTE_NUM[min*24], 16+x_dot+19, posy, color_f, color_b);   
		}else{
			// 残り分数表示(英語表示)
			draw24x24Char(&MINUTE_LEFT_E[24*0],24+x_dot+24*0,posy,color_f,color_b);
			draw24x24Char(&MINUTE_LEFT_E[24*1],24+x_dot+24*1,posy,color_f,color_b);
			draw24x24Char(&MINUTE_LEFT_E[24*2],24+x_dot+24*2,posy,color_f,color_b);
			draw16x24Char(&MINUTE_NUM[min*24], 16+x_dot+0, posy, color_f, color_b);   
		}
	} else {
		uint16_t h1,h2,m1,m2;
		h1 = time.hh / 10;
		h2 = time.hh % 10;
		m1 = time.mm / 10;
		m2 = time.mm % 10;

		// 時刻表示
		if (h1 == 0){
			fillRectangleSt7789(x_dot, posy , 16, 24, LCD_BLK); // 時の10の位の最初の文字が0の時は表示しない 
		}else{
			draw16x24Char(&MINUTE_NUM_B[h1*24], 16+x_dot+0, posy, color_f, color_b);	// 時10の位表示
		}		
		draw16x24Char(&MINUTE_NUM_B[h2*24], 16+x_dot+16*1, posy, color_f, color_b);	// 時1の位表示
		draw16x24Char(&MINUTE_NUM_B[10*24], 16+x_dot+16*2, posy, color_f, color_b);	// 「：」表示 横は８ドット分だけ 以下座標を8ドットずらす
		draw16x24Char(&MINUTE_NUM_B[m1*24], 16+x_dot+16*2+8, posy, color_f, color_b);	// 分10の位表示
		draw16x24Char(&MINUTE_NUM_B[m2*24], 16+x_dot+16*3+8, posy, color_f, color_b);	// 時1の位表示
	}

}



/// @brief 24ドット文字スクロール表示関数：
/// 渡された文字列をスクロール表示する。
/// 文字列データは最初のワードに色情報＋最終文字フラグを付けた17ワードを１文字データとする。
/// 0x0000 白
/// 0x0001 緑
/// 0x0002 赤
/// 0x0003 橙
/// 0x0010 が付いていたら、点滅文字とする。
/// 0x0100 が付いていたら、最終文字とする。
/// @param char_line 表示する文字列。ポインタ渡し
/// @param posy 表示したい座標位置（LCD絶対位置指定）
/// @param start_frg 表示を開始するフラグ。true指定で最初から表示を行う。
/// @param scrl_pos スクロールポジション。どこまで表示が進んでいるかを指定する。
/// @return スクロールが終了したら1を返す
uint16_t drawScroll24x24Char(const uint32_t *char_line ,uint32_t posy, uint16_t start_frg, uint16_t *scrl_pos)
{
	extern	uint8_t		isBlinkOn;		// 文字列点滅用
	int32_t x;						// X軸位置
	uint32_t color;					// 565色データ変数
	uint32_t pixel;					// ピクセルデータ変数
	
	uint16_t char_num = 0;			// 表示文字番号（左から）
	
//	if ((*scrl_pos == 0) && (start_frg == 1))
	if (start_frg == 1)
		*scrl_pos = 1;				// スクロール開始
	
	if (*scrl_pos == 0)
		return 1;					// 文字表示していないことを示す。
	
	while(1) {
		// 文字表示位置設定
		x = char_num * 24 + LCD_XSIZE + 24 - *scrl_pos;
		
		if ((x > 0) && (x < LCD_XSIZE + 24))		// 表示可能エリアであれば文字表示
		{
			// 文字色指定
			pixel = char_line[char_num * 25] & 0x0003;	// 1文字あたり、25データある
			if (pixel == 0x0001)
				color = LCD_STATION_GREEN;
			else if (pixel == 0x0002)
				color = LCD_RED;
			else if (pixel == 0x0003)
				color = LCD_STATION_ORANGE;
			else
				color = LCD_WHT;
			
			// 点滅処理
			if ((char_line[char_num * 25] & 0x0010) && (isBlinkOn == 0))
				color = LCD_BLK;

		
			// 文字表示
        	draw24x24Char(&char_line[char_num * 25 + 1], x, posy, color, LCD_BLK);
        }

        // 最後の文字なら終了する。
		pixel = char_line[char_num * 25] & 0x0100;
		if ((pixel == 0x0100) ||  (char_num > 100))		// 最終文字 または100文字超
			break;
		
		// 次の文字へ移行
		char_num ++;
	}
	
	// １度に何ドットスクロールするか
	(*scrl_pos) += SCRLDOT;

	if (x < 1){
		*scrl_pos = 0;	// スクロールポジションを最初に戻す。
		return 1;		// スクロール文字が終了した。表示すべき文字列なし
	} else {
		return 0;		// まだ表示すべき文字が残っている
	}
}

/// @brief 線路地図を描画する。
/// @param posy 描画したい座標位置（LCD絶対位置指定）
void drawRailway(uint32_t posy)
{
	// 文字表示
	uint16_t x=0;
	for (x=0;x<3;x++){
		draw24x24Char(&RAILWAY_A[24*0], 24+24*0+x*24*3, posy, LCD_STATION_GREEN, LCD_BLK);
		draw24x24Char(&RAILWAY_A[24*1], 24+24*1+x*24*3, posy, LCD_STATION_GREEN, LCD_BLK);
		draw24x24Char(&RAILWAY_A[24*2], 24+24*2+x*24*3, posy, LCD_STATION_GREEN, LCD_BLK);

		drawBMP_A(&BMP_RAILWAY_B[0], &RAILWAY_B[0], 24*9 ,posy ,24 , 24);
	}

}

/// @brief 車両を描画する
/// @param posx 描画したい座標位置　横軸
/// @param posy 描画したい座標位置　縦軸
void drawTrain(uint32_t posx, uint32_t posy)
{
	uint16_t ysize = 8;

	uint32_t data[8];			// SPI送信データ
	uint16_t xsize = 24;			// フォントのX軸方法のサイズ
	uint16_t x, y, i;				// 汎用レジスタ
	uint32_t pixel;			// ピクセルチェック用レジスタ
	uint32_t bitmask;		// ビットマスク位置
	uint32_t color;			// 565色データ変数
	//
	uint32_t col0, col1, row0, row1;	// LCD書き込み位置保存用
	
	// マスクビット位置
	bitmask = 1 << (xsize-1);

	//X軸開始位置
	col0 = posx;
	//x軸終了位置
	col1 = posx + xsize - 1;
	//Y軸開始位置設定
	row0 = posy;
	//Y軸終了位置設定
	row1 = posy + ysize - 1;

	// col,rowをLCDへ送る
	setAddressWindowSt7789(col0, row0, col1, row1);

	// フォントデータ描画
	// フォントデータは引数で渡されている *ch_num24x24
	i=0;
	for (y = 0; y < ysize; y++)
	{
		pixel = TRAIN_MODEL[i];					// フォントデータ1行ロード
		for (x = 0; x < xsize; x++)
		{
			color = (pixel & bitmask)? LCD_STATION_ORANGE : LCD_BLK;
			//LCD_Send_Pixel_s(color);	// ドット描画
			send1PixelColor(color);	// ドット描画
			pixel = pixel << 1;	// 次のドットに移行
		}
		i++;			// インクリメント 次の行
	}
}

/// @brief マップ情報表示関数：
/// 線路を描画し、車両位置情報に従って車両を描画する。
/// @param posy 描画したい座標位置　縦軸
/// @param no1train １両目車両の到着までの分数 ６分以内から表示される
/// @param no2train ２両目車両の到着までの分数 ６分以内から表示される
void drawMapInfo(uint32_t posy,uint16_t no1train, uint16_t no2train)
{
	// レール表示
	drawRailway(posy);

	uint16_t x;

	// 1車両目
	if (no1train >= 1 && no1train <= 6){
		x = (6 - no1train) * 36;		// 1分あたり、24dot x 1.5char分進む
		drawTrain(x,posy);
	}

	// 2車両目
	if (no2train >= 1 && no2train <= 6){
		x = (6 - no2train) * 36;		// 1分あたり、24dot x 1.5char分進む
		drawTrain(x,posy);
	}
}

/// @brief 電車接近情報表示「電車がまいります」「Train approaching」
/// @param posy 描画したい座標位置　縦軸
/// @param isJapanese 日本語か英語かを選択するフラグ trueで日本語表示
void drawApproaching(uint32_t posy,bool isJapanese)
{
	uint32_t color_f;
	uint16_t i;
	extern	uint8_t		isBlinkOn;		// 文字列点滅用

	//点滅フラグ
	if (isBlinkOn)
		color_f = LCD_RED;
	else
		color_f = LCD_BLK;

	if(isJapanese){
		// 日本語表示「電車がまいります」8文字表示
		for (i = 0; i < 8; i++){
			draw24x24Char(&SIGNBOARD24APP_J[24*i], 24+24*i, posy, color_f, LCD_BLK);
		}
	} else {
		// 英語表示「Train approaching」7文字+空白で8文字にする
		for (i = 0; i < 8; i++){
			draw24x24Char(&SIGNBOARD24APP_E[24*i], 24+24*i, posy, color_f, LCD_BLK);
		}
	}
}
