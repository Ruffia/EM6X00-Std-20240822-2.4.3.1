#if !defined(AFX_COMMWSM30KV_H__2288D8B7_8D46_4BC9_A91F_B3742B47B69B__INCLUDED_)
#define AFX_COMMWSM30KV_H__2288D8B7_8D46_4BC9_A91F_B3742B47B69B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommWSM30KV.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommWSM30KV window

class AFX_EXT_CLASS CCommWSM30KV
{
// Construction
public:
	CCommWSM30KV();
	virtual ~CCommWSM30KV();

// Attributes
public:
	HWND		m_hMain;

// Operations
public:
	void	SetParam( int nIndex, int nValue );
	void	InitHWND( HWND hWnd );
	BOOL	Init();
	BOOL	Close();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMWSM30KV_H__2288D8B7_8D46_4BC9_A91F_B3742B47B69B__INCLUDED_)
