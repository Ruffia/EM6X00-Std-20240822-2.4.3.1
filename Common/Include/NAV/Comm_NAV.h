#ifndef __CommNAV_H
#define __CommNAV_H
/*
#define WM_CHANGE_CAMERA_STATE	(WM_USER + 5000)
#define WM_CONTROL_STAGE_MOVE	(WM_USER + 6000)
#define WM_WRITE_STARTXY_TOPATH	(WM_USER + 6001)
*/
#define WM_USER_NAV_Update		(WM_USER + 5000)

#define WM_SET_OPENIMAGE		(WM_USER + 701)
#define WM_SET_READARGUMENT		(WM_USER + 702)
#define WM_SET_WRITEWORLD		(WM_USER + 703)
#define WM_SET_CREATEWORLD		(WM_USER + 704)

#define WM_SEND_START_SAMPLER	(WM_USER + 705)
#define WM_SEND_STOP_SAMPLER	(WM_USER + 706)
#define WM_SEND_SAVE_IMAGE		(WM_USER + 707)

typedef struct _NAV_Position{
	float	fPosX;
	float	fPosY;
	float	fPosZ;

/*	float   fPosXReceive;
	float   fPosYReceive;
	float   fPosZReceive;
	float   fPosRReceive;
	float   fPosTReceive;
*/
	_NAV_Position()
	{
		fPosX = 0.f;
		fPosY = 0.f;
		fPosZ = 0.f;
/*		fPosXReceive = 0.f;
		fPosYReceive = 0.f;
		fPosZReceive = 0.f;
		fPosRReceive = 0.f;
		fPosTReceive = 0.f;*/
	}
} NAV_Position;

typedef struct _NAV_Position5{
	float	fPosX;
	float	fPosY;
	float	fPosZ;
	float	fPosR;
	float	fPosT;
	_NAV_Position5()
	{
		fPosX = 0.f;
		fPosY = 0.f;
		fPosZ = 0.f;
		fPosR = 0.f;
		fPosT = 0.f;
	}
} NAV_Position5;

typedef struct _NAV_Settings_Move{
	NAV_Position	posStart;		// 导航拍照时移动到的位置
	NAV_Position	posEnd;			// 导航拍照结束后回到的位置???
	NAV_Position5	posOpen;		// 放气后开样品室时移动到的位置???

	CString			strExpressionX;	// X坐标转换公式表达式
	CString			strExpressionY;	// Y坐标转换公式表达式

	float			fDiffPos;		// 误差多大算到位
	int				nProcess;		// 摄像头的安装位置决定了执行的流程：0：样品室门；1：样品室内；2：样品室外
} NAV_Settings_Move;

typedef struct _NAV_Settings_Camera{
	float	fZoom;					// 放大倍数
	int		nExposureTime;			// 曝光时间
	int		nGain;					// 增益
	int		nFrame;					// 帧率

	int		nCameraW;				// 摄像头X分辨率
	int		nCameraH;				// 摄像头Y分辨率
	int		nOffsetX;
	int		nOffsetY;

	int		nTriggerMode;			// 触发模式
	int		nTriggerSource;			// 触发源
	int		nTriggerLine;
	int		nTriggerActivate;
} NAV_Settings_Camera;

typedef struct _NAV_Settings_ImageShow{
	int		nHorizontal;
	int		nVertical;
	int		nLeftRotateCount;
	int		nRightRotateCount;
} NAV_Settings_ImageShow;

typedef struct _NAV_Settings_ImageStorage{
	int		nTick;
	CString	strPath;
} NAV_Settings_ImageStorage;

typedef struct _NAV_Settings{
	NAV_Settings_Move			move;
	NAV_Settings_Camera			camera;
	NAV_Settings_ImageShow		imgShow;
	NAV_Settings_ImageStorage	imgStorage;
} NAV_Settings;

#endif
