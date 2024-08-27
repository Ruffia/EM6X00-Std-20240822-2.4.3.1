#ifndef __CommTMCM_H
#define __CommTMCM_H

#include <afxtempl.h>

#define WM_USER_TMCM_UpdateValue	(WM_USER+98)

/*
typedef struct
{
}KYKYCommMsg_Ver, *PKYKYCommMsg_Ver;
*/
typedef struct _TMCM_Params_Speed{
	float	fSpeedPos[5];		// 运行轴定位速度
	float	fSpeedDec[5];		// 运行轴减速度
	float	fSpeedAcc[5];		// 运行轴加速度
	float	fMaxSpeedPos[5];	// 运行轴定位速度-最大
	float	fMaxSpeedDec[5];	// 运行轴减速度-最大
	float	fMaxSpeedAcc[5];	// 运行轴加速度-最大
	float	fMinSpeedPos[5];	// 运行轴定位速度-最小
	float	fMinSpeedDec[5];	// 运行轴减速度-最小
	float	fMinSpeedAcc[5];	// 运行轴加速度-最小
	float	fSpeedHome[5];		// 运行轴回原点速度
	float   fSpeedTrack[5];
} TMCM_Params_Speed;


typedef struct _TMCM_Params_Fault{
	BOOL	bCommunicationFault[5];
	BOOL    bMotorFault[5];		// 电机故障 0为故障
} TMCM_Params_Fault;

typedef struct _TMCM_Params_Elec{
	float   fAxisCurSpeed[5];       // 运行电流
	float   fAxisStandbySpeed[5];	// 运行电流
	
} TMCM_Params_Elec;

typedef struct _TMCM_Params_Current{
	float	fSteps[5];		// 当前位置
	float	fOrigin[5];		// 用户原点位置
} TMCM_Params_Current;

typedef struct _TMCM_Params_Bl{
	short	nDir[5];		// -1表示负方向，1表示正方向
	float	fCount[5];		// 间隙量，单位码数
	int     nCoeff[5];		// 回程系数
} TMCM_Params_Bl;


typedef struct _TMCM_Params_LimitStatus{
	int		nL[5];
	int		nR[5];			// 极限\接地
	int		nGL;
	int		nGR;
}TMCM_Params_LimitStatus;

typedef struct _TMCM_Params_DriveDir{
	short   nDir[5];		// 驱动方向：-1表示负方向，1表示正方向
}TMCM_Params_DriveDir;

struct structTMCMPos{
	CString	strName;		// 名称
	float	fPos[5];		// 位置
};
typedef CTypedPtrList< CPtrList, structTMCMPos* > ListTMCMPos;

typedef struct _TMCM_Params_Sys{
	float   fPitch[5];			// 丝杠导程
	float	fRat[5];			// 传动比
	float   fStepAngle[5];		// 步距角
	int 	nAxisSubDivide[5];	// 细分
	float   fStepsRev[5];		// 每转脉冲数
} TMCM_Params_Sys;

typedef struct _TMCM_Params_Pointpos{
	float   fPointPosX[7];		
	float   fPointPosY[7];	
} TMCM_Params_Pointpos;

typedef struct _TMCM_Params_ZRes{
//	float   fCentrCoor[3],fCentrCur[3];     //扫描中心坐标X/Y/Z -定标后固定/当前
	float   fProbeCenCoor[3];	// 探头中心坐标X/Y/Z
	float   fTangCircent[3];	// 相切圆中心坐标
	float   fRightArcjudg[3];	// 右圆弧位置判定
	float   fLeftCylinleft[3];	// 圆柱形样品左
	float   fLeftObjectlow[3];	// 物镜下端点
	float   fLeftSquareleft[3];	// 方形样品座左
	float   fTo;				// 旋转中心与探头中心相较于Z轴的倾斜角（固定）
	float   fInclina;			// 圆形底座右上端与方形底座右上端连线倾斜度
	float   fTa;				// T轴旋转角度
	float	fHy;				// 样品高度H：用户[5-30)；调试人员[3,30)。缺省为16
	float   fHb;				// 中心定标工具
	float   fHs;				// 样品台物理尺寸
	float   fHt;				// T轴旋转基底工作距离
	float   fRy;				// 圆柱形样品半径
	float   fRtb;				// 探头半径
	float   fRqy;				// 当前Z对应相切圆半径
	float   fDis;				// 样品中心与相切圆中心距离（与R样+R切圆比较）
	float	fZ0wd;				// Z向零点对应的工作距离Z0wd
	float	fZsink;				// 二次电子探头下缘距物镜面Z下沉
	float	fZbr[2];			// 避让面：br1为Z向从零位上升到开始限位的位置
								//			Zbr1 = fT0 -Hy
								//			Zbr0 = fZ0wd -Hy -Zsink
	float	fSafeWD;			// 安全物距
	float	fCali[3];			// X/Y/Z 单向限制标定位置，其中X标定、Y标定由边界线计算
								//							Z标定 = Z0wd -safeWD -Hy
	float	fRx0;				// 24.5
	float	fRy0;				// 16.0

	float	fLimitPos[5];		// X/Y/Z/R/T 正向极限
	float	fLimitNeg[5];		// X/Y/Z/R/T 负向极限

} TMCM_Params_ZRes;


typedef struct _TMCM_Params_Style{
	short	nVersion;
	short	nType;				// 0：KYKY中型台；1：KYKY大型台；

} TMCM_Params_Style;

typedef struct _TMCM_Params_LimitParam{
	int		nLeftLimit[5];			// 0：限位启用；1：限位禁用
	int		nRightLimit[5];	
	int		nExchangeLimit[5];		// 0：限位不交换；1：限位交换
	int     nLeftLevelReversal[5];
	int     nRightLevelReversal[5];	// 0：低电平；1：高电平

} TMCM_Params_LimitParam;
#endif
