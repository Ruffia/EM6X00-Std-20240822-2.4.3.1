//************************* c3601.c ****************************
//* This file contains DLL for CPS serial port controlled
//* power supplies.
//* Written by Alexander Kostromitin
//* Copyright (C) CPS, 2006
//*
//* Compile and link this file with ..\serial\serial.c
//**************************************************************


#include <time.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <commdlg.h>    // includes common dialog functionality
#include <commctrl.h>
#include <dlgs.h>       // includes common dialog template defines
#include <cderr.h>      // includes the common dialog error codes
#include <io.h>
#include <math.h>


#include "..\serial\serial.h"
#include "c3601.h"

#define __libfunc__     __declspec (dllexport)

#define COM_TIMEOUT     100
#define LIM_0_1(v)      ((v) < 0 ? 0 : (v) > 1 ? 1 : (v))
/*
struct param
  {
  // setpoint: used to convert required voltage to output DAC value.
  double k0, k1, k2;     // linearization coefficients for voltage
  // internal meter: used to convert ADC values to actual current.
  double iscale_a;       // output current scale, uA
  double iscale_e;       // output current scale, uA
  double iscale_f;       // output current scale, mA
  // used for both: setpoint and internal meter.
  double vscale_a;       // output voltage scale, V
  double vscale_e;       // output voltage scale, V
  double vscale_f;       // output voltage scale, V
  double tscale;         // output temperatue scale, C
  };
*/

#define CH_V        0       // Voltage channel.
#define CH_I        1       // Current channel.
#define CH_T        2       // Temperature channel.

////////////////////// Function prototypes //////////////////////////

int selectdev(int port, int dev);
int sendpacket(int port, unsigned char *buf, int len);
int recpacket(int port, unsigned char *buf, int len);
int getmeter(int port, int dev, int ch);
int getraw(int port, int dev, int ch);
int getalarm(int port);

void error(char *fmt, ... );


//////////////////////////// globals ////////////////////////////////

static char errorstr[256];


// Coefficients to convert between DAC/ADC and physical values:
// physical_value = corr * converter_value (or converter_value = physical_value / corr).

double iconv_a    = 5.0 / 0.0005 / 10e3 / 0x7fffff;                     // 5V ref / 0.0005A scale / 10kOhm (23 bits)
double iconv_e    = 5.0 / 0.0005 / 10e3 / 0x7fffff;                     // 5V ref / 0.0005A scale / 10kOhm (23 bits)
double iconv_f_s  = 5.0 / 5 / ((50.5 / 1.2 + 1) * 0.02) / 0xffffff;    // 5V ref / 5A scale / 0.02 Ohm / amp_gain (24 bits)
double iconv_f    = 5.0 / 5 / ((50.5 / 1.2 + 1) * 0.02) / 0x7fffff;    // 5V ref / 5A scale / 0.02 Ohm / amp_gain (23 bits)
double vconv_a_s  = 5.0 / 30000 * (50000.0 / 10.05) / (9.614 / 19.614) * 0.99 / 0xffffff;  // 5V ref * HV_div / amp_gain * default_SP_gain (24 bits)
double vconv_a    = 5.0 / 30000 * (50000.0 / 10.05) / (9.614 / 19.614) / 0x7fffff; // 5V ref * HV_div / amp_gain (23 bits)
double vconv_e_s  = 5.0 / 10000 * (30000.0 / 10.05) / (10.0 / 34.8 + 1) / 0xffffff;  // 5V ref * HV_div / amp_gain (24 bits)
double vconv_e    = 5.0 / 10000 * (30000.0 / 10.05) / (10.0 / 34.8 + 1) / 0x7fffff;  // 5V ref * HV_div / amp_gain (23 bits)
double vconv_f    = 5.0 / 5.0 / 0x7fffff;                               // 5V ref / 5V scale (23 bits)
double tconv_a    = 1.0 / 4;                                            // 1/4C digital scale
double tconv_e    = 2.5 / 0.01 / 0x7fff;                                // 2.5V ref / 0.01(V/C) (15 bits)

static int status[MAX_PORTS];

DCB dcb[MAX_PORTS];             // The DCB structure defines the control setting for a serial communications device.
static int openflag[MAX_PORTS]; // Port open flag.

//////////////////////////////////////////////////////////////////
//Exported functions
//////////////////////////////////////////////////////////////////

//////// Initialization, shutdown and status.

// Open port. Port 0 is COM1, port 1 is COM2, etc.
// Return: 0 if OK or error code.
__libfunc__ int __stdcall C3601_OpenPort(int port, int baudrate)
{
  int err;

  dcb[port].BaudRate          = baudrate; // baud rate
  dcb[port].fBinary           = 1;        // binary mode, no EOF check
  dcb[port].fParity           = 0;        // enable parity checking
  dcb[port].fOutxCtsFlow      = 0;        // CTS output flow control
  dcb[port].fOutxDsrFlow      = 0;        // DSR output flow control
  dcb[port].fDtrControl       = DTR_CONTROL_DISABLE;        // DTR flow control type
  dcb[port].fDsrSensitivity   = 0;        // DSR sensitivity

  dcb[port].fTXContinueOnXoff = 0;        // XOFF continues Tx
  dcb[port].fOutX             = 0;        // XON/XOFF out flow control
  dcb[port].fInX              = 0;        // XON/XOFF in flow control
  dcb[port].fErrorChar        = 0;        // enable error replacement
  dcb[port].fNull             = 0;        // enable null stripping
  dcb[port].fRtsControl       = RTS_CONTROL_DISABLE;        // RTS flow control
  dcb[port].fAbortOnError     = 0;        // abort reads/writes on error

  dcb[port].XonLim            = 2048;     // transmit XON threshold
  dcb[port].XoffLim           = 512;      // transmit XOFF threshold
  dcb[port].ByteSize          = 8;        // number of bits/byte, 4-8
  dcb[port].Parity            = NOPARITY;        // 0-4=no,odd,even,mark,space
  dcb[port].StopBits          = ONESTOPBIT;        // 0,1,2 = 1, 1.5, 2
  dcb[port].XonChar           = 0x11;     // Tx and Rx XON character
  dcb[port].XoffChar          = 0x13;     // Tx and Rx XOFF character
  dcb[port].ErrorChar         = 0;        // error replacement character

  dcb[port].EofChar           = 0;        // end of input character
  dcb[port].EvtChar           = 0;        // received event character

  err = OpenCom(port, &dcb[port], COM_TIMEOUT, 0);
  openflag[port] = !err;
  return err;
}


// Close port. Port 0 is COM1, port 1 is COM2, etc.
__libfunc__ void __stdcall C3601_ClosePort(int port)
{
  CloseCom(port);
}


// Read n bytes from a COM port to buf.
// In binary mode (DCB.fBinary==1) may return '\0's !!!
// Returns 0 on if OK or error code; actual number of bytes read stored in nbytes.
__libfunc__ int __stdcall C3601_ReadPort(int port, int dev, unsigned char *buf, int *nbytes)
{
  selectdev(port, dev);
  return ReadCom(port, buf, nbytes);
}


// Write n bytes to a COM port from buf.
// Return: 0 if OK or error code.
__libfunc__ int __stdcall C3601_WritePort(int port, int dev, unsigned char *buf, int nbytes)
{
  selectdev(port, dev);
  return WriteCom(port, buf, nbytes);
}


// Get firmware version.
// Return: version number or error code (negative).
__libfunc__ int __stdcall C3601_GetVersion(int port, int dev)
{
  unsigned char buf[4];
  int err;

  selectdev(port, dev);
  buf[0] = 0xf3;
  err = sendpacket(port, buf, 1);
  if(err)
    {
    error("<sendpacket> error 0x%08x in line %d", err, __LINE__);
    return err;
    }
  err = recpacket(port, buf, 2);
  if(err)
    {
    error("<recpacket> error 0x%08x in line %d", err, __LINE__);
    return err;
    }
  return(buf[0] << 8) | buf[1];
}


// Set watchdog interval in ms units; 0-disable.
// Return: 0 if OK or error code.
__libfunc__ int __stdcall C3601_SetWD(int port, int dev, int msec)
{
  unsigned char buf[4];
  int err;

  msec = msec == 0 ? 0 : msec < 10 ? 1 : msec / 10;

  selectdev(port, dev);
  buf[0] = 0xf0;
  buf[1] = (unsigned char)msec;
  err = sendpacket(port, buf, 2);
  if(err)
    {
    error("<sendpacket> error 0x%08x in line %d", err, __LINE__);
    return err;
    }
  err = recpacket(port, buf, 0);
  if(err)
    {
    error("<recpacket> error 0x%08x in line %d", err, __LINE__);
    return err;
    }
  return 0;
}


// Get all detected non-fatal fault flags (ORed). Clear Reset and ComErr flags.
// Return: flags or error code (negative).
__libfunc__ int __stdcall C3601_GetFaultFlags(int port, int dev)
{
  unsigned char buf[4];
  int err, res;

  selectdev(port, dev);
  buf[0] = 0xfc;
  err = sendpacket(port, buf, 1);
  if(err)
    {
    error("<sendpacket> error 0x%08x in line %d", err, __LINE__);
    return err;
    }
  err = recpacket(port, buf, 1);
  if(err)
    {
    error("<recpacket> error 0x%08x in line %d", err, __LINE__);
    return err;
    }
  res = buf[0];

  // Clear Reset and ComErr flags.
  buf[0] = 0xf8;
  err = sendpacket(port, buf, 1);
  if(err)
    {
    error("<sendpacket> error 0x%08x in line %d", err, __LINE__);
    return err;
    }
  err = recpacket(port, buf, 0);
  if(err)
    {
    error("<recpacket> error 0x%08x in line %d", err, __LINE__);
    return err;
    }
  return res;

}


// Get all status flags (ORed), returned by last read from particular port.
// Return: flags or error code (negative).
__libfunc__ int __stdcall C3601_GetStatusFlags(int port)
{
  if(port < 0 || port >= MAX_PORTS)
    return ERR_INVALID_PORT;
  return status[port];
}


//////// Get and set values.

// Get output voltage.
// err: pointer to returned error code; ignore if NULL.
// Return: output voltage in Volts.
__libfunc__ double __stdcall C3601_GetVoltage(int port, int dev, int *err)
{
  int raw;
  double res;

  selectdev(port, dev);
  raw = getraw(port, dev, CH_V);
  if(raw<0)
    {
    error("<getraw> error 0x%08x in line %d", raw, __LINE__);
    *err=raw;
    return 0;
    }
  if(raw & 0x800000)                    // out of range
    {
    if(raw & 0x400000)                  // negative
      {
      raw |= 0xff000000;                // convert to 32bit negative
      if(err != NULL)
        *err = ERR_ADC_NEGATIVE;
      }
    else                                // negative
      {
      if(err != NULL)
        *err = ERR_ADC_OVERFLOW;
      }
    }
  else if(err != NULL)
    *err = 0;
  res = (double)raw * (dev == DEV_A ? vconv_a : dev == DEV_E ? vconv_e : vconv_f);
  return res;
}


// Get output current.
// err: pointer to returned error code; ignore if NULL.
// Return: output current in Ampers.
__libfunc__ double __stdcall C3601_GetCurrent(int port, int dev, int *err)
{
  int raw;
  double res;

  selectdev(port, dev);
  raw = getraw(port, dev, CH_I);
  if(raw<0)
    {
    error("<getraw> error 0x%08x in line %d", raw, __LINE__);
    *err=raw;
    return 0;
    }
  if(raw & 0x800000)                    // out of range
    {
    if(raw & 0x400000)                  // negative
      {
      raw |= 0xff000000;                // convert to 32bit negative
      if(err != NULL)
        *err = ERR_ADC_NEGATIVE;
      }
    else                                // negative
      {
      if(err != NULL)
        *err = ERR_ADC_OVERFLOW;
      }
    }
  else if(err != NULL)
    *err = 0;
  res = (double)raw * (dev == DEV_A ? iconv_a : dev == DEV_E ? iconv_e : iconv_f);
  return res;
}


// Get internal temperature.
// err: pointer to returned error code; ignore if NULL.
// Return: internal temperature in Celsius degrees.
__libfunc__ double __stdcall C3601_GetTempearture(int port, int dev, int *err)
{
  int raw;
  double res;

  selectdev(port, dev);
  raw = getmeter(port, dev, CH_T);
  if(raw < 0)
    {
    if(err != NULL)
      *err = raw;
    return 0;
    }
  if(err != NULL)
    *err = 0;

  res = dev == DEV_A ? (double)(raw >> 5) * tconv_a : (double)(raw & 0x7fff) * tconv_e;
  return res;
}


// Set output voltage in Volts and <enab> flag.
// Return: 0 if OK or error code.
__libfunc__ int __stdcall C3601_SetValue(int port, int dev, double v, int enab)
{
  unsigned char buf[4];
  int iv, err;

  selectdev(port, dev);

  //v = LIM_0_1(v);

  v /= (dev == DEV_A ? vconv_a_s : dev == DEV_E ? vconv_e_s : iconv_f_s);

  iv = v + 0.5;
  if(iv > 0xffffff)
    iv = 0xffffff;
  buf[0] = (unsigned char)((dev == DEV_F ? 0x20 : 0x10) | (enab ? 1 : 0));
  buf[1] = (unsigned char)(iv >> 16);
  buf[2] = (unsigned char)(iv >> 8);
  buf[3] = (unsigned char)iv;
  err = sendpacket(port, buf, 4);
  if(err)
    {
    error("<sendpacket> error 0x%08x in line %d", err, __LINE__);
    return err;
    }
  err = recpacket(port, buf, 0);
  if(err)
    {
    error("<recpacket> error 0x%08x in line %d", err, __LINE__);
    return err;
    }
  return 0;
}


// Reset: type 0 is soft reset, type 1 is hard reset.
// Return: 0 if OK or error code.
__libfunc__ int __stdcall C3601_Reset(int port, int dev, int type)
{
  unsigned char buf[4];
  int err;

  selectdev(port, dev);
  C3601_SetValue(port, dev, 0, 0);
  Sleep(500);

  buf[0] = (unsigned char)(type == 0 ? 0xf4 : 0xf5);
  err = sendpacket(port, buf, 1);
  if(err)
      return err;
  Sleep(100);
  FlushCom(port);
  return 0;
}


//////// Get and set EEPROM.

// Get one byte from EEPROM.
// Return: byte read or error code (negative).
__libfunc__ int __stdcall C3601_GetEEPROM(int port, int dev, int address)
{
  unsigned char buf[4];
  int err;

  if(address < 0 || address > 0x7f)
    return ERR_INVALID_VALUE;

  selectdev(port, dev);
  buf[0] = 0xf2;
  buf[1] = (unsigned char)address;
  err = sendpacket(port, buf, 2);
  if(err)
    {
    error("<sendpacket> error 0x%08x in line %d", err, __LINE__);
    return err;
    }
  err = recpacket(port, buf, 1);
  if(err)
    {
    error("<recpacket> error 0x%08x in line %d", err, __LINE__);
    return err;
    }
  return buf[0];
}


// Set one byte to EEPROM.
// Return: 0 if OK or error code.
__libfunc__ int __stdcall C3601_SetEEPROM(int port, int dev, int address, int value)
{
  unsigned char buf[4];
  int err;

  if(address < 0 || address > 0x7f)
    return ERR_INVALID_VALUE;

  selectdev(port, dev);
  buf[0] = 0xf1;
  buf[1] = (unsigned char)address;
  buf[2] = (unsigned char)value;
  err = sendpacket(port, buf, 3);
  if(err)
    {
    error("<sendpacket> error 0x%08x in line %d", err, __LINE__);
    return err;
    }
  Sleep(100);       // wait for write completed

  err = recpacket(port, buf, 0);
  if(err)
    {
    error("<recpacket> error 0x%08x in line %d", err, __LINE__);
    return err;
    }
  return 0;
}


// Retrive status of Alarm line.
// Return: status of Alarm line.
__libfunc__ int __stdcall C3601_GetAlarm(int port)
{
  return getalarm(port);
}


// Copy internal error message string to user's buffer <buf> with <maxlen> bytes
// lenth and clear the internal error message string.
// Return: actual lenth of string.
__libfunc__ int __stdcall C3601_GetErrorString(char *buf, int maxlen)
{
  int len;

  strncpy(buf, errorstr, min(maxlen, sizeof(errorstr)));
  len = min(strlen(errorstr), maxlen);
  errorstr[0] = 0;
  return len;
}


////////////////////// Internal functions //////////////////////////

// select control device: Accelerator, Extractor or Filament
int selectdev(int port, int dev)
{
  dcb[port].fDtrControl = dev == DEV_A ? DTR_CONTROL_DISABLE : DTR_CONTROL_ENABLE;
  dcb[port].fRtsControl = dev == DEV_A ? RTS_CONTROL_DISABLE : RTS_CONTROL_ENABLE;
  return SetComState(port, &dcb[port]);
}


// get alarm line (DSR) status
int getalarm(int port)
{
  int res;

  res = GetComStatus(port);
  if(res & MS_RLSD_ON)
    return 1;
  return 0;
}


// get one value from internal meter: T only
int getmeter(int port, int dev, int ch)
{
  unsigned char buf[4];
  int res, err;

  if(dev != DEV_A && dev != DEV_E && dev != DEV_F || ch != CH_T)
    return ERR_INVALID_VALUE;

  buf[0] = (unsigned char)ch;
  err = sendpacket(port, buf, 1);
  if(err)
    {
    error("<sendpacket> error 0x%08x in line %d", err, __LINE__);
    return err;
    }
  err = recpacket(port, buf, 2);
  if(err)
    {
    error("<recpacket> error 0x%08x in line %d", err, __LINE__);
    return err;
    }
  res = (buf[0] << 8) | buf[1];
  return res;
}


// get raw data: v or i
int getraw(int port, int dev, int ch)
{
  unsigned char buf[4];
  int err;

  if(dev != DEV_A && dev != DEV_E && dev != DEV_F || ch != CH_V && ch != CH_I)
    return ERR_INVALID_VALUE;

  if(dev == DEV_A)
    buf[0] = (unsigned char)(0xf9 + ch);
  else if(dev == DEV_E)
    buf[0] = ch == CH_V ? 0xe0 : 0xe1;
  else
    buf[0] = ch == CH_V ? 0xe2 : 0xe3;

  err = sendpacket(port, buf, 1);
  if(err)
    {
    error("<sendpacket> error 0x%08x in line %d", err, __LINE__);
    return err;
    }
  err = recpacket(port, buf, 3);
  if(err)
    {
    error("<recpacket> error 0x%08x in line %d", err, __LINE__);
    return err;
    }
  return (buf[0] << 16) | (buf[1] << 8) | buf[2];
}


#define MAXCPACK 13  // maximum bytes in command packet

// send a packet to PIC controller
int sendpacket(int port, unsigned char *buf, int len)
{
  unsigned char pack[MAXCPACK], sum;
  int ind, err;

  if(len > MAXCPACK - 2)
    return ERR_INTERNAL;
  sum = pack[0] = (unsigned char)(len + 2);
  for(ind = 1; ind <= len; ind++)
    sum += pack[ind] = buf[ind - 1];
  pack[ind] = sum;
  err = WriteCom(port, pack, len + 2);
  return err ? ERR_WRITE_FAIL : 0;
}


#define MAXRPACK 11  // maximum bytes in response packet

// receive a packet from PIC controller
int recpacket(int port, unsigned char *buf, int len)
{
  unsigned char pack[MAXRPACK], *ptr, sum;
  int ind, nb, nrec, err;

  if(len > MAXRPACK - 3)
    return ERR_INTERNAL;

  memset(pack, 0, 10);

  for(nb = len + 3, ptr = pack; nb > 0; nb -= nrec, ptr += nrec)
    {
    nrec = nb;
    err = ReadCom(port, ptr, &nrec);
    if(err || nrec == 0)
      return ERR_READ_FAIL;
    }
  FlushCom(port);       // clear all extra bytes in input buffer
  for(ind = 0, sum = 0; ind < len + 2; ind++)
    sum += pack[ind];
  for(ind = 2; ind < len + 2; ind++)
    buf[ind - 2] = pack[ind];
  if(pack[ind] != sum)
    return ERR_READ_SUM;
  status[port] = pack[1];
  return 0;
}


// Error message
void error(char *fmt, ... )
{
  va_list argptr;
                                                              
  va_start(argptr, fmt);
  vsprintf(errorstr, fmt, argptr);
  va_end(argptr);
}






