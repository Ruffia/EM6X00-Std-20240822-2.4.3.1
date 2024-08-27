// insample0.h : main header file for the INSAMPLE0 application
//

#if !defined(AFX_INSAMPLE0_H__5D74169A_FA08_41B6_A584_26249D602878__INCLUDED_)
#define AFX_INSAMPLE0_H__5D74169A_FA08_41B6_A584_26249D602878__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "USB_Functions.h"

#include "AIOUSB.h"
#include "FT232HDevice.h"

#include "USB_Recv.h"
#include "USB_Send.h"

/////////////////////////////////////////////////////////////////////
#define	FLAGBIT( flag, flag_bit )	( ( (flag) & (flag_bit) ) == (flag_bit) )

const COLORREF g_crOFF			= RGB( 0xA0, 0xA0, 0xA0 );
const COLORREF g_crON_Test		= RGB( 0x00, 0xC0, 0x00 );
const COLORREF g_crON_Comm		= RGB( 0xFF, 0x00, 0x00 );
const COLORREF g_crSYS			= RGB( 0xD8, 0xE4, 0xF8 );
const COLORREF g_crScanEmphasis	= RGB( 0xFF, 0x80, 0x80 );
const COLORREF g_crImage		= RGB( 0xFF, 0x80, 0x00 );

const COLORREF g_crEmphasis1	= RGB( 0x80, 0x00, 0x80 );
const COLORREF g_crEmphasis2	= RGB( 0x00, 0x80, 0x00 );
const COLORREF g_crEmphasis3	= RGB( 0x00, 0x80, 0xFF );
const COLORREF g_crEmphasis4	= RGB( 0xFF, 0x00, 0x00 );

const COLORREF g_crImageBkg		= RGB( 160, 160, 160 );


#define tmrReadPort		1
#define	tmrReadFIFO		2
#define	tmrClear		3
#define	tmrWaitToInit	4

#define	tmrDiag			5

/////////////////////////////////////////////////////////////////////////////
//Read data in one line in *.ini.
//In one line the data like ***,*****,*****, they are seperated by comma.
const UINT INTSTYLE		= 1;
const UINT DOUBLESTYLE	= 2;
const UINT LONGSTYLE	= 3;
const UINT STRSTYLE		= 4;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
#define	Diag_Stig_X			0
#define	Diag_Stig_Y			1
#define	Diag_Shift_X		2
#define	Diag_Shift_Y		3
#define	Diag_xover_X		4
#define	Diag_xover_Y		5
#define	Diag_CL				6
#define	Diag_OL				7
#define	Diag_Align_X		8
#define	Diag_Align_Y		9
#define	Diag_Wob			10

#define	Diag_CalI_TC0		11
#define	Diag_CalI_TC1		12
#define	Diag_CalI_TC2		13
#define	Diag_CalI_TC3		14
#define	Diag_SetPoint_TC0	15
#define	Diag_SetPoint_TC1	16
#define	Diag_SetPoint_TC2	17
#define	Diag_SetPoint_TC3	18
#define	Diag_CalI_TC4		19
#define	Diag_CalI_TC5		20
#define	Diag_SetPoint_TC4	21
#define	Diag_SetPoint_TC5	22

#define	Diag_SPRDAC_ChA		23
#define	Diag_SPRDAC_ChB		24
#define	Diag_SPRDAC_ChC		25
#define	Diag_SPRDAC_ChD		26

#define	Diag_Offset_0		27
#define	Diag_Offset_1		28
#define	Diag_Offset_2		29
#define	Diag_Offset_3		30
#define	Diag_Offset_4		31
#define	Diag_Offset_5		32
#define	Diag_FineMag_X		33
#define	Diag_FineMag_Y		34

#define	Diag_Total			35

#define Logging_CL			0
#define Logging_OL			1
#define	Logging_Align_X		2
#define Logging_Align_Y		3
#define Logging_Wob			4
#define Logging_Comp_CL		5
#define Logging_Comp_OL		6
#define Logging_Comp_Align_X 7
#define Logging_Comp_Align_Y 8
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
typedef struct tagDL_Diag_Param
{
	int		nCheck;
	int		nMin;
	int		nMax;
	int		nStep;
	int		nIntv;
	int		nCurPos;
	int		nFinish;
	CString	strLabel;
}DL_Diag_Param;

typedef struct tagDL_Logging_Param
{
	BOOL	bLog[6*16];
	CString	strCalc[6*16];
	int		nCalc[6*16];
	BOOL	bLogParam[9];
	int		nCheckComp[4];
	double	dAve[100];
}DL_Logging_Param;

typedef struct tagDL_Graph_Param
{
	BOOL	bGraph;
	int		nIndex;
	long	lTimes;
	int		nZoomXIndex;
	int		nZoomYIndex;
	double	dXMin;
	double	dXMax;
	double	dYMin;
	double	dYMax;
}DL_Graph_Param;

struct ADCchnParam
{
	int		nNum;	// 0:SCN; 1:LNS; 2:VAC#0; 3:VAC#1; 4:SPR#0; 5:SPR#1
	int		nMID;	// 1:SCN; 2:LNS; 3:VAC; 4:SPR
	int		nCID;
	int		nChn;
	BOOL	bType;	// 1:2448; 0:1859

	// src
	CString	strLabel;
	BOOL	bDiff;
	double	dFactor;
	double	dMin;
	double	dMax;
	// src

	double	dVref;
	double	dCOMoffset;
	double	dCampsPerVolt;
};

typedef struct tagDL_ADCs_Param
{
	ADCchnParam	chn[6*16];			// 各ADC通道的参数
	int			nAveCounts;			// ADC回读平均次数
}DL_ADCs_Param;

struct logParam
{
	int		nType;	// 1:ADC; 2:gauge compr; 3:Ext. Switch

	// 下面都是ADC回读时的参数
	int		nNum;
	int		nMID;
	int		nCID;
	int		nChn;
	BOOL	bType;	// 1:2448; 0:1859
	BOOL	bDiff;	// 1:diff; 0:S.E.
};

typedef struct tagDL_Logging2_Param
{
	int			nCounts;
	logParam	logParams[6*16];
}DL_Logging2_Param;

typedef struct tagDL_Other_Param
{
	int		nElapseChanging;	// 间隔时间
	int		nElapseLogging;
	BOOL	bGaugeCompr;		// 是否记录gauge compr
	BOOL	bGraph;				// 是否绘图
	BOOL	bDebug;				// 是否记录单次回读值
	HANDLE	hLogFile;			// 记录文件
	long	lTimes;				// 当前记录次数
	long	lLoggingDataCounts;	// 13.05.09 Debug模式下，当前记录次数
}DL_Other_Param;

#define GraphDataBuffGroups	4
#define GraphDataBuffCounts	360000		// 按最快100ms一个数据计算，1秒10个，1分钟600个，1小时36000个，10小时360000个

typedef struct tagDiagLogging_Param
{
	DL_Diag_Param		diag[Diag_Total];
	DL_Logging_Param	log;
	DL_Graph_Param		graph[GraphDataBuffGroups];			// 绘图数据
	DL_ADCs_Param		adc;
	DL_Other_Param		other;
	DL_Logging2_Param	log2;
}DL_Sum_Param;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
typedef struct tagAutoTest_Param
{
	double*	pAutoTestPower;
	double*	pAutoTestPowerExp;
	double*	pAutoTestPowerErr;
	BOOL*	pPower_OutofRange;

	double*	pAutoTestVAC;
	BOOL	bVAC_OutofRange[22];

	double* pAutoTestLNS[4];			// 4条曲线
	int		nAutoTestLNS_Total[4];		// 记录每个曲线的数据个数
	int		nAutoTestLNS_OldPos[4];		// 记录做自动检测前参数的位置

	double* pAutoTestSCN[4];			// 4条曲线
	int		nAutoTestSCN_Total[4];		// 记录每个曲线的数据个数
	int		nAutoTestSCN_OldPos[4];		// 记录做自动检测前参数的位置

	double* pAutoTestMCI[4];			// 4条曲线
	int		nAutoTestMCI_Total[4];		// 记录每个曲线的数据个数
	int		nAutoTestMCI_OldPos[4];		// 记录做自动检测前参数的位置

	double* pAutoTestSPR[4];			// 4条曲线
	int		nAutoTestSPR_Total[4];		// 记录每个曲线的数据个数
	int		nAutoTestSPR_OldPos[4];		// 记录做自动检测前参数的位置

	HANDLE	hLogFile;					// 记录文件
}AutoTest_Param;

#define AutoTest_16bitDAC_Steps		24
#define AutoTest_16bitDAC_StepsEach	6

#define AutoTest_12bitDAC_Steps		16
#define AutoTest_12bitDAC_StepsEach	4

#define Cali_16bitDAC_Steps			32
#define Cali_12bitDAC_Steps			32
#define Cali_16bitDAC_dChange		2048	// 参数范围内取32个点，每次设置改变量为：65536 /32 = 2048
#define Cali_12bitDAC_dChange		128		// 参数范围内取32个点，每次设置改变量为：4096 /32 = 128
#define Cali_Ave_Counts				32		// 每个点读32次取平均值
#define Elapse_Cali_2448			3000	// 每个点读32次取平均值，所需时间：80 + 32 * 80 = 2640 ms
#define Elapse_Cali_1859			600		// 每个点读32次取平均值，所需时间：5 + 32 * 5 = 165 ms (16 +32 *16 = 528 ms)

typedef struct tagCali_Param
{
	double* m_pCaliLNS[4];
	int		m_nCaliLNS_Total[4];
	int		m_nCaliLNS_OldPos[4];

	double* m_pCaliSCN[4];
	int		m_nCaliSCN_Total[4];
	int		m_nCaliSCN_OldPos[4];

	double* m_pCaliMCI[4];
	int		m_nCaliMCI_Total[4];
	int		m_nCaliMCI_OldPos[4];

	double* m_pCaliSPR[4];
	int		m_nCaliSPR_Total[4];
	int		m_nCaliSPR_OldPos[4];

	double	m_dCaliCoC[10];
	double	m_dCaliCoK[10];
	BOOL	m_bCaliCoSel[10];
	int		m_nCaliCoCounts;

	HANDLE	hLogFile;
}Cali_Param;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// MCI Power readback
const	WORD	g_wPwrbitsRange	= 0x0FFF;	// 4095
const	double	g_dPwrbitsRange	= 4.096;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//button size
const int BUTTON_WIDTH	= 90;
const int BUTTON_HEIGHT	= 30;
const int BUTTON_INTV	= 120;

//buttons positions
//buttons positions
const CPoint BTNMCI_LOCATION(	20,								-10);
const CPoint BTNSCNLNS_LOCATION(20 + ( BUTTON_WIDTH +200 ),		-10);
const CPoint BTNVACSPR_LOCATION(20 + ( BUTTON_WIDTH +200 )*2,	-10);

const CPoint BTNPower_LOCATION(	20,										-10);
const CPoint BTNVac_LOCATION(	20 + ( BUTTON_WIDTH +BUTTON_INTV ),		-10);
const CPoint BTNLns_LOCATION(	20 + ( BUTTON_WIDTH +BUTTON_INTV )*2,	-10);
const CPoint BTNScn_LOCATION(	20 + ( BUTTON_WIDTH +BUTTON_INTV )*3,	-10);
const CPoint BTNMci_LOCATION(	20 + ( BUTTON_WIDTH +BUTTON_INTV )*4,	-10);
const CPoint BTNSpr_LOCATION(	20 + ( BUTTON_WIDTH +BUTTON_INTV )*4,	-10);
const CPoint BTNCo_LOCATION(	20 + ( BUTTON_WIDTH +BUTTON_INTV )*5,	-10);
const CPoint BTNMon_LOCATION(	5,										-10);
const CPoint BTNAdvance_LOCATION(5 + ( BUTTON_WIDTH +BUTTON_INTV ),		-10);
const CPoint BTNDebug_LOCATION(	5 + ( BUTTON_WIDTH +BUTTON_INTV )*2,	-10);
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
#include "ximage.h"

#define SEMIMAGE_SUPPORT_SEM	1
const int SEMIMAGE_FORMAT_SEM	= CMAX_IMAGE_FORMATS + 1;
const int SEM_IMAGE_FORMATS		= CMAX_IMAGE_FORMATS + 2;

struct DocType
{
public:
	int		nID;
	BOOL	bRead;
	BOOL	bWrite;
	const	TCHAR*	description;
	const	TCHAR*	ext;
};
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
#define WORDALIGNMENT_CENTER	1
#define WORDALIGNMENT_RIGHT		2
#define WORDALIGNMENT_LEFT		3
#define WORDALIGNMENT_AVERAGE	4
///////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CInsample0App:
// See insample0.cpp for the implementation of this class
//

class CInsample0App : public CWinApp
{
public:
	CInsample0App();

	unsigned long	deviceIndex;
	int				ft232Index;
	BOOL			m_bIsDI16AReady, m_bIsFT232Ready;
	BOOL			m_bIsStop;

	void	InitParams();
	void	InitChangingParams( char* path );
	void	InitLoggingParams( char* path );
	void	InitSelectedADC(char *path);
	void	InitADCcfg( char* path, char* Entry1, char* Entry2, char* Entry3, int nTotal, int nNum, int nMID, int nCID );
	void	InitAutoTestParams(char *path);

	int		ReadInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator=',');
	int		SaveInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator=',');

	void	WriteToBoard( BYTE bOpcode, BOOL blOperand, WORD wData );
	void	CommToBoards( BYTE bOp1, BYTE bOp2 );
	void	CommToBoards2( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4 );
	void	CommToBoards3( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, BYTE bOp5, BYTE bOp6 );
	long	CommToBoardsReadADC( int nProcessID, int nADCnum, int nADCchn, BOOL bAnalyze );

	DWORD	BoardReadback( BYTE bOp1, BYTE bOp2, BYTE bOp3, BYTE bOp4, WORD& wData1, WORD& wData2, CString& strMsg );
	long	BoardRead0816( int nRsize, int nModID, BYTE bCktAddr, WORD wWdata, CString& strMsgCounts, CString& strMsgData, BOOL bAnalyze );
	long	BoardRead2432( int nRsize, int nModID, BYTE bCktAddr, DWORD dwWdata, CString& strMsgCounts, CString& strMsgData, BOOL bAnalyze );
	void	BoardWrite0816( int nWsize, int nModID, BYTE bCktAddr, WORD wWdata );
	void	BoardWrite24( int nWsize, int nModID, BYTE bCktAddr, DWORD dwWdata );
	long	ReadComprs( unsigned short* pnData, CString &strRes );
	void	UpdateListString( CString listStr);

	void	Delay( int msecond );
	CString	ChangedADCselectOB( int nNum );
	void	ChangedADCsChSelect( int nChan );
	long	ReadSelectedADC( BOOL bAnalyze = TRUE );
	CString	ReadSelectedADC_Logging( DWORD dwMilliseconds );
	CString	ReadSelectedADC_Logging2( DWORD dwMilliseconds, double &dValue );
	CString ReadSelectedADC_Logging_onetime( DWORD dwMilliseconds, double &dValue, BOOL bDelay = FALSE );	// 13.05.09 配合Logging试验设置的变量
	CString	ADCvconv_Logging( double &dValue, BOOL bLog = TRUE );

	void	OnlySendReadSelectedADCcmd( logParam lP);
	CString	ADCvconv_Logging( int nData1, int nData2, logParam lP, double &dValue, BOOL bLog = TRUE );

	double	GetPhysicsFromAdcs( int nIndex, double dVadc );
	int		m_nBoardReadDataBuf[256*2];
	int		m_nRBwdCnt;

	int		m_nADCnum, m_nADCchn;

	double*	m_pGraphDataBuff[GraphDataBuffGroups];

// 	Diag_Param	m_diagParam[Diag_Total];
// 	int			m_nElapseChanging;
// 	int			m_nElapseLogging;
// 	BOOL		m_bGaugeCompr;
// 	BOOL		m_bLog[6*16];
// 	BOOL		m_bLNSLogParam[9];
// 	int			m_nCheckComp[4];
// 	HANDLE		m_hLogFile;
// 	long		m_lTimes;

//	Log_Param	m_logParam;

	DL_Sum_Param	m_DL;
	AutoTest_Param	m_AT;
	Cali_Param		m_Cali;

	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	CFT232HDevice m_232Device;
	BOOL	Serial_Write( void *pData, DWORD dwBufferSize, DWORD &dwBytesWritten );
	int		Serial_Read( void *Buffer, DWORD &dwBytesReturned );
/*
	CUSB_Recv*	m_pRecv;
	CUSB_Send*	m_pSend;
	BOOL		m_bRecvReady, m_bSendReady;

	SharedParams_Cfg_ADCs*		m_pSPcfgADCs;
	SharedParams_Back_ADCs*		m_pSPbackADCs;
	SharedParams_Back_Others*	m_pSPbackOthers;
	SharedParams_Record_List*	m_pSPlist;
*/	/////////////////////////////////////////////

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsample0App)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CInsample0App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CInsample0App theApp;


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSAMPLE0_H__5D74169A_FA08_41B6_A584_26249D602878__INCLUDED_)
