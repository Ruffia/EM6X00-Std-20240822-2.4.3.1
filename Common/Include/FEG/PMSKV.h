
#ifndef __PMSKV_H
#define __PMSKV_H

#define WM_USER_SPELLMAN_PMSKV_SETPARAM		(WM_USER	+75)	// Status byte from the controller is passed to user via the wParam argument
#define WM_USER_SPELLMAN_PMSKV_UpdateSet	(WM_USER	+76)
#define WM_USER_SPELLMAN_PMSKV_UpdateMon	(WM_USER	+77)
#define WM_USER_SPELLMAN_PMSKV_UpdateStatus	(WM_USER	+78)

///////////////////////////////////////////////////////////////////////////////
// FEG 10KV
///////////////////////////////////////////////////////////////////////////////

typedef struct tagPMSKVcfg{
	short	nPort;	// COM端口号
	BOOL	bReady;	// 端口是否工作正常

	short	nID;	// unit address
	int		nBD;	// baud rate
	short	nMode;	// <ARG> 1 = Remote, 0 = Local in ASCII format
	short	nEn;	// <ARG> 1 = HV-ON, 0 = HV-OFF in ASCII format
	float	fRangeV;// full V range
	float	fRangeI;// full I range
	float	fOutV;	// request present value of output voltage
	float	fMonV;	// voltage monitor
	float	fMonI;	// current monitor
} CfgPMSKV;
///////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// RS232
const WORD WSM30KV_SendToDevice_DataSize	= 17;
const WORD WSM30KV_ReadFromDevice_DataSize	= 513;

typedef struct tagWSM30KV_SEND_MESSAGE
{
	unsigned short	nLength;			// Avialable cData Length
	char	cData[ WSM30KV_SendToDevice_DataSize ];
} WSM30KV_Send_Msg;

typedef struct tagWSM30KV_RECV_MESSAGE
{
	unsigned short	nLength;			// Avialable bData Length
	char	cData[ WSM30KV_ReadFromDevice_DataSize ];
} WSM30KV_Recv_Msg;
/////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// SEM 30KV
///////////////////////////////////////////////////////////////////////////////

typedef struct tagWSM30KVcfg{
	short	nPort;	// COM端口号
	BOOL	bReady;	// 端口是否工作正常
} Cfg_WSM30KV;

// Auto State

typedef enum tagWSM30KV_AutoState
{
	WSM30KVAS_Rise_ACL1		= 0,
	WSM30KVAS_Rise_ACL2		= 1,
	WSM30KVAS_Rise_FIL		= 2,
	WSM30KVAS_Rise_EMS		= 3,
	WSM30KVAS_NULL			= 4,
}WSM30KV_AutoState;

typedef enum tagWSM30KV2_AutoState
{
	WSM30KV2AS_Rise_FIL		= 0,
	WSM30KV2AS_Rise_ACL1	= 1,
	WSM30KV2AS_Rise_ACL2	= 2,
	WSM30KV2AS_Rise_EMS		= 3,
	WSM30KV2AS_NULL			= 4,
}WSM30KV2_AutoState;

// 本产品符合Modbus协议，包括串行链路（RS232和RS485）上的ASCII传输模式
// 以及TCP/IP上的传输，参见《Modbus协议》，支持以下功能码：
const	BYTE	byteWSM30KV_SnBroadcast	= 0x00;	// 广播
const	BYTE	byteWSM30KV_Sn01		= 0x01;	// 从机地址 power#1
const	BYTE	byteWSM30KV_Sn25		= 0x19;	// 从机地址 power#25
const	BYTE	byteWSM30KV_FuncCodeR	= 0x03;	// 读保持寄存器
const	BYTE	byteWSM30KV_FuncCodeWRm	= 0x10;	// 写多个寄存器
const	BYTE	byteWSM30KV_FuncCodeWR	= 0x06;	// 写单个寄存器
const	BYTE	byteWSM30KV_FuncCodeWC	= 0x05;	// 写单个线圈
const	BYTE	byteWSM30KV_FuncCodeS	= 0x16;	// 屏蔽写寄存器
const	BYTE	byteWSM30KV_FuncCodeRWm	= 0x17;	// 读/写多个寄存器：实现一个读操作和一个写操作的组合

const	short	byteWSM30KV_LenReqRecv	= 7;	// 回读的数据至少7个字符
const	short	byteWSM30KV_Offset		= 1000;	// 偏移量

const	short	nWSM30KVCount_Registers	= 66;	// 总计66个寄存器

// 寄存器地址 power#1 通道
const	short	AddrWSM30KVR01_INTStatus		= 1000;	// INT状态
const	short	AddrWSM30KVR01_OutputStatus1	= 1001;	// 输出状态1
const	short	AddrWSM30KVR01_ProtectStatusB	= 1003;	// 基本保护状态
const	short	AddrWSM30KVR01_ProtectStatusV	= 1004;	// 电压保护状态
const	short	AddrWSM30KVR01_ProtectStatusI	= 1005;	// 电流保护状态
const	short	AddrWSM30KVR01_InquireStatus1	= 1040;	// 请求状态1
const	short	AddrWSM30KVR01_InquireStatus2	= 1041;	// 请求状态2
const	short	AddrWSM30KVR01_InquireStatus3	= 1042;	// 请求状态3

const	short	AddrWSM30KVR01_FB_P3V3			= 1023;	// 3.3V监测:0~54600 对应 0 到 3.3V.
const	short	AddrWSM30KVR01_FB_P5V			= 1024;	// 5V监测:	0~54600 对应 0 到 5V.
const	short	AddrWSM30KVR01_FB_P15V			= 1025;	// 15V监测:	0~54600 对应 0 到 15V.
const	short	AddrWSM30KVR01_FB_N15V			= 1026;	// -15V监测:0~54600 对应 0 到 -15V.
const	short	AddrWSM30KVR01_FB_P24V			= 1027;	// 24V监测:	0~54600 对应 0 到 24V.
const	short	AddrWSM30KVR01_FB_Temp			= 1028;	// 温度监测:0~54600 对应 0 到 250 ℃.

const	short	AddrWSM30KVR01_FB_SCI_V			= 1007;	// SCI 电压反馈: 0~54600 对应 0 到最大输出电压值 ,单位: V.
const	short	AddrWSM30KVR01_FB_SCI_I			= 1008;	// SCI 电流反馈: 0~54600 对应 0 到最大输出电流值 ,单位: mA.
const	short	AddrWSM30KVR01_FB_ACL_V			= 1009;	// ACL 电压反馈: 0~54600 对应 0 到最大输出电压值 ,单位: V.
const	short	AddrWSM30KVR01_FB_ACL_I			= 1010;	// ACL 电流反馈: 0~54600 对应 0 到最大输出电流值 ,单位: mA.
const	short	AddrWSM30KVR01_FB_COL_V			= 1015;	// COL 电压反馈: 0~54600 对应 0 到最大输出电压值 ,单位: V.
const	short	AddrWSM30KVR01_FB_COL_I			= 1016;	// COL 电流反馈: 0~54600 对应 0 到最大输出电流值 ,单位: mA.
const	short	AddrWSM30KVR01_FB_PMT_V			= 1017;	// PMT 电压反馈: 0~54600 对应 0 到最大输出电压值 ,单位: V.
const	short	AddrWSM30KVR01_FB_PMT_I			= 1018;	// PMT 电流反馈: 0~54600 对应 0 到最大输出电流值 ,单位: mA.

const	short	AddrWSM30KVR01_Given_SCI_V		= 1032;	// SCI电压给定：DAC 输出值: 0~65535 对应 0 到最大DAC 输出值, 单位: V.
const	short	AddrWSM30KVR01_Given_ACL_V		= 1033;	// ACL电压给定：DAC 输出值: 0~65535 对应 0 到最大DAC 输出值, 单位: V.
const	short	AddrWSM30KVR01_Given_ACL_I		= 1034;	// ACL电流给定：DAC 输出值: 0~65535 对应 0 到最大DAC 输出值, 单位: mA.
const	short	AddrWSM30KVR01_Given_COL_V		= 1036;	// COL电压给定：DAC 输出值: 0~65535 对应 0 到最大DAC 输出值, 单位: V.
const	short	AddrWSM30KVR01_Given_PMT_V		= 1037;	// PMT电压给定：DAC 输出值: 0~65535 对应 0 到最大DAC 输出值, 单位: V.

const	short	AddrWSM30KVR01_Given_SCI_V_M	= 1043;	// SCI电压主要给定：0~65535 对应 0 到最大电压给定值, 单位:V.
const	short	AddrWSM30KVR01_Given_SCI_V_A	= 1044;	// SCI电压辅助给定：0~255 对应负的最大到正的最大微调电压给定值,单位:V.
const	short	AddrWSM30KVR01_Given_ACL_V_M	= 1047;	// ACL电压主要给定：0~65535 对应 0 到最大电压给定值, 单位:V.
const	short	AddrWSM30KVR01_Given_ACL_V_A	= 1048;	// ACL电压辅助给定：0~255 对应负的最大到正的最大微调电压给定值,单位:V.
const	short	AddrWSM30KVR01_Given_ACL_I_M	= 1051;	// ACL电流主要给定：0~65535 对应 0 到最大电流给定值, 单位:mA.
const	short	AddrWSM30KVR01_Given_ACL_I_A	= 1052;	// ACL电流辅助给定：0~255 对应负的最大到正的最大微调电流给定值,单位:mA.
const	short	AddrWSM30KVR01_Given_COL_V_M	= 1059;	// COL电压主要给定：0~65535 对应 0 到最大电压给定值, 单位:V.
const	short	AddrWSM30KVR01_Given_COL_V_A	= 1060;	// COL电压辅助给定：0~255 对应负的最大到正的最大微调电压给定值,单位:V.
const	short	AddrWSM30KVR01_Given_PMT_V_M	= 1063;	// PMT电压主要给定：0~65535 对应 0 到最大电压给定值, 单位:V.
const	short	AddrWSM30KVR01_Given_PMT_V_A	= 1064;	// PMT电压辅助给定：0~255 对应负的最大到正的最大微调电压给定值,单位:V.

// 寄存器地址 power#25 通道
const	short	AddrWSM30KVR25_INTStatus		= 1000;	// INT状态
const	short	AddrWSM30KVR25_OutputStatus1	= 1001;	// 输出状态1
const	short	AddrWSM30KVR25_ProtectStatusB	= 1003;	// 基本保护状态
const	short	AddrWSM30KVR25_ProtectStatusV	= 1004;	// 电压保护状态
const	short	AddrWSM30KVR25_ProtectStatusI	= 1005;	// 电流保护状态
const	short	AddrWSM30KVR25_ProtectStatusArc	= 1006;	// ARC保护状态
const	short	AddrWSM30KVR25_InquireStatus1	= 1040;	// 请求状态1
const	short	AddrWSM30KVR25_InquireStatus2	= 1041;	// 请求状态2
const	short	AddrWSM30KVR25_InquireStatus3	= 1042;	// 请求状态3

const	short	AddrWSM30KVR25_FB_P3V3			= 1023;	// 3.3V监测 : 0~54600 对应 0 到 3.3V.
const	short	AddrWSM30KVR25_FB_P5V			= 1024;	// 5V监测 : 0~54600 对应 0 到 5V.
const	short	AddrWSM30KVR25_FB_P15V			= 1025;	// 15V监测 : 0~54600 对应 0 到 15V.
const	short	AddrWSM30KVR25_FB_N15V			= 1026;	// -15V监测 : 0~54600 对应0 到 -15V.
const	short	AddrWSM30KVR25_FB_P24V			= 1027;	// 24V监测 : 0~54600 对应 0 到 24V.
const	short	AddrWSM30KVR25_FB_Temp			= 1028;	// 温度监测 : 0~54600 对应 0 到 250 ℃.

const	short	AddrWSM30KVR25_FB_FIL_V			= 1009;	// FIL 电压反馈: 0~54600 对应 0 到最大输出电压值 ,单位: V.
const	short	AddrWSM30KVR25_FB_FIL_I			= 1010;	// FIL 电流反馈: 0~54600 对应 0 到最大输出电流值 ,单位: mA.
const	short	AddrWSM30KVR25_Given_FIL_I		= 1033;	// FIL主输出：DAC 输出值: 0~65535 对应 0 到最大DAC 输出值, 单位:A.
const	short	AddrWSM30KVR25_Given_FIL_I_M	= 1047;	// FIL主要给定：0~65535 对应 0到最大电流给定值, 单位:A.
const	short	AddrWSM30KVR25_Given_FIL_I_A	= 1048;	// FIL辅助给定：0~255 对应负的最大到正的最大微调电流给定值, 单位:mA.

// 线圈地址 power#1 通道
const	short	AddrWSM30KVC01_ReqOn_Power1		= 1084;	// 请求电源 SCI 高压开
const	short	AddrWSM30KVC01_ReqOn_Power2		= 1085;	// 请求电源 ACL 高压开
const	short	AddrWSM30KVC01_ReqOn_Power3		= 1086;	// 请求电源 EMS 束流开
const	short	AddrWSM30KVC01_ReqOn_Power5		= 1088;	// 请求电源 COL 高压开
const	short	AddrWSM30KVC01_ReqOn_Power6		= 1089;	// 请求电源 PMT 高压开
const	short	AddrWSM30KVC01_ReqPause_Power1	= 1092;	// 请求电源 SCI 高压 暂停动作
const	short	AddrWSM30KVC01_ReqPause_Power2	= 1093;	// 请求电源 ACL 高压 暂停动作
const	short	AddrWSM30KVC01_ReqPause_Power3	= 1094;	// 请求电源 EMS 束流 暂停动作
const	short	AddrWSM30KVC01_ReqPause_Power5	= 1096;	// 请求电源 COL 高压 暂停动作
const	short	AddrWSM30KVC01_ReqPause_Power6	= 1097;	// 请求电源 PMT 高压 暂停动作
const	short	AddrWSM30KVC01_ReqReset_Power1	= 1108;	// 请求电源 SCI 报警复位
const	short	AddrWSM30KVC01_ReqReset_Power2	= 1109;	// 请求电源 ACL 报警复位
const	short	AddrWSM30KVC01_ReqReset_Power3	= 1110;	// 请求电源 EMS 报警复位
const	short	AddrWSM30KVC01_ReqReset_Power5	= 1112;	// 请求电源 COL 报警复位
const	short	AddrWSM30KVC01_ReqReset_Power6	= 1113;	// 请求电源 PMT 报警复位
const	short	AddrWSM30KVC01_ReqReset			= 1117;	// 请求全部复位

const	short	AddrWSM30KVC25_ReqOn_Power2		= 1085;	// 请求电源 FIL 灯丝开
const	short	AddrWSM30KVC25_ReqPause_Power2	= 1093;	// 请求电源 FIL 暂停动作
const	short	AddrWSM30KVC25_ReqReset_Power2	= 1109;	// 请求电源 FIL 报警复位
const	short	AddrWSM30KVC25_ReqReset			= 1117;	// 请求全部复位

// 最大值
const	WORD	g_wWSM30KVFS_1		= 54600;
const	WORD	g_wWSM30KVFS_2		= 65535;
const	BYTE	g_bWSM30KVFS_3		= 255;

const	float	g_fWSM30KVFS_SCI_mon_V	= 10000.0;	// V
const	float	g_fWSM30KVFS_SCI_mon_I	= 250.0;	// uA
const	float	g_fWSM30KVFS_ACL_mon_V	= 30000.0;	// V
const	float	g_fWSM30KVFS_ACL_mon_I	= 400.0;	// uA
const	float	g_fWSM30KVFS_COL_mon_V	= 800.0;	// ?????
const	float	g_fWSM30KVFS_COL_mon_I	= 100.0;	// uA
const	float	g_fWSM30KVFS_PMT_mon_V	= 1300.0;	// V
const	float	g_fWSM30KVFS_PMT_mon_I	= 1.2f;		// mA
const	float	g_fWSM30KVFS_FIL_mon_V	= 5.0f;		// V
const	float	g_fWSM30KVFS_FIL_mon_I	= 4.0f;		// A


/////////////////////////////////////////////////
// INT状态（1000）位说明
typedef struct tagWSM30KVStatus_INTStatus_BitDefine{
	WORD			: 9;	// 未用
	WORD	ACL_FIL	: 1;	//
	WORD	SCI		: 1;	//
	WORD	PMT		: 1;	//
	WORD	COL		: 1;	//
	WORD			: 3;	// 未用
}StatusWSM30KV_INTStatus_BitDefine;

typedef union tagWSM30KVStatus_INTStatus
{
	WORD	wStatus;
	StatusWSM30KV_INTStatus_BitDefine	status;	
}StatusWSM30KV_INTStatus;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// 输出状态1（1001）位说明
typedef struct tagWSM30KVStatus_Output1Status_BitDefine{
	WORD	SCI		: 1;	// 00000000 0000000?：SCI -- 在SCI ON动作时,该位输出信号1.(1 使能)
	WORD	ACL_FIL	: 1;	// 00000000 000000?0：ACL/FIL -- 在ACL/FIL ON动作时,该位输出信号1.(1 使能)
	WORD	EMS		: 1;	// 00000000 00000?00：EMS -- 在EMS ON动作时,该位输出信号1.(1 使能)
	WORD			: 1;	// 00000000 0000?000：未用
	WORD	COL		: 1;	// 00000000 000?0000：COL -- 在COL ON动作时,该位输出信号1.(1 使能)
	WORD	PMT		: 1;	// 00000000 00?00000：PMT -- 在PMT ON动作时,该位输出信号1.(1 使能)
	WORD			: 10;	// ???????? ??000000：未用
}StatusWSM30KV_Output1Status_BitDefine;

typedef union tagWSM30KVStatus_Output1
{
	WORD	wStatus;
	StatusWSM30KV_Output1Status_BitDefine	status;	
}StatusWSM30KV_Output1Status;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// 基本保护状态（1003）位说明
typedef struct tagWSM30KVStatus_ProtectStatusB_BitDefine{
	WORD	P3V3	: 1;	// 0000000?：+3V3 保护(0 正常； 1 保护)
	WORD	P5V		: 1;	// 000000?0：+5V 保护(0 正常； 1 保护)
	WORD	P15V	: 1;	// 00000?00：+15V 保护(0 正常； 1 保护)
	WORD	N15V	: 1;	// 0000?000：-15V 保护(0 正常； 1 保护)
	WORD	P24V	: 1;	// 000?0000：+24V 保护(0 正常； 1 保护)
	WORD	Temp	: 1;	// 00?00000：温度1 保护(0 正常； 1 保护)
	WORD			: 10;	// 未用
}StatusWSM30KV_ProtectStatusB_BitDefine;

typedef union tagWSM30KVStatus_ProtectStatusB
{
	WORD	wStatus;
	StatusWSM30KV_ProtectStatusB_BitDefine	status;	
}StatusWSM30KV_ProtectStatusB;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// 电压保护状态（1004）位说明
typedef struct tagWSM30KVStatus_ProtectStatusV_BitDefine{
	WORD	SCI		: 1;	// 00000000 0000000?：SCI 过压保护(0 正常； 1 保护)
	WORD	ACL_FIL	: 1;	// 00000000 000000?0：ACL/FIL 过压保护(0 正常； 1 保护)
	WORD			: 2;	// 00000000 0000??00：未用
	WORD	COL		: 1;	// 00000000 000?0000：COL 过压保护(0 正常； 1 保护)
	WORD	PMT		: 1;	// 00000000 00?00000：PMT 过压保护(0 正常； 1 保护)
	WORD			: 10;	// ???????? ??000000：未用
}StatusWSM30KV_ProtectStatusV_BitDefine;

typedef union tagWSM30KVStatus_ProtectStatusV
{
	WORD	wStatus;
	StatusWSM30KV_ProtectStatusV_BitDefine	status;	
}StatusWSM30KV_ProtectStatusV;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// 电流保护状态（1005）位说明
typedef struct tagWSM30KVStatus_ProtectStatusI_BitDefine{
	WORD	SCI		: 1;	// 00000000 0000000?：SCI 过流保护(0 正常； 1 保护)
	WORD	ACL_FIL	: 1;	// 00000000 000000?0：ACL/FIL 过流保护(0 正常； 1 保护)
	WORD			: 2;	// 00000000 0000??00：未用
	WORD	COL		: 1;	// 00000000 000?0000：COL 过流保护(0 正常； 1 保护)
	WORD	PMT		: 1;	// 00000000 00?00000：PMT 过流保护(0 正常； 1 保护)
	WORD			: 10;	// ???????? ??000000：未用
}StatusWSM30KV_ProtectStatusI_BitDefine;

typedef union tagWSM30KVStatus_ProtectStatusI
{
	WORD	wStatus;
	StatusWSM30KV_ProtectStatusI_BitDefine	status;	
}StatusWSM30KV_ProtectStatusI;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// ARC保护状态（1006）位说明
typedef struct tagWSM30KVStatus_ProtectStatusArc_BitDefine{
	WORD			: 1;	// 0000000?：未用
	WORD	ACL		: 1;	// 000000?0：ACL 拉弧保护(0 正常； 1 保护)
	WORD			: 14;	// ??????00：未用
}StatusWSM30KV_ProtectStatusArc_BitDefine;

typedef union tagWSM30KVStatus_ProtectStatusArc
{
	WORD	wStatus;
	StatusWSM30KV_ProtectStatusArc_BitDefine	status;	
}StatusWSM30KV_ProtectStatusArc;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// 请求状态1（1040）位说明
typedef struct tagWSM30KVStatus_InquireStatus1_BitDefine{
	WORD	SCI		: 1;	// 0000000?：请求 SCI 高压开（1 请求）
	WORD	ACL_FIL	: 1;	// 000000?0：请求 ACL/FIL 高压开（1 请求）
	WORD	EMS		: 1;	// 00000?00：请求 EMS 高压开（1 请求）
	WORD			: 1;	// 0000?000：未用
	WORD	COL		: 1;	// 000?0000：请求 COL 高压开（1 请求）
	WORD	PMT		: 1;	// 00?00000：请求 PMT 高压开（1 请求）
	WORD			: 10;	// ??000000：未用
}StatusWSM30KV_InquireStatus1_BitDefine;

typedef union tagWSM30KVStatus_InquireStatus1
{
	WORD	wStatus;
	StatusWSM30KV_InquireStatus1_BitDefine	status;	
}StatusWSM30KV_InquireStatus1;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// 请求状态2（1041）位说明
typedef struct tagWSM30KVStatus_InquireStatus2_BitDefine{
	WORD			: 8;
	WORD	SCI		: 1;	// 0000000?：请求 SCI 复位（1 请求）
	WORD	ACL_FIL	: 1;	// 000000?0：请求 ACL/FIL 复位（1 请求）
	WORD	EMS		: 1;	// 00000?00：请求 EMS 复位（1 请求）
	WORD			: 1;	// 0000?000：未用
	WORD	COL		: 1;	// 000?0000：请求 COL 复位（1 请求）
	WORD	PMT		: 1;	// 00?00000：请求 PMT 复位（1 请求）
	WORD			: 2;	// ??000000：未用
}StatusWSM30KV_InquireStatus2_BitDefine;

typedef union tagWSM30KVStatus_InquireStatus2
{
	WORD	wStatus;
	StatusWSM30KV_InquireStatus2_BitDefine	status;	
}StatusWSM30KV_InquireStatus2;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// 请求状态3（1042）位说明
typedef struct tagWSM30KVStatus_InquireStatus3_BitDefine{
	WORD			: 1;	// 0000000?：未用
	WORD	Reset	: 1;	// 000000?0：请求全部复位（1 请求）
	WORD			: 14;	// ??????00：未用
}StatusWSM30KV_InquireStatus3_BitDefine;

typedef union tagWSM30KVStatus_InquireStatus3
{
	WORD	wStatus;
	StatusWSM30KV_InquireStatus3_BitDefine	status;	
}StatusWSM30KV_InquireStatus3;
/////////////////////////////////////////////////



typedef struct tagWSM30KVStatus{
	StatusWSM30KV_INTStatus			statusINT01;
	StatusWSM30KV_Output1Status		statusO01;
	StatusWSM30KV_ProtectStatusB	statusB01;
	StatusWSM30KV_ProtectStatusV	statusV01;
	StatusWSM30KV_ProtectStatusI	statusI01;
	StatusWSM30KV_InquireStatus1	status101;
	StatusWSM30KV_InquireStatus2	status201;
	StatusWSM30KV_InquireStatus3	status301;

	StatusWSM30KV_INTStatus			statusINT25;
	StatusWSM30KV_Output1Status		statusO25;
	StatusWSM30KV_ProtectStatusB	statusB25;
	StatusWSM30KV_ProtectStatusV	statusV25;
	StatusWSM30KV_ProtectStatusI	statusI25;
	StatusWSM30KV_ProtectStatusArc	statusA25;
	StatusWSM30KV_InquireStatus1	status125;
	StatusWSM30KV_InquireStatus2	status225;
	StatusWSM30KV_InquireStatus3	status325;
}Status_WSM30KV;

typedef struct tagWSM30KVMon_VoltTemp{
	float	fP3V3[2];
	float	fP5V[2];
	float	fP15V[2];
	float	fN15V[2];
	float	fP24V[2];
	float	fTemp[2];
}MonVT_WSM30KV;

typedef struct tagWSM30KVMon_DACOutput{
	float	fSCIv;
	float	fACLv;
	float	fACLi;
	float	fCOLv;
	float	fPMTv;
	float	fFILi;
}MonDACOutput_WSM30KV;

typedef struct tagWSM30KVMon_feedback{
	float	fSCIv;
	float	fSCIi;
	float	fACLv;
	float	fACLi;
	float	fCOLv;
	float	fCOLi;
	float	fPMTv;
	float	fPMTi;
	float	fFILv;
	float	fFILi;
}MonFeedback_WSM30KV;

typedef struct tagWSM30KVMon{
	MonVT_WSM30KV			monVT;
	MonDACOutput_WSM30KV	monOP;
	MonFeedback_WSM30KV		monFB;
}Mon_WSM30KV;

typedef struct tagWSM30KVSetting{
	float	fSCIv;
	float	fACLv;
	float	fACLi;
	float	fCOLv;
	float	fPMTv;
	float	fFILi;
}Setting_WSM30KV;
///////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////
// SEM 30KV: SEM30N6X7605（国仪使用的30KV钨丝枪高压电源）
///////////////////////////////////////////////////////////////////////////////
/*
typedef struct tagWSM30KV2cfg{
	short	nPort;	// COM端口号
	BOOL	bReady;	// 端口是否工作正常
} Cfg_WSM30KV2;

// Auto State
typedef enum tagWSM30KV2_AutoState
{
	WSM30KV2AS_Rise_ACL1	= 0,
	WSM30KV2AS_Rise_ACL2	= 1,
	WSM30KV2AS_Rise_FIL		= 2,
	WSM30KV2AS_Rise_EMS		= 3,
	WSM30KV2AS_NULL			= 4,
}WSM30KV2_AutoState;
*/
// 本产品符合Modbus协议，包括串行链路（RS232和RS485）上的ASCII传输模式
// 以及TCP/IP上的传输，参见《Modbus协议》，支持以下功能码：
const	BYTE	byteWSM30KV2_SnBroadcast	= 0x00;	// 广播
const	BYTE	byteWSM30KV2_Sn				= 0x01;	// 从机地址 power#1
const	BYTE	byteWSM30KV2_FuncCodeR		= 0x03;	// 读保持寄存器
const	BYTE	byteWSM30KV2_FuncCodeWRm	= 0x10;	// 写多个寄存器
const	BYTE	byteWSM30KV2_FuncCodeWR		= 0x06;	// 写单个寄存器
const	BYTE	byteWSM30KV2_FuncCodeWC		= 0x05;	// 写单个线圈
const	BYTE	byteWSM30KV2_FuncCodeS		= 0x16;	// 屏蔽写寄存器
const	BYTE	byteWSM30KV2_FuncCodeRWm	= 0x17;	// 读/写多个寄存器：实现一个读操作和一个写操作的组合

const	short	byteWSM30KV2_LenReqRecv		= 7;	// 回读的数据至少7个字符
const	short	byteWSM30KV2_Offset			= 1000;	// 偏移量

const	short	nWSM30KV2Count_Registers	= 68;	// 总计68个寄存器

// 寄存器地址 power#1 通道
//const	short	AddrWSM30KV2R_INTStatus			= 1000;	// INT状态
const	short	AddrWSM30KV2R_OutputStatus1		= 1001;	// 输出状态1
const	short	AddrWSM30KV2R_ProtectStatusB	= 1003;	// 基本保护状态
const	short	AddrWSM30KV2R_ProtectStatusV	= 1004;	// 电压保护状态
const	short	AddrWSM30KV2R_ProtectStatusI	= 1005;	// 电流保护状态
const	short	AddrWSM30KV2R_ProtectStatusA	= 1006;	// ARC保护状态
const	short	AddrWSM30KV2R_InquireStatus1	= 1040;	// 请求状态1
const	short	AddrWSM30KV2R_InquireStatus2	= 1041;	// 请求状态2
const	short	AddrWSM30KV2R_InquireStatus3	= 1042;	// 请求状态3

const	short	AddrWSM30KV2R_FB_P3V3			= 1023;	// 3.3V监测:0~54600 对应 0 到 3.3V.
const	short	AddrWSM30KV2R_FB_P5V			= 1024;	// 5V监测:	0~54600 对应 0 到 5V.
const	short	AddrWSM30KV2R_FB_P15V			= 1025;	// 15V监测:	0~54600 对应 0 到 15V.
const	short	AddrWSM30KV2R_FB_N15V			= 1026;	// -15V监测:0~54600 对应 0 到 -15V.
const	short	AddrWSM30KV2R_FB_P24V			= 1027;	// 24V监测:	0~54600 对应 0 到 24V.
const	short	AddrWSM30KV2R_FB_Temp			= 1028;	// 温度监测:0~54600 对应 0 到 250 ℃.

const	short	AddrWSM30KV2R_FB_BIAS_V			= 1007;	// BIAS 电压反馈: 0~54600 对应 0 到 -2000V 输出电压值 ,单位: V.
const	short	AddrWSM30KV2R_FB_FIL_I			= 1010;	// FIL 电流反馈: 0~54600 对应 0 到 3A 输出电流值 ,单位: mA.
const	short	AddrWSM30KV2R_FB_ACL_V			= 1011;	// ACL 电压反馈: 0~54600 对应 0 到 -30000V 输出电压值 ,单位: V.
const	short	AddrWSM30KV2R_FB_ACL_I			= 1012;	// ACL 电流（束流）反馈: 0~54600 对应 0 到 240uA 输出电流值 ,单位: uA.
const	short	AddrWSM30KV2R_FB_SCI_V			= 1013;	// SCI 电压反馈: 0~54600 对应 0 到 10000V 输出电流值 ,单位: V.
const	short	AddrWSM30KV2R_FB_SCI_I			= 1014;	// SCI 电流反馈: 0~54600 对应 0 到 250uA 输出电流值 ,单位: uA.
const	short	AddrWSM30KV2R_FB_PMT_V			= 1015;	// PMT 电压反馈: 0~54600 对应 0 到 -1300V 输出电压值 ,单位: V.
const	short	AddrWSM30KV2R_FB_PMT_I			= 1016;	// PMT 电流反馈: 0~54600 对应 0 到 1.2mA 输出电流值 ,单位: mA.
const	short	AddrWSM30KV2R_FB_COL_V			= 1017;	// COL 电压反馈: 0~54600 对应 0 到 400V 输出电压值 ,单位: V.
const	short	AddrWSM30KV2R_FB_COL_I			= 1018;	// COL 电流反馈: 0~54600 对应 0 到 5mA 输出电流值 ,单位: mA.

const	short	AddrWSM30KV2R_Given_BIAS_V		= 1043;	// BIAS电压主要给定：0~65535 对应 0 到 -2000V 电压给定值,单位:V.
const	short	AddrWSM30KV2R_Given_FIL_I		= 1047;	// FIL电流主要给定：0~65535 对应 0 到 3A 电流给定值,单位:A.
const	short	AddrWSM30KV2R_Given_ACL_V		= 1051;	// ACL电压主要给定：0~65535 对应 0 到 -30000V 电压给定值, 单位:V.
const	short	AddrWSM30KV2R_Given_SCI_V		= 1055;	// SCI电压主要给定：0~65535 对应 0 到 10000V 电压给定值, 单位:V.
const	short	AddrWSM30KV2R_Given_ACL_I		= 1059;	// ACL电流（束流）主要给定：0~65535 对应 0 到 240uA 电流给定值, 单位:mA.
const	short	AddrWSM30KV2R_Given_COL_V		= 1063;	// COL电压主要给定：0~65535 对应 0 到 400V 电压给定值, 单位:V.
const	short	AddrWSM30KV2R_Given_PMT_V		= 1067;	// PMT电压主要给定：0~65535 对应 0 到 -1300V 电压给定值, 单位:V.

// 线圈地址 power#1 通道
const	short	AddrWSM30KV2C_ReqOn_Power1		= 1084;	// 请求电源 BIAS 高压开
const	short	AddrWSM30KV2C_ReqOn_Power2		= 1085;	// 请求电源 FIL 灯丝开
const	short	AddrWSM30KV2C_ReqOn_Power3		= 1086;	// 请求电源 ACL 高压开
const	short	AddrWSM30KV2C_ReqOn_Power4		= 1087;	// 请求电源 SCI 高压开
const	short	AddrWSM30KV2C_ReqOn_Power5		= 1088;	// 请求电源 EMS(ACL束流)开
const	short	AddrWSM30KV2C_ReqOn_Power6		= 1089;	// 请求电源 COL 高压开
const	short	AddrWSM30KV2C_ReqOn_Power7		= 1090;	// 请求电源 PMT 高压开
const	short	AddrWSM30KV2C_ReqPause_Power1	= 1092;	// 请求电源 BIAS 高压 暂停动作
const	short	AddrWSM30KV2C_ReqPause_Power2	= 1093;	// 请求电源 FIL 灯丝 暂停动作
const	short	AddrWSM30KV2C_ReqPause_Power3	= 1094;	// 请求电源 ACL 高压 暂停动作
const	short	AddrWSM30KV2C_ReqPause_Power4	= 1095;	// 请求电源 SCI 高压 暂停动作
const	short	AddrWSM30KV2C_ReqPause_Power5	= 1096;	// 请求电源 EMS(ACL束流) 暂停动作
const	short	AddrWSM30KV2C_ReqPause_Power6	= 1097;	// 请求电源 COL 高压 暂停动作
const	short	AddrWSM30KV2C_ReqPause_Power7	= 1098;	// 请求电源 PMT 高压 暂停动作
const	short	AddrWSM30KV2C_ReqReset_Power1	= 1108;	// 请求电源 BIAS 报警复位
const	short	AddrWSM30KV2C_ReqReset_Power2	= 1109;	// 请求电源 FIL 报警复位
const	short	AddrWSM30KV2C_ReqReset_Power3	= 1110;	// 请求电源 ACL 报警复位
const	short	AddrWSM30KV2C_ReqReset_Power4	= 1111;	// 请求电源 SCI 报警复位
const	short	AddrWSM30KV2C_ReqReset_Power5	= 1112;	// 请求电源 EMS(ACL束流) 报警复位
const	short	AddrWSM30KV2C_ReqReset_Power6	= 1113;	// 请求电源 COL 报警复位
const	short	AddrWSM30KV2C_ReqReset_Power7	= 1114;	// 请求电源 PMT 报警复位
const	short	AddrWSM30KV2C_ReqReset			= 1117;	// 请求全部复位

// 最大值
const	WORD	g_wWSM30KV2FS_1		= 54600;
const	WORD	g_wWSM30KV2FS_2		= 65535;

const	float	g_fWSM30KV2FS_BIAS_mon_V= 2000.0;	// V
const	float	g_fWSM30KV2FS_FIL_mon_I	= 3.0f;		// A
const	float	g_fWSM30KV2FS_ACL_mon_V	= 30000.0;	// V
const	float	g_fWSM30KV2FS_ACL_mon_I	= 240.0;	// uA
const	float	g_fWSM30KV2FS_SCI_mon_V	= 10000.0;	// V
const	float	g_fWSM30KV2FS_SCI_mon_I	= 250.0;	// uA
const	float	g_fWSM30KV2FS_PMT_mon_V	= 1300.0;	// V
const	float	g_fWSM30KV2FS_PMT_mon_I	= 1.2f;		// mA
const	float	g_fWSM30KV2FS_COL_mon_V	= 400.0;	// V
const	float	g_fWSM30KV2FS_COL_mon_I	= 5.0;		// mA

/*
/////////////////////////////////////////////////
// INT状态（1000）位说明
typedef struct tagWSM30KV2Status_INTStatus_BitDefine{
	WORD			: 9;	// 未用
	WORD	ACL_FIL	: 1;	//
	WORD	SCI		: 1;	//
	WORD	PMT		: 1;	//
	WORD	COL		: 1;	//
	WORD			: 3;	// 未用
}StatusWSM30KV2_INTStatus_BitDefine;

typedef union tagWSM30KV2Status_INTStatus
{
	WORD	wStatus;
	StatusWSM30KV2_INTStatus_BitDefine	status;	
}StatusWSM30KV2_INTStatus;
/////////////////////////////////////////////////
*/
/////////////////////////////////////////////////
// 输出状态1（1001）位说明
typedef struct tagWSM30KV2Status_Output1Status_BitDefine{
	WORD	BIAS	: 1;	// 00000000 0000000?：BIAS -- 在BIAS ON动作时,该位输出信号1.(1 使能)
	WORD	FIL		: 1;	// 00000000 000000?0：FIL -- 在FIL ON动作时,该位输出信号1.(1 使能)
	WORD	ACL		: 1;	// 00000000 00000?00：ACL -- 在ACL ON动作时,该位输出信号1.(1 使能)
	WORD	SCI		: 1;	// 00000000 0000?000：SCI -- 在SCI ON动作时,该位输出信号1.(1 使能)
	WORD	EMS		: 1;	// 00000000 000?0000：EMS -- 在EMS ON(ACL束流开关)动作时,该位输出信号1.(1 使能)
	WORD	COL		: 1;	// 00000000 00?00000：COL -- 在COL ON动作时,该位输出信号1.(1 使能)
	WORD	PMT		: 1;	// 00000000 0?000000：PMT -- 在PMT ON动作时,该位输出信号1.(1 使能)
	WORD			: 1;	// 00000000 ?0000000：未用
	WORD	INTLK	: 1;	// 0000000? 00000000：INTLK -- 该位输入信号为1，互锁闭合
	WORD			: 7;	// ???????0 00000000：未用
}StatusWSM30KV2_Output1Status_BitDefine;

typedef union tagWSM30KV2Status_Output1
{
	WORD	wStatus;
	StatusWSM30KV2_Output1Status_BitDefine	status;	
}StatusWSM30KV2_Output1Status;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// 基本保护状态（1003）位说明
typedef struct tagWSM30KV2Status_ProtectStatusB_BitDefine{
	WORD	P3V3	: 1;	// 0000000?：+3V3 保护(0 正常； 1 保护)
	WORD	P5V		: 1;	// 000000?0：+5V 保护(0 正常； 1 保护)
	WORD	P15V	: 1;	// 00000?00：+15V 保护(0 正常； 1 保护)
	WORD	N15V	: 1;	// 0000?000：-15V 保护(0 正常； 1 保护)
	WORD	P24V	: 1;	// 000?0000：+24V 保护(0 正常； 1 保护)
	WORD			: 1;	// 00?00000：未用
	WORD	Temp	: 1;	// 0?000000：温度 保护(0 正常； 1 保护)
	WORD			: 9;	// 未用
}StatusWSM30KV2_ProtectStatusB_BitDefine;

typedef union tagWSM30KV2Status_ProtectStatusB
{
	WORD	wStatus;
	StatusWSM30KV2_ProtectStatusB_BitDefine	status;	
}StatusWSM30KV2_ProtectStatusB;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// 电压保护状态（1004）位说明
typedef struct tagWSM30KV2Status_ProtectStatusV_BitDefine{
	WORD	BIAS	: 1;	// 00000000 0000000?：BIAS 过压保护(0 正常； 1 保护)
	WORD			: 1;	// 00000000 000000?0：未用
	WORD	ACL		: 1;	// 00000000 00000?00：ACL 过压保护(0 正常； 1 保护)
	WORD	SCI		: 1;	// 00000000 0000?000：SCI 过压保护(0 正常； 1 保护)
	WORD	PMT		: 1;	// 00000000 000?0000：PMT 过压保护(0 正常； 1 保护)
	WORD	COL		: 1;	// 00000000 00?00000：COL 过压保护(0 正常； 1 保护)
	WORD			: 10;	// ???????? ??000000：未用
}StatusWSM30KV2_ProtectStatusV_BitDefine;

typedef union tagWSM30KV2Status_ProtectStatusV
{
	WORD	wStatus;
	StatusWSM30KV2_ProtectStatusV_BitDefine	status;	
}StatusWSM30KV2_ProtectStatusV;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// 电流保护状态（1005）位说明
typedef struct tagWSM30KV2Status_ProtectStatusI_BitDefine{
	WORD			: 1;	// 00000000 0000000?：未用
	WORD	FIL		: 1;	// 00000000 000000?0：FIL 过流保护(0 正常； 1 保护)
	WORD	ACL		: 1;	// 00000000 00000?00：ACL 过流保护(0 正常； 1 保护)
	WORD	SCI		: 1;	// 00000000 0000?000：SCI 过流保护(0 正常； 1 保护)
	WORD	PMT		: 1;	// 00000000 000?0000：PMT 过流保护(0 正常； 1 保护)
	WORD	COL		: 1;	// 00000000 00?00000：COL 过流保护(0 正常； 1 保护)
	WORD			: 10;	// ???????? ??000000：未用
}StatusWSM30KV2_ProtectStatusI_BitDefine;

typedef union tagWSM30KV2Status_ProtectStatusI
{
	WORD	wStatus;
	StatusWSM30KV2_ProtectStatusI_BitDefine	status;	
}StatusWSM30KV2_ProtectStatusI;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// ARC保护状态（1006）位说明
typedef struct tagWSM30KV2Status_ProtectStatusArc_BitDefine{
	WORD			: 2;	// 000000??：未用
	WORD	ACL		: 1;	// 00000?00：ACL 拉弧保护(0 正常； 1 保护)
	WORD			: 13;	// ?????000：未用
}StatusWSM30KV2_ProtectStatusArc_BitDefine;

typedef union tagWSM30KV2Status_ProtectStatusArc
{
	WORD	wStatus;
	StatusWSM30KV2_ProtectStatusArc_BitDefine	status;	
}StatusWSM30KV2_ProtectStatusArc;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// 请求状态1（1040）位说明
typedef struct tagWSM30KV2Status_InquireStatus1_BitDefine{
	WORD	ON_BIAS		: 1;	// 00000000 0000000?：请求 BIAS 高压开（1 请求）
	WORD	ON_FIL		: 1;	// 00000000 000000?0：请求 FIL 灯丝开（1 请求）
	WORD	ON_ACL		: 1;	// 00000000 00000?00：请求 ACL 高压开（1 请求）
	WORD	ON_SCI		: 1;	// 00000000 0000?000：请求 SCI 高压开（1 请求）
	WORD	ON_EMS		: 1;	// 00000000 000?0000：请求 EMS(ACL束流)开（1 请求）
	WORD	ON_COL		: 1;	// 00000000 00?00000：请求 COL 高压开（1 请求）
	WORD	ON_PMT		: 1;	// 00000000 0?000000：请求 PMT 高压开（1 请求）
	WORD				: 1;	// 00000000 ?0000000：未用
	WORD	PAUSE_BIAS	: 1;	// 0000000? 00000000：请求 BIAS 暂停动作（1 请求）
	WORD	PAUSE_FIL	: 1;	// 000000?0 00000000：请求 FIL 暂停动作（1 请求）
	WORD	PAUSE_ACL	: 1;	// 00000?00 00000000：请求 ACL 暂停动作（1 请求）
	WORD	PAUSE_SCI	: 1;	// 0000?000 00000000：请求 SCI 暂停动作（1 请求）
	WORD	PAUSE_EMS	: 1;	// 000?0000 00000000：请求 EMS(ACL束流)暂停动作（1 请求）
	WORD	PAUSE_COL	: 1;	// 00?00000 00000000：请求 COL 暂停动作（1 请求）
	WORD	PAUSE_PMT	: 1;	// 0?000000 00000000：请求 PMT 暂停动作（1 请求）
	WORD				: 1;	// ?0000000 00000000：未用
}StatusWSM30KV2_InquireStatus1_BitDefine;

typedef union tagWSM30KV2Status_InquireStatus1
{
	WORD	wStatus;
	StatusWSM30KV2_InquireStatus1_BitDefine	status;	
}StatusWSM30KV2_InquireStatus1;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// 请求状态2（1041）位说明
typedef struct tagWSM30KV2Status_InquireStatus2_BitDefine{
	WORD			: 8;
	WORD	BIAS	: 1;	// 0000000?：请求 BIAS 复位（1 请求）
	WORD	FIL		: 1;	// 000000?0：请求 FIL 复位（1 请求）
	WORD	ACL		: 1;	// 00000?00：请求 ACL 复位（1 请求）
	WORD	SCI		: 1;	// 0000?000：请求 SCI 复位（1 请求）
	WORD	EMS		: 1;	// 000?0000：请求 EMS(ACL束流) 复位（1 请求）
	WORD	COL		: 1;	// 00?00000：请求 COL 复位（1 请求）
	WORD	PMT		: 1;	// 0?000000：请求 PMT 复位（1 请求）
	WORD			: 1;	// ?0000000：未用
}StatusWSM30KV2_InquireStatus2_BitDefine;

typedef union tagWSM30KV2Status_InquireStatus2
{
	WORD	wStatus;
	StatusWSM30KV2_InquireStatus2_BitDefine	status;	
}StatusWSM30KV2_InquireStatus2;
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// 请求状态3（1042）位说明
typedef struct tagWSM30KV2Status_InquireStatus3_BitDefine{
	WORD			: 1;	// 0000000?：未用
	WORD	Reset	: 1;	// 000000?0：请求全部复位（1 请求）
	WORD			: 14;	// ??????00：未用
}StatusWSM30KV2_InquireStatus3_BitDefine;

typedef union tagWSM30KV2Status_InquireStatus3
{
	WORD	wStatus;
	StatusWSM30KV2_InquireStatus3_BitDefine	status;	
}StatusWSM30KV2_InquireStatus3;
/////////////////////////////////////////////////



typedef struct tagWSM30KV2Status{
	StatusWSM30KV2_Output1Status	statusO;
	StatusWSM30KV2_ProtectStatusB	statusB;
	StatusWSM30KV2_ProtectStatusV	statusV;
	StatusWSM30KV2_ProtectStatusI	statusI;
	StatusWSM30KV2_ProtectStatusArc	statusA;
	StatusWSM30KV2_InquireStatus1	status1;
	StatusWSM30KV2_InquireStatus2	status2;
	StatusWSM30KV2_InquireStatus3	status3;
}Status_WSM30KV2;

typedef struct tagWSM30KV2Mon_VoltTemp{
	float	fP3V3;
	float	fP5V;
	float	fP15V;
	float	fN15V;
	float	fP24V;
	float	fTemp;
}MonVT_WSM30KV2;

typedef struct tagWSM30KV2Mon_DACOutput{
	float	fBIASv;
	float	fFILi;
	float	fACLv;
	float	fSCIv;
	float	fACLi;
	float	fCOLv;
	float	fPMTv;
}MonDACOutput_WSM30KV2;

typedef struct tagWSM30KV2Mon_feedback{
	float	fBIASv;
	float	fFILi;
	float	fACLv;
	float	fACLi;
	float	fSCIv;
	float	fSCIi;
	float	fPMTv;
	float	fPMTi;
	float	fCOLv;
	float	fCOLi;
}MonFeedback_WSM30KV2;

typedef struct tagWSM30KV2Mon{
	MonVT_WSM30KV2			monVT;
	MonDACOutput_WSM30KV2	monOP;
	MonFeedback_WSM30KV2	monFB;
}Mon_WSM30KV2;

typedef struct tagWSM30KV2Setting{
	float	fBIASv;
	float	fFILi;
	float	fACLv;
	float	fSCIv;
	float	fACLi;
	float	fCOLv;
	float	fPMTv;
}Setting_WSM30KV2;
///////////////////////////////////////////////////////////////////////////////


#endif