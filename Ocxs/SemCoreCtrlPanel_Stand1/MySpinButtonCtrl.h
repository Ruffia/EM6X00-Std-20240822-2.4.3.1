// Last Modify : 2005.04.07

#if !defined(AFX_MYSPINBUTTONCTRL_H__50773217_EDE9_434F_BBDA_A79E1DF61209__INCLUDED_)
#define AFX_MYSPINBUTTONCTRL_H__50773217_EDE9_434F_BBDA_A79E1DF61209__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySpinButtonCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMySpinButtonCtrl window

class CMySpinButtonCtrl : public CSpinButtonCtrl
{
// Construction
public:
	CMySpinButtonCtrl();
	virtual void Init();
	void	DrawPressedRect(CDC &dc,const CRect& rctDown) const;

// Attributes
public:
	CBitmap	m_bmpUp;
	CBitmap	m_bmpDown;

	BOOL	m_bActiveSpinPressed;
	CRect	m_rcIsPressed;

	int		m_nPrevPos;
	int		m_nMinPos;
	int		m_nMaxPos;

// Operations
public:
	void	SetBitmap( UINT uIDUp, UINT uIDDown );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySpinButtonCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMySpinButtonCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMySpinButtonCtrl)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	HPEN m_penLightShadow;
	HPEN m_penDarkShadow;
	HPEN m_penShadow;
	HPEN m_penLight;
	HPEN m_penButtonFace;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSPINBUTTONCTRL_H__50773217_EDE9_434F_BBDA_A79E1DF61209__INCLUDED_)
