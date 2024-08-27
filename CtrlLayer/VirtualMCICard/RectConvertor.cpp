#include "stdafx.h"
#include "RectManager.h"
#include "MCICardCommonSetting.h"

CRect LayWindowMapClip_Graph_to_Image( const CRect& rcGrapic )
{
	const CRect& rcImageWindow = CMCICardCommonSetting::Instance().m_rcImageWindow;
	const double dblScaleImageToGraph_H = CMCICardCommonSetting::Instance().m_dblScaleImageToGraph_H;
	const double dblScaleImageToGraph_W = CMCICardCommonSetting::Instance().m_dblScaleImageToGraph_W;

	CRect	rcImage;
	CPoint	ptOrigin(rcImageWindow.left, rcImageWindow.top);

	//////////////////////////////////////////////////////////////////////
	rcImage.left = (int)(rcGrapic.left / dblScaleImageToGraph_W + 0.5);
	rcImage.right = (int)(rcGrapic.right / dblScaleImageToGraph_W + 0.5);
	rcImage.top = (int)(rcGrapic.top / dblScaleImageToGraph_H + 0.5);
	rcImage.bottom = (int)(rcGrapic.bottom / dblScaleImageToGraph_H + 0.5);
	//////////////////////////////////////////////////////////////////////

	rcImage.OffsetRect( ptOrigin );
	rcImage &= rcImageWindow;

	return rcImage;
}



CRect LayWindowMapClip_Image_to_Graph(CRect& rcImage)
{
	const CSize& szGraph = CMCICardCommonSetting::Instance().m_szGraph;
	const CRect& rcImageWindow = CMCICardCommonSetting::Instance().m_rcImageWindow;
	const double dblScaleImageToGraph_H = CMCICardCommonSetting::Instance().m_dblScaleImageToGraph_H;
	const double dblScaleImageToGraph_W = CMCICardCommonSetting::Instance().m_dblScaleImageToGraph_W;

	CRect	rcGrapic;
	CPoint	ptOrigin(rcImageWindow.left, rcImageWindow.top);

	rcImage.OffsetRect( - ptOrigin );

	//////////////////////////////////////////////////////////////////////
	rcGrapic.left = (int)(rcImage.left * dblScaleImageToGraph_W + 0.5);
	rcGrapic.right = (int)(rcImage.right * dblScaleImageToGraph_W + 0.5);
	rcGrapic.top = (int)(rcImage.top * dblScaleImageToGraph_H + 0.5);
	rcGrapic.bottom = (int)(rcImage.bottom * dblScaleImageToGraph_H + 0.5);

	rcGrapic &= CRect(0, 0, szGraph.cx, szGraph.cy);
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	rcImage.left = (int)(rcGrapic.left / dblScaleImageToGraph_W + 0.5);
	rcImage.right = (int)(rcGrapic.right / dblScaleImageToGraph_W + 0.5);
	rcImage.top = (int)(rcGrapic.top / dblScaleImageToGraph_H + 0.5);
	rcImage.bottom = (int)(rcGrapic.bottom / dblScaleImageToGraph_H + 0.5);
	//////////////////////////////////////////////////////////////////////

	rcImage.OffsetRect( ptOrigin );

	return rcGrapic;
}