#if !defined(AFX_COMMWSM30KV2_H__285C06A2_D461_4F0E_931C_E7AC3758D1FD__INCLUDED_)
#define AFX_COMMWSM30KV2_H__285C06A2_D461_4F0E_931C_E7AC3758D1FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommWSM30KV2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommWSM30KV2 window

class AFX_EXT_CLASS CCommWSM30KV2
{
// Construction
public:
	CCommWSM30KV2();
	virtual ~CCommWSM30KV2();

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

#endif // !defined(AFX_COMMWSM30KV2_H__285C06A2_D461_4F0E_931C_E7AC3758D1FD__INCLUDED_)
