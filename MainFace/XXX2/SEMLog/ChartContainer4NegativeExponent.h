#pragma once
#include "ChartContainer.h"
enum ChartType
{
	Chart_CCG = 0,
	Chart_HV,
	Chart_Fila,
};

class CChartContainer4NegativeExponent : public CChartContainer
{
public:
	CChartContainer4NegativeExponent();

	int AddChart(ChartProperty* pAttr,  CChart* pChart, V_CHARTDATAD& vData, bool bRedraw = false);

	void AddChart(ChartType eType);

protected:
	virtual void DrawDataCurve( RectF rGdiF, Graphics* grPtr );
};