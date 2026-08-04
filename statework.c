///////////////////////////////////////////////
// ステート実行関数
// stateCtrl.c
///////////////////////////////////////////////

// include

//#include "pico/stdlib.h"
//#include "hardware/spi.h"
//#include "pico/multicore.h"
//#include "hardware/pwm.h"
#include <stdio.h>

#include "station_define.h"
#include "st7789.h"
#include "timer_ctrl.h"
#include "station_disp.h"
//#include "signboard_bmp240.h"
//#include "station_signboard.h"
//#include "trainTimetable.h"
//#include "stname.h"
#include "core1_sound.h"
//#include "sounddata.h"
//#include "button.h"

#include "statework.h"

/// @brief ステートマシンの状態に従って実行する関数を呼び出す
/// @param st 
/// @param tb 
void doStateWork(struct line_st *st, const struct timetable tb[]) {
    switch (st->lineState) {
        // 基本表示その1 (日本語/英語サイクル)
        case ST_JP_DUAL_DISPLAY1:             // 上下二列に日本語表示を行う
            doJpDualDisplay(st, tb);
            break;

        case ST_JP_DUAL_WAIT1:                // 上下二列の日本語表示時間を待つ
            doJPDualWait(st, tb);
            break;

        case ST_EN_DUAL_DISPLAY1:             // 上下二列に英語表示を行う
            doEnDualDisplay(st, tb);
            break;

        case ST_EN_DUAL_WAIT1:                // 上下二列の英語表示時間を待つ
            doEnDualWait(st, tb);
            break;

        // 案内表示
        case ST_GUIDANCE_INIT:                // 案内表示を開始する
            doGuidanceInit(st, tb);
            break;

        case ST_JP_GUIDANCE_DISPLAY:          // 日本語＆案内表示を実施
            doJpGuidanceDisplay(st, tb);
            break;

        case ST_JP_GUIDANCE_WAIT:             // 日本語＆案内表示終了待ち
            doJpGuidanceWait(st, tb);
            break;

        case ST_EN_GUIDANCE_DISPLAY:          // 英語＆案内表示を実施
            doEnGuidanceDisplay(st, tb);
            break;

        case ST_EN_GUIDANCE_WAIT:             // 英語＆案内表示終了待ち
            doEnGuidanceWait(st, tb);
            break;

        // 基本表示その2 (日本語/英語サイクル)
        case ST_JP_DUAL_DISPLAY2:             // 上下二列に日本語表示を行う
            doJpDualDisplay(st, tb);
            break;

        case ST_JP_DUAL_WAIT2:                // 上下二列の日本語表示時間を待つ
            doJPDualWait(st, tb);
            break;

        case ST_EN_DUAL_DISPLAY2:             // 上下二列に英語表示を行う
            doEnDualDisplay(st, tb);
            break;

        case ST_EN_DUAL_WAIT2:                // 上下二列の英語表示時間を待つ
            doEnDualWait(st, tb);
            break;

        // マップ表示
        case ST_JP_MAP_DISPLAY:               // 日本語＆マップ表示を行う
            doJpMapDisplay(st, tb);
            break;

        case ST_JP_MAP_WAIT:                  // 日本語＆マップ表示待ち
            doJpMapWait(st, tb);
            break;

        case ST_EN_MAP_DISPLAY:               // 英語＆マップ表示を行う
            doEnMapDisplay(st, tb);
            break;

        case ST_EN_MAP_WAIT:                  // 英語＆マップ表示待ち
            doEnMapWait(st, tb);
            break;

        // 電車接近表示
        case ST_APPROACHING_MELODY_START:     // 接近アナウンス放送開始
            doApproachingMelodyStart(st, tb);
            break;

        case ST_JP_APPROACHING_DISPLAY:       // 日本語電車接近表示
            doJpApproachingDisplay(st, tb);
            break;

        case ST_JP_APPROACHING_WAIT:          // 日本語電車接近表示待ち
            doJpApproachingWait(st, tb);
            break;

        case ST_EN_APPROACHING_DISPLAY:       // 英語電車接近表示
            doEnApproachingDisplay(st, tb);
            break;

        case ST_EN_APPROACHING_WAIT:          // 英語電車接近表示待ち
            doEnApproachingWait(st, tb);
            break;

        // 特殊表示・音声
        case ST_JP_SINGLE_LINE_DISPLAY:       // 日本語1列のみ表示
            doJpSingleLineDisplay(st, tb);
            break;

        case ST_JP_SINGLE_LINE_WAIT:          // 日本語1列のみ表示待ち
            doJpSingleLineWait(st, tb);
            break;

        case ST_DEPARTURE_MELODY_START:       // 発車メロディ再生開始
            doDepatureMelodyStart(st, tb);
            break;

        case ST_DEPARTURE_MELODY_PLAYING:     // 発車メロディ再生中 (再生停止待ち)
            doDepatureMelodyPlaying(st, tb);
            break;

        case ST_DOOR_CLOSE_ANNOUNCE_START:    // ドアが閉まります案内再生開始
            doDoorCloseAnnounceStart(st, tb);
            break;

        case ST_DOOR_CLOSE_ANNOUNCE_PLAYING:  // ドアが閉まります案内再生中 (再生停止待ち)
            doDoorCloseAnnouncePlaying(st, tb);
            break;

        case ST_SET_NEXT_TRAIN:               // 次の電車のデータを用意する
            doSetNextTrain(st, tb);
            break;

        default:
            // 未定義のステートの場合の処理
            break;
    }
}

/// @brief 上段下段に日本語で行き先と時刻を表示する
/// @param st 車線ステータス
/// @param tb 時刻表テーブル
void doJpDualDisplay(struct line_st *st,const struct timetable tb[]){
    // 上段日本語表示
    drawTrainType(tb[st->timetableNo].train_type,true,st->posy + 24*2);
    // debug
    //printf("JA%2d B%2d T1=%2d T2=%2d\n",tb[st->timetableNo].destination,tb[st->timetableNo+1].destination,tb[st->timetableNo].train_type,tb[st->timetableNo+1].train_type);

    drawDestination(tb[st->timetableNo].destination,true,st->posy + 24*2);
    drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,true,st->posy +24*2);
    // 下段日本語表示
    drawTrainType(tb[st->timetableNo+1].train_type,true,st->posy + 24*3);
    drawDestination(tb[st->timetableNo+1].destination,true,st->posy + 24*3);
    drawTime(tb[st->timetableNo+1].isDrawLeftTime,tb[st->timetableNo+1].departure_time,st->nextnexttrainmin,true,st->posy +24*3);                  
}

/// @brief 上段下段に日本語表示の終了待ち
/// @param st 
/// @param tb 
void doJPDualWait(struct line_st *st,const struct timetable tb[]){

}

/// @brief 上段下段に英語語で行き先と時刻を表示する
/// @param st 車線ステータス
/// @param tb 時刻表テーブル
void doEnDualDisplay(struct line_st *st,const struct timetable tb[]){
    // 上段英語表示
    drawTrainType(tb[st->timetableNo].train_type,false,st->posy + 24*2);      
    drawDestination(tb[st->timetableNo].destination,false,st->posy + 24*2);
    drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,false,st->posy +24*2); 

    // 下段英語表示
    //LCD_Draw_YAMANOTE(false,st->posy + 24*3, LCD_STATION_ORANGE, LCD_BLK);
    drawTrainType(tb[st->timetableNo+1].train_type,false,st->posy + 24*3);      
    drawDestination(tb[st->timetableNo+1].destination,false,st->posy + 24*3);
    drawTime(tb[st->timetableNo+1].isDrawLeftTime,tb[st->timetableNo+1].departure_time,st->nextnexttrainmin,false,st->posy +24*3);       
    printf("EA%2d B%2d T1=%2d T2=%2d\n",tb[st->timetableNo].destination,tb[st->timetableNo+1].destination,tb[st->timetableNo].train_type,tb[st->timetableNo+1].train_type);
}

/// @brief 上段下段に英語表示の終了待ち
/// @param st 車線ステータス
/// @param tb 時刻表テーブル
void doEnDualWait(struct line_st *st,const struct timetable tb[]){

}

/// @brief スクロール案内表示を開始する
/// @param st 車線ステータス
/// @param tb 時刻表テーブル
void doGuidanceInit(struct line_st *st,const struct timetable tb[]){
    // 上段日本語表示
    drawTrainType(tb[st->timetableNo].train_type,true,st->posy + 24*2);
    drawDestination(tb[st->timetableNo].destination,true,st->posy + 24*2);
    drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,true,st->posy +24*2);

    // スクロール案内表示を開始
    st->start_flg = true;                              // スクロール開始フラグセット
    st->end_flg = false;                               // スクロール終了エンドフラグをクリア


    // 上記のフラグはすべて実行関数内でセット、リセットしたらすっきりするのではないか？

}

/// @brief スクロール案内表示中（上段に日本語表示上書き)
/// @param st 
/// @param tb 
void doJpGuidanceDisplay(struct line_st *st,const struct timetable tb[]){
    // 上段日本語表示
    drawTrainType(tb[st->timetableNo].train_type,true,st->posy + 24*2);
    drawDestination(tb[st->timetableNo].destination,true,st->posy + 24*2);
    drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,true,st->posy +24*2);
}

/// @brief 上段日本語表示下段スクロール中ウエイト
/// @param st 
/// @param tb 
void doJpGuidanceWait(struct line_st *st,const struct timetable tb[]){

}


/// @brief スクロール案内表示中（上段に日本語表示上書き)
/// @param st 
/// @param tb 
void doEnGuidanceDisplay(struct line_st *st,const struct timetable tb[]){
    // 上段英語表示
    drawTrainType(tb[st->timetableNo].train_type,false,st->posy + 24*2);
    drawDestination(tb[st->timetableNo].destination,false,st->posy + 24*2);
    drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,false,st->posy +24*2);
}

/// @brief 上段英語表示下段スクロール中ウエイト
/// @param st 
/// @param tb 
void doEnGuidanceWait(struct line_st *st,const struct timetable tb[]){

}

/// @brief 上段日本語表示　下段地図表示
/// @param st 
/// @param tb 
void doJpMapDisplay(struct line_st *st,const struct timetable tb[]){
    // 上段日本語表示
    drawTrainType(tb[st->timetableNo].train_type,true,st->posy + 24*2);      
    drawDestination(tb[st->timetableNo].destination,true,st->posy + 24*2);
    drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,true,st->posy +24*2);
    // 下段地図表示
    drawMapInfo(st->posy + 24*3,st->nexttrainmin, st->nextnexttrainmin);
}

/// @brief 上段日本語表示　下段地図表示中ウエイト
/// @param st 
/// @param tb 
void doJpMapWait(struct line_st *st,const struct timetable tb[]){

}

/// @brief 上段英語表示　下段地図表示
/// @param st 
/// @param tb 
void doEnMapDisplay(struct line_st *st,const struct timetable tb[]){
    // 上段英語表示
    drawTrainType(tb[st->timetableNo].train_type,false,st->posy + 24*2);      
    drawDestination(tb[st->timetableNo].destination,false,st->posy + 24*2);
    drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,false,st->posy +24*2);
    // 下段地図表示
    drawMapInfo(st->posy + 24*3,st->nexttrainmin, st->nextnexttrainmin);
}

/// @brief 上段英語表示　下段地図表示中ウエイト
/// @param st 
/// @param tb 
void doEnMapWait(struct line_st *st,const struct timetable tb[]){

}


/// @brief 電車接近アナウンス放送開始
/// @param st 
/// @param tb 
void doApproachingMelodyStart(struct line_st *st,const struct timetable tb[]){
    announceTrainApproach(st->line_no);  // 接近アナウンス放送開始
}

/// @brief 電車接近表示（日本語）
/// @param st 
/// @param tb 
void doJpApproachingDisplay(struct line_st *st,const struct timetable tb[]){
    // 上段日本語表示
    drawTrainType(tb[st->timetableNo].train_type,true,st->posy + 24*2);      
    drawDestination(tb[st->timetableNo].destination,true,st->posy + 24*2);

    // 山手線等、残り分数表示の場合は黒塗りつぶしをする。発車時刻表示の場合は時刻をそのまま表示する。
    if (tb[st->timetableNo].isDrawLeftTime)
        fillRectangleSt7789(24*3, st->posy + 24*2, 24*3, 24, LCD_BLK); // 残り時間非表示 
    else
        drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,true,st->posy +24*2);               

    // 「電車がまいります」表示
    fillRectangleSt7789(0, st->posy + 24*3, LCD_XSIZE, 24, LCD_BLK);     // 下段クリア「電車がまいります」表示の準備
    drawApproaching(st->posy + 24*3,true);       // 電車がまいります。日本語表示
}

/// @brief 電車がまいります。表示中
/// @param st 
/// @param tb 
void doJpApproachingWait(struct line_st *st,const struct timetable tb[]){
    // 点滅のために絶えず描画する。
    drawApproaching(st->posy + 24*3,true);       // 電車がまいります。日本語表示
}

/// @brief 電車接近表示（英語）
/// @param st 
/// @param tb 
void doEnApproachingDisplay(struct line_st *st,const struct timetable tb[]){
    // 上段英語表示
    drawTrainType(tb[st->timetableNo].train_type,false,st->posy + 24*2);      
    drawDestination(tb[st->timetableNo].destination,false,st->posy + 24*2);
    
    // 山手線等、残り分数表示の場合は黒塗りつぶしをする。発車時刻表示の場合は時刻をそのまま表示する。
    if (tb[st->timetableNo].isDrawLeftTime)
        fillRectangleSt7789(24*3, st->posy + 24*2, 24*3, 24, LCD_BLK); // 残り時間非表示 
    else
        drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,false,st->posy +24*2);

    // train approach 英語表示
    fillRectangleSt7789(0, st->posy + 24*3, LCD_XSIZE, 24, LCD_BLK); // black                   
    drawApproaching(st->posy + 24*3,false);      // train approach 英語表示

}

/// @brief train approach 表示中
/// @param st 
/// @param tb 
void doEnApproachingWait(struct line_st *st,const struct timetable tb[]){
    // 点滅のために絶えず描画する。
    drawApproaching(st->posy + 24*3,false);      // train approach 英語表示
}


/// @brief 上段のみ日本語表示（電車発車前表示）
/// @param st 
/// @param tb 
void doJpSingleLineDisplay(struct line_st *st,const struct timetable tb[]){
    // 上段日本語表示
    drawTrainType(tb[st->timetableNo].train_type,true,st->posy + 24*2);      
    drawDestination(tb[st->timetableNo].destination,true,st->posy + 24*2);

    // 山手線等、残り分数表示の場合は黒塗りつぶしをする。発車時刻表示の場合は時刻をそのまま表示する。
    if (tb[st->timetableNo].isDrawLeftTime)
        fillRectangleSt7789(24*3, st->posy + 24*2, 24*3, 24, LCD_BLK); // 残り時間非表示 
    else
        drawTime(tb[st->timetableNo].isDrawLeftTime,tb[st->timetableNo].departure_time,st->nexttrainmin,true,st->posy +24*2);               

    // 下段クリア
    fillRectangleSt7789(0, st->posy + 24*3, LCD_XSIZE, 24, LCD_BLK); 

    // 音声停止
    stopSound(st->line_no);
}

/// @brief 上段のみ日本語表示　発車メロディ鳴動待ち
/// @param st 
/// @param tb 
void doJpSingleLineWait(struct line_st *st,const struct timetable tb[]){

}

/// @brief 発車メロディ鳴動開始
/// @param st 
/// @param tb 
void doDepatureMelodyStart(struct line_st *st,const struct timetable tb[]){
    playDepartureMelody(st->line_no);
}

/// @brief 発車メロディ鳴動中
/// @param st 
/// @param tb 
void doDepatureMelodyPlaying(struct line_st *st,const struct timetable tb[]){

}

/// @brief ドア締めアナウンス鳴動開始
/// @param st 
/// @param tb 
void doDoorCloseAnnounceStart(struct line_st *st,const struct timetable tb[]){
    announceDoorCloseing(st->line_no);
}

/// @brief ドア締めアナウンス鳴動終了待ち
/// @param st 
/// @param tb 
void doDoorCloseAnnouncePlaying(struct line_st *st,const struct timetable tb[]){

}

/// @brief 電車発車　次のデータ準備
/// @param st 
/// @param tb 
void doSetNextTrain(struct line_st *st,const struct timetable tb[]){

    // 現在時刻を取得
    struct time_struct now_time;
    now_time = getClock();

    // 電車出発
    // 1段目　次の電車までの到着分数を再設定
    st->nexttrainmin = st->nextnexttrainmin;        // 次の電車の値をコピーする
    if (st->nexttrainmin > 5)                       // 6だと次の電車にくっついてしまうので、5分未満とする
        st->nexttrainmin = 5;
    
    st->timetableNo++;                              // 次の時刻表に移行する

    // 2段目
    // 時刻分数差分を取得
    int16_t minutes;
    // 次の時刻表までの差分を計算する
    minutes = getTimeDifference(tb[st->timetableNo+1].departure_time, now_time.timehms);
    
    // 時間切り替えモードならちゃんと計算、ボタンモードなら固定値
    if (isChangeByTime){
        // 時間切り替えモード
        if(minutes > st->nexttrainmin)
            st->nextnexttrainmin = minutes;		// 次の電車より時間が長いなら、そのまま設定
        else
            st->nextnexttrainmin = NEXTNEXTTRAININI;
    } else {
        // ボタン切り替えモード
        st->nextnexttrainmin = NEXTNEXTTRAININI;    // 次の次の電車は7分後で固定				
    }

}