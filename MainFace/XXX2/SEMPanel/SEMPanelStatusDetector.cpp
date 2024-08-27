#include "StdAfx.h"
#include "SEMPanelStatusDetector.h"
#include "PanelDataProcessorBase.h"
#include "USB_Functions.h"

CSEMPanelStatusDetector& CSEMPanelStatusDetector::Instance()
{
	static CSEMPanelStatusDetector Inst;
	return Inst;
}


CSEMPanelStatusDetector::CSEMPanelStatusDetector(void)
{
	m_hWnd = NULL;
	m_dwSleep = 1;
}


int CSEMPanelStatusDetector::Initialize( HWND hWnd)
{
	usb_handle = NULL;
	//1. load user data.
	udev.vid = VID;
	udev.pid = PID;
	udev.bInterfaceClass = LIBUSB_CLASS_PER_INTERFACE;
	udev.bInterfaceSubClass = LIBUSB_CLASS_PER_INTERFACE;
	udev.bmAttributes = LIBUSB_TRANSFER_TYPE_BULK;
	udev.dev = NULL;

	char sz[512] = {0};
	ZeroMemory(sz,512);
	sprintf_s(sz,512,"CSEMPanelStatusDetector::Initialize \r\n" );
	OutputDebugString(sz);

	//2. init libusb.
	int ret = libusb_init(NULL);
	if(ret < 0)
	{
		ZeroMemory(sz,512);
		sprintf_s(sz,512,"CSEMPanelStatusDetector::Initialize,libusb_init Failed \r\n" );
		OutputDebugString(sz);

		return -1;
	}

	//3. search for specified usb device.
	ret = get_device_descriptor(&udev_desc, &udev);
	if(ret < 0) {
		ZeroMemory(sz,512);
		sprintf_s(sz,512,"CSEMPanelStatusDetector::Initialize,get_device_descriptor Failed \r\n" );
		OutputDebugString(sz);

		return -2;
	}

	ret = get_device_endpoint(&udev_desc, &udev);
	if(ret < 0) {
		ZeroMemory(sz,512);
		sprintf_s(sz,512,"CSEMPanelStatusDetector::Initialize,get_device_endpoint Failed \r\n" );
		OutputDebugString(sz);
		return -3;
	}

	/*4.open device and start communication by usb*/
	//open the usb device
	usb_handle = libusb_open_device_with_vid_pid(NULL, udev.vid, udev.pid);
	if(usb_handle == NULL) {
		ZeroMemory(sz,512);
		sprintf_s(sz,512,"CSEMPanelStatusDetector::Initialize,libusb_open_device_with_vid_pid Failed \r\n" );
		OutputDebugString(sz);
		return -4;
	}

	ret = libusb_claim_interface(usb_handle, udev.bInterfaceNumber);
	if(ret < 0) {
		ret = libusb_detach_kernel_driver(usb_handle, udev.bInterfaceNumber);
		if(ret < 0) {
			ZeroMemory(sz,512);
			sprintf_s(sz,512,"CSEMPanelStatusDetector::Initialize,libusb_detach_kernel_driver Failed \r\n" );
			OutputDebugString(sz);
			return -5;
		}
		ret = libusb_claim_interface(usb_handle, udev.bInterfaceNumber);
		if(ret < 0)
		{
			ZeroMemory(sz,512);
			sprintf_s(sz,512,"CSEMPanelStatusDetector::Initialize,libusb_claim_interface Failed \r\n" );
			OutputDebugString(sz);
			return -6;
		}
	}

	m_hWnd = hWnd;
	m_i64LastTimeStamp = _GetCurrentTimeStamp();
	return ret;
}


DWORD CSEMPanelStatusDetector::DoJob()
{
	char sz[512] = {0};
	__int64 i64CurTimeStamp = _GetCurrentTimeStamp();
	if ( i64CurTimeStamp - m_i64LastTimeStamp >= 50)
	{
		ZeroMemory(dataReceive,64);
		int rlen = 0;
		int ret = libusb_bulk_transfer(usb_handle, BULK_RECV_EP, dataReceive, 64, &rlen, TRANSFER_TIMEOUT);
		if (ret < 0)
		{
			//CString strErr;
			//strErr.Format("%s,%d",strCommand.c_str(),len2);
			//MessageBox(strErr);
			ZeroMemory(sz,512);
			sprintf_s(sz,512,"CSEMPanelStatusDetector::DoJob,libusb_bulk_transfer Failed, ret = %d \r\n",ret );
			OutputDebugString(sz);

			return -1L;
		}


		BYTE data[CmdLen] = {0};
		memcpy(data,dataReceive,CmdLen);
		const int nFunctionKey = data[FunctionCode];
		PostMessage(m_hWnd,WM_USER_ControlPanel_MSG,(WPARAM)dataReceive,0);

		m_i64LastTimeStamp = i64CurTimeStamp;
	}

	return 0L;
}


__int64 CSEMPanelStatusDetector::_GetCurrentTimeStamp()
{
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);	
	CTime curTime(sysTime);
	__int64	timeStamp = curTime.GetTime();
	__int64 curTimeStamp = timeStamp*1000 + sysTime.wMilliseconds;
	return curTimeStamp;
}


void CSEMPanelStatusDetector::Release()
{
	//6£¬×¢Ïú
	libusb_close(usb_handle);
	libusb_release_interface(usb_handle, udev.bInterfaceNumber);
	libusb_free_device_list(udev.devs, 1);
	libusb_exit(NULL);
	usb_handle = NULL;
	__super::Release();
}