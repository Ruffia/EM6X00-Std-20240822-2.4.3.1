#ifndef __CommMC6600_H
#define __CommMC6600_H

#include <afxtempl.h>

#define WM_USER_MC6600_UpdateValue	(WM_USER+98)

/*
typedef struct
{
}KYKYCommMsg_Ver, *PKYKYCommMsg_Ver;
*/
typedef struct _MC6600_Params_Speed{
	float	fSpeed[5];		// 运行轴速度
	float	fSpeedInit[5];	// 运行轴初速度
	float	fSpeedAcc[5];	// 运行轴加速度
	float	fSpeedHome[5];	// 运行轴回原点速度
	float   fSpeedTrack[5];
} MC6600_Params_Speed;

typedef struct _MC6600_Params_Fault{
	BOOL	bCommunicationFault[5];
	BOOL    bMotorFault[5];		//电机故障 0为故障
} MC6600_Params_Fault;
typedef struct _MC6600_Params_Current{
	float	fSteps[5];		// 当前位置
	float	fOrigin[5];		// 用户原点位置
	int		nStatus[5];		// 当前状态
} MC6600_Params_Current;

typedef struct _MC600_Params_Bl{
	short	nDir[5];		// -1表示负方向，1表示正方向
	float	fCount[5];		// 间隙量，单位码数
	int     nCoeff[5];		// 回程系数
} MC6600_Params_Bl;

typedef struct _MC6600_Params_AxialAddr{
	char    cAxial[5];
	BYTE	byteAddr[5];	// 从站地址
} MC6600_Params_AxialAddr;

typedef struct _MC6600_Params_LimitDir{
	float   fA[5];
	float   fB[5];			// 极限方向
}MC6600_Params_LimitDir;

typedef struct _MC6600_Params_DriveDir{
	short   nDir[5];		// 驱动方向：-1表示负方向，1表示正方向
}MC6600_Params_DriveDir;

typedef struct _MC6600_Params_SetupWizard{
	int		nSetMotorType[5];	//电机类型
	int		nEncoderLines[5];	//编码器线数
	int		nControlMode[5];	//控制方式
	int		nControlSignal[5];	//输入信号
	int		nMaxCurrent[5];		//最大电流
}MC6600_Params_SetupWizard;	// 设置向导

typedef struct _MC6600_Params_PID{
	float    fP[5];
	float    fI[5];
	float    fD[5];
}MC6600_Params_PID;


struct structMC6600Pos{
	CString	strName;		// 名称
	float	fPos[5];		// 位置
};
typedef CTypedPtrList< CPtrList, structMC6600Pos* > ListMC6600Pos;

typedef struct _MC6600_Params_ZRes{
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

} MC6600_Params_ZRes;


typedef struct _MC6600_Params_Style{
	unsigned char ucAxis;	// 老控制器字符
	short	nAxis;			// 老控制器索引
	BOOL	bOld;			// 是否有老控制器

	short	nType;			// 0：KYKY中型台；1：KYKY大型台；2：Klein中型台；3：Klein大型台
} MC6600_Params_Style;

#endif
