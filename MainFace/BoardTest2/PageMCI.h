#if !defined(AFX_PAGEMCI_H__883DBA28_B524_4CB8_AEED_E7C8B16453AB__INCLUDED_)
#define AFX_PAGEMCI_H__883DBA28_B524_4CB8_AEED_E7C8B16453AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageMCI.h : header file
//

#include "MacButtons.h"

#include "DlgDisplay.h"

// 扫描速度
typedef enum tagUSB_ScanSpeed
{
	USB_SS_125ns			= 0,	// 点停留时间   1us
	USB_SS_250ns			= 1,	// 点停留时间   1us
	USB_SS_375ns			= 2,	// 点停留时间   1us
	USB_SS_500ns			= 3,	// 点停留时间   1us
	USB_SS_625ns			= 4,	// 点停留时间   1us
	USB_SS_750ns			= 5,	// 点停留时间   1us
	USB_SS_875ns			= 6,	// 点停留时间   1us
	USB_SS_1us				= 7,	// 点停留时间   1us
	USB_SS_2us				= 8,	// 点停留时间   2us
	USB_SS_5us				= 9,	// 点停留时间   5us
	USB_SS_10us				= 10,	// 点停留时间  10us
	USB_SS_16us				= 11,	// 点停留时间  16us
	USB_SS_20us				= 12,	// 点停留时间  20us
	USB_SS_40us				= 13,	// 点停留时间  40us
	USB_SS_80us				= 14,	// 点停留时间  80us
	USB_SS_120us			= 15,	// 点停留时间 120us
	USB_SS_160us			= 16,	// 点停留时间 160us
}USB_ScanSpeed;

typedef struct tagUSB_InitRampParam
{
	WORD	wOffset;
	WORD	wValley;
	WORD	wRise;
	WORD	wDRise;
	WORD	wPeak;
	WORD	wFall;
	WORD	wDFall;
//	WORD	wDirection;
}USB_InitScanRampParam;

#define	ValleyX		4
#define	ValleyY		4
#define	PeakX		4
#define	PeakY		2
#define	FallX		4
#define	FallY		2
#define	DFallX		15000
#define	DFallY		31000

typedef struct tagUSB_InitData
{
	USB_InitScanRampParam	ScanRampParamX;
	USB_InitScanRampParam	ScanRampParamY;
	USB_ScanSpeed			Speed;
}USB_InitData;

typedef union tagUSB_InitParam
{
	WORD			wData[14];	// 参数数据表
	USB_InitData	Data;		// 参数数据结构
}USB_InitParam;

typedef struct tagUSB_TransferData
{
	long	lLoops;
	long	lSize;
	LPVOID	pBuff;
}USB_TransferData;

/////////////////////////////////////////////////////////////////////////////
// Pixel rate in units of 1/8 MHz, namely 125ns.
// Pixel rate = N means N * 125nS for pixel time
// The range of N is from 1 to 2K, namely 1,2,3,...0x7FF
/////////////////////////////////////////////////////////////////////////////
// 附：常用值，像素率即点逗留时间
// tp		0.125us 0.250us 0.375us 0.5us 1us  2us  5us 10us 16us 20us 40us 80us 100us 120us 160us 200us 250us
// N		1,      2,      3,      4,    8,   16,  40, 80,  128, 160, 320, 640, 800,  960,  1280, 1600, 2000
//---------------------------------------------------------------------------
// tp		40us 80us 120us 160us	50us 90us	照相常用值
// N		320, 640, 960,  1280,   400, 720
/////////////////////////////////////////////////////////////////////////////

//                                  0.125us 0.25us 0.375us 0.5us 0.625us 0.75us 0.875us 1us   2us   5us   10us   16us   20us   40us   80us   120us   160us
const WORD	USB_PIXEL_RATES[]	= {	1,      2,     3,      4,    5,      6,     7,      8,    16,   40,   80,    128,   160,   320,   640,   960,    1280, };
const DWORD	USB_SCANSPEED_TP[]	= {	125,    250,   375,    500,  625,    750,   875,    1000, 2000, 5000, 10000, 16000, 20000, 40000, 80000, 120000, 160000, };	// unit: ns

/////////////////////////////////////////////////////////////////////////////
// CPageMCI dialog

class CPageMCI : public CDialog
{
// Construction
public:
	CPageMCI(CWnd* pParent = NULL);   // standard constructor
	~CPageMCI();

	void	Init( BOOL bReady = TRUE );
	/////////////////////////////////////////////////////////////////
	void	ReadBackdata( int nBits = 8 );
	void	PwrVconV( BYTE bData1, BYTE bData2, CDialog* pParent );
	double	m_dPwrReadback[8];
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	void	DrawGrayScale(int nXDim, int nYDim);

	void	ScanStart();
	void	ScanStop();

	CString	PixReceive( long lDataWords );
	int		PixConvert( const CString str, DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ );
	int		PixConvert( unsigned short* pnFrameData, long lBytesTransferred, DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ );
	int		PixConvert2( unsigned short* pnFrameData, long lBytesTransferred, DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ );
	long	PixConvertZZZ( unsigned short* pnFrameData, long lBytesTransferred, DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ );
	long	PixConvertZZZ2( unsigned short* pnFrameData, long lBytesTransferred, DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ );
	void	PixDisplayByDemoImage( int nCounts, DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ );
	void	PixDisplayByReceiveSerial( int nCounts, DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ );
	void	PixDisplayByReceiveDelay( int nCounts, DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ );
	void	PixDisplayByReceive2( int nCounts, DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ );
	void	PixDisplayByReceiveZZZ( int nCounts, DWORD* pdwPixX, DWORD* pdwPixY, DWORD* pdwPixZ );

	CSize	GetSizes();
	void	CheckIndexs( int nLineCounts, int nFrameCounts, int& nX, int& nY );

	void	Det03SB_Change( int nPos );
	void	Det45SB_Change( int nPos );
	void	DetGainSB_Change( int nPos );
	void	BWselSB_Change( int nPos );

	BOOL	ReadBoardID( int nMID, unsigned short* pBoardID );

	USB_ScanSpeed	m_Speed;
	BOOL	m_bVideoMode;		// 0: XYZ mode;	1: Raster mode
	BOOL	m_bDetMode;			// 0: Single;	1: Duel
	BOOL	m_bDispMode;
	BOOL	m_bRotate;

	unsigned short*		m_pBuffUSB;
	long	m_lLoopCounts;
	long	m_lReadSize;
	DWORD	m_dwX, m_dwY, m_dwZa;
	int		m_nIndexX;		// 有效的数据的X序号
	int		m_nIndexY;		// 有效的数据的Y序号
	int		m_nX, m_nY;
	int		m_nDisX, m_nDisY;
	BOOL	m_bPeakX, m_bPeakY;
	BOOL	InitData();
	void	GetPenParam( int& nWidth, int& nLength );

	CDlgDisplay	m_dlgDisplay;
	void	Message(LPCTSTR lpszMessage);

	void	InitDiagnose( BOOL bEnd );
	void	SetTimers( UINT nIDEvent, BOOL bSet );
	int		m_nPosDet03[4], m_nPosDet45[4];
	UINT	m_nDet03Chn2, m_nDet45Chn2;

	///////////////////////////////////////////////////////
	// FT232 Test
	BYTE	m_byteFT232Send[64];
	int		m_nFT232SendCount;
	unsigned char m_rxBuf[1024];
	void	FT232Test_SendData();
	void	FT232Test_RecvData();
	///////////////////////////////////////////////////////

// Dialog Data
	//{{AFX_DATA(CPageMCI)
	enum { IDD = IDD_PAGE_MCI };
	CMacButton	m_btnModeSpot;
	CMacButton	m_btnModeLineVertical;
	CMacButton	m_btnModeLineHorizontal;
	CMacButton	m_btnModeNormal;
	CMacCheckBox	m_btnVideo;
	CMacCheckBox	m_btnMainsLock;
	CMacRadioButton	m_btnDispSerial;
	CMacRadioButton	m_btnDispDelay;
	CMacRadioButton	m_radioResolution1;
	CMacRadioButton	m_radioResolution2;
	CMacRadioButton	m_radioResolution3;
	CMacRadioButton	m_radioResolution4;
	CMacRadioButton	m_radioResolution5;
	CMacRadioButton	m_radioResolution6;
	CMacRadioButton	m_radioResolution7;
	CMacRadioButton	m_radioPixClkEq1;
	CMacRadioButton	m_radioPixClkEq2;
	CMacRadioButton	m_radioPixClkEq3;
	CMacRadioButton	m_radioPixClkEq4;
	CMacRadioButton	m_radioPixClkEq5;
	CMacRadioButton	m_radioPixClkEq6;
	CMacRadioButton	m_radioPixClkEq7;
	CMacRadioButton	m_radioPixClkEq8;
	CMacRadioButton	m_radioPixClkEq9;
	CMacRadioButton	m_radioPixClkEq10;
	CMacButton	m_btnRotate;
	CScrollBar	m_scrollBWsel;
	CScrollBar	m_scrollDetGain;
	CScrollBar	m_scrollDet45;
	CScrollBar	m_scrollDet03;
	CMacRadioButton	m_btnDetSingle;
	CMacRadioButton	m_btnDetDual;
	CMacRadioButton	m_btnVideoRaster;
	CMacRadioButton	m_btnVideoXYZ;
	CMacButton	m_btn1;
	CMacButton	m_btnReceive;
	CMacButton	m_btnDemoImage;
	CMacButton	m_btnDefaultRamp;
	CMacButton	m_btnUpdateY;
	CMacButton	m_btnUpdateX;
	CMacButton	m_btnScanStop;
	CMacButton	m_btnScanStart;
	CString	m_strPixelRateHB;
	CString	m_strPixelRateLB;
	CString	m_strSegNum;
	CString	m_strSegHB;
	CString	m_strSegLB;
	CString	m_strLineHB;
	CString	m_strLineLB;
	int		m_nReso;
	int		m_nPixClk;
	UINT	m_nDetGainNo2;
	UINT	m_nDetPair2;
	int		m_nDispType;
	int		m_nBoardID;
	UINT	m_nPwrChn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageMCI)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageMCI)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnReadPortA();
	afx_msg void OnWritePortA();
	afx_msg void OnReadPortB();
	afx_msg void OnWritePortB();
	afx_msg void OnReadPortC();
	afx_msg void OnWritePortC();
	afx_msg void OnReadPortD();
	afx_msg void OnWritePortD();
	afx_msg void OnReadAll();
	afx_msg void OnWriteAll();
	afx_msg void OnSetPixelRate();
	afx_msg void OnSetPixClkEq250KHz();
	afx_msg void OnSetPixClkEq1MHz();
	afx_msg void OnSetXrampSeg();
	afx_msg void OnScanStart();
	afx_msg void OnScanStop();
	afx_msg void OnReceive();
	afx_msg void OnInHexCheck();
	afx_msg void OnUpdateX();
	afx_msg void OnUpdateY();
	afx_msg void OnUpdateDefaultRamps();
	afx_msg void OnSetRampSegment();
	afx_msg void OnDet03Load();
	afx_msg void OnDet45Load();
	afx_msg void OnBoardComm();
	afx_msg void OnStartImage();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDemoImage();
	afx_msg void OnScanModeNormal();
	afx_msg void OnScanModeLineHorizontal();
	afx_msg void OnScanModeLineVertical();
	afx_msg void OnScanModeSpot();
	afx_msg void OnSaveListResult();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDeltaposDet03(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposDet45(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposDetGain(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposDetPair(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnScanRotation();
	afx_msg void OnScanRotationUpdate();
	afx_msg void OnPaint();
	afx_msg void OnReadVersion();
	afx_msg void OnScanStartWithParams();
	afx_msg void OnINT_ClearDB();
	afx_msg void OnINT_SetIMR();
	afx_msg void OnINT_INTtest();
	afx_msg void OnCHECKMainsLock();
	afx_msg void OnFakeMainslock();
	afx_msg void OnCHECKVideo();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnFreeze();
	afx_msg void OnResetCommunicate();
	afx_msg void OnFreezeX();
	afx_msg void OnFreezeY();
	afx_msg void OnReadINTSourceRegister();
	afx_msg void OnWriteINTEnableRegister();
	afx_msg void OnDetPairSelect();
	afx_msg void OnReadMCIPower();
	afx_msg void OnReadMCIPowerAll();
	afx_msg void OnBUTTONFT232TestSend();
	afx_msg void OnBUTTONFT232TestStop();
	afx_msg void OnBUTTONFT232TestRecv();
	afx_msg void OnCHECKFT232TestLoop();
	afx_msg void OnBUTTONSendInOneTime();
	afx_msg void OnDeltaposSPINPwrChn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReadBoardIDs();
	//}}AFX_MSG
	afx_msg void OnResolutionChanged( UINT nID );
	afx_msg void OnPixClkChanged( UINT nID );
	afx_msg void OnDisplayModeChanged( UINT nID );
	afx_msg void OnVideoModeChanged( UINT nID );
	afx_msg void OnDetectorModeChanged( UINT nID );
	BOOL memberFxn( UINT id, NMHDR * pTTTStruct, LRESULT * pResult );		//在线提示
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEMCI_H__883DBA28_B524_4CB8_AEED_E7C8B16453AB__INCLUDED_)
