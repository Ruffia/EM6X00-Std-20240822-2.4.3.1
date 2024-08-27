// MP.h: interface for the CMP class.
//
// Last Modify : 2011.07.19

#if !defined(AFX_MP_H__46D7167A_C6EA_46C0_B9D7_C115F1302135__INCLUDED_)
#define AFX_MP_H__46D7167A_C6EA_46C0_B9D7_C115F1302135__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mcio.h>

class CUSB_TransceiverNew;
class CUsbDI16A;
//class CAdapterUSB;

class AFX_EXT_CLASS CMP  
{
	BOOL			m_bReady;

	CUSB_TransceiverNew*	m_pDI16A;
	BOOL				m_bUSBReady;

	void DeleteAllTransceiver();

public:
	static CMP& Instance();

	void Close();
	BOOL isReady(){ return (m_bUSBReady); }

	// USB
	BOOL	USB_Init( );
//	BOOL	USB_Init( CAdapterUSB* pUsb );
	BOOL	USB_isReady(){ return m_bUSBReady; }

	BOOL	USB_MSGCheckXOR( USB_SEM_ControlMessage& hostMSG );

	BOOL	USB_PostMSG( USB_SEM_ControlMessage& hostMSG );
	BOOL	USB_PostMSGEx( USB_SEM_ControlMessage& hostMSG );
	BOOL	USB_PostMSGtoHost( USB_SEM_ControlMessage& mciMSG );
	BOOL	USB_GetMSG( USB_SEM_ControlMessage& mcMSG );
	BOOL	USB_GetLastError( CString& csError );

	BOOL	USB_GetStatistics( USB_COMM_Statistics& Statistics );

	BOOL	USB_SetCommPause(  BOOL	blPause );

	BOOL	USB_SetCoadjustParam( double dHV );
	// USB

protected:
	CMP();
	virtual ~CMP();
};

#endif // !defined(AFX_MP_H__46D7167A_C6EA_46C0_B9D7_C115F1302135__INCLUDED_)
