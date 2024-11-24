///////////////////////////////////////////////
// 電光掲示板 文字列等定義
// station_signboard.c
///////////////////////////////////////////////

#include "pico/stdlib.h"
#include <stdio.h>
#include "station_signboard.h"

// 文字列データ1
//駅構内や列車内でのお忘れ物には十分ご注意ください。
const uint32_t SIGNBOARD24_1[] = {
// 文字列データは最初のワードに色情報＋最終文字フラグを付けた24x32bitワードを１文字データとする。
// 
// 0x000000 白
// 0x000001 緑
// 0x000002 赤
// 0x000003 橙

// 0x000010 が付いていたら、点滅文字とする。
// 0x000100 が付いていたら、最終文字とする。


//駅
0x000003, // 橙
0x7FDFF8,0x7FDFF8,0x661818,0x661818,0x7FD818,0x7FD818,0x661818,0x661FF8,
0x7FDFF8,0x7FD998,0x661980,0x7FD880,0x7FD8C0,0x00D8C0,0x57D8C0,0x55D860,
0x55D870,0x54D838,0xD0B01C,0x47B00C,0x078000,0x000000,0x000000,0x000000,

//構
0x000003, // 橙
0x0018C0,0x1818C0,0x1818C0,0x18FFF8,0x18FFF8,0xFF7FF0,0xFF7FF0,0x1818C0,
0x19FFFC,0x39FFFC,0x3C0600,0x3F7FF0,0x7B7FF0,0x796630,0xD87FF0,0x587FF0,
0x186630,0x19FFFC,0x19FFFC,0x186030,0x186070,0x186070,0x000000,0x000000,

//内
0x000003, // 橙
0x000000,0x003000,0x003000,0x003000,0x003000,0x3FFFF0,0x3FFFF0,0x303030,
0x303030,0x307030,0x307830,0x306E30,0x30C730,0x31C3B0,0x3381F0,0x3700B0,
0x360030,0x300030,0x300030,0x3000F0,0x3000F0,0x000000,0x000000,0x000000,

//や
0x000001, // 緑
0x000000,0x000000,0x0C0C00,0x0C1800,0x0C1800,0x0C1F00,0x063FE0,0x07F070,
0x0F8030,0x7E0038,0xFE0018,0x430018,0x030830,0x031FF0,0x018FE0,0x018000,
0x018000,0x00C000,0x00C000,0x00E000,0x006000,0x000000,0x000000,0x000000,

//列
0x000003, // 橙
0x000000,0x3FF830,0x3FF830,0x060330,0x060330,0x060330,0x07F330,0x0FF330,
0x0C6330,0x186330,0x3E6330,0x376330,0xF1E330,0x60C330,0x01C330,0x018330,
0x038030,0x0F0030,0x3E0030,0x3800F0,0x1000E0,0x000000,0x000000,0x000000,

//車
0x000003, // 橙
0x003000,0x003000,0x7FFFF0,0x7FFFF0,0x003000,0x1FFFE0,0x1FFFE0,0x183060,
0x183060,0x1FFFE0,0x1FFFE0,0x183060,0x1FFFE0,0x1FFFE0,0x003000,0xFFFFFC,
0xFFFFFC,0x003000,0x003000,0x003000,0x003000,0x000000,0x000000,0x000000,

//内
0x000003, // 橙
0x000000,0x003000,0x003000,0x003000,0x003000,0x3FFFF0,0x3FFFF0,0x303030,
0x303030,0x307030,0x307830,0x306E30,0x30C730,0x31C3B0,0x3381F0,0x3700B0,
0x360030,0x300030,0x300030,0x3000F0,0x3000F0,0x000000,0x000000,0x000000,

//で
0x000001, // 緑
0x000000,0x000000,0x000060,0x007FE0,0xFFFF00,0x7F3800,0x00E080,0x00C0E0,
0x018230,0x018380,0x0180C0,0x030000,0x030000,0x018000,0x018000,0x01C000,
0x00E000,0x007C00,0x003F80,0x000700,0x000000,0x000000,0x000000,0x000000,

//の
0x000001, // 緑
0x000000,0x000000,0x000000,0x00FE00,0x03FF80,0x0721C0,0x0E30E0,0x187070,
0x387030,0x306038,0x706038,0x606038,0x60E038,0x60C038,0x60C030,0x71C070,
0x3180E0,0x3F81E0,0x1F0780,0x040F00,0x000400,0x000000,0x000000,0x000000,

//お
0x000012, // 点滅赤
0x018000,0x018000,0x018080,0x0189C0,0x01F8E0,0x3FF870,0x3F8038,0x018010,
0x018000,0x019C00,0x01FFC0,0x07C0E0,0x0F8070,0x1D8030,0x398030,0x318030,
0x318830,0x319C70,0x330FE0,0x1F07C0,0x0C0000,0x000000,0x000000,0x000000,

//忘
0x000012, // 点滅赤
0x006000,0x006000,0xFFFFF0,0xFFFFF0,0x180000,0x180000,0x180000,0x180000,
0x1FFFC0,0x1FFFC0,0x000000,0x002000,0x066000,0x3631C0,0x3638E0,0x761060,
0x660370,0x660338,0xE70720,0x07FF00,0x01FC00,0x000000,0x000000,0x000000,

//れ
0x000012, // 点滅赤
0x060000,0x060000,0x060000,0x060000,0x060000,0x079F00,0x7F3F80,0x777180,
0x06C180,0x078180,0x070300,0x0E0300,0x1E0300,0x3E0300,0x360308,0x66030C,
0xE60318,0x4603F8,0x0601E0,0x060000,0x060000,0x000000,0x000000,0x000000,

//物
0x000012, // 点滅赤
0x0C0000,0x0C1800,0x0C1800,0x6C1800,0x6C1800,0x7FBFF8,0x7FBFF8,0x6C76D8,
0x6C66D8,0xCCC6D8,0x0CCCD8,0x0C8CD8,0x0F9998,0x3F3998,0x7C7398,0x4CE318,
0x0C4618,0x0C0E18,0x0C1C18,0x0C3838,0x0C71F0,0x0C20E0,0x000000,0x000000,

//に
0x000001, // 緑
0x000000,0x000000,0x100000,0x180000,0x380000,0x387FE0,0x307FE0,0x300000,
0x300000,0x300000,0x300000,0x300000,0x300000,0x300000,0x300000,0x306000,
0x307800,0x303FF8,0x3007F8,0x300000,0x300000,0x000000,0x000000,0x000000,

//は
0x000001, // 緑
0x000000,0x000000,0x200600,0x300600,0x700600,0x600600,0x60FFF0,0x63FFF0,
0x620600,0x600600,0x600600,0x600600,0x600600,0x600600,0x607E00,0x61FF00,
0x6187C0,0x6186F0,0x618630,0x61FE10,0x00F800,0x000000,0x000000,0x000000,

//十
0x000012, // 点滅赤
0x006000,0x006000,0x006000,0x006000,0x006000,0x006000,0x006000,0x006000,
0xFFFFF0,0xFFFFF0,0x006000,0x006000,0x006000,0x006000,0x006000,0x006000,
0x006000,0x006000,0x006000,0x006000,0x006000,0x000000,0x000000,0x000000,

//分
0x000012, // 点滅赤
0x020C00,0x030C00,0x070600,0x0E0600,0x0C0300,0x1C0380,0x3801C0,0x7000E0,
0xE00078,0xCFFF30,0x0FFF10,0x018300,0x018300,0x030300,0x030300,0x070300,
0x0E0300,0x1C0300,0x780700,0xF03E00,0x403800,0x000000,0x000000,0x000000,

//ご
0x000012, // 点滅赤
0x000080,0x0001C0,0x000260,0x0003A0,0x3FFDC0,0x3FFC40,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x600000,0xE00000,
0x600000,0x700000,0x7FFF00,0x1FFF00,0x000000,0x000000,0x000000,0x000000,

//注
0x000012, // 点滅赤
0x003000,0x607800,0x701E00,0x380C00,0x1BFFF0,0x03FFF0,0x400C00,0xE00C00,
0x780C00,0x100C00,0x000C00,0x01FFE0,0x01FFE0,0x180C00,0x300C00,0x300C00,
0x600C00,0xE00C00,0xC7FFF8,0xC7FFF8,0x000000,0x000000,0x000000,0x000000,

//意
0x000012, // 点滅赤
0x003000,0x3FFFE0,0x3FFFE0,0x020300,0x030600,0xFFFFFC,0xFFFFFC,0x000000,
0x1FFF80,0x1FFF80,0x180180,0x1FFF80,0x180180,0x1FFF80,0x1FFF80,0x233020,
0x333870,0x7311B8,0x630198,0x63FF90,0x01FF00,0x000000,0x000000,0x000000,

//く
0x000012, // 点滅赤
0x000200,0x000700,0x000E00,0x001C00,0x003800,0x007000,0x00E000,0x01C000,
0x038000,0x070000,0x060000,0x070000,0x038000,0x01E000,0x00F000,0x003800,
0x001C00,0x000E00,0x000700,0x000700,0x000200,0x000000,0x000000,0x000000,

//だ
0x000012, // 点滅赤
0x0600C0,0x060070,0x060330,0x0601C0,0x0660C0,0xFFE000,0xFF8000,0x0C0000,
0x0C03E0,0x0C3FE0,0x0C3C00,0x0C0000,0x180000,0x180000,0x180000,0x380000,
0x304000,0x307800,0x707FE0,0x6007E0,0x000000,0x000000,0x000000,0x000000,

//さ
0x000012, // 点滅赤
0x007000,0x003000,0x003000,0x003000,0x003FC0,0x1FFFE0,0x0FF800,0x000C00,
0x000E00,0x000700,0x000380,0x01FFC0,0x07FFE0,0x0E00C0,0x0C0000,0x0C0000,
0x0C0000,0x0E0000,0x07F800,0x01FF00,0x000F00,0x000000,0x000000,0x000000,

//い
0x000012, // 点滅赤
0x000000,0x000000,0x000000,0x180000,0x380000,0x380380,0x3801C0,0x3000E0,
0x300060,0x300070,0x300030,0x300038,0x300038,0x300038,0x384018,0x38E010,
0x18C000,0x1DC000,0x1F8000,0x0F0000,0x000000,0x000000,0x000000,0x000000,

//。
0x000112, // 点滅赤 最終文字
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x038000,0x044000,
0x082000,0x082000,0x082000,0x044000,0x038000,0x000000,0x000000,0x000000

};

// 文字列データ2
//【遅延】湘南新宿ラインは、宇都宮線内での人身事故の影響で、宇都宮線・横須賀線方面の一部列車に遅れと運休が出ています。
const uint32_t SIGNBOARD24_2[] = {
// 文字列データは最初のワードに色情報＋最終文字フラグを付けた24x32bitワードを１文字データとする。
// 
// 0x000000 白
// 0x000001 緑
// 0x000002 赤
// 0x000003 橙

// 0x000010 が付いていたら、点滅文字とする。
// 0x000100 が付いていたら、最終文字とする。
//

// 【
0x000012, // 点滅赤
0x01FC00,0x01FC00,0x01F800,0x01F000,0x01E000,0x01C000,0x01C000,0x01C000,
0x01C000,0x018000,0x018000,0x018000,0x018000,0x018000,0x01C000,0x01C000,
0x01C000,0x01E000,0x01E000,0x01F000,0x01F800,0x01FC00,0x000000,0x000000,

// 遅 U+9045
0x000012, // 点滅赤
0x01FFF0,0x31FFF0,0x398030,0x198030,0x01FFF0,0x01FFF0,0x018CC0,0x018CC0,
0x01FFF8,0xF9FFF8,0xF98300,0x19BFF0,0x19BFF0,0x1B8300,0x1B7FF8,0x1B7FF8,
0x1A0300,0x3C0300,0x778000,0x63FFFC,0x40FFFC,0x000000,0x000000,0x000000,

// 延
0x000012, // 点滅赤
0x0400E0,0xFEFFE0,0xFCFF80,0x0C0600,0x180600,0x180600,0x306600,0x7E67F0,
0x7E67F0,0xE66600,0x066600,0x266600,0x646600,0x3C6600,0x3DFFF0,0x3DFFF0,
0x180000,0x3E0000,0x77FFF8,0xE1FFF8,0x000000,0x000000,0x000000,0x000000,

// 情
0x000012, // 点滅赤
0x180700,0x180600,0x18FFF8,0x18FFF8,0x180600,0x5A7FF0,0x7B7FF0,0x7A0600,
0x59FFFC,0xD9FFFC,0xD80000,0xD87FF0,0x187FF0,0x186030,0x187FF0,0x187FF0,
0x186030,0x187FF0,0x187FF0,0x186030,0x1860F0,0x186070,0x000000,0x000000,

// 報
0x000012, // 点滅赤
0x060000,0x060FF0,0x7FCFF0,0x7FCC30,0x060C30,0x060CF0,0xFFEC70,0xFFEC00,
0x318FF8,0x198FF8,0x190F18,0x7FED18,0x7FED90,0x060DB0,0x060CF0,0x7FECE0,
0x7FECE0,0x060CF0,0x060DB8,0x060F1C,0x060E08,0x000000,0x000000,0x000000,

// 】
0x000012, // 点滅赤
0x01FC00,0x01FC00,0x00FC00,0x007C00,0x003C00,0x003C00,0x001C00,0x001C00,
0x001C00,0x000C00,0x000C00,0x000C00,0x000C00,0x001C00,0x001C00,0x001C00,
0x001C00,0x003C00,0x003C00,0x007C00,0x00FC00,0x01FC00,0x000000,0x000000,

// 高
0x000003, // 橙
0x003000,0x003000,0x003000,0xFFFFF8,0xFFFFF8,0x000000,0x0FFF80,0x0FFF80,
0x0C0180,0x0FFF80,0x0FFF80,0x000000,0x3FFFF0,0x3FFFF0,0x300030,0x33FF30,
0x33FF30,0x330330,0x33FF30,0x33FF30,0x330070,0x300070,0x000000,0x000000,

// 崎
0x000003, // 橙
0x000300,0x180300,0x187FF0,0x187FF0,0xFB8700,0xDB0DC0,0xDB3CF8,0xDB7030,
0xDB0000,0xDBFFFC,0xDBFFFC,0xDB0030,0xDB3F30,0xDB3F30,0xFF3330,0xFF3330,
0xC33F30,0xC03F30,0x000030,0x0000F0,0x0000F0,0x000000,0x000000,0x000000,

// 線
0x000003, // 橙
0x0C0300,0x0C0300,0x183FF0,0x333FF0,0x733030,0x763FF0,0x3C3FF0,0x1D3030,
0x19B030,0x3FBFF0,0xFFFFF0,0x0C8318,0x0C7F38,0x6DFFF0,0x6D8FC0,0x6D8BE0,
0x6D9B70,0x6C3B38,0x6C731C,0x0C6708,0x0C4700,0x000000,0x000000,0x000000,

// ・
0x000003, // 橙
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x002000,0x007000,0x00F800,0x007000,0x002000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,

// 宇
0x000003, // 橙
0x003000,0x003000,0x003000,0x7FFFF8,0x7FFFF8,0x600018,0x600018,0x0FFF80,
0x0FFF80,0x003000,0x003000,0xFFFFFC,0xFFFFFC,0x003000,0x003000,0x003000,
0x003000,0x003000,0x003000,0x00F000,0x00F000,0x000000,0x000000,0x000000,

// 都
0x000003, // 橙
0x060000,0x060000,0x061BF0,0x3FDBF8,0x3FF318,0x063330,0x066330,0x06C330,
0x7FFB60,0x7FFB60,0x070370,0x0FF330,0x3FF318,0xF83318,0xD8331C,0x1FF30C,
0x1FF31C,0x18337C,0x183378,0x1FF300,0x1FF300,0x183300,0x000000,0x000000,

// 宮
0x000003, // 橙
0x006000,0x006000,0xFFFFF0,0xFFFFF0,0xC00030,0xC00030,0xDFFFB0,0xD801B0,
0x180180,0x180180,0x1FFF80,0x1FFF80,0x00C000,0x00C000,0x3FFFC0,0x3000C0,
0x3000C0,0x3000C0,0x3000C0,0x3FFFC0,0x3FFFC0,0x3000C0,0x000000,0x000000,
// 線
0x000003, // 橙
0x0C0300,0x0C0300,0x183FF0,0x333FF0,0x733030,0x763FF0,0x3C3FF0,0x1D3030,
0x19B030,0x3FBFF0,0xFFFFF0,0x0C8318,0x0C7F38,0x6DFFF0,0x6D8FC0,0x6D8BE0,
0x6D9B70,0x6C3B38,0x6C731C,0x0C6708,0x0C4700,0x000000,0x000000,0x000000,
// は
0x000003, // 橙
0x000000,0x400C00,0x600C00,0xE00C00,0xC00C00,0xC1FFE0,0xC7FFE0,0xC40C00,
0xC00C00,0xC00C00,0xC00C00,0xC00C00,0xC00C00,0xC0FC00,0xC3FE00,0xC30F80,
0xC30DE0,0xC30C60,0xC3FC20,0x01F000,0x000000,0x000000,0x000000,0x000000,

// 大	5927
0x000003, // 橙
0x006000,0x006000,0x006000,0x006000,0x006000,0xFFFFF0,0x006000,0x006000,
0x006000,0x00F000,0x00F000,0x00D000,0x019800,0x030C00,0x030C00,0x060600,
0x0C0380,0x3801C0,0xF000F0,0xC00030,0x000000,0x000000,0x000000,0x000000,

// 宮 5bae
0x000003, // 橙
0x006000,0x006000,0xFFFFF0,0xFFFFF0,0xC00030,0xC00030,0xDFFFB0,0xD801B0,
0x180180,0x180180,0x1FFF80,0x1FFF80,0x00C000,0x00C000,0x3FFFC0,0x3000C0,
0x3000C0,0x3000C0,0x3000C0,0x3FFFC0,0x3FFFC0,0x3000C0,0x000000,0x000000,

// 駅
0x000003, // 橙
0x7FDFF8,0x7FDFF8,0x661818,0x661818,0x7FD818,0x7FD818,0x661818,0x661FF8,
0x7FDFF8,0x7FD998,0x661980,0x7FD880,0x7FD8C0,0x00D8C0,0x57D8C0,0x55D860,
0x55D870,0x54D838,0xD0B01C,0x47B00C,0x078000,0x000000,0x000000,0x000000,

// で
0x000003, // 橙
0x000000,0x000000,0x000060,0x007FE0,0xFFFF00,0x7F3800,0x00E080,0x00C0E0,
0x018230,0x018380,0x0180C0,0x030000,0x030000,0x018000,0x018000,0x01C000,
0x00E000,0x007C00,0x003F80,0x000700,0x000000,0x000000,0x000000,0x000000,

// の
0x000003, // 橙
0x000000,0x000000,0x00FE00,0x03FF80,0x0721C0,0x0E30E0,0x187070,0x387030,
0x306038,0x706038,0x606038,0x60E038,0x60C038,0x60C030,0x71C070,0x3180E0,
0x3F81E0,0x1F0780,0x040F00,0x000400,0x000000,0x000000,0x000000,0x000000,

// 信
0x000003, // 橙
0x0CFFE0,0x0CFFE0,0x0C0000,0x180000,0x1BFFF8,0x1BFFF8,0x380000,0x38FFE0,
0x78FFE0,0xF80000,0xD8FFE0,0x58FFE0,0x180000,0x180000,0x18FFF0,0x18FFF0,
0x18C030,0x18C030,0x18FFF0,0x18FFF0,0x18C030,0x000000,0x000000,0x000000,

// 号
0x000003, // 橙
0x0FFFC0,0x0FFFC0,0x0C00C0,0x0C00C0,0x0C00C0,0x0FFFC0,0x0FFFC0,0x0C00C0,
0x000000,0xFFFFFC,0xFFFFFC,0x030000,0x070000,0x07FFC0,0x0FFFC0,0x0C00C0,
0x0000C0,0x0000C0,0x000180,0x001F80,0x001F00,0x000000,0x000000,0x000000,

// 機
0x000003, // 橙
0x182620,0x186660,0x186640,0x185ED8,0x7EB7F0,0x7EE6E0,0x187678,0x385EC8,
0x39FFFC,0x3DEE08,0x7E0660,0x7BFFFC,0xD9FFFC,0xD8C220,0xD8E230,0x18FB60,
0x18DBC0,0x18C388,0x1987CC,0x1B9E7C,0x1B1838,0x000000,0x000000,0x000000,

// ト
0x000003, // 橙
0x000000,0x00C000,0x00C000,0x00C000,0x00C000,0x00C000,0x00C000,0x00C000,
0x00F000,0x00FE00,0x00DFC0,0x00C3E0,0x00C0E0,0x00C000,0x00C000,0x00C000,
0x00C000,0x00C000,0x00C000,0x00C000,0x00C000,0x000000,0x000000,0x000000,

// ラ
0x000003, // 橙
0x000000,0x0FFF80,0x0FFF80,0x000000,0x000000,0x000000,0x3FFFE0,0x3FFFE0,
0x000060,0x0000E0,0x0000C0,0x0000C0,0x0001C0,0x000380,0x000700,0x000F00,
0x001E00,0x00F800,0x01F000,0x008000,0x000000,0x000000,0x000000,0x000000,

// ブ
0x000003, // 橙
0x000160,0x0003B0,0x0000D8,0x000000,0x3FFFC0,0x3FFFE0,0x0000C0,0x0000C0,
0x0000C0,0x0001C0,0x0001C0,0x000180,0x000380,0x000700,0x000E00,0x001E00,
0x007800,0x01F000,0x0FE000,0x078000,0x040000,0x000000,0x000000,0x000000,

// ル
0x000003, // 橙
0x000000,0x000000,0x003000,0x0C3000,0x0C3000,0x0C3000,0x0C3000,0x0C3000,
0x0C3000,0x0C3000,0x0C3010,0x0C3038,0x0C3070,0x1830E0,0x1831E0,0x383380,
0x303F00,0x703E00,0xE03000,0x400000,0x000000,0x000000,0x000000,0x000000,

// の
0x000003, // 橙
0x000000,0x000000,0x00FE00,0x03FF80,0x0721C0,0x0E30E0,0x187070,0x387030,
0x306038,0x706038,0x606038,0x60E038,0x60C038,0x60C030,0x71C070,0x3180E0,
0x3F81E0,0x1F0780,0x040F00,0x000400,0x000000,0x000000,0x000000,0x000000,

// 影
0x000003, // 橙
0x3FF020,0x303070,0x3030E0,0x3FF1C0,0x303780,0x303600,0x3FF000,0x030018,
0xFFF830,0xFFF8E0,0x0003C0,0x3FF780,0x3FF200,0x303008,0x3FF01C,0x3FF038,
0x136070,0x3B71E0,0x7333C0,0x6F0F00,0x0F0400,0x000000,0x000000,0x000000,

// 響
0x000003, // 橙
0x080000,0x18FDF8,0x1685B0,0x7CFDB0,0x3885A0,0x32FDB0,0xFE9998,0xF68D98,
0x1DF5B8,0x7935B0,0x603000,0x1FFFE0,0x030300,0x030300,0xFFFFFC,0x000000,
0x0FFF80,0x0C0180,0x0FFF80,0x0C0180,0x0FFF80,0x0C0180,0x000000,0x000000,

// で
0x000003, // 橙
0x000000,0x000000,0x000060,0x007FE0,0xFFFF00,0x7F3800,0x00E080,0x00C0E0,
0x018230,0x018380,0x0180C0,0x030000,0x030000,0x018000,0x018000,0x01C000,
0x00E000,0x007C00,0x003F80,0x000700,0x000000,0x000000,0x000000,0x000000,

// 、
0x000003, // 橙
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x100000,0x080000,0x060000,0x060000,0x000000,0x000000,

// 一
0x000003, // 橙

0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000020,
0x000030,0x7FFFF8,0x3FFFF8,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,

// 部
0x000003, // 橙
0x030000,0x0303F0,0x3FF3F8,0x3FF318,0x186330,0x18E330,0x18C330,0x0CC320,
0xFFFB60,0xFFFB60,0x000330,0x000318,0x3FE318,0x3FE318,0x30631C,0x306318,
0x306378,0x3FE370,0x3FE300,0x306300,0x300300,0x000000,0x000000,0x000000,

// 電
0x000003, // 橙
0x7FFFC0,0x7FFFC0,0x006000,0xFFFFF0,0xC06030,0xDF6FB0,0xC06030,0xC06030,
0x1F6F80,0x006000,0x3FFFC0,0x3FFFC0,0x3060C0,0x3FFFC0,0x3FFFC0,0x3060C0,
0x3FFFC0,0x3FFFD8,0x306018,0x007FF8,0x007FF0,0x000000,0x000000,0x000000,

// 車
0x000003, // 橙
0x003000,0x003000,0x7FFFF0,0x7FFFF0,0x003000,0x1FFFE0,0x1FFFE0,0x183060,
0x183060,0x1FFFE0,0x1FFFE0,0x183060,0x1FFFE0,0x1FFFE0,0x003000,0xFFFFFC,
0xFFFFFC,0x003000,0x003000,0x003000,0x003000,0x000000,0x000000,0x000000,
// に
0x000003, // 橙
0x000000,0x000000,0x100000,0x180000,0x380000,0x387FE0,0x307FE0,0x300000,
0x300000,0x300000,0x300000,0x300000,0x300000,0x300000,0x300000,0x306000,
0x307800,0x303FF8,0x3007F8,0x300000,0x300000,0x000000,0x000000,0x000000,

// 遅
0x000003, // 橙
0x01FFF0,0x31FFF0,0x398030,0x198030,0x01FFF0,0x01FFF0,0x018CC0,0x018CC0,
0x01FFF8,0xF9FFF8,0xF98300,0x19BFF0,0x19BFF0,0x1B8300,0x1B7FF8,0x1B7FF8,
0x1A0300,0x3C0300,0x778000,0x63FFFC,0x40FFFC,0x000000,0x000000,0x000000,

// れ
0x000003, // 橙
0x060000,0x060000,0x060000,0x060000,0x060000,0x079F00,0x7F3F80,0x777180,
0x06C180,0x078180,0x070300,0x0E0300,0x1E0300,0x3E0300,0x360308,0x66030C,
0xE60318,0x4603F8,0x0601E0,0x060000,0x060000,0x000000,0x000000,0x000000,

// が
0x000003, // 橙
0x000040,0x020070,0x038038,0x0301C8,0x0300E0,0x060120,0x060380,0xFFE180,
0xFFF1C0,0x0C30E0,0x0C3060,0x0C3070,0x183030,0x183030,0x383030,0x303000,
0x303000,0x623000,0xE7F000,0xC3E000,0x400000,0x000000,0x000000,0x000000,

// 生
0x000003, // 橙
0x006000,0x086000,0x0C6000,0x186000,0x1FFFE0,0x1FFFE0,0x386000,0x306000,
0x706000,0xE06000,0xC06000,0x1FFFC0,0x1FFFC0,0x006000,0x006000,0x006000,
0x006000,0x006000,0xFFFFF0,0xFFFFF0,0x000000,0x000000,0x000000,0x000000,

// じ
0x000003, // 橙
0x000000,0x180000,0x1C1C00,0x180600,0x183200,0x181800,0x180800,0x180000,
0x180000,0x180000,0x180000,0x180000,0x180000,0x180000,0x180000,0x180080,
0x0C01C0,0x0C0380,0x0E0F00,0x07FE00,0x03F800,0x000000,0x000000,0x000000,

// て
0x000003, // 橙
0x000000,0x000000,0x000060,0x007FE0,0xFFFF00,0x7F3800,0x00E000,0x00C000,
0x018000,0x018000,0x018000,0x030000,0x030000,0x018000,0x018000,0x01C000,
0x00E000,0x007C00,0x003F80,0x000700,0x000000,0x000000,0x000000,0x000000,

// い
0x000003, // 橙
0x000000,0x000000,0x000000,0x180000,0x380000,0x380380,0x3801C0,0x3000E0,
0x300060,0x300070,0x300030,0x300038,0x300038,0x300038,0x384018,0x38E010,
0x18C000,0x1DC000,0x1F8000,0x0F0000,0x000000,0x000000,0x000000,0x000000,

//ま　uni307E
0x000003, // 橙
0x006000,0x006000,0x006000,0x007FC0,0xFFFFC0,0xFFE000,0x006000,0x006000,
0x006000,0x7FFF80,0x7FF800,0x006000,0x006000,0x026000,0x1FF800,0x307F00,
0x20C7C0,0x20C1C0,0x31C000,0x1F8000,0x040000,0x000000,0x000000,0x000000,

//す　3059
0x000003, // 橙
0x003000,0x003000,0x003000,0x003010,0x7FFFF0,0xFFFFC0,0x003000,0x003000,
0x03F000,0x067000,0x0C3000,0x0C3000,0x0C3000,0x07F000,0x03B000,0x003000,
0x007000,0x00E000,0x01C000,0x038000,0x030000,0x000000,0x000000,0x000000,

// 。
0x000103, // 橙 最終文字
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x038000,0x044000,
0x082000,0x082000,0x082000,0x044000,0x038000,0x000000,0x000000,0x000000
};


// 文字列データ3
//★発車間際の駆け込み乗車は大変危険です。ゆとりあるご乗車をお願い致します。
const uint32_t SIGNBOARD24_3[] = {
// 文字列データは最初のワードに色情報＋最終文字フラグを付けた24x32bitワードを１文字データとする。
// 
// 0x000000 白
// 0x000001 緑
// 0x000002 赤
// 0x000003 橙

// 0x000010 が付いていたら、点滅文字とする。
// 0x000100 が付いていたら、最終文字とする。
//

// ★
0x000002, // 赤
0x004000,0x006000,0x006000,0x00E000,0x00F000,0x00F000,0x01F000,0xFFFFF0,
0x3FFFC0,0x1FFF80,0x07FE00,0x03FC00,0x03FC00,0x07FE00,0x07FE00,0x071E00,
0x0E0700,0x0C0300,0x100100,0x000000,0x000000,0x000000,0x000000,0x000000,

// 発
0x000003, // 橙
0x002000,0x0FF080,0x0FF9C0,0x187F30,0x1CCE70,0x0F87E0,0x0703C0,0x0E01E0,
0x3FFF7C,0xF3FF38,0x40CC00,0x00CC00,0x3FFFF0,0x3FFFF0,0x018C00,0x018C00,
0x018C18,0x070C18,0x1F0E38,0x7C0FF0,0x7003E0,0x000000,0x000000,0x000000,

// 車
0x000003, // 橙
0x003000,0x003000,0x7FFFF0,0x7FFFF0,0x003000,0x1FFFE0,0x1FFFE0,0x183060,
0x183060,0x1FFFE0,0x1FFFE0,0x183060,0x1FFFE0,0x1FFFE0,0x003000,0xFFFFFC,
0xFFFFFC,0x003000,0x003000,0x003000,0x003000,0x000000,0x000000,0x000000,

// 間
0x000003, // 橙
0xFF9FF0,0xFF9FF0,0xC19830,0xFF9FF0,0xFF9FF0,0xC19830,0xFF9FF0,0xFF9FF0,
0xC00030,0xC00030,0xC7FC30,0xC7FC30,0xC60C30,0xC7FC30,0xC7FC30,0xC60C30,
0xC7FC30,0xC7FC30,0xC60C30,0xC000F0,0xC000E0,0x000000,0x000000,0x000000,

// 際
0x000003, // 橙
0x008000,0xF8C600,0xFCF6F0,0xCDFAF8,0xCBB330,0xDB7360,0xDF61C0,0xD0E1C0,
0xF0FFE0,0xF1FFF0,0xDB8038,0xDB0018,0xCC0000,0xCDFFE0,0xCDFFE0,0xCC0C00,
0xCCECC0,0xFCCCE0,0xD9CC70,0xC38C30,0xC13C00,0xC03C00,0x000000,0x000000,

// の
0x000003, // 橙
0x000000,0x000000,0x000000,0x00FE00,0x03FF80,0x0721C0,0x0E30E0,0x187070,
0x387030,0x306038,0x706038,0x606038,0x60E038,0x60C038,0x60C030,0x71C070,
0x3180E0,0x3F81E0,0x1F0780,0x040F00,0x000400,0x000000,0x000000,0x000000,

// 駆
0x000012, // 点滅赤
0x7FE000,0x7FFFF8,0x661FF8,0x661820,0x7FD830,0x7FDB30,0x661BB0,0x6619A0,
0x7FD8E0,0x7FD8E0,0x6618E0,0x7FF8F0,0x7FF9B0,0x00F998,0x55DB18,0x55DB10,
0x55D800,0x55D800,0xD05FFC,0x43DFFC,0x039800,0x000000,0x000000,0x000000,

// け
0x000012, // 点滅赤
0x000000,0x000300,0x1C0300,0x180300,0x180300,0x180300,0x3803F8,0x30FFF8,
0x30FF00,0x300300,0x300300,0x300300,0x300300,0x300300,0x300300,0x380300,
0x180600,0x180600,0x100E00,0x001C00,0x000800,0x000000,0x000000,0x000000,

// 込
0x000012, // 点滅赤
0x200000,0x70FC00,0x38FC00,0x1C0C00,0x080C00,0x000C00,0x000E00,0x001E00,
0x001E00,0xF81B00,0xF83300,0x187380,0x18E1C0,0x19E0E0,0x1BC070,0x1B8030,
0x190000,0x3C0000,0xEFFFF8,0xC3FFF8,0x800000,0x002000,0x000000,0x000000,

// み
0x000012, // 点滅赤
0x000000,0x002000,0x03F000,0x0FF000,0x0C7000,0x0060C0,0x0060C0,0x00C0C0,
0x00C0C0,0x0FF0C0,0x1FFFC0,0x3187C0,0x6300F8,0xC301BC,0xC60188,0xCE0380,
0xFC0300,0x780700,0x000E00,0x000C00,0x000000,0x000000,0x000000,0x000000,

// 乗
0x000012, // 点滅赤
0x000380,0x1FFFC0,0x1FF800,0x003000,0x3FFFF0,0x3FFFF0,0x063180,0x063180,
0x7FFFF8,0x7FFFF8,0x063180,0x063180,0x3FFFF0,0x3FFFF0,0x00FC00,0x01B600,
0x073380,0x1E31E0,0xFC30FC,0x703038,0x003000,0x000000,0x000000,0x000000,

// 車
0x000012, // 点滅赤
0x003000,0x003000,0x7FFFF0,0x7FFFF0,0x003000,0x1FFFE0,0x1FFFE0,0x183060,
0x183060,0x1FFFE0,0x1FFFE0,0x183060,0x1FFFE0,0x1FFFE0,0x003000,0xFFFFFC,
0xFFFFFC,0x003000,0x003000,0x003000,0x003000,0x000000,0x000000,0x000000,

// は
0x000012, // 点滅赤
0x000000,0x000000,0x200600,0x300600,0x700600,0x600600,0x60FFF0,0x63FFF0,
0x620600,0x600600,0x600600,0x600600,0x600600,0x600600,0x607E00,0x61FF00,
0x6187C0,0x6186F0,0x618630,0x61FE10,0x00F800,0x000000,0x000000,0x000000,

// 大
0x000012, // 点滅赤
0x006000,0x006000,0x006000,0x006000,0x006000,0xFFFFF0,0xFFFFF0,0x006000,
0x007000,0x00F000,0x00F000,0x00D800,0x019C00,0x018C00,0x038E00,0x070700,
0x0E03C0,0x3C01F0,0xF800F0,0xE00030,0x800000,0x000000,0x000000,0x000000,

// 変
0x000012, // 点滅赤
0x006000,0x006000,0x006000,0xFFFFF0,0xFFFFF0,0x019800,0x099900,0x1D9BC0,
0x7999E0,0xF398F0,0x473860,0x0E3800,0x3D8000,0x31FF00,0x03FF00,0x0F8600,
0x1CDC00,0x787800,0x20FC00,0x07DFC0,0xFF07F0,0xF80070,0x000000,0x000000,

// 危
0x000012, // 点滅赤
0x030000,0x03FF00,0x07FF00,0x060600,0x0C0C00,0x1FFFF8,0x7FFFF8,0xF80000,
0x580000,0x18FF80,0x18FF80,0x18C180,0x18C180,0x18C180,0x18C780,0x18C380,
0x18C018,0x30C01C,0x70C018,0x60FFF8,0x407FF0,0x000000,0x000000,0x000000,

// 険
0x000012, // 点滅赤
0x001C00,0xFC1E00,0xFC3300,0xCC73C0,0xC9E0F8,0xDB8078,0xDB7F80,0xD87F80,
0xD80C00,0xD9FFE0,0xCDFFE0,0xCD8C60,0xCD8C60,0xC7FFE0,0xCFFFE0,0xDC0E00,
0xDC1B00,0xC039C0,0xC0F0F0,0xC7C078,0xC70018,0x000000,0x000000,0x000000,

// で
0x000003, // 橙
0x000000,0x000000,0x000060,0x007FE0,0xFFFF00,0x7F3800,0x00E080,0x00C0E0,
0x018230,0x018380,0x0180C0,0x030000,0x030000,0x018000,0x018000,0x01C000,
0x00E000,0x007C00,0x003F80,0x000700,0x000000,0x000000,0x000000,0x000000,

// す
0x000003, // 橙
0x003000,0x003000,0x003000,0x003010,0x7FFFF0,0xFFFFC0,0x003000,0x003000,
0x03F000,0x067000,0x0C3000,0x0C3000,0x0C3000,0x07F000,0x03B000,0x003000,
0x007000,0x00E000,0x01C000,0x038000,0x030000,0x000000,0x000000,0x000000,

// 。
0x000003, // 橙
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x038000,0x044000,
0x082000,0x082000,0x082000,0x044000,0x038000,0x000000,0x000000,0x000000,

// ゆ
0x000001, // 緑
0x003000,0x003000,0x003000,0xE03E00,0xC1FF80,0xC3B1C0,0xC630C0,0xCC30E0,
0xD83060,0xF03060,0xE03060,0xE23060,0xC330C0,0xC3B0C0,0xC1FF80,0xC0FF00,
0xC06000,0xC06000,0x00E000,0x01C000,0x008000,0x000000,0x000000,0x000000,

// と
0x000001, // 緑
0x018000,0x018000,0x018000,0x018000,0x018000,0x018000,0x018080,0x018780,
0x01BF80,0x00FC00,0x03E000,0x070000,0x1E0000,0x180000,0x380000,0x380000,
0x380000,0x1C0000,0x1FFF80,0x03FF80,0x000000,0x000000,0x000000,0x000000,

// り
0x000001, // 緑
0x000300,0x0E0300,0x0C0300,0x0C0300,0x0C0300,0x0C0300,0x0C0300,0x0C0300,
0x0C0300,0x0C4300,0x0CE300,0x0DC300,0x0F8300,0x070300,0x000700,0x000600,
0x000E00,0x001C00,0x007800,0x00F000,0x004000,0x000000,0x000000,0x000000,

// あ
0x000001, // 緑
0x030000,0x030000,0x060000,0x067E00,0x7FFE00,0x7F0000,0x061000,0x061800,
0x07FE00,0x07FF80,0x1E31C0,0x3E30E0,0x733060,0x636060,0x436060,0xC3C060,
0xC18060,0xC380E0,0x7E83C0,0x7C1F80,0x000E00,0x000000,0x000000,0x000000,

// る
0x000001, // 緑
0x001800,0x0FFC00,0x0FF800,0x003000,0x006000,0x00C000,0x018000,0x038000,
0x07FC00,0x0FFF00,0x1E0380,0x380180,0x7001C0,0xE380C0,0x47C0C0,0x0460C0,
0x043180,0x063380,0x073F00,0x03FE00,0x003000,0x000000,0x000000,0x000000,

// ご
0x000001, // 緑
0x000080,0x0001C0,0x000260,0x0003A0,0x3FFDC0,0x3FFC40,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x600000,0xE00000,
0x600000,0x700000,0x7FFF00,0x1FFF00,0x000000,0x000000,0x000000,0x000000,

// 乗
0x000001, // 緑
0x000380,0x1FFFC0,0x1FF800,0x003000,0x3FFFF0,0x3FFFF0,0x063180,0x063180,
0x7FFFF8,0x7FFFF8,0x063180,0x063180,0x3FFFF0,0x3FFFF0,0x00FC00,0x01B600,
0x073380,0x1E31E0,0xFC30FC,0x703038,0x003000,0x000000,0x000000,0x000000,

// 車
0x000001, // 緑
0x003000,0x003000,0x7FFFF0,0x7FFFF0,0x003000,0x1FFFE0,0x1FFFE0,0x183060,
0x183060,0x1FFFE0,0x1FFFE0,0x183060,0x1FFFE0,0x1FFFE0,0x003000,0xFFFFFC,
0xFFFFFC,0x003000,0x003000,0x003000,0x003000,0x000000,0x000000,0x000000,

// を
0x000001, // 緑
0x018000,0x018000,0x038000,0x07FE00,0xFFFE00,0x860000,0x060000,0x0C0000,
0x1FC000,0x3DC380,0xF07FC0,0xE0FC00,0x03E000,0x076000,0x1C6000,0x186000,
0x306000,0x300000,0x380000,0x1FFE00,0x0FFE00,0x000000,0x000000,0x000000,

// お
0x000001, // 緑
0x018000,0x018000,0x018080,0x0189C0,0x01F8E0,0x3FF870,0x3F8038,0x018010,
0x018000,0x019C00,0x01FFC0,0x07C0E0,0x0F8070,0x1D8030,0x398030,0x318030,
0x318830,0x319C70,0x330FE0,0x1F07C0,0x0C0000,0x000000,0x000000,0x000000,

// 願
0x000001, // 緑
0x7FEFFC,0x7FEFFC,0x610080,0x630180,0x630FF8,0x6FEFF8,0x6FEC18,0x6C6C18,
0x6FEFF8,0x6FEFF8,0x6C6C18,0x6C6C18,0x6FEFF8,0x6FEFF8,0x6DEC18,0x6DBC18,
0x6DBFF8,0x79BFF8,0xD9A770,0xCB9E1C,0xC39804,0x000000,0x000000,0x000000,

// い
0x000001, // 緑
0x000000,0x000000,0x000000,0x180000,0x380000,0x380380,0x3801C0,0x3000E0,
0x300060,0x300070,0x300030,0x300038,0x300038,0x300038,0x384018,0x38E010,
0x18C000,0x1DC000,0x1F8000,0x0F0000,0x000000,0x000000,0x000000,0x000000,

// 致
0x000001, // 緑
0x000300,0x7FE600,0x7FE600,0x0C0600,0x0D87FC,0x19CFFC,0x18CC30,0xFFFE30,
0x7E5E30,0x063E30,0x061260,0x060260,0x3FC360,0x3FC3C0,0x0601C0,0x0601C0,
0x06E3E0,0x0FEF78,0x7E3C3C,0x60781C,0x006000,0x000000,0x000000,0x000000,

// し
0x000001, // 緑
0x1C0000,0x180000,0x180000,0x180000,0x180000,0x180000,0x180000,0x180000,
0x180000,0x180000,0x180000,0x180000,0x180000,0x180000,0x180000,0x180080,
0x0C01C0,0x0C0380,0x0E0F00,0x07FE00,0x03F800,0x000000,0x000000,0x000000,

// ま
0x000001, // 緑
0x006000,0x006000,0x006000,0x007FC0,0xFFFFC0,0xFFE000,0x006000,0x006000,
0x006000,0x7FFF80,0x7FF800,0x006000,0x006000,0x026000,0x1FF800,0x307F00,
0x20C7C0,0x20C1C0,0x31C000,0x1F8000,0x040000,0x000000,0x000000,0x000000,

// す
0x000001, // 緑
0x003000,0x003000,0x003000,0x003010,0x7FFFF0,0xFFFFC0,0x003000,0x003000,
0x03F000,0x067000,0x0C3000,0x0C3000,0x0C3000,0x07F000,0x03B000,0x003000,
0x007000,0x00E000,0x01C000,0x038000,0x030000,0x000000,0x000000,0x000000,

// 。
0x000101, // 緑 最終文字
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x038000,0x044000,
0x082000,0x082000,0x082000,0x044000,0x038000,0x000000,0x000000,0x000000
};
