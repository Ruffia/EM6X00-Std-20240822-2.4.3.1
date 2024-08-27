
#ifndef __USBFunctions_H
#define __USBFunctions_H

#include "FEG\VACobtain.h"
#include "ScanControlStructure.h"
#include <afxtempl.h>

// 常数π
#define PI 3.1415926535
#define EPSILON (0.0000001)
#define EPSILON4 (0.0001)
#define COM_Counts	30

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//====== PC -> MCI BOARD MessageID ==============================

#define	FLAGBIT( flag, flag_bit )	( ( (flag) & (flag_bit) ) == (flag_bit) )

#define		USB_Flag_Bit0		(BYTE)0x01
#define		USB_Flag_Bit1		(BYTE)0x02
#define		USB_Flag_Bit2		(BYTE)0x04
#define		USB_Flag_Bit3		(BYTE)0x08

#define		USB_Flag_Bit4		(BYTE)0x10
#define		USB_Flag_Bit5		(BYTE)0x20
#define		USB_Flag_Bit6		(BYTE)0x40
#define		USB_Flag_Bit7		(BYTE)0x80

// User defined message to notify application program of an interrupt
#define	WM_USER_NEWIMAGE	(WM_USER	+1)
#define WM_USER_PROGRESS	(WM_USER	+2)
#define WM_PARAM_CHANGED	(WM_USER	+3)

#define ImageData_ColumnCount 1024
#define ImageData_RowCount    768

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// VACUUM
#define WM_USER_VAC_MSG		(WM_USER	+12)	// Status byte from the controller is passed to user via the wParam argument
#define WM_USER_VAC_TCAmp	(WM_USER	+13)

#define	RelayEnAddr_Vf		(BYTE)0x41	// 前级阀FOREL
#define	RelayEnAddr_Vc		(BYTE)0x42	// 镜筒阀COLUMN
#define	RelayEnAddr_Va		(BYTE)0x44	// 放气阀AIR
#define	RelayEnAddr_TMP		(BYTE)0x45	// 主阀或分子泵MAIN/TMP，需要一对0x45 & 0x46
#define RelayEnAddr_V1		(BYTE)0x47	// 16.07 加气动V1阀控制方式

#define	RelayEn_Open		(BYTE)0x1
#define	RelayEn_Close		(BYTE)0x0

// 该状态结构应作为PostMessage的参数
// 在该线程和主线程直接传递
// 主线程只负责根据这些参数同步界面各状态显示
typedef struct tagSMvac_Param
{
	BOOL	bPower;		// 真空电源按钮：按下为1；抬起为0
	BOOL	bChamb;		// 样品室放气按钮：按下为1；抬起为0
	BOOL	bGun;		// 电子枪放气按钮：按下为1；抬起为0

	DWORD	dwTCs;		// 所有TC的状态
	DWORD	dwExtSwi;	// 外部开关的状态

	BOOL	bTCf;		// 前级真空
	BOOL	bTCc;		// 镜筒真空

	BOOL	bChambEn;	// 样品室放气阀使能
	BOOL	bGunEn;		// 电子枪放气阀使能

	BOOL	bVf;		// 前级阀开关
	BOOL	bVc;		// 镜筒阀开关
	BOOL	bVa;		// 放气阀开关
	BOOL	bV1;		// V1阀开关

	BOOL	bTMP;		// 分子泵开关
	BOOL	bMP;		// 机械泵开关
	BOOL	bHVInt;		// 高压连锁信号
}SMvac_Param;

typedef enum tagSMvac_State
{
	vacAllOFF	= 0,	// 全关
	vacMP		= 1,	// 机械泵开
	vacF		= 2,	// 前级阀开
	vacFC		= 3,	// 前级阀和镜筒阀开
	vacFCT		= 4,	// 准备状态，前级阀、镜筒阀和分子泵都开了，若TC都好了就可以工作了
	vacFCT1		= 5,	// 工作状态：准备状态+TC都好+V1开
	vacA		= 6,	// 样品室放气后只有放气阀开
	vacCA		= 7,	// 电子枪放气后只有镜筒阀和放气阀开

	vacNotN_FT	= 11,	// 非正常状态：前级阀、镜筒阀开后，强行开TMP，但只有前级真空好，则关镜筒阀、开TMP
	vacNotN		= 12,	// 非正常状态not normal
}SMvac_State;

#define VACIndex_Power	0
#define	VACIndex_Chamb	1
#define VACIndex_Gun	2
#define VACIndex_MP		3
#define VACIndex_Vf		4
#define VACIndex_Vc		5
#define VACIndex_TMP	6
#define VACIndex_V1		7
#define VACIndex_Va		8
#define VACIndex_TCf	9
#define VACIndex_TCc	10

typedef struct tagSMvac_TCAmp
{
	double	dTCfAmp;
	double	dTCcAmp;
	int		nState;
	double	dTCfMax;
	double	dTCcMax;
	double	dTCfMin;
	double	dTCcMin;
}SMvac_TCAmp;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// HVPower - CPS3603
#define WM_USER_CPS_Status		(WM_USER	+21)	// Status byte from the controller is passed to user via the wParam argument
#define WM_USER_CPS_MSG			(WM_USER	+22)
#define WM_USER_CPS_Param		(WM_USER	+23)
#define WM_USER_CPS_AutoParam	(WM_USER	+24)

typedef struct tagCPS3603_Param
{
	double	dVoutA;
	double	dIoutA;
	double	dVoutE;
	double	dIoutE;
	double	dVoutF;
	double	dIoutF;
	double	dRf;
	double	dPf;
	double	dTf;
	double	dTa;
	double	dThv;
	double	dCoronalevel;
}CPS3603_Param;

typedef struct tagCPS3603_Status
{
	int		nPort;
	int		nConnectA;
	int		nConnectE;
	int		nConnectF;
	int		nVerA;
	int		nVerE;
	int		nVerF;
	int		nUserMode;
	int		nPassiveBias;
	int		nExtraParams;
	int		nSeparatedFeedback;
	int		nVoltageControlF;
	int		nResistorControl;
}CPS3603_Status;

// Auto State
typedef enum tagCPS3603_AutoState
{
	AS_Rise_Acc1	= 0,
	AS_Rise_Acc2	= 1,
	AS_Rise_Bias	= 2,
	AS_Rise_Fila	= 3,
	AS_NULL			= 4,
}CPS3603_AutoState;
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// HVPower - Spellman-EBM30N12/771
#define WM_USER_SPELLMAN_Status		(WM_USER	+31)	// Status byte from the controller is passed to user via the wParam argument
#define WM_USER_SPELLMAN_MSG		(WM_USER	+32)
#define WM_USER_SPELLMAN_Param		(WM_USER	+33)
#define WM_USER_SPELLMAN_Param2		(WM_USER	+34)
#define WM_USER_SPELLMAN_AutoParam	(WM_USER	+35)
#define	WM_USER_SPELLMAN_Param3		(WM_USER	+36)
#define WM_USER_ControlPanel_MSG    (WM_USER    + 3201)

typedef struct tagSpellman_Param
{
	double	dVC;	// Collector
	double	dVE;	// EHT
	double	dIB;	// Beam
	double	dIH;	// Heater
	double	dVP;	// PMT
	double	dII;	// Beam current monitor
}Spellman_Param;

typedef struct tagSpellman_Status
{
	BOOL	bReady;

	BOOL	bCollector;
	BOOL	bScint;
	BOOL	bEHT;
	BOOL	bHeater;
	BOOL	bPMT;

	BYTE	bstatusSwitch;
	DWORD	dwstatusRegister;

	CString	strID;
	CString	strVer;
}Spellman_Status;

// Auto State
typedef enum tagSpellman_AutoState
{
	AS2_Rise_EHT1	= 0,
	AS2_Rise_EHT2	= 1,
	AS2_Rise_Heater	= 2,
	AS2_Rise_Beam	= 3,
	AS2_Rise_WaitEMI= 4,
	AS2_NULL		= 5,
}Spellman_AutoState;
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// System Options

#define SysOpt_clrTransparent		0
#define SysOpt_clrBack				1
#define SysOpt_clrEditText			2
#define SysOpt_clrLabelText			3
#define SysOpt_clrLabelLogoText		4
#define SysOpt_clrMobileUScaleText	5
#define SysOpt_clrMobileUScaleLine	6
#define SysOpt_clrOscillographGrid	7
#define SysOpt_clrOscillographGraph	8
#define SysOpt_clrFrameRate			9
#define SysOpt_clrAngleLine			10
#define SysOpt_clrAngleArc			11
#define SysOpt_clrAngleText			12

#define SysOpt_nameEditText			0
#define SysOpt_nameLabelText		1
#define SysOpt_nameMobileUScaleText	2

#define SysOpt_sizeEditText			0
#define SysOpt_sizeLabelText		1
#define SysOpt_sizeMobileUScaleText	2

#define SysOpt_showLabelTrans		0
#define SysOpt_showScrMag			1
#define SysOpt_showDate				2
#define SysOpt_showNote				3
#define SysOpt_showCCD				4
#define SysOpt_showFOV				5
#define SysOpt_showPixeSize			6

typedef struct tagSYS_Options
{
	COLORREF	color[13];
	CString		fName[3];
	int			fSize[3];
	int			nLineWidth;
	int			nShowParams[7];
	CString		strDateFormat;

	tagSYS_Options()
	{
		for (int i = 0; i < 13; i++)
		{
			color[i] = RGB(0,0,0);
		}
		for (int i = 0; i < 3;i++)
		{
			fName[i] = "";
		}
		for (int i = 0; i < 3;i++)
		{
			fSize[i] = 0;
		}
		for (int i = 0; i < 7;i++)
		{
			nShowParams[i] = 0;
		}

		nLineWidth = 1;
		strDateFormat = "";
	}
}SYS_Options;


// Footer Settings
#define	Footer_TIME		0
#define Footer_HV		1
#define Footer_WD		2
#define Footer_MAG		3
#define Footer_DET		4
#define Footer_LOGO		5
#define Footer_UBAR		6
#define Footer_NOTE		7
#define Footer_VAC		8
#define Footer_EMI		9
#define Footer_SPEED	10

typedef struct tagFooter_Info{
	BOOL	bVisible;	// 显隐
	CPoint	ptPos;		// 位置：x为列位置；y为第几行
	CString	strDisp;	// 显示字符串内容
	tagFooter_Info()
	{
		bVisible = TRUE;
		ptPos = CPoint(0,0);
		strDisp = "";
	}

}Footer_Info;

typedef struct tagFooter_Options{
	Footer_Info	opt[11];
}Footer_Options;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// const define
#define	ValleyX		4
#define	ValleyY		4
#define	PeakX		0
#define	PeakY		0
#define	FallX		2
#define	FallY		0
#define	DFallX		15000
#define	DFallY		31000

#define NUM_SCANS_Test			1
#define NUM_READINGS_Test		2048
#define	LOOP_COUNTS				1

#define ReadBack_DataSize		256
#define SharedImageBuffCounts	2048

#define MobileUScaleCounts		10
#define AngleMeasureCounts		100

struct AnglePosition
{
	double	dAngle;
	CPoint	ptS;		// 起始点 Start
	CPoint	ptC;		// 中心点 Center
	CPoint	ptE;		// 终点 End;
	CRect	rcBound;	// 圆弧所在圆的外接矩形
	CPoint	ptArcStart;	// 圆弧起始位置
	CPoint	ptArcEnd;	// 圆弧终止位置
	CPoint	ptLabel;	// 标注点位置
};

struct AngleOption
{
	COLORREF	clrLine;// 线色
	COLORREF	clrArc;	// 弧色
	COLORREF	clrText;// 标注颜色
	short		nSize;	// 标注字体大小
};

struct AngleMeasure
{
	BOOL	bStart;		// 起始点已被确认
	BOOL	bCenter;	// 中心点已被确认
	BOOL	bEnd;		// 终点已被确认

	CPoint	ptOriginal;

	int		nIndex;
	int		nAngleStatus;
	int		bAngleHitTest;

	AnglePosition	AP;
	AnglePosition	APbak;
	AngleOption		AO;
};

typedef CTypedPtrList< CPtrList, AngleMeasure* >		ListAngle;

//=============================================================================
// Serial
const	BYTE	byte_HeadSnd	= 0xCA;
const	BYTE	byte_HeadRec	= 0x80;
const	BYTE	byte_HeadINT	= 0x80;
const	BYTE	byte_LenSnd1	= 0x04;
const	BYTE	byte_LenSnd2	= 0x06;
const	BYTE	byte_TailSnd	= 0x0A;
const	BYTE	byte_TailRec	= 0x0E;
const	BYTE	byte_TailINT	= 0x0E;
const	BYTE	byte_cCOpc		= 0xF4;
const	BYTE	byte_cCOpr		= 0xF5;
const	BYTE	byte_cCSub		= 0xF1;

#define	fHead	0
#define fLen	1
#define fCC		2
#define fOpc	3
#define fOpr1	4
#define fOpr2	5
#define fTail	6
const	int frameLen	= 7;
const	int frameLen2	= 5;
//=============================================================================
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// 公共参数
// 0. 版本信息
typedef struct tagPublic_Version
{
	DWORD	dwSoft;
	DWORD	dwHard;
}PublicParam_Version;

// 1. 扫描参数
typedef struct tagPublic_Scan
{
	USB_ScanType	nScan_Type;		// 扫描类型：0冻结；1常规面扫；2活动区域扫；
							//			3水平线扫；4垂直线扫；5点扫；6同步扫描；
							//			7固定区域AutoBC；8固定区域AutoFocus；9固定区域AutoStig
	UINT	nScan_Speed;	// 扫描速度，单位us。0：表示500ns
	UINT	nScan_ResoX;	// 扫描分辨率X
	UINT	nScan_ResoY;	// 扫描分辨率Y
	UINT	nScan_left;		// 扫描区域横向起始点
	UINT	nScan_right;	// 扫描区域横向终点
	UINT	nScan_top;		// 扫描区域纵向起始点
	UINT	nScan_bottom;	// 扫描区域纵向终点
	UINT	nSync_Speed;	// 同步扫描速度，单位us

	UINT	nOffsetX;		// 扫描横向偏移量
	UINT	nOffsetY;		// 扫描纵向偏移量
	UINT	nCurrentY1;		// 最新一批图像数据的起始Y值
	UINT	nCurrentY2;		// 最新一批图像数据的终止Y值

	UINT	nSplit;			// 0：单探测器；1：多探测器
	UINT	nDet2;			// 0：0,2,4通道（默认）；1：1,3,5通道
	UINT	nStoreMode;		// 图像存储方式
}PublicParam_Scan;

// 2. 常用参数
typedef struct tagPublic_Comm
{
	UINT	nFilter;
	UINT	nGain0;
	UINT	nGain1;
	double	dBrt0;
	double	dBrt1;
	double	dCst;
	double	dCL;
	double	dOL;
	double	dALX;
	double	dALY;
	double	dStigX;
	double	dStigY;
	double	dMag;
	double	dHV;
	UINT	nRotate;
}PublicParam_Comm;

// 3. 自动台参数
typedef struct tagStageConfigXY{
	BOOL		bReady;
	UINT		nMtd;			// 使用第几种方式来找原点

	UINT		nGratingX;		// 100 nm光栅尺
	UINT		nGratingY;		// 100 nm光栅尺
	UINT		nRangeX;		// X向行程
	UINT		nRangeY;		// Y向行程

	CPoint		ptCurPosAb;		// 当前位置的绝对坐标
	float		fDispPos[2];	// 显示出的位置值，以位置绝对坐标为基础，综合考虑XY反向及X反向和Y反向
	UINT		nStageSPX;
	UINT		nStageSPY;

	CString		sXLD;			// X Left Direction
	CString		sXRD;			// X Right Direction
	CString		sYLD;			// Y Left Direction
	CString		sYRD;			// Y Right Direction
	BOOL		bRev;			// 1: left and right should move Y
	BOOL		bRevX;			// 1为反向；0为正向
	BOOL		bRevY;			// 1为反向；0为正向

	CString		sCmdFI1;		// Cmd when Finding Index in step1
	CString		sCmdFI21;		// SP Cmd when Finding Index in step2
	CString		sCmdFI22;		// PR Cmd when Finding Index in step2
	CString		sCmdFI3;		// Cmd when Finding Index in step3
	CString		sCmdLX;			// Cmd of X Limit symbol
	CString		sCmdLY;			// Cmd of Y Limit symbol
	CString		sCmdLX2;		// Cmd of X2 Limit symbol
	CString		sCmdLY2;		// Cmd of Y2 Limit symbol
	CString		sCmdLimitX;		// Cmd of X Limit
	CString		sCmdLimitY;		// Cmd of Y Limit
} ConfigStageXY;

typedef struct tagStageConfigZRT{
	int			nPort;
	BOOL		bStepMotorInited;
	BOOL		bMotionMode;
	BOOL		bReadyZ;
	BOOL		bReadyR;
	BOOL		bReadyT;

	float		fPosZ;
	float		fPosR;
	float		fPosT;
} ConfigStageZRT;

typedef struct tagStageConfig{
	ConfigStageXY	xy;
	ConfigStageZRT	zrt;
	short		nHasAutoStage;
} ConfigStage;

struct structStagePos
{
	DWORD		dwPosX;
	DWORD		dwPosY;
	DWORD		dwShiftX;
	DWORD		dwShiftY;
	CString		strPosName;
};
typedef CTypedPtrList< CPtrList, structStagePos* >		ListStagePos;

// 自动台跟踪标识
const int nSTAGE_TRACE_POINT_RADIUS	= 8;

#define	WM_USER_CCD_MSG		(WM_USER	+41)

// 4. CCD参数
typedef struct tagCCDVideoConfig{
	BOOL		bHasCCD;
	BOOL		bCCDReady;
} ConfigCCDVideo;

// 5. 其它参数
typedef struct tagPublic_Other{
	CString		strChnName1;
	CString		strChnName2;
	UINT		nChnIndex1;
	UINT		nChnIndex2;
}PublicParam_Other;

// 6. 保存的采集参数
typedef struct tagAcquireCondition
{
	CString	strTime;

	UINT	nSpeed;
	UINT	nIsSync;

	UINT	nHV;
	UINT	nHV2;
	UINT	nFil;
	UINT	nEMI;

	CString	strB;
	CString	strC;
	CString	strCL;
	CString	strOL;
	CString	strMag;
	CString	strWD;
	CString	strSignal;
	UINT	nGain;
	UINT	nGain2;
	UINT	nFilter;
}AcquireCondition;

// 7. 快速调节参数
typedef struct tagFastChangeParam
{
	int		nTotal;
	int		nFast[8];
}FastChangeParam;

typedef struct tagFastChanges
{
	FastChangeParam	brt;
	FastChangeParam	brt2;
	FastChangeParam	cst;
	FastChangeParam	cl;
	FastChangeParam	ol;
	FastChangeParam	mag;
	FastChangeParam	hv;
	FastChangeParam	bais;
	FastChangeParam	fila;
	FastChangeParam	beam;
}FastChanges;

/////////////////////////////////////////////////////////////////////
// 调试参数
// 常规参数
typedef struct tagDebug_Comm
{
	UINT	nFFsteps;
	WORD	wBrt0;
	WORD	wBrt1;
	WORD	wCst;
	WORD	wCL;
	WORD	wOL;
	WORD	wDF;
	WORD	wALX;
	WORD	wALY;
	WORD	wStigX;
	WORD	wStigY;
	WORD	wMagX;
	WORD	wMagY;
	WORD	wxoverX;
	WORD	wxoverY;
}DebugParam_Comm;

// 高压电源参数
typedef struct tagDebug_CPS3603
{
	CPS3603_Param	param;
	CPS3603_Status	status;
}DebugParam_CPS3603;

typedef struct tagDebug_Spellman
{
	Spellman_Param	pS;		// 用户（界面）设置值
	Spellman_Param	pM;		// 设置值的实际监测值
	Spellman_Param	pM2;	// 加速度的实际监测值
	Spellman_Param	pM3;	// 回读值监测
	Spellman_Status	status;	// 状态
}DebugParam_Spellman;

// 真空信息
typedef struct tagDebug_Vacuum
{
	SMvac_Param		main;	// Main
	SMvac_Param		mac;	// StateMachine
	SMvac_TCAmp		amp;
	CfgVAC_IP_CCG	ipg;
	CfgVAC_IP_CCG	ipc;
	CfgVAC_IP_CCG	ccg;
	CfgVAC_IP_CCG	board;
}DebugParam_Vac;
/////////////////////////////////////////////////

typedef struct tagParam
{
	PublicParam_Version	ver;
	PublicParam_Comm	comm;
	PublicParam_Scan	scan;
	PublicParam_Other	other;
	DebugParam_Comm		dcomm;
	DebugParam_CPS3603	cps;
	DebugParam_Spellman	spell;
	DebugParam_Vac		vac;
	ConfigStage			stage;
}KYKYParam;
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
const COLORREF g_crOFF			= RGB( 0xA0, 0xA0, 0xA0 );
const COLORREF g_crON_Test		= RGB( 0x00, 0xC0, 0x00 );
const COLORREF g_crON_Comm		= RGB( 0xFF, 0x00, 0x00 );
const COLORREF g_crSYS			= RGB( 0xD8, 0xE4, 0xF8 );
const COLORREF g_crScanEmphasis	= RGB( 0xFF, 0x80, 0x80 );
const COLORREF g_crImage		= RGB( 0xFF, 0x80, 0x00 );

const COLORREF g_crEmphasis1	= RGB( 0x80, 0x00, 0x80 );
const COLORREF g_crEmphasis2	= RGB( 0x00, 0x80, 0x00 );
const COLORREF g_crEmphasis3	= RGB( 0x00, 0x80, 0xFF );
const COLORREF g_crEmphasis4	= RGB( 0xFF, 0x00, 0x00 );

const COLORREF g_crImageBkg		= RGB( 160, 160, 160 );
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//Read data in one line in *.ini.
//In one line the data like ***,*****,*****, they are seperated by comma.
const UINT INTSTYLE		= 1;
const UINT DOUBLESTYLE	= 2;
const UINT LONGSTYLE	= 3;
const UINT STRSTYLE		= 4;
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//button size
const int BUTTON_WIDTH	= 80;
const int BUTTON_HEIGHT	= 30;
const int BUTTON_INTV	= 120;

//buttons positions
const CPoint BTNPower_LOCATION(	20,										-10);
const CPoint BTNVac_LOCATION(	20 + ( BUTTON_WIDTH +BUTTON_INTV ),		-10);
const CPoint BTNLns_LOCATION(	20 + ( BUTTON_WIDTH +BUTTON_INTV )*2,	-10);
const CPoint BTNScn_LOCATION(	20 + ( BUTTON_WIDTH +BUTTON_INTV )*3,	-10);
const CPoint BTNMci_LOCATION(	20 + ( BUTTON_WIDTH +BUTTON_INTV )*4,	-10);
const CPoint BTNSpr_LOCATION(	20 + ( BUTTON_WIDTH +BUTTON_INTV )*4,	-10);
const CPoint BTNCo_LOCATION(	20 + ( BUTTON_WIDTH +BUTTON_INTV )*5,	-10);
const CPoint BTNMon_LOCATION(	5,										-10);
const CPoint BTNAdvance_LOCATION(5 + ( BUTTON_WIDTH +BUTTON_INTV ),		-10);
const CPoint BTNDebug_LOCATION(	5 + ( BUTTON_WIDTH +BUTTON_INTV )*2,	-10);
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
const int g_n16bitDAC_Max			= 0xFFFF;
const int g_n16bitDAC_Mid			= 0x8000;
const int g_n14bitDAC_Max			= 0x3FFF;
const int g_n14bitDAC_Mid			= 0x2000;
const int g_n12bitDAC_Max			= 0x0FFF;
const int g_n12bitDAC_Mid			= 0x0800;

const int g_n16bitDAC_Min_Offset	= -32768;
const int g_n14bitDAC_Min_Offset	= -8192;
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
#define	Diag_Stig_X			0
#define	Diag_Stig_Y			1
#define	Diag_Shift_X		2
#define	Diag_Shift_Y		3
#define	Diag_xover_X		4
#define	Diag_xover_Y		5
#define	Diag_CL				6
#define	Diag_OL				7
#define	Diag_Align_X		8
#define	Diag_Align_Y		9
#define	Diag_Wob			10

#define	Diag_CalI_TC0		11
#define	Diag_CalI_TC1		12
#define	Diag_CalI_TC2		13
#define	Diag_CalI_TC3		14
#define	Diag_SetPoint_TC0	15
#define	Diag_SetPoint_TC1	16
#define	Diag_SetPoint_TC2	17
#define	Diag_SetPoint_TC3	18
#define	Diag_CalI_TC4		19
#define	Diag_CalI_TC5		20
#define	Diag_SetPoint_TC4	21
#define	Diag_SetPoint_TC5	22

#define	Diag_SPRDAC_ChA		23
#define	Diag_SPRDAC_ChB		24
#define	Diag_SPRDAC_ChC		25
#define	Diag_SPRDAC_ChD		26

#define	Diag_Offset_0		27
#define	Diag_Offset_1		28
#define	Diag_Offset_2		29
#define	Diag_Offset_3		30
#define	Diag_Offset_4		31
#define	Diag_Offset_5		32
#define	Diag_FineMag_X		33
#define	Diag_FineMag_Y		34
#define Diag_Pos_X			35
#define Diag_Pos_Y			36

#define	Diag_Total			37

#define Logging_CL			0
#define Logging_OL			1
#define	Logging_Align_X		2
#define Logging_Align_Y		3
#define Logging_Wob			4
#define Logging_Comp_CL		5
#define Logging_Comp_OL		6
#define Logging_Comp_Align_X 7
#define Logging_Comp_Align_Y 8
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
typedef struct tagDL_Diag_Param
{
	int		nCheck;
	int		nMin;
	int		nMax;
	int		nStep;
	int		nIntv;
	int		nCurPos;
	int		nFinish;
	CString	strLabel;
}DL_Diag_Param;

typedef struct tagDL_Logging_Param
{
	BOOL	bLog[6*16];
	CString	strCalc[6*16];
	double	dValue[6*16];
	BOOL	bLogParam[9];
	int		nCheckComp[4];
	double	dAve[100];
}DL_Logging_Param;

typedef struct tagDL_Graph_Param
{
	BOOL	bGraph;
	int		nIndex;
	int		nZoomXIndex;
	int		nZoomYIndex;
	double	dXMin;
	double	dXMax;
	double	dYMin;
	double	dYMax;
}DL_Graph_Param;

struct ADCchnParam
{
	CString	strLabel;
	BOOL	bDiff;
	double	dFactor;
	double	dMin;
	double	dMax;
};

typedef struct tagDL_ADCs_Param
{
	ADCchnParam	chn[6*16];			// 各ADC通道的参数
	int			nAveCounts;			// ADC回读平均次数
}DL_ADCs_Param;

struct logParam
{
	int		nType;	// 1:ADC; 2:gauge compr; 3:Ext. Switch
	
	// 下面都是ADC回读时的参数
	int		nNum;
	int		nMID;
	int		nCID;
	int		nChn;
	BOOL	bType;	// 1:2448; 0:1859
	BOOL	bDiff;	// 1:diff; 0:S.E.
};

typedef struct tagDL_Logging2_Param
{
	int			nCounts;
	logParam	logParams[6*16];
}DL_Logging2_Param;

typedef struct tagDL_Other_Param
{
	int		nElapseChanging;	// 间隔时间
	int		nElapseLogging;
	BOOL	bGaugeCompr;		// 是否记录gauge compr
	BOOL	bGraph;				// 是否绘图
	BOOL	bDebug;				// 是否记录单次回读值
	HANDLE	hLogFile;			// 记录文件
	long	lTimes;				// 当前记录次数
	long	lLoggingDataCounts;	// 13.05.09 Debug模式下，当前记录次数
}DL_Other_Param;
/////////////////////////////////////////////////////////////////////

#define GraphDataBuffGroups	4
#define GraphDataBuffCounts	360000		// 按最快100ms一个数据计算，1秒10个，1分钟600个，1小时36000个，10小时360000个
#define RESO_COUNT				18

/////////////////////////////////////////////////////////////////////
typedef struct tagDiagLogging_Param
{
	DL_Diag_Param		diag[Diag_Total];
	DL_Logging_Param	log;
	DL_Graph_Param		graph[GraphDataBuffGroups];			// 绘图数据
	DL_Graph_Param		dgraph[GraphDataBuffGroups];		// SCN板4组差分绘图数据
	DL_ADCs_Param		adc;
	DL_Other_Param		other;
	DL_Logging2_Param	log2;
}DL_Sum_Param;

typedef struct tagAutoTest_Param
{
	double*	pAutoTestPower;
	double*	pAutoTestPowerExp;
	double*	pAutoTestPowerErr;
	BOOL*	pPower_OutofRange;

	double*	pAutoTestVAC;
	BOOL	bVAC_OutofRange[22];

	double* pAutoTestLNS[4];			// 4条曲线
	int		nAutoTestLNS_Total[4];		// 记录每个曲线的数据个数
	int		nAutoTestLNS_OldPos[4];		// 记录做自动检测前参数的位置

	double* pAutoTestSCN[4];			// 4条曲线
	int		nAutoTestSCN_Total[4];		// 记录每个曲线的数据个数
	int		nAutoTestSCN_OldPos[4];		// 记录做自动检测前参数的位置

	double* pAutoTestMCI[4];			// 4条曲线
	int		nAutoTestMCI_Total[4];		// 记录每个曲线的数据个数
	int		nAutoTestMCI_OldPos[4];		// 记录做自动检测前参数的位置

	double* pAutoTestSPR[4];			// 4条曲线
	int		nAutoTestSPR_Total[4];		// 记录每个曲线的数据个数
	int		nAutoTestSPR_OldPos[4];		// 记录做自动检测前参数的位置

	HANDLE	hLogFile;					// 记录文件
}AutoTest_Param;
/////////////////////////////////////////////////////////////////////

typedef struct
{
	//	BYTE	nField[4];
	//	int		nPort;

	WSADATA	ws;
	SOCKET	ServerSocket;
	struct	sockaddr_in	LocalAddr;
	int		Ret;
	int		AddrLen;
	HANDLE	hThread;
}Net_Param;
///////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
#define AutoTest_16bitDAC_Steps		24
#define AutoTest_16bitDAC_StepsEach	6

#define AutoTest_12bitDAC_Steps		16
#define AutoTest_12bitDAC_StepsEach	4
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
#define Cali_16bitDAC_Steps			32
#define Cali_12bitDAC_Steps			32
#define Cali_16bitDAC_dChange		2048	// 参数范围内取32个点，每次设置改变量为：65536 /32 = 2048
#define Cali_12bitDAC_dChange		128		// 参数范围内取32个点，每次设置改变量为：4096 /32 = 128
#define Cali_Ave_Counts				32		// 每个点读32次取平均值
#define Elapse_Cali_2448			3000	// 每个点读32次取平均值，所需时间：80 + 32 * 80 = 2640 ms
#define Elapse_Cali_1859			600		// 每个点读32次取平均值，所需时间：5 + 32 * 5 = 165 ms (16 +32 *16 = 528 ms)

typedef struct tagCali_Param
{
	double* pCaliLNS[4];
	int		nCaliLNS_Total[4];
	int		nCaliLNS_OldPos[4];

	double* pCaliSCN[4];
	int		nCaliSCN_Total[4];
	int		nCaliSCN_OldPos[4];

	double* pCaliMCI[4];
	int		nCaliMCI_Total[4];
	int		nCaliMCI_OldPos[4];

	double* pCaliSPR[4];
	int		nCaliSPR_Total[4];
	int		nCaliSPR_OldPos[4];

	double	dCaliCoX[11];
	double	dCaliCoY[11];
	double	dCaliCoWD[11];
	BOOL	bCaliCoSel[11];
	int		nCaliCoCounts;

	HANDLE	hLogFile;
}Cali_Param;
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
typedef struct tagMCI_Param
{
	int		nDetPair;
	BOOL	bDet2nd;

	int		nOffset[6];
	int		nGain[6];
	int		nFilter[3];

	CString	strChnName[6];
	int		nChnIndex[2];
}MCI_Param;
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
#define WORDALIGNMENT_CENTER	1
#define WORDALIGNMENT_RIGHT		2
#define WORDALIGNMENT_LEFT		3
#define WORDALIGNMENT_AVERAGE	4
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/*  MCI Machine Instructions
	(Peter S. Chao, Long River Tech, LLC, 
	May 7, 2010: Initial starter set
	Jan. 22, 2011: Significant update)

This is a working document that will continue expand and evolve.
The machine instructions are crafted to fit the operations of
the Master Control and Interface (MCI) board using Acces IO Products' USB DI16A card
as an USB bridge to communicate with the host PC.
The 16 bit high speed "data" channel is used primarily for
streaming video pixels from MCI to the host.
The 18 bits of slow IO pins are used for
sending instructions from host to MCI, and receiving responses from MCI to host.

0.	ACCES IO Basics:
On power up, all ACCES IO ports are tri-stated.
The host configure the IO ports according to
"Hard Coded (Embedded) Configuration" - ACCES IO User Manual, Page 21,
using function "DIO_ConfigureEx".
Configure Ports A and C as output, and Ports B and D as input,
and the high speed data channel as input. Usage of the slow ports are as:

	Port C[2..0] - host command code to MCI
	Port A[7..0] - opcode or operand from host to MCI
	Port D[2..0] - response code from MCI to host
	Port B[3..0] - response data from MCI to host

Note: Ports C and D are always in opposite directions.

After configuration, un-tristate all ports.
(Suggestion: Gain full understanding of DIO_ConfigureEx from ACCES IO sample code.)

From here on, us the function "DIO_WriteAll"
for sending instructions to MCI via 32bit DIOData.  Format for DIOData follows:

Byte_D, Byte_C, Byte_B, Byte_A

Byte_D:  LSb 3 bits for receiving MCI response code
Byte_C:  LSb 3 bits for sending command code to MCI 
Byte_B:  LSb 4 bits for receiving MCI response data
Byte_A:  all 8 bits for sending opcode or operand to MCI

Example: DIO_WriteAll sends 3bit command code 0b101,
		 and 8bit opcode 0b00010001 to MCI by setting 

 	DioData = 0b11111111,11110101,11111111,00010001, or
			= 0xFFF5FF11

I.	Command protocol from host to MCI:
A host command consists of a command code (cc), an operation code (opcode),
and zero or one operand, which can be one or two bytes long.

 	Command code: C[2..0] =	0b000 - Clear
 						0b001 - Operand low byte in Channel A
 						0b010 - Operand high byte in Channel A
 						0b011 - TBD
 						0b100 - Opcode in Channel A, no operand
 						0b101 - Opcode in Channel A, operand to follow
 						0b111 - Simultaneous update of opcode and operand to MCI
 	Sending a command:
 	A command is sent in three to five steps, depending on the related operand.
 	 	For instruction with opcode only, no operand
 		Step 1:	Set cc = 0b100 from Channel C
 				Set opcode in Channel A
		Step 2:	Set cc = 0b111 in Channel C
		Step 3:	Set cc = 0b000 in Channel C
 
 		For instructions with a 2-byte long operand
  		Step 1:	Set cc = 0b101 in Channel C
 				Set opcode in Channel A
 		Step 2:	Set cc = 0b001 in Channel C
 				Set operand low byte in Channel A
 		Step 3:	Set cc = 0b010 in Channel C
 				Set operand high byte in Channel A
		Step 4:	Set cc = 0b111 in Channel C
		Step 5:	Set cc = 0b000 in Channel C

II.	Instruction for setting pixel clock rate:	
	MCI uses a Phase Lock Loop (PLL) circuit to generate,
	from an 40MHz external oscillator, to generate
	a 32MHz systems clock, and a 8MHz base pixel clock as well.
	In addition, a custom clock generator is used to produce variable pixel clocks on demand.
	This is a "divide by N" clock generator, where N = 1, 2, 3, …, 2K,
	with 50% duty cycle for even N, off by 1 tick for odd N.
	For a 8MHz base, it means the pixel clock can be set from 8MHz to 4KHz.  

	The pixel rate in units of 1/8 MHz, namely 125nS.
	Pixel rate = N means N * 125nS for pixel time.

 	Instruction Set_Pixel_Rate:	Opcode = 0x11
 								Operand =0b0000,RRRR, high byte,0bRRRR,RRRR low byte

IV.	Instructions for setting scan ramps:
	A 4-segment scan ramp consists of the following segments:

 	Vo - Offset in DAC unit for the scan DAC.  MCI uses 16 bit DACs for scanning,
		so the units range from 0 to 64K.  MSB 14 bits are used by the scan DAC.

 	valley	-	number of pixels for staying at DAC value Vo
 
	rise	-	number of pixels for climbing up the ramp

 	dRise	-	for each pixel, how many DAC units to climb

	peak	-	number of pixels for staying at the top of the ramp

 	fall	- 	number of pixels for coming down the ramp

	dFall	-	for each pixel, how many DAC units to descent
*/

#define USB_PORT_A									0
#define USB_PORT_B									1
#define USB_PORT_C									2
#define USB_PORT_D									4

#define	USB_SetPixelRate							0x11
#define	USB_StartScan								0x12
#define	USB_StopScan								0x13
#define	USB_UpdateXramp								0x14
#define	USB_UpdateYramp								0x15
#define	USB_SetDefaultRamps							0x16
#define	USB_UpdateRotation							0x17
#define	USB_RotationOFF								0x18
#define	USB_RotationON								0x19
#define	USB_RotationSetCos							0x1A
#define	USB_RotationSetSin							0x1B

#define	USB_SetXoffset								0x21
#define	USB_SetXvalley								0x22
#define	USB_SetXrise								0x23
#define	USB_SetdXrise								0x24
#define	USB_SetXpeak								0x25
#define	USB_SetXfall								0x26
#define	USB_SetdXfall								0x27
#define	USB_SetYoffset								0x28
#define	USB_SetYvalley								0x29
#define	USB_SetYrise								0x2A
#define	USB_SetdYrise								0x2B
#define	USB_SetYpeak								0x2C
#define	USB_SetYfall								0x2D
#define	USB_SetdYfall								0x2E

#define	USB_Det03StoreDAC							0x31
#define	USB_Det03LoadDACs							0x32
#define	USB_Det45StoreDAC							0x33
#define	USB_Det45LoadDACs							0x34
#define	USB_SetDetFilter							0x35
#define	USB_SetDetGain								0x36

#define	USB_VideoXYZ								0x41
#define	USB_VideoZZZ								0x42
#define	USB_ScanModeNormal							0x43
#define	USB_ScanModeLineHorizontal					0x44
#define	USB_ScanModeLineVertical					0x45
#define	USB_ScanModeSpot							0x46

#define	USB_ReadVersion								0x48	// no operand, MCI returns: port D 0x01, port B MCI version number

#define	USB_FreezeX									0x49
#define	USB_FreezeY									0x4A

/*
Opcodes:	0x49 - freezeX, operand = pixel number
				MCI computes Xramp DAC value as Xoffset + pixel number * dXrise
			0x4A - freezeY, operand = line number
				MCI computes Yramp DAC value as Yoffset + line number * dYrise

			0x43 - normal scan mode, no operand
			0x44 - horizotal line mode, line number given by "freezeY"
			0x45 - vertical line mode, line number given by "freezeX"
			0x46 - spot mode, with spot at (freezeX, freezeY)
*/

#define	USB_DuelDet									0x4B	// open duel detector
#define	USB_SingleDet								0x4C	// close duel detector

#define	USB_Video_ON								0x4D
#define	USB_Video_OFF								0x4E

#define	USB_DetPairSelection						0x55

/*
For detector pair selection:      
Opcode:		x55,
operand:	x00	-	Detector pair 0-1, default (Note, old default was pair 4-5)
			x01	-	Detector pair 2-3
			x02	-	Detector pair 4-5
			x03	-	spare
input connector:	J5A - pair0-1, J5B - pair2-3, J6A - pair4-5
*/
#define USB_Read_Pwr								0x56

///////////////////////////////////////////////////////////////////////////////
/*
I.  Set interrupt mask register (IMR)
Opcode - 0xF4, operand - LSB[5..0], one bit of each interrupt source.
Bit set to 1 to enable, 0 to mask off.
	Bit 0 -	INT 4 ( IMR = 0x01 ), for SPR board, highest priority, cB will be 0x04
	Bit 1 -	INT 3 ( IMR = 0x02 ), for VAC board, next higher priority, cB will be 0x03
	Bit 2 -	INT 2 ( IMR = 0x04 ), for LNS board, no interrupt from it at present; MCI skips it,
									SW can also set it to 0, cB will be 0x02
	Bit 3 -	INT 1 ( IMR = 0x08 ), for SCN board, cB will be 0x01
	Bit 4 -	INT 5, spare
	Bit 5 -	INT 6, spare

II.  Protocol
1.	On system starting up, host software masks off appropriate bits in the IMR for wanted time
	duration, then sets right bits to enable wanted interrupts.
2.	MCI monitors interrupts from down-stream boards and notifies the host .  If interrupts come 
	from more than one board, the higher priority one will be handled first.

	Case A - while scanning, MCI sets the LSB bit, Z[0], of the video data to 1, USB bridge Channel
				cD to 2, Channel cB to INT number.
				Upon detecting Z[0]=1, host software services required interrupt of the 
				corresponding board indicated by the INT number.
				Once service is done, software sends acknowledgement to MCI by sending "clear_DB"
				command, opcode=ox47, no operand.
				MCI then stops setting Z[0] to 1, and clears cD and cB .
	Case B - while not scanning, host software monitors cD and cB via periodic polling, services
				the interrupt and send MCI "clear_DB" when done.
3.	Send host cD = 3, cB = 1 when BoardCommReady is asserted.
4.	MCI sends active INT# corresponding to the "1" bits in the "INT_mask (IMR)", with
	bit0 having the highest priority, bit1 next highest, etc. It then waits for the host ACK
	(clear_DB).
	Example: IMR=000111, INT4 and INT3 are active, cB=0x4 will be sent to the host
	first. Upon receiving host ACK, cB=0x4 will be sent again if INT4 remains active.
	Otherwise cB=0x3 will be sent.

For temporary testing with MCI only, use command INT_test, opcode 0xF5.
MCI will send 2 interrupts to the host: INT4 and INT1.
*/
#define	USB_ClearDB									0x47	// clear port B & D, no operand
#define	USB_SetIMR									0xF4	// set interrupt mask regiter
//#define	USB_INT_test								0xF5
//#define	USB_Fake_Mains								0xF6	// MCI generates a fake Mainslock pulse to start the next Xramp for one line

#define	USB_MainsLock_ON							0x1C
#define	USB_MainsLock_OFF							0x1D

#define	USB_MCI_COMM_RESET							0xF2	// MCI communicate reset

///////////////////////////////////////////////////////////////////////////////
/*
Individual interrupt testing:

INT_test_ON - Opcode 0xF5
INT_test_OFF- Opcode 0xF6

INT_test_SPR - Opcode 0xF7, MCI sets (D,B) to (4,2)
INT_test_VAC - Opcode 0xF8, MCI sets (D,B) to (3,2)
INT_test_LNS - Opcode 0xF9, MCI sets (D,B) to (2,2)
INT_test_SCN - Opcode 0xFA, MCI sets (D,B) to (1,2)

Process:
	INT_test_ON,
		INT_test_XXX,  Clear_DB
		INT_test_YYY,  Clear_DB
		. . . . 
	INT_test_OFF
*/
#define	USB_INT_test_ON								0xF5
#define	USB_INT_test_OFF							0xF6
#define	USB_INT_test_SPR							0xF7
#define	USB_INT_test_VAC							0xF8
#define	USB_INT_test_LNS							0xF9
#define	USB_INT_test_SCN							0xFA
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/*
Line Sync and Frame Sync:

In case a clean mainslock signal is not available, a fake mainslock signal internally generated
in MCI can be used for testing.  This signal, fakemains,  is a square wave currently set at
about 500Hz for easier scoping.
MCI selects either normal mainslock input signal or generate fakemains as mains_pulse, depending ON whether the command fakemains_ON/OFF has been invoked.
Default is fakemains OFF.
A short pulse (1 pixel clock wide), mains_pulse_X is extracted from the rise edge of,
mains_pulse as for line sync, and a wider one, mains_pulse_Y, for frame sync.

Commands:	mainslock_ON - 0x1C
			mianslock_OFF - 0x1D
			line_sync option -0x1E
			frame_sync option - 0x1F
			the above are mutually exclusive, line_sync is the default;
			upon receiving one of the two commands, MCI automatically turns the
			other OFF.
			fakemains_ON - 0xFB
			fakemains_OFF - 0xFC
*/
#define	USB_MainsLock_LineSync						0x1E
#define	USB_MainsLock_FrameSync						0x1F
#define	USB_FakeMains_ON							0xFB
#define	USB_FakeMains_OFF							0xFC
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//====== PC -> SUB BOARD MessageID ==============================
/*  3200 SEM SubBoard Communications (Dan Downer, Long River Tech, LLC, Version Dec. 30, 2011)

Communication to all boards "downstream" from the MCI requires issuance of
at least two contiguously sequential instruction clusters with operands.
Some operations require three or more instruction clusters, depending on the purpose.
For example, 24 bit writes require 3 instruction clusters to be written,
and reads require additional interaction with the MCI's USB interface.  

Each instruction cluster is comprised of five back-to-back 32 bit writes
to the MCI board over the USB interface.
The opcode and operands are sent to the MCI using DIO bytes C and A as follows:


0xFFF5FFF1  		(Instruction for subboard comm.)
0xFFF1FF00 Logically OR'ed with 8 bits of Operand 1 data 
0xFFF2FF00 Logically OR'ed with 8 bits of Operand 2 data
0xFFF7FF00 			(initiate the transfer)
0xFFF0FF00 			(finalize the transfer)


The first instruction cluster defines the transfer type and circuit selection.
Subsequent instruction clusters define the data sent from the MCI to the destination circuit.
One additional instruction cluster is sufficient for 8 bit and 16 bit writes.
A third instruction cluster is required for 24 bit and 32 bit writes.
(At the time this document was written, there have been no 32 bit writes required,
 but several 24 bit writes are implemented.)

Transfer type (TT), module selection (MMMM) and circuit selection (CCCCCCCC):
TTTTMMMMCCCCCCCC

TT[3..0]	= binary 0001 for 8 bit write, 0010 for 16 bit write, 0011 for 24 bit write, 0100 for 32 bit write
			= binary 1001 for 8 bit read, 1010 for 16 bit read (1 word back to MCI)
			= binary 1011 for 24 bit read, 1100 for 32 bit read* (2 words back to MCI)
				* 32 bit reads not necessarily supported by all boards as of 12-3-11
			= binary 0101 for clearing of INT's from subboards (data selects which INT to clear)

M[3..0] = module selection:
MCI (local) address: 1101 (for diagnostic tests only - not for streaming video)
SCN board address:	0001
LNS board address:	0010
VAC board address:	0011
SPR module address:	0100

C[7..0] = Circuit selection (see individual module sections for details)
*/
//===========================================================
// Transfer type
//===========================================================
#define	USB_TTTT_Write_08							0x01	// 8 bit write
#define	USB_TTTT_Write_16							0x02	// 16 bit write
#define	USB_TTTT_Write_24							0x03	// 24 bit write
#define	USB_TTTT_Write_32							0x04	// 32 bit write

#define	USB_TTTT_Read_08							0x09	// 8 bit read
#define	USB_TTTT_Read_16							0x0A	// 16 bit read
#define	USB_TTTT_Read_24							0x0B	// 24 bit read
#define	USB_TTTT_Read_32							0x0C	// 32 bit read

#define	USB_TTTT_INT								0x02	// for clearing of INT's from subboards (data selects which INT to clear)
#define	USB_TTTT_Mon_Err							0x03	// have error when read monitor data
#define	USB_TTTT_Switches							0x04	// read selected external switch state


//===========================================================
// MID --- Module Selection
//===========================================================
#define	USB_MID_MCI1								0x0D	// MCI (local), for diagnostic tests only - not for streaming video
#define	USB_MID_MCI2								0x0E	// MCI (local), for enable 262ms inactivity timer self-reset of communication

#define	USB_MID_SCN									0x01	// SCN board
#define	USB_MID_LNS									0x02	// LNS board
#define	USB_MID_VAC									0x03	// VAC board
#define	USB_MID_SPR									0x04	// SPR module


//===========================================================
// TTTTMMMM --- Type & Module Selection
//===========================================================
#define	USB_TM_SCN_Write_08							0x11
#define	USB_TM_SCN_Write_16							0x21
#define	USB_TM_SCN_Write_24							0x31
#define	USB_TM_SCN_Write_32							0x41
#define	USB_TM_SCN_Read_08							0x91
#define	USB_TM_SCN_Read_16							0xA1
#define	USB_TM_SCN_Read_24							0xB1
#define	USB_TM_SCN_Read_32							0xC1

#define	USB_TM_LNS_Write_08							0x12
#define	USB_TM_LNS_Write_16							0x22
#define	USB_TM_LNS_Write_24							0x32
#define	USB_TM_LNS_Write_32							0x42
#define	USB_TM_LNS_Read_08							0x92
#define	USB_TM_LNS_Read_16							0xA2
#define	USB_TM_LNS_Read_24							0xB2
#define	USB_TM_LNS_Read_32							0xC2

#define	USB_TM_VAC_Write_08							0x13
#define	USB_TM_VAC_Write_16							0x23
#define	USB_TM_VAC_Write_24							0x33
#define	USB_TM_VAC_Write_32							0x43
#define	USB_TM_VAC_Read_08							0x93
#define	USB_TM_VAC_Read_16							0xA3
#define	USB_TM_VAC_Read_24							0xB3
#define	USB_TM_VAC_Read_32							0xC3

#define	USB_TM_SPR_Write_08							0x14
#define	USB_TM_SPR_Write_16							0x24
#define	USB_TM_SPR_Write_24							0x34
#define	USB_TM_SPR_Write_32							0x44
#define	USB_TM_SPR_Read_08							0x94
#define	USB_TM_SPR_Read_16							0xA4
#define	USB_TM_SPR_Read_24							0xB4
#define	USB_TM_SPR_Read_32							0xC4

/*
Operand data alignment:

Each instruction cluster of 5 USB write transfers to the MCI board allow up to two operands of data.
The first cluster's operands 1 and 2 specify the transfer type,
module selection and circuit selection as described previously.
The second (and third if necessary) instruction(s)' operands 3 and 4,
and 5 and 6 if necessary, contain data to be written to the selected module and circuit.  

All reads of subboards include Operands 3 and 4 for write data
for preconditioning the circuitry for the read in process.
This is necessary for operations such as reading a multi-channel ADC (analog to digital converter)
that must first be instructed to select a channel or conversion mode.
In other cases no write data is necessary, in which case it is recommended to set Operands 3 and 4 to 0.

Instruction cluster #1 (always required):
0xFFF5FFF1	(Instruction for subboard comm.)
0xFFF1FF00 	Logically OR'ed with 8 bits of Operand 1 data 
0xFFF2FF00 	Logically OR'ed with 8 bits of Operand 2 data
0xFFF7FF00	(initiate the transfer)
0xFFF0FF00	(finalize the transfer)

Instruction cluster #2 (always required):
0xFFF5FFF1	(Instruction for subboard comm.)
0xFFF1FF00 	Logically OR'ed with 8 bits of Operand 3 data 
0xFFF2FF00 	Logically OR'ed with 8 bits of Operand 4 data
0xFFF7FF00	(initiate the transfer)
0xFFF0FF00	(finalize the transfer)

Third instruction cluster - send ONLY  for 24 bit or 32 bit write transfer types 
- DO NOT SEND a third instruction cluster for other transfer types!
0xFFF5FFF1	(Instruction for subboard comm.)
0xFFF1FF00 	Logically OR'ed with 8 bits of Operand 5 data 
0xFFF2FF00 	Logically OR'ed with 8 bits of Operand 6 data
0xFFF7FF00	(initiate the transfer)
0xFFF0FF00	(finalize the transfer)

Required contents of each operand per transfer type are shown in the table below.
Where "Set to 0" is indicated, operand value should be set to a value of 0.
Note that data operands (Operands 3 through 6) contain different byte fields
depending on the type of transfer.  Where "xx" appears, operands 5 and 6 do not exist.

Transfer Type	Operand 1	Operand 2	Operand 3		Operand 4		Operand 5	Operand 6
8 bit write		CCCCCCCC	0001MMMM	Set to 0		data[7..0]		xx			xx
16 bit write	CCCCCCCC	0010MMMM	data[7..0]		data[15..8]		xx			xx
24 bit write	CCCCCCCC	0011MMMM	data[15..8]		data[23..16]	Set to 0	data[7..0]
32 bit write	CCCCCCCC	0100MMMM	data[23..16]	data[31..24]	data[7..0]	data[15..8]
All reads		CCCCCCCC	TTTTMMMM	data[7..0]		data[15..8]		xx			xx
Clear INT		CCCCCCCC	0101MMMM	Set to 0		data[7..0]		xx			xx
*/
//===========================================================
// CID --- Circuit Selection
//===========================================================
//																Write		Read	Data Format / Usage Comments
// SCN board
#define	USB_CID_SCN_Stigmation						0x12	// 16 bit	-			MSB: Set to 0 for X Stig, set to 1 for Y Stig.  12 bit value in bits 11...0

#define	USB_CID_SCN_Xshift_Crossover				0x22	// 16 bit	-			MSB: Set to 0 for X shift, set to 1 for X Crossover Scaling value.  12 bit value in bits 11..0
#define	USB_CID_SCN_Yshift_Crossover				0x32	// 16 bit	-			MSB: Set to 0 for Y shift, set to 1 for Y Crossover Scaling value.  12 bit value in bits 11..0

#define	USB_CID_SCN_XMainGain_FeedbackSetting		0x24	// 8 bit	-			Bits 3..2 select X main path feedback, Bits 1..0 select gain.
#define	USB_CID_SCN_XLowRangeFeedbackSelect			0x26	// 8 bit	-			Bits 3..0 select feedback for X low range path
#define	USB_CID_SCN_XLowRangeAnalogMultiplexerSel	0x28	// 8 bit	-			Bits 3..0 select X low range mux

#define	USB_CID_SCN_YMainGain_FeedbackSetting		0x34	// 8 bit	-			Bits 3..2 select Y main path feedback, Bits 1..0 select gain.
#define	USB_CID_SCN_YLowRangeFeedbackSelect			0x36	// 8 bit	-			Bits 3..0 select feedback for Y low range path
#define	USB_CID_SCN_YLowRangeAnalogMultiplexerSel	0x38	// 8 bit	-			Bits 3..0 select Y low range mux

#define	USB_CID_SCN_ExpanderChip					0x72	// 8 bit	-			Set bit 7 to strobe to High, Set bit 6 to strobe to Low.  Bits 4..0 select output pin to be changed.

#define	USB_CID_SCN_ADC_Write						0xC2
#define	USB_CID_SCN_ADC_Busy						0xC5	// 16 bit	-	8 bit	Bit 0 = 1 if ADC is busy (conversion not done), Bit 0 = 0 if ADC data is ready to send data to host
#define	USB_CID_SCN_ADC_Read						0xC8	// 16 bit	-	32 bit	Upper 13 bits written at the beginning of a read sequence.  Read clocks shared with write clocks, one cycle delayed.
#define	USB_CID_SCN_Test_LED						0xD0	// 8 bit	-			Bit 0	

#define	USB_CID_SCN_ReadBoardID						0xE1	//			-	8 bit	Initial LRT released SCN board value is fixed at 0xA1
#define	USB_CID_SCN_Soft_Reset						0xEE	// 8 bit	-			Data-less write causes "soft" reset/initialization of SCN board.  DAC's are set to midrange, outputs off.

// ExpanderChip definition
#define	SCN_ExpanderChip_YGainRlyCtrl				0x01	// SCN_ExpanderChip_Xcros1
#define	SCN_ExpanderChip_Xcros2						0x02
#define	SCN_ExpanderChip_Xcros3						0x03
#define	SCN_ExpanderChip_Xcros4						0x04
#define	SCN_ExpanderChip_XcrossFBsel1				0x05
#define	SCN_ExpanderChip_XcrossFBsel2				0x06
#define	SCN_ExpanderChip_XcrossFBsel3				0x07
#define	SCN_ExpanderChip_XcrossFBsel4				0x08
#define	SCN_ExpanderChip_Ycros1						0x09
#define	SCN_ExpanderChip_Ycros2						0x0A	// 10
#define	SCN_ExpanderChip_Ycros3						0x0B	// 11
#define	SCN_ExpanderChip_Ycros4						0x0C	// 12
#define	SCN_ExpanderChip_YcrossFBsel1				0x0D	// 13
#define	SCN_ExpanderChip_YcrossFBsel2				0x0E	// 14
#define	SCN_ExpanderChip_YcrossFBsel3				0x0F	// 15
#define	SCN_ExpanderChip_YcrossFBsel4				0x10	// 16
#define	SCN_ExpanderChip_XGainRlyCtrl				0x11	// 17
#define	SCN_ExpanderChip_XShortRlyCtrl				0x12	// 18
#define	SCN_ExpanderChip_XAmpStigESCtrl				0x13	// 19
#define	SCN_ExpanderChip_XMainRlyCtrl				0x14	// 20
#define	SCN_ExpanderChip_XMainCathCtrl				0x15	// 21
#define	SCN_ExpanderChip_XMainAmpESCtrl				0x16	// 22
#define	SCN_ExpanderChip_XShiftESCtrl				0x17	// 23 SCN_ExpanderChip_XCrosESCtrl
#define	SCN_ExpanderChip_XLowAmpESCtrl				0x18	// 24
#define	SCN_ExpanderChip_YShortRlyCtrl				0x19	// 25
#define	SCN_ExpanderChip_YAmpStigESCtrl				0x1A	// 26
#define	SCN_ExpanderChip_YMainRlyCtrl				0x1B	// 27
#define	SCN_ExpanderChip_YMainCathCtrl				0x1C	// 28
#define	SCN_ExpanderChip_YMainAmpESCtrl				0x1D	// 29
#define	SCN_ExpanderChip_YShiftESCtrl				0x1E	// 30 SCN_ExpanderChip_YCrosESCtrl
#define	SCN_ExpanderChip_YLowAmpESCtrl				0x1F	// 31
// ExpanderChip definition

//===========================================================
// LNS board
#define	USB_CID_LNS_CondenserLens					0x12	// 16 bit	-			Bits 1 and 0 are ignored if LTC1591-1 type DAC is used instead of LTC1597-1.
#define	USB_CID_LNS_CondenserLens_LoopTest			0x1A	// 8 bit	-			
#define	USB_CID_LNS_CondenserLens_Wobbler			0x1C	// 8 bit	-			Set bit 0 to 1 to enable wobbler to modulate Condenser lens.
#define	USB_CID_LNS_CondenserLens_OutputDrive		0x1E	// 8 bit	-			

#define	USB_CID_LNS_ObjectiveLens_Static			0x22	// 16 bit	-			Bits 1 and 0 are ignored if LTC1591-1 type DAC is used instead of LTC1597-1.
//#define	USB_CID_LNS_ObjectiveLens_DynFocusPolarity	0x24	// 8 bit	-			Set bit 0 to 1 to INCREASE value each scan line, set bit 0 to 0 to DECREASE value each scan line.
#define	USB_CID_LNS_ObjectiveLens_DynFocusPolarityAtten		0x28	// 8 bit	-	Bit 2=DF polarity (0=neg ramp, 1=pos ramp), Bit 1=1 sets full range DF, Bit0=1 sets 10% range dynamic focus. Bits 1,0=00 for off.
#define	USB_CID_LNS_ObjectiveLens_LoopTest			0x2A	// 8 bit	-			
#define	USB_CID_LNS_ObjectiveLens_Wobbler			0x2C	// 8 bit	-			Set bit 0 to 1 to enable wobbler to modulate Objective lens.
#define	USB_CID_LNS_ObjectiveLens_OutputDrive		0x2E	// 8 bit	-			

#define	USB_CID_LNS_ObjectiveLens_DynFocusSync		0x30	// 8 bit	-			Data-less write signals Objective Lens dynamic focus circuit that a new scan line has just been started.
#define	USB_CID_LNS_ObjectiveLens_DynFocusSteps		0x32	// 16 bit	-			Objective Lens dynamic focus change per image scan line, in DAC steps (14 bit or 16 bit DAC depending on build option).
#define	USB_CID_LNS_ObjectiveLens_DnyFocusStartPoint 0x34	// 16 bit	-			Objective Lens dynamic focus level at start of line, bit count (14 or 16) same as U51 DAC.
#define	USB_CID_LNS_ObjectiveLens_DnyFocusEndBound	0x36	// 16 bit	-			Objective Lens dynamic focus level boundary (either pos or neg direction), bit count (14 or 16) same as U51 DAC.

#define	USB_CID_LNS_XAlignment						0x42	// 16 bit	-			
#define	USB_CID_LNS_XAlignment_LoopTest				0x4A	// 8 bit	-			
#define	USB_CID_LNS_XAlignment_Wobbler				0x4C	// 8 bit	-			
#define	USB_CID_LNS_XY_OutputDrive					0x4E	// 8 bit	-			

#define	USB_CID_LNS_YAlignment						0x52	// 16 bit	-			
#define	USB_CID_LNS_YAlignment_LoopTest				0x5A	// 8 bit	-			
#define	USB_CID_LNS_YAlignment_Wobbler				0x5C	// 8 bit	-			

#define	USB_CID_LNS_WobblerSetting					0x62	// 16 bit	-			12 bit value sets Wobbler level

#define	USB_CID_LNS_ADC_Write						0xC2	// 16 bit	-			
#define	USB_CID_LNS_ADC_HD_Start_Conversion			0xC4	// 16 bit	-			Can start conversion independently, then wait for required conversion time, then read.
#define	USB_CID_LNS_ADC_Read						0xC8	// 16 bit	-	16 bit	Writing while reading results in data being returned to host from previous conversion.
#define	USB_CID_LNS_Test_LED						0xD0	// 8 bit	-			Set bit 0 to 1 to test front panel LED 4 (near term engineering use only - will be reassigned at a later date)

#define	USB_CID_LNS_ReadBoardID						0xE1	//			-	8 bit	Initial LRT released LNS board value is fixed at 0xA2
#define	USB_CID_LNS_Soft_Reset						0xEE	// 8 bit	-			Data-less write causes "soft" reset/initialization of LNS board.  DAC's are set to midrange, outputs off.

//===========================================================
// VAC board
#define	USB_CID_VAC_CalCur_TC0_3					0x12	// 24 bit	-			ChA: TC0 Cal I, ChB: TC1 Cal I, ChC: TC2 Cal I, ChD: TC3 Cal I  (DAC type: DAC7565)
#define	USB_CID_VAC_CompAdjPoints_TC0_3				0x22	// 24 bit	-			ChA: TC0 comparator adjustment, ChB: TC1 adj, ChC: TC2 adj, ChD: TC3 adj (DAC type: DAC7565)
#define	USB_CID_VAC_CalCur_CompAdjPoints_TC4_5		0x32	// 24 bit	-			ChA: TC5 Cal I, ChB: TC6 Cal I, ChC: TC5 comparator adjustment, ChD: TC6 adj (DAC type: DAC7565)

#define	USB_CID_VAC_Relay00Enable					0x40	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay01Enable					0x41	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay02Enable					0x42	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay03Enable					0x43	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay04Enable					0x44	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay05Enable					0x45	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay06Enable					0x46	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay07Enable					0x47	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay08Enable					0x48	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay09Enable					0x49	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay10Enable					0x4A	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off
#define	USB_CID_VAC_Relay11Enable					0x4B	// 8 bit	-			Set bit 0 to 1 to enable relay ON (if logic also enables it to be on), Set bit 0 to 0 to force relay off

//#define	USB_CID_VAC_Auxiliary_IO_Comp				0x52	//			-	8 bit	2 lines, Aux1 and Aux0 (parallel read)

#define	USB_CID_VAC_ExpanderChip					0x72	// 8 bit	-			Set bit 7 to strobe to High, Set bit 6 to strobe to Low.  Bits 5..0 select output pin to be changed.

//#define	USB_CID_VAC_ReadExternalSwitch				0x92	//			-	8 bit	8 lines, SwClrIn[7..0] (parallel read)
#define	USB_CID_VAC_ReadThermoCCGComp				0x94	// 16 bit	-	16 bit	Bits 13 & 12 are CCG comparators, Bits 11..0 are TC comparators 5 through 0 respectively (B/high and A/low per channel)
#define	USB_CID_VAC_ReadSelExtSwitchState			0x98	//			-	8 bit	The switch to be read is selected by the VAC Expander, setting outputs 0x19, 0x1A and 0x1B to select the switch

#define	USB_CID_VAC_INT_Enable_Register				0xA1	// 16 bit	-			Each bit enables a designated interrupt source (see "Interrupts" tab for details)
#define	USB_CID_VAC_INT_Source_Register				0xA4	//			-	16 bit	Identifies which (if any) enabled interrupts are currently being asserted. Reading this register clears all bits unless condition persists.
//#define	USB_CID_VAC_ExtSwitch_Polarity_Selects		0xA8	// 8 bit	-			Bits 0 through 7 choose whether each external switch generates an (enabled) interrupt in CLOSED or OPEN positions

#define	USB_CID_VAC_ADC0_Write						0xC2	//						It is possible that no dedicated writes will be needed since writes are also part of read sequence.
#define	USB_CID_VAC_ADC01_Busy						0xC5	// 16 bit	-	16 bit	Bit 0 = 1 if ADC #0 is busy (conversion not done), Bit 0 = 0 if ADC #0 data is ready to send data to host. Bit 1 same for ADC #1
#define	USB_CID_VAC_ADC0_Read						0xC6	// 16 bit	-	32 bit	Upper 13 bits written at the beginning of a read sequence.  Read clocks shared with write clocks, one cycle delayed.
#define	USB_CID_VAC_ADC1_Write						0xCA	//						It is possible that no dedicated writes will be needed since writes are also part of read sequence.
//#define	USB_CID_VAC_ADC1_Busy						0xCD	// 16 bit	-	8 bit	Bit 0 = 1 if ADC #1 is busy (conversion not done), Bit 0 = 0 if ADC #1 data is ready to send data to host
#define	USB_CID_VAC_ADC1_Read						0xCE	// 16 bit	-	32 bit	Upper 13 bits written at the beginning of a read sequence.  Read clocks shared with write clocks, one cycle delayed.

#define	USB_CID_VAC_Test_LED						0xD0	// 8 bit	-			Set bit 0 to 1 to test front panel LED 4 (near term engineering use only - will be reassigned at a later date)

#define	USB_CID_VAC_ReadBoardID						0xE1	//			-	16 bit	Initial LRT released VAC board value is fixed at 0xA3A3
#define	USB_CID_VAC_Soft_Reset						0xEE	// 8 bit	-			Data-less write causes "soft" reset/initialization of VAC board.  DAC's are midranged. Most drives set to off.

//===========================================================
// SPR module
#define	USB_CID_SPR_AnalogPower_Enable				0x10	// 8 bit	-			Set bit 0 to 1 to begin orderly analog power-up sequence.  Set bit 0 to 0 to begin analog power shutdown sequence.
#define	USB_CID_SPR_General_Purpose_DACs			0x14	// 24 bit	-			ChA: PMT, ChB: AnOut2, ChC: AnOut3, ChD: AnOut4  (DAC type: DAC8565)

#define	USB_CID_SPR_Fan_PWM_Control					0x44	// 8 bit	-			Set FF for full speed control of fans, 0 for off, other values for PWM speed control
#define	USB_CID_SPR_ExpanderChip					0x72	// 8 bit	-			Set bit 7 to strobe to High, Set bit 6 to strobe to Low.  Bits 5..0 select output pin to be changed.

#define	USB_CID_SPR_PowerStatus_Readback			0x80	// 16 bit	-	8 bit	Bits 2..0 Report status of Emerson Power Supply ACOK and DCOK signals, Fan OK

#define	USB_CID_SPR_ReadExternalSwitch				0x92	// 16 bit	-	8 bit	2 lines, SwClrIn[1..0] (parallel read) (Note: could be expanded to four in future if necessary)

#define	USB_CID_SPR_INT_Enable_Register				0xA1	// 8 bit	-			Each bit enables a designated interrupt source
#define	USB_CID_SPR_INT_Source_Register				0xA4	//			-	8 bit	Identifies which (if any) enabled interrupts are currently being asserted. Reading this register clears all bits unless condition persists.
#define	USB_CID_SPR_ExtSwitch_Polarity_Selects		0xA8	// 8 bit	-			Bits 0 through 3 choose whether each external switch generates an (enabled) interrupt in CLOSED or OPEN positions

#define	USB_CID_SPR_Legacy_HV_Power					0xB4	// 16 bit	-			This is for optional control of the older CPS Power Supply used in legacy systems.  9 bits controls 0-30kV output level.

#define	USB_CID_SPR_ADC0_Write						0xC2	//						It is possible that no dedicated writes will be needed since writes are also part of read sequence.
#define	USB_CID_SPR_ADC01_Busy						0xC5	//			-	8 bit	Bit 0 = 1 if ADC #0 is busy (conversion not done), Bit 0 = 0 if ADC #0 data is ready to send data to host. Bit 1 same for ADC #1
#define	USB_CID_SPR_ADC0_Read						0xC6	// 16 bit	-	32 bit	Upper 14 bits written at the beginning of a read sequence.  Read clocks shared with write clocks, one cycle delayed.
#define	USB_CID_SPR_ADC1_Write						0xCA	//						It is possible that no dedicated writes will be needed since writes are also part of read sequence.
//#define	USB_CID_SPR_ADC1_Busy						0xCD	//			-	8 bit	Bit 0 = 1 if ADC #1 is busy (conversion not done), Bit 0 = 0 if ADC #1 data is ready to send data to host
#define	USB_CID_SPR_ADC1_Read						0xCE	// 16 bit	-	32 bit	Upper 14 bits written at the beginning of a read sequence.  Read clocks shared with write clocks, one cycle delayed.

#define	USB_CID_SPR_Test_LED						0xD0	// 8 bit	-			Set bit 0 to 1 to test front panel LED 4 (near term engineering use only - will be reassigned at a later date)

#define	USB_CID_SPR_ReadBoardID						0xE1	//			-	8 bit	Initial LRT released SPR board value is fixed at 0xA4
#define	USB_CID_SPR_Soft_Reset						0xEE	// 8 bit	-			Data-less write causes "soft" reset/initialization of VAC board.  DAC's are midranged. Most drives set to off.

//===========================================================
// MCI (Test only - does not use comm link)
#define	USB_CID_MCI_ReadRevisionDate1				0x00	// 16 bit	-	16 bit	Data-less 8 bit write - returns Communications Firmware Revision Code - 4 nibbles: Year (1 digit) + R.RR (2103 as of 1-8-12)
#define	USB_CID_MCI_ReadRevisionDate2				0x00	//			-	16 bit	Returns 4 nibbles: M,DD,Y (last digit only)
#define	USB_CID_MCI_Readback16wordsofABCD			0x00	// 16 bit	-	16 bit	Data-less 8 bit write results in MCI reading back 16 words of 0xABCD
#define	USB_CID_MCI_EnableMarker					0x00	// 8 bit	-			Set bit 0 to 1 to enable host readback marker generation (One 16 bit word: 0xD22D) at start of ALL board readbacks.  As of 2-5-12, Marker preceding subboard readback data is ALWAYS enabled.
#define	USB_CID_MCI_Enable_SelfReset				0x00	// 8 bit	-			Set bit 0 to 1 to enable automatic self-reset of communications circuitry in the event of a software/hardware sync problem
#define	USB_CID_MCI_Reset_Init_Board_Comm			0x00	//						Note that this command uses a different MCI opcode: "F2" that is unique to this reset function only.

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////






///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//====== PC -> Z80A MessageID ==============================

//#define Z80A_										(BYTE)0
#define Z80A_ScanFunction_SpotScan					(BYTE)1
#define Z80A_ScanFunction_LineScan					(BYTE)2
#define Z80A_ScanFunction_AreaScan					(BYTE)3
#define Z80A_ScanFunction_DualMagnification			(BYTE)4
#define Z80A_ScanFunction_SplitScan					(BYTE)5
#define Z80A_ScanFunction_RasterScan512				(BYTE)6
#define Z80A_ScanFunction_RasterScan1024			(BYTE)7
//#define Z80A_										(BYTE)8
//#define Z80A_										(BYTE)9
//#define Z80A_										(BYTE)10
#define Z80A_Inquirer								(BYTE)11
#define Z80A_DigitalPhoto							(BYTE)12
#define Z80A_AnalogPhoto							(BYTE)13
#define Z80A_HV										(BYTE)14
#define Z80A_DFocus									(BYTE)15
#define Z80A_DObjectiveLens							(BYTE)16
#define Z80A_ERotate								(BYTE)17
#define Z80A_YAdjust								(BYTE)18
#define Z80A_DCondenserLens							(BYTE)19
#define Z80A_RotationScan							(BYTE)20
#define Z80A_COMMTEST								(BYTE)21

//#define Z80A_										(BYTE)

//==========================================================


//====== PC -> Z80B MessageID ==============================

#define Z80B_ObjectiveLens							(BYTE)0
#define Z80B_CondenserLens							(BYTE)1
#define Z80B_StigmatorX								(BYTE)2
#define Z80B_StigmatorY								(BYTE)3
#define Z80B_Contrast								(BYTE)4
#define Z80B_BeamAlignmentY							(BYTE)5
#define Z80B_BeamAlignmentX							(BYTE)6
#define Z80B_Brightness								(BYTE)7
#define Z80B_Amplifier								(BYTE)8
#define Z80B_Signal2Contrast						(BYTE)9
#define Z80B_Signal2Brightness						(BYTE)10
#define Z80B_EMoveX									(BYTE)11
#define Z80B_EMoveY									(BYTE)12
#define Z80B_ScanShiftX								Z80B_EMoveX
#define Z80B_ScanShiftY								Z80B_EMoveY
#define Z80B_FilamentI								(BYTE)13
#define Z80B_FilamentV								(BYTE)14	// 3200 废止
#define Z80B_AutoObjectiveLens						(BYTE)15	// 3200 废止
#define Z80B_Inquirer								(BYTE)16
#define Z80B_AutoStigmator							(BYTE)17	// 3200 废止

// 1999-3-8 Lian Ning
//#define	Z80B_SignalConverter						(BYTE)18 // SE3 -> Pc10B
//#define	Z80B_SignalProcesser						(BYTE)19 // SE4
//#define	Z80B_AutoFilament							(BYTE)21

#define	Z80B_SignalProcesser						(BYTE)18	// 3200 废止 // SE4 -> Pc10B
#define	Z80B_AutoFilament							(BYTE)19	// 3200 废止

#define Z80B_ObjectiveLensReverse					(BYTE)20
#define Z80B_CondenserLensReverse					(BYTE)21
#define Z80B_SACP									(BYTE)22
#define Z80B_AuxiliaryStigmation					(BYTE)23
#define Z80B_RobinsonDetectorSwitch					(BYTE)24

#define Z80B_Channel_A								(BYTE)25	// 3200 废止
#define Z80B_Channel_B								(BYTE)26	// 3200 废止
#define Z80B_Channel_C								(BYTE)27	// 3200 废止
#define Z80B_Channel_D								(BYTE)28	// 3200 废止
#define Z80B_Channel_E								(BYTE)29	// 3200 废止
#define Z80B_HeatingFilamentElectricity				(BYTE)30	// 3200 废止

// 2004-1-15 Leo 3200
#define Z80B_ImageInvert							(BYTE)25	// 3200 图像信号翻转
#define Z80B_PC10_SE6								(BYTE)14	//31	// 3200 图像处理与通道选择1
#define Z80B_PC10_SE1								(BYTE)15	//32	// 3200 通道选择2

// 2007.10.17
#define	Z80B_BseBySeDetectorSwitch					(BYTE)32	//使用二次电子探测器看背散射像开关
//===========================================================
// OSE: Optial Shaft Encoder
#define Z80B_OSE_SetupParam							(BYTE)31 

typedef struct tagOSEncoderParam{
	BYTE	bFunction;
	BYTE	bStep;
	BYTE	bNotifyTimer;
	BYTE	bPulseDivier;
	WORD	nCtrlMin;
	WORD	nCtrlMax;
}OSEncoderParam;
#define OSE_Fn_NULL						(BYTE)255
#define OSE_Fn_ObjectiveLens			Z80B_ObjectiveLens
#define OSE_Fn_CondenserLens			Z80B_CondenserLens
#define OSE_Fn_StigmatorX				Z80B_StigmatorX
#define OSE_Fn_StigmatorY				Z80B_StigmatorY
#define OSE_Fn_Contrast					Z80B_Contrast
#define OSE_Fn_BeamAlignmentY			Z80B_BeamAlignmentY
#define OSE_Fn_BeamAlignmentX			Z80B_BeamAlignmentX
#define OSE_Fn_Brightness				Z80B_Brightness
#define OSE_Fn_ScanShiftX					Z80B_ScanShiftX
#define OSE_Fn_ScanShiftY					Z80B_ScanShiftY
//===========================================================

//#define Z80B_										(BYTE)

//==========================================================

//====== Z80B -> PC MessageID ==============================

#define	H_Z80B_Flag									(BYTE)16

#define		H_Z80B_Flag_HV				(BYTE)0x01
#define		H_Z80B_Flag_Filament		(BYTE)0x02
#define		H_Z80B_Enable_HV			(BYTE)0x04
//#define	H_Z80B_Flag_Vacuum			(BYTE)0x04
#define		H_Z80B_Flag_BUSY			(BYTE)0x08		//2003-11-7 Leo

//#define		H_Z80B_Flag_HOLD			(BYTE)0x10	//2003-11-7 Leo
//#define		H_Z80B_Flag_CST				(BYTE)0x20	//2003-11-7 Leo
#define		H_Z80B_Flag_CST				(BYTE)0x10		//2003-11-7 Leo
#define		H_Z80B_Flag_HOLD			(BYTE)0x20		//2003-11-7 Leo
#define		H_Z80B_Flag_Bit6			(BYTE)0x40		//2003-11-7 Leo
#define		H_Z80B_Flag_Vacuum			(BYTE)0x80
 

#define	H_Z80B_GunI									(BYTE)8
#define	H_Z80B_GunISaturated						(BYTE)13
#define	H_Z80B_AutoFocus							(BYTE)15
#define	H_Z80B_AutoStigmator						(BYTE)17

// 1999-3-8 Lian Ning
//#define	H_Z80B_SignalProcesser						(BYTE)19
//#define	H_Z80B_Reset								(BYTE)20
//#define	H_Z80B_AutoFilament							(BYTE)21

#define	H_Z80B_SignalProcesser						(BYTE)18	// SE4 -> Pc10B
#define	H_Z80B_AutoFilament							(BYTE)19

//===========================================================
// OSE: Optial Shaft Encoder
#define H_Z80B_OSE_Ctrl								(BYTE)31 // OSE: Optial Shaft Encoder
typedef struct tagOSEncoderCtrl{
	BYTE	bCtrlFunction;
	union{
		BYTE	bData;
		WORD	nData;
	}Ctrl;
}OSEncoderCtrl;
//===========================================================

#define H_Z80B_FilamentHeatingCurrent				(BYTE)32

#define	H_Z80B_Reset								(BYTE)40

/*
#define	Z80B_SignalProcesser_SE0					(BYTE)18
#define	Z80B_SignalProcesser_SE1					(BYTE)19
#define	Z80B_SignalProcesser_SE2					(BYTE)20
#define	Z80B_SignalProcesser_SE3					(BYTE)21
#define	Z80B_SignalProcesser_SE4					(BYTE)22
#define	Z80B_SignalProcesser_SE5					(BYTE)23
#define	Z80B_SignalProcesser_SE6					(BYTE)24
#define	Z80B_SignalProcesser_SE7					(BYTE)25
*/


//====== Z80A -> PC MessageID ==============================

#define	H_Z80A_Reset								(BYTE)40

//==========================================================

//====== Z80A/Z80B -> PC MessageID =========================
// 2004-5-19_处理Z80接收到未识别的消息
#define	H_Z80_UnrecognizedMsg						(BYTE)41

// 2004-5-20_处理Z80接收到校验错误的SEM消息
#define	H_Z80_CheckedErrMsg							(BYTE)42


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Cypress Semiconductor microcontroller on MCB board
// USB message ID
//===========================================================
// OSE: Optial Shaft Encoder
#define USB_OSE_SetupParam							(BYTE)31 
/*
typedef struct tagOSEncoderParam{
	BYTE	bFunction;
	BYTE	bStep;
	BYTE	bNotifyTimer;
	BYTE	bPulseDivier;
	WORD	nCtrlMin;
	WORD	nCtrlMax;
}OSEncoderParam;
#define OSE_Fn_NULL						(BYTE)255
#define OSE_Fn_ObjectiveLens			Z80B_ObjectiveLens
#define OSE_Fn_CondenserLens			Z80B_CondenserLens
#define OSE_Fn_StigmatorX				Z80B_StigmatorX
#define OSE_Fn_StigmatorY				Z80B_StigmatorY
#define OSE_Fn_Contrast					Z80B_Contrast
#define OSE_Fn_BeamAlignmentY			Z80B_BeamAlignmentY
#define OSE_Fn_BeamAlignmentX			Z80B_BeamAlignmentX
#define OSE_Fn_Brightness				Z80B_Brightness
#define OSE_Fn_ScanShiftX					Z80B_ScanShiftX
#define OSE_Fn_ScanShiftY					Z80B_ScanShiftY
//===========================================================
*/
//===========================================================
// OSE: Optial Shaft Encoder
#define H_USB_OSE_Ctrl								(BYTE)31 // OSE: Optial Shaft Encoder
/*
typedef struct tagOSEncoderCtrl{
	BYTE	bCtrlFunction;
	union{
		BYTE	bData;
		WORD	nData;
	}Ctrl;
}OSEncoderCtrl;*/
//===========================================================
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif
