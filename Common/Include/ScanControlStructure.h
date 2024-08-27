// ScanControlStructure.h :
//

#if !defined(_ScanControlStructure_H_)
#define _ScanControlStructure_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const DWORD			ThreadSwitchOptionTime			= 5;	// ms
const DWORD			ThreadSwitchOptionTime_USB		= 8;	// ms
/*
// ADDRESS[9，2]	CONTENT      ( bit31--------------------------------------------------------bit0)
// =================================================================================================
// 10000001B	ENABLE   （bit0）
// 10000010B	FLAG[30..28],[27],MODEL[26..24],[23],AVE[22..20],RESO[19..18],FREDIV[17..0]（bit0）
// 10000011B	DOT0X[23..12],DOT0Y[11..0]
// 10000100B	POINTX[23..12],POINTY[11..0]
// 10000101B	STEPX[24..0]
// 10000110B	STEPY[24..0]
// 10000111B	FIFO_OFFSET[31..0]
// 10001000B	COMA[7..4][1..0]
// 10001001B	COMB[7..4][1..0]

// FIFO_OFFSET_FULL
// FIFO_OFFSET_EMPTY
// ld[31..24]          ld[23..16]          ld[15..8]               ld[7..0]
// full_offset高8位    full_offset低8位    empty_offset高8位        empty_offset低8位

typedef struct tagInitData_ADDR_10000001B{
	DWORD	ENABLE		: 1;	// 00000000 00000000 00000000 0000000?
	DWORD	PHOTO_ENABLE: 1;	// 00000000 00000000 00000000 000000?0
	DWORD	reserve1	: 30;	// ???????? ???????? ???????? ??????00
}InitData_ADDR_10000001B;

typedef struct tagInitData_ADDR_10000010B{
	DWORD	FREDIV		: 18;	// 00000000 000000?? ???????? ????????
	DWORD	RESO		: 2;	// 00000000 0000??00 00000000 00000000
	DWORD	AVE			: 3;	// 00000000 0???0000 00000000 00000000
	DWORD	reserve1	: 1;	// 00000000 ?0000000 00000000 00000000
	DWORD	MODEL		: 3;	// 00000??? 00000000 00000000 00000000
	DWORD	reserve2	: 1;	// 0000?000 00000000 00000000 00000000
	DWORD	FLAG		: 3;	// 0???0000 00000000 00000000 00000000
	DWORD	reserve3	: 1;	// ?0000000 00000000 00000000 00000000
}InitData_ADDR_10000010B;

typedef struct tagInitData_ADDR_10000011B{
	DWORD	DOT0Y		: 12;	// 00000000 00000000 0000???? ????????
	DWORD	DOT0X		: 12;	// 00000000 ???????? ????0000 00000000
	DWORD	reserve1	: 8;	// ???????? 00000000 00000000 00000000
}InitData_ADDR_10000011B;

typedef struct tagInitData_ADDR_10000100B{
	DWORD	POINTY		: 12;	// 00000000 00000000 0000???? ????????
	DWORD	POINTX		: 12;	// 00000000 ???????? ????0000 00000000
	DWORD	reserve1	: 8;	// ???????? 00000000 00000000 00000000
}InitData_ADDR_10000100B;

typedef struct tagInitData_ADDR_10000101B{
	DWORD	STEPX		: 24;	// 00000000 ???????? ???????? ????????
	DWORD	STEPX_F		: 1;	// 0000000? 00000000 00000000 00000000
	DWORD	reserve1	: 7;	// ???????0 00000000 00000000 00000000
}InitData_ADDR_10000101B;

typedef struct tagInitData_ADDR_10000110B{
	DWORD	STEPY		: 24;	// 00000000 ???????? ???????? ????????
	DWORD	STEPY_F		: 1;	// 0000000? 00000000 00000000 00000000
	DWORD	reserve1	: 7;	// ???????0 00000000 00000000 00000000
}InitData_ADDR_10000110B;

typedef struct tagInitData_ADDR_10000111B{
	DWORD	FIFO_EMPTY	: 16;	// 00000000 00000000 ???????? ????????
	DWORD	FIFO_FULL	: 16;	// ???????? ???????? 00000000 00000000
}InitData_ADDR_10000111B;

typedef struct tagInitData{
	InitData_ADDR_10000001B		ADDR_10000001B;		// ADDRESS[9，2] : 10000001B
	InitData_ADDR_10000010B		ADDR_10000010B;		// ADDRESS[9，2] : 10000010B
	InitData_ADDR_10000011B		ADDR_10000011B;		// ADDRESS[9，2] : 10000011B
	InitData_ADDR_10000100B		ADDR_10000100B;		// ADDRESS[9，2] : 10000100B
	InitData_ADDR_10000101B		ADDR_10000101B;		// ADDRESS[9，2] : 10000101B
	InitData_ADDR_10000110B		ADDR_10000110B;		// ADDRESS[9，2] : 10000110B
	InitData_ADDR_10000111B		ADDR_10000111B;		// ADDRESS[9，2] : 10000111B
}InitData;

typedef union tagInitParam
{
	DWORD		dwData[7];	// 参数数据表
	InitData	Data;		// 参数数据结构
}InitParam;

// ADDRESS[9，2]	CONTENT      ( bit31--------------------------------------------------------bit0)
// =================================================================================================
// 10000110B	LENS[7..4][1..0]
// 10000111B	SCAN[7..4][1..0]
// 10001000B	OVERFLOW[28], DACX[27..16], FIFO_NUM[15..0]
// 10001001B	DACY[11..0]

typedef struct tagReadData_ADDR_10001000B{
	DWORD	FIFO_NUM	: 16;	// 00000000 00000000 ???????? ????????
	DWORD	DACX		: 12;	// 0000???? ???????? 00000000 00000000
	DWORD	OVERFLOW	: 1;	// 000?0000 00000000 00000000 00000000
	DWORD	reserve1	: 3;	// ???00000 00000000 00000000 00000000
}ReadData_ADDR_10001000B;

typedef struct tagReadData_ADDR_10001001B{
	DWORD	DACY		: 12;	// 00000000 00000000 0000???? ????????
	DWORD	reserve1	: 4;	// 00000000 00000000 ????0000 00000000
	DWORD	PHOTO_FIFO	: 1;	// 00000000 0000000? 00000000 00000000
	DWORD	reserve2	: 15;	// ???????? ???????0 00000000 00000000
}ReadData_ADDR_10001001B;


/////////////////////////////////////////////////////////////////////////////
// 大FIFO /AF (almost full) 触发条件参数
// FLAG				2,	   1,	 3,	   6,	 4,    5,	7,   0
// FIFO SIZE		16384, 8192, 4096, 2048, 1024, 512, 256, 128
//---------------------------------------------------------------------------
const DWORD	FLAG_FIFO_SIZE[] = 
	{ 128, 8192, 16384, 4096, 1024, 512, 2048, 256, };
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// AVE[22..20]			平均滤波分档数（0~7）
// RESO [19, 18]        分辨率分档数（0~3）
// FREDIV [17, 0]		采样脉冲分频数（1~2p18-1）
// 由点停留时间tp确定AVE、FREDIV
// 再根据采集的速度及显示频率，确定大FIFO /AF (almost full) 触发条件参数FLAG
/////////////////////////////////////////////////////////////////////////////
// (点停留时间tp	单位:ns)
// FREDIV = (tp/50ns - 13) / (POWER(2,AVE+1) + 3) - 1			AVE = 1-7 ;
// FREDIV =	1													AVE = 0	  ;
//---------------------------------------------------------------------------
// (点停留时间tp	单位:ns)	AVE = 7
//						==>		FREDIV = (tp*20 - 13) / 259 - 1
/////////////////////////////////////////////////////////////////////////////
// 附：常用值(假定每秒钟采集50次)
// tp		1us	 2us 5us 10us 16us 20us 40us 1ms  10ms  100ms
// AVE		0,	 1,	 2,	 3,	  5,   6,	7,	 7,   7,    7,   
// FREDIV	1,	 3,	 7,	 9,	  4,   2,	2,	 4CH, 303H, 1E28H,
// FLAG		2,	 1,	 3,	 6,	  4,   4,	5,	 0,   0,    0,
//---------------------------------------------------------------------------
// tp		40us 80us 120us 160us	50us 90us	照相常用值
// AVE		 7,   7,   7,    7,      7,   7, 
// FREDIV	 2,   5,   8,   11,      3,   6,
// FLAG		 5,   5,   5,    5,      5,   5,
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// 点停留时间常用值：
/////////////////////////////////////////////////////////////////////////////
typedef enum tagScanSpeed
{
	SS_1us				= 0,	// 点停留时间   1us
	SS_2us				= 1,	// 点停留时间   2us
	SS_5us				= 2,	// 点停留时间   5us
	SS_10us				= 3,	// 点停留时间  10us
	SS_16us				= 4,	// 点停留时间  16us
	SS_20us				= 5,	// 点停留时间  20us
	SS_40us				= 6,	// 点停留时间  40us
	SS_80us				= 7,	// 点停留时间  80us
	SS_120us			= 8,	// 点停留时间 120us
	SS_160us			= 9,	// 点停留时间 160us
}ScanSpeed;

/////////////////////////////////////////////////////////////////////////////
const DWORD	SCANSPEED_TP[]		= {	1,  2,  5, 10, 16, 20, 40, 80, 120, 160, };
const DWORD	INITDATA_AVE[]		= {	0,	1,	2,	3,	5,	6,  7,  7,   7,   7, };
const DWORD	INITDATA_FREDIV[]	= {	1,	3,	7,	9,	4,	2,	2,	5,   8,  11, };
const DWORD	INITDATA_FLAG[]		= {	3,	6,	4,	4,	5,	0,	0,	0,	 0,   0, };
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

typedef union tagReadParam
{
	DWORD	dwParam;		// 参数数据

	// 参数数据结构
	ReadData_ADDR_10001000B		ADDR_10001000B;		// ADDRESS[9，2] : 10001000B
	ReadData_ADDR_10001001B		ADDR_10001001B;		// ADDRESS[9，2] : 10001001B
}ReadParam;

const DWORD ADDR_10000000B = 0x00;
const DWORD ADDR_10000001B = 0x04;
const DWORD ADDR_10001000B = 0x20;
const DWORD ADDR_10001001B = 0x24;
/*
//////////////////////////////////////////////////////////////////////
// 采集卡支持的六种工作方式：
// 面扫(全屏/选区)、线扫、双放、图像分割、模拟照相、数字照相
typedef enum tagScanFunction
{
	SF_Stop					= 0,	// 停止
	SF_RasterScan			= 1,	// 面扫	001B
	SF_LineScan				= 2,	// 线扫	010B
	SF_DualMagnification	= 3,	// 双放	011B
	SF_SplitScan			= 4,	// 分割	100B
	SF_AnalogPhoto			= 5,	// 模拟照相	101B
	SF_DigitalPhoto			= 6,	// 数字照相	110B
}ScanFunction;

// 采集卡支持的四种分辨率：
//		 512 * 384
//		1024 * 768
//		2048 * 1536
//		4096 * 3072
typedef enum tagScanResolution
{
	SR_512x384				= 0,	//  512 *  384 分辨率
	SR_1024x768				= 1,	// 1024 *  768 分辨率
	SR_2048x1536			= 2,	// 2048 * 1536 分辨率
	SR_4096x3072			= 3,	// 4096 * 3072 分辨率
}ScanResolution;

const LONG ScanResolution_X_Pixels[] = { 512, 1024, 2048, 4096, };

// 面扫：( 点停留时间，分辨率，) 起始点坐标，终止点坐标
typedef struct tagExtrParamRasterScan
{
	LONG	dwStartX;	// 起始点 X 坐标
	LONG	dwStartY;	// 起始点 Y 坐标
	LONG	dwStopX;	// 终止点 X 坐标
	LONG	dwStopY;	// 终止点 Y 坐标
}ExtrParamRasterScan;

// 线扫：( 点停留时间，分辨率，) 起始点坐标，终止点坐标，线扫点数
typedef struct tagExtrParamLineScan
{
	LONG	dwStartX;		// 起始点 X 坐标
	LONG	dwStartY;		// 起始点 Y 坐标
	LONG	dwStopX;		// 终止点 X 坐标
	LONG	dwStopY;		// 终止点 Y 坐标
	LONG	dwPixelNumber;	// 线扫点数
}ExtrParamLineScan;

// 双放：(点停留时间， 分辨率 = 1024 * 768 )
//		小区域起始点坐标，小区域终止点坐标
typedef struct tagExtrParamDualMagnification
{
	LONG	dwStartX;	// 小区域起始点 X 坐标
	LONG	dwStartY;	// 小区域起始点 Y 坐标
	LONG	dwStopX;	// 小区域终止点 X 坐标 (保留)
	LONG	dwStopY;	// 小区域终止点 Y 坐标
}ExtrParamDualMagnification;

typedef union tagScanParam
{
	ExtrParamDualMagnification	DualMag;		// 双放参数
	ExtrParamLineScan			LineScan;		// 线扫参数
	ExtrParamRasterScan			RasterScan;		// 面扫参数
}ScanParam;
*/
// 图像分割：点停留时间( ,分辨率 = 1024 * 768 )
// 模拟照相：点停留时间( ,分辨率 = 1024 * 768 )
// 数字照相：点停留时间( ,分辨率 = 1024 * 768 )
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////
// USB
/////////////////////////////////////////////////////////////////////
/*
// 扫描速度
typedef enum tagUSB_ScanSpeed
{
	USB_SS_125ns			= 0,
	USB_SS_250ns			= 1,
	USB_SS_500ns			= 2,
	USB_SS_1us				= 3,
	USB_SS_2us				= 4,
	USB_SS_4us				= 5,
	USB_SS_5us				= 6,
	USB_SS_8us				= 7,
	USB_SS_10us				= 8,
	USB_SS_12us				= 9,
	USB_SS_16us				= 10,
	USB_SS_20us				= 11,
	USB_SS_25us				= 12,
	USB_SS_30us				= 13,
	USB_SS_32us				= 14,
	USB_SS_35us				= 15,
	USB_SS_40us				= 16,
	USB_SS_50us				= 17,
	USB_SS_60us				= 18,
	USB_SS_63750ns			= 19,
	USB_SS_64us				= 20,
	USB_SS_70us				= 21,
	USB_SS_80us				= 22,
	USB_SS_90us				= 23,
	USB_SS_100us			= 24,
	USB_SS_110us			= 25,
	USB_SS_120us			= 26,
	USB_SS_130us			= 27,
	USB_SS_140us			= 28,
	USB_SS_150us			= 29,
	USB_SS_160us			= 30,
}USB_ScanSpeed;
*/
typedef struct tagUSB_InitRampParam
{
	WORD	wOffset;
	WORD	wValley;
	WORD	wRise;
	WORD	wDRise;
	WORD	wPeak;
	WORD	wFall;
	WORD	wDFall;
//	WORD	wDirection;
}USB_InitScanRampParam;

typedef struct tagUSB_InitData
{
	USB_InitScanRampParam	ScanRampParamX;
	USB_InitScanRampParam	ScanRampParamY;
	unsigned short			PixRate;
}USB_InitData;

typedef union tagUSB_InitParam
{
	WORD			wData[14];	// 参数数据表
	USB_InitData	Data;		// 参数数据结构
}USB_InitParam;

// 面扫：( 点停留时间，分辨率，) 起始点坐标，终止点坐标
typedef struct tagUSB_ExtrParamRasterScan
{
	LONG	dwStartX;	// 起始点 X 坐标
	LONG	dwStartY;	// 起始点 Y 坐标
	LONG	dwStopX;	// 终止点 X 坐标
	LONG	dwStopY;	// 终止点 Y 坐标
}USB_ExtrParamRasterScan;

// 线扫：( 点停留时间，分辨率，) 起始点坐标，终止点坐标，线扫点数
typedef struct tagUSB_ExtrParamLineScan
{
	LONG	dwStartX;		// 起始点 X 坐标
	LONG	dwStartY;		// 起始点 Y 坐标
	LONG	dwStopX;		// 终止点 X 坐标
	LONG	dwStopY;		// 终止点 Y 坐标
	LONG	dwPixelNumber;	// 线扫点数
}USB_ExtrParamLineScan;

// 双放：(点停留时间， 分辨率 = 1024 * 768 )
//		小区域起始点坐标，小区域终止点坐标
typedef struct tagUSB_ExtrParamDualMagnification
{
	LONG	dwStartX;	// 小区域起始点 X 坐标
	LONG	dwStartY;	// 小区域起始点 Y 坐标
	LONG	dwStopX;	// 小区域终止点 X 坐标 (保留)
	LONG	dwStopY;	// 小区域终止点 Y 坐标
}USB_ExtrParamDualMagnification;

typedef union tagUSB_ScanParam
{
	USB_ExtrParamDualMagnification	DualMag;		// 双放参数
	USB_ExtrParamLineScan			LineScan;		// 线扫参数
	USB_ExtrParamRasterScan			RasterScan;		// 面扫参数
}USB_ScanParam;

// 采集卡支持的六种工作方式：
// 面扫、选区、线扫、点扫、模拟照相

typedef enum tagUSB_ScanFunction
{
	USB_SF_Stop					= 0,	// 停止
	USB_SF_RasterScan			= 1,	// 面扫
	USB_SF_AreaScan				= 2,	// 选区
	USB_SF_LineScan_HORZ		= 3,	// 水平线扫
	USB_SF_LineScan_VERT		= 4,	// 垂直线扫
	USB_SF_SpotScan				= 5,	// 点扫
	USB_SF_AnalogPhoto			= 6,	// 模拟照相
}USB_ScanFunction;

typedef enum tagUSB_ScanType
{
	USB_ST_Stop					= 0,	// 冻结图象扫描
	USB_ST_RasterScan			= 1,	// 常规扫描
	USB_ST_AreaScan				= 2,	// 选区扫描
	USB_ST_LineScan_HORZ		= 3,	// 水平线扫描
	USB_ST_LineScan_VERT		= 4,	// 垂直线扫描
	USB_ST_SpotScan				= 5,	// 点扫描
	USB_ST_AnalogPhoto			= 6,	// 模拟照相
	USB_ST_AutoBCArea			= 7,	// 固定选区扫描	AutoBC
	USB_ST_AutoFocusArea		= 8,	// 固定选区扫描	AutoFocus
	USB_ST_AutoAstigmatArea		= 9,	// 固定选区扫描 AutoAstigmat
	USB_ST_Max					=10,	// Max
}USB_ScanType;

/*
/////////////////////////////////////////////////////////////////////////////
// Pixel rate in units of 1/8 MHz, namely 125ns.
// Pixel rate = N means N * 125nS for pixel time
// The range of N is from 1 to 2K, namely 1,2,3,...0x7FF
/////////////////////////////////////////////////////////////////////////////
// 附：常用值，像素率即点逗留时间
// tp		0.125us 0.250us 0.5us 1us  2us  5us 10us 16us 20us 40us 80us 100us 120us 160us 200us 250us
// N		1,      2,      4,    8,   16,  40, 80,  128, 160, 320, 640, 800,  960,  1280, 1600, 2000
//---------------------------------------------------------------------------
// tp		40us 80us 120us 160us	50us 90us	照相常用值
// N		320, 640, 960,  1280,   400, 720
/////////////////////////////////////////////////////////////////////////////

//                                  500n	1us	2us	3	4us		8us		11		16us	19		20us	22		23		27		32us	36	37		40us   48us	 55		64us	72		80us	90		100us	110		120us	128us	140us  160us	180		200us	220		240us	255us	256us	280us  320us  360us	400us	440us	480us	512us   560us	600us	640us
const WORD	USB_PIXEL_RATES[]	= {	1,		2,  4,	6,	8,		16,		22,		32,		38,		40,		44,		46,		54,		64,		72,	74,		80,    96,	 110,	128,	144,	160,	180,	200,	220,	240,	256,	280,   320,		360,	400,	440,	480,	510,	512,	560,   640,   720,	800,	880,	960,    1240,	1120,	1200,	1280, };
const DWORD	USB_SCANSPEED_TP[]	= {	125,	250,500,750,1000,	2000,	2750,	4000,	4750,	5000,	5500,	5750,	6750,	8000,	9000,9250,	10000, 12000,13750,	16000,	18000,	20000,	22500,	25000,	27500,	30000,	32000,	35000, 40000,	45000,	50000,	55000,	60000,  63750,	64000,	70000, 80000, 90000,100000, 110000, 120000, 128000, 140000, 150000, 160000, };	// unit: ns

//										0.125us 0.25us 0.5us 1us   2us   5us   10us   16us   20us	40us	80us   120us   160us
//	const WORD	USB_PIXEL_RATES[]	= {	1,      2,     4,    8,    16,   40,   80,    128,   160,	320,	640,   960,    1280, };
//	const DWORD	USB_SCANSPEED_TP[]	= {	125,    250,   500,  1000, 2000, 5000, 10000, 16000, 20000, 40000, 80000, 120000, 160000, };	// unit: ns

// MCI常用的分辨率：
typedef enum tagUSB_ScanResolution
{
	SR_16x16				= 0,
	SR_32x32				= 1,
	SR_64x64				= 2,
	SR_128x128				= 3,
	SR_256x256				= 4,
	SR_512x512				= 5,	//  512 *  512 分辨率
	SR_1024x1024			= 6,	// 1024 * 1024 分辨率
	SR_2048x2048			= 7,	// 2048 * 2048 分辨率
	SR_4096x4096			= 8,	// 4096 * 4096 分辨率
	SR_8192x8192			= 9,	// 8192 * 8192 分辨率
	SR_1x1					= 10,
}USB_ScanResolution;
*/
#endif // _ScanControlStructure_H_
