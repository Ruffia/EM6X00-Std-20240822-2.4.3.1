#include "stdafx.h"
#include "MCICardCommonSetting.h"

CMCICardCommonSetting& CMCICardCommonSetting::Instance()
{
	static CMCICardCommonSetting Inst;
	return Inst;
}

double CMCICardCommonSetting::GetMagScreen(double dMag)
{
	double dMagScreen = dMag * m_sizeReso.cx /(127 * m_dblPixelsPerMM_HORZ);
	return dMagScreen;
}



CMCICardCommonSetting::CMCICardCommonSetting()
{
	m_sizeReso = CSize(1024,768);
	m_szGraph = CSize(0,0);
	m_szImage = CSize(0,0);
	m_dblScaleImageToGraph_W = 1.0;
	m_dblScaleImageToGraph_H = 1.0;
	m_dblPixelsPerMM_HORZ = 1.0;
	m_dblPixelsPerMM_VERT = 1.0;
	m_rcImageWindow = CRect(0,0,0,0);	// Graphic Frame map to ImageWindow
}

