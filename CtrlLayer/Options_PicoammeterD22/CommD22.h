#if !defined(AFX_COMMD22_H__BA3805E5_8328_4CAE_AA77_68FDCCA559ED__INCLUDED_)
#define AFX_COMMD22_H__BA3805E5_8328_4CAE_AA77_68FDCCA559ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommD22.h : header file
//

#define WM_USER_D22_UpdateValue	(WM_USER+101)

/////////////////////////////////////////////////////////////////////////////
// CCommD22 window

class AFX_EXT_CLASS CCommD22
{
// Construction
public:
	CCommD22();
	~CCommD22();

// Attributes
public:

// Operations
public:
	void	InitHWND( HWND hWnd );
	BOOL	Open(int nPort);

	BOOL	GetVersion();
	float	GetCurrentBeam();
	void	Status();
	BOOL	SetPause( BOOL bPause );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMD22_H__BA3805E5_8328_4CAE_AA77_68FDCCA559ED__INCLUDED_)
