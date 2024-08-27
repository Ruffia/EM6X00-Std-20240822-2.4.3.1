#pragma once

// CCameraSetting 对话框

#include "afxwin.h" 
#include "MvCamera.h"
#include "resource.h"
#include <string>
#include "Comm_NAV.h"

class CPanelNAV;
class CDlgCameraSetting : public CDialog
{
	//DECLARE_DYNAMIC(CDlgCameraSetting)
public:
	CDlgCameraSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCameraSetting();

	void	Localize(void);
	char	m_cPathLocalize[256];

	CMvCamera*		m_pcMyCamera;
	NAV_Settings	m_navSets;

	//NAV_Position	m_navPosStart, m_navPosEnd;
	//NAV_Position5	m_navPosOpenMove;
	//int				m_nHor, m_nVer, m_nLeftRotateCount, m_nRightRotateCount;

	//CString	m_strExpressionX;
	//CString	m_strExpressionY;
	//float		m_fDiffPos;

	//CString	m_strImageStoragePath;
	//int		m_nImageStorageTick;

	//float		m_fZoom;
	//int		m_nFrame;
	//int		m_nExposureTime;
	//int		m_nGain;

	//int		m_nTriggerMode;	
	//int		m_nTriggerSource;
	//int		m_nTriggerLine;
	//int		m_nTriggerActivite;

	//int		m_nCamera_H, m_nCamera_W;
	//int		m_nOffsetX, m_nOffsetY ;

	//开始采集和停止采集移动的时间，以毫秒为单位
	float	m_fMoveTimeStart;
	float	m_fMoveTimeEnd;

	CPanelNAV *dlg;
	// 对话框数据
	enum { IDD = IDD_CAMERA_SETTINGS };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	int		GetExposureTime();				// ch:设置曝光时间 | en:Set Exposure Time
	int		SetExposureTime(); 
	int		GetGain();						// ch:设置增益 | en:Set Gain
	int		SetGain();
	int		GetFrameRate();					// ch:设置帧率 | en:Set Frame Rate
	int		SetFrameRate();
	int		SetCameraSize();				//ch:设置相机分辨率
	int		GetCameraSize();				//ch:获取相机分辨率
	int		SetCameraROI();					//ch:设置相机ROI区域
	int		GetCameraROI();
	DECLARE_MESSAGE_MAP()
public:
	void	InitCameraSet();
	void	ConfigRead();
	void	ConfigSave( int nIndex );	

	afx_msg void OnTriggerContinuous();
	afx_msg void OnTriggerHard();	
	afx_msg void OnSavePath();

	afx_msg void OnCreateWorldCoordinateSystem();
	afx_msg void OnDistortionCorrection();
	afx_msg void OnOpenImage();
	afx_msg void OnReadParam();

	afx_msg void OnCalibration();
	afx_msg void OnBrowse();

	afx_msg void OnBnClickedSendStartSampler();
	afx_msg void OnBnClickedSendStopSampler();
	afx_msg void OnBnClickedSendSaveImage();
	afx_msg void OnEnChangeFrameRateEdit();
};
