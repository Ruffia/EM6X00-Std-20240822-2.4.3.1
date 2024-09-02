#include "StdAfx.h"
#include "WaferRatioRuler.h"

CWaferRatioRuler::CWaferRatioRuler()
{
	m_nWaferRadius = Wafer_Inch6_Radius;
	_Calculate();
}


void CWaferRatioRuler::_Calculate()
{
	const float fDeltaX = m_nWaferRadius* 2 * 0.12f;
	const float fDeltaY = m_nWaferRadius* 2 * 0.08f;
	m_fXRatio = (m_nWaferRadius*2 - fDeltaX) *1.f/Fill_RowCount;
	m_fYRatio = (m_nWaferRadius*2 - fDeltaY) *1.f/Fill_ColumnCount;
}