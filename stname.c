///////////////////////////////////////////////
// 目的地等駅名フォント 数値フォントデータ
// stname.c
///////////////////////////////////////////////

#include "pico/stdlib.h"
#include <stdio.h>

// 駅名データ　24ビットフォント4文字　日本語 英語の順
// 行き先駅順番
// 0	上野東京方面
// 1	池袋新宿方面
// 2	大船
// 3	磯子
// 4	蒲田
// 5	桜木町
// 6	鶴見
// 7	東神奈川
const uint32_t STNAME_FONT24[] ={
// 上野 東京 方面
0x000000,0x00800F,0x008008,0x008008,0x008008,0x00800F,0x008008,0x008408,
0x00FE08,0x00800F,0x008000,0x008000,0x008000,0x008000,0x00800F,0x008000,
0x008000,0x008000,0x008000,0x008000,0x008041,0xFFFFCE,0x000000,0x000000,

0x000800,0xFBFC00,0x880800,0x891000,0x88E000,0xF82000,0x881000,0x880000,
0x8FFC00,0xF84400,0x804478,0x804878,0x805078,0x804078,0xF84000,0x804000,
0x804000,0x804000,0x804000,0x884000,0xF04000,0x004000,0x07C000,0x000000,

0x008000,0x008200,0x7FFF3F,0x008000,0x008000,0x3FFE00,0x20820F,0x208208,
0x208208,0x208208,0x3FFE08,0x208208,0x208208,0x20820F,0x3FFE00,0x02A001,
0x049002,0x088804,0x108408,0x208210,0x4081A0,0x018001,0x008000,0x000000,

0x400040,0x411FFE,0xFF8040,0x000040,0x0000F8,0x020104,0xFF0204,0x020404,
0x020804,0x021004,0x0200F8,0x020000,0x021FFE,0xFE0080,0x400100,0x501FFE,
0x481122,0x4411E2,0x421122,0x411122,0x40D1E2,0xC01122,0x401FFE,0x000000,

// UENO & TOKYO
0x205FE8,0x20500C,0x20500A,0x20500A,0x20500A,0x205FC9,0x205008,0x205008,
0x205008,0x109008,0x0F1FE8,0x000000,0x0F0000,0x108000,0x204000,0x204000,
0x108000,0x1F1000,0x222000,0x214000,0x208000,0x114000,0x0E3000,0x000000,

0x11E000,0x121000,0x140800,0x140800,0x140800,0x940800,0x540800,0x540800,
0x540800,0x321000,0x11E000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,

0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0xFF9F88,0x080008,0x080008,0x080F08,
0x081089,0x08204E,0x082049,0x082048,0x082048,0x081088,0x080F08,0x000000,

0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x1413F0,0x241000,0x441000,0x8411E0,
0x022210,0x014408,0x008408,0x808408,0x408408,0x208210,0x1081E0,0x000000,

//池袋・新宿方面
0x000000,0x801000,0x401000,0x201001,0x111083,0x0111C5,0x811289,0x411481,
0x213881,0x115081,0x019081,0x071081,0x111080,0x21110F,0x211600,0x211000,
0xC11000,0x411003,0x41104D,0x410041,0x408041,0x807F86,0x000000,0x000000,

0x003800,0x840000,0x847000,0x058003,0x060001,0x3A0001,0x020001,0x010000,
0x008807,0x005000,0x0021E0,0x1011E3,0x1001E0,0xFFF9E1,0x180001,0x641001,
0x821001,0x011002,0x00A002,0x404002,0x802002,0x001804,0x000000,0x000000,

0x000000,0x402000,0x404000,0xF980FF,0x120080,0x120088,0x120090,0x920817,
0xFBFC30,0x422030,0x422050,0xFA2053,0xE22092,0x522092,0x4A2112,0x4A2013,
0x4A2012,0x4A2012,0x4A2012,0x4A2012,0x422013,0x042012,0x000000,0x000000,

0x000040,0x801FFE,0x800040,0xFF8040,0x0080F8,0x008104,0x020204,0xFF0404,
0x200804,0x401004,0x8400F8,0xFE0000,0x021FFE,0x020080,0x020100,0xFE1FFE,
0x021122,0x0211E2,0x021122,0x021122,0xFE11E2,0x021122,0x001FFE,0x000000,

//IKEBUKURO & SHINJUKU
0x1D02FF,0x090480,0x090880,0x091080,0x092080,0x09C0FE,0x092080,0x091080,
0x090880,0x090480,0x1D02FF,0x000000,0x0F0000,0x108000,0x204000,0x204000,
0x108000,0x1F1000,0x222000,0x214000,0x208000,0x114000,0x0E3000,0x000000,

0x7E2050,0x412050,0x40A050,0x40A051,0x412052,0x7E205C,0x412052,0x40A051,
0x40A050,0x411090,0x7E0F10,0x000000,0x1E205D,0x212049,0x40A049,0x40A049,
0x202049,0x1E3FC9,0x012049,0x40A049,0x40A049,0x212049,0x1E205D,0x000000,

0x2817E0,0x481411,0x88140A,0x08140A,0x081412,0x0817E2,0x081482,0x081442,
0x881422,0x442411,0x23C408,0x000000,0x020E81,0x820481,0x420481,0x420481,
0x220481,0x220481,0x120481,0x0A0481,0x0A8481,0x064842,0x02303C,0x000000,

0xF00000,0x080000,0x040000,0x040000,0x040000,0x040000,0x040000,0x040000,
0x040000,0x080000,0xF00000,0x000000,0x40A040,0x412040,0x422040,0x442040,
0x482040,0x702040,0x482040,0x442040,0x422040,0x411080,0x408F00,0x000000,
	
//大船
0x000006,0x000006,0x000006,0x000006,0x000006,0x000FFF,0x000FFF,0x000006,
0x000007,0x00000F,0x00000F,0x00000D,0x000019,0x000018,0x000038,0x000070,
0x0000E0,0x0003C0,0x000F80,0x000E00,0x000800,0x000000,0x000000,0x000000,

0x000000,0x000000,0x000000,0x000000,0x000000,0xFF0000,0xFF0000,0x000000,
0x000000,0x000000,0x000000,0x800000,0xC00000,0xC00000,0xE00000,0x700000,
0x3C0000,0x1E0000,0x0F0000,0x030000,0x010000,0x000000,0x000000,0x000000,

0x0000E0,0x0000C0,0x0003F8,0x0003F8,0x000318,0x0003D8,0x000359,0x000379,
0x00031B,0x000FFD,0x000FFC,0x000319,0x000359,0x000359,0x000359,0x000359,
0x000659,0x0006D9,0x000619,0x000C39,0x000439,0x000000,0x000000,0x000000,

0x440000,0x640000,0x660000,0xC60000,0xC60000,0xC30000,0x830000,0x818000,
0x01C000,0x00C000,0x000000,0xFF0000,0xFF0000,0x830000,0x830000,0x830000,
0x830000,0x830000,0xFF0000,0xFF0000,0x810000,0x000000,0x000000,0x000000,

// OFUNA
0xFFFE1F,0xFFFE1F,0x000018,0x1FF018,0x3FF818,0x701C18,0x600C18,0xE00E18,
0xC0061F,0xC0061F,0xC00618,0xC00618,0xC00618,0xC00618,0xC00618,0xC00618,
0xC00618,0xE00E18,0x600C18,0x701C18,0x3FF818,0x1FF018,0x000000,0x000000,

0xFF8600,0xFF8600,0x000600,0x000600,0x000600,0x000600,0x000600,0x000600,
0xFF0600,0xFF0600,0x000600,0x000600,0x000600,0x000600,0x000600,0x000600,
0x000600,0x000700,0x000300,0x000380,0x0001FF,0x0000FF,0x000000,0x000000,

0x30C006,0x30E006,0x30F006,0x30D006,0x30D806,0x30CC06,0x30C406,0x30C606,
0x30C306,0x30C306,0x30C106,0x30C186,0x30C0C6,0x30C0C6,0x30C066,0x30C066,
0x30C036,0x70C036,0x60C016,0xE0C00E,0xC0C00E,0x80C006,0x000000,0x000000,

0x007000,0x00F800,0x01FC00,0x038E00,0x070700,0x0E0380,0x1C01C0,0x1800C0,
0x1800C0,0x1800C0,0x1800C0,0x1800C0,0x1800C0,0x1FFFC0,0x1FFFC0,0x1800C0,
0x1800C0,0x1800C0,0x1800C0,0x1800C0,0x1800C0,0x1800C0,0x000000,0x000000,

//磯子
0x000003,0x0007E3,0x0007E2,0x000306,0x00030D,0x00030F,0x000307,0x000306,
0x0007E4,0x0007EF,0x000F60,0x000F60,0x000B6F,0x00036F,0x000366,0x000367,
0x0003E6,0x0003E6,0x00030E,0x00033D,0x000018,0x000000,0x000000,0x000000,

0x310000,0x330000,0x320000,0xB48000,0xBD8000,0x3F0000,0x320000,0xB68000,
0xFFC000,0xFC4000,0x320000,0x330000,0xFFC000,0xFFC000,0x310000,0x930000,
0xDE0000,0x1CC000,0x3FC000,0xE3C000,0x818000,0x000000,0x000000,0x000000,

0x0001FF,0x0001FF,0x000000,0x000000,0x000002,0x000003,0x000003,0x000003,
0x000003,0x000FFF,0x000FFF,0x000003,0x000003,0x000003,0x000003,0x000003,
0x000003,0x000003,0x000003,0x00000F,0x00000F,0x000000,0x000000,0x000000,

0xFC0000,0xFE0000,0x1C0000,0x300000,0xE00000,0xC00000,0x000000,0x000000,
0x000000,0xFFC000,0xFFC000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,

//ISOGO
0x1FE01F,0x1FE03F,0x030070,0x030060,0x0300C0,0x0300C0,0x0300C0,0x0300E0,
0x0300E0,0x030070,0x03003F,0x03001F,0x030000,0x030000,0x030000,0x0300C0,
0x0300C0,0x0300E0,0x030060,0x030070,0x1FE03F,0x1FE01F,0x000000,0x000000,

0xF003FE,0xF807FF,0x1C0E03,0x0C0C01,0x0E1801,0x061800,0x061800,0x001800,
0x001800,0x001800,0xF01800,0xF81800,0x1C1800,0x0C1800,0x0E1800,0x061800,
0x061800,0x0E1C01,0x0C0C01,0x1C0E03,0xF807FF,0xF003FE,0x000000,0x000000,

0x007FC0,0x00FFE0,0x81C070,0x818030,0xC30038,0xC30018,0xC30000,0xC30000,
0xC30000,0xC30000,0xC307F8,0xC307F8,0xC30018,0xC30018,0xC30018,0xC30018,
0xC30018,0xC38078,0x818078,0x81C0D8,0x00FFD8,0x007F98,0x000000,0x000000,

0x0FF800,0x1FFC00,0x380E00,0x300600,0x600700,0x600300,0x600300,0x600300,
0x600300,0x600300,0x600300,0x600300,0x600300,0x600300,0x600300,0x600300,
0x600300,0x700700,0x300600,0x380E00,0x1FFC00,0x0FF800,0x000000,0x000000,

// 蒲田
0x00000C,0x00000C,0x0001FF,0x0001FF,0x00000C,0x000000,0x0000E0,0x000077,
0x000027,0x000000,0x0001C7,0x0000E7,0x000046,0x000007,0x000037,0x000036,
0x000067,0x0000E7,0x0001C6,0x000186,0x000106,0x000000,0x000000,0x000000,

0x180000,0x180000,0xFFE000,0xFFE000,0x010000,0x1BC000,0x188000,0xFFF000,
0xFFF000,0x180000,0xFFC000,0xFFC000,0x18C000,0xFFC000,0xFFC000,0x18C000,
0xFFC000,0xFFC000,0x18C000,0x19C000,0x19C000,0x000000,0x000000,0x000000,

0x000000,0x000000,0x000000,0x0001FF,0x0001FF,0x000181,0x000181,0x000181,
0x000181,0x000181,0x0001FF,0x0001FF,0x000181,0x000181,0x000181,0x000181,
0x000181,0x000181,0x0001FF,0x0001FF,0x000180,0x000000,0x000000,0x000000,

0x000000,0x000000,0x000000,0xFF8000,0xFF8000,0x818000,0x818000,0x818000,
0x818000,0x818000,0xFF8000,0xFF8000,0x818000,0x818000,0x818000,0x818000,
0x818000,0x818000,0xFF8000,0xFF8000,0x018000,0x000000,0x000000,0x000000,

//KAMATA
0xC01803,0xC03807,0xC0700F,0xC0E01C,0xC1C038,0xC38070,0xC700E0,0xCE00C0,
0xFC00C0,0xF800C0,0xF800C0,0xDC00C0,0xCE00C0,0xC700FF,0xC380FF,0xC1C0C0,
0xC0E0C0,0xC070C0,0xC038C0,0xC01CC0,0xC00EC0,0xC006C0,0x000000,0x000000,

0x80C006,0xC0E00E,0xE0F01E,0x70D016,0x38D836,0x1CCC66,0x0EC446,0x06C6C6,
0x06C386,0x06C386,0x06C006,0x06C006,0x06C006,0xFEC006,0xFEC006,0x06C006,
0x06C006,0x06C006,0x06C006,0x06C006,0x06C006,0x06C006,0x000000,0x000000,

0x0380FF,0x07C0FF,0x0FE003,0x1C7003,0x383803,0x701C03,0xE00E03,0xC00603,
0xC00603,0xC00603,0xC00603,0xC00603,0xC00603,0xFFFE03,0xFFFE03,0xC00603,
0xC00603,0xC00603,0xC00603,0xC00603,0xC00603,0xC00603,0x000000,0x000000,

0xFE0380,0xFE07C0,0x000FE0,0x001C70,0x003838,0x00701C,0x00E00E,0x00C006,
0x00C006,0x00C006,0x00C006,0x00C006,0x00C006,0x00FFFE,0x00FFFE,0x00C006,
0x00C006,0x00C006,0x00C006,0x00C006,0x00C006,0x00C006,0x000000,0x000000,

// 桜木町
0x000C04,0x000C4E,0x000CE6,0x000C67,0x007F73,0x007F32,0x000C20,0x001C02,
0x001C06,0x001E06,0x003F7F,0x003F7F,0x006D0C,0x00EC18,0x00CC3C,0x004C3F,
0x000C23,0x000C07,0x000C1E,0x000CFC,0x000CE0,0x000000,0x000000,0x000000,

0x000003,0x180003,0x380003,0x300003,0x3007FF,0x7007FF,0x600007,0x000007,
0x000007,0x00000F,0xFC001B,0xFC001B,0x600033,0x600063,0xE001C3,0xC00383,
0xC00F03,0xF00603,0x780003,0x180003,0x000003,0x000000,0x000000,0x000000,

0x000000,0x0000FF,0x0000FF,0x0000CC,0xFF80CC,0xFF80CC,0x0000CC,0x8000FF,
0x8000FF,0xC000CC,0xE000CC,0x7000CC,0x3800CC,0x1C00CC,0x0E00CC,0x0780FF,
0x0380FF,0x0180C0,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,

0x000000,0xC00000,0xC00000,0xC00000,0xFFF800,0xFFF800,0xC18000,0xC18000,
0xC18000,0xC18000,0xC18000,0xC18000,0xC18000,0xC18000,0xC18000,0xC18000,
0xC18000,0xC18000,0x018000,0x078000,0x070000,0x000000,0x000000,0x000000,

// SAKURAGI-CHO
0x3E07C3,0x7F0FE3,0xE19833,0xC19833,0xC01833,0x7E1FF3,0x3F1FF3,0x019833,
0xC19833,0xC19833,0x7F1833,0x000000,0x3E1833,0x7F1833,0xE19830,0xC19830,
0xC01831,0xC01FF3,0xC01FF3,0xC19833,0xE19833,0x7F1831,0x3E1830,0x000000,

0x06C19F,0x0EC19F,0x1CC198,0x38C198,0xE0C198,0xE0C19F,0x38C19F,0x1CC19F,
0x0EE39B,0x067F18,0x063E18,0x000000,0xFE0000,0xFE0000,0x000000,0xF80000,
0xFC0000,0x8E0000,0x060000,0x060000,0x8E0000,0xFC0000,0xF80000,0x000000,

0xC0F81F,0xE1FC3F,0x330660,0x330660,0x730660,0xE3FE6F,0xC3FE6F,0x030660,
0xC30660,0xF30670,0x33063F,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,

0x0FC000,0x8FC000,0xC30000,0xC30000,0x030000,0xC30F00,0xC30F00,0xC30000,
0xC30000,0xCFC000,0xCFC000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,

//鶴見
0x000000,0x000060,0x000040,0x0007FD,0x0007FD,0x0004D5,0x0004B5,0x0001B1,
0x0003FD,0x0003FD,0x000731,0x000731,0x0003FD,0x0003FD,0x000331,0x0003FD,
0x0003FC,0x000331,0x000331,0x0003FD,0x0003FE,0x000300,0x000000,0x000000,

0x100000,0x180000,0x100000,0xFF0000,0xFF0000,0x830000,0xFF0000,0x830000,
0xFF0000,0xFF0000,0x800000,0xFFC000,0xFFC000,0x800000,0xFFC000,0xFFC000,
0x04C000,0x56C000,0x5A8000,0x588000,0x478000,0x078000,0x000000,0x000000,

0x000000,0x0000FF,0x0000FF,0x0000C0,0x0000C0,0x0000FF,0x0000FF,0x0000C0,
0x0000C0,0x0000FF,0x0000FF,0x0000C0,0x0000FF,0x0000FF,0x0000CC,0x00000C,
0x00000C,0x00001C,0x000038,0x0000F0,0x0007E0,0x000780,0x000000,0x000000,

0x000000,0xFC0000,0xFC0000,0x0C0000,0x0C0000,0xFC0000,0xFC0000,0x0C0000,
0x0C0000,0xFC0000,0xFC0000,0x0C0000,0xFC0000,0xFC0000,0xCC0000,0xC00000,
0xC00000,0xC00000,0xC1C000,0xC18000,0xFF8000,0x7F0000,0x000000,0x000000,

//TSURUMI
0xFFF0FE,0xFFF1FF,0x060383,0x060301,0x060601,0x060600,0x060600,0x060700,
0x060700,0x060380,0x0601FE,0x0600FF,0x060003,0x060001,0x060001,0x060600,
0x060600,0x060701,0x060301,0x060383,0x0601FF,0x0600FE,0x000000,0x000000,

0x18037F,0x18037F,0x980360,0x980360,0xD80360,0xD80360,0xD80360,0x180360,
0x18037F,0x18037F,0x180378,0x18037C,0x98036C,0x980366,0xD80367,0xD80363,
0xD80361,0xDC0761,0x8C0660,0x8E0E60,0x07FC60,0x03F860,0x000000,0x000000,

0xF18036,0xF98037,0x1D8037,0x0D8036,0x0D8036,0x0D8036,0x0D8036,0x1D8036,
0xF98036,0xF18036,0x018036,0x018036,0x018036,0x018036,0x018036,0x018036,
0x818036,0xC1C076,0xC0C066,0x60E0E6,0x707FC6,0x383F86,0x000000,0x000000,

0x0037F8,0x0077F8,0x80F0C0,0x80B0C0,0xC1B0C0,0x6330C0,0x2230C0,0x3630C0,
0x1C30C0,0x1C30C0,0x0030C0,0x0030C0,0x0030C0,0x0030C0,0x0030C0,0x0030C0,
0x0030C0,0x0030C0,0x0030C0,0x0030C0,0x0037F8,0x0037F8,0x000000,0x000000,

//東神奈川
0x006000,0x006000,0xFFFFF0,0xFFFFF0,0x006000,0x1FFF80,0x1FFF80,0x186180,
0x186180,0x1FFF80,0x1FFF80,0x186180,0x1FFF80,0x1FFF80,0x00F000,0x01FC00,
0x076700,0x1E63E0,0xF861F8,0xE06070,0x006000,0x000000,0x000000,0x000000,

0x0C0000,0x0C0180,0x0C0180,0x0C0180,0x0C0180,0x7FFFF8,0x7FBFF8,0x033198,
0x033198,0x063FF8,0x0E3FF8,0x1F3198,0x3FF198,0x7DB198,0xEC3FF8,0x0C3FF8,
0x0C3198,0x0C0180,0x0C0180,0x0C0180,0x0C0180,0x0C0180,0x000000,0x000000,

0x003000,0x006000,0x006000,0x7FFFF8,0x7FFFF8,0x00CC00,0x018600,0x038380,
0x0E01E0,0x7C00FC,0xF3FF18,0x03FF00,0x000000,0x3FFFF0,0x3FFFF0,0x003000,
0x033100,0x073380,0x0E31E0,0x3C30F8,0x70F030,0x20F000,0x000000,0x000000,

0x000000,0x000000,0x1C0030,0x181830,0x181830,0x181830,0x181830,0x181830,
0x181830,0x181830,0x181830,0x181830,0x181830,0x181830,0x181830,0x181830,
0x181830,0x181830,0x301830,0x700030,0x600030,0x000000,0x000000,0x000000,

//HIGASHI-KANAGAWA
0xC19F87,0xC19F8F,0xC18618,0xC18618,0xC18618,0xFF861B,0xFF861B,0xC18618,
0xC18618,0xC1861C,0xC19F8F,0x000000,0xC187C3,0xC38FE3,0xC71833,0xCE1833,
0xF81833,0xF81FF3,0xCE1FF3,0xC71833,0xC39833,0xC19833,0xC19833,0x000000,

0xC0F81F,0xE1FC3F,0x330670,0x330660,0x030660,0xF3FE3F,0xF3FE1F,0x330600,
0x330660,0x330660,0xF3063F,0x000000,0x861F03,0xC63F87,0x4660CC,0x4660CC,
0x6660CC,0x367FCD,0x367FCD,0x1660CC,0x1E60CC,0x0E60CE,0x0E60C7,0x000000,

0x0C19F8,0x8C19F8,0xCC1860,0xCC1860,0x0C1860,0x0FF860,0x8FF860,0xCC1860,
0xCC1860,0xCC1860,0x8C19F8,0x000000,0xE07C30,0xF0FE30,0x198330,0x198330,
0x018330,0xF9FF37,0xF9FF37,0x198335,0x198335,0x198338,0xF98330,0x000000,

0x000000,0x000000,0x000000,0x000000,0x000000,0xE00000,0xE00000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x61F000,0x63F800,0x660C00,0x660C00,
0x660C00,0x67FC00,0x67FC00,0x660C00,0xE60C00,0xE60C00,0x660C00,0x000000

};

// 電車種別表示用　山手線 各駅停車 快速のフォントデータ
// 24ビットフォント3文字　日本語　英語の順
const uint32_t trainTypeFONT24[] = {
//山手線
0x006000,0x006000,0x006000,0x006000,0x006000,0xC06030,0xC06030,0xC06030,
0xC06030,0xC06030,0xC06030,0xC06030,0xC06030,0xC06030,0xC06030,0xC06030,
0xC06030,0xFFFFF0,0xFFFFF0,0xC00030,0xC00030,0x000000,0x000000,0x000000,

0x000030,0x0007F0,0x3FFF00,0x3F3000,0x003000,0x003000,0x003000,0x3FFFF0,
0x003000,0x003000,0x003000,0x003000,0xFFFFFC,0xFFFFFC,0x003000,0x003000,
0x003000,0x003000,0x003000,0x03F000,0x00C000,0x000000,0x000000,0x000000,

0x000300,0x0C0300,0x183FF8,0x193FF8,0xF32018,0x732018,0x663FF8,0x3D3FF8,
0x3DA018,0x19A018,0x30BFF8,0xFF8100,0xCCC18C,0x0C7D98,0x6D0DF8,0x6D0D70,
0x4D1960,0x4D3130,0xCC7118,0xCCE11C,0x4C8F0C,0x0C0C00,0x000000,0x000000,

//yamanote line
0x821082,0x8228C6,0x8244AA,0x4444AA,0x288292,0x108282,0x108282,0x10FE82,
0x108282,0x108282,0x108282,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,

0x108238,0x288244,0x44C282,0x44A282,0x82A282,0x829282,0x828A82,0xFE8A82,
0x828E82,0x828244,0x82821C,0x000000,0x000203,0x000201,0x000201,0x000201,
0x000201,0x000201,0x000201,0x000201,0x000201,0x000201,0x0003FB,0x000000,

0xFEFE00,0x108000,0x108000,0x108000,0x108000,0x10FC7C,0x108000,0x108000,
0x108000,0x108000,0x10FE00,0x000000,0x905FC0,0x105000,0x185000,0x145000,
0x145000,0x125F80,0x115000,0x115000,0x11D000,0x105000,0x905FC0,0x000000,

//各駅停車
0x080000,0x0C003F,0x1FF83F,0x1FF826,0x381826,0x7C303F,0xE6603F,0xC3C026,
0x83C026,0x07E03F,0x1E7C3F,0x781F26,0xF0073F,0xFFFC3F,0x3FFC00,0x300C57,
0x300C55,0x300C55,0x300C54,0x3FFCD0,0x3FFC47,0x300C07,0x000000,0x000000,

0x000001,0xDFE0C1,0xDFE0C1,0x1020BF,0x1021BF,0xD02180,0xD0218F,0x102388,
0x1FE78F,0xDFE78F,0xD32D80,0x130DBF,0xD109BF,0xD189A0,0xD181AF,0xD1818F,
0xD0C180,0xD0E180,0xD06180,0xB02180,0xB02183,0x800183,0x000000,0x000000,

0xC00300,0x800300,0x800300,0xFEFFFC,0xFEFFFC,0x000300,0xF8FFFC,0x08FFFC,
0xF8C30C,0xF8C30C,0x00FFFC,0xFEFFFC,0xFEC30C,0x02FFFC,0xFAFFFC,0xF80300,
0xC1FFFE,0xC1FFFE,0xC00300,0xC00300,0xC00300,0xC00300,0x000000,0x000000,

//LOCAL
0x60007F,0x6000FF,0x6001C0,0x600180,0x600300,0x600300,0x600300,0x600300,
0x600300,0x600300,0x600300,0x600300,0x600300,0x600300,0x600300,0x600300,
0x600300,0x600300,0x600180,0x6001C0,0x7FF8FF,0x7FF87F,0x000000,0x000000,

0x80FF00,0xC1FF80,0xE381C0,0x6300C1,0x360003,0x360007,0x36000E,0x36000C,
0x36000C,0x36000C,0x36000C,0x36000C,0x36000C,0x36000F,0x36000F,0x36000C,
0x36000C,0x36000C,0x6300CC,0xE381CC,0xC1FF8C,0x80FF0C,0x000000,0x000000,

0x301800,0x781800,0xFC1800,0xCE1800,0x871800,0x039800,0x01D800,0x00D800,
0x00D800,0x00D800,0x00D800,0x00D800,0x00D800,0xFFD800,0xFFD800,0x00D800,
0x00D800,0x00D800,0x00D800,0x00D800,0x00DFFE,0x00DFFE,0x000000,0x000000,

// 快速
0x00180C,0x00180C,0x00180C,0x00180C,0x005E7F,0x007B7F,0x007B0C,0x00790C,
0x00580C,0x00D80C,0x00D9FF,0x0019FF,0x00180E,0x00180E,0x00180F,0x001819,
0x001819,0x001830,0x0018E0,0x0019C0,0x001980,0x000000,0x000000,0x000000,

0x000000,0x000000,0x000006,0x000007,0xE00003,0xE00001,0x600000,0x600000,
0x600000,0x600000,0xFC001F,0xFC001F,0x000003,0x000003,0x000003,0x800003,
0xC00003,0xE00003,0x780007,0x3C000E,0x08000C,0x000008,0x000000,0x000000,

0x00C000,0x00C000,0x00C000,0x3FFF00,0xBFFF00,0x00C000,0x3FFE00,0x3FFE00,
0x30C600,0x30C600,0x3FFE00,0x3FFE00,0x01C000,0x03E000,0x07F800,0x0EDE00,
0x7CCF80,0x30C300,0x80C000,0xF00000,0x7FFF80,0x1FFF80,0x000000,0x000000,

// RAPID
0x7FF003,0x7FF807,0x601C0F,0x600C1C,0x600C38,0x600C70,0x600CE0,0x601CC0,
0x7FF8C0,0x7FF0C0,0x7800C0,0x7C00C0,0x6C00C0,0x6600FF,0x6700FF,0x6300C0,
0x6180C0,0x61C0C0,0x60C0C0,0x6060C0,0x6070C0,0x6030C0,0x000000,0x000000,

0x00FFE1,0x80FFF1,0xC0C038,0xE0C018,0x70C018,0x38C018,0x1CC018,0x0CC038,
0x0CFFF0,0x0CFFE0,0x0CC000,0x0CC000,0x0CC000,0xFCC000,0xFCC000,0x0CC000,
0x0CC000,0x0CC000,0x0CC000,0x0CC000,0x0CC001,0x0CC001,0x000000,0x000000,

0xF9FFC0,0xF9FFE0,0x618070,0x618030,0x618018,0x618018,0x618018,0x618018,
0x618018,0x618018,0x618018,0x618018,0x618018,0x618018,0x618018,0x618018,
0x618018,0x618018,0x618030,0x618070,0xF9FFE0,0xF9FFC0,0x000000,0x000000
};


// 残り分数表示フォントデータ（日本語）「約　分後」
const uint32_t MINUTE_LEFT_J[] ={
// 約  分後
0x082000,0x082000,0x082000,0x102000,0x222000,0x423F80,0x824080,0x448080,
0x280080,0x100080,0x222080,0x422080,0x812080,0xFF1080,0x110880,0x108080,
0x500080,0x940080,0x930080,0x910100,0x910200,0x100400,0x101800,0x000000,

0x000004,0x000024,0x000024,0x000022,0x000042,0x000041,0x000080,0x000100,
0x000200,0x000400,0x0009FF,0x000010,0x000010,0x000010,0x000010,0x000010,
0x000010,0x000020,0x000040,0x000080,0x000101,0x000202,0x00043C,0x000000,

0x002040,0x002040,0x002040,0x002490,0x004510,0x008210,0x810A20,0x420940,
0x201084,0x902104,0xC8413E,0x80CFC2,0x814102,0x824102,0x8042F8,0x804408,
0x804A08,0x805110,0x8040A0,0x804040,0x0041A0,0x004618,0x005806,0x000000
};

// 残り分数表示フォントデータ（英語）「min.」
const uint32_t MINUTE_LEFT_E[] ={
// min.
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000019,0x00000A,
0x00000C,0x000008,0x000008,0x000008,0x000008,0x000008,0x000008,0x000008,
0x000008,0x000008,0x000008,0x000008,0x000008,0x000008,0x00001C,0x000000,

0x000000,0x000000,0x000000,0x000060,0x000060,0x000000,0xC78060,0x284020,
0x102020,0x102020,0x102020,0x102020,0x102020,0x102020,0x102020,0x102020,
0x102020,0x102020,0x102020,0x102020,0x102020,0x102020,0x7C70F8,0x000000,

0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x67E000,0x281000,
0x300800,0x200800,0x200800,0x200800,0x200800,0x200800,0x200800,0x200800,
0x200800,0x200800,0x200800,0x200800,0x200800,0x200860,0x701C60,0x000000
};

// 残り分数表示用　数値フォントデータ(0-9)
const uint16_t MINUTE_NUM[] ={
//0
0x07C0,0x0820,0x1010,0x2008,0x2008,0x2008,0x2008,0x2008,
0x2008,0x2008,0x2008,0x2008,0x2008,0x2008,0x2008,0x2008,
0x2008,0x2008,0x2008,0x2008,0x1010,0x0820,0x07C0,0x0000,

//1
0x0100,0x0300,0x0D00,0x0100,0x0100,0x0100,0x0100,0x0100,
0x0100,0x0100,0x0100,0x0100,0x0100,0x0100,0x0100,0x0100,
0x0100,0x0100,0x0100,0x0100,0x0100,0x0100,0x0FE0,0x0000,

//2
0x07C0,0x0820,0x1010,0x2008,0x2008,0x2008,0x0008,0x0008,
0x0008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0400,
0x0800,0x1000,0x2000,0x2008,0x2008,0x2008,0x3FF8,0x0000,

//3
0x07C0,0x0820,0x1010,0x2008,0x2008,0x2008,0x0008,0x0008,
0x0008,0x0010,0x0020,0x01C0,0x0020,0x0010,0x0008,0x0008,
0x0008,0x2008,0x2008,0x2008,0x1010,0x0820,0x07C0,0x0000,

//4
0x0100,0x0100,0x0300,0x0500,0x0500,0x0500,0x0900,0x0900,
0x0900,0x1100,0x1100,0x1100,0x2100,0x2100,0x3FF8,0x0100,
0x0100,0x0100,0x0100,0x0100,0x0100,0x0100,0x07C0,0x0000,

//5
0x3FF0,0x2000,0x2000,0x2000,0x2000,0x2000,0x2000,0x2000,
0x2000,0x27C0,0x2820,0x3010,0x0008,0x0008,0x0008,0x0008,
0x0008,0x2008,0x2008,0x2008,0x1010,0x0820,0x07C0,0x0000,

//6
0x07C0,0x0820,0x1010,0x2008,0x2000,0x2000,0x2000,0x2000,
0x2000,0x27C0,0x2820,0x3010,0x2008,0x2008,0x2008,0x2008,
0x2008,0x2008,0x2008,0x2008,0x1010,0x0820,0x07C0,0x0000,

//7
0x3FF8,0x2008,0x2008,0x2008,0x2010,0x0010,0x0010,0x0020,
0x0020,0x0020,0x0020,0x0040,0x0040,0x0040,0x0040,0x0080,
0x0080,0x0080,0x0080,0x0100,0x0100,0x0100,0x0100,0x0000,

//8
0x07C0,0x0820,0x1010,0x2008,0x2008,0x2008,0x2008,0x2008,
0x2008,0x1010,0x0820,0x07C0,0x0820,0x1010,0x2008,0x2008,
0x2008,0x2008,0x2008,0x2008,0x1010,0x0820,0x07C0,0x0000,

//9
0x07C0,0x0820,0x1010,0x2008,0x2008,0x2008,0x2008,0x2008,
0x2008,0x2008,0x2018,0x1028,0x0848,0x07C8,0x0008,0x0008,
0x0008,0x0008,0x0008,0x2008,0x1010,0x0820,0x07C0,0x0000
};

// 列車到着時刻用　数値フォントデータ(0-9 :)
const uint16_t MINUTE_NUM_B[] ={
//0
0x0FE0,0x1C70,0x3838,0x701C,0x701C,0x701C,0x701C,0x701C,
0x701C,0x701C,0x701C,0x701C,0x701C,0x701C,0x701C,0x701C,
0x701C,0x701C,0x701C,0x701C,0x3838,0x1C70,0x0FE0,0x0000,
//1
0x0380,0x0780,0x0F80,0x1380,0x0380,0x0380,0x0380,0x0380,
0x0380,0x0380,0x0380,0x0380,0x0380,0x0380,0x0380,0x0380,
0x0380,0x0380,0x0380,0x0380,0x0380,0x0380,0x1FF0,0x0000,
//2
0x07E0,0x1C70,0x3838,0x701C,0x701C,0x701C,0x001C,0x001C,
0x001C,0x0038,0x0070,0x00E0,0x01C0,0x0380,0x0700,0x0E00,
0x1C00,0x3800,0x7000,0x700C,0x700C,0x700C,0x7FFC,0x0000,
//3
0x0FE0,0x1C70,0x3838,0x701C,0x701C,0x701C,0x001C,0x001C,
0x001C,0x0038,0x0070,0x07C0,0x0070,0x0038,0x001C,0x001C,
0x001C,0x701C,0x701C,0x701C,0x3838,0x1C70,0x0FE0,0x0000,
//4
0x0380,0x0380,0x0780,0x0F80,0x0F80,0x0F80,0x1B80,0x1B80,
0x1B80,0x3380,0x3380,0x3380,0x6380,0x6380,0x7FFC,0x0380,
0x0380,0x0380,0x0380,0x0380,0x0380,0x0380,0x0FE0,0x0000,
//5
0x7FF8,0x7000,0x7000,0x7000,0x7000,0x7000,0x7000,0x7000,
0x7000,0x77E0,0x7870,0x7038,0x001C,0x001C,0x001C,0x001C,
0x001C,0x701C,0x701C,0x701C,0x3838,0x1C70,0x0FE0,0x0000,
//6
0x0FE0,0x1C70,0x3838,0x701C,0x7000,0x7000,0x7000,0x7000,
0x7000,0x77E0,0x7870,0x7038,0x701C,0x701C,0x701C,0x701C,
0x701C,0x701C,0x701C,0x701C,0x3838,0x1C70,0x07C0,0x0000,
//7
0x7FFC,0x701C,0x701C,0x701C,0x7038,0x0038,0x0038,0x0070,
0x0070,0x0070,0x0070,0x00E0,0x00E0,0x00E0,0x00E0,0x01C0,
0x01C0,0x01C0,0x01C0,0x0380,0x0380,0x0380,0x0380,0x0000,
//8
0x0FE0,0x1C70,0x3838,0x701C,0x701C,0x701C,0x701C,0x701C,
0x701C,0x3838,0x1C70,0x07C0,0x1C70,0x3838,0x701C,0x701C,
0x701C,0x701C,0x701C,0x701C,0x3838,0x1C70,0x0FE0,0x0000,
//9
0x0FE0,0x1C70,0x3838,0x701C,0x701C,0x701C,0x701C,0x701C,
0x701C,0x701C,0x701C,0x383C,0x1C5C,0x0F9C,0x001C,0x001C,
0x001C,0x001C,0x001C,0x701C,0x3838,0x1C70,0x0FC0,0x0000,
//:
0x0000,0x0000,0x0000,0x0000,0x3800,0x3800,0x3800,0x0000,
0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
0x3800,0x3800,0x3800,0x0000,0x0000,0x0000,0x0000,0x0000
};

// 地図表示用レールフォント
const uint32_t RAILWAY_A[] ={

0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x7FFFFE,0x400002,0x400002,0xC00003,
0x400002,0x400002,0x400002,0x400002,0xC00003,0x400002,0x7FFFFE,0x000000,


0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xFFFFFF,
0xFFF800,0xFFF800,0xFFF800,0xFFF800,0xFFFFFF,0x000000,0x000000,0x000000,

0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xFFFFFF,
0xFFF800,0xFFF800,0xFFF800,0xFFF800,0xFFFFFF,0x000000,0x000000,0x000000
};

// 地図表示用レールフォント当駅ビットマップ色データ
uint16_t BMP_RAILWAY_B[16] = {
	0x0000, // 0 BLACK
	0xffff, // 1 
	0xffff, // 2 
	0xffff, // 3 
	0xffff, // 4 
	0xffff, // 5 
	0x27e4, // 6 GREEN
	0xffff, // 7 
	0xffff, // 8 
	0xff00, // 9 ORANGE
	0xffff, // a 
	0xffff, // b 
	0xf800, // c RED
	0xffff, // d 
	0xffff, // e 
	0xffff  // f 
};

// 地図表示用レールフォント当駅ビットマップ
const uint8_t RAILWAY_B[] ={
//0
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//1
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//2
0x00,0x00,0x00,0x0C,0xCC,0xCC,0xCC,0xCC,0x00,0x00,0x00,0x00,
//3
0x00,0x00,0x00,0x0C,0xCC,0xCC,0xCC,0xCC,0x00,0x00,0x00,0x00,
//4
0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0xC0,0x00,0x00,0x00,0x00,
//5
0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0xC0,0x00,0x00,0x00,0x00,
//6
0x00,0x00,0x00,0x00,0x0C,0xCC,0xCC,0x00,0x00,0x00,0x00,0x00,
//7
0x00,0x00,0x00,0x00,0x0C,0xCC,0xCC,0x00,0x00,0x00,0x00,0x00,
//8
0x00,0x00,0x00,0x00,0x00,0xCC,0xC0,0x00,0x00,0x00,0x00,0x00,
//9
0x00,0x00,0x00,0x00,0x00,0xCC,0xC0,0x00,0x00,0x00,0x00,0x00,
//10
0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,
//11
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//12
0x09,0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x90,
//13
0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x90,
//14
0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x90,
//15
0x69,0x00,0x0C,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xC0,0x00,0x96,
//16
0x09,0x00,0x0C,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xC0,0x00,0x90,
//17
0x09,0x00,0x0C,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xC0,0x00,0x90,
//18
0x09,0x00,0x0C,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xC0,0x00,0x90,
//19
0x09,0x00,0x0C,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xC0,0x00,0x90,
//20
0x69,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x96,
//21
0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x90,
//22
0x09,0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x90,
//23
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

// 電車表示(24dot x 9dot)
const uint32_t TRAIN_MODEL[] ={
0x7FFFC0,
0x7FFFE0,
0x7FFFF0,
0x7FFFF8,
0x7FFFFC,
0x7FFFFE,
0x7FFFFE,
0x7FFFFE,
0x7FFFFE
};

// 「電車がまいります」 フォントデータ
const uint32_t SIGNBOARD24APP_J[] = {

// 電
0x7FFFC0,0x7FFFC0,0x006000,0xFFFFF0,0xC06030,0xDF6FB0,0xC06030,0xC06030,
0x1F6F80,0x006000,0x3FFFC0,0x3FFFC0,0x3060C0,0x3FFFC0,0x3FFFC0,0x3060C0,
0x3FFFC0,0x3FFFD8,0x306018,0x007FF8,0x007FF0,0x000000,0x000000,0x000000,

//車
0x003000,0x003000,0x7FFFF0,0x7FFFF0,0x003000,0x1FFFE0,0x1FFFE0,0x183060,
0x183060,0x1FFFE0,0x1FFFE0,0x183060,0x1FFFE0,0x1FFFE0,0x003000,0xFFFFFC,
0xFFFFFC,0x003000,0x003000,0x003000,0x003000,0x000000,0x000000,0x000000,

//が　304c
0x000040,0x020070,0x038038,0x0301C8,0x0300E0,0x060120,0x060380,0xFFE180,
0xFFF1C0,0x0C30E0,0x0C3060,0x0C3070,0x183030,0x183030,0x383030,0x303000,
0x303000,0x623000,0xE7F000,0xC3E000,0x400000,0x000000,0x000000,0x000000,

//ま　uni307E
0x006000,0x006000,0x006000,0x007FC0,0xFFFFC0,0xFFE000,0x006000,0x006000,
0x006000,0x7FFF80,0x7FF800,0x006000,0x006000,0x026000,0x1FF800,0x307F00,
0x20C7C0,0x20C1C0,0x31C000,0x1F8000,0x040000,0x000000,0x000000,0x000000,

//い
0x000000,0x000000,0x000000,0x180000,0x380000,0x380380,0x3801C0,0x3000E0,
0x300060,0x300070,0x300030,0x300038,0x300038,0x300038,0x384018,0x38E010,
0x18C000,0x1DC000,0x1F8000,0x0F0000,0x000000,0x000000,0x000000,0x000000,

//り　uni308A
0x000180,0x070180,0x060180,0x060180,0x060180,0x060180,0x060180,0x060180,
0x060180,0x062180,0x067180,0x06E180,0x07C180,0x038180,0x000380,0x000300,
0x000700,0x000E00,0x003C00,0x007800,0x002000,0x000000,0x000000,0x000000,

//ま　uni307E
0x006000,0x006000,0x006000,0x007FC0,0xFFFFC0,0xFFE000,0x006000,0x006000,
0x006000,0x7FFF80,0x7FF800,0x006000,0x006000,0x026000,0x1FF800,0x307F00,
0x20C7C0,0x20C1C0,0x31C000,0x1F8000,0x040000,0x000000,0x000000,0x000000,

//す　3059
0x003000,0x003000,0x003000,0x003010,0x7FFFF0,0xFFFFC0,0x003000,0x003000,
0x03F000,0x067000,0x0C3000,0x0C3000,0x0C3000,0x07F000,0x03B000,0x003000,
0x007000,0x00E000,0x01C000,0x038000,0x030000,0x000000,0x000000,0x000000,

};

// 「Train approaching」フォントデータ
const uint32_t SIGNBOARD24APP_E[] = {
//Train approaching
0xFFE000,0x842000,0x040000,0x040000,0x040000,0x040000,0x040000,0x040000,
0x040000,0x0401F0,0x040A09,0x040400,0x040400,0x040400,0x040400,0x040400,
0x040400,0x040401,0x040401,0x040401,0x040401,0x0E0E00,0x000000,0x000000,

0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x001000,
0x000000,0xF83078,0x041284,0x021102,0x021102,0x021102,0x1E1102,0x621102,
0x821102,0x021102,0x021102,0x021102,0x021102,0xFD3902,0x000000,0x000000,

0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x0003E1,0x000410,0x000008,0x000008,0x000008,0x000078,0x000188,
0x000208,0x000408,0x000408,0x000408,0x000408,0x0003F4,0x000000,0x000001,

0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x7C2F80,0x821045,0x811022,0x811022,0x811022,0x811022,0x811022,
0xC11822,0xA21442,0x9C1382,0x801002,0x801002,0x801007,0x801000,0xC03800,

0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0xF8783E,0x048441,0x010200,0x010200,0x010200,0x010207,0x010218,
0x010220,0x010240,0x010240,0x010240,0x008440,0x00783F,0x000000,0x000000,

0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000C00,0x000400,
0x000400,0x078401,0x0845C0,0x902620,0x900410,0x900410,0x900410,0x900410,
0x900410,0x900410,0x900410,0x902410,0x884410,0x478E39,0x000000,0x000000,

0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x800000,
0x000002,0x83C0F4,0x942108,0x881204,0x881204,0x881108,0x8810F0,0x881100,
0x881200,0x8811F8,0x881204,0x881202,0x881202,0xC81202,0x000104,0x0000F8,

0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000
};
