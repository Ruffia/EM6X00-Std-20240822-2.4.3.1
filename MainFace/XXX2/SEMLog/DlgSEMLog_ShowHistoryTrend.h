#pragma once
#include "resource.h"
#include "afxdialogex.h"
#include "DataSet.h"
#include "ChartContainer4NegativeExponent.h"
#include "afxwin.h"

// CDlgSEMLog_ShowHistoryTrend �Ի���
class CDlgSEMLog_ShowHistoryTrend : public CDialog
{
	DECLARE_DYNAMIC(CDlgSEMLog_ShowHistoryTrend)

public:
	CDlgSEMLog_ShowHistoryTrend(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSEMLog_ShowHistoryTrend();
	void	Localize(void);

// �Ի�������
	enum { IDD = IDD_DLG_SEMLog_HistoryTrend };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedPreviousPage();
	afx_msg void OnBnClickedQuery();
	afx_msg void OnBnClickedNextPage();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	HRESULT OnLoadPageData(WPARAM wParam, LPARAM lParam);

protected:

	//��ƿؼ�����
	void _DesignLayout();

	//��ֱ�ƶ��ؼ�
	//nVeticalPix,��ֱ�ƶ������ص�λ
	//nHorizontalPix,ˮƽ�ƶ������ص�λ
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
	int  m_nCurrentPage;  //��ǰҳ��
	int  m_nTotalPage;    //��ҳ��
	const int  m_nRecordCountPerPage;
	CString       m_strPageNO;         //��ǰҳ��
	CString       m_strTimeInterval;   //��ҳ���ݵ�ʱ������
	CDataSet      m_ds;      //���ݼ�
	V_CHARTDATAD  m_vDataCCG;    //CCG�㼯
	V_CHARTDATAD  m_vDataHV;     //��ѹ�㼯
	V_CHARTDATAD  m_vDataFila;   //��˿�����㼯
	V_CHARTDATAD  m_vDataEMI;   //�����㼯

private:
	//ʱ��ؼ�
	CDateTimeCtrl m_DayBegin;
	CDateTimeCtrl m_TimeBegin;
	CDateTimeCtrl m_DayEnd;
	CDateTimeCtrl m_TimeEnd;
	BOOL          m_bChartCreated;    //ͼ���Ƿ��Ѿ�����
	int           m_nChartIdx;        //����
	CButton       m_btnPlayPrevious;
    CButton m_btnPlayNext;
};
