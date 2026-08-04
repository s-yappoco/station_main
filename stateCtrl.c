///////////////////////////////////////////////
// ステートマシン制御
// stateCtrl.c
///////////////////////////////////////////////

// include
//#include "pico/stdlib.h"
//#include "hardware/spi.h"
#include "pico/multicore.h"
//#include "hardware/pwm.h"
//#include <stdio.h>

#include "station_define.h"
//#include "st7789.h"
#include "timer_ctrl.h"
//#include "station_disp.h"
//#include "signboard_bmp240.h"
//#include "station_signboard.h"
#include "trainTimetable.h"
//#include "stname.h"
#include "core1_sound.h"
//#include "sounddata.h"
#include "button.h"
#include "stateCtrl.h"

/// @brief 車線ステータス初期化関数
void initializeLineState(){

    // 構造体値初期化
    line1.line_no = 0;                          // 車線番号（ボタン番号指定用)
    line1.posy = LINE1POSY;                     // 表示高さ設定
    line1.current_state = NEXT_J_ST;            // ステートマシン初期値
    line1.current_phase = NEXT_A_P;             // ステートフェーズ初期値
    line1.updatestate = true;                   // ステートマシン更新フラグ
    line1.info_no = 0;                          // スクロール文字種別番号初期値
    line1.start_flg = false;                    // スクロール文字開始フラグ
    line1.end_flg = true;                       // スクロール文字終了フラグ
    line1.scrl_pos = 0;                         // スクロール文字ポジション
    line1.button_gpio = PIN_APPBTN1;            // 電車接近のボタンのGIO番号
    line1.phase_timer = 0;                      // フェーズ切り替え用タイマー
    line1.language_timer = 0;                   // 言語切り替え用タイマー
    line1.next_st_timer = 0;                    // 電車がホームに到着してから発車するまでに使用するタイマ

    line1.dec_min_timer = 0;                    // 残り分数減算タイマー
    line1.timetableNo = 0;                      // 次の電車の時刻表は前から何番目か
    line1.lineState = ST_INITIAL;               // 最初のステートマシンの状態

    if(isChangeByTime){
        // 時刻表から最初の分数の値を計算して初期値とする
        line1.nexttrainmin = getTimeDifference(line1TB[line1.timetableNo].departure_time, time_disp.timehms);          // 次の電車までの分数初期値
        line1.nextnexttrainmin = getTimeDifference(line1TB[line1.timetableNo+1].departure_time, time_disp.timehms);  // 次の次の電車までの分数　初期値
    }else{
        line1.nexttrainmin = NEXTTRAININI;          // 次の電車までの分数初期値
        line1.nextnexttrainmin = NEXTNEXTTRAININI;  // 次の次の電車までの分数　初期値
    }

    line2.line_no = 1;                          // 車線番号(ボタン番号指定用)
    line2.posy = LINE2POSY;                     // 表示高さ設定
    line2.current_state = NEXT_J_ST;            // ステートマシン初期値
    line2.current_phase = NEXT_A_P;             // ステートフェーズ初期値
    line2.updatestate = true;                   // ステートマシン更新フラグ
    line2.info_no = 0;                          // スクロール文字種別番号初期値
    line2.start_flg = false;                    // スクロール文字開始フラグ
    line2.end_flg = true;                       // スクロール文字終了フラグ
    line2.scrl_pos = 0;                         // スクロール文字ポジション
    line2.button_gpio = PIN_APPBTN2;            // 電車接近のボタンのGIO番号
    line2.phase_timer = 20*1;                   // フェーズ切り替え用タイマー（初期値を1番線のタイマーから1秒ずらす）
    line2.language_timer = 20*1;                // 言語切り替え用タイマー
    line2.next_st_timer = 0;                    // 電車がホームに到着してから発車するまでに使用するタイマ

    line2.dec_min_timer = 20*1;                 // 残り分数減算タイマー
    line2.timetableNo = 0;                      // 次の電車の時刻表は前から何番目か
    line1.lineState = ST_INITIAL;               // 最初のステートマシンの状態

    if(isChangeByTime){
        // 時刻表から最初の分数の値を計算して初期値とする
        line2.nexttrainmin = getTimeDifference(line2TB[line2.timetableNo].departure_time, time_disp.timehms);          // 次の電車までの分数初期値
        line2.nextnexttrainmin = getTimeDifference(line2TB[line2.timetableNo+1].departure_time, time_disp.timehms);  // 次の次の電車までの分数　初期値
    }else{
        line2.nexttrainmin = NEXTTRAININI;          // 次の電車までの分数初期値
        line2.nextnexttrainmin = NEXTNEXTTRAININI;  // 次の次の電車までの分数　初期値
    }

}

/// @brief 電光掲示板表示内容制御ステートマシン
/// @param st 車線ステートマシン（参照渡し）
/// @param tb 時刻表テーブル（参照渡し：定数）
void stationStateCtrl(struct line_st *st, const struct timetable tb[]){

	// 電車が来るかどうかを確認する。
	bool nextTrainFlg;
    nextTrainFlg = isTrainApproaching(st, tb);

    // ステートマシン記述
    switch (st->lineState)
    {
        case    ST_INITIAL:                        // 初期設定（起動ステータス）
            st->lineState = ST_JP_DUAL_DISPLAY1;
            st->end_flg = true;                                 // スクロール終了エンドフラグをセット
            st->start_flg = false;                              // スクロール開始フラグセット（スクロールスタート）
            st->phase_timer = 0;                                // フェーズ移行タイマークリア
            st->language_timer = 0;                             // 英語日本語切り替えタイマークリア     
            break;

                // 基本表示その1 (日本語/英語サイクル)
        case    ST_JP_DUAL_DISPLAY1:              // 上下二列に日本語表示を行う
            st->lineState = ST_JP_DUAL_WAIT1;
            st->end_flg = true;                                 // スクロール終了エンドフラグをセット
            st->start_flg = false;                              // スクロール開始フラグセット（スクロールスタート）
            st->language_timer = 0;                             // 英語日本語切り替えタイマークリア
            break;

        case    ST_JP_DUAL_WAIT1:                 // 上下二列の日本語表示時間を待つ
            if (nextTrainFlg){                                  // 次の電車が接近
                st->lineState = ST_APPROACHING_MELODY_START;    // ★電車接近フェーズへ移行
                st->end_flg = true;                             // スクロール終了エンドフラグをセット
                st->start_flg = false;                          // スクロール開始フラグセット（スクロールスタート）
                st->phase_timer = 0;                            // フェーズ移行タイマークリア
                st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
            } else if (st->phase_timer > PHASETIME_A){          // ★スクロール案内表示へ移行
                st->lineState = ST_GUIDANCE_INIT;               // インフォメーションフェーズへ移行
                st->end_flg = true;                             // スクロール終了エンドフラグをセット
                st->start_flg = false;                          // スクロール開始フラグセット（スクロールスタート）
                st->phase_timer = 0;                            // フェーズ移行タイマークリア
                st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
            } else if (st->language_timer > LANGUAGETIME){      // ★ 日本語>英語表示切り替え
                st->lineState = ST_EN_DUAL_DISPLAY1;             // インフォメーションフェーズへ移行
                st->end_flg = true;                             // スクロール終了エンドフラグをセット
                st->start_flg = false;                          // スクロール開始フラグセット（スクロールスタート）
                st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
            }

            break;

        case    ST_EN_DUAL_DISPLAY1:              // 上下二列に英語表示を行う
            st->lineState = ST_EN_DUAL_WAIT1;
            st->end_flg = true;                                 // スクロール終了エンドフラグをセット
            st->start_flg = false;                              // スクロール開始フラグセット（スクロールスタート）
            st->language_timer = 0;                             // 英語日本語切り替えタイマークリア
            break;

        case    ST_EN_DUAL_WAIT1:                 // 上下二列の英語表示時間を待つ
            if (nextTrainFlg){                                  // 次の電車が接近
                st->lineState = ST_APPROACHING_MELODY_START;    // ★電車接近フェーズへ移行
                st->end_flg = true;                             // スクロール終了エンドフラグをセット
                st->start_flg = false;                          // スクロール開始フラグセット（スクロールスタート）
                st->phase_timer = 0;                            // フェーズ移行タイマークリア
                st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
            } else if (st->phase_timer > PHASETIME_A){          // ★スクロール案内表示へ移行
                st->lineState = ST_GUIDANCE_INIT;               // インフォメーションフェーズへ移行
                st->end_flg = true;                             // スクロール終了エンドフラグをセット
                st->start_flg = false;                          // スクロール開始フラグクリア
                st->phase_timer = 0;                            // フェーズ移行タイマークリア
                st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
            } else if (st->language_timer > LANGUAGETIME){      // ★ 英語>日本語表示切り替え
                st->lineState = ST_JP_DUAL_DISPLAY1;             // インフォメーションフェーズへ移行
                st->end_flg = true;                             // スクロール終了エンドフラグをセット
                st->start_flg = false;                          // スクロール開始フラグクリア
                st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
            }
            break;

                // 案内表示
        case    ST_GUIDANCE_INIT:                // 案内表示を開始する
            st->lineState = ST_JP_GUIDANCE_DISPLAY;
            st->end_flg = false;                                // スクロール終了エンドフラグをクリア
            st->start_flg = true;                               // スクロール開始フラグセット（スクロールスタート）
            st->language_timer = 0;                             // 英語日本語切り替えタイマークリア
            break;

        case    ST_JP_GUIDANCE_DISPLAY:          // 日本語＆案内表示を実施
            st->lineState = ST_JP_GUIDANCE_WAIT;
            st->language_timer = 0;                             // 英語日本語切り替えタイマークリア
            break;

        case    ST_JP_GUIDANCE_WAIT:             // 日本語＆案内表示終了待ち
            if (nextTrainFlg){                                  // 次の電車が接近
                st->lineState = ST_APPROACHING_MELODY_START;    // ★電車接近フェーズへ移行
                st->end_flg = true;                             // スクロール終了エンドフラグをセット
                st->start_flg = false;                          // スクロール開始フラグセット（スクロールスタート）
                st->phase_timer = 0;                            // フェーズ移行タイマークリア
                st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
            } else if (st->end_flg){                            // ★スクロール終了
                st->lineState = ST_JP_DUAL_DISPLAY2;            // 基本表示 その2
                st->end_flg = true;                             // スクロール終了エンドフラグをセット
                st->start_flg = false;                          // スクロール開始フラグクリア
                st->phase_timer = 0;                            // フェーズ移行タイマークリア
                //st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
            } else if (st->language_timer > LANGUAGETIME){      // ★ 日本語>英語語表示切り替え
                st->lineState = ST_EN_GUIDANCE_DISPLAY;         // インフォメーションフェーズへ移行
                //st->end_flg = true;                             // スクロール終了エンドフラグをセット
                //st->start_flg = false;                          // スクロール開始フラグクリア
                st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
            }
            break;

        case    ST_EN_GUIDANCE_DISPLAY:          // 英語＆案内表示を実施
            st->lineState = ST_EN_GUIDANCE_WAIT;
            st->language_timer = 0;                             // 英語日本語切り替えタイマークリア
            break;

        case    ST_EN_GUIDANCE_WAIT:             // 英語＆案内表示終了待ち
            if (nextTrainFlg){                                  // 次の電車が接近
                st->lineState = ST_APPROACHING_MELODY_START;    // ★電車接近フェーズへ移行
                st->end_flg = true;                             // スクロール終了エンドフラグをセット
                st->start_flg = false;                          // スクロール開始フラグセット（スクロールスタート）
                st->phase_timer = 0;                            // フェーズ移行タイマークリア
                st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
            } else if (st->end_flg){                            // ★スクロール終了
                st->lineState = ST_EN_DUAL_DISPLAY2;            // 基本表示 その2
                st->end_flg = true;                             // スクロール終了エンドフラグをセット
                st->start_flg = false;                          // スクロール開始フラグクリア
                st->phase_timer = 0;                            // フェーズ移行タイマークリア
                //st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
            } else if (st->language_timer > LANGUAGETIME){      // ★ 英語>日本語表示切り替え
                st->lineState = ST_JP_GUIDANCE_DISPLAY;         // インフォメーションフェーズへ移行
                //t->end_flg = true;                             // スクロール終了エンドフラグをセット
                //st->start_flg = false;                          // スクロール開始フラグクリア
                st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
            }
            break;

                // 基本表示その2 (日本語/英語サイクル)
            case    ST_JP_DUAL_DISPLAY2:              // 上下二列に日本語表示を行う
                st->lineState = ST_JP_DUAL_WAIT2;
                st->end_flg = true;                                 // スクロール終了エンドフラグをセット
                st->start_flg = false;                              // スクロール開始フラグセット（スクロールスタート）
                st->language_timer = 0;                             // 英語日本語切り替えタイマークリア
                break;
    
            case    ST_JP_DUAL_WAIT2:                 // 上下二列の日本語表示時間を待つ
                if (nextTrainFlg){                                  // 次の電車が接近
                    st->lineState = ST_APPROACHING_MELODY_START;    // ★電車接近フェーズへ移行
                    st->end_flg = true;                             // スクロール終了エンドフラグをセット
                    st->start_flg = false;                          // スクロール開始フラグセット（スクロールスタート）
                    st->phase_timer = 0;                            // フェーズ移行タイマークリア
                    st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
                } else if (st->phase_timer > PHASETIME_A){          // ★マップ表示へ移行
                    st->lineState = ST_JP_MAP_DISPLAY;              // マップフェーズへ移行
                    st->end_flg = true;                             // スクロール終了エンドフラグをセット
                    st->start_flg = false;                          // スクロール開始フラグセット（スクロールスタート）
                    st->phase_timer = 0;                            // フェーズ移行タイマークリア
                    //st->language_timer = 0;                       // 英語日本語切り替えタイマークリア
                } else if (st->language_timer > LANGUAGETIME){      // ★ 日本語>英語表示切り替え
                    st->lineState = ST_EN_DUAL_DISPLAY2;            // 英語へ切り替え
                    st->end_flg = true;                             // スクロール終了エンドフラグをセット
                    st->start_flg = false;                          // スクロール開始フラグセット（スクロールスタート）
                    st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
                }
    
                break;
    
            case    ST_EN_DUAL_DISPLAY2:              // 上下二列に英語表示を行う
                st->lineState = ST_EN_DUAL_WAIT2;
                st->end_flg = true;                                 // スクロール終了エンドフラグをセット
                st->start_flg = false;                              // スクロール開始フラグセット（スクロールスタート）
                st->language_timer = 0;                             // 英語日本語切り替えタイマークリア
                break;
    
            case    ST_EN_DUAL_WAIT2:                 // 上下二列の英語表示時間を待つ
                if (nextTrainFlg){                                  // 次の電車が接近
                    st->lineState = ST_APPROACHING_MELODY_START;    // ★電車接近フェーズへ移行
                    st->end_flg = true;                             // スクロール終了エンドフラグをセット
                    st->start_flg = false;                          // スクロール開始フラグセット（スクロールスタート）
                    st->phase_timer = 0;                            // フェーズ移行タイマークリア
                    st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
                } else if (st->phase_timer > PHASETIME_A){          // ★スクロール案内表示へ移行
                    st->lineState = ST_EN_MAP_DISPLAY;              // マップフェーズへ移行
                    st->end_flg = true;                             // スクロール終了エンドフラグをセット
                    st->start_flg = false;                          // スクロール開始フラグクリア
                    st->phase_timer = 0;                            // フェーズ移行タイマークリア
                    //st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
                } else if (st->language_timer > LANGUAGETIME){      // ★ 英語>日本語表示切り替え
                    st->lineState = ST_JP_DUAL_DISPLAY2;            // インフォメーションフェーズへ移行
                    st->end_flg = true;                             // スクロール終了エンドフラグをセット
                    st->start_flg = false;                          // スクロール開始フラグクリア
                    st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
                }
                break;
            
                // マップ表示
        case    ST_JP_MAP_DISPLAY:               // 日本語＆マップ表示を行う
            st->lineState = ST_JP_MAP_WAIT;
            st->end_flg = true;                                 // スクロール終了エンドフラグをセット
            st->start_flg = false;                              // スクロール開始フラグクリア
            //st->language_timer = 0;                             // 英語日本語切り替えタイマークリア
            break;

        case    ST_JP_MAP_WAIT:                  // 日本語＆マップ表示待ち
            if (nextTrainFlg){                                  // 次の電車が接近
                st->lineState = ST_APPROACHING_MELODY_START;    // ★電車接近フェーズへ移行
                st->end_flg = true;                             // スクロール終了エンドフラグをセット
                st->start_flg = false;                          // スクロール開始フラグセット（スクロールスタート）
                st->phase_timer = 0;                            // フェーズ移行タイマークリア
                st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
            } else if (st->phase_timer > PHASETIME_A){          // ★マップ表示へ移行
                st->lineState = ST_JP_DUAL_DISPLAY1;            // 基本表示へ移行
                st->end_flg = true;                             // スクロール終了エンドフラグをセット
                st->start_flg = false;                          // スクロール開始フラグセット（スクロールスタート）
                st->phase_timer = 0;                            // フェーズ移行タイマークリア
                //st->language_timer = 0;                       // 英語日本語切り替えタイマークリア
            } else if (st->language_timer > LANGUAGETIME){      // ★ 日本語>英語表示切り替え
                st->lineState = ST_EN_MAP_DISPLAY;             // 英語へ切り替え
                st->end_flg = true;                             // スクロール終了エンドフラグをセット
                st->start_flg = false;                          // スクロール開始フラグセット（スクロールスタート）
                st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
            }
            break;

        case    ST_EN_MAP_DISPLAY:               // 英語＆マップ表示を行う
            st->lineState = ST_EN_MAP_WAIT;
            st->end_flg = true;                                 // スクロール終了エンドフラグをセット
            st->start_flg = false;                              // スクロール開始フラグセット（スクロールスタート）
            //st->language_timer = 0;                             // 英語日本語切り替えタイマークリア
            break;

        case    ST_EN_MAP_WAIT:                  // 英語＆マップ表示待ち
            if (nextTrainFlg){                                  // 次の電車が接近
                st->lineState = ST_APPROACHING_MELODY_START;    // ★電車接近フェーズへ移行
                st->end_flg = true;                             // スクロール終了エンドフラグをセット
                st->start_flg = false;                          // スクロール開始フラグセット（スクロールスタート）
                st->phase_timer = 0;                            // フェーズ移行タイマークリア
                st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
            } else if (st->phase_timer > PHASETIME_A){          // ★マップ表示へ移行
                st->lineState = ST_EN_DUAL_DISPLAY1;            // 基本表示へ移行
                st->end_flg = true;                             // スクロール終了エンドフラグをセット
                st->start_flg = false;                          // スクロール開始フラグセット（スクロールスタート）
                st->phase_timer = 0;                            // フェーズ移行タイマークリア
                //st->language_timer = 0;                       // 英語日本語切り替えタイマークリア
            } else if (st->language_timer > LANGUAGETIME){      // ★ 日本語>英語表示切り替え
                st->lineState = ST_JP_MAP_DISPLAY;              // 日本語へ切り替え
                st->end_flg = true;                             // スクロール終了エンドフラグをセット
                st->start_flg = false;                          // スクロール開始フラグクリア
                st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
            }
            break;
                // 電車接近表示

        case    ST_APPROACHING_MELODY_START:     // 電車接近放送開始
            st->lineState = ST_JP_APPROACHING_DISPLAY;
            st->phase_timer = 0;                            // フェーズ移行タイマークリア
            st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
            break;
    

        case    ST_JP_APPROACHING_DISPLAY:       // 日本語電車接近表示
            st->lineState = ST_JP_APPROACHING_WAIT;
            st->end_flg = true;                                 // スクロール終了エンドフラグをセット
            st->start_flg = false;                              // スクロール開始フラグクリア
            st->language_timer = 0;                             // 英語日本語切り替えタイマークリア
            break;

        case    ST_JP_APPROACHING_WAIT:          // 日本語電車接近表示待ち
            if (isChangeByTime){            // 時間切り替えモード
                //if(st->next_st_timer > SILENT1TIME){
                if(st->phase_timer > SILENT1TIME){
                    st->lineState = ST_JP_SINGLE_LINE_DISPLAY; 
                    st->end_flg = true;                             // スクロール終了エンドフラグをセット
                    st->start_flg = false;                          // スクロール開始フラグクリア
                    st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
                } else if(st->language_timer > LANGUAGETIME){
                    st->lineState = ST_EN_APPROACHING_DISPLAY; 
                    st->end_flg = true;                             // スクロール終了エンドフラグをセット
                    st->start_flg = false;                          // スクロール開始フラグクリア
                    st->language_timer = 0;                         // 英語日本語切り替えタイマークリア                    
                }
             } else {                    // ボタン切り替えモード
                if(is_button_released_flag_lineno(st->line_no)){    // ボタンが押されたら
                    st->lineState = ST_JP_SINGLE_LINE_DISPLAY; 
                    st->end_flg = true;                             // スクロール終了エンドフラグをセット
                    st->start_flg = false;                          // スクロール開始フラグクリア
                    st->language_timer = 0;                         // 英語日本語切り替えタイマークリア 
                    
                    //  音を止める処理必要


                    clear_button_released_flag_lineno(st->line_no);     
                } else if(st->language_timer > LANGUAGETIME){
                    st->lineState = ST_EN_APPROACHING_DISPLAY; 
                    st->end_flg = true;                             // スクロール終了エンドフラグをセット
                    st->start_flg = false;                          // スクロール開始フラグクリア
                    st->language_timer = 0;                         // 英語日本語切り替えタイマークリア                    
                }      
            }

            break;

        case    ST_EN_APPROACHING_DISPLAY:       // 英語電車接近表示
            st->lineState = ST_EN_APPROACHING_WAIT;
            st->end_flg = true;                                 // スクロール終了エンドフラグをセット
            st->start_flg = false;                              // スクロール開始フラグクリア
            st->language_timer = 0;                             // 英語日本語切り替えタイマークリア
            break;

        case    ST_EN_APPROACHING_WAIT:          // 英語電車接近表示待ち
            if (isChangeByTime){            // 時間切り替えモード
                if(st->phase_timer > SILENT1TIME){
                    st->lineState = ST_JP_SINGLE_LINE_DISPLAY; 
                    st->end_flg = true;                             // スクロール終了エンドフラグをセット
                    st->start_flg = false;                          // スクロール開始フラグクリア
                    st->language_timer = 0;                         // 英語日本語切り替えタイマークリア
                } else if(st->language_timer > LANGUAGETIME){
                    st->lineState = ST_JP_APPROACHING_DISPLAY; 
                    st->end_flg = true;                             // スクロール終了エンドフラグをセット
                    st->start_flg = false;                          // スクロール開始フラグクリア
                    st->language_timer = 0;                         // 英語日本語切り替えタイマークリア                    
                }
             } else {                    // ボタン切り替えモード
                if(is_button_released_flag_lineno(st->line_no)){    // ボタンが押されたら
                    st->lineState = ST_JP_SINGLE_LINE_DISPLAY; 
                    st->end_flg = true;                             // スクロール終了エンドフラグをセット
                    st->start_flg = false;                          // スクロール開始フラグクリア
                    st->language_timer = 0;                         // 英語日本語切り替えタイマークリア 

                    //  音を止める処理必要


                    clear_button_released_flag_lineno(st->line_no);     
                } else if(st->language_timer > LANGUAGETIME){
                    st->lineState = ST_JP_APPROACHING_DISPLAY; 
                    st->end_flg = true;                             // スクロール終了エンドフラグをセット
                    st->start_flg = false;                          // スクロール開始フラグクリア
                    st->language_timer = 0;                         // 英語日本語切り替えタイマークリア                    
                }      
            }

            break;
        
                // 特殊表示・音声
        case    ST_JP_SINGLE_LINE_DISPLAY:       // 日本語1列のみ表示
            st->lineState = ST_JP_SINGLE_LINE_WAIT;
            st->end_flg = true;                                 // スクロール終了エンドフラグをセット
            st->start_flg = false;                              // スクロール開始フラグクリア
            st->phase_timer = 0;
            st->language_timer = 0;                             // 英語日本語切り替えタイマークリア
            break;

        case    ST_JP_SINGLE_LINE_WAIT:          // 日本語1列のみ表示待ち
            if (isChangeByTime){            // 時間切り替えモード
                if(st->phase_timer > SILENT2TIME){
                    st->lineState = ST_DEPARTURE_MELODY_START;
                    st->end_flg = true;                                 // スクロール終了エンドフラグをセット
                    st->start_flg = false;                              // スクロール開始フラグクリア
                    st->phase_timer = 0;
                    st->language_timer = 0;                             // 英語日本語切り替えタイマークリア
                }
            } else {                    // ボタン切り替えモード
                if(is_button_released_flag_lineno(st->line_no)){    // ボタンが押されたら
                    st->lineState = ST_DEPARTURE_MELODY_START; 
                    st->end_flg = true;                             // スクロール終了エンドフラグをセット
                    st->start_flg = false;                          // スクロール開始フラグクリア
                    st->language_timer = 0;                         // 英語日本語切り替えタイマークリア 
                    clear_button_released_flag_lineno(st->line_no);
                }
            }    
            break;           

        case    ST_DEPARTURE_MELODY_START:       // 発車メロディ再生開始
            st->lineState = ST_DEPARTURE_MELODY_PLAYING;
            st->end_flg = true;                                 // スクロール終了エンドフラグをセット
            st->start_flg = false;                              // スクロール開始フラグクリア
            st->phase_timer = 0;
            st->language_timer = 0;                             // 英語日本語切り替えタイマークリア
            break;        

        case    ST_DEPARTURE_MELODY_PLAYING:     // 発車メロディ再生中 (再生停止待ち)
            if (isChangeByTime){        // 時間切り替えモード
                if(isSoundStop(st->line_no)){                   // チャイムが鳴り終わったら
                    st->lineState = ST_DOOR_CLOSE_ANNOUNCE_START;
                    st->phase_timer = 0;
                }
            } else {                    // ボタン切り替えモード
                if(isSoundStop(st->line_no) || is_button_released_flag_lineno(st->line_no)){                   // チャイムが鳴り終わったら
                    st->lineState = ST_DOOR_CLOSE_ANNOUNCE_START;
                    st->phase_timer = 0;
                    clear_button_released_flag_lineno(st->line_no);     
                }         
            }
            break;


        case    ST_DOOR_CLOSE_ANNOUNCE_START:    // ドアが閉まります案内再生開始
            st->lineState = ST_DOOR_CLOSE_ANNOUNCE_PLAYING;
            st->end_flg = true;                                 // スクロール終了エンドフラグをセット
            st->start_flg = false;                              // スクロール開始フラグクリア
            st->phase_timer = 0;
            st->language_timer = 0;                             // 英語日本語切り替えタイマークリア
            break;

        case    ST_DOOR_CLOSE_ANNOUNCE_PLAYING:  // ドアが閉まります案内再生中 (再生停止待ち)
            if (st->phase_timer > SILENT3TIME){
                st->lineState = ST_SET_NEXT_TRAIN;
                st->end_flg = true;                             // スクロール終了エンドフラグをセット
                st->start_flg = false;                          // スクロール開始フラグクリア
                st->phase_timer = 0;
                st->language_timer = 0;                         // 英語日本語切り替えタイマークリア               
            }
            break;
        case    ST_SET_NEXT_TRAIN:               // 次の電車のデータを用意する
            st->lineState = ST_JP_DUAL_DISPLAY1;
            st->end_flg = true;                                 // スクロール終了エンドフラグをセット
            st->start_flg = false;                              // スクロール開始フラグクリア
            st->phase_timer = 0;
            st->language_timer = 0;                             // 英語日本語切り替えタイマークリア
            break;           
    }
}

/// @brief 電車が近づいているかどうかを判定する
/// @param st 車線ステータス
/// @param tb 時刻表テーブル
/// @return 接近アナウンス放送へ移行するときtrue
bool isTrainApproaching(struct line_st *st,const struct timetable tb[]){

	// 次の列車に移行するべきかどうかのフラグ
	bool nextTrainFlg;

	// 現時刻を取得し、構造体に格納する
	struct time_struct now_time;
	now_time = getClock();
		
	if (isChangeByTime){
		// 時間切り替えモード
		// 時刻表が現在時刻より過去だったら、移行フラグ設定
		if(isBefore(tb[st->timetableNo].departure_time, now_time.timehms)){
			nextTrainFlg = true;
		} else {
			nextTrainFlg = false;
		}
		
	}else{
		// ボタン切り替えモード
		// ボタン状態取得
		if (is_button_released_flag_lineno(st->line_no)){
            clear_button_released_flag_lineno(st->line_no);
			nextTrainFlg = true;
		} else {
			nextTrainFlg = false;
		}	
	
    }

    return (nextTrainFlg);

}