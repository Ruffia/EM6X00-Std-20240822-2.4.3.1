
#ifndef MCIO_h
#define MCIO_h

// 主机与Z80通讯部分

// 主机与Z80通讯状态统计数据
typedef struct tagCOMM_Statistics
{
	DWORD	dwHostMsgPostRate;
	DWORD	dwHostMsgFifoOverflowRate;
	DWORD	dwHostMsgSendRate;
	DWORD	dwHostMsgSendErrorRate;
	
	DWORD	dwZ80MsgReceiveRate;
	DWORD	dwZ80MsgReceiveErrorRate;
	DWORD	dwZ80MsgProcessRate;
}COMM_Statistics;

//--ControlMessage
const WORD wSEM_ControlMessage_DATASIZE = 13;

enum MicrocomputerID
{
	SEM_Z80ID_Z80A,
	SEM_Z80ID_Z80B,
	SEM_MCI,
	SEM_MCB,
	SEM_CPS
};

typedef struct tagSEM_ControlMessage
{
	BYTE		bCtlMSGLength;							// Avialable bData Length
	union{
	BYTE		bMessageID;
	BYTE		bData[ wSEM_ControlMessage_DATASIZE ];	// SEM Control_Message Data
	};
}SEM_ControlMessage;


typedef struct tagSEM_CtlMSG_Flag
{
	MicrocomputerID				mc;						// Microcomputer ID
	int							nPriority;				// Send Priority		* reserved
	BOOL						blOverableFlag;			// Over-able Flag		* reserved
	WORD						wSynchroFlag;			// Synchronizer Flag	* reserved
}SEM_CtlMSG_Flag;


typedef struct tagSEM_SIO_Package
{
	SEM_CtlMSG_Flag		flag;
	SEM_ControlMessage	msg;
}SEM_SIO_Package;


// SEM Comm Port
/*
// SCAN
const DWORD SEM_Z80A_ReadAddr			= 0x10;
const DWORD SEM_Z80A_WriteAddr			= 0x20;

// LENS
const DWORD SEM_Z80B_ReadAddr			= 0x30;
const DWORD SEM_Z80B_WriteAddr			= 0x40;
*/

// SCAN
const DWORD SEM_Z80A_ReadAddr			= 0x1C;
const DWORD SEM_Z80A_WriteAddr			= 0x20;

// LENS
const DWORD SEM_Z80B_ReadAddr			= 0x18;
const DWORD SEM_Z80B_WriteAddr			= 0x24;


// SEM_MC_CommPortState
const BYTE SEM_Z80A_MC_Ready			= 0xA3;
const BYTE SEM_Z80A_MC_Request			= 0x01;
const BYTE SEM_Z80A_MC_Response0		= 0x30;
const BYTE SEM_Z80A_MC_Response1		= 0x31;
const BYTE SEM_Z80A_MC_FrameSycFlag0	= 0x00;
const BYTE SEM_Z80A_MC_FrameSycFlag1	= 0x01;

const BYTE SEM_Z80A_HOST_Ready			= 0xA3;
const BYTE SEM_Z80A_HOST_Request		= 0xF1;
const BYTE SEM_Z80A_HOST_Response0		= 0x30;
const BYTE SEM_Z80A_HOST_Response1		= 0x31;
const BYTE SEM_Z80A_HOST_FrameSycFlag0	= 0x00;
const BYTE SEM_Z80A_HOST_FrameSycFlag1	= 0x01;

const BYTE SEM_Z80B_MC_Ready			= 0xA3;
const BYTE SEM_Z80B_MC_Request			= 0x02;
const BYTE SEM_Z80B_MC_Response0		= 0xC0;
const BYTE SEM_Z80B_MC_Response1		= 0xC2;
const BYTE SEM_Z80B_MC_FrameSycFlag0	= 0x00;
const BYTE SEM_Z80B_MC_FrameSycFlag1	= 0x02;

const BYTE SEM_Z80B_HOST_Ready			= 0xA3;
const BYTE SEM_Z80B_HOST_Request		= 0xF2;
const BYTE SEM_Z80B_HOST_Response0		= 0xC0;
const BYTE SEM_Z80B_HOST_Response1		= 0xC2;
const BYTE SEM_Z80B_HOST_FrameSycFlag0	= 0x00;
const BYTE SEM_Z80B_HOST_FrameSycFlag1	= 0x02;

typedef struct tagSEM_CommPortState
{
	BYTE		mcReady;
	BYTE		mcRequest;
	BYTE		mcResponse0;
	BYTE		mcResponse1;
	BYTE		mcFrameSycFlag0;
	BYTE		mcFrameSycFlag1;

	BYTE		hostReady;
	BYTE		hostRequest;
	BYTE		hostResponse0;
	BYTE		hostResponse1;
	BYTE		hostFrameSycFlag0;
	BYTE		hostFrameSycFlag1;
}SEM_CommPortState;


const int SET_PARAMS_StoreMode			= 1;	// 设置数据存储模式
const int SET_PARAMS_DetectorMode		= 2;	// 设置探测器模式
const int SET_PARAMS_NeedMon			= 3;	// 设置采集时是否需要监测数据

const int SET_PARAMS_DebugMode			= 1;	// 设置调试模式
const int SET_PARAMS_Reverse			= 2;	// 设置反转
const int SET_PARAMS_DetPair			= 3;	// 设置探测器组
const int SET_PARAMS_Rate				= 4;	// 设置统调比例系数
const int SET_PARAMS_Det2nd				= 5;	// 设置探测器组中的第二通道

const int SET_PARAMS_DFocus_Start		= 11;	// 设置动态聚焦起始值
const int SET_PARAMS_DFocus_End			= 12;	// 设置动态聚焦终点值


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// USB
const WORD USB_wSEM_ControlMessage_DATASIZE = 6;

typedef struct tagUSB_SEM_ControlMessage
{
	BYTE		bType;										// 0: to MCI; 1: to other boards
	BYTE		bCtlMSGLength;								// Avialable bData Length
	union{
	BYTE		bMessageID;									// 实际上没用，以后可以去掉
	BYTE		bData[ USB_wSEM_ControlMessage_DATASIZE ];	// SEM Control_Message Data
	};
}USB_SEM_ControlMessage;


typedef struct tagUSB_SEM_CtlMSG_Flag
{
	MicrocomputerID				mc;						// Microcomputer ID
	int							nPriority;				// Send Priority		* reserved
	BOOL						blOverableFlag;			// Over-able Flag		* reserved
	WORD						wSynchroFlag;			// Synchronizer Flag	* reserved
}USB_SEM_CtlMSG_Flag;


typedef struct tagUSB_SEM_SIO_Package
{
	USB_SEM_CtlMSG_Flag		flag;
	USB_SEM_ControlMessage	msg;
}USB_SEM_SIO_Package;



typedef struct tagUSB_SEM_CommPortState
{
	BYTE		mcReady;
	BYTE		mcRequest;
	BYTE		mcResponse0;
	BYTE		mcResponse1;
	BYTE		mcFrameSycFlag0;
	BYTE		mcFrameSycFlag1;

	BYTE		hostReady;
	BYTE		hostRequest;
	BYTE		hostResponse0;
	BYTE		hostResponse1;
	BYTE		hostFrameSycFlag0;
	BYTE		hostFrameSycFlag1;
}USB_SEM_CommPortState;


// 主机与Z80通讯状态统计数据
typedef struct tagUSB_COMM_Statistics
{
	DWORD	dwHostMsgPostRate;
	DWORD	dwHostMsgFifoOverflowRate;
	DWORD	dwHostMsgSendRate;
	DWORD	dwHostMsgSendErrorRate;
	
	DWORD	dwZ80MsgReceiveRate;
	DWORD	dwZ80MsgReceiveErrorRate;
	DWORD	dwZ80MsgProcessRate;
}USB_COMM_Statistics;



#endif // MCIO_h

