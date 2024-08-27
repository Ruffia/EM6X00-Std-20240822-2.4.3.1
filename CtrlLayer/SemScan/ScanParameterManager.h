#pragma once
#include "USB_Functions.h"

//时间：2021-02-25
//功能：管理USB扫描参数相关信息

class AFX_EXT_CLASS CScanParameterManager 
{
public:
	static CScanParameterManager& Instance();

	void	USB_SetScanRectParam();
	void	USB_GetResoSize( int nScanResolution );
	void    InitScanParam();
	bool	NotifyPixRate();
	void    ReadFromINI(char* path);
	void    SavePixRate2INI(char* path,int nPixRateIndex1,int nPixRateIndex2,int nPixRateIndex3,int nPixRateIndex4);
protected:
	CScanParameterManager();
	int    _GetPixRate(const int nPixRateIndex);

public:
	// USB
	unsigned short	m_nusbScanResoIndex;
	unsigned short	m_nusbPixRateIndex;
	unsigned short	m_nusbPhotoPixRateIndex;
	USB_ScanType	m_nusbScanType;
	USB_ScanType	m_nusbScanTypeSaved;
	unsigned short	m_nusbScanStoreMode;
	unsigned short	m_nusbScanDetMode;		// 12.02.15 探测器模式
	PublicParam_Scan	m_scanParamLive;
	PublicParam_Scan	m_scanParamBak;


	USB_ScanFunction		m_usbFunction;
	USB_ScanParam			m_usbParam;
	unsigned short			m_nusbPixRate;
	unsigned short			m_nusbPhotoPixRate;
	unsigned short			m_nusbPixRateSaved;

	long				m_nLineScanParam;
	CPoint				m_ptPointScanParam;
	CRect				m_rcAreaScanParam;
	CRect				m_rcStaticAreaScanParam;
	CRect				m_rcAutoBCAreaScanParam;
	CRect				m_rcAutoFocusAreaScanParam;
	CRect				m_rcAutoAstigmatAreaScanParam;
	CRect				m_rcDualMagParam;

	CSize               m_sizeReso;

	UINT	m_nPixRateIndex[9], m_nPixRatesIndex[2];;// = { 1, 2, 4, 8, 16, 32, 64, 128, 256 };//40, 80, 128 };
	int		m_nResos[RESO_COUNT], m_nResosIndex[2];
	int		m_nResoX[8];
	int		m_nResoY[8];
	double	m_dResoRate;
	int		m_nSEratio;
};
