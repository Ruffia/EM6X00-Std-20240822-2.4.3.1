#include "stdafx.h"
#include "Chart4EMI.h"
#include "ChartContainer.h"

CChart4EMI::CChart4EMI()
{
	m_bSelected = false;
}

bool CChart4EMI::DrawChartCurve(V_CHARTDATAD& vDataPntsD,GraphicsPath* grPathPtr, Graphics* grPtr, float dpiRatio)
{
	if (vDataPntsD.size()== 0)    // Just for safe programming; the function is never called on count zero
		return false;

	if (!m_bVisible)
	{
		return false;
	}

	m_pAxis->RestoreModeX();  // Might be in the middle of settung zoon borders; annuls the mode
	m_pAxis->RestoreModeY();

	// Draw axes
	if (!m_pContainer->HasChartWithData(-1))
	{
		m_pAxis->DrawGrid(PointF(0.0f, 0.0f), grPtr,1.0);
		return false;
	}

	// Proceed normally even with one point data
	const double startX = m_pAxis->m_vecGridXGraduation[0];
	const double endX = m_pAxis->m_vecGridXGraduation[m_pAxis->m_vecGridXGraduation.size() - 1];

	const double minExtY = 0;
	const double maxExtY = max(m_pAxis->m_vecGridYGraduation[0] ,m_pAxis->m_vecGridYGraduation[m_pAxis->m_vecGridYGraduation.size() - 1]);
	PointF pntOrigF = m_pAxis->DrawAxes(startX, endX, minExtY, maxExtY, grPtr);
	// Draw a grid    
	m_pAxis->DrawGrid(pntOrigF, grPtr,1.0);
	// Update scales
	std::pair<double, double> pair_scales = m_pAxis->UpdateScales(m_drawRF, startX, endX, minExtY, maxExtY);
	m_pAxis->m_scX = pair_scales.first;
	m_pAxis->m_scY = pair_scales.second;
	// Set transform matrix
	MatrixD matrixD;
	matrixD.Translate(pntOrigF.X, pntOrigF.Y);
	matrixD.Scale(m_pAxis->m_scX, -m_pAxis->m_scY);
	// Account for not even axes placement
	if (m_pAxis->m_axXPos != BETWEEN || m_pAxis->m_axYPos != MIDDLE)
	{
		double translateX = (m_pAxis->m_axXPos == RIGHT) ? -endX : (m_pAxis->m_axXPos == LEFT) ? -startX : 0.0;
		double translateY = (m_pAxis->m_axYPos == BOTTOM) ? -minExtY : (m_pAxis->m_axYPos == TOP) ? -maxExtY : 0.0;
		matrixD.Translate(translateX, translateY);
	}
	if (m_pAxis->m_pMatrixD != NULL)
		delete m_pAxis->m_pMatrixD;
	m_pAxis->m_pMatrixD = matrixD.Clone();     // Copy constructor is private ??


	V_CHARTDATAF vDataPntsF;
	// Convert the pntsD to the screen pntsF
	if (!ConvertChartData(vDataPntsD, vDataPntsF, m_pAxis->m_pMatrixD, startX, endX)) 
		return false;

	V_CHARTDATAF::iterator itF = vDataPntsF.begin();
	V_CHARTDATAF::pointer ptrDataPntsF = vDataPntsF.data();
	size_t vSize = vDataPntsF.size();

	// Add the curve to grPath
	Pen pen(m_pAxis->GetAxesColor(), m_pProperty->m_fPenWidth*dpiRatio);
	pen.SetDashStyle(m_pProperty->m_dashStyle);
	if (!m_pProperty->m_bShowPnts&&(vSize == 2))   // Are outside or at boundaries of clipping area
	{                                 // Make special semi-transparent dash pen
		Color col(SetAlpha(m_pAxis->GetAxesColor(), ALPHA_NOPNT));
		pen.SetColor(col);
	}

	if (m_pProperty->m_dashStyle != DashStyleCustom)
	{
		if (vSize > 1)
		{
			grPtr->DrawCurve(&pen, ptrDataPntsF, vSize, m_pProperty->m_fTension);

			if (m_bSelected && (dpiRatio == 1.0f))  // Mark the chart as selectes on screen only
			{
				Pen selPen(Color(SetAlpha(m_pAxis->GetAxesColor(), ALPHA_SELECT)), (m_pProperty->m_fPenWidth + PEN_SELWIDTH)*dpiRatio);
				grPtr->DrawCurve(&selPen, ptrDataPntsF, vSize, m_pProperty->m_fTension);
			}
		}

		// Now add the points
		if (m_pProperty->m_bShowPnts || (vSize == 1))
		{
			itF = adjacent_find(vDataPntsF.begin(), vDataPntsF.end() , 
				lesser_adjacent_interval<PointF, false>(PointF(dpiRatio*CHART_PNTSTRSH, 0.0f)));
			if (itF == vDataPntsF.end())    // All intervals are greater than CHART_PNTSTRSH   
			{
				itF = vDataPntsF.begin();    // Base
				for (; itF != vDataPntsF.end(); ++itF)
				{
					RectF rPntF = RectFFromCenterF(*itF, dpiRatio*CHART_DTPNTSZ, 
						dpiRatio*CHART_DTPNTSZ);
					grPathPtr->AddEllipse(rPntF);
				}
			}
		}
	}
	else
	{
		PointF pntF;
		PointF pntFX(dpiRatio*CHART_DTPNTSZ/2, 0.0f);
		PointF pntFY(0.0f, dpiRatio*CHART_DTPNTSZ/2);

		for (; itF != vDataPntsF.end(); ++itF)
		{
			pntF = *itF;
			grPathPtr->StartFigure();
			grPathPtr->AddLine(pntF - pntFX, pntF + pntFX);
			grPathPtr->StartFigure();
			grPathPtr->AddLine(pntF - pntFY, pntF + pntFY);
		}
		if (vSize == 1)
		{
			grPathPtr->StartFigure();
			grPathPtr->AddEllipse(RectFFromCenterF(pntF, 2.0f*pntFX.X, 2.0f*pntFY.Y));
		}
	}

	if (grPathPtr->GetPointCount() > 0)          // Has points to draw
	{  
		pen.SetWidth(1.0f*dpiRatio);
		pen.SetDashStyle(DashStyleSolid);
		grPtr->DrawPath(&pen, grPathPtr);
		if (((m_pProperty->m_dashStyle == DashStyleCustom)||(vSize == 1))&& m_bSelected && (dpiRatio == 1.0f))
		{
			pen.SetColor(Color(SetAlpha(m_pAxis->GetAxesColor(), ALPHA_SELECT)));
			pen.SetWidth(m_pProperty->m_fPenWidth + PEN_SELWIDTH);
			grPtr->DrawPath(&pen, grPathPtr);
		}
		grPathPtr->Reset();
	}
	return true;
}


bool CChart4EMI::ConvertChartData(V_CHARTDATAD& vDataPnts, V_CHARTDATAF& vDataPntsF, MatrixD* pMatrixD, double startX, double endX)
{
	if(vDataPnts.size() <= 0) return false;
	if(m_pAxis->m_vYSection.size() <= 0) return false;
	for (V_CHARTDATAD::iterator it = vDataPnts.begin(); it != vDataPnts.end();it++)
	{
		PointD& pntD = *it;
		double X = m_drawRF.GetLeft() +  (pntD.X - startX) * pMatrixD->m_scX;
		double Y = pMatrixD->m_scY*pntD.Y + pMatrixD->m_offsY;
		for (int i = 0; i < m_pAxis->m_vYSection.size() - 1;i++ )
		{
			const AXISSECTION& YSection = m_pAxis->m_vYSection[i];
			const AXISSECTION& YSectionNext = m_pAxis->m_vYSection[i + 1];
			if (YSection.m_dValue < pntD.Y && pntD.Y < YSectionNext.m_dValue)
			{
				double dStep = pntD.Y/YSection.m_dValue;
				Y =  YSection.m_dScreenValueMin - (YSection.m_dScreenValueMax - YSection.m_dScreenValueMin)/10.0 * dStep;
				break;
			}
		}

		vDataPntsF.push_back(PointF(X,Y));
	}
	return true;
}