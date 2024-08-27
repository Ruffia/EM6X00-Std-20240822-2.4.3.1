#pragma once
#include "USB_Functions.h"
#include "SemCtrl.h"

class AFX_EXT_CLASS CBoardComm 
{
public:
	static CBoardComm& Instance();
	void    Release();

	long	BoardRead0816( int nRsize, int nModID, BYTE bCktAddr, WORD wWdata, CString& strMsgCounts, CString& strMsgData, BOOL bAnalyze);
	long	BoardRead2432( int nRsize, int nModID, BYTE bCktAddr, DWORD dwWdata, CString& strMsgCounts, CString& strMsgData, BOOL bAnalyze);
	void	BoardWrite0816( int nWsize, int nModID, BYTE bCktAddr, WORD wWdata);
	void	BoardWrite24( int nWsize, int nModID, BYTE bCktAddr, DWORD dwWdata);

	void    ResetADC();
	CString	ReadComprs_Logging();
	void	ChangedADCselectOB( int nNum );
	void	ChangedADCsChSelect( int nChan,ADCchnParam	chn[] );
	long	ReadSelectedADC( BOOL bAnalyze = TRUE );
	CString	ReadSelectedADC_Procedure( DWORD dwMilliseconds, DL_ADCs_Param& adc, DL_Logging_Param& log, BOOL bLogging = FALSE );
	CString	ReadSelectedADC_Logging( DWORD dwMilliseconds, double &dValue, ADCchnParam  chn[] );
	CString	ReadSelectedADC_Logging2( DWORD dwMilliseconds, double &dValue, DL_ADCs_Param& adc, DL_Logging_Param& log );
	CString ReadSelectedADC_Logging_onetime( DWORD dwMilliseconds, double &dValue, ADCchnParam chn[], BOOL bDelay = FALSE );	// 13.05.09 配合Logging试验设置的变量
	CString	ADCvconv_Logging( double &dValue, ADCchnParam chn[],BOOL bLog = TRUE );

	void	OnlySendReadSelectedADCcmd( logParam lP);
	CString	ADCvconv_Logging( int nData1, int nData2, logParam lP, double &dValue, ADCchnParam chn[], BOOL bLog = TRUE );
	CString	ReadComprs_Logging( int nData );
	double	GetPhysicsFromAdcs( int nIndex, double dVadc, CString strCalc[] );

	double  GetChnRangeMin(ADCchnParam chn[]);
	double  GetChnRangeMax(ADCchnParam chn[]);

	DWORD   GetADCReadRaw();

protected:
	CBoardComm();

public:
	int		m_nADCnum, m_nADCchn, m_nADCmid, m_nADCcid;	// Module Selection ID, Circuit Selection ID
	double	m_dADCcurrentMeas, m_dADCvoltsPerAmp, m_dADCampsPerVolt, m_dADCvref, m_dADCCOMoffset;
	BOOL	m_bADCtype, m_bADCdiff;

	int		m_nBoardReadDataBuf[256*2];
	int		m_nRBwdCnt;
	BOOL    m_bBoardsReady;

	/////////////////////////////////////////////////////////////////
	unsigned short*	m_pnReadComprData;
	unsigned short*	m_pnReadADCData;
	unsigned short*	m_pnReadCommonData;
};
