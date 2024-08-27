// DllTestView.h : interface of the CDllTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLLTESTVIEW_H__B6250117_EC17_4A9F_9228_454DD62487EF__INCLUDED_)
#define AFX_DLLTESTVIEW_H__B6250117_EC17_4A9F_9228_454DD62487EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DllTestDoc.h"

const long HandleAreaW = 16;
const long HandleAreaH = 12;
const long HandleAreaW2 = 20;
const long HandleAreaH2 = 20;
/*
#define AngleMeasureCounts	5

struct AngleMeasure
{
	CPoint	ptS;		// 起始点 Start
	CPoint	ptC;		// 中心点 Center
	CPoint	ptE;		// 终点 End;
	double	dAngle;

	int		nR;			// 圆弧所在圆的半径
	CRect	rcBound;	// 圆弧所在圆的外接矩形
	CPoint	ptArcStart;	// 圆弧起始位置
	CPoint	ptArcEnd;	// 圆弧终止位置
	CPoint	ptLabel;	// 标注点位置

	BOOL	bStart;		// 起始点已被确认
	BOOL	bCenter;	// 中心点已被确认
	BOOL	bEnd;		// 终点已被确认

	CPoint	m_ptOriginal;

	int		nIndex;
	int		nAngleStatus;
	int		bAngleHitTest;
};*/
/*
struct AnglePosition
{
	double	dAngle;
	CPoint	ptS;		// 起始点 Start
	CPoint	ptC;		// 中心点 Center
	CPoint	ptE;		// 终点 End;
	CRect	rcBound;	// 圆弧所在圆的外接矩形
	CPoint	ptArcStart;	// 圆弧起始位置
	CPoint	ptArcEnd;	// 圆弧终止位置
	CPoint	ptLabel;	// 标注点位置
};

struct AngleMeasureNew
{
	BOOL	bStart;		// 起始点已被确认
	BOOL	bCenter;	// 中心点已被确认
	BOOL	bEnd;		// 终点已被确认

	CPoint	ptOriginal;

	int		nIndex;
	int		nAngleStatus;
	int		bAngleHitTest;

	AnglePosition	AP;
	AnglePosition	APbak;
};

typedef CTypedPtrList< CPtrList, AngleMeasureNew* >		ListAngle;
*/

struct LineMeasure
{
	CPoint pt1;
	CPoint pt2;

	CPoint pt1bak;
	CPoint pt2bak;
};

class CDllTestView : public CView
{
	enum {
			GraphHitTest_Normal	= 0,
			GraphHitTest_E		= 1,
			GraphHitTest_W		= 2,
			GraphHitTest_In		= 3,	// 整体
			GraphHitTest_C		= 4,	// 中心
			GraphHitTest_L		= 5,	// 标注
			GraphHitTest_NULL	= 6,
	};

protected: // create from serialization only
	CDllTestView();
	DECLARE_DYNCREATE(CDllTestView)

// Attributes
public:
	CDllTestDoc* GetDocument();
	CString CalculateUScale2( int nScreenWidth, double dfMag );
	int		m_nScalePixels;
	CString	m_csUScal;
	CString	m_csUScaleUnit;
	double	m_dfMobileUScaleParameter;

	CString CalculateUScaleSrc( int nScreenWidth, double dfMag );
	int		m_nScalePixelsSrc;
	CString	m_csUScalSrc;
	CString	m_csUScaleUnitSrc;
	double	m_dfMobileUScaleParameterSrc;

	/////////////////////////////////////////////
	/////////////////////////////////////////////
	CFont	m_Font;

	BOOL	m_bAddAngle;
	HCURSOR m_hCursors[7];
//	int		m_nAngleStatus[AngleMeasureCounts];
//	BOOL	m_bAngleHitTest[AngleMeasureCounts];
	int		m_nAngleCounts;
//	AngleMeasure	m_AM[AngleMeasureCounts], m_AMbak[AngleMeasureCounts];	// Angle Measure
	ListAngle		*m_plistAngle;
	AngleOption		m_AngleOption;

	void	AngleAddNew( int nIndex );
	double	GetAngleOfTwoVector( CPoint ptS, CPoint ptE, CPoint ptC );
	CPoint	GetCoordinates( CPoint ptCenter, double dAngle, int nR );
	void	GetAngleMeasureInfo( AnglePosition& AP );
	void	GetAnglePoints( AngleMeasure* pAM, CPoint ptPos );

	CPoint	ScreenToGraph( const CPoint point );
	int		GraphHitTest( const CPoint ptGraphPos );

	/////////////////////////////////////////////
	/////////////////////////////////////////////
	BOOL	m_bLine;
	int		m_nHitLine;
	LineMeasure	m_Line;

	void	DrawUScale( CDC* pDC );
	void	DrawAngle( CDC* pDC );
	void	DrawLine( CDC* pDC );

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDllTestView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDllTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDllTestView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTestAngleMeasure();
	afx_msg void OnUpdateTestAngleMeasure(CCmdUI* pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChangeAngleOptions();
	afx_msg void OnChangeAngleDelete();
	afx_msg void OnTestLine();
	afx_msg void OnUpdateTestLine(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DllTestView.cpp
inline CDllTestDoc* CDllTestView::GetDocument()
   { return (CDllTestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLTESTVIEW_H__B6250117_EC17_4A9F_9228_454DD62487EF__INCLUDED_)
