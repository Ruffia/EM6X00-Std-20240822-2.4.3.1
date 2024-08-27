#if !defined(AFX_COMMBOARDVAC_H__E74FCDCC_9AD4_441E_BAF9_B172C5C17F80__INCLUDED_)
#define AFX_COMMBOARDVAC_H__E74FCDCC_9AD4_441E_BAF9_B172C5C17F80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommBoardVAC.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommBoardVAC window

class AFX_EXT_CLASS CCommBoardVAC
{
// Construction
public:
	CCommBoardVAC();
	virtual ~CCommBoardVAC();

// Attributes
public:

// Operations
public:
	void	InitHWND( HWND hWnd );
	BOOL	Init( int nIndex );
	BOOL	Close( int nIndex );
	void	SetParam( int nIndex, int nValue );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMBOARDVAC_H__E74FCDCC_9AD4_441E_BAF9_B172C5C17F80__INCLUDED_)
