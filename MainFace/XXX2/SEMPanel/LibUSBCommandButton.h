#pragma once
#include <string>
using namespace std;
#include "Tools/CustomButton.h"
#include "LibUSB_Function.h"

class CCommandButton : public CCustomButton
{
	DECLARE_DYNAMIC(CCommandButton)

public:
	CCommandButton();
	virtual ~CCommandButton();

	void Initialize(libusb_device_handle* handle,usb_dev* dev,libusb_device_descriptor* desc);

	void SetCommand(string& strCommand){ m_strCommand = strCommand;}
	string  GetCommand() { return m_strCommand;}

	void ExecuteComand(byte dataSend[64],byte dataReceive[64]);

protected:
	string m_strCommand;
	libusb_device_handle* usb_handle;
	usb_dev* udev;
	libusb_device_descriptor* udev_desc;
};
