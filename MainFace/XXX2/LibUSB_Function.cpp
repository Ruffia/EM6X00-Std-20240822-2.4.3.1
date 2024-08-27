#include "stdafx.h"
#include "LibUSB_Function.h"
/*
    根据PID与VID检查指定设备是否挂载。
*/
int get_device_descriptor(struct libusb_device_descriptor *dev_desc, usb_dev* user_device)
{
    int rv = -2;
    int i = 0;

    libusb_device **devs;
    libusb_device *dev;

    rv = libusb_get_device_list(NULL, &devs);
    if (rv < 0)
        return rv;

    //遍历USB设备
    while ((dev = devs[i++]) != NULL) {
        rv = libusb_get_device_descriptor(dev, dev_desc);
        if(rv == 0) {
        }
    }

    i = 0;
    while ((dev = devs[i++]) != NULL) {
        rv = libusb_get_device_descriptor(dev,dev_desc);
        if(rv < 0) {
            return -1;
        }

        if(dev_desc->idProduct == user_device->pid && dev_desc->idVendor == user_device->vid)
        {
            user_device->dev = dev;
            user_device->devs = devs;

            return 0;
        }
    }
    
    return -2;
}

int match_with_endpoint(const struct libusb_interface_descriptor * interface, usb_dev* user_device)
{
    int i;
    int ret=0;
    printf("bNumEndpoints:%d\n", interface->bNumEndpoints);
    for(i=0; i<interface->bNumEndpoints; i++)
    {
		int attribute = interface->endpoint[i].bmAttributes&0x03;
        if((interface->endpoint[i].bmAttributes&0x03 ) == user_device->bmAttributes) //transfer type :bulk ,control, interrupt
        {
            if(interface->endpoint[i].bEndpointAddress&0x80) //out endpoint & in endpoint
            {
               ret|=1;
               user_device->bInEndpointAddress = interface->endpoint[i].bEndpointAddress;
            }
            else
            {
               ret|=2;
               user_device->bOutEndpointAddress = interface->endpoint[i].bEndpointAddress;
            }
        }
     }

     if(ret==3 || ret == 1)
     {
       return 1;
     }
     else
     {
         return 0;
      }
}


int get_device_endpoint(struct libusb_device_descriptor *dev_desc, usb_dev* user_device)
{
    int rv = -2;
    int i,j,k;
    struct libusb_config_descriptor *conf_desc;
    unsigned char isFind = 0;
    
    for (i = 0; i < dev_desc->bNumConfigurations; i++)
    {
        if(user_device->dev != NULL)
            rv = libusb_get_config_descriptor(user_device->dev, i, &conf_desc);
        
        if(rv < 0) {
            return -1;
        }
        
        for (j = 0; j < conf_desc->bNumInterfaces; j++)
        {
            for (k=0; k < conf_desc->interface[j].num_altsetting; k++)
            {
				const uint8_t  bInterfaceClass = conf_desc->interface[j].altsetting[k].bInterfaceClass;
                if(bInterfaceClass == user_device->bInterfaceClass)
                {
                    if(match_with_endpoint(&(conf_desc->interface[j].altsetting[k] ), user_device))
                    {
                        user_device->bInterfaceNumber = conf_desc->interface[j].altsetting[k].bInterfaceNumber;
                        libusb_free_config_descriptor(conf_desc);
                        rv = 0;
                        return rv;
                    }
                }
            }
        }
    }
    
    return -2;  //don't find user device
}

