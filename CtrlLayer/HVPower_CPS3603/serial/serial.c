//*********************** serial.c ****************************
//* This file contains port interface functions for CPS serial
//* port controlled power supplies.
//* Written by Alexander Kostromitin
//* Copyright (C) CPS, 2006
//*************************************************************


#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <commdlg.h>    // includes common dialog functionality
#include <commctrl.h>
#include <dlgs.h>       // includes common dialog template defines
#include <cderr.h>      // includes the common dialog error codes

#include "..\serial\serial.h"

#define NPORTS  8       // maximum number of ports
HANDLE hndtab[NPORTS];


// Open a COM port. Port is 0 for COM1, 1 for COM2, etc.
// The DCB structure defines the control setting for a serial communications device.
// If ask!=0, a driver-supplied configuration dialog box will be displayed to fill the DCB.
// Returns 0 on success or an error code.
int OpenCom(int port, DCB *dcb, int timeout, int ask)
{
  char name[20];
  COMMCONFIG comcfg;
  COMMTIMEOUTS timeouts;
  int err;

  if(port < 0 || port >= NPORTS)
    return ERROR_INVALID_NAME;
  sprintf(name, "COM%d", port + 1);
  if(hndtab[port] == 0)     // not open: open now
    {
    hndtab[port] = CreateFile(
        name,               // pointer to name of device
        GENERIC_READ | GENERIC_WRITE,  // access mode
        0,    // share mode: comm devices must be opened with exclusive access
        NULL, // pointer to security attributes: no security attributes
        OPEN_EXISTING, // type of creation: comm devices must use OPEN_EXISTING
        FILE_ATTRIBUTE_NORMAL,    // file attributes: no overlapped I/O
        NULL  // handle to file with attributes to copy: must be NULL for comm devices
        );
    if(hndtab[port] == INVALID_HANDLE_VALUE)
      return GetLastError();
    }
  if(ask)                   // ask user to fill the DCB
    {
    DWORD size =sizeof(comcfg);

    comcfg.wVersion = 1;
    comcfg.dwProviderSubType = PST_RS232;
    comcfg.dwProviderOffset = 0;
    comcfg.dwSize = sizeof(comcfg);

    err = !GetCommConfig(hndtab[port], &comcfg, &size);
    if(err)
      {
      err = GetLastError();
      CloseHandle(hndtab[port]);
      hndtab[port] = 0;
      return err;
      }
    err = !CommConfigDialog(
        name,               // device name
        0,                  // handle to window
        &comcfg             // pointer to comm configuration structure
        );
    if(err)
      {
      err = GetLastError();
      CloseHandle(hndtab[port]);
      hndtab[port] = 0;
      return err;
      }
    *dcb = comcfg.dcb;
    }
  err = !SetCommState(hndtab[port], dcb);
  if(err)
    {
    err = GetLastError();
    CloseHandle(hndtab[port]);
    hndtab[port] = 0;
    return err;
    }
  err = !SetCommMask(hndtab[port], EV_RXCHAR/* | EV_TXEMPTY*/);
  if(err)
    {
    err = GetLastError();
    CloseHandle(hndtab[port]);
    hndtab[port] = 0;
    return err;
    }

  timeouts.ReadIntervalTimeout          = MAXDWORD;//10000;
  timeouts.ReadTotalTimeoutMultiplier   = 0;//10000;
  timeouts.ReadTotalTimeoutConstant     = timeout;
  timeouts.WriteTotalTimeoutMultiplier  = timeout;
  timeouts.WriteTotalTimeoutConstant    = timeout;
  SetCommTimeouts(hndtab[port], &timeouts);
  return 0;
}


// Close a COM port.
// Returns 0 on success or an error code.
int CloseCom(int port)
{
  if(port < 0 || port >= NPORTS)
    return ERROR_INVALID_NAME;
  if(hndtab[port])          // is open?
    {
    CloseHandle(hndtab[port]);
    hndtab[port] = 0;
    }
  return 0;
}


// Read n bytes from a COM port to buf.
// In binary mode (DCB.fBinary==1) may return '\0's !!!
// Returns 0 on success or an error code; actual number of bytes read stored in nbytes.
int ReadCom(int port, unsigned char *buf, int *nbytes)
{
  //DWORD event = EV_RXCHAR;      // A character was received and placed in the input buffer.
  int nb = *nbytes;             // Store for ReadFile.

  if(port < 0 || port >= NPORTS)
    return ERROR_INVALID_NAME;
  if(hndtab[port] == 0)         // not open
    return ERROR_INVALID_HANDLE;
  //if(WaitCommEvent(hndtab[port], &event, NULL))
    {
    if(ReadFile(hndtab[port], buf, nb, (DWORD*)nbytes, NULL))
      {
      //FlushFileBuffers(hndtab[port]);
      return 0;
      }
    }
  return GetLastError();
}


// Write n bytes to a COM port from buf.
// Returns 0 on success or an error code.
int WriteCom(int port, unsigned char *buf, int nbytes)
{
  int nb;

  if(port < 0 || port >= NPORTS)
    return ERROR_INVALID_NAME;
  if(hndtab[port] == 0)         // not open
    return ERROR_INVALID_HANDLE;

  //FlushFileBuffers(hndtab[port]);

  if(WriteFile(hndtab[port], buf, nbytes, (DWORD*)&nb, NULL))
    {
    if(nb == nbytes)
      return 0;
    return ERROR_INVALID_DATA;
    }
  return GetLastError();
}


// Flush port's buffers, clear input.
void FlushCom(int port)
{
  //int nb;

  if(port < 0 || port >= NPORTS)
    return;
  if(hndtab[port] == 0)         // not open
    return;

  PurgeComm(hndtab[port], PURGE_RXCLEAR);
}


// Reset a COM port.
void ResetCom(int port)
{
  if(port < 0 || port >= NPORTS)
    return;
  if(hndtab[port] == 0)         // not open
    return;
  FlushFileBuffers(hndtab[port]);
}


int SetComState(int port, DCB *dcb)
  {
  int err = !SetCommState(hndtab[port], dcb);
  if(err)
    {
    err = GetLastError();
    CloseHandle(hndtab[port]);
    hndtab[port] = 0;
    }
  return err;
  }

int GetComStatus(int port)
{
  int res;
  DWORD sts;

  res = GetCommModemStatus(hndtab[port], &sts);
  if(res)
    return sts;
  return 0;
}



