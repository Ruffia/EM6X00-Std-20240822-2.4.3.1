#if !defined(AFX_IMAGEVIEW_H__60D5E8CE_92C1_4B18_A63B_2C1869E84283__INCLUDED_)
#define AFX_IMAGEVIEW_H__60D5E8CE_92C1_4B18_A63B_2C1869E84283__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageView view

class CImageView : public CScrollView
{
protected:
	CImageView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CImageView)

	CPoint m_RefPoint,m_RefScroll;
	CBrush m_brHatch;

// Attributes
public:
	CImageDoc* GetDocument();
	WORD	m_wLanguageID;

// Operations
public:
	void	Localize(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate();     // first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	void GetImageXY(CImageDoc *pDoc, CxImage *ima, long *x, long *y);

protected:
	virtual ~CImageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CImageView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnViewPanel3();
	afx_msg void OnUpdateViewPanel3(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnNewImage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNewProgress(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ImageView.cpp
inline CImageDoc* CImageView::GetDocument()
   { return (CImageDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEVIEW_H__60D5E8CE_92C1_4B18_A63B_2C1869E84283__INCLUDED_)
