
#ifndef __PLCvacBrd_H
#define __PLCvacBrd_H

#define WM_USER_PLCvacBrd_SendMsg		(WM_USER	+110)
#define WM_USER_PLCvacBrd_RecvMsg		(WM_USER	+111)
#define WM_USER_PLCvacBrd_UpdateStatus	(WM_USER	+112)

#define PLCvacBoard_PortPLC		2000
#define PLCvacBoard_PortPC		5000

typedef struct tagCfg_PLCvacBrd{
	double			dThreshold_PreVac;	// 阈值1
	double			dThreshold_VacOK;	// 阈值2
	double			dThreshold_VacNotOK;// 阈值3

	BOOL			bCreate;	// socket创建
	BOOL			bConnect;	// socket连接
	BOOL			bReady;		// 通讯测试正常
} Cfg_PLCvacBrd;


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

const	BYTE	byte_PLCvacBRDHead			= 0x25;
const	BYTE	byte_PLCvacBRDTail			= 0x0D;
const	BYTE	byte_PLCvacBRDLenSend1		= 0x04;	// 各命令
const	BYTE	byte_PLCvacBRDLenSend2		= 0x06;	// 阈值
const	BYTE	byte_PLCvacBRDLenRecv1		= 0x04;	// 各命令
const	BYTE	byte_PLCvacBRDLenRecv2		= 0x09;	// 阈值

const	BYTE	byte_PLCvacBoard_Offset		= 0x20;

/////////////////////////////////////////////////////////////////////
// 1. PLC与PC间通讯协议帧格式
//	帧头Byte1	命令名称Byte2	操作数Byte3 ~ Byte(N-1)		帧尾Byte N
//	25H			opcXXX			XX...XX						0DH

// 2. PC下送PLC的命令名称、对应值和操作数
const	BYTE	byte_PLCvacBRDopr_Default				= 0x01;		// oprand 默认值
const	BYTE	byte_PLCvacBRDopr_Open					= 0x01;		// oprand = 0x01：开启
const	BYTE	byte_PLCvacBRDopr_Close					= 0x02;		// oprand = 0x02：关闭

const	BYTE	byte_PLCvacBRDopc_SwitchMode			= 0x01;		// 切换手动/自动模式
		const	BYTE	byte_PLCvacBRDopr_ModeManual	= 0x01;		// oprand = 0x01：手动
		const	BYTE	byte_PLCvacBRDopr_ModeAuto		= 0x02;		// oprand = 0x02：自动（默认）

const	BYTE	byte_PLCvacBRDopc_PumpVent_SEM			= 0x02;		// 钨丝枪抽气/放气
		const	BYTE	byte_PLCvacBRDopr_SEMpvPump		= 0x01;		// oprand = 0x01：抽气
		const	BYTE	byte_PLCvacBRDopr_SEMpvVentChamb= 0x02;		// oprand = 0x02：样品室放气
		const	BYTE	byte_PLCvacBRDopr_SEMpvVentGun	= 0x03;		// oprand = 0x03：枪室放气
		const	BYTE	byte_PLCvacBRDopr_SEMpvSwitch	= 0x06;		// oprand = 0x06：切换钨灯丝
		const	BYTE	byte_PLCvacBRDopr_SEMpvStandby	= 0x07;		// oprand = 0x07：待机

const	BYTE	byte_PLCvacBRDopc_GaugeType				= 0x03;		// 真空计类型
		const	BYTE	byte_PLCvacBRDopr_GaugeInficon	= 0x01;		// oprand = 0x01：INFICON MPG400
		const	BYTE	byte_PLCvacBRDopr_GaugeWPC		= 0x02;		// oprand = 0x02：宜准 WPC400

const	BYTE	byte_PLCvacBRDopc_PumpVent_FEM			= 0x04;		// 场枪抽气/放气
		const	BYTE	byte_PLCvacBRDopr_FEMpvPump		= 0x01;		// oprand = 0x01：抽气
		const	BYTE	byte_PLCvacBRDopr_FEMpvVent		= 0x02;		// oprand = 0x02：样品室放气
		const	BYTE	byte_PLCvacBRDopr_FEMpvStandby	= 0x07;		// oprand = 0x07：待机
		const	BYTE	byte_PLCvacBRDopr_FEMpvSwitch	= 0x08;		// oprand = 0x08：切换场枪

const	BYTE	byte_PLCvacBRDopc_V1					= 0x05;		// 自动模式开关V1阀
const	BYTE	byte_PLCvacBRDopc_CCD					= 0x06;		// 开关CCD电源

const	BYTE	byte_PLCvacBRDopc_SetThreshold			= 0x07;		// 设置阈值（用整数乘以响应的倍数换算）
		const	BYTE	byte_PLCvacBRDopr_VacPre		= 0x01;		// oprand = 0x01 XXXX：PreVac	*1E+3
		const	BYTE	byte_PLCvacBRDopr_VacOK			= 0x02;		// oprand = 0x02 XXXX：VacOK	*1E+7
		const	BYTE	byte_PLCvacBRDopr_VacNotOK		= 0x03;		// oprand = 0x03 XXXX：VacNotOK	*1E+7

const	BYTE	byte_PLCvacBRDopc_SetVentInterval		= 0x08;		// 设置放气阀间隔时间（备用）
const	BYTE	byte_PLCvacBRDopc_WarnReset				= 0x09;		// 报警复位命令：oprand = 0x01：报警复位

// 手动模式
const	BYTE	byte_PLCvacBRDopc_Manual_V1				= 0x11;		// 手动模式开关V1阀
const	BYTE	byte_PLCvacBRDopc_Manual_V2				= 0x12;		// 手动模式开关V2阀
const	BYTE	byte_PLCvacBRDopc_Manual_V3				= 0x13;		// 手动模式开关V3阀
const	BYTE	byte_PLCvacBRDopc_Manual_V4				= 0x14;		// 手动模式开关V4阀
const	BYTE	byte_PLCvacBRDopc_Manual_TMP			= 0x15;		// 手动模式开关TMP
const	BYTE	byte_PLCvacBRDopc_Manual_MP				= 0x16;		// 手动模式开关MP

const	BYTE	byte_PLCvacBRDopc_NAV					= 0x19;		// 请求允许开关NAV
const	BYTE	byte_PLCvacBRDopc_BSE					= 0x1D;		// 请求BSE：oprand = 0x01：移入；oprand = 0x02：移出
const	BYTE	byte_PLCvacBRDopc_BeamDetect			= 0x1F;		// 请求开关束流检测命令

// 获取
const	BYTE	byte_PLCvacBRDopc_InquireVACstatus		= 0x21;	// 获取真空状态
const	BYTE	byte_PLCvacBRDopc_InquireVACvalve		= 0x22;	// 获取阀门状态
const	BYTE	byte_PLCvacBRDopc_InquireVACthreshold	= 0x27;	// 获取当前阈值

const	BYTE	byte_PLCvacBRDopc_InquireCCG			= 0x31;	// 获取CCG值
const	BYTE	byte_PLCvacBRDopc_InquireTMPr			= 0x32;	// 获取分子泵转速rotational speed
const	BYTE	byte_PLCvacBRDopc_InquireTMPd			= 0x33;	// 获取分子泵温度degrees
const	BYTE	byte_PLCvacBRDopc_InquireTMPt			= 0x34;	// 获取分子泵运行时长time

const	BYTE	byte_PLCvacBRDopc_InquireStatus			= 0x51;	// 获取所有状态值


/////////////////////////////////////////////////////////////////////
// 3. PLC发送PC的响应

// 切换手/自动模式命令			0x01	0x0X：收到命令，X值同接收
// 6000样品室抽真空/放气命令	0x02	0x0X：收到命令，X值同接收
// Standby						0x03	备用
// 8000抽真空/放气命令			0x04	0x0X：收到命令，X值同接收
// 开关V1阀命令					0x05	0x0X：收到命令，X值同接收
// 开关CCD电源命令				0x06	0x0X：收到命令，X值同接收
// 设置阈值命令					0x07	0x0X：收到命令，X值同接收
// 设置放气阀间隔时间命令		0x08	备用
// 报警复位命令					0x09	0x0X：收到命令，X值同接收
// 手动模式开关V1阀命令			0x11	0x0X：收到命令，X值同接收
// 手动模式开关V2阀命令			0x12	0x0X：收到命令，X值同接收
// 手动模式开关V3阀命令			0x13	0x0X：收到命令，X值同接收
// 手动模式开关V4阀命令			0x14	0x0X：收到命令，X值同接收
// 手动模式开关TMP命令			0x15	0x0X：收到命令，X值同接收
// 获取真空状态命令				0x21	0xXX XX：发送真空状态
// 获取阀门状态命令				0x22	0xXX XX：发送阀门状态
// 获取当前阈值命令（Unit：Torr）
// （用整数除以相应的倍数换算）	0x27	0xXXXX XXXX XXXX：发送下面阈值
//										PreVac    /1E+3
//										VacOK     /1E+7
//										VacNotOK  /1E+7
// 获取CCG值命令				0x31	0xXX XX：发送CCG值
// 获取分子泵转速命令			0x32	0xXX XX：发送分子泵转速值
// 获取分子泵温度命令			0x33	0xXX XX：发送分子泵温度值
// 获取分子泵运行时间命令		0x34	0xXX XX：发送分子泵运行时间值
// 获取总状态命令				0x51	0xXX…XX：发送总状态值


/////////////////////////////////////////////////////////////////////
// 4. PLC发送PC的响应帧命令名称、对应值和参数
// const	BYTE	byte_PLCvacBRDopc_InquireVACstatus		= 0x21;	// 返回真空状态和其它状态
//				参数1：0xXX（自动流程状态）
//						0 手动；1 自动停止；2 粗抽状态；3 粗真空；4 低真空
//						5 TMP运行启动；6 TMP达正常转速；7 TMP运行VacOK
//						10 准备放气；11 慢速放气；12 快速放气
//						19 待机状态；20 真空运行错误
//				参数2：0b ABCD EFGH （1为有报警，0为无报警）
//						A：保留
//						B：CCG故障；C：MP错误；D：TMP连接；E：TMP Fail；F：TMP错误
//						G：真空错误；H：样品室真空达标

// const	BYTE	byte_PLCvacBRDopc_InquireVACvalve		= 0x22;	// 返回阀门状态
//				参数1：0b ABCD EFGH （1为开，0为关）
//						A：CCD；B：TMP；C：CCG；D：V5；E：V4；F：V3；G：V2；H：V1
//						G：真空错误；H：样品室真空达标
//				参数2：0b ABCD EFGH （1为开，0为关）
//						A、B、C、D、E、F：保留
//						G：钨丝枪V1阀状态
//						H：1为自动模式；0为手动模式


// const	BYTE	byte_PLCvacBRDopc_InquireCCG			= 0x31;	// 返回CCG值
//				参数1及2（均为0xXX）对应16位ADC值，需转换为真空值（单位Torr）
// U = (ADC/27648.0) *10.0	// ?????
// P = 10 ^ (1.667 *U -d)	// d = 11.46
// valid in the range: 3.8E-9 Torr < P < 750 Torr
// Measurement range: 1.82 ... 8.6V

// const	BYTE	byte_PLCvacBRDopc_InquireTMPr			= 0x32;	// 获取分子泵转速rotational speed
// const	BYTE	byte_PLCvacBRDopc_InquireTMPd			= 0x33;	// 获取分子泵温度degrees
// const	BYTE	byte_PLCvacBRDopc_InquireTMPt			= 0x34;	// 获取分子泵运行时长time
//				参数1及2（均为0xXX）对应16位ADC值，需转换为转速（r/S）、温度（摄氏度）、运行时间（小时）

// const	BYTE	byte_PLCvacBRDopc_InquireStatus			= 0x51;	// 获取所有状态值
//				参数1,2：	真空状态
//				参数3,4：	阀门状态
//				参数5,6：	CCG值
//				参数7,8：	分子泵转速值
//				参数9,10：	分子泵温度值
//				参数11,12：	分子泵运行时间值
//				参数13：	01为钨灯丝；02为场枪
//				参数14：	01为允许开启NAV

/////////////////////////////////////////////////////////////////////
// Thresholds:
// SEM:
const	double	d_PLCvacBRD_ThresholdSEM_VacPre1	= 1.0;
const	double	d_PLCvacBRD_ThresholdSEM_VacPre2	= 0.1;
const	double	d_PLCvacBRD_ThresholdSEM_VacOK1		= 1E-4;
const	double	d_PLCvacBRD_ThresholdSEM_VacOK2		= 8E-5;
const	double	d_PLCvacBRD_ThresholdSEM_VacNotOK1	= 1E-4;
const	double	d_PLCvacBRD_ThresholdSEM_VacNotOK2	= 8E-5;
// FEM:
const	double	d_PLCvacBRD_ThresholdFEM_VacPre1	= 3.75;
const	double	d_PLCvacBRD_ThresholdFEM_VacPre2	= 0.01;
const	double	d_PLCvacBRD_ThresholdFEM_VacOK1		= 9E-5;
const	double	d_PLCvacBRD_ThresholdFEM_VacOK2		= 1E-7;
const	double	d_PLCvacBRD_ThresholdFEM_VacNotOK1	= 9E-5;
const	double	d_PLCvacBRD_ThresholdFEM_VacNotOK2	= 1E-7;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

typedef struct tagStatus_PLCvacBrd_vac{
	WORD	XX			: 8;	// 00000000 ????????
	WORD	VacChambOK	: 1;	// 0000000? 00000000
	WORD	VacErr		: 1;	// 000000?0 00000000
	WORD	TMPErr		: 1;	// 00000?00 00000000
	WORD	TMPFail		: 1;	// 0000?000 00000000
	WORD	TMPLink		: 1;	// 000?0000 00000000
	WORD	MPErr		: 1;	// 00?00000 00000000
	WORD	CCGErr		: 1;	// 0?000000 00000000
	WORD	rev			: 1;	// ?0000000 00000000
}Status_PLCvacBrd_vac;

typedef union tagParam_PLCvacBrd_vac
{
	WORD		wStatus;
	Status_PLCvacBrd_vac	status;	
}Param_PLCvacBrd_vac;

typedef struct tagStatus_PLCvacBrd_valve{
	WORD	V1			: 1;	// 00000000 0000000?
	WORD	V2			: 1;	// 00000000 000000?0
	WORD	V3			: 1;	// 00000000 00000?00
	WORD	V4			: 1;	// 00000000 0000?000
	WORD	V5			: 1;	// 00000000 000?0000
	WORD	MP			: 1;	// 00000000 00?00000
	WORD	TMP			: 1;	// 00000000 0?000000
	WORD	CCD			: 1;	// 00000000 ?0000000
	WORD	mode		: 1;	// 0000000? 00000000
	WORD	V1_SEM		: 1;	// 000000?0 00000000
	WORD	REV			: 6;	// ??????00 00000000
}Status_PLCvacBrd_valve;

typedef union tagParam_PLCvacBrd_valve
{
	WORD		wValve;
	Status_PLCvacBrd_valve	valve;	
}Param_PLCvacBrd_valve;

typedef struct tagParam_PLCvacBrd_TMP
{
	WORD	wR;	// rotate speed
	WORD	wD;	// degree
	WORD	wT;	// time
}Param_PLCvacBrd_TMP;

typedef struct tagStatus_PLCvacBrd{
	double	dPccg;
	int		nType;	// 01为钨丝枪；02为场枪

	Param_PLCvacBrd_vac		statusVac;
	Param_PLCvacBrd_valve	statusValve;
	Param_PLCvacBrd_TMP		tmp;
} Status_PLCvacBrd;

#endif
