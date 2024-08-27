// SemCtrl_PC10SE6.h: interface for the CSemCtrl_PC10SE6 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_PC10SE6_H__E96EDCE4_8540_4CAE_9BD3_8E1ED7DABE4D__INCLUDED_)
#define AFX_SEMCTRL_PC10SE6_H__E96EDCE4_8540_4CAE_9BD3_8E1ED7DABE4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

enum SignalProcesserState
{
	SignalProcesser_Close				= 0,
	SignalProcesser_LowPassFilter		= 1,
	SignalProcesser_SignalEnhancement	= 2,
};

class AFX_EXT_CLASS CSemCtrl_PC10SE6 : public CSemBasisCtrl  
{
	long	m_lSPState;

	BYTE FormatControlFlag();

public:
	CSemCtrl_PC10SE6();
	virtual ~CSemCtrl_PC10SE6();

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );

	VARIANT GetSPState( )
		{ 	VARIANT State;
			State.lVal = m_lSPState;
			return State;	};
	void SetSPState( VARIANT State )
		{ m_lSPState = State.lVal; };
};

#endif // !defined(AFX_SEMCTRL_PC10SE6_H__E96EDCE4_8540_4CAE_9BD3_8E1ED7DABE4D__INCLUDED_)
