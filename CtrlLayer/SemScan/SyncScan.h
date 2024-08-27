#if !defined(AFX_SYNCSCAN_H__90E8EE6D_988A_43E1_93B2_6725D614D210__INCLUDED_)
#define AFX_SYNCSCAN_H__90E8EE6D_988A_43E1_93B2_6725D614D210__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SyncScan.h : header file
//

#include "RasterScan.h"

/////////////////////////////////////////////////////////////////////////////
// CSyncScan thread

class CSyncScan : public CRasterScan
{
	DECLARE_DYNCREATE(CSyncScan)
		
public:
	CSyncScan();           // protected constructor used by dynamic creation

protected:

// Attributes
public:

// Operations
protected:
	BOOL	InputData();
	BOOL	OutputData();

public:
	virtual	BOOL	InitData();
	virtual void	MainLoop();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSyncScan)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSyncScan();

	// Generated message map functions
	//{{AFX_MSG(CSyncScan)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYNCSCAN_H__90E8EE6D_988A_43E1_93B2_6725D614D210__INCLUDED_)
