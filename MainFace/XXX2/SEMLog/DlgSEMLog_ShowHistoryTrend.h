#pragma once
#include "resource.h"
#include "afxdialogex.h"
#include "DataSet.h"
#include "ChartContainer4NegativeExponent.h"
#include "afxwin.h"

// CDlgSEMLog_ShowHistoryTrend 对话框
class CDlgSEMLog_ShowHistoryTrend : public CDialog
{
	DECLARE_DYNAMIC(CDlgSEMLog_ShowHistoryTrend)

public:
	CDlgSEMLog_ShowHistoryTrend(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSEMLog_ShowHistoryTrend();
	void	Localize(void);

// 对话框数据
	enum { IDD = IDD_DLG_SEMLog_HistoryTrend };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedPreviousPage();
	afx_msg void OnBnClickedQuery();
	afx_msg void OnBnClickedNextPage();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	HRESULT OnLoadPageData(WPARAM wParam, LPARAM lParam);

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

	void _SetTimeIvterval(float fMin,float fMax);

	void _UpdateChartData(const string& strCharName,V_CHARTDATAD& vData);

	__int64 _GetTimeStamp(bool bBeginDateTime);

	void _LoadPageData(const int nType);

	void ShowData(CDataSet &ds);

public:
	CChartContainer4NegativeExponent m_chartContainer;

public:
	int  m_nCurrentPage;  //当前页码
	int  m_nTotalPage;    //总页码
	const int  m_nRecordCountPerPage;
	CString       m_strPageNO;         //当前页码
	CString       m_strTimeInterval;   //本页数据的时间区间
	CDataSet      m_ds;      //数据集
	V_CHARTDATAD  m_vDataCCG;    //CCG点集
	V_CHARTDATAD  m_vDataHV;     //高压点集
	V_CHARTDATAD  m_vDataFila;   //灯丝电流点集
	V_CHARTDATAD  m_vDataEMI;   //束流点集

private:
	//时间控件
	CDateTimeCtrl m_DayBegin;
	CDateTimeCtrl m_TimeBegin;
	CDateTimeCtrl m_DayEnd;
	CDateTimeCtrl m_TimeEnd;
	BOOL          m_bChartCreated;    //图表是否已经创建
	int           m_nChartIdx;        //索引
	CButton       m_btnPlayPrevious;
    CButton m_btnPlayNext;
};
