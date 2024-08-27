#if !defined(CRC_Calc__INCLUDED_)
#define CRC_Calc__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CRC_Calc.h : header file
//

///////////////////////////////////////////////////////////////////////////////

unsigned short CRC16_CCITT(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_CCITT_FALSE(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_XMODEM(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_X25(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_MODBUS2(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_IBM(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_MAXIM(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_USB(unsigned char *puchMsg, unsigned int usDataLen);

unsigned int CRC32(unsigned char *puchMsg, unsigned int usDataLen);

///////////////////////////////////////////////////////////////////////////////

#endif //CRC_Calc__INCLUDED_