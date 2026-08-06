///////////////////////////////////////////////
// File: initialize.c
// Project: station_main.c
///////////////////////////////////////////////

// include
//#include "pico/stdlib.h"
#include "hardware/spi.h"
//#include "pico/multicore.h"
//#include "hardware/pwm.h"
//#include <stdio.h>

#include "station_define.h"
#include "st7789.h"
#include "timer_ctrl.h"
//#include "station_disp.h"
//#include "signboard_bmp240.h"
//#include "station_signboard.h"
//#include "trainTimetable.h"
//#include "stname.h"
#include "core1_sound.h"
//#include "sounddata.h"
#include "button.h"
#include "initialize.h"



///@brief 初期化関数
/// 初期化全体関数　それぞれの初期化処理を呼び出す。
void initializeSettings(){

    // spi 初期化
    spi_initialize();

    // GPIO初期化
    gpio_initialize();

    // ディスプレイ初期化
    display_initialize();

    // 時刻初期化
    // おもちゃの開始時刻を設定する。
    initializeClock();

    // タイトル表示
    showTitle();

    // ATOSモード設定
    setAtosMode();

    // core1を起動
    core1_initialize();

    // ジングル鳴動(line0)
    sleep_ms(200);
    playJingleSound();
    sleep_ms(2000);

    // 音が鳴り止むまでwaitする
    while(true){
        if (isSoundStop(0)){
            break;
        }
    }
    // 初期画面表示時間中、LEDを点滅させます
    // for (uint16_t i=0; i<10; i++){
    //     sleep_ms(200);
    //     gpio_put(LED_PIN,1);
    //     sleep_ms(200);
    //     gpio_put(LED_PIN,0);
    // }

    // 画面を黒で塗りつぶす
    fillScreenSt7789(LCD_BLK); // BLACK

    // ボタンフラグクリア
    for (uint16_t i=0; i<MAX_BUTTONS; i++){
        clear_button_released_flag(i);
    }
}

/// @brief SPIの初期化を行う。
void spi_initialize(){
    //ピンの接続:
    //SPI SCK (Clock) を GP2 (ピン4)
    //SPI TX (MOSI) を GP3 (ピン5)
    //SPI CS をGP5(ピン7)->採用しているディスプレイにはCSピンがないので、未接続
    //ディスプレイのRST ピンを GP12 (ピン16)
    //ディスプレイのDC ピンを GP8 (ピン11)

    // 通信速度設定
    //spi_init(SPI_PORT, 1000 * 1000); // 1 MHz
    spi_init(SPI_PORT, 1000 * 1000 * 20); // 20 MHz
    // ピン設定
    gpio_set_function(PIN_SPI_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SPI_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS, GPIO_FUNC_SPI);

    spi_set_format(
        SPI_PORT,       // SPIポート
	    8,              // データビット数
        SPI_CPOL_1,     // クロック極性（CPOL）
        SPI_CPHA_1,     // クロック位相（CPHA）
        SPI_MSB_FIRST   // MSBファースト
	); 
}

/// @brief GPIO初期化関数
/// GPIOの初期化を行う。
void gpio_initialize(){

    // 各種ピン設定
    gpio_init(PIN_RST);             // SPI リセットピン初期化
    gpio_init(PIN_DC);              // DCピン初期化
    gpio_init(LED_PIN);
    gpio_init(PIN_APPBTN1);         // 1番線用電車接近ボタン
    gpio_init(PIN_APPBTN2);         // 2番線用電車接近ボタン
    gpio_init(PIN_APPBTN3);         // 1番線用電車接近ボタン　拡張
    gpio_init(PIN_APPBTN4);         // 2番線用電車接近ボタン　拡張
    gpio_init(PIN_POWERSAVE);       // DCDCパワーセーブ制御
    gpio_init(PIN_AUDIOMUTE);       // 音声ミュート制御ピン(highでミュート)
    gpio_init(PIN_BLK);             // バックライト制御ピン（highで点灯)

    //入出力方向設定
    gpio_set_dir(PIN_RST, GPIO_OUT);        // RSTピン出力
    gpio_set_dir(PIN_DC, GPIO_OUT);         // DCピン出力
    gpio_set_dir(LED_PIN, GPIO_OUT);        // オンボードLED出力
    gpio_set_dir(PIN_APPBTN1, GPIO_IN);     // ボタン入力
    gpio_set_dir(PIN_APPBTN2, GPIO_IN);     // ボタン入力
    gpio_set_dir(PIN_APPBTN3, GPIO_IN);     // ボタン入力 拡張
    gpio_set_dir(PIN_APPBTN4, GPIO_IN);     // ボタン入力 拡張
    gpio_set_dir(PIN_POWERSAVE, GPIO_OUT);  // DCDCパワーセーブ制御
    gpio_set_dir(PIN_AUDIOMUTE,GPIO_OUT);   // 音声ミュート制御ピン(highでミュート)
    gpio_set_dir(PIN_BLK, GPIO_OUT);        // バックライト制御ピン (highで点灯)
    
    // プルアップ設定
    gpio_pull_up(PIN_APPBTN1);      // ボタン入力ピンをpull up設定
    gpio_pull_up(PIN_APPBTN2);      // ボタン入力ピンをpull up設定
    gpio_pull_up(PIN_APPBTN3);      // ボタン入力ピンをpull up設定
    gpio_pull_up(PIN_APPBTN4);      // ボタン入力ピンをpull up設定

    // 初期値出力
    //　gpio_put(PIN_AUDIOMUTE,1);          // ミュート出力
    gpio_put(PIN_AUDIOMUTE,0);          // ミュート解除
    gpio_put(PIN_BLK,1);                // ディスプレイバックライト点灯

    // スタンダードIO初期設定
    stdio_init_all();

    //DCDCパワーセーブモード制御
    gpio_put(PIN_POWERSAVE,1);      // ノイズ対策のためパワーセーブをOFFにする(highでオフ)
                                    // デフォルトは基板上でプルダウンされている(lowでセーブモード)
}

/// @brief ディスプレイ初期化
void display_initialize(){
    ///////////////////////////////////////////
    // ST7789ディスプレイ初期設定
    ///////////////////////////////////////////
    // Initialize the display
    initializeSt7789();

    // 画面回転設定
    // 設定角度 0:0度 1:90度 2:180度 3:270度
    setRotationSt7789(2) ;

    // Draw a red rectangle in the center
    //fillRectangleSt7789(60, 60, 120, 120, LCD_RED); // Red

    // Example: Draw pixels of different colors
    //draw1PixelSt7789(120, 120, 0x07E0); // Green
}

/// @brief タイトル表示関数
void showTitle(){
    ///////////////////////////////////////////
    // 初期画面表示
    ///////////////////////////////////////////  
    // 画面を黒で塗りつぶす
    fillScreenSt7789(LCD_BLK); // BLACK

    // 初期文字列表示
    locateLcdPrintf(0,0);
    setColorLcdPrintf(LCD_GRN,LCD_BLK);
    printfSt7789("===============");

    locateLcdPrintf(0,1);
    setColorLcdPrintf(LCD_YEL,LCD_GRY);
    printfSt7789("Train Info     ");
    locateLcdPrintf(0,2);
    printfSt7789("Display system2");

    locateLcdPrintf(0,3);
    setColorLcdPrintf(LCD_GRN,LCD_BLK);
    printfSt7789("===============");


    locateLcdPrintf(0,5);
    setColorLcdPrintf(LCD_CYN,LCD_BLK);
    printfSt7789("ver 2.00a"); 

    locateLcdPrintf(0,10);
    setColorLcdPrintf(LCD_WHT,LCD_BLK);
    printfSt7789("@2026  yamapy"); 

    locateLcdPrintf(0,14);
    setColorLcdPrintf(LCD_WHT,LCD_BLK);
    printfSt7789("0123456789ABCDE"); 

}

/// @brief ATOSモード切替
/// 起動時にボタンを押しているかどうかで、
/// ボタンモードとするか、時間モードとするかを決定する。
void setAtosMode(){
    // ボタンを押下していなかったら、時間切り替えモードとする
    if(gpio_get(PIN_APPBTN1)==0 || gpio_get(PIN_APPBTN2)==0 || gpio_get(PIN_APPBTN3)==0 || gpio_get(PIN_APPBTN4)==0)
        isChangeByTime = false;
    else
        isChangeByTime = true;
	
    // ATOSモードを液晶に表示する
    locateLcdPrintf(0,6);       //6行目を指定

    if (isChangeByTime){
        setColorLcdPrintf(LCD_MAG,LCD_BLK);
        printfSt7789("Time chg mode"); 
    }else{
        setColorLcdPrintf(LCD_RED,LCD_BLK);
		printfSt7789("Button chg mode"); 		
    }
}

/// @brief core1イニシャル処理
/// core1を起動する
void core1_initialize(){

   ///////////////////////////////////////////
    // マルチタスク処理実行
    ///////////////////////////////////////////
    // セマフォを初期化
    sem_init(&sem, 1, 1);
    // セマフォの許可を解除
    sem_release(&sem);

    // core1で動作させる関数を実行する。
    multicore_launch_core1(core1_main);


}
