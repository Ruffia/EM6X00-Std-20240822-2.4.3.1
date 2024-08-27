#include "stdafx.h"
#include <math.h>
#include "ScanParameterManager.h"
#include "SemCoreAssemble.h"
#include "CommonFunctionManager.h"

CScanParameterManager& CScanParameterManager::Instance()
{
	static CScanParameterManager Inst;
	return Inst;
}

CScanParameterManager::CScanParameterManager(): m_sizeReso(1024,768)
{
	m_usbFunction		= USB_SF_RasterScan;
	m_nusbScanResoIndex	= 4;
	m_nusbPixRate		= 2;					// 1us
	m_nusbPixRateIndex	= 1;					// 扫描速度为1档
	m_nusbPhotoPixRate	= 128;					// 64us
	m_nusbPhotoPixRateIndex	= 4;//11;			// pix clk = 20us, actual dwell time = 80us
	//	m_usbParam			= ;
	m_nusbScanStoreMode	= 1;					// XYZ存储方式
	m_nusbScanDetMode	= 0;					// 12.02.15 单探测器模式

	m_dResoRate = 1.0;

	m_nLineScanParam = m_sizeReso.cy / 2;

	m_ptPointScanParam.x = m_sizeReso.cx / 2;
	m_ptPointScanParam.y = m_sizeReso.cy / 2;

	//	1024	512		256
	//	 768	384		192
	m_rcAreaScanParam.left		= 256;
	m_rcAreaScanParam.top		= 192;
	m_rcAreaScanParam.right		= 256 + 512;
	m_rcAreaScanParam.bottom	= 192 + 384;

	//	1024	512		256
	//	 768	384		192
	m_rcStaticAreaScanParam.left	= 256;
	m_rcStaticAreaScanParam.top		= 192;
	m_rcStaticAreaScanParam.right	= 256 + 512;
	m_rcStaticAreaScanParam.bottom	= 192 + 384;

	//	1024	512		256
	//	 768	384		192
	m_rcAutoBCAreaScanParam.left	= (1024 - 512) / 2;
	m_rcAutoBCAreaScanParam.top		= (768 - 384) / 2;
	m_rcAutoBCAreaScanParam.right	= 512 + m_rcAutoBCAreaScanParam.left;
	m_rcAutoBCAreaScanParam.bottom	= 384 + m_rcAutoBCAreaScanParam.top;

	//	1024	512		256
	//	 768	384		192
	m_rcAutoFocusAreaScanParam.left		= (1024 - 256) / 2;
	m_rcAutoFocusAreaScanParam.top		= (768 - 256) / 2;
	m_rcAutoFocusAreaScanParam.right	= 256 + m_rcAutoFocusAreaScanParam.left;
	m_rcAutoFocusAreaScanParam.bottom	= 256 + m_rcAutoFocusAreaScanParam.top;

	//	1024	512		256
	//	 768	384		192
	m_rcAutoAstigmatAreaScanParam.left		= (1024 - 256) / 2;
	m_rcAutoAstigmatAreaScanParam.top		= (768 - 256) / 2;
	m_rcAutoAstigmatAreaScanParam.right		= 256 + m_rcAutoAstigmatAreaScanParam.left;
	m_rcAutoAstigmatAreaScanParam.bottom	= 256 + m_rcAutoAstigmatAreaScanParam.top;

	//	1024	512		256
	//	 768	384		192
	m_rcDualMagParam.left	= 128;
	m_rcDualMagParam.top	= 192;
	m_rcDualMagParam.right	= 128 + 256;
	m_rcDualMagParam.bottom	= 192 + 384;

	USB_SetScanRectParam();
}


void CScanParameterManager::USB_SetScanRectParam()
{
	// usb param
	m_usbParam.RasterScan.dwStartX	= 0;
	m_usbParam.RasterScan.dwStartY	= 0;
	m_usbParam.RasterScan.dwStopX	= m_sizeReso.cx;
	m_usbParam.RasterScan.dwStopY	= m_sizeReso.cy;

	m_nLineScanParam = m_sizeReso.cy / 2;

	m_ptPointScanParam.x = m_sizeReso.cx / 2;
	m_ptPointScanParam.y = m_sizeReso.cy / 2;

	//	1024	512		256
	//	 768	384		192
	m_rcAreaScanParam.left		= m_sizeReso.cx * 5 / 16;
	m_rcAreaScanParam.top		= m_sizeReso.cy * 5 / 16;
	m_rcAreaScanParam.right		= m_sizeReso.cx * 11 / 16;
	m_rcAreaScanParam.bottom	= m_sizeReso.cy * 11 / 16;

	//	1024	512		256
	//	 768	384		192
	m_rcStaticAreaScanParam.left	= m_sizeReso.cx /2 -128;
	if( m_rcStaticAreaScanParam.left < 0 )
		m_rcStaticAreaScanParam.left = 0;
	m_rcStaticAreaScanParam.top		= m_sizeReso.cy /2 -128;
	if( m_rcStaticAreaScanParam.top < 0 )
		m_rcStaticAreaScanParam.top = 0;
	m_rcStaticAreaScanParam.right	= m_sizeReso.cx /2 +128;
	if( m_rcStaticAreaScanParam.right > m_sizeReso.cx )
		m_rcStaticAreaScanParam.right = m_sizeReso.cx;
	m_rcStaticAreaScanParam.bottom	= m_sizeReso.cy /2 +128;
	if( m_rcStaticAreaScanParam.bottom > m_sizeReso.cy )
		m_rcStaticAreaScanParam.bottom = m_sizeReso.cy;

	m_rcAutoBCAreaScanParam			= m_rcStaticAreaScanParam;
	m_rcAutoFocusAreaScanParam		= m_rcStaticAreaScanParam;
	m_rcAutoAstigmatAreaScanParam	= m_rcStaticAreaScanParam;

	//	1024	512		256
	//	 768	384		192
	m_rcDualMagParam.left	= m_sizeReso.cx / 8;
	m_rcDualMagParam.top	= m_sizeReso.cy / 4;
	m_rcDualMagParam.right	= m_sizeReso.cx * 3 / 8;
	m_rcDualMagParam.bottom	= m_sizeReso.cy * 3 / 4;
}


void CScanParameterManager::USB_GetResoSize( int nScanResolution )
{
	m_sizeReso.cx = m_nResos[m_nResoX[m_nusbScanResoIndex]];
	if( m_dResoRate < 0 )
		m_sizeReso.cy = m_nResos[m_nResoY[m_nusbScanResoIndex]];
	else
		m_sizeReso.cy = (long)(m_nResos[m_nResoX[m_nusbScanResoIndex]] *m_dResoRate);
}


void CScanParameterManager::InitScanParam() 
{
	m_scanParamLive.nScan_Type		= USB_ST_RasterScan;
	m_scanParamLive.nScan_Speed		= 1;	// 500ns
	m_scanParamLive.nSync_Speed		= m_nusbPhotoPixRate;
	m_scanParamLive.nScan_ResoX		= 512;	// 512*384
	m_scanParamLive.nScan_ResoY		= 384;
	m_scanParamLive.nScan_left		= 0;
	m_scanParamLive.nScan_top		= 0;
	m_scanParamLive.nScan_right		= 512;
	m_scanParamLive.nScan_bottom	= 384;
	m_scanParamLive.nSplit			= 0;
	m_scanParamLive.nDet2			= 0;
	m_scanParamLive.nStoreMode		= 1;
}

bool  CScanParameterManager::NotifyPixRate()
{
	if(m_nusbPixRate == m_nPixRateIndex[m_nusbPixRateIndex])
	{
		return false;
	}

	m_nusbPixRate = _GetPixRate(m_nPixRateIndex[m_nusbPixRateIndex]);

	// 扫描速度改变时，要通知DFocus重新计算Valley个数
	CSemCoreAssemble::Instance().m_DFocus.SetParams( 16, m_nusbPixRate );
	return true;
}


void  CScanParameterManager::ReadFromINI(char* path)
{
	char szBuf[64] = {0};

	// 读ini中的分辨率
	int* pnReso = new int[RESO_COUNT];
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, "Options", "Resos", INTSTYLE, RESO_COUNT, pnReso) == RESO_COUNT )
	{
		for(int i=0; i<RESO_COUNT; i++ )
			m_nResos[i] = pnReso[i];
	}
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, "Options", "ResoX", INTSTYLE, 8, pnReso) == 8 )
	{
		for(int i=0; i<8; i++ )
			m_nResoX[i] = pnReso[i];
	}
	else
	{
		for(int i=0; i<8; i++ )
			m_nResoX[i] = (int)(pow((double)2,i) *16);
	}
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, "Options", "ResoY", INTSTYLE, 8, pnReso) == 8 )
	{
		for(int i=0; i<8; i++ )
			m_nResoY[i] = pnReso[i];
	}
	else
	{
		for(int i=0; i<8; i++ )
			m_nResoY[i] = (int)(pow((double)2,i) *16);
	}

	// 2021.06 放宽图像分辨率：
	// 通过修改ini中的ResosIndex数组值，
	// 改变给用户显示的图像分辨率的起始值和终止值
	// 普通用户使用4,17，即图像分辨率从256开始到4096
	// 特殊用户可以使用0,17，即图像分辨率从16开始到4096（也可0,12，即只到1024）
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, "Options", "ResosIndex", INTSTYLE, 2, pnReso) == 2 )
	{
		for(int i=0; i<2; i++ )
			m_nResosIndex[i] = pnReso[i];
		if( m_nResosIndex[0] > m_nResosIndex[1] )
			m_nResosIndex[0] = m_nResosIndex[1] -1;
		if( m_nResosIndex[0] < 0 )
			m_nResosIndex[0] = 0;
		if( m_nResosIndex[1] > RESO_COUNT -1 )
			m_nResosIndex[1] = RESO_COUNT -1;
	}
	else
	{
		m_nResosIndex[0] = 4;
		m_nResosIndex[1] = RESO_COUNT -1;
	}

	delete [] pnReso;

	// 根据新读取的起始值和终止值，调整分辨率数组，防止越界
	for(int i=4; i<8; i++ )
	{
		if( m_nResoX[i] > m_nResosIndex[1] )
			m_nResoX[i] = m_nResosIndex[1];
		else if( m_nResoX[i] < m_nResosIndex[0] )
			m_nResoX[i] = m_nResosIndex[0];

		if( m_nResoY[i] > m_nResosIndex[1] )
			m_nResoY[i] = m_nResosIndex[1];
		else if( m_nResoY[i] < m_nResosIndex[0] )
			m_nResoY[i] = m_nResosIndex[0];
	}


	m_dResoRate = 0.75;
	if(GetPrivateProfileString("Options", "ResoR", "", szBuf, 32, path) != 0)
	{
		m_dResoRate = atof( szBuf );
	}

	// 读ini中的扫描速度
	//unsigned short g_nPixRateIndex[9] = { 1, 2, 4, 8, 16, 32, 64, 128, 256 };//40, 80, 128 };
	int* pnPixRate = new int[9];
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, "Options", "PixRate", INTSTYLE, 9, pnPixRate) == 9 )
	{
		for(int i=0; i<9; i++ )
			m_nPixRateIndex[i] = pnPixRate[i];
	}
	else
	{
		for(int i=0; i<9; i++ )
			m_nPixRateIndex[i] = (int)(pow((double)2,i));
	}
	
	// 2021.06 放宽扫描速度：
	// 通过修改ini中的PixRatesIndex数组值，
	// 改变给用户显示的扫描速度的起始值和终止值
	// 普通用户使用0,80，即扫描速度从500ns开始到80us
	// 特殊用户可以使用0,256，即扫描速度从500ns开始到256us
	// 目前起始值只支持0，终止值可改。如以后有需要，起始值也变为可变值
	if( CCommonFunctionManager::Instance().ReadInOneLine(path, "Options", "PixRatesIndex", INTSTYLE, 2, pnPixRate) == 2 )
	{
		for(int i=0; i<2; i++ )
			m_nPixRatesIndex[i] = pnPixRate[i];
		if( m_nPixRatesIndex[0] > m_nPixRatesIndex[1] )
			m_nPixRatesIndex[0] = m_nPixRatesIndex[1] -1;
		if( m_nPixRatesIndex[0] < 0 )
			m_nPixRatesIndex[0] = 0;
		if( m_nPixRatesIndex[1] > 256 )
			m_nPixRatesIndex[1] = 256;
	}
	else
	{
		m_nPixRatesIndex[0] = 0;
		m_nPixRatesIndex[1] = 80;
	}

	delete [] pnPixRate;

	m_nusbPhotoPixRate = GetPrivateProfileInt( "Options", "PhotoPixRate", 0, path );
	m_nSEratio = GetPrivateProfileInt( "Options", "SEratio", 50, path );
}

int CScanParameterManager::_GetPixRate(const int nPixRateIndex)
{
	int nRealPixRate = nPixRateIndex;
	if (nPixRateIndex == 81 *2)
	{
		nRealPixRate = 120;
	}
	else if (nPixRateIndex == 82 * 2)
	{
		nRealPixRate = 160;
	}
	else if (nPixRateIndex == 83 * 2)
	{
		nRealPixRate = 200;
	}
	else if (nPixRateIndex == 84 * 2)
	{
		nRealPixRate = 240;
	}

	return nRealPixRate;
}

void  CScanParameterManager::SavePixRate2INI(char* path,int nPixRateIndex1,int nPixRateIndex2,int nPixRateIndex3,int nPixRateIndex4)
{
	// 扫描速度
	m_nPixRateIndex[1] = (nPixRateIndex1 == 0 ? 1 : nPixRateIndex1 *2);
	m_nPixRateIndex[2] = (nPixRateIndex2 == 0 ? 1 : nPixRateIndex2 *2);
	m_nPixRateIndex[3] = (nPixRateIndex3 == 0 ? 1 : nPixRateIndex3 *2);
	m_nPixRateIndex[4] = (nPixRateIndex4 == 0 ? 1 : nPixRateIndex4 *2);

	// 将扫描速度写入配置文件
	CCommonFunctionManager::Instance().SaveInOneLine(path, "Options", "PixRate", INTSTYLE, 9, m_nPixRateIndex );
}