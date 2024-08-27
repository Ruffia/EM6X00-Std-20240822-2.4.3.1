// DlgSEMLog_ShowCCG.cpp : 实现文件
//

#include "stdafx.h"
#include <random>
#include <iostream>
#include "DlgSEMLog_ShowRealTimeTrend.h"
#include "xxx2.h"
#include "Message.h"
#include "RealTimeDataManager.h"
#include "Util.h"
#include "Chart.h"
#include "Chart4NegativeExponent.h"
#include "AxisCCG.h"
#include "AxisFila.h"
#include "Chart4Fila.h"
#include "AxisEMI.h"
#include "Chart4EMI.h"
#include "AxisHV.h"
#include "Chart4HV.h"


// CDlgSEMLog_ShowRealTimeTrend 对话框

IMPLEMENT_DYNAMIC(CDlgSEMLog_ShowRealTimeTrend, CDialog)

CDlgSEMLog_ShowRealTimeTrend::CDlgSEMLog_ShowRealTimeTrend(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSEMLog_ShowRealTimeTrend::IDD, pParent),m_bChartCreated(FALSE),
	 m_nMaxMinute(25),m_nFilterMinute(20),m_nMoveMinute(5),m_bAutoRefresh(TRUE)
{
	m_vDataCCG.clear();
	m_vDataFila.clear();
	m_vDataHV.clear();
	m_vDataEMI.clear();
	m_i64MinDisplayTimeStamp = 0;
	m_i64MaxDisplayTimeStamp = 1;
}

CDlgSEMLog_ShowRealTimeTrend::~CDlgSEMLog_ShowRealTimeTrend()
{
}

void CDlgSEMLog_ShowRealTimeTrend::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STCHARTCONTAINER, m_chartContainer);
	DDX_Control(pDX, IDC_BUTTON_Previous, m_btnPlayPrevious);
	DDX_Control(pDX, IDC_BUTTON_Next, m_btnPlayNext);
}


BEGIN_MESSAGE_MAP(CDlgSEMLog_ShowRealTimeTrend, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_Previous, &CDlgSEMLog_ShowRealTimeTrend::OnBnClickedPrevious)
	ON_BN_CLICKED(IDC_BUTTON_Next, &CDlgSEMLog_ShowRealTimeTrend::OnBnClickedNext)
    ON_BN_CLICKED(IDC_BTN_AutoRefresh, &CDlgSEMLog_ShowRealTimeTrend::OnBnClickedBtnAutorefresh)
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


void CDlgSEMLog_ShowRealTimeTrend::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDC_BTN_AutoRefresh)->SetWindowText(GetResString(IDC_BTN_AutoRefresh));
}

// CDlgSEMLog_ShowRealTimeTrend 消息处理程序
void CDlgSEMLog_ShowRealTimeTrend::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType,cx,cy);
	_DesignLayout();
}


void CDlgSEMLog_ShowRealTimeTrend::OnTimer(UINT nIDEvent) 
{
	_ShowData();
	CDialog::OnTimer(nIDEvent);
}



//设计控件布局
void CDlgSEMLog_ShowRealTimeTrend::_DesignLayout()
{
	CWnd* pCtrl = GetDlgItem(IDC_STCHARTCONTAINER);
	if(!pCtrl) return;

	_MoveControl(IDC_BUTTON_Previous,35,65);
	_MoveControl(IDC_BUTTON_Next,35,65);
	_MoveControl(IDC_BTN_AutoRefresh,35,65);
	_MoveControl(IDC_STATIC_TimeInterval,40,0);
	
	CRect rc;
	pCtrl->GetWindowRect(rc);
	rc.left += -1;
	rc.top +=  -6;
	rc.right += 20;
	rc.bottom -= 40;
	ScreenToClient(rc);
	pCtrl->MoveWindow(rc);
}


void CDlgSEMLog_ShowRealTimeTrend::_MoveControl(int nCtrlID,int nVeticalPix,int nHorizontalPix)
{
	CWnd* pCtrl = GetDlgItem(nCtrlID);
	if(!pCtrl) return;

	CRect rc;
	pCtrl->GetWindowRect(rc);
	rc.left += nHorizontalPix;
	rc.right += nHorizontalPix;
	rc.top +=  nVeticalPix;
	rc.bottom +=  nVeticalPix;
	ScreenToClient(rc);
	pCtrl->MoveWindow(rc);
}


static bool Comp(CRealTimeLogData& pLeft,CRealTimeLogData& pRight)
{
	return pLeft.m_i64Time < pRight.m_i64Time;
};

void CDlgSEMLog_ShowRealTimeTrend::_ShowData()
{
	m_vDataCCG.clear();
	m_vDataHV.clear();
	m_vDataFila.clear();
	m_vDataEMI.clear();

	//当前时间
	__int64 _i64CurrentTime = GetCurrentTimeStampMS();
	if (m_bAutoRefresh)
	{
		if (m_i64MinDisplayTimeStamp <= _i64CurrentTime && _i64CurrentTime < m_i64MaxDisplayTimeStamp)
		{

		}
		else
		{
			if (_i64CurrentTime >= m_i64MaxDisplayTimeStamp)
			{
				//向前回溯一定的时间
				m_i64MinDisplayTimeStamp  = _i64CurrentTime - m_nFilterMinute * 60 * 1000;
				m_i64MaxDisplayTimeStamp  = m_i64MinDisplayTimeStamp + m_nMaxMinute * 60 * 1000;
			}
		}
	}

	if (!m_bAutoRefresh)
	{
		//如果在手动查看数据的情况下，时间区间已经移到当前时间点，则不刷新页面
		if (m_i64MinDisplayTimeStamp < _i64CurrentTime && _i64CurrentTime <= m_i64MaxDisplayTimeStamp)
		{
			return;
		}
	}

	map<__int64,CRealTimeLogData> mapShowData;
	CRealTimeDataManager::Instance().GetData(m_i64MinDisplayTimeStamp,m_i64MaxDisplayTimeStamp,mapShowData);

	const double dMinCCG = 1.0E+2;     //最差真空度
	const double dMaxCCG = 1.0E-10;    //最优真空度

	for (map<__int64,CRealTimeLogData>::iterator it = mapShowData.begin();
		it != mapShowData.end();it++)
	{
		CRealTimeLogData& objLogData = it->second;
		long long  _i64Value = objLogData.m_i64Time;
		double dCCG = objLogData.m_dCCG > dMinCCG ? dMinCCG:objLogData.m_dCCG;
		dCCG = objLogData.m_dCCG < dMaxCCG ? dMaxCCG:objLogData.m_dCCG;
		m_vDataCCG.push_back(PointD(_i64Value,dCCG));
		m_vDataHV.push_back(PointD(_i64Value,objLogData.m_dHV));
		m_vDataFila.push_back(PointD(_i64Value,objLogData.m_dFila));
		m_vDataEMI.push_back(PointD(_i64Value,objLogData.m_dEMI));
	}

	//std::random_device rd;  //将用于获取随机数引擎的种子
	//std::mt19937 gen(rd()); //以rd（）作为种子的标准mersenne_twister_engine
	//std::uniform_int_distribution<> distrib(0, 300);

	//for (int i = 0; i < m_vDataFila.size(); i++)
	//{
	//	PointD& pt = m_vDataFila[i];
	//	pt.Y = distrib(gen)/100.0; 
	//}

	//for (int i = 0; i < m_vDataEMI.size(); i++)
	//{
	//	PointD& pt = m_vDataEMI[i];
	//	pt.Y = distrib(gen); 
	//}

	if (!m_bChartCreated)
	{
		_AddChart_CCG(m_vDataCCG);
		_AddChart_Fila(m_vDataFila);
		_AddChart_EMI(m_vDataEMI);
		_AddChart_HV(m_vDataHV);
	}
	else
	{
		_UpdateChartData("CCG",m_vDataCCG);
		_UpdateChartData("Fila",m_vDataFila);
		_UpdateChartData("EMI",m_vDataEMI);
		_UpdateChartData("HV",m_vDataHV);
	}	
}


BOOL CDlgSEMLog_ShowRealTimeTrend::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetTimer(1,3000,0);
	m_btnPlayPrevious.SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_PlayPrevious)));
	m_btnPlayNext.SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_PlayNext)));
	m_chartContainer.SetContainerName(string_t(_T("LogData chart container")));

	__int64 _i64AppStartTime = CRealTimeDataManager::Instance().GetAppStartTime();
	//X坐标轴总长度25分钟
	__int64 _i64MaxExpectTime = _i64AppStartTime + m_nMaxMinute * 60 * 1000;
	//当前时间
	__int64 _i64CurrentTime = GetCurrentTimeStampMS();
	//计算自启动以来程序运行了多少秒
	if (_i64CurrentTime < _i64MaxExpectTime)
	{
		m_i64MinDisplayTimeStamp = _i64AppStartTime;
		m_i64MaxDisplayTimeStamp = _i64MaxExpectTime;
	}
	else
	{
		//向前回溯一定的时间
		m_i64MinDisplayTimeStamp  = _i64CurrentTime - m_nFilterMinute * 60 * 1000;
		m_i64MaxDisplayTimeStamp  = m_i64MinDisplayTimeStamp+ m_nMaxMinute * 60 * 1000;
	}

	_ShowData();

	UpdateData(FALSE);
	Localize();
	return TRUE; 
}

void CDlgSEMLog_ShowRealTimeTrend::_AddChart_Fila(V_CHARTDATAD& vData)
{
	USES_CONVERSION;
	if(vData.size() <= 0) return;

	float ftension = 0;
	Gdiplus::Color colChart(Color(255, 0, 128, 0));
	int	dashStyle = 0; 
	double precisionY = 1.0E-4;
	int nPntsNmb  = vData.size();
	float fPenWidth = 1;
	float fMultY     = pow(10.0, -4);
	CString tmpStr;
	string strName = _T("Fila");
	string strYValName = _T("YFila");

	const double dDeltaX = 60.0;
	const double dDeltaY = 1.0E-1;
	const double YAxisMax = 3.0;     //Y轴最大指数值
	const double YAxisMin = 0.0;     //Y轴最小指数值
	vector<double> vAxisX;           //X轴刻度值
	vector<double> vAxisY;           //Y轴刻度值

	int nMinX   = vData[0].X;
	for (int i = 0; i < m_nMaxMinute;i++)
	{
		vAxisX.push_back(nMinX + i * 60);
	}

	for (double dY = YAxisMax;dY > YAxisMin; dY -= dDeltaY)
	{
		vAxisY.push_back(dY);
	}

	CAxis* pAxis = new CAxisFila;
	pAxis->SetGridXGroupCount(vAxisX.size());
	pAxis->SetGridYGroupCount(vAxisY.size());
	pAxis->SetGridXGraducation(vAxisX);
	pAxis->SetGridYGraducation(vAxisY);
	pAxis->m_strName = "Fila";

	ChartProperty* pAttr = new ChartProperty(true, 
		strName.c_str(), strYValName.c_str(), precisionY, 
		DashStyle(dashStyle), 
		fPenWidth, float(ftension), 
		colChart);

	CChart* chartPtr = new CChart4Fila; 
	chartPtr->SetAxis(pAxis);
	chartPtr->SetChartAttr(pAttr);

	m_nChartIdx = m_chartContainer.AddChart(pAttr,chartPtr,vData, true); 
	if (m_nChartIdx == -1)
	{
		return;
	}

	strName.clear();
	UpdateData(FALSE);
	m_chartContainer.SetFocus();
	m_bChartCreated = TRUE;
}


void CDlgSEMLog_ShowRealTimeTrend::_AddChart_CCG(V_CHARTDATAD& vData)
{
	USES_CONVERSION;
	if(vData.size() <= 0) return;

	float ftension = 0;
	Gdiplus::Color colChart(Color(255, 0, 0, 128));
	int	dashStyle = 0; 
	double precisionY = 1.0E-4;
	int nPntsNmb  = vData.size();
	float fPenWidth = 1;
	float fMultY     = pow(10.0, -4);
	CString tmpStr;
	string strName = _T("CCG");
	string strYValName = _T("YCCG");

	const double dDeltaX = 60.0;
	const double dDeltaY = 1.0E-1;
	const double YAxisMax = 1.0E+2;  //Y轴最大指数值
	const double YAxisMin = 1.0E-5;  //Y轴最小指数值
	vector<double> vAxisX;           //X轴刻度值
	vector<double> vAxisY;           //Y轴刻度值

	//横坐标划分为多少个区域,区域间隔60秒
	int nMinX   = vData[0].X;
	for (int i = 0; i < m_nMaxMinute;i++)
	{
		vAxisX.push_back(nMinX + i * 60);
	}

	for (double dY = YAxisMax;dY > YAxisMin * dDeltaY; dY *= dDeltaY)
	{
		vAxisY.push_back(dY);
	}

	CAxis* pAxis = new CAxisCCG;
	pAxis->SetGridXGroupCount(vAxisX.size());
	pAxis->SetGridYGroupCount(vAxisY.size());
	pAxis->SetGridXGraducation(vAxisX);
	pAxis->SetGridYGraducation(vAxisY);
	pAxis->m_strName = "CCG";

	ChartProperty* pAttr = new ChartProperty(true, 
		strName.c_str(), strYValName.c_str(), precisionY, 
		DashStyle(dashStyle), 
		fPenWidth, float(ftension), 
		colChart);

	CChart* chartPtr = new CChart4NegativeExponent; 
	chartPtr->SetAxis(pAxis);
	chartPtr->SetChartAttr(pAttr);

	m_nChartIdx = m_chartContainer.AddChart(pAttr,chartPtr,vData, true); 
	if (m_nChartIdx == -1)
	{
		return;
	}

	strName.clear();
	UpdateData(FALSE);
	m_chartContainer.SetFocus();
	m_bChartCreated = TRUE;
}


void CDlgSEMLog_ShowRealTimeTrend::_AddChart_EMI(V_CHARTDATAD& vData)
{
	USES_CONVERSION;
	if(vData.size() <= 0) return;

	float ftension = 0;
	Gdiplus::Color colChart(Color(255, 0, 128, 0));
	int	dashStyle = 0; 
	double precisionY = 1.0E-4;
	int nPntsNmb  = vData.size();
	float fPenWidth = 1;
	float fMultY     = pow(10.0, -4);
	CString tmpStr;
	string strName = _T("EMI");
	string strYValName = _T("YEMI");
     
	const double dDeltaX = 60.0;
	const double dDeltaY = 10.0;
	const double YAxisMax = 300.0;     //Y轴最大指数值
	const double YAxisMin = 0.0;     //Y轴最小指数值
	vector<double> vAxisX;           //X轴刻度值
	vector<double> vAxisY;           //Y轴刻度值

	//横坐标划分为多少个区域,区域间隔60秒
	int nMinX   = vData[0].X;
	for (int i = 0; i < m_nMaxMinute;i++)
	{
		vAxisX.push_back(nMinX + i * 60);
	}

	for (double dY = YAxisMax;dY > YAxisMin; dY -= dDeltaY)
	{
		vAxisY.push_back(dY);
	}

	CAxis* pAxis = new CAxisEMI;
	pAxis->SetGridXGroupCount(vAxisX.size());
	pAxis->SetGridYGroupCount(vAxisY.size());
	pAxis->SetGridXGraducation(vAxisX);
	pAxis->SetGridYGraducation(vAxisY);
	pAxis->m_strName = "EMI";

	ChartProperty* pAttr = new ChartProperty(true, 
		strName.c_str(), strYValName.c_str(), precisionY, 
		DashStyle(dashStyle), 
		fPenWidth, float(ftension), 
		colChart);

	CChart* chartPtr = new CChart4EMI; 
	chartPtr->SetAxis(pAxis);
	chartPtr->SetChartAttr(pAttr);

	m_nChartIdx = m_chartContainer.AddChart(pAttr,chartPtr,vData, true); 
	if (m_nChartIdx == -1)
	{
		return;
	}

	strName.clear();
	UpdateData(FALSE);
	m_chartContainer.SetFocus();
	m_bChartCreated = TRUE;
}


void CDlgSEMLog_ShowRealTimeTrend::_AddChart_HV(V_CHARTDATAD& vData)
{
	USES_CONVERSION;
	if(vData.size() <= 0) return;

	float ftension = 0;
	Gdiplus::Color colChart(Color(255, 0, 128, 0));
	int	dashStyle = 0; 
	double precisionY = 1.0E-4;

	int nPntsNmb  = vData.size();
	float fPenWidth = 1;
	float fMultY     = pow(10.0, -4);
	CString tmpStr;
	string strName = _T("HV");
	string strYValName = _T("YHV");
     
	const double dDeltaX = 60.0;
	const double dDeltaY = 1;
	const double YAxisMax = 30;     //Y轴最大指数值
	const double YAxisMin = 0;      //Y轴最小指数值
	vector<double> vAxisX;           //X轴刻度值
	vector<double> vAxisY;           //Y轴刻度值

	//横坐标划分为多少个区域,区域间隔60秒
	int nMinX   = vData[0].X;
	for (int i = 0; i < m_nMaxMinute;i++)
	{
		vAxisX.push_back(nMinX + i * 60);
	}

	for (double dY = YAxisMax;dY > YAxisMin; dY -= dDeltaY)
	{
		vAxisY.push_back(dY);
	}

	CAxis* pAxis = new CAxisHV;
	pAxis->SetGridXGroupCount(vAxisX.size());
	pAxis->SetGridYGroupCount(vAxisY.size());
	pAxis->SetGridXGraducation(vAxisX);
	pAxis->SetGridYGraducation(vAxisY);
	pAxis->m_strName = "HV";

	ChartProperty* pAttr = new ChartProperty(true, 
		strName.c_str(), strYValName.c_str(), precisionY, 
		DashStyle(dashStyle), 
		fPenWidth, float(ftension), 
		colChart);

	CChart* chartPtr = new CChart4HV; 
	chartPtr->SetAxis(pAxis);
	chartPtr->SetChartAttr(pAttr);

	m_nChartIdx = m_chartContainer.AddChart(pAttr,chartPtr,vData, true); 
	if (m_nChartIdx == -1)
	{
		return;
	}

	strName.clear();
	UpdateData(FALSE);
	m_chartContainer.SetFocus();
	m_bChartCreated = TRUE;
}


void CDlgSEMLog_ShowRealTimeTrend::_UpdateChartData(const string& strCharName,V_CHARTDATAD& vData)
{
	USES_CONVERSION;
	if(vData.size() <= 0) return;
	//float fChMinX   = GetRelativeTime(vData[0].X,true);
	//float fChMaxX   = GetRelativeTime(vData[vData.size() - 1].X,false);
	int nMinX   = vData[0].X;

	int nPntsNmb  = vData.size();
	//横坐标划分为多少个区域,区域间隔60秒
	const double dDeltaX = 60.0;
	vector<double> vAxisX;           //X轴刻度值
	for (double dX = nMinX;dX <= nMinX + m_nMaxMinute * 60; dX = dX + dDeltaX)
	{
		vAxisX.push_back(dX);

		time_t lTimeSatamp = dX;
		lTimeSatamp += 28800;//GTM偏移8个时区得到北京时间
		tm stime;
		gmtime_s(&stime, &lTimeSatamp);
		char sz[64] = {0};
		strftime(sz, 64, "%H:%M:%S", &stime);
		string_t strX = sz;
	}


	CChart* pChart = m_chartContainer.FindChartByName(strCharName);
	if(!pChart) return;
	CAxis* pAxis = pChart->GetAxis();
	if(!pAxis) return;
	pAxis->SetGridXGroupCount(vAxisX.size() - 1);
	pAxis->SetGridXGraducation(vAxisX);

	m_chartContainer.ReplaceChartData(pChart->m_nIdx,vData);

	UpdateData(FALSE);
	m_chartContainer.SetFocus();
	m_chartContainer.RefreshWnd();
}

void CDlgSEMLog_ShowRealTimeTrend::OnBnClickedPrevious()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bAutoRefresh = FALSE;
	//当前时间
	__int64 _i64CurrentTime = GetCurrentTimeStampMS();
	//向前回溯一定的时间
	__int64 _i64MinDisplayTimeStamp  = m_i64MinDisplayTimeStamp - m_nMoveMinute * 60 * 1000;
	__int64 _i64MaxDisplayTimeStamp  = _i64MinDisplayTimeStamp + m_nMaxMinute * 60 * 1000;

	map<__int64,CRealTimeLogData> mapShowData;
	CRealTimeDataManager::Instance().GetData(_i64MinDisplayTimeStamp,_i64MaxDisplayTimeStamp,mapShowData);
	const int nDataCount = mapShowData.size();
	if (nDataCount <= 0)
	{
		MessageBox("没有更多数据了");
		return;
	}
	
	if (nDataCount >= m_nMaxMinute * 60)
	{
		m_i64MinDisplayTimeStamp = _i64MinDisplayTimeStamp;
		m_i64MaxDisplayTimeStamp = _i64MaxDisplayTimeStamp;
	}
	else
	{
		vector<CRealTimeLogData> vRealTimeData;
		for(map<__int64,CRealTimeLogData>::iterator it = mapShowData.begin();
			it != mapShowData.end();it++)
		{
			vRealTimeData.push_back(it->second);
		}

		::sort(vRealTimeData.begin(),vRealTimeData.end(),Comp);
		m_i64MinDisplayTimeStamp = vRealTimeData[0].m_i64Time * 1000;
		m_i64MaxDisplayTimeStamp = m_i64MinDisplayTimeStamp + m_nMaxMinute * 60 * 1000;
	}

}


void CDlgSEMLog_ShowRealTimeTrend::OnBnClickedNext()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bAutoRefresh = FALSE;
	//当前时间
	__int64 _i64CurrentTime = GetCurrentTimeStampMS();

	//向后查看一定的时间
	__int64 _i64MinDisplayTimeStamp  = m_i64MinDisplayTimeStamp + m_nMoveMinute * 60 * 1000;
	__int64 _i64MaxDisplayTimeStamp  = _i64MinDisplayTimeStamp + m_nMaxMinute * 60 * 1000;

	map<__int64,CRealTimeLogData> mapShowData;
	CRealTimeDataManager::Instance().GetData(_i64MinDisplayTimeStamp,_i64MaxDisplayTimeStamp,mapShowData);
	const int nDataCount = mapShowData.size();
	if (nDataCount <= 0)
	{
		MessageBox("没有更多数据了");
		return;
	}

	if (nDataCount >= m_nMaxMinute * 60)
	{
		m_i64MinDisplayTimeStamp = _i64MinDisplayTimeStamp;
		m_i64MaxDisplayTimeStamp = _i64MaxDisplayTimeStamp;
	}
	else
	{
		vector<CRealTimeLogData> vRealTimeData;
		for(map<__int64,CRealTimeLogData>::iterator it = mapShowData.begin();
			it != mapShowData.end();it++)
		{
			vRealTimeData.push_back(it->second);
		}

		::sort(vRealTimeData.begin(),vRealTimeData.end(),Comp);
		m_i64MinDisplayTimeStamp = vRealTimeData[0].m_i64Time * 1000;
		m_i64MaxDisplayTimeStamp = m_i64MinDisplayTimeStamp + m_nMaxMinute * 60 * 1000;
	}
}


void CDlgSEMLog_ShowRealTimeTrend::OnBnClickedBtnAutorefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bAutoRefresh = TRUE;
}
