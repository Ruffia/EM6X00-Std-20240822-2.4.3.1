#if !defined(AFX_DLGDIAGLOGGRAPH_H__10A4A284_6878_47CC_99D3_F88558B4F4BE__INCLUDED_)
#define AFX_DLGDIAGLOGGRAPH_H__10A4A284_6878_47CC_99D3_F88558B4F4BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDiagLogGraph.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDiagLogGraph dialog

class CDlgDiagLogGraph : public CDialog
{
// Construction
public:
	CDlgDiagLogGraph(CWnd* pParent = NULL);   // standard constructor
	void	UpdateGraphData( int nTotal );
	void	PaintGraph( CDC* pDC, int nIndex, BOOL bErase=FALSE );
	void	PaintGraph_FromNodes( CDC* pDC, int nIndex );
	void	SaveGraph( CString filename );

	int		m_nTotal;
	void	InitGraphData();
	int		m_nIntv;

	void	Graph1XSB_Change( int nPos );
	void	Graph2XSB_Change( int nPos );
	void	Graph3XSB_Change( int nPos );
	void	Graph4XSB_Change( int nPos );

// Dialog Data
	//{{AFX_DATA(CDlgDiagLogGraph)
	enum { IDD = IDD_Diagnose_Logging_Graph };
	CScrollBar	m_scroll4;
	CScrollBar	m_scroll3;
	CScrollBar	m_scroll2;
	CScrollBar	m_scroll1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDiagLogGraph)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDiagLogGraph)
	afx_msg void OnPaint();
	afx_msg void OnBUTTONGraph1Setup();
	afx_msg void OnBUTTONGraph2Setup();
	afx_msg void OnBUTTONGraph3Setup();
	afx_msg void OnBUTTONGraph4Setup();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSPINGraph1Y(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSPINGraph2Y(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSPINGraph3Y(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSPINGraph4Y(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBUTTONGraph1Default();
	afx_msg void OnBUTTONGraph2Default();
	afx_msg void OnBUTTONGraph3Default();
	afx_msg void OnBUTTONGraph4Default();
	afx_msg void OnDeltaposSPINGraph1X(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSPINGraph2X(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSPINGraph3X(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSPINGraph4X(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBUTTONGraphSaveGraph();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDIAGLOGGRAPH_H__10A4A284_6878_47CC_99D3_F88558B4F4BE__INCLUDED_)
