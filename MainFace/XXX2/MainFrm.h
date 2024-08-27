// MainFrm.h : interface of the CMainFrame class
//
// Last Modify : 2011.07.15

/////////////////////////////////////////////////////////////////////////////
// 11.07.15 增加保护，当高压未退至0时不允许关闭软件
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__36AC4593_0773_47D2_AC9A_A806E752E618__INCLUDED_)
#define AFX_MAINFRM_H__36AC4593_0773_47D2_AC9A_A806E752E618__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FlatSplitterWnd.h"

#include "DlgImpPanelLog.h"
#include "DlgImpPanelThumb.h"
#include "DlgImpPanelAll2.h"
#include "DlgImpPanelDebug.h"
#include "DlgImpPanelDebugR.h"
#include "PanelMainDebug.h"

#include "DlgDiagLogGraph.h"
#include "DlgDiagLogDiffGraph.h"
#include "DlgAutoTestMain.h"
#include "DlgCaliMain.h"
#include "DlgDebugMain.h"

#include "DlgHisto.h"
#include "PanelStage.h"
#include "TestParameterDlg.h"

#include <afxpriv.h>
#include "BeamButton.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	CToolBar		m_wndScanCtrlBar;

	CComboBox		m_wndSpeed;
	CEdit			m_wndPixRateEdit;
	CSpinButtonCtrl	m_wndPixRateSpin;
	int				m_nusbPixRate;
	void			UpdatePixRate( int& nPixRate );

	CSpinButtonCtrl m_wndVideoGraphSpin;
	CEdit			m_wndVideoGraphOffsetEdit;
	CSpinButtonCtrl	m_wndVideoGraphOffsetSpin;
	
	DebugOSC	m_debugOSC;

	CDlgDiagLogGraph		m_dlgGraph;
	CDlgDiagLogDiffGraph	m_dlgDiffGraph;
	CDlgAutoTestMain		m_dlgAutotest;
//	CDlgCaliMain			m_dlgCali;
	CDlgCaliCoadjust		m_dlgCali;
	CDlgImpPanelDebug		m_dlgDebugVIP;
	CDlgImpPanelDebugR		m_dlgDebugR;
	CDlgHisto				m_HistoBar;
	CTestParameterDlg       m_dlgTestParameter;
	// 09.12.10 增加调试状态功能
//	CSemCtrlDlgDebug*	m_pdlgDebug;
	CDlgDebugMain	m_dlgDebug;
	BOOL		m_bIsDebugging;
	HACCEL		m_hAccelDebug;
	void		SetupDebugParam( BOOL bDebug, BOOL bDebugAccel, BOOL bDebugVac, BOOL bDebugStage, int nBrightness, int nContrast );
	void		DebugClose();
	// 09.12.10 增加调试状态功能

	// 10.04 新界面面板实验
	CDlgImpPanelLog				m_wndImpPanelLog;
	CDlgImpPanelThumb			m_wndImpPanelThumb;
	CDlgImpPanelAll2			m_wndImpPanelAll2;
	CPanelMainDebug				m_MainPanelDebug;
	CPanelStage					m_dlgStage;

	/////////////////////////////////////////////////////////////////
	// 2024.05 束流检测
	CToolBar		m_wndBeamCtrlBar;
	CComboBox		m_comboBeamTar, m_comboBeamPort, m_comboBeamAper;
	CStatic			m_staticBeamTar, m_staticBeamCur, m_staticBeamPort, m_staticBeamAper, m_staticBeamUnit;
	CEdit			m_editCurrentBeam;
	CBeamButton     m_btnBeamDetection;		//CButton		m_wndBeamDetection;
	CButton			m_btnBeamSetup, m_btnBeamSaveXYpos;

	int			m_nPortD22;
	BOOL		m_bIsD22Ready;
	float		m_fBeamFaradayCupPos[2], m_fBeamStagePosCur[5];   //法拉第杯初始位置XY，样品台当前位置
	int			m_nBeamCount;
	int			m_nBeamStatesShow,m_nBeamStateAper;
	double		m_dBeamMagCur,m_dBeamMagTar;	// 用于进行束流检测前保存当前放大倍数，进行束流检测时变为固定放大倍数
	float		m_fBeamCur;
	int 		m_nBeamAper;	//m_nAccVolt,m_nBeamAperNum[6];
	BOOL		m_bBeamFlag;
	int			m_nBeamLandNumNew,m_nBeamLandNumOld;
	int			m_nBeamLand,m_nBeamSelect[6];
	int			m_nBeamFineAper,m_dBeamCondenser;

	CFont		m_BeamFont;

	int			_CreatePanelBeam();
	int			_CreatePanelBeam_short();
	void		D22_Init();
	void		D22_InitParam();
	void		D22_Relink();
	void		D22_SetStatus(BOOL bEnable);
	void		Beam_SaveToINI( int nIndex );
	void		Beam_AutoCali();
//	void		Beam_CovertValue();
	void		Beam_StepsParams(int nID, int nPerID);
	void		Beam_AutoProcess();

	void		Beam_UpdateStagePos( int nIndex );
	void		Beam_UpdateCtrls( int nIndex );
	void		Beam_Timer(UINT nIDEvent);
	/////////////////////////////////////////////////////////////////

	void	LoggingFilamentLifetime();
	void	LoggingFilamentLifetime_Start();
	void	LoggingFilamentLifetime_End();

	void	LoggingAppLifetime();
	void	LoggingAppLifetime_Start();
	void	LoggingAppLifetime_End();

// Operations
public:
	void	Message(LPCTSTR lpszMessage);
	void	Message2(LPCTSTR lpszMessage);
	void	Localize(void);
	void	AdjustPanelsPos();
	void	SetEnableClose( BOOL bEnable );		// 11.07.15

	BOOL	ActivateMDIChild( CString strCaption = "SemScan" );
	BOOL	DestroyMDIChild( CString strCaption );
	void	MinimizeImageMDIChild();

	void	SetTimers( int nID, BOOL bSet );
	void    _SaveScenario();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;

	UINT		m_nSemMessageTimer;
	UINT		m_nSemAutosaveTimer;

	UINT		m_nReadStatusTimer;

	UINT		m_nMCB_SemMessageTimer;
	UINT		m_nCPS_SemMessageTimer;

	UINT		m_nFilaLifeTimer;
	UINT		m_nAppLifeTimer;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnOptionsDebug();
	afx_msg void OnUpdateOptionsDebug(CCmdUI* pCmdUI);
	afx_msg void OnOptionsTestParameter();
	afx_msg void OnUpdateOptionsTestParameter(CCmdUI* pCmdUI);
	afx_msg void OnOptionsManageScenario();
	afx_msg void OnUpdateOptionsManageScenario(CCmdUI* pCmdUI);
	afx_msg void OnOptionsLog();
	afx_msg void OnUpdateOptionsLog(CCmdUI* pCmdUI);
	afx_msg void OnLog_Show();
	afx_msg void OnUpdateLog_Show(CCmdUI* pCmdUI);
	afx_msg void OnLog_Settings();
	afx_msg void OnUpdateLog_Settings(CCmdUI* pCmdUI);
	afx_msg void OnLog_AdvancedSettings();
	afx_msg void OnUpdateLog_AdvancedSettings(CCmdUI* pCmdUI);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnViewPanel1();
	afx_msg void OnUpdateViewPanel1(CCmdUI* pCmdUI);
	afx_msg void OnViewPanel2();
	afx_msg void OnUpdateViewPanel2(CCmdUI* pCmdUI);
	afx_msg void OnViewPanel3();
	afx_msg void OnUpdateViewPanel3(CCmdUI* pCmdUI);
	afx_msg void OnViewPanel4();
	afx_msg void OnUpdateViewPanel4(CCmdUI* pCmdUI);
	afx_msg void OnViewPanel5();
	afx_msg void OnUpdateViewPanel5(CCmdUI* pCmdUI);
	afx_msg void OnViewPanel6();
	afx_msg void OnUpdateViewPanel6(CCmdUI* pCmdUI);
	afx_msg void OnViewPanel7();
	afx_msg void OnUpdateViewPanel7(CCmdUI* pCmdUI);
	afx_msg void OnViewPanel8();
	afx_msg void OnUpdateViewPanel8(CCmdUI* pCmdUI);
	afx_msg void OnViewPanel9();
	afx_msg void OnUpdateViewPanel9(CCmdUI* pCmdUI);
	afx_msg void OnPANELMAINBtnLastStep();
	afx_msg void OnPANELMAINBtnNextStep();
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnViewBar0();
	afx_msg void OnUpdateViewBar0(CCmdUI* pCmdUI);
	afx_msg void OnViewBar1();
	afx_msg void OnUpdateViewBar1(CCmdUI* pCmdUI);
	afx_msg void OnToolbarDropDown(NMTOOLBAR* pnmtb, LRESULT *plr);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG
	afx_msg void OnSelchangeComboSpeed();
	afx_msg void OnDeltaposSPINVideoGraph(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSPINVideoGraphOffset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSPINPixRate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);
	afx_msg LRESULT OnStageMC600msgChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayDebugMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAutoStageXY_FI_Init(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSyncFineMag(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCoUpdateScan(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateBeemAlignmentX(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateBeemAlignmentY(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowDFocusCtrl(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSyncContrastCtrlText(WPARAM wParam, LPARAM lParam);
	//自动亮度对比度(手控盒)
	afx_msg LRESULT OnControlPanelAutoBrightContrast(WPARAM wParam, LPARAM lParam);
	//自动聚焦(手控盒)
	afx_msg LRESULT OnControlPanelAutoFocus(WPARAM wParam, LPARAM lParam);
	//切换扫描速度(手控盒)
	afx_msg LRESULT OnControlPanelChangeScanSpeed(WPARAM wParam, LPARAM lParam);
	//F1 快捷键，照相预览(手控盒)
	afx_msg LRESULT OnControlPanelPhotoPreview(WPARAM wParam, LPARAM lParam);
	//F2 快捷键，同步扫描(手控盒)
	afx_msg LRESULT OnControlPanelSyncScan(WPARAM wParam, LPARAM lParam);
	//F3 快捷键，冻结图像(手控盒)
	afx_msg LRESULT OnControlPanelStopScan(WPARAM wParam, LPARAM lParam);
	//S1 快捷键，常规扫描/选区扫描切换(手控盒)
	afx_msg LRESULT OnControlPanelChangeScan(WPARAM wParam, LPARAM lParam);
	//检查CCG是否应该报警
	afx_msg LRESULT OnCheckVACAlarm(WPARAM wParam, LPARAM lParam);
	// 束流检测
	afx_msg void OnDropdownComboBeamTarget();
	afx_msg void OnBtnBeamDetection();
	afx_msg void OnBtnBeamSetup();
	afx_msg void OnSelchangeBeamD22Target();
	afx_msg void OnSelchangeBeamD22Port();
	afx_msg void OnSelchangeBeamD22Aperture();
	afx_msg void OnBtnBeamSaveXYpos();
	afx_msg LRESULT OnUpdateD22Value(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

protected:
	int   _CreatePanelLog();
	int   _CreatePanelThumb();
	void  _LoadSettingsFromRegistry();
	int   _CreatePanelAll2();
	int   _CreateDlgAutoTest();
	int   _CreateDlgStage();
	void  _CreateHistogramBar();
	int   _CreateDlgDebug();
	int   _CreateDlgDiffGraph();
	int   _CreateDlgGraph();
	int   _CreateDlgDebugR();
	int   _CreateDlgDebugVIP();
	int   _CreateDlgCalibration();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__36AC4593_0773_47D2_AC9A_A806E752E618__INCLUDED_)
