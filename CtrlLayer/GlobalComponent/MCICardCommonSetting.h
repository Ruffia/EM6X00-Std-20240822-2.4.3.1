#pragma once
#include "USB_Functions.h"
#include "RectManager.h"

//时间：2020-08-17
//目的：将若干原MCICard类的成员抽取出来，形成一个全局对象
//原因：这几个参数除了在MCICard中使用外，在Label,Oscillograph, 标尺模块中，这几个参数也频繁使用
//      为了使得这些参数的引用情况更简洁，故而将这几个参数提取出来

class AFX_EXT_CLASS CMCICardCommonSetting  
{
public:
	static CMCICardCommonSetting& Instance();

	double GetMagScreen(double d);

protected:
	CMCICardCommonSetting();


public:
	CSize				m_sizeReso;
	CSize	            m_szGraph;
	CSize	            m_szImage;
	double				m_dblScaleImageToGraph_W;
	double				m_dblScaleImageToGraph_H;
	double	            m_dblPixelsPerMM_HORZ;
	double              m_dblPixelsPerMM_VERT;
	CRect				m_rcImageWindow;	// Graphic Frame map to ImageWindow


	///////////////////////////////////////////////////////////////////
	CRectManager	m_UpdateRectsImage;
	CRectManager	m_UpdateRectsGraph;

	CRectManager	m_UpdateRectsGraphBack;
};
