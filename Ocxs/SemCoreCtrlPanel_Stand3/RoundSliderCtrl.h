#if !defined(AFX_ROUNDSLIDERCTRL_H__856231BA_609B_11D5_BDDC_0050BADA3C27__INCLUDED_)
#define AFX_ROUNDSLIDERCTRL_H__856231BA_609B_11D5_BDDC_0050BADA3C27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RoundSliderCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRoundSliderCtrl window

class CRoundSliderCtrl : public CSliderCtrl
{
protected:
	CPoint		m_ptCenter;
	CPoint		m_ptKnobCenter;
	int			m_nRadius;
	CRgn		m_rgn;

	int			m_nZero;
	bool		m_bInverted;

	CString		m_strText;
	int			m_nKnobRadius;

	bool		m_bDragging;
	bool		m_bDragByKnobOnly;
	bool		m_bDragChanged;
	bool		m_bDrawRadioButton;

	LOGFONT		m_lf;
	CFont		m_font;
	COLORREF	m_crText;
	HBRUSH		m_hKnobBrush;
	HBRUSH		m_hDialBrush;

public:
	DECLARE_DYNAMIC(CRoundSliderCtrl)

	CRoundSliderCtrl();
	virtual ~CRoundSliderCtrl();

	// You may use '%ld' to show the slider value. Default: "%ld?
	virtual void SetText(const CString& strNewText);
	virtual CString GetText() const;

	// Default: 7, Minimum: 5
	virtual void SetKnobRadius(const int nNewKnobRadius);
	virtual int GetKnobRadius() const;

	virtual void SetDragByKnobOnly(const bool bDragByKnobOnly);
	virtual bool GetDragByKnobOnly() const;

	// 0-359, 0 = Top, 90 = Right, 180 = Bottom, 270 = Left
	virtual void SetZero(const int nZero);
	virtual int GetZero() const;

	// CW/CCW
	virtual void SetInverted(const bool bNewInverted = true);
	virtual bool GetInverted() const;

	virtual void SetRadioButtonStyle(const bool bNewRadioButtonStyle = true);
	virtual bool GetRadioButtonStyle() const;

	virtual void SetDialColor(const COLORREF crBkgnd);
	virtual void SetKnobColor(const COLORREF crBkgnd);
	virtual void SetTextColor(const COLORREF crText);

	virtual void SetFontBold(const bool bSet = true);
	virtual void SetFontUnderline(const bool bSet = true);
	virtual void SetFontItalic(const bool bSet = true);
	virtual void SetFontSize(const int nSize);
	virtual void SetFontName(const CString& strFont);

protected:
	// Override this function for customized output
	virtual CString OnFormatText() const;

	//{{AFX_VIRTUAL(CRoundSliderCtrl)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	virtual void ReconstructFont();

	virtual void Init();
	virtual bool SetKnob(const CPoint& pt);
	virtual void PostMessageToParent(const int nTBCode) const;

	//{{AFX_MSG(CRoundSliderCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

#ifdef _DEBUG
public:
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif // _DEBUG
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROUNDSLIDERCTRL_H__856231BA_609B_11D5_BDDC_0050BADA3C27__INCLUDED_)
