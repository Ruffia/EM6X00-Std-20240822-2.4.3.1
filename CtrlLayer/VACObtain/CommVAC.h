#if !defined(AFX_COMMVAC_H__374FF363_6811_4F91_961F_57609A234531__INCLUDED_)
#define AFX_COMMVAC_H__374FF363_6811_4F91_961F_57609A234531__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommVAC.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommVAC window

class AFX_EXT_CLASS CCommVAC
{
// Construction
public:
	CCommVAC();
	virtual ~CCommVAC();

// Attributes
public:

// Operations
public:
	void	SetParam( int nIndex, int nValue );
	void	InitHWND( HWND hWnd );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMVAC_H__374FF363_6811_4F91_961F_57609A234531__INCLUDED_)
