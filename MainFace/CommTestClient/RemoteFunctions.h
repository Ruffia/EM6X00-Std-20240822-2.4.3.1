#ifndef __RemoteFunctions_H
#define __RemoteFunctions_H

//发送命令字定义
#define	KYKYRM_TimerID_Listen	200
#define	KYKYRM_TimerID_Receive	201
#define	KYKYRM_TimerID_Send		202

#define	KYKYRM_IMAGEDATA		2000	//请求图像数据


#define	KYKYRM_CMD_Sys_Password			0x01
#define KYKYRM_CMD_Sys_Connect_Success	0x02
#define KYKYRM_CMD_Sys_Disconnect		0x03
#define KYKYRM_CMD_Sys_ErrInfo			0x04

#define KYKYRM_CMD_Image_Sync			0x11
#define KYKYRM_CMD_Image_Stop			0x12
#define KYKYRM_CMD_Image_Snap			0x13
#define KYKYRM_CMD_Image_Save			0x14

#define KYKYRM_CMD_ScanCtrl_Freeze		0x21
#define KYKYRM_CMD_ScanCtrl_Normal		0x22
#define KYKYRM_CMD_ScanCtrl_ActiveArea	0x23
#define KYKYRM_CMD_ScanCtrl_Line_HORZ	0x24
#define KYKYRM_CMD_ScanCtrl_Line_VERT	0x25
#define KYKYRM_CMD_ScanCtrl_Spot		0x26
#define KYKYRM_CMD_ScanCtrl_Sync		0x27

#define KYKYRM_CMD_ScanCtrl_Speed1		0x31
#define KYKYRM_CMD_ScanCtrl_Speed2		0x32
#define KYKYRM_CMD_ScanCtrl_Speed3		0x33
#define KYKYRM_CMD_ScanCtrl_Speed4		0x34

#define KYKYRM_CMD_ScanCtrl_Reso		0x35
#define KYKYRM_CMD_ScanCtrl_Reso1		0x36
#define KYKYRM_CMD_ScanCtrl_Reso2		0x37
#define KYKYRM_CMD_ScanCtrl_Reso3		0x38
#define KYKYRM_CMD_ScanCtrl_Reso4		0x39

#define KYKYRM_CMD_Query_Version		0x41
#define KYKYRM_CMD_Query_Magnification	0x42
#define KYKYRM_CMD_Query_WorkDistance	0x43
#define KYKYRM_CMD_Query_ScanRotAngle	0x44
#define KYKYRM_CMD_Query_HighVoltage	0x45
#define KYKYRM_CMD_Query_EMI			0x46

/*
#define WM_USER_KYKYRM_StartListen			WM_USER+501
#define WM_USER_KYKYRM_Error_Bind			WM_USER+502
#define WM_USER_KYKYRM_Error_Listen			WM_USER+503
*/
#define WM_USER_KYKYRM_Thread_Connect		WM_USER+511
#define WM_USER_KYKYRM_Connect_Status		WM_USER+512
#define WM_USER_KYKYRM_Timeout				WM_USER+513
#define WM_USER_KYKYRM_Thread_Read			WM_USER+514
#define WM_USER_KYKYRM_Thread_Send			WM_USER+515
#define WM_USER_KYKYRM_Thread_Send_Connect	WM_USER+516
#define WM_USER_KYKYRM_Recv					WM_USER+517

//#define nRowLen  8
#define KYKYRM_SnapMode					1
#define KYKYRM_SimulScanMode			2
#define KYKYRM_ListenPort1				3000
#define KYKYRM_ListenPort2				3500

#define KYKYRM_IMGSIZE					(2048*16)	//1024*768 +1    2048*1536
#define KYKYRM_MALLOC_SIZE				20			//用于存储接收到的图像数据的内存块的数量，以一次接收图像数据块为单位

const	UINT KYKYRM_Remote_Timer		= 50;


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
