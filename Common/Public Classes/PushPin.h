// PushPin.h: interface for the CPushPin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUSHPIN_H__A72F1532_5211_11D5_BDDC_0050BADA3C27__INCLUDED_)
#define AFX_PUSHPIN_H__A72F1532_5211_11D5_BDDC_0050BADA3C27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ButtonST.h"

class CPushPin : public CButtonST  
{
public:
	CPushPin();
	virtual ~CPushPin();

// Attributes
private:
	BOOL	m_bPinned;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CButtonST)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Unpinned() { m_bPinned = FALSE; };
	BOOL IsPinned() { return m_bPinned; };

protected:
    //{{AFX_MSG(CButtonST)
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_PUSHPIN_H__A72F1532_5211_11D5_BDDC_0050BADA3C27__INCLUDED_)
