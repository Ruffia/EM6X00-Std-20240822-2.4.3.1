// MCICard.cpp : implementation file
//

#include "stdafx.h"
#include "Ddutil.h"
#include "MCICard.h"
#include <string>
#include "RectConvertor.h"
#include "MCICardOption.h"
#include "MCICardCommonSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMCICard

BOOL CMCICard::ShowTime(BOOL boolShow)
{
	if ( m_pLabelControler->m_boolShowTime == boolShow )
		return m_pLabelControler->m_boolShowTime;
	m_pLabelControler->m_boolShowTime = boolShow;
	if( m_pLabelControler->m_boolShowLabel )
		m_pLabelControler->RedrawLabelLay(m_bSplit,m_bDet2nd);
	return TRUE;
}

BOOL CMCICard::ShowNote(BOOL boolShow)
{
	if ( m_pLabelControler->m_boolShowNote == boolShow )
		return m_pLabelControler->m_boolShowNote;
	m_pLabelControler->m_boolShowNote = boolShow;
	if( m_pLabelControler->m_boolShowLabel )
		m_pLabelControler->RedrawLabelLay(m_bSplit,m_bDet2nd);
	return TRUE;
}

BOOL CMCICard::ShowPixelSize(BOOL boolShow)
{
	if ( m_pLabelControler->m_boolShowPixelSize == boolShow )
		return m_pLabelControler->m_boolShowPixelSize;
	m_pLabelControler->m_boolShowPixelSize = boolShow;
	if( m_pLabelControler->m_boolShowLabel )
		m_pLabelControler->RedrawLabelLay(m_bSplit,m_bDet2nd);
	return TRUE;
}



void CMCICard::UpdateMAG(
	CString		csMag,					// �Ŵ�����ʾ�ַ�
	int			nUScalePixelNum,		// �̶������ʾ����
	CString		csUScal,				// �̶������ʾ�ַ�
	CString		csMobileUScaleText )	// �ƶ������ʾ�ַ�
{

	m_pLabelControler->UpdateMAG(csMag,nUScalePixelNum,csUScal);
	m_pMobileUScaleControler->UpdateMAG(csMobileUScaleText);
}

// 16.06.12 ���ƶ����ʵ��
void CMCICard::UpdateMAGArray( int nIndex,
	CString		csMobileUScaleText )	// �ƶ������ʾ�ַ�
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_pMobileUScaleControler->m_csMobileUScaleTextArray[nIndex]	= csMobileUScaleText;
	m_pLabelControler->m_boolRedrawLabel = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}
// 16.06.12 ���ƶ����ʵ��

// 19.02.18 �ױ��Զ���
void CMCICard::UpdateFooter( int nIndex, BOOL bVisible, CPoint ptPos )
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	CMCICardOption::Instance().m_footerOpt.opt[nIndex].bVisible	= bVisible;
	CMCICardOption::Instance().m_footerOpt.opt[nIndex].ptPos.x	= (int)(CMCICardCommonSetting::Instance().m_sizeReso.cx *ptPos.x /100.0);
	if( ptPos.y < 2 )
		CMCICardOption::Instance().m_footerOpt.opt[nIndex].ptPos.y = 5;
	else
		CMCICardOption::Instance().m_footerOpt.opt[nIndex].ptPos.y = m_pLabelControler->m_szLabel.cy /2 +5;
	m_pLabelControler->m_boolRedrawLabel = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}

void CMCICard::UpdateFooterText( int nIndex, CString csText )
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	CMCICardOption::Instance().m_footerOpt.opt[nIndex].strDisp	= csText;
	m_pLabelControler->m_boolRedrawLabel = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}
// 19.02.18 �ױ��Զ���

