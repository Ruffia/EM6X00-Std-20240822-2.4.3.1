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
			unsigned short&				nPixRate,	// ������ = ��ͣ��ʱ�� /4������Ϊ����
			USB_ExtrParamRasterScan&	Param,
			double						dHV = 30.0 );
	
	BOOL	USB_SFLineScan(
			CSize&					szReso,
			unsigned short&			nPixRate,		// ������ = ��ͣ��ʱ�� /4������Ϊ����
			USB_ExtrParamLineScan&	Param,
			double					dHV = 30.0 );
	
	BOOL	USB_SFSpotScan(
			CSize&					szReso,
			unsigned short&			nPixRate,		// ������ = ��ͣ��ʱ�� /4������Ϊ����
			USB_ExtrParamLineScan&	Param,
			double					dHV = 30.0 );
	
	BOOL	USB_SFSyncScan(
			CSize&					szReso,
			unsigned short&				nPixRate,	// ������ = ��ͣ��ʱ�� /4������Ϊ����
			USB_ExtrParamRasterScan&	Param,
			double						dHV = 30.0 );
	
// 	BOOL	USB_SFDualMagnification(
// 			USB_ScanSpeed&						Speed,
// 			USB_ExtrParamDualMagnification&		Param );

	CSize					m_sizeReso;				// �ֱ���
	USB_ScanFunction		m_usbFunction;			// ɨ�跽ʽ
	unsigned short			m_nusbPixRate;
	int						m_nXOffset, m_nYOffset;	// Ϊ��֤�ͷŴ�����ͼ����༰�϶�û�л������Ҫ�ù������ݸ���
	int		m_nDataStoreMode;						// ���ݴ洢ģʽ
	BOOL	m_bNeedMon;								// �ɼ�ʱ�Ƿ���Ҫ�������
	int		m_nMCIdetPair, m_nMCIfilter[3];			// MCI�������ͨ������˲���

	///////////////////////////////////////////////////////////////////////////
	BOOL					m_bDebugScan;			// ����ʱ������MCI�����ֶ��޸ĵĲɼ�������ɨ�貨�β���������ʱ��
	USB_InitScanRampParam	m_usbDebugParamX;		// DebugMode����ʱ���洢�ֶ����õ�X���β���
	USB_InitScanRampParam	m_usbDebugParamY;		// DebugMode����ʱ���洢�ֶ����õ�Y���β���
	unsigned short			m_nusbDebugPixRate;		// DebugMode����ʱ���洢�ֶ����õ�����ʱ��Nֵ
	BOOL					m_bSplit;				// �ָ�ɨ�裬����˵��̽����ɨ��
	void	Get_DataSize_Interval_Debug(
			DWORD &dwDataSize, DWORD &dwInterval );
	void	Get_DataSize_Interval_DebugEx(
			DWORD &dwDataSize, DWORD &dwInterval );
	///////////////////////////////////////////////////////////////////////////

	BOOL	USB_SetGraph( 
			USB_ScanFunction&		Function,		// ������ʽ
			CSize&					szReso,			// �ֱ���
			USB_ScanParam&			Param );		// ɨ�����

	BOOL	USB_ClearScan();
	void	USB_NormalScan();

	void	USB_SetRampParam(
//			USB_ScanFunction&		Function,	
			CSize					szReso, 
			CRect rcFrame, USB_InitData& data,
			double	dHV = 30.0 );							// �����Ķ�ɨ�貨����
	void	USB_SetRampParam2(
//			USB_ScanFunction&		Function,	
			CSize					szReso, 
			CRect rcFrame, USB_InitData& data,
			double	dHV = 30.0 );							// �����Ķ�ɨ�貨����
	BOOL	USB_WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData );	// ��MCI��д����
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
			USB_ScanFunction&		Function,		// ������ʽ
			CSize&					szReso,			// �ֱ���
			unsigned short&			nPixRate,		// ������ = ��ͣ��ʱ�� /4������Ϊ����
			USB_ScanParam&			Param,			// ɨ�����
			double					dHV );			// ��ѹ
	void	USB_SetRampParam_CoadjustHV( double dHV );	// ��ѹͳ��ʱ�����ڲɼ�����ֻͳ��ɨ�貨�β���

	void	USB_SetParams( int nType, int nParam );	// ���ò���

	BOOL	USB_ScanStop();
	/////////////////////////////////////////////
};

#endif // !defined(AFX_SCAN_H__E95CDE64_9F4A_4694_A77E_4A88960D3E76__INCLUDED_)
