#pragma once
#include "resource.h"
#include "afxdialogex.h"
#include "ChartContainer4NegativeExponent.h"
#include "afxwin.h"

// CDlgSEMLog_ShowRealTimeTrend 对话框
class CDlgSEMLog_ShowRealTimeTrend : public CDialog
{
	DECLARE_DYNAMIC(CDlgSEMLog_ShowRealTimeTrend)

public:
	CDlgSEMLog_ShowRealTimeTrend(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSEMLog_ShowRealTimeTrend();
	void	Localize(void);

// 对话框数据
	enum { IDD = IDD_DLG_SEMLog_RealTimeTrend };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedPrevious();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedBtnAutorefresh();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);

protected:

	//设计控件布局
	void _DesignLayout();

	//垂直移动控件
	//nVeticalPix,垂直移动的像素单位
	//nHorizontalPix,水平移动的像素单位
	void _MoveControl(int nCtrlID,int nVeticalPix,int nHorizontalPix);

	void _AddChart_CCG(V_CHARTDATAD& vData);
	void _AddChart_Fila(V_CHARTDATAD& vData);
	void _AddChart_EMI(V_CHARTDATAD& vData);
	void _AddChart_HV(V_CHARTDATAD& vData);
	void _UpdateChartData(const string& strCharName,V_CHARTDATAD& vData);
	void _ShowData();

public:
	CChartContainer4NegativeExponent m_chartContainer;

public:
	V_CHARTDATAD  m_vDataCCG;    //CCG点集
	V_CHARTDATAD  m_vDataHV;     //高压点集
	V_CHARTDATAD  m_vDataFila;   //灯丝电流点集
	V_CHARTDATAD  m_vDataEMI;    //束流点集
	__int64       m_i64MinDisplayTimeStamp;    //显示区间内的最小时间值(秒)
	__int64       m_i64MaxDisplayTimeStamp;    //显示区间内的最大时间值(秒)
	const int     m_nMaxMinute;                //最多显示多少分钟的数据
	const int     m_nFilterMinute;             //过滤后，保留多长时间（分钟）的数据
	const int     m_nMoveMinute;               //向前，向后移动多长时间（分钟）的数据

private:
	//时间控件
	BOOL          m_bChartCreated;    //图表是否已经创建
	int           m_nChartIdx;        //索引
	CButton       m_btnPlayPrevious;
    CButton       m_btnPlayNext;
	BOOL          m_bAutoRefresh;     //是否处于自动刷新模式
};
