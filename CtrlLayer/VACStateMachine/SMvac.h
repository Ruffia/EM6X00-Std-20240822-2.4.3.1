#if !defined(AFX_SMVAC_H__0E21EBF9_49C0_4329_9D42_5FB9931A26A2__INCLUDED_)
#define AFX_SMVAC_H__0E21EBF9_49C0_4329_9D42_5FB9931A26A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SMvac.h : header file
//

#include "MP.h"
// USB
#include "UsbDI16A.h"

/////////////////////////////////////////////////////////////////////////////
// CSMvac window

class CVac;

class AFX_EXT_CLASS CSMvac
{
	CVac*		m_pVac;
	BOOL		m_bReady;

// Construction
public:
	static CSMvac& Instance();

protected:
	CSMvac();
	virtual ~CSMvac();

// Attributes
public:

// Operations
public:
	BOOL	Close();
	BOOL	Reset( CWnd* pWnd );
	BOOL	isReady(){ return m_bReady; };
	BOOL	SetParams( int nType, DWORD dwParam );
	BOOL	SetParams2( int nType, double dParam );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMVAC_H__0E21EBF9_49C0_4329_9D42_5FB9931A26A2__INCLUDED_)
