// CommTestSampleDlg.h : header file
//

#if !defined(AFX_COMMTESTSAMPLEDLG_H__DC586E60_8C49_48CE_87BC_EFD7BB0CD546__INCLUDED_)
#define AFX_COMMTESTSAMPLEDLG_H__DC586E60_8C49_48CE_87BC_EFD7BB0CD546__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KYKYSEMComm.h"
#include "VirtualDisplay.h"

/////////////////////////////////////////////////////////////////////////////
// CCommTestSampleDlg dialog

typedef struct tag_StageConfig{
	int		nStatus;	// 状态值
	int		nType;		// 是几轴台
	BOOL	bReady;		// 状态是否准备好
} StageConfig;

class CCommTestSampleDlg : public CDialog
{
// Construction
public:
	CCommTestSampleDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CCommTestSampleDlg();

	void	Message(LPCTSTR lpszMessage);

	void	Disconnect();
	void	SetConnectStatus(BOOL bEnable);
	void	SetSendStatus(BOOL bEnable);
	void	EnablePowerSupply();

	void	InitReadParams();
	void	InitReadParams_Sys();
	void	InitReadParams_SEM();
	void	InitReadParams_VacGun();
	void	InitReadParams_Stage();

	void	UpdateStageStatus();
	void	UpdateStageEnables();
	void	StageMove( int nID );

	CKYKYSEMComm	m_Client;
	BOOL	m_bConnected;
	int		m_nTypeSEM, m_nTypeHV, m_nStatusVac, m_nStatusGun;
	int		m_nTimer;
	StageConfig m_StageCfg;
	double	m_dStageRange[2][5];

	CPoint	m_pointScanSpots[100];
	int		m_nScanSpotsIndex;

	/////////////////////////////////////////////////////////////////
	void	ChangeResolution();
	void	ResetImageWindow();
	void	InterFaceClear();
	void	RecvImgDataShow();	
	static	DWORD WINAPI ThreadImageProcess(LPVOID lParam);

	CVirtualDisplay	m_ImageCard;
	BOOL	m_bImgConnected;
	CSize	m_imageReso, m_preImageReso;
	volatile BOOL	m_bEndImgProcess;

// Dialog Data
	//{{AFX_DATA(CCommTestSampleDlg)
	enum { IDD = IDD_COMMTESTSAMPLE_DIALOG };
	CStatic	m_Image;
	CIPAddressCtrl	m_IPAddr;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommTestSampleDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCommTestSampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnBUTTONConnect();
	afx_msg void OnBUTTONDisconnect();
	afx_msg void OnRADIOQuerySEMType();
	afx_msg void OnRADIOQueryHVType();
	afx_msg void OnRADIOQueryMag();
	afx_msg void OnRADIOQueryMagRange();
	afx_msg void OnRADIOQueryRotAngleInit();
	afx_msg void OnRADIOQueryRotAngle();
	afx_msg void OnRADIOQueryWD();
	afx_msg void OnRADIOQueryHV();
	afx_msg void OnRADIOQueryFila();
	afx_msg void OnRADIOQueryBeamCurrent();
	afx_msg void OnRADIOStageStatus();
	afx_msg void OnRADIOStageGrating();
	afx_msg void OnRADIOStagePosition();
	afx_msg void OnRADIOStageRange();
	afx_msg void OnRADIOQueryStigmator();
	afx_msg void OnBUTTONStagePositionSet();
	afx_msg void OnBUTTONSetMag();
	afx_msg void OnBUTTONSetScnRotAngle();
	afx_msg void OnBUTTONSetStig();
	afx_msg void OnBUTTONSetHV();
	afx_msg void OnBUTTONSetFila();
	afx_msg void OnBUTTONSetBeamCurrent();
	afx_msg void OnBUTTONStageStop();
	afx_msg void OnBUTTONAutoBC();
	afx_msg void OnRADIOQueryBrt();
	afx_msg void OnRADIOQueryCst();
	afx_msg void OnBUTTONSetBrt();
	afx_msg void OnBUTTONSetCst();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRADIOQueryGunStatus();
	afx_msg void OnRADIOQueryVacStatus();
	afx_msg void OnRADIOScanCtrlSpeed1();
	afx_msg void OnRADIOScanCtrlSpeed2();
	afx_msg void OnRADIOScanCtrlSpeed3();
	afx_msg void OnRADIOScanCtrlSpeed4();
	afx_msg void OnRADIOScanCtrlReso1();
	afx_msg void OnRADIOScanCtrlReso2();
	afx_msg void OnRADIOScanCtrlType1();
	afx_msg void OnRADIOScanCtrlType2();
	afx_msg void OnRADIOScanCtrlType3();
	afx_msg void OnRADIOScanCtrlType4();
	afx_msg void OnRADIOScanCtrlType5();
	afx_msg void OnRADIOScanCtrlType6();
	afx_msg void OnRADIOScanCtrlType7();
	afx_msg void OnBUTTONSetWD();
	afx_msg void OnBUTTONStageGoHome();
	afx_msg void OnRADIOQueryEMI();
	afx_msg void OnRADIOQueryAll();
	afx_msg void OnCHECKPolling();
	afx_msg void OnLogClear();
	afx_msg void OnBUTTONClear();
	afx_msg void OnCHECKScanSpots();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBUTTONShutdownBeam();
	afx_msg void OnClose();
	afx_msg void OnRADIOQuerySpeed();
	afx_msg void OnRADIOQueryResolution();
	afx_msg void OnBUTTONImageClear();
	afx_msg void OnRADIOQueryFrameData();
	afx_msg void OnRADIOQueryImage();
	afx_msg void OnBUTTONSaveParams();
	afx_msg void OnBUTTONLoadParams();
	afx_msg void OnRADIOQueryFOV();
	//}}AFX_MSG
	afx_msg LRESULT OnGetConnectStatus(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMTESTSAMPLEDLG_H__DC586E60_8C49_48CE_87BC_EFD7BB0CD546__INCLUDED_)
