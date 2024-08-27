
#include "stdafx.h"

extern "C" {   

// This file is in the Windows DDK available from Microsoft.
#include "hidsdi.h"			// Project根目录下的头文件

#include <setupapi.h>		// VC安装目录下的头文件
}

/*
' This module is gives Visual Basic programs access to the Windows API
'
' Copyright 2005, John Hyde, USB Design By Example
' You may use this program for development but you may not sell or publish it
' without written permission from the author
*/
/*
const GENERIC_READ = &H80000000
const GENERIC_WRITE = &H40000000
const FILE_SHARE_NONE = 0
const FILE_SHARE_READWRITE = 3
const OPEN_EXISTING = 3
const FILE_FLAG_OVERLAPPED = &H40000000
const WAIT_TIMEOUT = &H102
const DIGCF_PRESENT = &H2
const DIGCF_DEVICEINTERFACE = &H10
*/

SECURITY_ATTRIBUTES		SA;

HANDLE MCB_OpenForRead( CString strFileName )
{
	// Open a device that has already been identified as present
	// The device only sends reports on button changes, therefore I must use overlapped IO
	return CreateFile(	strFileName,
						GENERIC_READ,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						&SA,
						OPEN_EXISTING,
						FILE_FLAG_OVERLAPPED,
						0	);
}

HANDLE MCB_OpenForWrite( CString strFileName )
{
	// Open a device that has already been identified as present
	// I need a write handle that is not overlapped
	return CreateFile(	strFileName,
						GENERIC_WRITE,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						&SA,
						OPEN_EXISTING,
						0,
						0	);
}

void MCB_ErrorExit( CString strReason )
{
	DWORD dwErrorCode = GetLastError();
	CString str;
	str.Format( "%s ErrorCode = [ %ld ]", strReason, dwErrorCode );
	MessageBox( NULL, str, "Error Exit", MB_OK|MB_ICONERROR );

//	If (ErrorCode <> 0) Then Stop
//	if( dwErrorCode != 0 )
//		ExitProcess(dwErrorCode); 
}
/*
例如，Disk类的GUID为“53f56307-b6bf-11d0-94f2-00a0c91efb8b”，在我们的程序里可以定义为 

const GUID DiskClassGuid = {0x53f56307L, 0xb6bf, 0x11d0, {0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b)};

或者用一个宏来定义 

DEFINE_GUID(DiskClassGuid, 0x53f56307L, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);
*/

// USB类的GUID为{36FC9E60-C465-11CF-8056-444553540000}
//const GUID GUID_DEVINTERFACE_USB_DEVICE = 
//{ 0x36FC9E60L, 0xC465, 0x11CF, { 0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } };

const GUID GUID_DEVINTERFACE_USB_DEVICE = 
{ 0xf18a0e88, 0xc30c, 0x11d0, { 0x88, 0x15, 0x00, 0xa0, 0xc9, 0x06, 0xbe, 0xd8 } };
//DEFINE_GUID(GUID_DEVINTERFACE_USB_DEVICE, 0x36FC9E60L, 0xC465, 0x11CF, 0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00);




/*
USB specific GUIDs
*/

/*
// f18a0e88-c30c-11d0-8815-00a0c906bed8 //
DEFINE_GUID(GUID_DEVINTERFACE_USB_HUB, 0xf18a0e88, 0xc30c, 0x11d0, 0x88, 0x15, 0x00, \
0xa0, 0xc9, 0x06, 0xbe, 0xd8);

// A5DCBF10-6530-11D2-901F-00C04FB951ED //
DEFINE_GUID(GUID_DEVINTERFACE_USB_DEVICE, 0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, \
0xC0, 0x4F, 0xB9, 0x51, 0xED);

// 3ABF6F2D-71C4-462a-8A92-1E6861E6AF27 //
DEFINE_GUID(GUID_DEVINTERFACE_USB_HOST_CONTROLLER, 0x3abf6f2d, 0x71c4, 0x462a, 0x8a, 0x92, 0x1e, \
0x68, 0x61, 0xe6, 0xaf, 0x27);

// 4E623B20-CB14-11D1-B331-00A0C959BBD2 //
DEFINE_GUID(GUID_USB_WMI_STD_DATA, 0x4E623B20L, 0xCB14, 0x11D1, 0xB3, 0x31, 0x00,\
0xA0, 0xC9, 0x59, 0xBB, 0xD2);

// 4E623B20-CB14-11D1-B331-00A0C959BBD2 //
DEFINE_GUID(GUID_USB_WMI_STD_NOTIFICATION, 0x4E623B20L, 0xCB14, 0x11D1, 0xB3, 0x31, 0x00,\
0xA0, 0xC9, 0x59, 0xBB, 0xD2); 
*/

/*
Obsolete device interface class GUID names.
(use of above GUID_DEVINTERFACE_* names is recommended).
--*/
/*
#define GUID_CLASS_USBHUB GUID_DEVINTERFACE_USB_HUB
#define GUID_CLASS_USB_DEVICE GUID_DEVINTERFACE_USB_DEVICE
#define GUID_CLASS_USB_HOST_CONTROLLER GUID_DEVINTERFACE_USB_HOST_CONTROLLER

#define FILE_DEVICE_USB FILE_DEVICE_UNKNOWN
*/



CString MCB_FindHIDInterface( int nVID, int nPID, int nUsagePage )
{
/*
1.调用SetupDiGetClassDevs.这一部正常获得句柄

2.调用SetupDiEnumDeviceInterfaces,枚举设备.在调用时无论如何返回值也是false.

3.两次调用SetupDiGetDeviceInterfaceDetail,获取设备详细信息,设备名.

4.调用CreateFile,打开设备句柄.
*/
	// This function searches the system HID tables for a device matching VID, PID and UsagePage
	// If found then it returns the system name for the device (for a later open)
	GUID					guidHid;// = GUID_DEVINTERFACE_USB_DEVICE;
	HDEVINFO				hPnPHandle;
	HANDLE					hHidHandle;
	DWORD					dwHidEntry;
	CString					strHidName, strNameMatch;
	BOOL					bSuccess, bFound;
	SP_DEVICE_INTERFACE_DATA			DeviceInterfaceData;
//	SP_DEVICE_INTERFACE_DETAIL_DATA		DeviceInterfaceDetailData;
	HIDD_ATTRIBUTES			Attributes;
	PHIDP_PREPARSED_DATA	PreparsedData;
	HIDP_CAPS				HidCapabilities;
//	DWORD					dwBytesReturned, dwTemp;

	// Clear the name, return this empty string if the device is not found
	strNameMatch = "";
	// First, get the HID class identifier
	HidD_GetHidGuid(&guidHid);

	// Get a handle for the Plug and Play node, request currently active HID devices
	hPnPHandle = SetupDiGetClassDevs(&guidHid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	if (hPnPHandle == INVALID_HANDLE_VALUE)
		MCB_ErrorExit ("Could not attach to PnP node");

	dwHidEntry	= 0;
	bFound		= FALSE;
	ZeroMemory(&DeviceInterfaceData, sizeof(SP_DEVICE_INTERFACE_DATA));
	DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	// Step through the table of HID devices
	while (SetupDiEnumDeviceInterfaces(hPnPHandle, NULL, &guidHid, dwHidEntry, &DeviceInterfaceData) && !bFound)
	{	
		//第一次调用出错，但可以返回正确的Size
		PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData;
		DWORD strsize=0, requiresize=0;
		SetupDiGetDeviceInterfaceDetail(hPnPHandle,&DeviceInterfaceData,NULL,0,&strsize,NULL);

		requiresize = strsize;
		DeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(requiresize);
		DeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		//第二次调用传递返回值，调用即可成功
		SP_DEVINFO_DATA infodata;
		infodata.cbSize=sizeof(SP_DEVINFO_DATA);
		bSuccess = SetupDiGetDeviceInterfaceDetail(hPnPHandle,&DeviceInterfaceData,
													DeviceInterfaceDetailData,strsize,&requiresize,NULL);
		if (!bSuccess)
		{
			free(DeviceInterfaceDetailData);
			MCB_ErrorExit ("Could not get the name of this HID device");
			SetupDiDestroyDeviceInfoList ( hPnPHandle );
			return strNameMatch;
		}

//		CString ctemp;
//		ctemp = DeviceInterfaceDetailData->DevicePath;
//		*DevicePath=ctemp;                                                    //返回设备路径,以便打开设备时使用
//		*PUID = ctemp.Mid(32, 16);                         //保留设备PUID号,以便查看该号是否存在
//		free(DeviceInterfaceDetailData);


		// Convert returned C string to Visual Basic String
/*		strHidName = "";
		i = 0;
		while (DeviceInterfaceDetailData->DevicePath[i] != 0 )
		{
			strHidName = strHidName + DeviceInterfaceDetailData.DevicePath[i];
			i++;
		}	// Loop
*/		strHidName = DeviceInterfaceDetailData->DevicePath;

		// Can now open this HID device for inspection
		hHidHandle = CreateFile(strHidName, 0, FILE_SHARE_READ|FILE_SHARE_WRITE, &SA, OPEN_EXISTING, 0, 0);
		if (hHidHandle == INVALID_HANDLE_VALUE)
		{
			free(DeviceInterfaceDetailData);
			MCB_ErrorExit ("Could not open HID device");
			SetupDiDestroyDeviceInfoList ( hPnPHandle );
			return strNameMatch;
		}
		// Is it OUR HID device?
		if ( HidD_GetAttributes(hHidHandle, &Attributes) )
		{
			if ((Attributes.VendorID == nVID) && (Attributes.ProductID == nPID))
			{
				// Is it the correct interface?
				bSuccess = HidD_GetPreparsedData(hHidHandle, &PreparsedData);
				if (bSuccess)
				{
					bSuccess = HidP_GetCaps(PreparsedData, &HidCapabilities);
					if (bSuccess)
					{
						if (HidCapabilities.UsagePage == nUsagePage)
						{
							bFound = TRUE;
							strNameMatch = strHidName;
						}
					}
				}
			}
			CloseHandle( hHidHandle );
		}// 'HidD_GetAttributes
		dwHidEntry++;	// Check next entry
		free(DeviceInterfaceDetailData);
	}	//Loop	// SetupDiEnumDeviceInterfaces returns FALSE when there are no more entries
	SetupDiDestroyDeviceInfoList ( hPnPHandle );
	return strNameMatch;

}

