#if !defined(AFX_COMM3603_H__B2E06495_A45E_4995_8563_7148C4C94875__INCLUDED_)
#define AFX_COMM3603_H__B2E06495_A45E_4995_8563_7148C4C94875__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Comm3603.h : header file
//

#include "serial\serial.h"
#include "3601\c3601.h"
#include "3601\c3601win.h"

#include <USB_Functions.h>

#define USERMODE    1       // disable some "for internal use" functions

/////////////////////////////////////////////////////////////////////////////
// CComm3603 window

class AFX_EXT_CLASS CComm3603
{
// Construction
public:
	CComm3603();
	virtual ~CComm3603();

// Attributes
public:

// Operations
public:
	void	SetParam( int nIndex, int nValue );
	void	GetParam( int nIndex, int& nValue );
	BOOL	InitDevice( CWnd* pWnd );
	BOOL	IsReady();
	BOOL	CloseDevice();
	void	InitParam();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMM3603_H__B2E06495_A45E_4995_8563_7148C4C94875__INCLUDED_)
