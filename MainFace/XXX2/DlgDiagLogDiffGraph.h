#if !defined(AFX_DLGDIAGLOGDIFFGRAPH_H__BDA41181_E811_4074_8619_8F64CA888861__INCLUDED_)
#define AFX_DLGDIAGLOGDIFFGRAPH_H__BDA41181_E811_4074_8619_8F64CA888861__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDiagLogDiffGraph.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDiagLogDiffGraph dialog

class CDlgDiagLogDiffGraph : public CDialog
{
// Construction
public:
	CDlgDiagLogDiffGraph(CWnd* pParent = NULL);   // standard constructor
	void	UpdateGraphData( int nTotal );
	void	PaintGraph( CDC* pDC, int nIndex, BOOL bErase=FALSE );
	void	SaveGraph( CString filename );
	void	InitGraphData();

	int		m_nTotal;
	int		m_nIntv;
	double*	m_pdChnValue[8];	// SCN上8个差分通道的原始数据

	int		m_nSemType;			// 产品型号对应的统调方式
	int		m_nGraphType;		// 0:日志记录曲线；1:统调数据曲线1；2:统调数据曲线2
	double	m_dFactors[3];
	void	UpdateGraphData();
	void	PaintGraph2( CDC* pDC, int nIndex, BOOL bErase=FALSE );
	double	*m_pdWD_Obj, *m_pdObj_MagMin;

// Dialog Data
	//{{AFX_DATA(CDlgDiagLogDiffGraph)
	enum { IDD = IDD_Diagnose_Logging_Diff_Graph };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDiagLogDiffGraph)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDiagLogDiffGraph)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDIAGLOGDIFFGRAPH_H__BDA41181_E811_4074_8619_8F64CA888861__INCLUDED_)
