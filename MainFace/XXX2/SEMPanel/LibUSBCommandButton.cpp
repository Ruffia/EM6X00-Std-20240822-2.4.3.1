#include "StdAfx.h"
#include "LibUSBCommandButton.h"


IMPLEMENT_DYNAMIC(CCommandButton, CCustomButton)
CCommandButton::CCommandButton():usb_handle(NULL),udev(NULL),udev_desc(NULL)
{

}

CCommandButton::~CCommandButton()
{

}


void CCommandButton::Initialize(libusb_device_handle* handle,usb_dev* dev,libusb_device_descriptor* desc)
{
	usb_handle = handle;
	udev = dev;
	udev_desc = desc;
}


void CCommandButton::ExecuteComand(byte dataSend[64],byte dataReceive[64])
{
	string strCommand = GetCommand();
	if (strCommand == "libusb_init")
	{
		int ret = libusb_init(NULL);
		if (ret < 0)
		{
			CString strErr;
			strErr.Format("%s,%d",strCommand.c_str(),ret);
			MessageBox(strErr);
			return;
		}
		setWordColor(RGB(255,0,0));
	}
	else if(strCommand == "get_device_descriptor")
	{
		int ret = get_device_descriptor(udev_desc, udev);
		if (ret < 0)
		{
			CString strErr;
			strErr.Format("%s,%d",strCommand.c_str(),ret);
			MessageBox(strErr);
			return;
		}
		setWordColor(RGB(255,0,0));
	}
	else if (strCommand == "get_device_endpoint")
	{
		int ret = get_device_endpoint(udev_desc, udev);
		if (ret < 0)
		{
			CString strErr;
			strErr.Format("%s,%d",strCommand.c_str(),ret);
			MessageBox(strErr);
			return;
		}
		setWordColor(RGB(255,0,0));
	}
	else if(strCommand == "libusb_open_device_with_vid_pid")
	{
		usb_handle = libusb_open_device_with_vid_pid(NULL, udev->vid, udev->pid);
		if(usb_handle == NULL) {
			CString strErr;
			strErr.Format("%s,%d",strCommand.c_str(),GetLastError());
			MessageBox(strErr);
			return;
		}
		setWordColor(RGB(255,0,0));
	}
	else if(strCommand == "libusb_claim_interface")
	{
		int ret = libusb_claim_interface(usb_handle, udev->bInterfaceNumber);
		if(ret < 0) 
		{
			ret = libusb_detach_kernel_driver(usb_handle, udev->bInterfaceNumber);
			if(ret < 0) {
				CString strErr;
				strErr.Format("%s,%d","libusb_detach_kernel_driver",ret);
				MessageBox(strErr);
				return;
			}
			ret = libusb_claim_interface(usb_handle, udev->bInterfaceNumber);
			if(ret < 0)
			{
				CString strErr;
				strErr.Format("%s,%d","libusb_claim_interface",ret);
				MessageBox(strErr);
				return;
			}
		}
		setWordColor(RGB(255,0,0));

	}
	else if (strCommand == "libusb_bulk_transfer_send")
	{
		int wlen = 0;
		int len1 = libusb_bulk_transfer(usb_handle, BULK_SEND_EP, dataSend, 64, &wlen, TRANSFER_TIMEOUT);
		if (len1 < 0)
		{
			CString strErr;
			strErr.Format("%s,%d",strCommand.c_str(),len1);
			MessageBox(strErr);
		}
		return;
	}
	else if (strCommand == "libusb_bulk_transfer_receive")
	{
		int rlen = 0;
		int len2 = libusb_bulk_transfer(usb_handle, BULK_RECV_EP, dataReceive, 64, &rlen, TRANSFER_TIMEOUT);
		if (len2 < 0)
		{
			CString strErr;
			strErr.Format("%s,%d",strCommand.c_str(),len2);
			MessageBox(strErr);
		}
		return;
	}

}