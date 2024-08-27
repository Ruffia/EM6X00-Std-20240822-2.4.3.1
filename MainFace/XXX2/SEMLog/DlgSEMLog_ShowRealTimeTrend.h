#pragma once
#include "resource.h"
#include "afxdialogex.h"
#include "ChartContainer4NegativeExponent.h"
#include "afxwin.h"

// CDlgSEMLog_ShowRealTimeTrend �Ի���
class CDlgSEMLog_ShowRealTimeTrend : public CDialog
{
	DECLARE_DYNAMIC(CDlgSEMLog_ShowRealTimeTrend)

public:
	CDlgSEMLog_ShowRealTimeTrend(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSEMLog_ShowRealTimeTrend();
	void	Localize(void);

// �Ի�������
	enum { IDD = IDD_DLG_SEMLog_RealTimeTrend };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedPrevious();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedBtnAutorefresh();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);

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
	void _UpdateChartData(const string& strCharName,V_CHARTDATAD& vData);
	void _ShowData();

public:
	CChartContainer4NegativeExponent m_chartContainer;

public:
	V_CHARTDATAD  m_vDataCCG;    //CCG�㼯
	V_CHARTDATAD  m_vDataHV;     //��ѹ�㼯
	V_CHARTDATAD  m_vDataFila;   //��˿�����㼯
	V_CHARTDATAD  m_vDataEMI;    //�����㼯
	__int64       m_i64MinDisplayTimeStamp;    //��ʾ�����ڵ���Сʱ��ֵ(��)
	__int64       m_i64MaxDisplayTimeStamp;    //��ʾ�����ڵ����ʱ��ֵ(��)
	const int     m_nMaxMinute;                //�����ʾ���ٷ��ӵ�����
	const int     m_nFilterMinute;             //���˺󣬱����೤ʱ�䣨���ӣ�������
	const int     m_nMoveMinute;               //��ǰ������ƶ��೤ʱ�䣨���ӣ�������

private:
	//ʱ��ؼ�
	BOOL          m_bChartCreated;    //ͼ���Ƿ��Ѿ�����
	int           m_nChartIdx;        //����
	CButton       m_btnPlayPrevious;
    CButton       m_btnPlayNext;
	BOOL          m_bAutoRefresh;     //�Ƿ����Զ�ˢ��ģʽ
};
