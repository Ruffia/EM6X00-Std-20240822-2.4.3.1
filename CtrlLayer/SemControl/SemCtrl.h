// SemCtrl.h: interface for the CSemCtrl class.
//
// Last Modify : 2011.07.19

/////////////////////////////////////////////////////////////////////////////
// 11.07.19 按不同机型设置不同的IoRead循环次数
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_H__F11E564F_3C3B_40B5_A71A_F846CFBCAA34__INCLUDED_)
#define AFX_SEMCTRL_H__F11E564F_3C3B_40B5_A71A_F846CFBCAA34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MP.h"
#include "Scan.h"

//09.03.23
#include "LanguageManagerCtrl.h"

/////////////////////////////////////////////

#include "SemVirtualMCICard.h"

class AFX_EXT_CLASS CSemCtrl  
{
	// USB
	BOOL				m_bUSBReady;

public:
	CScan				m_SemScan;
	/////////////////////////////////////////////
	// 真空板类型：0为LRT；1为KYKY（2020.11）
	// 真空板类型：0为LRT；1为KYKY；2为KYKY且使用LaB6+IPG（2021.04）
	int					m_nVACBoardType;

public:
	static CSemCtrl& Instance();
	void  Release();

	BOOL	Close();
	// 09.03.23
	BOOL	InitLanguageManager( CLanguageManagerCtrl* pLanguageManCtrl );
	void	SetVACBoardType( int nType );

//	BOOL	GetMSG( MicrocomputerID Z80ID, SEM_ControlMessage& mcMSG );
	BOOL	GetLastCommError( MicrocomputerID Z80ID, CString& csError );

	/////////////////////////////////////////////
	// USB
	int		USB_Init();
	BOOL	USB_isReady(){ return m_bUSBReady; }
	BOOL	USB_GetMSG( MicrocomputerID Z80ID, USB_SEM_ControlMessage& mcMSG );

	int		USB_ReinitCommPort();
	int		USB_ReinitImagePort();

	BOOL	USB_GetInterruptNumber( unsigned int& nID );
	BOOL	USB_ClearPortDB();
	BOOL	USB_Interrupt();
	BOOL	USB_LoopbackAndReadData( unsigned short nModID, unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred, short nNeedLog );
	BOOL	USB_Loopback();
	BOOL	USB_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData );
	void	USB_CommToBoards( BYTE byteLo, BYTE byteHi );
	void	USB_CommToBoards2( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4 );
	void	USB_CommToBoards3( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, BYTE bOp5, BYTE bOp6 );
	DWORD	USB_ReadBoardData( unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred );
	BOOL	USB_BoardRead2432( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, unsigned long FramePoints, unsigned short* pnFrameData, unsigned long *BytesTransferred );

	// Debug
	DWORD	USB_ConfigureEx(void *pOutMask, void *pData, void *pTristateMask);
	DWORD	USB_ClearFIFO();
	DWORD	USB_StreamOpen( unsigned long bIsRead );
	DWORD	USB_StreamFrame( unsigned long FramePoints, unsigned short *pFrameData, unsigned long *BytesTransferred, short nNeedLog );
	DWORD	USB_StreamClose();

	DWORD	USB_VideoON();
	DWORD	USB_VideoOFF();

	void	USB_GetPortInfo( BYTE* pPort );

	/////////////////////////////////////////////
	// Serial FT232H
	BOOL	Serial_ReadbackData( int nMID, int nCID, unsigned short* pData, int nChn = 0, BOOL bSel = FALSE );
	/////////////////////////////////////////////

protected:
	CSemCtrl();
	virtual ~CSemCtrl();
};

#endif // !defined(AFX_SEMCTRL_H__F11E564F_3C3B_40B5_A71A_F846CFBCAA34__INCLUDED_)
