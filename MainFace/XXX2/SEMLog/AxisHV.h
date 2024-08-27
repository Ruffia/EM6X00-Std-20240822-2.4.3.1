#pragma once
#include "ChartDef.h"
#include "Axis.h"

class CAxisHV : public CAxis
{
public:
	CAxisHV();
	~CAxisHV();

	virtual PointF DrawAxes(double startX, double endX, double minY, 
		double maxY, Graphics* grPtr, float dpiRatio = 1.0f);

	virtual void DrawGrid(PointF coordOrigF, Graphics* grPtr, float dpiRatio);
};
