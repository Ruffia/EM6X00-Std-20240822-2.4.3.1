#if !defined(AFX_COMMBSD_H__45861F16_8FE8_418E_870A_7511BC9CB86A__INCLUDED_)
#define AFX_COMMBSD_H__45861F16_8FE8_418E_870A_7511BC9CB86A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommBSD.h : header file
//

#define WM_USER_BSD_Update	(WM_USER+91)

/////////////////////////////////////////////////////////////////////////////
// CCommBSD window

class CCommBSD
{
// Construction
public:
	CCommBSD();
	virtual ~CCommBSD();

// Operations
public:
	void	InitHWND( HWND hWnd );
	BOOL	Open( short port );
	BOOL	Close();

	BOOL	ChangeChannel( int nIndex );
	BOOL	ChangeCoarse( int nIndex );
	BOOL	ChangeFine( WORD wValue );
	BOOL	ChangeBrt( WORD wValue );

	void	SetCommTest( BOOL bNeed );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMBSD_H__45861F16_8FE8_418E_870A_7511BC9CB86A__INCLUDED_)
