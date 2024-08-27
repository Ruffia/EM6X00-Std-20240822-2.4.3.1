#pragma once
#include <stdio.h>
#include <string.h>
#include "libusb.h"

#include <stdlib.h>
#include <iostream>
#include <stdarg.h>
using namespace std;
#include <windows.h>
//#include <unistd.h>
//#include <libusb-1.0/libusb.h>

#define BULK_RECV_EP    0x82
#define BULK_SEND_EP    0x02
#define INT_RECV_EP     0x81
#define INT_SEND_EP     0x01

#define TRANSFER_TIMEOUT 0


#define VID 0x0483
#define PID 0x572B


typedef struct {
	unsigned int pid;
	unsigned int vid;
	unsigned char bInterfaceClass;
	unsigned char bInterfaceSubClass;
	unsigned char bmAttributes;
	unsigned char bInEndpointAddress;
	unsigned char bOutEndpointAddress;
	unsigned char bInterfaceNumber;
	libusb_device *dev;
	libusb_device **devs;
} usb_dev;



/*
    根据PID与VID检查指定设备是否挂载。
*/
int get_device_descriptor(struct libusb_device_descriptor *dev_desc, usb_dev* user_device);

int match_with_endpoint(const struct libusb_interface_descriptor * interface, usb_dev* user_device);

int get_device_endpoint(struct libusb_device_descriptor *dev_desc, usb_dev* user_device);

