// Scan.h: interface for the CScan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCAN_H__E95CDE64_9F4A_4694_A77E_4A88960D3E76__INCLUDED_)
#define AFX_SCAN_H__E95CDE64_9F4A_4694_A77E_4A88960D3E76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include "ScanControlStructure.h"
#include <afxmt.h>
#include "MCICardParam.h"
#include "USB_Functions.h"

//////////////////////////////////////////////////////////////////////

class CBasisScan;

class CUsbDI16A;

class AFX_EXT_CLASS CScan  
{
//	CriticalResource		m_criticalOperation;
	static	CCriticalSection		m_criticalOperation;
	
	CBasisScan*				m_pScanCtrl;

	BOOL					m_bReady;	
	
	DWORD					m_dwExpectantDMABlockSize;
	DWORD					m_dwExpectantInterval;
	CRect					m_rcDestFrame;

	BOOL	USB_SetInitData( USB_InitData* pData );
	BOOL	USB_StartRasterScan();
	BOOL	USB_StartAnalogPhoto();
	BOOL	USB_StartDigitalPhoto();
	BOOL	USB_StartLineScan();
	BOOL	USB_StartSpotScan();
	BOOL	USB_StartSyncScan();

	BOOL	USB_SFRasterScan(
			USB_ScanFunction&			Function,	
			CSize&						szReso,
			unsigned short&				nPixRate,	// 像素率 = 点停留时间 /4，必须为整数
			USB_ExtrParamRasterScan&	Param,
			double						dHV = 30.0 );
	
	BOOL	USB_SFLineScan(
			CSize&					szReso,
			unsigned short&			nPixRate,		// 像素率 = 点停留时间 /4，必须为整数
			USB_ExtrParamLineScan&	Param,
			double					dHV = 30.0 );
	
	BOOL	USB_SFSpotScan(
			CSize&					szReso,
			unsigned short&			nPixRate,		// 像素率 = 点停留时间 /4，必须为整数
			USB_ExtrParamLineScan&	Param,
			double					dHV = 30.0 );
	
	BOOL	USB_SFSyncScan(
			CSize&					szReso,
			unsigned short&				nPixRate,	// 像素率 = 点停留时间 /4，必须为整数
			USB_ExtrParamRasterScan&	Param,
			double						dHV = 30.0 );
	
// 	BOOL	USB_SFDualMagnification(
// 			USB_ScanSpeed&						Speed,
// 			USB_ExtrParamDualMagnification&		Param );

	CSize					m_sizeReso;				// 分辨率
	USB_ScanFunction		m_usbFunction;			// 扫描方式
	unsigned short			m_nusbPixRate;
	int						m_nXOffset, m_nYOffset;	// 为保证低放大倍数下图像左侧及上端没有畸变而需要让过的数据个数
	int		m_nDataStoreMode;						// 数据存储模式
	BOOL	m_bNeedMon;								// 采集时是否需要监测数据
	int		m_nMCIdetPair, m_nMCIfilter[3];			// MCI板参数：通道组和滤波器

	///////////////////////////////////////////////////////////////////////////
	BOOL					m_bDebugScan;			// 启用时，将向MCI设置手动修改的采集参数：扫描波形参数及像素时钟
	USB_InitScanRampParam	m_usbDebugParamX;		// DebugMode启用时，存储手动设置的X波形参数
	USB_InitScanRampParam	m_usbDebugParamY;		// DebugMode启用时，存储手动设置的Y波形参数
	unsigned short			m_nusbDebugPixRate;		// DebugMode启用时，存储手动设置的像素时钟N值
	BOOL					m_bSplit;				// 分割扫描，或者说多探测器扫描
	void	Get_DataSize_Interval_Debug(
			DWORD &dwDataSize, DWORD &dwInterval );
	void	Get_DataSize_Interval_DebugEx(
			DWORD &dwDataSize, DWORD &dwInterval );
	///////////////////////////////////////////////////////////////////////////

	BOOL	USB_SetGraph( 
			USB_ScanFunction&		Function,		// 工作方式
			CSize&					szReso,			// 分辨率
			USB_ScanParam&			Param );		// 扫描参数

	BOOL	USB_ClearScan();
	void	USB_NormalScan();

	void	USB_SetRampParam(
//			USB_ScanFunction&		Function,	
			CSize					szReso, 
			CRect rcFrame, USB_InitData& data,
			double	dHV = 30.0 );							// 设置四段扫描波参数
	void	USB_SetRampParam2(
//			USB_ScanFunction&		Function,	
			CSize					szReso, 
			CRect rcFrame, USB_InitData& data,
			double	dHV = 30.0 );							// 设置四段扫描波参数
	BOOL	USB_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData );	// 向MCI板写数据
// 	void	Get_DataSize_Interval(
// 			USB_ScanResolution		Resolution,
// 			USB_ScanSpeed			Speed,
// 			DWORD &dwDataSize, DWORD &dwInterval );
	void	Get_DataSize_IntervalEx(
			CSize					szReso,
			unsigned short			nPixRate,
			DWORD &dwDataSize, DWORD &dwInterval );
	/////////////////////////////////////////////

	LPBYTE	m_pPhotoBuff;

public:
	CScan();
	virtual ~CScan();

	BOOL	isReady(){ return m_bReady; }

	void	Bind(LPBYTE pPhotoBuff){ m_pPhotoBuff = pPhotoBuff; };
	
	BOOL	IsAutoStop();

	BOOL	GetErrorInformation( CString& csError );

	BOOL	StopScan();

	BOOL	USB_SetScanFunctionEx(
			USB_ScanFunction&		Function,		// 工作方式
			CSize&					szReso,			// 分辨率
			unsigned short&			nPixRate,		// 像素率 = 点停留时间 /4，必须为整数
			USB_ScanParam&			Param,			// 扫描参数
			double					dHV );			// 高压
	void	USB_SetRampParam_CoadjustHV( double dHV );	// 高压统调时若正在采集，则只统调扫描波形参数

	void	USB_SetParams( int nType, int nParam );	// 设置参数

	BOOL	USB_ScanStop();
	/////////////////////////////////////////////
};

#endif // !defined(AFX_SCAN_H__E95CDE64_9F4A_4694_A77E_4A88960D3E76__INCLUDED_)
