#if !defined(AFX_DIGISTRING_H__F00A5DA1_6165_11D5_BDDC_0050BADA3C27__INCLUDED_)
#define AFX_DIGISTRING_H__F00A5DA1_6165_11D5_BDDC_0050BADA3C27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Digistring.h : header file
//

#include <vector>
using namespace std ;
#include "MEMDC.H"
#include "RGBCOLOR.H"

/////////////////////////////////////////////////////////////////////////////
// CDigistring window

// CRect class with double precision for accurate drawing.
class CDoubleRect
{
public:
	void SetRect(double x1, double y1, double x2, double y2)
	{ left = x1; top = y1; right = x2; bottom = y2;}
	double Width() const{return right - left;}
	double Height() const{return bottom - top;}
	void SetRectEmpty(){left = top = right = bottom = 0.0;}
public:
	double left, top, right, bottom; 
};

class CDSegment
{
public:
	CDSegment();
	CDSegment(const CDSegment& Segment);
	~CDSegment();
	void DefPoints(const POINT* lpPoints, const BYTE* lpTypes, int nCount);
	void Draw(CDC *pDC, CDoubleRect DrawPlace, int iWidth) const;
	void FreeSegment();
	CDSegment operator=(const CDSegment &Segment);

// Implementation
public:
	CPoint	  *	m_paPoints;
	BYTE	  *	m_paTypes;
	int			m_nCount;
};

typedef vector<CDSegment> DSegmentVector;

class CDigiChar
{
// Construction
public:
	CDigiChar();

//Attributes:
public:
	virtual ~CDigiChar();
	virtual void SetElementData(WORD wSegmData, int iDispStyle);
	void	Draw(CDC *pDC, CDoubleRect DrawPlace, CPen *pOffPen, CPen *pOnpen,
		CBrush *pOffBrush, CBrush *pOnBrush);
	void SetColor(COLORREF OffColor, COLORREF OnColor);
	int GetNormWidth() const;


protected:
	int m_Width;
	WORD	m_wSegmData;
	DSegmentVector m_SegmentVector;
	int	m_NSegments;
	COLORREF    m_OffColor;
	COLORREF    m_OnColor;

};

class CDigiColonDotChar : public CDigiChar
{
public:
	CDigiColonDotChar();
	void SetElementData(WORD wSegmData, int iDispStyle);
};

class CDigi7Segment : public CDigiChar
{
public:
	CDigi7Segment();
	void SetElementData(WORD wSegmData, int iDispStyle);
};

class CDigi14Segment : public CDigiChar
{
public:
	CDigi14Segment();
	void SetElementData(WORD wSegmData, int iDispStyle);
};

typedef vector<CDigiChar> DigiCharVector;

/////////////////////////////////////////////////////////////////////////////
// CDigistring class

class CDigistring : public CStatic
{
// Construction
public:
	CDigistring();

// Attributes
public:
	enum {
		DS_SMOOTH	= 1,	// Pioneer kind of characters
		DS_STYLE14	= 2,	// use allways 14 segment display.
		DS_SZ_PROP	= 4		// size proportional
	};

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDigistring)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDigistring();
	void	SetText(LPCTSTR lpszText);
	void	Format(LPCTSTR lpszFormat, ...);
	void	SetColor(COLORREF OffColor, COLORREF OnColor);
	void	SetBackColor(COLORREF BackColor = BLACK);
	BOOL    ModifyDigiStyle(DWORD dwRemove, DWORD dwAdd);

	// Generated message map functions
protected:
	CDigiChar * DefineChar(TCHAR cChar);
	void		BuildString();

	CString		m_strText;
	BOOL		m_Modified;
	DigiCharVector	m_CharVector;
	COLORREF    m_OffColor;
	COLORREF    m_OnColor;
	COLORREF    m_BackColor;
	DWORD		m_DispStyle;
	//{{AFX_MSG(CDigistring)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIGISTRING_H__F00A5DA1_6165_11D5_BDDC_0050BADA3C27__INCLUDED_)
