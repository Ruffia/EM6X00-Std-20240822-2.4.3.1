#pragma once
#include "afxwin.h"
#include "Comm_Nav.h"
#include "MvCamera.h"

#include "resource.h"
#include "DlgCameraSetting.h"
#include "DlgProgress.h"


// CPanelNAV dialog
class CPanelNAV : public CDialog
{
	CBrush	m_brushPowerON, m_brushPowerOFF;
	// Construction
	//DECLARE_DYNAMIC(CPanelNAV)
public:
	CPanelNAV(CWnd* pParent = NULL);	// Standard constructor
	~CPanelNAV();

	HWND parent_wnd;
	enum { IDD = IDD_PANEL_NAV };

	__int64	_GetCurrentTimeStamp();
	void	EnableControls(BOOL bIsCameraReady);
	void	ShowCameraSeting();
	void	ShowErrorMsg(CString csMessage, int nErrorNum);
	void	ChangeShowRectSize(BOOL islateral);
	
	void	ReSize();
	//初始化相机进程
	void	CallCameraInit();
	BOOL	CameraPrepare();
	int		m_nCameraFlag;//1表示先点击了样品室放气,0表示没有点击样品室放气

	//相机采集的相关设置
	BOOL	m_bInitCamera;
	BOOL	m_bThreadState;
	BOOL	m_bGrabbing;   // ch:是否开始抓图 | en:Whether to start grabbing
	BOOL	m_bInitCameraRunning;

	//控制台移动的相关设置
//	float _px, _py, _pz;
	NAV_Position m_navPos;
	NAV_Position m_navPosLast;      //上一次的坐标
	NAV_Position5 m_navPosRecv;
	BOOL	m_bFirstGetStageMoved;  //是否已经发过一次让自动台运动的指令
	void	NotifyStageMove(float fX, float fY, float fZ);
	void	ClickStageMove();
	void	NotifyPos( int nIndex );
	void	UpdatePos( float fX, float fY );

	CDlgCameraSetting	m_dlgCameraSetting;
	//进度条和下拉框
	CDlgProgress*		m_pdlgProgress;

	CRect	m_rcImageInit;
	CPoint	m_ptOld;
	//是否显示中心坐标
	BOOL	m_bIsShowCenter;

	//用于调整放大缩小
	int		m_nMultiple;
	CRect	m_rcDesk;		

	BOOL	m_bShowselfImg;
	BOOL	m_bNavPowerOn;	//用于记录目前的电源开关状态
	BOOL	m_bV1Status;	//用于记录目前的V1阀开关状态
	BOOL	m_bStageIsMoving;

	void	ShowImageFromPath(std::string path);
	void	SaveImageToPath(std::string path, unsigned char* SaveBuffer);

	void	UpdateNAVSwitch( BOOL bHVOn, BOOL bAction = TRUE );
	static UINT	GrabThreadProcess(LPVOID lpram);
	void	StopGrabbingFunc( BOOL bClear );
	void	ClearImageRect();

	void	SetLanguageID( WORD wID );
	char	m_cPathLocalize[256];

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedButtonNavigate();
	afx_msg void OnBnClickedInitImage();
	afx_msg void OnBnClickedAdvanced();		
	afx_msg void OnBnClickedOpenPower();
	afx_msg void OnTestStage();
	DECLARE_MESSAGE_MAP()

public:
	/*ch:图像采集 | en:Image Acquisition*/
	afx_msg void OnBnClickedStartGrabbingButton();	// ch:开始采集 | en:Start Grabbing
	afx_msg void OnBnClickedStopGrabbingButton();	// ch:结束采集 | en:Stop Grabbing
	/*ch:图像保存 | en:Image Save*/  
	afx_msg void OnBnClickedSaveJpgButton();		// ch:保存jpg | en:Save jpg

private:
	/*ch:最开始时的窗口初始化 | en:Window initialization*/
	void	DisplayWindowInitial();  
	// ch:保存图片 | en:Save Image
	int		SaveImage(MV_SAVE_IAMGE_TYPE enSaveImageType, CString imagePath, unsigned char* img_data); 
	// ch:去除自定义的像素格式 | en:Remove custom pixel formats
	BOOL	RemoveCustomPixelFormats(enum MvGvspPixelType enPixelFormat);

	//用于标定坐标系
	afx_msg LRESULT OnSetOpenImage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetReadArgument(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetUndistor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetCreateWorld(WPARAM wParam, LPARAM lParam);

private:
	CMvCamera*				m_pcMyCamera;			// ch:CMyCamera封装了常用接口
	HWND                    m_hwndDisplay;			// ch:显示句柄 

	MV_CC_DEVICE_INFO_LIST  m_stDevList;         
	CRITICAL_SECTION        m_hSaveImageMux;
	unsigned char*          m_pSaveImageBuf;
	unsigned int            m_nSaveImageBufSize;
	MV_FRAME_OUT_INFO_EX    m_stImageInfo;
//	void*                   m_hGrabThread;			// ch:取流线程句柄 | en:Grab thread handle
	CWinThread*				m_pGrabThread;
	HANDLE					m_hExitEvent;

	//是否保存图片和显示区域的左上角坐标
	BOOL	m_bSaveJpg;
	int		m_nDisplayLeft;

	CComboBox		m_comboShowMag;
};
