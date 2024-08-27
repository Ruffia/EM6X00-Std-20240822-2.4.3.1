#if !defined(AFX_PLCVACBOARD_H__F92EF900_7591_4D21_B510_0CFF623E4EED__INCLUDED_)
#define AFX_PLCVACBOARD_H__F92EF900_7591_4D21_B510_0CFF623E4EED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PLCvacBoard.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPLCvacBoard window

class AFX_EXT_CLASS CPLCvacBoard
{
// Construction
public:
	CPLCvacBoard();
	virtual ~CPLCvacBoard();

// Attributes
public:

// Operations
public:
	BOOL	Init(HWND hWnd);
	BOOL	Close();
	BOOL	SetCmd( BYTE bFunc, BYTE bOpr );
	BOOL	SetThresholds( BYTE bOpr, double dValue );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLCVACBOARD_H__F92EF900_7591_4D21_B510_0CFF623E4EED__INCLUDED_)
