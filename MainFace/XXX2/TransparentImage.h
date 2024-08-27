#if !defined(AFX_TRANSPARENTIMAGE_H__E5DDFDB4_251B_4C95_9D44_AE10BDC1A6D7__INCLUDED_)
#define AFX_TRANSPARENTIMAGE_H__E5DDFDB4_251B_4C95_9D44_AE10BDC1A6D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransparentImage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTransparentImage window

class CTransparentImage : public CStatic
{
// Construction
public:
	CTransparentImage();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransparentImage)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTransparentImage();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTransparentImage)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSPARENTIMAGE_H__E5DDFDB4_251B_4C95_9D44_AE10BDC1A6D7__INCLUDED_)
