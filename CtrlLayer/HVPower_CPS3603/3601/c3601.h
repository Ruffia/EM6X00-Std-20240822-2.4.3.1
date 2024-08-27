//////////////////////////////////////////////////////////////////////////////////
// Module    : c3601.h                                                          //
// Function  : Constants and types for c2600.dll                                //
// Target OS : OS-independent                                                   //
// Usage     : Included by all apps using 2600 family PS                        //
// Copyright : (C) 2006 CPS                                                     //
//////////////////////////////////////////////////////////////////////////////////

// Devices
#define DEV_A                      0           // Accelerator
#define DEV_E                   0x10           // Extractor
#define DEV_F                   0x11           // Filament

// Constants
#define MAX_PORTS                  8           // Maximum number of COM ports.
#define DEF_BAUD_RATE          57600           // Default baud rate

// Error codes returned by other C3601_xxxx() functions -----
#define ERR_INVALID_PORT          -1           // Incorrect COM port number.
#define ERR_INVALID_VALUE         -2           // Incorrect value of parameter.
#define ERR_WRITE_FAIL            -3           // Cannot write to port.
#define ERR_READ_FAIL             -4           // Cannot read from port.
#define ERR_ADC_OVERFLOW          -5           // ADC input too large.
#define ERR_ADC_NEGATIVE          -6           // ADC input negative.
#define ERR_READ_SUM              -7           // Incorrect checksum read.
#define ERR_INTERNAL             -99           // Internal error.

// Error flags returned by C3601_GetFaultFlags() ------------
#define ERR_EESum_FLAG          0x80           // EEPROM checksum error.
#define ERR_AdcV_FLAG           0x40           // Voltage metering ADC malfunction.
#define ERR_AdcI_FLAG           0x20           // Current metering ADC malfunction.
#define ERR_DAC_FLAG            0x08           // Setpoint DAC malfunction.


