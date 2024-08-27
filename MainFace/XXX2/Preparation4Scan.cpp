#include "StdAfx.h"
#include "Preparation4Scan.h"
using namespace Prepare4Scan;
#include "FilmPhotoData.h"
#include "ScanView.h"
#include "XXX2.h"
#include "Factory.h"
#include "ControlLayer.h"
#include "ScanParameterManager.h"
#include "MCICardCommonSetting.h"

// ����ͼ��ɨ��
IMPLEMENT_FACTORY(IPrepare4Scan,CLASS_USB_ST_Stop,USB_ScanType,USB_ST_Stop)
void CLASS_USB_ST_Stop::Prepare(CScanView* pScanview,USB_ScanParam	&Param,USB_ScanFunction& Function, unsigned short &nPixRate)
{
	CSemCtrl::Instance().m_SemScan.USB_ScanStop();
	theApp.DisplayMsg( "Change Scan: scan stop" );
}


// ����ɨ��
IMPLEMENT_FACTORY(IPrepare4Scan,CLASS_USB_ST_RasterScan,USB_ScanType,USB_ST_RasterScan)
void CLASS_USB_ST_RasterScan::Prepare(CScanView* pScanview,USB_ScanParam &Param,USB_ScanFunction& Function, unsigned short &nPixRate)
{
	Function		= USB_SF_RasterScan;
	nPixRate		= CScanParameterManager::Instance().m_nusbPixRate;

	Param.RasterScan.dwStartX	= 0;
	Param.RasterScan.dwStartY	= 0;
	Param.RasterScan.dwStopX	= CMCICardCommonSetting::Instance().m_sizeReso.cx;
	Param.RasterScan.dwStopY	= CMCICardCommonSetting::Instance().m_sizeReso.cy;
}

// �ѡ��ɨ��
IMPLEMENT_FACTORY(IPrepare4Scan,CLASS_USB_ST_AreaScan,USB_ScanType,USB_ST_AreaScan)
void CLASS_USB_ST_AreaScan::Prepare(CScanView* pScanview,USB_ScanParam &Param,USB_ScanFunction& Function, unsigned short &nPixRate)
{
	Function		= USB_SF_AreaScan;
	nPixRate		= CScanParameterManager::Instance().m_nusbPixRate;

	Param.RasterScan.dwStartX	= CScanParameterManager::Instance().m_rcAreaScanParam.left;
	Param.RasterScan.dwStartY	= CScanParameterManager::Instance().m_rcAreaScanParam.top;
	Param.RasterScan.dwStopX	= CScanParameterManager::Instance().m_rcAreaScanParam.right;
	Param.RasterScan.dwStopY	= CScanParameterManager::Instance().m_rcAreaScanParam.bottom;
}


// ˮƽ��ɨ��
IMPLEMENT_FACTORY(IPrepare4Scan,CLASS_USB_ST_LineScan_HORZ,USB_ScanType,USB_ST_LineScan_HORZ)
void CLASS_USB_ST_LineScan_HORZ::Prepare(CScanView* pScanview,USB_ScanParam &Param,USB_ScanFunction& Function, unsigned short &nPixRate)
{
	Function		= USB_SF_LineScan_HORZ;
	nPixRate		= CScanParameterManager::Instance().m_nusbPixRate;

	Param.RasterScan.dwStartX	= 0;
	Param.RasterScan.dwStopX	= CMCICardCommonSetting::Instance().m_sizeReso.cx;
	Param.RasterScan.dwStartY	= CScanParameterManager::Instance().m_nLineScanParam;
	Param.RasterScan.dwStopY	= CScanParameterManager::Instance().m_nLineScanParam + 1;
}


// ��ֱ��ɨ��
IMPLEMENT_FACTORY(IPrepare4Scan,CLASS_USB_ST_LineScan_VERT,USB_ScanType,USB_ST_LineScan_VERT)
void CLASS_USB_ST_LineScan_VERT::Prepare(CScanView* pScanview,USB_ScanParam &Param,USB_ScanFunction& Function, unsigned short &nPixRate)
{
	Function		= USB_SF_LineScan_VERT;
	nPixRate		= CScanParameterManager::Instance().m_nusbPixRate;

	Param.RasterScan.dwStartX	= CScanParameterManager::Instance().m_nLineScanParam;
	Param.RasterScan.dwStopX	= CScanParameterManager::Instance().m_nLineScanParam + 1;
	Param.RasterScan.dwStartY	= 0;
	Param.RasterScan.dwStopY	= CMCICardCommonSetting::Instance().m_sizeReso.cy;
}



// ��ɨ��
IMPLEMENT_FACTORY(IPrepare4Scan,CLASS_USB_ST_SpotScan,USB_ScanType,USB_ST_SpotScan)
void CLASS_USB_ST_SpotScan::Prepare(CScanView* pScanview,USB_ScanParam &Param,USB_ScanFunction& Function, unsigned short &nPixRate)
{
	Function		= USB_SF_SpotScan;
	nPixRate		= CScanParameterManager::Instance().m_nusbPixRate;

	Param.RasterScan.dwStartX	= CScanParameterManager::Instance().m_ptPointScanParam.x;
	Param.RasterScan.dwStopX	= CScanParameterManager::Instance().m_ptPointScanParam.x + 1;
	Param.RasterScan.dwStartY	= CScanParameterManager::Instance().m_ptPointScanParam.y;
	Param.RasterScan.dwStopY	= CScanParameterManager::Instance().m_ptPointScanParam.y + 1;
}


// ģ������
IMPLEMENT_FACTORY(IPrepare4Scan,CLASS_USB_ST_AnalogPhoto,USB_ScanType,USB_ST_AnalogPhoto)
void CLASS_USB_ST_AnalogPhoto::Prepare(CScanView* pScanview,USB_ScanParam &Param,USB_ScanFunction& Function, unsigned short &nPixRate)
{
	pScanview->m_bStageOrigin	= FALSE;
	CSemVirtualMCICard::Instance().GraphClear( pScanview->m_rcStageDragRectLast );
	Function		= USB_SF_AnalogPhoto;
	nPixRate		= CScanParameterManager::Instance().m_nusbPhotoPixRate;
	//		nPixRate		= m_nusbPixRate;
	{
		// ����ʱ����ǰͨ�����Filter����Ϊ5
		WORD wValue = (WORD)( (CControlLayer::Instance().m_BrdMCI.nDetPair << 8) | 0x05 );
		//			CSemCtrl::Instance().USB_WriteToBoard( USB_SetDetFilter, TRUE, wValue );
	}

	//		CSemCtrl::Instance().m_SemScan.USB_ScanStop();	// 2003-8-29
	MakeFilmPhotoDataByGraphLay();

	//����ʱֹͣ����ˢ�¶�ʱ��05.08.12
	const UINT nAutoTimerDate				= 110;
	pScanview->KillTimer( nAutoTimerDate );
}


// �̶�ѡ��ɨ��	AutoBC
IMPLEMENT_FACTORY(IPrepare4Scan,CLASS_USB_ST_AutoBCArea,USB_ScanType,USB_ST_AutoBCArea)
void CLASS_USB_ST_AutoBCArea::Prepare(CScanView* pScanview,USB_ScanParam &Param,USB_ScanFunction& Function, unsigned short &nPixRate)
{
	Function		= USB_SF_RasterScan;
	nPixRate		= CScanParameterManager::Instance().m_nusbPixRate;

	Param.RasterScan.dwStartX	= CScanParameterManager::Instance().m_rcAutoBCAreaScanParam.left;
	Param.RasterScan.dwStartY	= CScanParameterManager::Instance().m_rcAutoBCAreaScanParam.top;
	Param.RasterScan.dwStopX	= CScanParameterManager::Instance().m_rcAutoBCAreaScanParam.right;
	Param.RasterScan.dwStopY	= CScanParameterManager::Instance().m_rcAutoBCAreaScanParam.bottom;
}


// �̶�ѡ��ɨ��	AutoFocus
IMPLEMENT_FACTORY(IPrepare4Scan,CLASS_USB_ST_AutoFocusArea,USB_ScanType,USB_ST_AutoFocusArea)
void CLASS_USB_ST_AutoFocusArea::Prepare(CScanView* pScanview,USB_ScanParam &Param,USB_ScanFunction& Function, unsigned short &nPixRate)
{
	Function		= USB_SF_RasterScan;
	nPixRate		= CScanParameterManager::Instance().m_nusbPixRate;

	Param.RasterScan.dwStartX	= CScanParameterManager::Instance().m_rcAutoFocusAreaScanParam.left;
	Param.RasterScan.dwStartY	= CScanParameterManager::Instance().m_rcAutoFocusAreaScanParam.top;
	Param.RasterScan.dwStopX	= CScanParameterManager::Instance().m_rcAutoFocusAreaScanParam.right;
	Param.RasterScan.dwStopY	= CScanParameterManager::Instance().m_rcAutoFocusAreaScanParam.bottom;
}


// �̶�ѡ��ɨ�� AutoAstigmat
IMPLEMENT_FACTORY(IPrepare4Scan,CLASS_USB_ST_AutoAstigmatArea,USB_ScanType,USB_ST_AutoAstigmatArea)
void CLASS_USB_ST_AutoAstigmatArea::Prepare(CScanView* pScanview,USB_ScanParam &Param,USB_ScanFunction& Function, unsigned short &nPixRate)
{
	Function		= USB_SF_RasterScan;
	nPixRate		= CScanParameterManager::Instance().m_nusbPixRate;

	Param.RasterScan.dwStartX	= CScanParameterManager::Instance().m_rcAutoAstigmatAreaScanParam.left;
	Param.RasterScan.dwStartY	= CScanParameterManager::Instance().m_rcAutoAstigmatAreaScanParam.top;
	Param.RasterScan.dwStopX	= CScanParameterManager::Instance().m_rcAutoAstigmatAreaScanParam.right;
	Param.RasterScan.dwStopY	= CScanParameterManager::Instance().m_rcAutoAstigmatAreaScanParam.bottom;
}



