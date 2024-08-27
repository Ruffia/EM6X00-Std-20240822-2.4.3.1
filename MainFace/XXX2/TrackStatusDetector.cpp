#include "stdafx.h"
#include "TrackStatusDetector.h"

CTrackStatusDetector& CTrackStatusDetector::Instance()
{
	static CTrackStatusDetector Inst;
	return Inst;
}

CTrackStatusDetector::CTrackStatusDetector(void)
{
	m_hWnd		= NULL;
	m_dwSleep	= 5;
}

void CTrackStatusDetector::Init( HWND hWnd)
{
	m_hWnd		= hWnd;
	usb_handle	= NULL;
	m_bStagePodWorkState	= FALSE;
	m_bStagePodKeySelected	= FALSE;
	int i=0;
	for( i=0; i<2; i++ )
		m_cStagePodBtn[i] = 0;
	for( i=0; i<3; i++ )
		m_nStagePodState[i] = 0;
}

BOOL CTrackStatusDetector::Start()
{
	//1. load user data.
	udev.vid = StagePod_VID;
	udev.pid = StagePod_PID;
	udev.bInterfaceClass	= LIBUSB_CLASS_HID;//接口类型
	udev.bInterfaceSubClass	= LIBUSB_CLASS_HID;//接口子类型
	udev.bmAttributes = LIBUSB_TRANSFER_TYPE_INTERRUPT;	//bulk
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

	PostMessage( m_hWnd, WM_USER_StagePod_Update, 0, 1 );

	m_i64LastTimeStamp = _GetCurrentTimeStamp();
	BOOL bStart = __super::Start();
	return bStart;
}

__int64 CTrackStatusDetector::_GetCurrentTimeStamp()
{
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);	
	CTime curTime(sysTime);
	__int64	timeStamp = curTime.GetTime();
	__int64 curTimeStamp = timeStamp*1000 + sysTime.wMilliseconds;
	return curTimeStamp;
}

DWORD CTrackStatusDetector::DoJob()
{
	char sz[512] = {0};
	__int64 i64CurTimeStamp = _GetCurrentTimeStamp();
	//if ( i64CurTimeStamp - m_i64LastTimeStamp >= 5)
	{
		ZeroMemory(m_bufStagePodRead,8);
		int rlen = 0;
		libusb_clear_halt(usb_handle, 0);
		int ret = libusb_interrupt_transfer(usb_handle, 0x81, m_bufStagePodRead, sizeof(m_bufStagePodRead), &rlen, 0);
		if (ret < 0)
		{
			//ZeroMemory(sz,512);
			//sprintf_s(sz,512,"CSEMPanelStatusDetector::DoJob,libusb_bulk_transfer Failed, ret = %d \r\n",ret );
			//OutputDebugString(sz);
			return -1L;
		}

		// 0x0000-0x01FE: X向左,Y向下,Z逆时针
		// 0x0200:停止（实际使用中，有一定误差，所以判断条件不要太死）
		// 0x0200-0x03FF: X向右,Y向上,Z顺时针 
		m_nStagePodState[0] = (m_bufStagePodRead[1] << 8) | m_bufStagePodRead[0];
		m_nStagePodState[1] = (m_bufStagePodRead[3] << 8) | m_bufStagePodRead[2];
		m_nStagePodState[2] = (m_bufStagePodRead[5] << 8) | m_bufStagePodRead[4];
		m_cStagePodBtn[0] = ((m_bufStagePodRead[6] & 0x01) == 0x01);	// bit0:1键
		m_cStagePodBtn[1] = ((m_bufStagePodRead[6] & 0x02) == 0x02);	// bit1:2键

		//g_btn1 > 0,则该按键被按下，等于0则该按键没有被按下.
		if( m_cStagePodBtn[0] != 0 )
			m_bStagePodKeySelected = FALSE;
		else if( m_cStagePodBtn[1] != 0 )
			m_bStagePodKeySelected = TRUE;

		if(m_bStagePodKeySelected)
		{
			if( m_nStagePodState[0] > StagePod_Stop +StagePod_Offset_Value -1 || m_nStagePodState[0] < StagePod_Stop -StagePod_Offset_Value +1
				|| m_nStagePodState[1] > StagePod_Stop +StagePod_Offset_Value -1 || m_nStagePodState[1] < StagePod_Stop -StagePod_Offset_Value +1 )
			{
				// 2键按下：Z & T
				m_bStagePodWorkState = TRUE;
				PostMessage( m_hWnd, WM_USER_StagePod_Update, 2, (LPARAM)m_nStagePodState);
			}
			if( m_nStagePodState[0] > StagePod_Stop -StagePod_Offset_Value && m_nStagePodState[0] < StagePod_Stop +StagePod_Offset_Value
				&& m_nStagePodState[1] > StagePod_Stop -StagePod_Offset_Value && m_nStagePodState[1] < StagePod_Stop +StagePod_Offset_Value )
			{
				if(m_bStagePodWorkState)
				{
					m_bStagePodWorkState = FALSE;
					PostMessage( m_hWnd, WM_USER_StagePod_Update, 100, 0 );
				}
			}
		}
		else
		{
			if( m_nStagePodState[0] > StagePod_Stop +StagePod_Offset_Value -1 || m_nStagePodState[0] < StagePod_Stop -StagePod_Offset_Value +1
				|| m_nStagePodState[1] > StagePod_Stop +StagePod_Offset_Value -1 || m_nStagePodState[1] < StagePod_Stop -StagePod_Offset_Value +1
				|| m_nStagePodState[2] > StagePod_Stop +StagePod_Offset_Value -1 || m_nStagePodState[2] < StagePod_Stop -StagePod_Offset_Value +1 )
			{
				// 1键按下：X & Y & R
				m_bStagePodWorkState = TRUE;
				PostMessage( m_hWnd, WM_USER_StagePod_Update, 1, (LPARAM)m_nStagePodState);
			}
			if( m_nStagePodState[0] > StagePod_Stop -StagePod_Offset_Value && m_nStagePodState[0] < StagePod_Stop +StagePod_Offset_Value
				&& m_nStagePodState[1] > StagePod_Stop -StagePod_Offset_Value && m_nStagePodState[1] < StagePod_Stop +StagePod_Offset_Value
				&& m_nStagePodState[2] > StagePod_Stop -StagePod_Offset_Value && m_nStagePodState[2] < StagePod_Stop +StagePod_Offset_Value )
			{
				if(m_bStagePodWorkState)
				{
					m_bStagePodWorkState = FALSE;
					PostMessage( m_hWnd, WM_USER_StagePod_Update, 100, 0 );
				}
			}
		}
		m_i64LastTimeStamp = i64CurTimeStamp;
	}
	return 0L;
}

void CTrackStatusDetector::Release()
{
	libusb_close(usb_handle);
	libusb_release_interface(usb_handle, udev.bInterfaceNumber);
	libusb_free_device_list(udev.devs, 1);
	libusb_exit(NULL);
	usb_handle = NULL;

	__super::Release();
}

