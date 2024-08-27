#ifndef __CommFunctions_H
#define __CommFunctions_H

//发送命令字定义
#define	KYKYComm_TimerID_Listen		200
#define	KYKYComm_TimerID_Receive	201
#define	KYKYComm_TimerID_Send		202

#define	KYKYComm_IMAGEDATA			2000	//请求图像数据


#define	KYKYComm_CMD_Sys_Password			0x01
#define KYKYComm_CMD_Sys_Connect_Success	0x02
#define KYKYComm_CMD_Sys_Disconnect			0x03
#define KYKYComm_CMD_Sys_ErrInfo			0x04
#define KYKYComm_CMD_Sys_GetVersion			0x05
#define KYKYComm_CMD_Sys_GetSEMType			0x06
#define KYKYComm_CMD_Sys_GetHighVoltageType	0x07

#define KYKYComm_CMD_Image_Sync				0x11
#define KYKYComm_CMD_Image_Stop				0x12
#define KYKYComm_CMD_Image_Snap				0x13
#define KYKYComm_CMD_Image_Save				0x14
#define KYKYComm_CMD_Image_GetFrameData		0x15

#define KYKYComm_CMD_ScanCtrl_Freeze		0x21
#define KYKYComm_CMD_ScanCtrl_Normal		KYKYComm_CMD_ScanCtrl_Freeze +0x01
#define KYKYComm_CMD_ScanCtrl_ActiveArea	KYKYComm_CMD_ScanCtrl_Freeze +0x02
#define KYKYComm_CMD_ScanCtrl_Line_HORZ		KYKYComm_CMD_ScanCtrl_Freeze +0x03
#define KYKYComm_CMD_ScanCtrl_Line_VERT		KYKYComm_CMD_ScanCtrl_Freeze +0x04
#define KYKYComm_CMD_ScanCtrl_Spot			KYKYComm_CMD_ScanCtrl_Freeze +0x05
#define KYKYComm_CMD_ScanCtrl_Sync			KYKYComm_CMD_ScanCtrl_Freeze +0x06
#define KYKYComm_CMD_ScanCtrl_IsStopScan	KYKYComm_CMD_ScanCtrl_Freeze +0x07

#define KYKYComm_CMD_Get_ScanCtrl_Speed		0x31
#define KYKYComm_CMD_Set_ScanCtrl_Speed		KYKYComm_CMD_Get_ScanCtrl_Speed +0x01
// #define KYKYComm_CMD_ScanCtrl_Speed1		KYKYComm_CMD_ScanCtrl_Speed +0x01
// #define KYKYComm_CMD_ScanCtrl_Speed2		KYKYComm_CMD_ScanCtrl_Speed +0x02
// #define KYKYComm_CMD_ScanCtrl_Speed3		KYKYComm_CMD_ScanCtrl_Speed +0x03
// #define KYKYComm_CMD_ScanCtrl_Speed4		KYKYComm_CMD_ScanCtrl_Speed +0x04

#define KYKYComm_CMD_Get_ScanCtrl_Reso		0x38
#define KYKYComm_CMD_Set_ScanCtrl_Reso		KYKYComm_CMD_Get_ScanCtrl_Reso +0x01
// #define KYKYComm_CMD_ScanCtrl_Reso1		KYKYComm_CMD_Get_ScanCtrl_Reso +0x02
// #define KYKYComm_CMD_ScanCtrl_Reso2		KYKYComm_CMD_Get_ScanCtrl_Reso +0x03

#define KYKYComm_CMD_Get_MagRange			0x3C
#define KYKYComm_CMD_Get_ScanRotAngleInit	0x3D
#define KYKYComm_CMD_Get_VacStatus			0x3E
#define KYKYComm_CMD_Get_GunStatus			0x3F

#define KYKYComm_CMD_Get_Magnification		0x41
#define KYKYComm_CMD_Get_WorkDistance		KYKYComm_CMD_Get_Magnification +0x01
#define KYKYComm_CMD_Get_ScanRotAngle		KYKYComm_CMD_Get_Magnification +0x02
#define KYKYComm_CMD_Get_Stigs				KYKYComm_CMD_Get_Magnification +0x03
#define KYKYComm_CMD_Get_StigX				KYKYComm_CMD_Get_Magnification +0x04
#define KYKYComm_CMD_Get_StigY				KYKYComm_CMD_Get_Magnification +0x05
#define KYKYComm_CMD_Get_Brightness			KYKYComm_CMD_Get_Magnification +0x06
#define KYKYComm_CMD_Get_Contrast			KYKYComm_CMD_Get_Magnification +0x07
#define KYKYComm_CMD_Get_Brightness2		KYKYComm_CMD_Get_Magnification +0x08
#define KYKYComm_CMD_Get_FieldOfView		KYKYComm_CMD_Get_Magnification +0x09

#define KYKYComm_CMD_Set_Magnification		0x51
#define KYKYComm_CMD_Set_WorkDistance		KYKYComm_CMD_Set_Magnification +0x01
#define KYKYComm_CMD_Set_ScanRotAngle		KYKYComm_CMD_Set_Magnification +0x02
#define KYKYComm_CMD_Set_Stigs				KYKYComm_CMD_Set_Magnification +0x03
#define KYKYComm_CMD_Set_StigX				KYKYComm_CMD_Set_Magnification +0x04
#define KYKYComm_CMD_Set_StigY				KYKYComm_CMD_Set_Magnification +0x05
#define KYKYComm_CMD_Set_Brightness			KYKYComm_CMD_Set_Magnification +0x06
#define KYKYComm_CMD_Set_Contrast			KYKYComm_CMD_Set_Magnification +0x07
#define KYKYComm_CMD_Set_Brightness2		KYKYComm_CMD_Set_Magnification +0x08

#define KYKYComm_CMD_Get_HighVoltage		0x61
#define KYKYComm_CMD_Get_Fila				KYKYComm_CMD_Get_HighVoltage +0x01
#define KYKYComm_CMD_Get_BeamCurrent		KYKYComm_CMD_Get_HighVoltage +0x02
#define KYKYComm_CMD_Get_Bias				KYKYComm_CMD_Get_HighVoltage +0x03
#define KYKYComm_CMD_Set_HighVoltage		KYKYComm_CMD_Get_HighVoltage +0x04
#define KYKYComm_CMD_Set_Fila				KYKYComm_CMD_Get_HighVoltage +0x05
#define KYKYComm_CMD_Set_BeamCurrent		KYKYComm_CMD_Get_HighVoltage +0x06
#define KYKYComm_CMD_Set_Bias				KYKYComm_CMD_Get_HighVoltage +0x07
#define KYKYComm_CMD_Get_EMI				KYKYComm_CMD_Get_HighVoltage +0x08

#define KYKYComm_CMD_Stage_GetStatus		0x81
#define KYKYComm_CMD_Stage_GetRange			KYKYComm_CMD_Stage_GetStatus +0x01
#define KYKYComm_CMD_Stage_GetRange2		KYKYComm_CMD_Stage_GetStatus +0x02
#define KYKYComm_CMD_Stage_GetRange5		KYKYComm_CMD_Stage_GetStatus +0x03
#define KYKYComm_CMD_Stage_GetPos			KYKYComm_CMD_Stage_GetStatus +0x04
#define KYKYComm_CMD_Stage_GetPos2			KYKYComm_CMD_Stage_GetStatus +0x05
#define KYKYComm_CMD_Stage_GetPos5			KYKYComm_CMD_Stage_GetStatus +0x06
#define KYKYComm_CMD_Stage_SetPos			KYKYComm_CMD_Stage_GetStatus +0x07
#define KYKYComm_CMD_Stage_SetPos2			KYKYComm_CMD_Stage_GetStatus +0x08
#define KYKYComm_CMD_Stage_SetPos5			KYKYComm_CMD_Stage_GetStatus +0x09
#define KYKYComm_CMD_Stage_Stop				KYKYComm_CMD_Stage_GetStatus +0x0A
#define KYKYComm_CMD_Stage_GoHome			KYKYComm_CMD_Stage_GetStatus +0x0B
#define KYKYComm_CMD_Stage_GetGratings		KYKYComm_CMD_Stage_GetStatus +0x0C

#define KYKYComm_CMD_Func_AutoBC			0x91
#define KYKYComm_CMD_Func_ShutdownBeam		KYKYComm_CMD_Func_AutoBC +0x01

/*
#define WM_USER_KYKYComm_StartListen			WM_USER+501
#define WM_USER_KYKYComm_Error_Bind				WM_USER+502
#define WM_USER_KYKYComm_Error_Listen			WM_USER+503
*/
#define WM_USER_KYKYComm_Thread_Connect			WM_USER+511
#define WM_USER_KYKYComm_Connect_Status			WM_USER+512
#define WM_USER_KYKYComm_Timeout				WM_USER+513
#define WM_USER_KYKYComm_Thread_Read			WM_USER+514
#define WM_USER_KYKYComm_Thread_Send			WM_USER+515
#define WM_USER_KYKYComm_Thread_Send_Connect	WM_USER+516
#define WM_USER_KYKYComm_Recv					WM_USER+517
#define WM_USER_KYKYComm_Thread_Send_Img		WM_USER+518	// 传递图像数据

#define WM_USER_KYKYComm_ClientThreadSend_Command	WM_USER+521
#define WM_USER_KYKYComm_ClientThreadSend_Param	WM_USER+522

#define WM_USER_Stop_AutoFocus   WM_USER + 523
#define WM_USER_Start_AutoFocus  WM_USER + 524

//#define nRowLen  8
#define KYKYComm_SnapMode				1
#define KYKYComm_SimulScanMode			2
#define KYKYComm_ListenPort1			3000
#define KYKYComm_ListenPort2			3500

#define KYKYComm_IMGFRAMESIZE			(2048*1536)	//1024*768 +1    2048*1536	(2048*16)
#define KYKYComm_IMGSIZE				(2048*16)	//1024*768 +1    2048*1536	(2048*16)
#define KYKYComm_MALLOC_SIZE			20			//用于存储接收到的图像数据的内存块的数量，以一次接收图像数据块为单位

const	UINT KYKYComm_Remote_Timer		= 50;

typedef struct
{
	BYTE	bHead;
	BYTE	bCommand;
	WORD	versionNum;
}KYKYCommMsg_Ver, *PKYKYCommMsg_Ver;

typedef struct  
{
	// 为补齐ImgData前为4字节，添加bHead2与bCommand2;
	BYTE	bHead1;
	BYTE	bHead2;
	BYTE	bCommand1;
	BYTE	bCommand2;
	int  nSendData;
}KYKYCommMsg_SendDataS1i, *PKYKYCommMsg_SendDataS1i;

typedef struct
{
	BYTE	bHead;
	BYTE	bCommand;
	CSize	szSize;
}KYKYCommMsg_SendDataS2i, *PKYKYCommMsg_SendDataS2i;

typedef struct
{
	BYTE	bHead;
	BYTE	bCommand;
	double	dValue;
}KYKYCommMsg_SendDataS1f, *PKYKYCommMsg_SendDataS1f;

typedef struct  
{
	float	fValue1;
	float	fValue2;
}KYKYCommMsg_Data2f;

typedef struct  
{
	float	fValue1;
	float	fValue2;
	float	fValue3;
	float	fValue4;
	float	fValue5;
}KYKYCommMsg_Data5f;

typedef struct  
{
	char	cIndex;
	float	fValue;
}KYKYCommMsg_DataGroup;

typedef struct
{
	float	fValueX[2];
	float	fValueY[2];
}KYKYCommMsg_Data2fGroup;

typedef struct  
{
	float	fValueX[2];
	float	fValueY[2];
	float	fValueZ[2];
	float	fValueR[2];
	float	fValueT[2];
}KYKYCommMsg_Data5fGroup;

typedef struct
{
	BYTE	bHead;
	BYTE	bCommand;
	KYKYCommMsg_Data5f	Values;
}KYKYCommMsg_SendDataS5f, *PKYKYCommMsg_SendDataS5f;

typedef struct
{
	CRect   rcDest;
	BYTE	bImageBuff[KYKYComm_IMGSIZE];
}KYKYCommMsg_ImgData, *PKYKYCommMsg_ImgData;

typedef struct  
{
	// 为补齐ImgData前为4字节，添加bHead2与bCommand2;
	BYTE	bHead1;
	BYTE	bHead2;
	BYTE	bCommand1;
	BYTE	bCommand2;
	KYKYCommMsg_ImgData	msgImgData;
}KYKYCommMsg_Image;

typedef struct
{
	CRect   rcDest;
	BYTE	bImageBuff[KYKYComm_IMGFRAMESIZE];
}KYKYCommMsg_ImgFrameData, *PKYKYCommMsg_ImgFrameData;

typedef struct  
{
	// 为补齐ImgData前为4字节，添加bHead2与bCommand2;
	BYTE	bHead1;
	BYTE	bHead2;
	BYTE	bCommand1;
	BYTE	bCommand2;
	KYKYCommMsg_ImgFrameData	msgImgData;
}KYKYCommMsg_ImageFrame;

///////////////////////////////////////
typedef struct  
{
	BYTE	bHead1;
	BYTE	bHead2;
	BYTE	bCommand;
	int		nValue;
}KYKYCommMsg_SendDataC1i, *PKYKYCommMsg_SendDataC1i;	// Client send data: 1 int data

typedef struct
{
	BYTE	bHead1;
	BYTE	bHead2;
	BYTE	bCommand;
	CSize	szSize;
}KYKYCommMsg_SendDataC2i, *PKYKYCommMsg_SendDataC2i;	// Client send data: 2 int data

typedef struct
{
	BYTE	bHead1;
	BYTE	bHead2;
	BYTE	bCommand;
	float	fValue;
}KYKYCommMsg_SendDataC1f, *PKYKYCommMsg_SendDataC1f;	// Client send data: 1 float data

typedef struct
{
	BYTE	bHead1;
	BYTE	bHead2;
	BYTE	bCommand;
	float	fValue1;
	float	fValue2;
}KYKYCommMsg_SendDataC2f, *PKYKYCommMsg_SendDataC2f;	// Client send data: 2 float data

typedef struct
{
	BYTE	bHead1;
	BYTE	bHead2;
	BYTE	bCommand;
	KYKYCommMsg_Data5f Values;
}KYKYCommMsg_SendDataC5f, *PKYKYCommMsg_SendDataC5f;	// Client send data: 5 float data

typedef struct
{
	BYTE	bHead1;
	BYTE	bHead2;
	BYTE	bCommand;
	KYKYCommMsg_Data2fGroup Values;
}KYKYCommMsg_SendDataC2fGroup, *PKYKYCommMsg_SendDataC2fGroup;	// Client send data: 2 float data

typedef struct
{
	BYTE	bHead1;
	BYTE	bHead2;
	BYTE	bCommand;
	KYKYCommMsg_Data5fGroup Values;
}KYKYCommMsg_SendDataC5fGroup, *PKYKYCommMsg_SendDataC5fGroup;	// Client send data: 5 float data
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//发送命令字定义
#define  TimerID_Listen			0
#define  TimerID_Receive		1
#define  TimerID_Send			2

#define	IMAGEDATA				2000				//请求图像数据


#define CMD_PASSWORD			0X01
#define CMD_CONNECT_SECCESS		0X02
#define CMD_ERRINFO				0X03
#define CMD_QUERY_VERSION		0X04
#define CMD_SNAP				0X05
#define CMD_SIMULIMAGE			0X06
#define CMD_SAVE				0X07
#define CMD_DISCONNECT			0X08
#define CMD_RESOLUTION			0X09
#define CMD_SIMULSCAN			0x0A
#define CMD_NORMALSCAN			0x0B
#define CMD_STOPSIMULIMAGE		0x0C
#define CMD_FREEZESCAN			0X0D

#define CMD_SCAN_SPEED1			0X0E
#define CMD_SCAN_SPEED2			0X0F
#define CMD_SCAN_SPEED3			0X10
#define CMD_SCAN_SPEED4			0X11

#define CMD_IMAGE_RESOLUTION1	0X12
#define CMD_IMAGE_RESOLUTION2	0X13
#define CMD_IMAGE_RESOLUTION3	0X14
#define CMD_IMAGE_RESOLUTION4	0X15

#define CMD_MAGNIFICATION		0X16
#define CMD_SCANROT_ANGLE		0X17



#define WM_THREAD_READ			WM_USER+500
#define WM_RECV					WM_USER+501
#define WM_SEND_CONNECT_THREAD	WM_USER+502  
#define WM_THREAD_SEND			WM_USER+503 
#define WM_CONNECT_STATUS		WM_USER+504
#define WM_OUTTIME				WM_USER+505
#define WM_BINDERROR			WM_USER+506
#define WM_LISTENERROR			WM_USER+507
#define WM_STARTLISTEN			WM_USER+508

#define WP_RECV					100
#define WP_SEND					101

//#define nRowLen  8
#define nSnapMode				1
#define nSimulScanMode			2
#define nPort1					3000
#define nPort2					3500

#define IMGSIZE					(2048*16)	//1024*768 +1    2048*1536
#define MALLOC_SIZE				20			//用于存储接收到的图像数据的内存块的数量，以一次接收图像数据块为单位

const	UINT nRemote_Timer		= 50;

/*
typedef struct 
{
	CSize szReso;
}MSGIMAGESIZE, *PMSGIMAGESIZE;

typedef struct  
{
	BYTE bHead;
	BYTE bCommand;
	int  nSendVal;
}MSGSENDINT;

typedef struct
{
	BYTE bHead;
	BYTE bCommand;
	MSGIMAGESIZE msgSize;
}MSGRESOLUTION;

typedef struct
{
	CRect   rcDest;
	BYTE	bImageBuff[IMGSIZE];
}MSGIMAGEDATA, *PMSGIMAGEDATA;

typedef struct  
{
	BYTE	bHead;
	BYTE	bCommand;
	MSGIMAGEDATA	msgData;
}MSGIMAGE;

typedef struct
{
	BYTE	bHead;
	BYTE	bCommand;
	WORD	versionNum;
}MSGVERSION, *PMSGVERSION;

/*
typedef struct
{
	int  iCommand;
	CRect rcDest;
	BYTE bImageBuff[IMGSIZE];
}MSGIMAGEDATA;
*/

#endif
