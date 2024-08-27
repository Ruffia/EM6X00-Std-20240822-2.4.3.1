#pragma once

// DlgCCDCameraSetting 对话框
#include "CameraDS.h"
#include "afxcmn.h"

#define WM_SET_CAMERA_DS (WM_USER + 100)

struct CameraParameterRange {
	long Min;
	long Max;
	long Step;
	long Default;
	long Flags;
};

class CDlgCCDCameraSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgCCDCameraSetting)

public:
	CDlgCCDCameraSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCCDCameraSetting();

	///////////////////////////////////
	// 本地化相关
	void	Localize();		// 本地化
	char	m_cPathLocalize[256];
	///////////////////////////////////

// 对话框数据
	enum { IDD = IDD_DIALOG_Settings };
	CScrollBar m_scrollBrightness;
	CScrollBar m_scrollContrast;
	CScrollBar m_scrollHue;
	CScrollBar m_scrollGain;
	int m_nBrightness;
	int m_nContrast;
	int m_nHue;
	int m_nGain;

	IAMVideoProcAmp*		m_pVideoProcAmp;
	CameraParameterRange	m_RangeBrt;
	CameraParameterRange	m_RangeCst;
	CameraParameterRange	m_RangeGain;
	CameraParameterRange	m_RangeHue;

	HRESULT SetCameraParameter(IAMVideoProcAmp *pVideoProcAmp, long Property, long Value);
	HRESULT GetCameraParameter(IAMVideoProcAmp *pVideoProcAmp, long Property, long &Value);
	void	SetCameraDS(CCameraDS* ds);

	void	CheckValid( int& nValue, CameraParameterRange range );
	void	CCD_Brt_Change( int nPos );
	void	CCD_Cst_Change( int nPos );
	void	CCD_Gain_Change( int nPos );
	void	CCD_Hue_Change( int nPos );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditBrightness();
	afx_msg void OnChangeEditContrast();
	afx_msg void OnChangeEditHue();
	afx_msg void OnChangeEditGain();
	afx_msg void OnButtonCCDReset();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnSetCameraDS(WPARAM wParam, LPARAM lParam);
};

