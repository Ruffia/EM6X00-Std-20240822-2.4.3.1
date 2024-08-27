#pragma once
#include "../GlobalComponent/StatusDetector.h"
#include "LibUSB_Function.h"

class CSEMPanelStatusDetector : public CStatusDetector
{
protected:
	CSEMPanelStatusDetector(void);
	~CSEMPanelStatusDetector(void){};

public:
	static CSEMPanelStatusDetector& Instance();
	//初始化 LibUSB 
	int Initialize( HWND hWnd);
	virtual DWORD DoJob();
	virtual void Release();

protected:
	__int64 _GetCurrentTimeStamp();
	//_RecvData();

protected:
	HWND      m_hWnd;

	//以下三项是libusb 数据结构
	libusb_device_handle* usb_handle;
	usb_dev udev;
	libusb_device_descriptor udev_desc;

	BYTE dataReceive[64];

	__int64  m_i64LastTimeStamp;    //上一次时间戳
};
