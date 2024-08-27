#include "StdAfx.h"
#include "AxisEMI.h"

CAxisEMI::CAxisEMI()
{
	m_bDrawXAxis = FALSE;
	m_fOffSetYAxis = 0.f;
	m_colAxis = ((ARGB)Color::Orange);
	m_colGrid = ((ARGB)Color::Black);
}

CAxisEMI::~CAxisEMI()
{
}


// Draw functions
PointF CAxisEMI::DrawAxes(double startX, double endX, double minY, 
								 double maxY, Graphics* grPtr, float dpiRatio)
{
	Pen axisPen(m_colAxis, AXIS_PENWIDTH*dpiRatio);  

	m_fOffSetYAxis = -(m_drawRF.Width + 30);

	PointF pntStartF, pntEndF, pntCoordZeroF;
	// Let's begin with X axis

	PAIR_XAXPOS pair_axXYPos = GetXAxisPos(m_drawRF, minY, maxY);
	m_axYPos = pair_axXYPos.first;
	pntStartF.Y = pntEndF.Y = pair_axXYPos.second;
	pntStartF.X = m_drawRF.GetLeft();   // Don't worry: draw area is clipped
	pntEndF.X   = m_drawRF.GetRight();
	if (m_bDrawXAxis)
	{
		if (m_bVisible)
		{
			grPtr->DrawLine(&axisPen, pntStartF, pntEndF);
		}
	}
	
	pntCoordZeroF.Y = pntStartF.Y;

	// Y axis
	PAIR_YAXPOS pair_axYXPos = GetYAxisPos(m_drawRF, startX, endX);
	m_axXPos = pair_axYXPos.first;
	pntStartF.X = pntEndF.X = pair_axYXPos.second - m_fOffSetYAxis;
	pntStartF.Y = m_drawRF.GetBottom();
	pntEndF.Y   = m_drawRF.GetTop() - DR_SPACE*dpiRatio;
	if (m_bVisible)
	{
		grPtr->DrawLine(&axisPen, pntStartF, pntEndF);
	}	

	pntCoordZeroF.X = pntStartF.X;

	m_axOffsXF = pntCoordZeroF.Y; 
	return PointF(pair_axYXPos.second,pntCoordZeroF.Y);
}


void CAxisEMI::DrawGrid(PointF coordOrigF, Graphics* grPtr, float dpiRatio)
{
	USES_CONVERSION;
	float fDeltaX = m_drawRF.Width  * 1.f/m_nGridXGroupCount;
	float fDeltaY = m_drawRF.Height * 1.f/m_nGridYGroupCount;

	float limTop    = m_drawRF.GetTop();
	float limBottom = m_drawRF.GetBottom();
	float limLeft   = m_drawRF.GetLeft() - m_fOffSetYAxis;
	float limRight  = m_drawRF.GetRight();

	Pen gridPen(m_colGrid, 1.0f*dpiRatio);
	gridPen.SetDashStyle(DashStyleSolid);
	gridPen.SetDashCap(DashCapRound);

	FontFamily fontFamily(L"Verdana");
	Gdiplus::Font axFont(&fontFamily, 7.0f, FontStyleRegular);

	StringFormat strFormat;
	strFormat.SetAlignment(StringAlignmentNear);
	strFormat.SetLineAlignment(StringAlignmentCenter);

	SolidBrush axLabBr((ARGB)Color::Black);

	float xF, yF;
	yF = coordOrigF.Y - fDeltaY;

	vector<AXISSECTION> vYSection;
	vector<double> vYAxisWithZero;  //带0的Y轴刻度
	vYAxisWithZero.push_back(limBottom);
	int i = m_vecGridYGraduation.size() - 1;
	while (yF > limTop && i >= 0)
	{
		if (0 == i%5)
		{
			if (m_bVisible)
			{
				grPtr->DrawLine(&gridPen, limLeft, yF, limLeft + 10, yF);
			}
			
			TCHAR sz[16] = {0};
			_sntprintf(sz,16,_T("%.0f"),m_vecGridYGraduation[i]);
			string_t strX = sz;
			PointF PointX;
			PointX.X = limLeft + 16;
			PointX.Y = yF - 5;

			if (m_bVisible)
			{
				grPtr->DrawString(A2W(strX.c_str()),-1, &axFont,PointX, &axLabBr);
			}
		}
		else
		{
			if (m_bVisible)
			{
				grPtr->DrawLine(&gridPen, limLeft, yF, limLeft + 6, yF);
			}		
		}

		vYAxisWithZero.push_back(yF);

		yF -= fDeltaY;
		i--;	
	}

	string_t strX = "0";
	PointF PointX;
	PointX.X = limLeft + 16;
	PointX.Y = limBottom - 6;
	if (m_bVisible)
	{
		grPtr->DrawLine(&gridPen, limLeft,limBottom - 4, limLeft + 10, limBottom - 4);
		grPtr->DrawString(A2W(strX.c_str()),-1, &axFont,PointX, &axLabBr);
	}


	if (m_bVisible)
	{
		SolidBrush axLabBr(m_colAxis);
		PointF PointX;
		PointX.X = limLeft - 15;
		PointX.Y = m_drawRF.Y - 15;
		grPtr->DrawString(A2W("EMI(uA)"),-1, &axFont,PointX, &axLabBr);
	}

	for (int i = m_vecGridYGraduation.size() - 1, j = 0; i >= 0 && j < vYAxisWithZero.size() - 1;i--,j++)
	{
		vYSection.push_back(AXISSECTION(m_vecGridYGraduation[i],vYAxisWithZero[j+1],vYAxisWithZero[j]));
	}

	SetYSection(vYSection);

	if (!m_bDrawXAxis)
	{
		return;
	}

	int nX = 0;
	xF = coordOrigF.X;
	while (xF < limRight && abs(limRight - xF) >= 1)
	{
		if( 0 == nX % 3)
		{
			if (m_bVisible)
			{
				grPtr->DrawLine(&gridPen, xF, limBottom - 10, xF, limBottom);
			}
			
			time_t lTimeSatamp = __int64(m_vecGridXGraduation[nX]);
			lTimeSatamp += 28800;//GTM偏移8个时区得到北京时间
			tm stime;
			gmtime_s(&stime, &lTimeSatamp);
			char sz[64] = {0};
			strftime(sz, 64, "%H:%M:%S", &stime);
			string_t strX = sz;

			PointF PointX;
			PointX.X = xF - 20;
			PointX.Y = limBottom + 10;

			if (m_bVisible)
			{
				grPtr->DrawString(A2W(strX.c_str()),-1, &axFont,PointX, &axLabBr);
			}
		}
		else
		{
			if (m_bVisible)
			{
				grPtr->DrawLine(&gridPen, xF, limBottom - 5, xF, limBottom);
			}
		}

		xF += fDeltaX;
		nX++;
	}
}