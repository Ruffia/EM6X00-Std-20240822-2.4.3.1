#include "StdAfx.h"
#include "XXX2.h"
#include "FillButton.h"
#include "WaferRatioRuler.h"

IMPLEMENT_DYNAMIC(CFillButton, CCustomButton)
CFillButton::CFillButton():m_fXStagePos(0.f),m_fYStagePos(0.f),
m_nRow(-1),m_nCol(-1),m_pClient(NULL)
{

}

CFillButton::~CFillButton()
{

}


void CFillButton::CalculateStagePosition(const CPoint& ptWaferCenter)
{
    //按照晶圆形状，每行有多少个Fill
	const int nRow[Fill_RowCount] = {6,8,14,14,16,18,20,20,22,22,22,22,22,22,22,22,22,22,22,22,20,20,18,16,14,14};

	CWaferRatioRuler ruler;

	const int nColumnCount = nRow[m_nRow];
	if (m_nCol < nColumnCount/2)
	{
		m_fYStagePos -= (nColumnCount/2 - m_nCol) * ruler.m_fYRatio;
	}
	else
	{
		m_fYStagePos += (m_nCol - nColumnCount/2) * ruler.m_fYRatio;
	}

	if (m_nRow < Fill_RowCount/2)
	{
		m_fXStagePos += (m_nRow - Fill_RowCount/2) * ruler.m_fXRatio;
	}
	else
	{
		m_fXStagePos += (m_nRow - Fill_RowCount/2) * ruler.m_fXRatio;
	}
	
}


void CFillButton::ShowPostion()
{
	CString strPosition = _T("");
	strPosition.Format(_T("Row = %d,Column = %d,\r XStagePos = %.1f,YStagePos = %.1f"),m_nRow,m_nCol,m_fXStagePos,m_fYStagePos);
	MessageBox(strPosition);
}

void CFillButton::MoveStage()
{
	if(!m_pClient) return;
	//m_pClient->m_nCurFillID = m_nShowID;
	//m_pClient->m_eMovingType = StageMoving::AutoImage;
	//m_pClient->FeSEM_StageSetPos(m_fXStagePos,'x');
	//m_pClient->FeSEM_StageSetPos(m_fYStagePos,'y');
}