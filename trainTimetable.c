///////////////////////////////////////////////
// 時刻表（1番線　2番線）定義
// trainTimetable.c
///////////////////////////////////////////////

#include "pico/stdlib.h"
#include <stdio.h>
#include "trainTimetable.h"
#include "station_define.h"

// 1番線時刻表
const struct timetable line1TB[]=
{
//田端駅　京浜東北南行時刻表　10:00から
//発車時刻hhmmss,目的地,列車タイプ,残り分数標示フラグ
{{10, 3,30},ISOGO,	LOCAL,false},
{{10, 8,30},OFUNA,	LOCAL,false},
{{10,13,30},ISOGO,	LOCAL,false},
{{10,19,30},KAMATA,	RAPID,false},
{{10,23,30},OFUNA,	RAPID,false},
{{10,28,30},KAMATA,	RAPID,false},
{{10,33,30},OFUNA,	RAPID,false},
{{10,38,30},ISOGO,	RAPID,false},
{{10,43,30},OFUNA,	RAPID,false},
{{10,48,30},KAMATA,	RAPID,false},
{{10,53,30},OFUNA,	RAPID,false},
{{10,58,30},ISOGO,	RAPID,false},
{{11, 3,30},OFUNA,	RAPID,false},
{{11, 8,30},KAMATA,	RAPID,false},
{{11,13,30},OFUNA,	RAPID,false},
{{11,18,30},ISOGO,	RAPID,false},
{{11,23,30},OFUNA,	RAPID,false},
{{11,28,30},KAMATA,	RAPID,false},
{{11,33,30},OFUNA,	RAPID,false},
{{11,38,30},ISOGO,	RAPID,false},
{{11,43,30},OFUNA,	RAPID,false},
{{11,48,30},KAMATA,	RAPID,false},
{{11,53,30},OFUNA,	RAPID,false},
{{11,58,30},ISOGO,	RAPID,false},
{{12, 3,30},OFUNA,	RAPID,false},
{{12, 8,30},KAMATA,	RAPID,false},
{{12,13,30},OFUNA,	RAPID,false},
{{12,18,30},ISOGO,	RAPID,false},
{{12,23,30},OFUNA,	RAPID,false},
{{12,28,30},KAMATA,	RAPID,false},
{{12,33,30},OFUNA,	RAPID,false},
{{12,38,30},ISOGO,	RAPID,false},
{{12,43,30},OFUNA,	RAPID,false},
{{12,48,30},KAMATA,	RAPID,false},
{{12,53,30},OFUNA,	RAPID,false},
{{12,58,30},ISOGO,	RAPID,false},
{{13, 3,30},OFUNA,	RAPID,false},
{{13, 8,30},KAMATA,	RAPID,false}
};


// 2番線時刻表
const struct timetable line2TB[]=
{
//田端駅　外回り時刻表　10:00から
//発車時刻hhmmss,目的地,列車タイプ,残り分数標示フラグ
{{10, 2,0},TOKYOUENO,YAMANOTELINE,true},
{{10, 7,0},TOKYOUENO,YAMANOTELINE,true},
{{10,12,0},TOKYOUENO,YAMANOTELINE,true},
{{10,17,0},TOKYOUENO,YAMANOTELINE,true},
{{10,22,0},TOKYOUENO,YAMANOTELINE,true},
{{10,27,0},TOKYOUENO,YAMANOTELINE,true},
{{10,32,0},TOKYOUENO,YAMANOTELINE,true},
{{10,37,0},TOKYOUENO,YAMANOTELINE,true},
{{10,42,0},TOKYOUENO,YAMANOTELINE,true},
{{10,47,0},TOKYOUENO,YAMANOTELINE,true},
{{10,52,0},TOKYOUENO,YAMANOTELINE,true},
{{10,57,0},TOKYOUENO,YAMANOTELINE,true},
{{11, 2,0},TOKYOUENO,YAMANOTELINE,true},
{{11, 7,0},TOKYOUENO,YAMANOTELINE,true},
{{11,12,0},TOKYOUENO,YAMANOTELINE,true},
{{11,17,0},TOKYOUENO,YAMANOTELINE,true},
{{11,22,0},TOKYOUENO,YAMANOTELINE,true},
{{11,27,0},TOKYOUENO,YAMANOTELINE,true},
{{11,32,0},TOKYOUENO,YAMANOTELINE,true},
{{11,37,0},TOKYOUENO,YAMANOTELINE,true},
{{11,42,0},TOKYOUENO,YAMANOTELINE,true},
{{11,47,0},TOKYOUENO,YAMANOTELINE,true},
{{11,52,0},TOKYOUENO,YAMANOTELINE,true},
{{11,57,0},TOKYOUENO,YAMANOTELINE,true},
{{12, 2,0},TOKYOUENO,YAMANOTELINE,true},
{{12, 7,0},TOKYOUENO,YAMANOTELINE,true},
{{12,12,0},TOKYOUENO,YAMANOTELINE,true},
{{12,17,0},TOKYOUENO,YAMANOTELINE,true},
{{12,21,0},TOKYOUENO,YAMANOTELINE,true},
{{12,25,0},TOKYOUENO,YAMANOTELINE,true},
{{12,29,0},TOKYOUENO,YAMANOTELINE,true},
{{12,34,0},TOKYOUENO,YAMANOTELINE,true},
{{12,38,0},TOKYOUENO,YAMANOTELINE,true},
{{12,43,0},TOKYOUENO,YAMANOTELINE,true},
{{12,47,0},TOKYOUENO,YAMANOTELINE,true},
{{12,51,0},TOKYOUENO,YAMANOTELINE,true},
{{12,55,0},TOKYOUENO,YAMANOTELINE,true},
{{12,58,0},TOKYOUENO,YAMANOTELINE,true}
};


