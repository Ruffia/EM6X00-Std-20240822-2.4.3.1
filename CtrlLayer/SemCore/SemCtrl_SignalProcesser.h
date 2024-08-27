// SemCtrl_SignalProcesser.h: interface for the CSemCtrl_SignalProcesser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_SIGNALPROCESSER_H__3C02E407_EBE6_4F4D_8B79_3745191B317E__INCLUDED_)
#define AFX_SEMCTRL_SIGNALPROCESSER_H__3C02E407_EBE6_4F4D_8B79_3745191B317E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

enum SignalProcesserButton
{
	SignalProcesser_NULL,
	SignalProcesser_Power,
	SignalProcesser_Right,
	SignalProcesser_Left,
	SignalProcesser_Down,
	SignalProcesser_Up,
};

class AFX_EXT_CLASS CSemCtrl_SignalProcesser : public CSemBasisCtrl  
{
	UINT	m_nClickedButtonID;
	long	m_lSignalConverterState;

	BYTE FormatControlFlag();

public:
	CSemCtrl_SignalProcesser();
	virtual ~CSemCtrl_SignalProcesser();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

	VARIANT GetSignalConverter( )
		{ 	VARIANT Switch;
			Switch.lVal = m_lSignalConverterState;
			return Switch;	};
	void SetSignalConverter( VARIANT Switch )
		{ m_lSignalConverterState = Switch.lVal; };

	void ClickButton( UINT nButtonID )
		{ m_nClickedButtonID = nButtonID; };
};

#endif // !defined(AFX_SEMCTRL_SIGNALPROCESSER_H__3C02E407_EBE6_4F4D_8B79_3745191B317E__INCLUDED_)
