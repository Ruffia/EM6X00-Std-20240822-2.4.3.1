#pragma once
#include "USB_Functions.h"
#include "RectManager.h"

//ʱ�䣺2020-08-17
//Ŀ�ģ�������ԭMCICard��ĳ�Ա��ȡ�������γ�һ��ȫ�ֶ���
//ԭ���⼸������������MCICard��ʹ���⣬��Label,Oscillograph, ���ģ���У��⼸������ҲƵ��ʹ��
//      Ϊ��ʹ����Щ�����������������࣬�ʶ����⼸��������ȡ����

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
