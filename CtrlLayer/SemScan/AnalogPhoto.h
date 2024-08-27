#if !defined(AFX_ANALOGPHOTO_H__0706766C_DA0D_46B8_8479_F2B7BA36B077__INCLUDED_)
#define AFX_ANALOGPHOTO_H__0706766C_DA0D_46B8_8479_F2B7BA36B077__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnalogPhoto.h : header file
//

#include "RasterScan.h"

/////////////////////////////////////////////////////////////////////////////
// CAnalogPhoto thread

class CAnalogPhoto : public CRasterScan
{
	DECLARE_DYNCREATE(CAnalogPhoto)

public:
	CAnalogPhoto();           // protected constructor used by dynamic creation

protected:

// Attributes
public:
	LPBYTE				m_pGraphBuff;

	LPVOID				m_pPhotoBuff;
	DWORD				m_dwPhotoDMABlockSize;

	CRect				m_rcPhotoSlipWindow;
	DWORD				m_dwPhotoLineCounter;

	BOOL				m_boolPhotoReady;

// Operations
protected:
	BOOL OutputPhotoData();
	BOOL PreparePhotoData();

public:
	virtual BOOL InitData();
	virtual void MainLoop();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnalogPhoto)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAnalogPhoto();
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnalogPhoto)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANALOGPHOTO_H__0706766C_DA0D_46B8_8479_F2B7BA36B077__INCLUDED_)
