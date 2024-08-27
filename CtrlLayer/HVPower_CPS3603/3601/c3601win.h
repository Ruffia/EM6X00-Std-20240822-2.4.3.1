//////////////////////////////////////////////////////////////////////////////////
// Module    : c3601win.h                                                       //
// Function  : Declarations for pointers to DLL functions in c3601.dll          //
// Target OS : Windows                                                          //
// Usage     : Included by all apps using 3601 family PS                        //
// Copyright : (C) 2006 CPS                                                     //
//////////////////////////////////////////////////////////////////////////////////


#ifndef DEF_APP3601 //============================================================

// Error codes returned by C3601_DLLOpen() ------------------
#define ERR_LOAD_DLL            0x10           // Failed to open DLL.
#define ERR_FUNCADDR            0x20           // Failed to find function name in the DLL.


// DLL Startup and Shutdown functions -----------------
/*
#ifdef __cplusplus
extern "C" {
#endif
*/
extern int      C3601_DLLOpen(char *workdir);           // Open DLL and initialize function pointers.
extern void     C3601_DLLClose(void);                   // Close DLL.
/*
#ifdef __cplusplus
}
#endif
*/
#define DEF_APP3601
#endif // #ifndef DEF_APP3601 ======================================================


// If compiling an app module, declare as externals the pointers to DLL functions in WIN3601.C.
#ifndef XFUNC3601
#define XFUNC3601(RTNTYPE,FUNCNAME,ARGS)  extern RTNTYPE (__stdcall *FUNCNAME) ARGS
#endif


///////////////////////////////////////////////////////////////////////////////////
// Functions that are exported by DLL.
///////////////////////////////////////////////////////////////////////////////////
/*
#ifdef __cplusplus
extern "C" {
#endif
*/
// Initialization, shutdown and status.
XFUNC3601( int,  C3601_OpenPort,        ( int port, int baudrate ) );           // port 0 is COM1, port 1 is COM2, etc.
XFUNC3601( int,  C3601_ReadPort,        ( int port, int dev, unsigned char *buf, int *nbytes ) ); // read from a COM port
XFUNC3601( int,  C3601_WritePort,       ( int port, int dev, unsigned char *buf, int nbytes ) );  // write to a COM.
XFUNC3601( void, C3601_ClosePort,       ( int port ) );
XFUNC3601( int,  C3601_Reset,           ( int port, int dev, int type ) );      // type 0 is soft reset, type 1 is hard reset.
XFUNC3601( int,  C3601_GetVersion,      ( int port, int dev ) );                // get firmware version.
XFUNC3601( int,  C3601_SetWD,           ( int port, int dev, int msec ) );      // set watchdog interval in ms units; 0-disable.
XFUNC3601( int,  C3601_GetFaultFlags,   ( int port, int dev ) );                // get all detected non-fatal faults.
XFUNC3601( int,  C3601_GetStatusFlags,  ( int port ) );                         // get all status flags.
XFUNC3601( int,  C3601_GetAlarm,        ( int port ) );                         // get of Alarm line.
XFUNC3601( int,  C3601_GetErrorString,  ( char *buf, int maxlen ) );            // get internal error message string.

// Get and set values.
XFUNC3601( double, C3601_GetVoltage,    ( int port, int dev, int *err ) );       // get output voltage.
XFUNC3601( double, C3601_GetCurrent,    ( int port, int dev, int *err ) );       // get output current.
XFUNC3601( double, C3601_GetTempearture,( int port, int dev, int *err ) );       // get internal temperature.

XFUNC3601( int, C3601_SetValue,         ( int port, int dev, double v, int enab ) ); // set output v and <enab> flag.

// Get and set EEPROM.
XFUNC3601( int, C3601_GetEEPROM,        ( int port, int dev, int address ) );              // get one byte from EEPROM.
XFUNC3601( int, C3601_SetEEPROM,        ( int port, int dev, int address, int value ) );   // set one byte to EEPROM.
/*
#ifdef __cplusplus
}
#endif
*/
