#include "stdafx.h"
#include "ChartDef.h"
#include "ChartContainer4NegativeExponent.h"
#include "Chart.h"
#include "Util.h"
#include "Chart4NegativeExponent.h"
#include "DataLabel.h"

CChartContainer4NegativeExponent::CChartContainer4NegativeExponent()
{

}


void CChartContainer4NegativeExponent::AddChart(ChartType eType)
{

}

int CChartContainer4NegativeExponent::AddChart(ChartProperty* pAttr, CChart* chartPtr, V_CHARTDATAD& vData, bool bRedraw)
{
	int chartIdx = GetMaxChartIdx() + 1;
	bool bAddIdx = false;
	if (!pAttr->m_label.empty())
	{
		pAttr->m_label = NormalizeString(pAttr->m_label, STR_MAXLEN, STR_NORMSIGN);
		CChart* twinPtr = FindChartByName(pAttr->m_label);
		if (twinPtr != NULL)
			bAddIdx = true;
	}
	else
	{
		pAttr->m_label = string_t(_T("Cnart"));
		bAddIdx = true;
	}

	if (bAddIdx)
	{
		_TCHAR buffer_t[64];
		_itot_s(chartIdx, buffer_t, 10);  // Chart idx to string
		string_t idxStr(buffer_t);
		pAttr->m_label += string_t(_T("_")) + string_t(buffer_t);
	}

	chartPtr->SetChartContainer(this);
	chartPtr->SetIndex(chartIdx);
	
	size_t dataSize = vData.size();
	// Now transfer data and set min max values
	if (dataSize > 0)
	{
		chartPtr->m_vDataPnts.assign(vData.begin(), vData.end());
		chartPtr->m_vDataPnts.shrink_to_fit();

		// It is cheaper to sort right away than to look for max/min x and sort later if needed
		if (dataSize > 1)
			std::sort(chartPtr->m_vDataPnts.begin(), chartPtr->m_vDataPnts.end(), less_pnt<double, false>());

		double minValX = chartPtr->m_vDataPnts.front().X;
		double maxValX = chartPtr->m_vDataPnts.back().X;

		// Find min and max Y; works even for one-point vector
		PAIR_ITS pair_minmaxY = minmax_element(chartPtr->m_vDataPnts.begin(), chartPtr->m_vDataPnts.end(), less_pnt<double, true>());
		double minValY = pair_minmaxY.first->Y;
		double maxValY = pair_minmaxY.second->Y;

		// Save in the CChart
		chartPtr->SetMinValX(minValX);
		chartPtr->SetMaxValX(maxValX);
		chartPtr->SetMinValY(minValY);
		chartPtr->SetMaxValY(maxValY);
	}

	// Just in case: idx is unique for this container
	if (m_mapCharts.insert(MAP_CHARTS::value_type(chartPtr->GetChartIdx(), chartPtr)).second == false)
	{
		delete chartPtr;
		return -1;
	}

	// Now update the container's min maxes, saving the history of X
	if (dataSize > 0)
	{
		// Wil automatically take care of previous one-point charts
		chartPtr->UpdateExtX(chartPtr->GetMinValX(), chartPtr->GetMaxValX());
		chartPtr->UpdateExtY(chartPtr->GetMinValY(), chartPtr->GetMaxValY());

		if (IsWindow(m_hWnd) && m_pProperty->m_bTracking && IsLabWndExist(false))
			PrepareDataLegend(m_dataLegPntD, m_pProperty->m_epsX, m_pDataWnd->m_mapLabs, m_mapSelPntsD, true);

		if (bRedraw && IsWindow(m_hWnd)&&IsWindowVisible())
			UpdateContainerWnds(-1, true);
	}

	return chartIdx;
}

void CChartContainer4NegativeExponent::DrawDataCurve( RectF rGdiF, Graphics* grPtr )
{
	if (m_mapCharts.size() <= 0){ return; }

	RectF drawRF = rGdiF;   
	drawRF.X += 85;
	drawRF.Y += 20;
	drawRF.Width -= 206;
	drawRF.Height -= 55;

	// Begin draw curves
	GraphicsPath grPath;            // Will be used actoss the drawing routines
	MAP_CHARTS::iterator it = m_mapCharts.begin();
	MAP_CHARTS::iterator itE = m_mapCharts.end();
	for (; it != itE; ++it)
	{
		CChart* chartPtr = it->second;
		ChartProperty* pAttr = chartPtr->GetProperty();
		if (chartPtr->IsChartVisible() == false)
			continue;
		if (chartPtr->m_vDataPnts.size() == 0)
			continue;

		chartPtr->SetDrawRect(drawRF);
		chartPtr->DrawChartCurve(chartPtr->m_vDataPnts, &grPath, grPtr);
		if (m_pProperty->m_bTracking&&IsLabWndExist(false))              // Draw selection points if any
			DrawSelPntsD(chartPtr, m_mapSelPntsD, &grPath, grPtr, drawRF);
	}
}