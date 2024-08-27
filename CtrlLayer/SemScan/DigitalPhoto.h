#if !defined(AFX_DIGITALPHOTO_H__4BD3AA91_D23F_4FBE_AE18_2ABB5ADA5AC3__INCLUDED_)
#define AFX_DIGITALPHOTO_H__4BD3AA91_D23F_4FBE_AE18_2ABB5ADA5AC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DigitalPhoto.h : header file
//

#include "BasisScan.h"


/////////////////////////////////////////////////////////////////////////////
// CDigitalPhoto thread

class CDigitalPhoto : public CBasisScan
{
	DECLARE_DYNCREATE(CDigitalPhoto)
public:
	CDigitalPhoto();           // protected constructor used by dynamic creation
protected:

// Attributes
public:
	LPBYTE				m_pImageBuff;

	LPVOID				m_pPhotoBuff;
	DWORD				m_dwPhotoDMABlockSize;

	DWORD				m_dwExpectantInterval;
	CRect				m_rcDestFrame;
	BOOL				m_blRedirectToDump;

	CRect				m_rcPhotoSlipWindow;
	DWORD				m_dwPhotoLineCounter;

	BOOL				m_boolPhotoReady;
	CRect				m_rcPhotoIndication;

// Operations
protected:
	BOOL OutputPhotoData();
	BOOL PreparePhotoData();

public:
	virtual BOOL InitData();
	virtual void MainLoop();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDigitalPhoto)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDigitalPhoto();
protected:

	// Generated message map functions
	//{{AFX_MSG(CDigitalPhoto)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIGITALPHOTO_H__4BD3AA91_D23F_4FBE_AE18_2ABB5ADA5AC3__INCLUDED_)
