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
	//��ʼ���������
	void	CallCameraInit();
	BOOL	CameraPrepare();
	int		m_nCameraFlag;//1��ʾ�ȵ������Ʒ�ҷ���,0��ʾû�е����Ʒ�ҷ���

	//����ɼ����������
	BOOL	m_bInitCamera;
	BOOL	m_bThreadState;
	BOOL	m_bGrabbing;   // ch:�Ƿ�ʼץͼ | en:Whether to start grabbing
	BOOL	m_bInitCameraRunning;

	//����̨�ƶ����������
//	float _px, _py, _pz;
	NAV_Position m_navPos;
	NAV_Position m_navPosLast;      //��һ�ε�����
	NAV_Position5 m_navPosRecv;
	BOOL	m_bFirstGetStageMoved;  //�Ƿ��Ѿ�����һ�����Զ�̨�˶���ָ��
	void	NotifyStageMove(float fX, float fY, float fZ);
	void	ClickStageMove();
	void	NotifyPos( int nIndex );
	void	UpdatePos( float fX, float fY );

	CDlgCameraSetting	m_dlgCameraSetting;
	//��������������
	CDlgProgress*		m_pdlgProgress;

	CRect	m_rcImageInit;
	CPoint	m_ptOld;
	//�Ƿ���ʾ��������
	BOOL	m_bIsShowCenter;

	//���ڵ����Ŵ���С
	int		m_nMultiple;
	CRect	m_rcDesk;		

	BOOL	m_bShowselfImg;
	BOOL	m_bNavPowerOn;	//���ڼ�¼Ŀǰ�ĵ�Դ����״̬
	BOOL	m_bV1Status;	//���ڼ�¼Ŀǰ��V1������״̬
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
	/*ch:ͼ��ɼ� | en:Image Acquisition*/
	afx_msg void OnBnClickedStartGrabbingButton();	// ch:��ʼ�ɼ� | en:Start Grabbing
	afx_msg void OnBnClickedStopGrabbingButton();	// ch:�����ɼ� | en:Stop Grabbing
	/*ch:ͼ�񱣴� | en:Image Save*/  
	afx_msg void OnBnClickedSaveJpgButton();		// ch:����jpg | en:Save jpg

private:
	/*ch:�ʼʱ�Ĵ��ڳ�ʼ�� | en:Window initialization*/
	void	DisplayWindowInitial();  
	// ch:����ͼƬ | en:Save Image
	int		SaveImage(MV_SAVE_IAMGE_TYPE enSaveImageType, CString imagePath, unsigned char* img_data); 
	// ch:ȥ���Զ�������ظ�ʽ | en:Remove custom pixel formats
	BOOL	RemoveCustomPixelFormats(enum MvGvspPixelType enPixelFormat);

	//���ڱ궨����ϵ
	afx_msg LRESULT OnSetOpenImage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetReadArgument(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetUndistor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetCreateWorld(WPARAM wParam, LPARAM lParam);

private:
	CMvCamera*				m_pcMyCamera;			// ch:CMyCamera��װ�˳��ýӿ�
	HWND                    m_hwndDisplay;			// ch:��ʾ��� 

	MV_CC_DEVICE_INFO_LIST  m_stDevList;         
	CRITICAL_SECTION        m_hSaveImageMux;
	unsigned char*          m_pSaveImageBuf;
	unsigned int            m_nSaveImageBufSize;
	MV_FRAME_OUT_INFO_EX    m_stImageInfo;
//	void*                   m_hGrabThread;			// ch:ȡ���߳̾�� | en:Grab thread handle
	CWinThread*				m_pGrabThread;
	HANDLE					m_hExitEvent;

	//�Ƿ񱣴�ͼƬ����ʾ��������Ͻ�����
	BOOL	m_bSaveJpg;
	int		m_nDisplayLeft;

	CComboBox		m_comboShowMag;
};
