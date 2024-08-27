// SemCtrl_MCB.h: interface for the CSemCtrl_MCB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMCTRL_MCB_H__999839CF_0349_4C00_8A13_32E4322834A9__INCLUDED_)
#define AFX_SEMCTRL_MCB_H__999839CF_0349_4C00_8A13_32E4322834A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SemBasisCtrl.h"

class AFX_EXT_CLASS CSemCtrl_MCB : public CSemBasisCtrl
{
	OSEncoderParam	m_oseParam;

public:
	CSemCtrl_MCB();
	virtual ~CSemCtrl_MCB();

	virtual void SetRange( VARIANT varMin, VARIANT varMax ){};
	virtual VARIANT SetPos( VARIANT varPos ){ return GetPos(); };
	virtual VARIANT Plus( VARIANT varDelt ){ return GetPos(); };
	virtual VARIANT Minus( VARIANT varDelt ){ return GetPos(); };

	virtual VARIANT SetUp( const OSEncoderParam& oseParam );

	virtual BOOL Flush();
	virtual const CString& FormatText();
	virtual const CString& FormatText( VARIANT varPos );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCTRL_MCB_H__999839CF_0349_4C00_8A13_32E4322834A9__INCLUDED_)
