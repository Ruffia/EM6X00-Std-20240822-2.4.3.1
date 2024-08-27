
#ifndef __EBM30N6_H
#define __EBM30N6_H

#include "zcoma_ex.h"

#define WM_USER_SPELLMAN_SETPARAM		(WM_USER	+51)	// Status byte from the controller is passed to user via the wParam argument
#define WM_USER_SPELLMAN_UPDATESET		(WM_USER	+52)
#define WM_USER_SPELLMAN_UPDATEMON		(WM_USER	+53)
#define WM_USER_SPELLMAN_UPDATESTR		(WM_USER	+54)

///////////////////////////////////////////////////////////////////////////////
// FEG
/////////////////////////////////////////////////////////////////////////////
/*
typedef struct tagFEGConfig{
	BOOL			bFEG;			// 是否使用场枪电源模块
	int				nState;			// 电源模块状态
	BYTE			byteEn;			// 使能状态
	BYTE			byteInterlocks;	// Interlocks状态

	TREEITEM_DATA	*tiDataRoot;
	TREEITEM_DATA	*tiDataDevice;
	TREEITEM_DATA	*tiDataChn;
	TREEITEM_DATA	*tiDataNode;
	NODE_DATA		NodeData;

	int				nPortIPG;		// IPG COM端口号
	int				nPortIPC;		// IPC COM端口号
	int				nPortCCG;		// CCG COM端口号
	DWORD			dwFilament2;	// 灯丝额定电流
	int				nEmitterType;	// 灯丝类型（编号）

	double			dAccI;			// 加速极电流
	double			dExtI;			// 吸出极电流
	double			dFilaI;			// 灯丝电流
	int				nAccV;			// 高压值（单位V）
	double			dVacIPG;		// IPG真空值
} ConfigFEG;
*/
typedef struct tagFEGcfg_Spellman{
	BOOL			bFEG;			// 是否使用场枪电源模块
	BOOL			bFound;			// 是否找到设备
	BOOL			bInit;			// 设备初始化是否成功
	BOOL			bConnect;		// 设备是否连通
	BOOL			bService;		// 设备是否已在服务状态
	BOOL			bWork;			// 设备是否已在服务状态，且可以加高压
	int				nMonType;		// 监测类型：1监测所有值，2只监测发射电流，0不监测
	DWORD			dwFilament2;	// 灯丝额定电流
	int				nExtractor;		// 自动加灯丝时的吸出极额定值
	int				nEmitterType;	// 灯丝类型（编号）
} CfgFEG_Spellman;

typedef struct tayFEGcfg_Spellman_ZLG
{
	TREEITEM_DATA	*tiDataRoot;
	TREEITEM_DATA	*tiDataDevice;
	TREEITEM_DATA	*tiDataChn;
	TREEITEM_DATA	*tiDataNode;
	NODE_DATA		NodeData;

	int				nState;			// 电源模块状态
	BYTE			byteEn;			// 使能状态
	BYTE			byteInterlocks;	// Interlocks状态
} CfgFEG_Spellman_ZLG;

typedef struct tagFEGcfg_Spellman_Set{
	// 5个设置值
	DWORD			dwAccV;
	DWORD			dwFilaI;
	DWORD			dwExtV;
	DWORD			dwSupV;
	DWORD			dwLensV;
} CfgFEG_Spellman_Set;

typedef struct tagFEGcfg_Spellman_Mon{
	// 10个监测值
	double			dAccV;
	double			dAccI;
	double			dFilaV;
	double			dFilaI;
	double			dExtV;
	double			dExtI;
	double			dSupV;
	double			dSupI;
	double			dLensV;
	double			dLensI;
	double			dIem;
} CfgFEG_Spellman_Mon;
	
// 5个控制量的满量程值和位数
const	DWORD	g_dwFS_Accelerator			= 0x000FFFFF;	// 20 bits
const	DWORD	g_dwFS_Filament				= 0x00000FFF;	// 12 bits
const	DWORD	g_dwFS_Suppressor			= 0x0000FFFF;	// 16 bits;
const	DWORD	g_dwFS_Extractor			= 0x0000FFFF;	// 16 bits;
const	DWORD	g_dwFS_Lens					= 0x0000FFFF;	// 16 bits;

const	double	g_dbFS_Accelerator			= 33792.11;
const	double	g_dbFS_Filament				= 4.096;
const	double	g_dbFS_Suppressor			= 1024.05;
const	double	g_dbFS_Extractor			= 6148.1;
const	double	g_dbFS_Lens					= 6148.1;

// 5个控制量监测回读值的满量程值和位数
const	DWORD	g_dwFS_Accelerator_mon_V	= 0x0000FFFF;	// 16 bits
const	DWORD	g_dwFS_Accelerator_mon_I	= 0x0000FFFF;	// 16 bits
const	DWORD	g_dwFS_Filament_mon_V		= 0x0000FFFF;	// 16 bits
const	DWORD	g_dwFS_Filament_mon_I		= 0x0000FFFF;	// 16 bits
const	DWORD	g_dwFS_Suppressor_mon_V		= 0x0000FFFF;	// 16 bits;
const	DWORD	g_dwFS_Suppressor_mon_I		= 0x0000FFFF;	// 16 bits;
const	DWORD	g_dwFS_Extractor_mon_V		= 0x0000FFFF;	// 16 bits;
const	DWORD	g_dwFS_Extractor_mon_I		= 0x0000FFFF;	// 16 bits;
const	DWORD	g_dwFS_Lens_mon_V			= 0x0000FFFF;	// 16 bits;
const	DWORD	g_dwFS_Lens_mon_I			= 0x0000FFFF;	// 16 bits;
const	DWORD	g_dwFS_Emission_mon_I		= 0x0000FFFF;	// 16 bits;

const	double	g_dbFS_Accelerator_mon_V	= 37500.1;
const	double	g_dbFS_Accelerator_mon_I	= 500;
const	double	g_dbFS_Filament_mon_V		= 4.096;
const	double	g_dbFS_Filament_mon_I		= 4.096;
const	double	g_dbFS_Suppressor_mon_V		= 1024.05;
const	double	g_dbFS_Suppressor_mon_I		= 163.84;
const	double	g_dbFS_Extractor_mon_V		= 6148.1;
const	double	g_dbFS_Extractor_mon_I		= 819.2;
const	double	g_dbFS_Lens_mon_V			= 6148.1;
const	double	g_dbFS_Lens_mon_I			= 409.6;
const	double	g_dbFS_Emission_mon_I		= 1035;

// 需要的index和subindex
const	DWORD	g_dwIndexError				= 0x1003;
const	DWORD	g_dwIndexStatusInfo			= 0x2000;
const	DWORD	g_dwIndexCurrentState		= 0x2007;
const	DWORD	g_dwIndexRequiredState		= 0x2008;
const	DWORD	g_dwIndexDigitalInputs		= 0x6120;
const	DWORD	g_dwIndexDigitalOutputs		= 0x6320;
const	DWORD	g_dwIndexAnalogInputs		= 0x6402;
const	BYTE	g_byteSubIndexMonAccV		= 0x01;
const	BYTE	g_byteSubIndexMonAccI		= 0x02;
const	BYTE	g_byteSubIndexMonFilaV		= 0x03;
const	BYTE	g_byteSubIndexMonFilaI		= 0x04;
const	BYTE	g_byteSubIndexMonSupV		= 0x05;
const	BYTE	g_byteSubIndexMonSupI		= 0x06;
const	BYTE	g_byteSubIndexMonExtV		= 0x07;
const	BYTE	g_byteSubIndexMonExtI		= 0x08;
const	BYTE	g_byteSubIndexMonLensV		= 0x09;
const	BYTE	g_byteSubIndexMonLensI		= 0x0A;
const	BYTE	g_byteSubIndexEmI			= 0x0B;
const	DWORD	g_dwIndexAnalogOutputs		= 0x6412;
const	BYTE	g_byteSubIndexAccelerator	= 0x01;
const	BYTE	g_byteSubIndexFilament		= 0x02;
const	BYTE	g_byteSubIndexSuppressor	= 0x03;
const	BYTE	g_byteSubIndexExtractor		= 0x04;
const	BYTE	g_byteSubIndexLens			= 0x05;
/////////////////////////////////////////////////////////////////////////////

#define	U8		1
#define	U16		2
#define	U24		3
#define	U32		4

/////////////////////////////////////////////////////////////////////////////
#define	IDEvent_Set_Accelerator		2
#define	IDEvent_Read_Monitor_Value1	3
#define	IDEvent_Read_Monitor_Value2	4
#define	IDEvent_Read_State1			5
#define	IDEvent_Read_State2			6
#define	IDEvent_Read_Error			7

#define	ID_Timer_Auto_Rise_Filament1	11
#define	ID_Timer_Auto_Rise_Filament2	12
#define	ID_Timer_Auto_Rise_Suppressor	13
#define	ID_Timer_Auto_Rise_Extractor	14
#define	ID_Timer_Auto_Rise_Lens			15
#define	ID_Timer_Auto_Rise_Accelerator	16
#define	ID_Timer_Auto_Rise_ColdWait		17
#define	ID_Timer_Auto_Fall_Filament1	21
#define	ID_Timer_Auto_Fall_Filament2	22
#define	ID_Timer_Auto_Fall_Suppressor	23
#define	ID_Timer_Auto_Fall_Extractor	24
#define	ID_Timer_Auto_Fall_Lens			25
#define	ID_Timer_Auto_Fall_Accelerator	26

#define	ID_Timer_Auto_Vacuum1			31
#define	ID_Timer_Auto_Vacuum2			32
#define	ID_Timer_Vacuum_Wait1			33
#define	ID_Timer_Vacuum_Wait2			34
#define	ID_Timer_Auto_Vacuum_VGC401		35

// YPS
#define	ID_Timer_YPSAuto_Rise_Filament1		51
#define	ID_Timer_YPSAuto_Rise_Filament2		52
#define	ID_Timer_YPSAuto_Rise_Accelerator	53
#define	ID_Timer_YPSAuto_Rise_Lens			54
#define	ID_Timer_YPSAuto_Rise_Suppressor	55
#define	ID_Timer_YPSAuto_Rise_Extractor		56
#define	ID_Timer_YPSAuto_Rise_ColdWait		57
#define	ID_Timer_YPSAuto_Fall_Filament		61
#define	ID_Timer_YPSAuto_Fall_Extractor		62
#define	ID_Timer_YPSAuto_Fall_Suppressor	63
#define	ID_Timer_YPSAuto_Fall_Lens			64
#define	ID_Timer_YPSAuto_Fall_Accelerator	65
/////////////////////////////////////////////////////////////////////////////

const	double	g_dVacuumThreshold			= 1E-08;	// Torr
const	double	g_dVacuumThresholdCCG		= 5E-05;	// Torr
/////////////////////////////////////////////////////////////////////////////
// Auto steps
const	DWORD	g_dwAutoStep_Accelerator	= 200;
const	DWORD	g_dwAutoStep_Filament1		= 10;
const	DWORD	g_dwAutoStep_Filament2		= 1;
const	DWORD	g_dwAutoStep_Suppressor		= 20;
const	DWORD	g_dwAutoStep_Extractor		= 100;
const	DWORD	g_dwAutoStep_Lens			= 20;

// Rated value
const	DWORD	g_dwAutoRated_Accelerator	= 5000;
const	DWORD	g_dwAutoRated_Filament1		= 2000;
const	DWORD	g_dwAutoRated_Filament2		= 2330;
const	DWORD	g_dwAutoRated_Suppressor	= 300;
const	DWORD	g_dwAutoRated_Extractor		= 3500;
const	DWORD	g_dwAutoRated_Extractor2	= 1500;
const	DWORD	g_dwAutoRated_Lens			= 120;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// YPS Auto steps
const	DWORD	g_dwYPSAutoStep_Accelerator1	= 200;
const	DWORD	g_dwYPSAutoStep_Accelerator2	= 500;
const	DWORD	g_dwYPSAutoStep_Filament1		= 10;
const	DWORD	g_dwYPSAutoStep_Filament2		= 1;
const	DWORD	g_dwYPSAutoStep_Suppressor1		= 10;
const	DWORD	g_dwYPSAutoStep_Suppressor2		= 20;
const	DWORD	g_dwYPSAutoStep_Extractor1		= 5;
const	DWORD	g_dwYPSAutoStep_Extractor2		= 20;
const	DWORD	g_dwYPSAutoStep_Lens1			= 20;
const	DWORD	g_dwYPSAutoStep_Lens2			= 40;

// Rated value
const	DWORD	g_dwYPSAutoRated_Accelerator	= 5000;
const	DWORD	g_dwYPSAutoRated_Filament1		= 2000;
const	DWORD	g_dwYPSAutoRated_Filament2		= 2330;
const	DWORD	g_dwYPSAutoRated_Suppressor		= 300;
const	DWORD	g_dwYPSAutoRated_Extractor		= 2000;
const	DWORD	g_dwYPSAutoRated_Lens			= 500;
/////////////////////////////////////////////////////////////////////////////

// Auto State
typedef enum tagAutoState
{
	AS_Rise_Filament1	= 0,
	AS_Rise_Filament2	= 1,
	AS_Rise_Suppressor	= 2,
	AS_Rise_Extractor	= 3,
	AS_Rise_Wait		= 4,
	AS_Rise_Lens		= 5,
	AS_Rise_Accelerator	= 6,
	AS_Fall_Filament1	= 7,
	AS_Fall_Filament2	= 8,
	AS_Fall_Suppressor	= 9,
	AS_Fall_Extractor	=10,
	AS_Fall_Lens		=11,
	AS_Fall_Accelerator	=12,
	ASS_NULL			=13,
}AutoState;

// Auto State
typedef enum tagAutoStateYPS
{
	YPSAS_Rise_Filament1	= 0,
	YPSAS_Rise_Filament2	= 1,
	YPSAS_Rise_Accelerator	= 2,
	YPSAS_Rise_Lens			= 3,
	YPSAS_Rise_Suppressor	= 4,
	YPSAS_Rise_Extractor	= 5,
	YPSAS_Rise_Wait			= 6,
	YPSAS_Fall_Filament		= 7,
	YPSAS_Fall_Extractor	= 8,
	YPSAS_Fall_Suppressor	= 9,
	YPSAS_Fall_Lens			=10,
	YPSAS_Fall_Accelerator	=11,
	YPSAS_NULL				=12,
}AutoStateYPS;

const int g_nFEG_NodeState[] = {1, 4, 5, 127, 255};
/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif