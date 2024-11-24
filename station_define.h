///////////////////////////////////////////////
// メイン定義ファイル
// station_define.h
///////////////////////////////////////////////

#ifndef _STATION_DEFINE_H
#define _STATION_DEFINE_H

#include "pico/stdlib.h"
#include <stdio.h>

// ラズパイピコピン設定
#define PIN_RST			  (12)	//ディスプレイのRST	GP12(ピン16)
#define PIN_DC			  (8)		//ディスプレイのDC	GP8 (ピン11)
#define PIN_CS			  (5)		//ディスプレイのCS  GP5	(ピン7)液晶非対応1
#define PIN_SPI_SCK		(2)		//SPI SCK (Clock)	GP2 (ピン4)
#define PIN_SPI_MOSI	(3)		//SPI TX (MOSI)		GP3 (ピン5)

#define PIN_APPBTN1		(0)		//ボタン1(1番線接近情報) GP0 (ピン0)
#define PIN_APPBTN2		(1)		//ボタン2(2番線接近情報) GP1（ピン1)

// ラズパイピコSPI 0を使用
#define SPI_PORT spi0

// スタート時刻
// 時刻表データの最初の時刻の少し前にしておくべき
#define START_HH (10)		// 時
#define START_MM (1)		// 分
#define START_SS (1)		// 秒

// 終了時刻 この時刻を過ぎたら、スタート時刻にもどって最初からとなる。
// かならず時刻表の終電の時間の前を設定すること。終電過ぎるとエラーとなります。
#define END_HH    (17)  // 時
#define END_MM    (53)  // 分
#define END_SS    (0)   // 秒

// 表示切り替え時間定義 50msec毎にカウントアップするので、20をかけると1秒になる
#define PHASETIME_A       (15*20)   // 通常表示からスクロールインフォメーション移行までの時間
#define PHASETIME_B       (20*20)   // 電車がまいりますの表示期間
#define PHASETIME_C       (10*20)   // 電車が駅に到着してから発車するまでの時間（時刻部分が空欄の時間）
#define LANGUAGETIME      (5*20)    // 日本語と英語を切り替えるタイミング
#define LINE1POSY         (0*24)    // 1番線表示高さ
#define LINE2POSY         (5*24)    // 2番線表示高さ
#define NEXTTRAININI      (3)       // 次の電車までの分数の初期値
#define NEXTNEXTTRAININI  (7)       // 次の次の電車までの分数初期値
#define DECTIME           (30*20)   // 次の電車までの分数が減算されるまでの秒数(本来なら60秒x20回)50msec毎にインクリメントされる。
#define ONESECCOUNT       (5)       // 100msecを何回数えたら１秒とするか（本来なら10)
									                  // おもちゃなので時間が早く過ぎた方が面白い
#define SCRLDOT           (3)       // 電光掲示板を一度に何ドットスクロールさせるか
                                    // スクロールのなめらかさをとるか、速さをとるか・・・

// 時分秒の構造体定義
struct time_hms{
  uint8_t  hh;             // 時
  uint8_t  mm;             // 分
  uint8_t  ss;             // 秒
};

// 構造体定義
struct time_struct{
  struct time_hms timehms;  // 時分秒
  uint8_t	time_msec;	// 100msec毎にカウントアップ
};

// ステートを定義する enum 型
typedef enum {
  NEXT_J_ST,        // 通常の日本語表示
  NEXT_E_ST,        // 通常の英語表示
  INFO1_J_ST,       // インフォメーション1 日本語表示
  INFO1_E_ST,       // インフォメーション1 英語表示
  INFO2_J_ST,       // インフォメーション2 日本語表示
  INFO2_E_ST,       // インフォメーション2 英語表示
  MAP_J_ST,         // 地図表示 日本語
  MAP_E_ST,         // 地図表示 英語表示
  APPROACH_J_ST,    // 電車がまいります
  APPROACH_E_ST,    // Train approaching
  NEXT_STATION_ST   // 次は鶯谷駅に停車します
} station_state;

typedef enum{
  NEXT_A_P,         // 通常フェーズA
  INFO_P,           // インフォメーションフェーズ
  NEXT_B_P,         // 通常フェーズB
  MAP_P,            // 地図表示フェーズ
  APPROACH_P,       // 電車接近フェーズ
  NEXT_ST_P         // 次の案内フェーズ
} station_phase;

typedef enum{
  DO_STATE1,        // 1番線ステートマシン設定
  DO_SCRL1,         // 1番線スクロール文字表示
  DO_STATE2,        // 2番線ステートマシン設定
  DO_SCRL2,         // 2番線スクロール文字表示
  DO_DEBUG          // デバッグ用表示
} station_prosess;

//時刻表の構造体定義
struct timetable{
  struct time_hms  departure_time; // 発車時刻
  uint16_t  destination;    // 行き先
  uint16_t  train_type;     // 快速/各駅停車/山手線
  bool      isDrawLeftTime;   // 残り分数を標示するならtrue
};

struct line_st{
  uint16_t  posy;           // 上から何ドット目から表示するのか
  station_state current_state;  // ステートマシン(表示に関わる部分)
  station_phase current_phase;  // 現在のフェーズ(全体)
  bool      updatestate;    // ステート状態がアップデートされたかどうかを示す。
  uint16_t  info_no;        // スクロール文章の番号　0～
  bool      start_flg;      // スクロール文字のスタートフラグ
  bool      end_flg;        // スクロール文字の表示終了を示す。
  uint16_t  scrl_pos;       // スクロール文字の現在表示位置を格納する。
  uint16_t  button_gpio;    // 電車接近のボタンのGIO番号
  uint16_t  phase_timer;    // フェーズ切り替えようタイマー
  uint16_t  language_timer; // 言語切り替え用タイマー
  uint16_t  nexttrainmin;   // 次の電車の到着分数
  uint16_t  nextnexttrainmin; // 次の次の電車の到着分数
  uint16_t  dec_min_timer;    // 分数減算用タイマー
  uint16_t  timetableNo;    // 次にくる電車の現在の時刻表は何番目か
};



#endif // _STATION_DEFINE_H