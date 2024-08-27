// SemCtrl_OSE.h: interface for the CSemCtrl_OSE class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_OSE_H__48EB07EC_C6CC_4303_9467_407AB8D1E55D__INCLUDED_)
#define AFX_SEMCTRL_OSE_H__48EB07EC_C6CC_4303_9467_407AB8D1E55D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"
#include "USB_Functions.h"

class AFX_EXT_CLASS CSemCtrl_OSE : public CSemBasisCtrl  
{
	OSEncoderParam	m_oseParam;

public:
	CSemCtrl_OSE();
	virtual ~CSemCtrl_OSE();

	virtual void SetRange( VARIANT varMin, VARIANT varMax ){};
	virtual VARIANT SetPos( VARIANT varPos ){ return GetPos(); };
	virtual VARIANT Plus( VARIANT varDelt ){ return GetPos(); };
	virtual VARIANT Minus( VARIANT varDelt ){ return GetPos(); };

	virtual VARIANT SetUp( const OSEncoderParam& oseParam );

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );
};

#endif // !defined(AFX_SEMCTRL_OSE_H__48EB07EC_C6CC_4303_9467_407AB8D1E55D__INCLUDED_)
