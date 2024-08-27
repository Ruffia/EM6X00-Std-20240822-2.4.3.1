#if !defined(AFX_ROUNDSLIDERCTRLEX_H__ECB05461_6940_11D5_BDDC_0050BADA3C27__INCLUDED_)
#define AFX_ROUNDSLIDERCTRLEX_H__ECB05461_6940_11D5_BDDC_0050BADA3C27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RoundSliderCtrlEx.h : header file
//
#include "RoundSliderCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CRoundSliderCtrlEx window

class CRoundSliderCtrlEx : public CRoundSliderCtrl
{
	long		m_nRounds;
	long		m_nMinValue;
	long		m_nMaxValue;

	BOOL		m_boolCircle;

// Construction
public:
	CRoundSliderCtrlEx();

// Attributes
public:

// Operations
public:
	virtual void SetRange(long nMinValue, long nMaxValue);
	void SetRangeEx(long nMinValue, long nMaxValue, int nRoundRange);
	void SetRoundRange(int nRoundRange);

	long GetRangeMinEx() const{ return m_nMinValue; };
	long GetRangeMaxEx() const{ return m_nMaxValue; };

	long GetPosEx() const;
	void SetPosEx(long nPos);

	void SetCircle(BOOL boolCircle ){ m_boolCircle = boolCircle; };
	BOOL GetCircle( ){ return m_boolCircle; };


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRoundSliderCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRoundSliderCtrlEx();

	// Generated message map functions
protected:

	BOOL AdjustPosEx( int nNewPos );
	virtual bool SetKnob(const CPoint& pt);

	//{{AFX_MSG(CRoundSliderCtrlEx)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROUNDSLIDERCTRLEX_H__ECB05461_6940_11D5_BDDC_0050BADA3C27__INCLUDED_)
