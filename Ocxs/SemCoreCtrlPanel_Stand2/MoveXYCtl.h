#if !defined(AFX_MOVEXYCTL1_H__654EF04A_2B30_413A_9584_556B07E3C26E__INCLUDED_)
#define AFX_MOVEXYCTL1_H__654EF04A_2B30_413A_9584_556B07E3C26E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MoveXYCtl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMoveXYCtl dialog
class CSemCoreCtrlPanel_Stand2Ctrl;

class CMoveXYCtl : public CDialog
{
	BOOL		m_blMouseCtrl;
	CPoint		m_MousePos;
	void CrossLinesSetScrollBarsPos( double x, double y );

	CPoint		m_ScrollBarsPos;
	void ScrollBarsSetCrossLinesPos( int nScrollH, int nScrollV, BOOL blReportPos = TRUE );

	CPoint		m_MoveXYPos;
	CRect		m_MoveXYFrame;

	BOOL							m_blReady;
	CSemCoreCtrlPanel_Stand2Ctrl*	m_pCoreCtrlPanel;
	void ReportPos();

	CFont		m_font;

public:
	BOOL Bind( CSemCoreCtrlPanel_Stand2Ctrl* pCoreCtrlPanel );

	BOOL		m_bDebugInfoVisible;

	BOOL		m_bCaptionVisible;
	CString		m_strCaption;
	
	CString		m_csXMinString;
	CString		m_csXMaxString;
	CString		m_csYMinString;
	CString		m_csYMaxString;

// Construction
public:
	CMoveXYCtl(CWnd* pParent = NULL);   // standard constructor
	~CMoveXYCtl();
	
	void SetPos( int nXpos, int nYpos );
	void SetRange( int nXmin, int nXmax, int nYmin, int nYmax );


// Dialog Data
	//{{AFX_DATA(CMoveXYCtl)
	enum { IDD = IDD_DIALOG_SEMCORECTRLPANEL_STAND2_DLG_MoveXYCtl };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMoveXYCtl)
	public:
	virtual void OnSetFont(CFont* pFont);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMoveXYCtl)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOVEXYCTL1_H__654EF04A_2B30_413A_9584_556B07E3C26E__INCLUDED_)
