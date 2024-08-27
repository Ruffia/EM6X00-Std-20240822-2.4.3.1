#pragma once
static const int nScaleCount = 12;

class AFX_EXT_CLASS CScaleManager  
{
public:
	static CScaleManager& Instance();
	double GetCurScale();
	void   UpdateScale(double dScale, BOOL bZoomFit); 
	void   OnViewZoomIn(BOOL& bZoomFit);
	void   OnViewZoomOut(BOOL& bZoomFit); 
	BOOL   ISMinIndex();
	BOOL   ISMaxIndex();
	void   OnViewActualSize(BOOL& bZoomFit);
	void   OnViewZoomFit();
	void   ScreenZoom(const double dScale);

private:
	double	g_dScaleScanTable[nScaleCount];
	int		g_nScaleScanIndexMax;
	int		g_nScaleScanIndexMin;
	int		g_nScaleScanIndexFit;
	int		g_nZoomNormal;
	int		g_nScaleScanIndex;

protected:
	CScaleManager();

};
