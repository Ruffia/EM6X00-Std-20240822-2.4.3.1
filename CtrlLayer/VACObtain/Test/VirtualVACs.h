#if !defined(AFX_VIRTUALVACS_H__C8F162AB_1C23_48E2_BFC2_60432E195DF5__INCLUDED_)
#define AFX_VIRTUALVACS_H__C8F162AB_1C23_48E2_BFC2_60432E195DF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VirtualVACs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVirtualVACs window

class AFX_EXT_CLASS CVirtualVACs
{
// Construction
public:
	CVirtualVACs();
	virtual ~CVirtualVACs();

// Attributes
public:

// Operations
public:
	BOOL	ClearDevice();
	BOOL	Close( int nType );
	BOOL	Reset( CWnd* pWnd );
	void	SetParam( int index, int nValue );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIRTUALVACS_H__C8F162AB_1C23_48E2_BFC2_60432E195DF5__INCLUDED_)
