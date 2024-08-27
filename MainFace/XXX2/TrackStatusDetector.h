#pragma once
#include "LibUSB_Function.h"
#include "../GlobalComponent/StatusDetector.h"

#define WM_USER_StagePod_Update	(WM_USER+110)

#define	StagePod_VID	0x054C
#define	StagePod_PID	0x0061
#define	StagePod_Stop	0x0200
#define StagePod_Offset_Value	50
#define StagePod_Offset_Action	50//100

class CTrackStatusDetector : public CStatusDetector
{
protected:
	CTrackStatusDetector();
	~CTrackStatusDetector(){};

public:
	static CTrackStatusDetector& Instance();
	//初始化 LibUSB 
	void	Init( HWND hWnd);
	virtual	BOOL Start();

	virtual	DWORD DoJob();
	virtual	void Release();


protected:
	__int64 _GetCurrentTimeStamp();
	//_RecvData();

protected:
	HWND      m_hWnd;

	//以下三项是libusb 数据结构
	libusb_device_handle* usb_handle;
	usb_dev udev;
	libusb_device_descriptor udev_desc;

	BYTE m_bufStagePodRead[8];
	//x, y, z data
	unsigned char m_cStagePodBtn[2];
	unsigned short m_nStagePodState[3];

	BOOL m_bStagePodWorkState, m_bStagePodKeySelected;

	__int64  m_i64LastTimeStamp;    //上一次时间戳
};


