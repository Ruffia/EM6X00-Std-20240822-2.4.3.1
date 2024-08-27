// AutoTest.h : main header file for the AUTOTEST DLL
//

#if !defined(AFX_AUTOTEST_H__976834A6_B188_4A21_BCB0_C8D3211AFF12__INCLUDED_)
#define AFX_AUTOTEST_H__976834A6_B188_4A21_BCB0_C8D3211AFF12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "SemCtrl.h"
#include "SemCtrlPanelManager.h"
#include <math.h>

/////////////////////////////////////////////////////////////////////////////
// CAutoTestApp
// See AutoTest.cpp for the implementation of this class
//

class CAutoTestApp : public CWinApp
{
public:
	CAutoTestApp();

	DWORD	SemCoreCtrlPanelMessageProc(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber, const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag);
	double	GetPhysicsFromAdcs( int nIndex, double dVadc );

	void	ChangedADCselectOB( int nNum );
	void	ChangedADCsChSelect( int nChan );
	long	ReadSelectedADC( BOOL bAnalyze = TRUE );
	CString	ReadSelectedADC_Procedure( DWORD dwMilliseconds, BOOL bLogging = FALSE );
	CString	ReadSelectedADC_Logging( DWORD dwMilliseconds, double &dValue );
	CString	ReadSelectedADC_Logging2( DWORD dwMilliseconds, double &dValue );
	CString ReadSelectedADC_Logging_onetime( DWORD dwMilliseconds, double &dValue, BOOL bDelay = FALSE );	// 13.05.09 配合Logging试验设置的变量
	CString	ADCvconv_Logging( double &dValue, BOOL bLog = TRUE );

	void	OnlySendReadSelectedADCcmd( logParam lP);
	CString	ADCvconv_Logging( int nData1, int nData2, logParam lP, double &dValue, BOOL bLog = TRUE );
	CString	ReadComprs_Logging();
	CString	ReadComprs_Logging( int nData );

	void	MCI_Det03SB_Change( int nPos, int nChn );
	void	MCI_Det45SB_Change( int nPos, int nChn );
	void	MCI_DetGainSB_Change( int nPos, int nChn );
	void	MCI_BWselSB_Change( int nPos, int nChn );
	void	MCI_UpdateSpotX( int nPos, BOOL bUpdate );
	void	MCI_UpdateSpotY( int nPos, BOOL bUpdate );
	void	SCN_XstigDACSB_Change( int nPos );
	void	SCN_YstigDACSB_Change( int nPos );
	void	SCN_XshiftDACSB_Change( int nPos );
	void	SCN_YshiftDACSB_Change( int nPos );
	void	SCN_XxoverDACSB_Change( int nPos );
	void	SCN_YxoverDACSB_Change( int nPos );
	void	LNS_L1DACSB_Change( int nPos );
	void	LNS_L2SDACSB_Change( int nPos );
	void	LNS_L2DDACSB_Change( int nPos );
	void	LNS_XalDACSB_Change( int nPos );
	void	LNS_YalDACSB_Change( int nPos );
	void	LNS_WobDACSB_Change( int nPos );
	void	SPR_DACupdate( int nPos );

	HANDLE	m_hTimeDelay;
	void	Delay( int msecond );

	long	BoardRead0816( int nRsize, int nModID, BYTE bCktAddr, WORD wWdata, CString& strMsgCounts, CString& strMsgData, BOOL bAnalyze );
	long	BoardRead2432( int nRsize, int nModID, BYTE bCktAddr, DWORD dwWdata, CString& strMsgCounts, CString& strMsgData, BOOL bAnalyze );
	void	BoardWrite0816( int nWsize, int nModID, BYTE bCktAddr, WORD wWdata );
	void	BoardWrite24( int nWsize, int nModID, BYTE bCktAddr, DWORD dwWdata );

	int		ReadInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator=',');
	int		SaveInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator=',');
	void	CFGfile_InitParams();
	void	CFGfile_InitChangingParams( char *path );
	void	CFGfile_InitLoggingParams( char *path );
	void	CFGfile_InitSelectedADC(char *path);
	void	CFGfile_InitAutoTestParams(char *path);

	AutoTest_Param	m_AT;
	Cali_Param		m_Cali;
	DL_Sum_Param	m_DL;
	MCI_Param		m_BrdMCI;

	BOOL	m_bDiagLogging;		// 13.01.05 诊断记录标志

//	int		m_nDACTCvalue[12];
//	int		m_nSPRDACvalues[4];
	int		m_nBoardReadDataBuf[256*2];

	int		m_nRBwdCnt;
	int		m_nADCnum, m_nADCchn, m_nADCmid, m_nADCcid;	// Module Selection ID, Circuit Selection ID
	double	m_dADCcurrentMeas, m_dADCvoltsPerAmp, m_dADCampsPerVolt, m_dADCvref, m_dADCCOMoffset;
	BOOL	m_bADCtype, m_bADCdiff;

	/////////////////////////////////////////////////////////////////
	unsigned short*	m_pnReadComprData;
	unsigned short*	m_pnReadADCData;
//	unsigned short*	m_pnReadCommonData;
	
//	double*	m_pGraphDataBuff[GraphDataBuffGroups];
//	double* m_pDiffGraphDataBuff[GraphDataBuffGroups];
	/////////////////////////////////////////////////////////////////

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoTestApp)
	public:
	virtual int ExitInstance();
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAutoTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CAutoTestApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOTEST_H__976834A6_B188_4A21_BCB0_C8D3211AFF12__INCLUDED_)
