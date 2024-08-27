#ifndef __CommMC600_H
#define __CommMC600_H

#include <afxtempl.h>

#define WM_USER_MC600_UpdateValue	(WM_USER+98)

const	int nSpeedXY		= 960;
const	int	nSpeedInitXY	= 800;
const	int	nSpeedHomeXY	= 800;

// typedef struct
// {
// }KYKYCommMsg_Ver, *PKYKYCommMsg_Ver;

typedef enum _MC600_Param_Speed{
	Speed,			// 运行轴速度
	SpeedInit,		// 运行轴初速度
	SpeedAcc,		// 运行轴加速度
	SpeedHome		// 运行轴回原点速度
} MC600_Param_Speed;

typedef enum _MC600_Param_Home{
	Origin,			// 运动轴归零模式
	HomeMode,		// 运动轴归零方式
	UserOrigin		// 运动轴工作原点
} MC600_Param_Home;

typedef enum _MC600_Param_Limit{
	LimitAble,		// 运动轴软限位状态
	LimitPositive,	// 运动轴正向软件极限
	LimitNegative	// 运动轴负向软件极限
} MC600_Param_Limit;

typedef enum _MC600_Param_Sys{
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
} MC600_Param_Sys;

// PortOutput,	// 控制器IO口

typedef struct _MC600_Params_Sys{
	float	fPitch[4];		// 丝杠导程
	float	fStepsRev[4];	// 每转脉冲数？？？
	float	fRat[4];		// 传动比
	float	fDistance[4];	// 平移台满行程
} MC600_Params_Sys;

typedef struct _MC600_Params_Speed{
	float	fSpeed[4];		// 运行轴速度
	float	fSpeedInit[4];	// 运行轴初速度
	float	fSpeedAcc[4];	// 运行轴加速度
	float	fSpeedHome[4];	// 运行轴回原点速度
} MC600_Params_Speed;

typedef struct _MC600_Params_Pos{
	float	fSteps[4];		// 当前位置
	float	fOrigin[4];		// 用户原点位置
} MC600_Params_Pos;

typedef struct _MC600_Params_Bl{	// 间隙参数
	short	nDir[4];		// -1表示负方向，1表示正方向
	int		nCount[4];		// 间隙量，单位pp
} MC600_Params_Bl;

struct structMC600Pos{
	CString	strName;		// 名称
	float	fPos[4];		// 位置
};
typedef CTypedPtrList< CPtrList, structMC600Pos* > ListMC600Pos;

typedef struct _MC600_Params_ZRes{
	float	fZ0WD;			// Z向光电零面对应的工作距离Z0wd
	float	fSafeWD;		// 安全物距
	float	fZsink;			// 二次电子探头下缘距物镜面Z下沉
//	float	fZ16;			// Z向上升到开始限位的位置

	float	fZbr[2];		// 避让面：br1为Z向从零位上升到开始限位的位置
							//			Zbr1 = Z16 = Z0wd -H -Zsink
							//			Zbr0 = Z16 +H = Z0wd -Zsink
	float	fCali[3];		// X/Y/Z 单向限制标定位置，其中X标定=-20.2；Y标定=-30.0；
							//							Z标定 = Z0wd -safeWD -H
	float	fLimitPos[3];	// X/Y/Z 正向极限
	float	fLimitNeg[3];	// X/Y/Z 负向极限

	float	fRt;			// 探头半径：10.0
	float	fRyp;			// 样品半径：[8,50)。用户可输入，缺省为16
	float	fRx0;			// 24.5
	float	fRy0;			// 16.0

	float	fH;				// 样品高度H：用户[5-30)；调试人员[3,30)。缺省为16
} MC600_Params_ZRes;
#endif
