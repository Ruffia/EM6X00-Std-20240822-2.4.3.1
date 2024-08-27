#if !defined(AFX_XSTATICTEXT_H__CA6E2F82_11B1_11D5_B6BF_00E07D8144D0__INCLUDED_)
#define AFX_XSTATICTEXT_H__CA6E2F82_11B1_11D5_B6BF_00E07D8144D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// xStaticText.h : header file
/* 15/03/2001
 * ing.davide.pizzolato@libero.it
 */

#define SST_NORMAL	0
#define SST_SUNKEN	1
#define SST_RAISED	2

/////////////////////////////////////////////////////////////////////////////
// CxStaticText window
class CxStaticText : public CStatic
{
// Construction
public:
	CxStaticText();
// Attributes
public:
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CxStaticText)
	//}}AFX_VIRTUAL
// Implementation
public:
	void SetWindowText( LPCTSTR lpszString );
	void SetTextStyle(int style);
	void SetBlink(BOOL bBlink=TRUE, int blink_time=333, int blink_count=0);
	virtual ~CxStaticText();
	COLORREF SetTextColor(COLORREF new_color);
	COLORREF SetBlinkColor(COLORREF new_color);
	// Generated message map functions
protected:
	int m_Style;
	COLORREF	m_ActualColor;
	int			m_BlinkCounter;
	COLORREF	m_BlinkColor;
	UINT		m_TimerID;				//Blink Timer ID
	COLORREF	m_TextColor;			//button text color
	void InvalidateParent();
	//{{AFX_MSG(CxStaticText)
	afx_msg void OnPaint();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_XSTATICTEXT_H__CA6E2F82_11B1_11D5_B6BF_00E07D8144D0__INCLUDED_)
