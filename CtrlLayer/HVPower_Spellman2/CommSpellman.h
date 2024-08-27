#if !defined(AFX_COMMSPELLMAN_H__6EEC2DAB_819C_4CAF_B294_4AF449F66343__INCLUDED_)
#define AFX_COMMSPELLMAN_H__6EEC2DAB_819C_4CAF_B294_4AF449F66343__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommSpellman.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommSpellman window

class AFX_EXT_CLASS CCommSpellman
{
// Construction
public:
	CCommSpellman();
	virtual ~CCommSpellman();

// Attributes
public:

// Operations
public:
	void	SetSharedParam( void* pParam );
	void	SetParam( int nIndex, int nValue );
	BOOL	Init( HWND hWnd );
	BOOL	Close();
	void	SetCommand( CString strCmd );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMSPELLMAN_H__6EEC2DAB_819C_4CAF_B294_4AF449F66343__INCLUDED_)
