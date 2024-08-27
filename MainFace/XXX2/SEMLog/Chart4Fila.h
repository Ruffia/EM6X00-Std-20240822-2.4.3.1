#pragma once
#include "Chart.h"
#include "ChartDef.h"
#include "Util.h"
using namespace Gdiplus;

class CChart4Fila:public CChart
{
public:
	CChart4Fila();

	// Draws the curve and, if the distance between chart points allows, draws data points
	virtual bool DrawChartCurve(V_CHARTDATAD& vDataPnts, GraphicsPath* grPathPtr, Graphics* grPtr, float dpiRatio = 1.0f);

	virtual bool ConvertChartData(V_CHARTDATAD& vDataPnts, V_CHARTDATAF& vDataPntsF, MatrixD* pMatrixD, double startX, double endX);
};