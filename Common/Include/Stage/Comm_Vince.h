#ifndef __CommVince_H
#define __CommVince_H

#include <afxtempl.h>

#define WM_USER_Vince_UpdateValue	(WM_USER+99)

/////////////////////////////////////////////////////////////////////
// 马达转速限制0.3转~1转，对应32微步细分，大约是1920 ~ 6400 (200步每1转)
// 电机步距角为1.8度，32细分相当于用32步走完1.8度
// 此细分下，马达 1 STEP = 1.8 / 32
// T向的齿轮传动比是 36:1
// 1 STEP 对应的倾斜终端转角应该为 1.8 / 32 / 36 = 1 / 640
// 对应不同细分状态下，1 STEP计数对应的终端转角为：
//		= 1.8 / 细分数 / 减速传动比
/////////////////////////////////////////////////////////////////////

const int	nStep		= 200;	// 200步每转
const int	nDriveRat	= 36;	// T向齿轮传动比

typedef struct tag_Vince_Params_Sys{
	short	mcs;	// 微步细分
	long	acc;	// 加速度
	long	dec;	// 减速度
	int		zsd;	// 归零速度
	long	zsp;	// 归零后安全位置设置
} Vince_Params_Sys;

typedef struct tag_Vince_Params{
	DWORD	speed;	// 速度
	int		pos;	// 位置
} Vince_Params;

// 每一个脉冲数所对应的角度为：StepAngle / (细分数 *Step) /DriveRat
// 即：360 / (32 *200) / 36 = 0.0015625
// T向脉冲数：-11953 ~ 46267，换算后即为：-18.8756 ~ 72.2921875 度
// 归零后安全位置zsp固定设置为4800，保证到负向限位开关后能退出来，保证弹簧片不被压

typedef struct tag_Vince_Status{
	DWORD	S1		: 1;	// 00000000 00000000 00000000 0000000?	// 传感器S1状态位，0为低电平，1为高电平
	DWORD	S2		: 1;	// 00000000 00000000 00000000 000000?0
	DWORD	S3		: 1;	// 00000000 00000000 00000000 00000?00
	DWORD	S4		: 1;	// 00000000 00000000 00000000 0000?000
	DWORD	pos		: 1;	// 00000000 00000000 00000000 000?0000	// 当前位置与目标位置是否相等
	DWORD	spd		: 1;	// 00000000 00000000 00000000 00?00000	// 当前速度与目标速度是否相等
	DWORD	err_hw	: 1;	// 00000000 00000000 00000000 0?000000	// 硬件错误状态位
	DWORD	org		: 1;	// 00000000 00000000 00000000 ?0000000	// 原点标志位
	DWORD	stp		: 1;	// 00000000 00000000 0000000? 00000000	// 停止标志位
	DWORD	err_cmd	: 1;	// 00000000 00000000 000000?0 00000000	// 指令错误标志位
	DWORD	err_wr	: 1;	// 00000000 00000000 00000?00 00000000	// 存储器读写错误标志位
	DWORD	off		: 1;	// 00000000 00000000 0000?000 00000000	// 离线运行标志位
	DWORD	shake	: 1;	// 00000000 00000000 000?0000 00000000	// 握手信号标志位
	DWORD	en		: 1;	// 00000000 00000000 00?00000 00000000	// 电镜使能标志位
	DWORD	Zerofin	: 1;	// 00000000 00000000 0?000000 00000000	// 归零结束标志位
	DWORD	Rev15	: 1;	// 00000000 00000000 ?0000000 00000000
	DWORD	S5		: 1;	// 00000000 0000000? 00000000 00000000	// 传感器S5状态位，0为低电平，1为高电平
	DWORD	S6		: 1;	// 00000000 000000?0 00000000 00000000	// 传感器S5状态位，0为低电平，1为高电平
	DWORD	Rev1819	: 2;	// 00000000 0000??00 00000000 00000000
	DWORD	ovrh	: 1;	// 00000000 000?0000 00000000 00000000	// 过热保护标志位，0为正常
	DWORD	ovrc	: 1;	// 00000000 00?00000 00000000 00000000	// 过流保护标志位，0为正常
	DWORD	ovrv	: 1;	// 00000000 0?000000 00000000 00000000	// 过压保护标志位，0为正常
	DWORD	Rev		: 9;	// ???????? ?0000000 00000000 00000000
} Vince_Status;

typedef union tag_Vince_Param_Status
{
	DWORD			dwStatus;
	Vince_Status	status;	
} Vince_Param_Status;

typedef enum _Vince_Param_Cfg{
	Cfg_brd,	// 波特率					// 9600 ~ 921600
	Cfg_cid,	// 通讯ID					// 1 ~ 32
	Cfg_mcs,	// 微步细分					// 0 ~ 8 (5, 32微步)
	Cfg_spd,	// 速度						// -192000 ~ 192000
	Cfg_acc,	// 加速度					// 0 ~ 19200 0000
	Cfg_dec,	// 减速度					// 0 ~ 19200 0000
	Cfg_cra,	// 加速电流					// 0 ~ 2.5
	Cfg_crn,	// 匀速运行电流				// 0 ~ 2.5
	Cfg_crh,	// 保持电流					// 0 ~ 2.5
	Cfg_s1f,	// 传感器1下降沿触发事件	// 0 ~ 9
	Cfg_s1r,	// 传感器1上升沿触发事件	// 0 ~ 9
	Cfg_s2f,	// 传感器2下降沿触发事件	// 0 ~ 9
	Cfg_s2r,	// 传感器2上升沿触发事件	// 0 ~ 9
	Cfg_s3f,	// 传感器3下降沿触发事件	// 0 ~ 9
	Cfg_s3r,	// 传感器3上升沿触发事件	// 0 ~ 9
	Cfg_s4f,	// 传感器4下降沿触发事件	// 0 ~ 9
	Cfg_s4r,	// 传感器4上升沿触发事件	// 0 ~ 9
	Cfg_s5f,	// 传感器5下降沿触发事件	// 0 ~ 9
	Cfg_s5r,	// 传感器5上升沿触发事件	// 0 ~ 9
	Cfg_s6f,	// 传感器6下降沿触发事件	// 0 ~ 9
	Cfg_s6r,	// 传感器6上升沿触发事件	// 0 ~ 9
	Cfg_s3,		// 传感器3模式设置			// 0为输入(sf/sr有效)，1为输出(sf/sr无效)
	Cfg_s4,		// 传感器4模式设置			// 0为输入(sf/sr有效)，1为输出(sf/sr无效)
	Cfg_s5,		// 传感器5模式设置			// 0为输入(sf/sr有效)，1为输出(sf/sr无效)
	Cfg_s6,		// 传感器6模式设置			// 0为输入(sf/sr有效)，1为输出(sf/sr无效)
	Cfg_zmd,	// 归零功能					// 0 ~ 5
	Cfg_snr,	// 归零用传感器				// 0 ~ 5
	Cfg_osv,	// 归零用传感器常开常闭设置	// 0:开放状态时为低电平；1:高电平
	Cfg_zsd,	// 归零速度					// -192000 ~ 192000
	Cfg_zsp,	// 归零后安全位置			// -2147483647 ~ 2147483647
	Cfg_sds,	// 无感归零灵敏度			// 0 ~ 100
	Cfg_zcr,	// 无感归零电流				// 0.2 ~ 2.5
	Cfg_dmd,	// 离线运行模式				// 0:普通模式；1:归零模式，在离线运行开始前进行归零
	Cfg_dar,	// 离线运行模式自动开启时间	// 0 ~ 60：0为开机不自动执行离线运行模式；1 ~ 60，单位秒
	Cfg_msr,	// 负极限传感器				// 0 ~ 6
	Cfg_msv,	// 负极线传感器触发电平		// 0为低电平触发；1为高电平触发
	Cfg_psr,	// 正极限传感器				// 0 ~ 6
	Cfg_psv,	// 正极限传感器触发电平		// 0为低电平触发；1为高电平触发
	Cfg_pae,	// 上电使能					// 0为上电不使能；1为上电使能
	Cfg_zar,	// 上电归零					// 0为上电不自动归零；1为上电自动归零
} Vince_Param_Cfg;

/*
typedef enum _Vince_Param_Speed{
	Speed,			// 运行轴速度
	SpeedInit,		// 运行轴初速度
	SpeedAcc,		// 运行轴加速度
	SpeedHome		// 运行轴回原点速度
} Vince_Param_Speed;

typedef enum _Vince_Param_Home{
	Origin,			// 运动轴归零模式
	HomeMode,		// 运动轴归零方式
	UserOrigin		// 运动轴工作原点
} Vince_Param_Home;

typedef enum _Vince_Param_Limit{
	LimitAble,		// 运动轴软限位状态
	LimitPositive,	// 运动轴正向软件极限
	LimitNegative	// 运动轴负向软件极限
} Vince_Param_Limit;

typedef enum _Vince_Param_Sys{
	Unit,			// 运动轴使用单位
	Style,			// 台子类型
	WorkState,		// 台子工作状态
	Pitch,			// 丝杠导程
	StepsRev,		// 每转脉冲数？？？
	DriveRat,		// 传动比
	Distance		// 平移台满行程
// 	Radius,			// 光栅半径
// 	EncoderStyle,	// 光栅尺类型
// 	EncoderReso		// 光栅尺分辨率
} Vince_Param_Sys;

// PortOutput,	// 控制器IO口

typedef struct _Vince_Params_Sys{
	float	fPitch[5];		// 丝杠导程
	float	fStepsRev[5];	// 每转脉冲数？？？
	float	fRat[5];		// 传动比
	float	fDistance[5];	// 平移台满行程
} Vince_Params_Sys;

typedef struct _Vince_Params_Speed{
	float	fSpeed[5];		// 运行轴速度
	float	fSpeedInit[5];	// 运行轴初速度
	float	fSpeedAcc[5];	// 运行轴加速度
	float	fSpeedHome[5];	// 运行轴回原点速度
} Vince_Params_Speed;

typedef struct _Vince_Params_Pos{
	float	fSteps[5];		// 当前位置
	float	fOrigin[5];		// 用户原点位置
} Vince_Params_Pos;

typedef struct _Vince_Params_Bl{	// 间隙参数
	short	nDir[5];		// -1表示负方向，1表示正方向
	int		nCount[5];		// 间隙量，单位pp
} Vince_Params_Bl;

struct structVincePos{
	CString	strName;		// 名称
	float	fPos[5];		// 位置
};
typedef CTypedPtrList< CPtrList, structVincePos* > ListVincePos;
*/
#endif
