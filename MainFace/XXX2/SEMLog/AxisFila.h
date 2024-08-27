#pragma once
#include "ChartDef.h"
#include "Axis.h"

class CAxisFila : public CAxis
{
public:
	CAxisFila();
	~CAxisFila();

	virtual PointF DrawAxes(double startX, double endX, double minY, 
		double maxY, Graphics* grPtr, float dpiRatio = 1.0f);

	virtual void DrawGrid(PointF coordOrigF, Graphics* grPtr, float dpiRatio);
};
