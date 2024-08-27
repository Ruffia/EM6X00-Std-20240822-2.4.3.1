// RoundSliderCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "RoundSliderCtrlEx.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRoundSliderCtrlEx

static const double pi = 3.141592653589793238462643383279;

CRoundSliderCtrlEx::CRoundSliderCtrlEx()
{
	m_nRounds = 0;
	m_boolCircle = FALSE;
}

CRoundSliderCtrlEx::~CRoundSliderCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CRoundSliderCtrlEx, CRoundSliderCtrl)
	//{{AFX_MSG_MAP(CRoundSliderCtrlEx)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRoundSliderCtrlEx message handlers

void CRoundSliderCtrlEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	const int nMin = GetRangeMin();
	const int nMax = GetRangeMax()+1;

	switch(nChar)
	{
	case VK_LEFT:
	case VK_UP:
		{
			int nNewPos = GetPos()-GetLineSize();
			while(nNewPos < nMin)
			{
				nNewPos += (nMax - nMin);
				m_nRounds --;
			}
			//SetPos(nNewPos);
			AdjustPosEx( nNewPos );
			RedrawWindow();
			PostMessageToParent(TB_LINEUP);
		}
		break;
	
	case VK_RIGHT:
	case VK_DOWN:
		{
			int nNewPos = GetPos()+GetLineSize();
			while(nNewPos >= nMax)
			{
				nNewPos -= (nMax - nMin);
				m_nRounds ++;
			}
			//SetPos(nNewPos);
			AdjustPosEx( nNewPos );
			RedrawWindow();
			PostMessageToParent(TB_LINEDOWN);
		}
		break;

	case VK_PRIOR:
		{
			int nNewPos = GetPos()-GetPageSize();
			while(nNewPos < nMin)
			{
				nNewPos += (nMax - nMin);
				m_nRounds --;
			}
			//SetPos(nNewPos);
			AdjustPosEx( nNewPos );
			RedrawWindow();
			PostMessageToParent(TB_PAGEUP);
		}
		break;

	case VK_NEXT:
		{
			int nNewPos = GetPos()+GetPageSize();
			while(nNewPos >= nMax)
			{
				nNewPos -= (nMax - nMin);
				m_nRounds ++;
			}
			//SetPos(nNewPos);
			AdjustPosEx( nNewPos );
			RedrawWindow();
			PostMessageToParent(TB_PAGEDOWN);
		}
		break;

	case VK_HOME:
	case VK_END:
		// Do nothing (ignore keystroke)
		break;

	default:
		CRoundSliderCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

bool CRoundSliderCtrlEx::SetKnob(const CPoint& pt)
{
	const int nMin = GetRangeMin();
	const int nMax = GetRangeMax()+1;

	CSize szDelta = pt - m_ptCenter;
	if(m_bInverted) szDelta.cx *= -1;

	double dNewPos = 0.0;

	if(szDelta.cx != 0)
	{
		dNewPos = 90.0 - atan(-(double)szDelta.cy / (double)szDelta.cx) * 180.0 / pi;
	}

	if(((szDelta.cx == 0) && (szDelta.cy >= 0)) || (szDelta.cx < 0))
	{
		dNewPos += 180.0;
	}

	dNewPos -= m_nZero;

	while(dNewPos < 0.0) dNewPos += 360.0;
	while(dNewPos >= 360.0) dNewPos -= 360.0;

	const int nNewPos = nMin + (int)(dNewPos*(nMax-nMin)/360.0);
	const int nOldPos = GetPos();
	const bool bChanged = (nNewPos != nOldPos);
	const int nDelta = nNewPos - nOldPos;

	if(bChanged)
	{
		if (nDelta >0)
		{
			if (nDelta > (nMax-nMin)/2)
				m_nRounds --;
		}
		else
		{
			if (-nDelta > (nMax-nMin)/2)
				m_nRounds ++;
		}

		//SetPos(nNewPos);
		AdjustPosEx( nNewPos );
	}

	return bChanged;
}

BOOL CRoundSliderCtrlEx::AdjustPosEx( int nNewPos )
{
	const long nMin = GetRangeMin();
	const long nMax = GetRangeMax()+1;
	const long nNewValue = m_nRounds * (nMax - nMin) + nNewPos + GetRangeMinEx();

	BOOL blRet = FALSE;

	if (nNewValue > m_nMaxValue)
	{
		if ( !m_boolCircle )
			SetPosEx( m_nMaxValue );
		else
			SetPosEx( m_nMinValue );
	}
	else if (nNewValue < m_nMinValue)
	{
		if ( !m_boolCircle )
			SetPosEx( m_nMinValue );
		else
			SetPosEx( m_nMaxValue );
	}
	else
	{
		SetPos(nNewPos);
		blRet = TRUE;
	}

	if ( !blRet ) MessageBeep(-1);
	
	return blRet;
}

long CRoundSliderCtrlEx::GetPosEx() const
{
	const long nMin = GetRangeMin();
	const long nMax = GetRangeMax()+1;

	return m_nRounds * (nMax - nMin) + GetPos() + GetRangeMinEx();
}

void CRoundSliderCtrlEx::SetPosEx(long nPos)
{
	const long nRoundRange = (long)GetRangeMax()-(long)GetRangeMin()+1;

	m_nRounds = nPos / nRoundRange;
	SetPos( nPos % nRoundRange );
}

void CRoundSliderCtrlEx::SetRange(long nMinValue, long nMaxValue)
{
	SetRangeEx( nMinValue, nMaxValue, (nMaxValue - nMinValue) );
}

void CRoundSliderCtrlEx::SetRangeEx(long nMinValue, long nMaxValue, int nRoundRange)
{
	m_nMinValue = nMinValue;
	m_nMaxValue = nMaxValue;
	
	CRoundSliderCtrl::SetRange( 0, nRoundRange-1);
	CRoundSliderCtrl::SetZero( 0 );
}

void CRoundSliderCtrlEx::SetRoundRange(int nRoundRange)
{
	const long nPos = GetPosEx();
	CRoundSliderCtrl::SetRange(0, nRoundRange-1);
	SetPosEx( nPos );
}
