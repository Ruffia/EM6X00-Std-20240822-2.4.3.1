// DlgSEMLog_ShowCCG.cpp : 实现文件
//

#include "stdafx.h"
#include <random>
#include <iostream>
#include "DlgSEMLog_ShowHistoryTrend.h"
#include "xxx2.h"
#include "Message.h"
#include "DBDataManager.h"
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


// CDlgSEMLog_ShowHistoryTrend 对话框

IMPLEMENT_DYNAMIC(CDlgSEMLog_ShowHistoryTrend, CDialog)

CDlgSEMLog_ShowHistoryTrend::CDlgSEMLog_ShowHistoryTrend(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSEMLog_ShowHistoryTrend::IDD, pParent),m_bChartCreated(FALSE),
	m_nCurrentPage(0),m_nTotalPage(1),m_nRecordCountPerPage(3000),m_strPageNO(_T(""))
	, m_strTimeInterval(_T(""))
{
	m_vDataCCG.clear();
}

CDlgSEMLog_ShowHistoryTrend::~CDlgSEMLog_ShowHistoryTrend()
{
}

void CDlgSEMLog_ShowHistoryTrend::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STCHARTCONTAINER, m_chartContainer);
	DDX_Text(pDX, IDC_STATIC_PageNO_CCGCurve, m_strPageNO);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DateBegin, m_DayBegin);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TimeBegin, m_TimeBegin);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DateEnd, m_DayEnd);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TimeEnd, m_TimeEnd);
	DDX_Text(pDX, IDC_STATIC_TimeInterval, m_strTimeInterval);
	DDX_Control(pDX, IDC_BUTTON_PreviousPage_CCGCurve, m_btnPlayPrevious);
	DDX_Control(pDX, IDC_BUTTON_NextPage_CCGCurve, m_btnPlayNext);
}


BEGIN_MESSAGE_MAP(CDlgSEMLog_ShowHistoryTrend, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PreviousPage_CCGCurve, &CDlgSEMLog_ShowHistoryTrend::OnBnClickedPreviousPage)
	ON_BN_CLICKED(IDC_BUTTON_NextPage_CCGCurve, &CDlgSEMLog_ShowHistoryTrend::OnBnClickedNextPage)
	ON_BN_CLICKED(IDC_BUTTON_Query, &CDlgSEMLog_ShowHistoryTrend::OnBnClickedQuery)
	ON_MESSAGE(WM_MsgAsynLoadPageData,OnLoadPageData)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CDlgSEMLog_ShowHistoryTrend::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDC_STATIC_TimeBegin)->SetWindowText(GetResString(IDC_STATIC_TimeBegin));
	GetDlgItem(IDC_STATIC_TimeEnd)->SetWindowText(GetResString(IDC_STATIC_TimeEnd));
	GetDlgItem(IDC_BUTTON_Query)->SetWindowText(GetResString(IDC_BUTTON_Query));
}

// CDlgSEMLog_ShowHistoryTrend 消息处理程序
void CDlgSEMLog_ShowHistoryTrend::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType,cx,cy);
	_DesignLayout();
}


//设计控件布局
void CDlgSEMLog_ShowHistoryTrend::_DesignLayout()
{
	CWnd* pCtrl = GetDlgItem(IDC_STCHARTCONTAINER);
	if(!pCtrl) return;

	_MoveControl(IDC_BUTTON_PreviousPage_CCGCurve,35,65);
	_MoveControl(IDC_STATIC_PageNO_CCGCurve,40,65);
	_MoveControl(IDC_BUTTON_NextPage_CCGCurve,35,65);
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


void CDlgSEMLog_ShowHistoryTrend::_MoveControl(int nCtrlID,int nVeticalPix,int nHorizontalPix)
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


__int64 CDlgSEMLog_ShowHistoryTrend::_GetTimeStamp(bool bBeginDateTime)
{
	CTime tDay, tTime;
	__int64 timestamp = 0;
	DWORD dwResult = 0;
	if (bBeginDateTime)
	{
		dwResult = m_DayBegin.GetTime(tDay);
		dwResult = m_TimeBegin.GetTime(tTime);
	}
	else
	{
		DWORD dwResult = m_DayEnd.GetTime(tDay);
		dwResult = m_TimeEnd.GetTime(tTime);
	}

	COleDateTime tDateTime(tDay.GetYear(), tDay.GetMonth(), tDay.GetDay(), 
		tTime.GetHour(), tTime.GetMinute(), tTime.GetSecond());

	SYSTEMTIME systime;
	VariantTimeToSystemTime(tDateTime, &systime);
	//时间戳最小值为北京时间:1970-01-01 08:00:00
	if (systime.wYear <= 1970 && systime.wMonth <= 1 && systime.wDay <= 1 && systime.wHour <= 7 
		&& systime.wMinute <= 59 && systime.wSecond <= 59)
		return -1;
	CTime cTimeFromDB(systime);
	timestamp = cTimeFromDB.GetTime() * pow(10,3);//CTime->时间戳
	return timestamp;
}


HRESULT CDlgSEMLog_ShowHistoryTrend::OnLoadPageData(WPARAM wParam, LPARAM lParam)
{
	int nType = int(wParam);
	_LoadPageData(nType);
	UpdateData(FALSE);
	return 0L;
}


void CDlgSEMLog_ShowHistoryTrend::_LoadPageData(const int nType) 
{
	const __int64 tTimeStampBegin = _GetTimeStamp(true);
	const __int64 tTimeStampEnd   = _GetTimeStamp(false);
	if (tTimeStampBegin > tTimeStampEnd)
	{
		MessageBox("结束时间大于开始时间!");
		return;
	}

	char szTimeCondition[128] = {0};
	sprintf_s(szTimeCondition,128,"Time >= %I64d and Time <= %I64d",tTimeStampBegin,tTimeStampEnd);

	char szSQL[256] = {0};
	const char* sField4Cluster = "Time";     // 聚集索引字段名
	const char* sCompareCondition = "<=";    // 比较符号
	string strTableName = CDBDataManager::Instance().GetLogTableName();

	int nPage = 0;
	if (0 == nType)
	{
		nPage = m_nCurrentPage;
	}
	else if (1 == nType)
	{
		nPage = m_nCurrentPage - 1;
	}
	else if (2 == nType)
	{
		nPage = m_nCurrentPage + 1;
	}

	sprintf_s(szSQL,256,"select Time,CCG,EHT_m,Heat_m2,EMI from %s where %s order by %s asc limit %d offset %d",
		strTableName.c_str(),szTimeCondition, sField4Cluster, m_nRecordCountPerPage, nPage*m_nRecordCountPerPage);

	string sSQL = szSQL;
	CDBDataManager::Instance().LoadData(sSQL,strTableName,m_ds);
	if (m_ds.Size() > 0)
	{
		if (1 == nType)
		{
			m_nCurrentPage--;
		}
		else if (2 == nType)
		{
			m_nCurrentPage++;
		}	
	}
	m_strPageNO.Format(_T("第 %d 页"), m_nCurrentPage + 1);

	//将数据显示在UI上
	ShowData(m_ds);
}


bool Comp(PointD& pLeft,PointD& pRight)
{
	return pLeft.Y < pRight.Y;
};

void CDlgSEMLog_ShowHistoryTrend::ShowData(CDataSet &ds)
{
	m_vDataCCG.clear();
	m_vDataHV.clear();
	m_vDataFila.clear();
	m_vDataEMI.clear();
	const int nCount = ds.Size();
	for (int i = 0; i < nCount; i++)
	{
		CRecord* pRecord = ds.GetAt(i);
		if(!pRecord) continue;
		CField* pFieldTime = pRecord->GetField("Time");
		if(!pFieldTime) continue;
		CField* pFieldCCG = pRecord->GetField("CCG");
		if(!pFieldCCG) continue;
		CField* pFieldEHT = pRecord->GetField("EHT_m");
		if(!pFieldEHT) continue;
		CField* pFieldHeat = pRecord->GetField("Heat_m2");
		if(!pFieldHeat) continue;

		CField* pFieldEMI = pRecord->GetField("EMI");
		if(!pFieldEMI) continue;

		long long  _i64Value = pFieldTime->GetValueAsLongLong();
		double dValueCCG = pFieldCCG->GetValueAsDouble();
		m_vDataCCG.push_back(PointD(_i64Value,dValueCCG));

		double dValueHV = pFieldEHT->GetValueAsFloat()/1000;
		m_vDataHV.push_back(PointD(_i64Value,dValueHV));

		double dValueFila = pFieldHeat->GetValueAsFloat();
		m_vDataFila.push_back(PointD(_i64Value,dValueFila));

		double dValueEMI = pFieldEMI->GetValueAsFloat();
		m_vDataEMI.push_back(PointD(_i64Value,dValueEMI));
	}

	::reverse(m_vDataCCG.begin(),m_vDataCCG.end());

	double dMin = 1.0E+2;
	double dMax = 1.0E-10;
	map<__int64,double> mapTimeStamp2CCG;
	map<__int64,double> mapTimeStamp2EHT;
	map<__int64,double> mapTimeStamp2Fila;
	map<__int64,double> mapTimeStamp2EMI;
	for (int i = 0; i < m_vDataCCG.size();i++)
	{
		dMin = m_vDataCCG[i].Y < dMin ? m_vDataCCG[i].Y:dMin;
		dMax = m_vDataCCG[i].Y > dMax ? m_vDataCCG[i].Y:dMax;

		int nSecond = m_vDataCCG[i].X/1000;
		mapTimeStamp2CCG[nSecond] = m_vDataCCG[i].Y;
		mapTimeStamp2EHT[nSecond] = m_vDataHV[i].Y;
		mapTimeStamp2Fila[nSecond] = m_vDataFila[i].Y;
		mapTimeStamp2EMI[nSecond] = m_vDataEMI[i].Y;
	}

	m_vDataCCG.clear();
	m_vDataHV.clear();
	m_vDataFila.clear();
	m_vDataEMI.clear();
	for (map<__int64,double>::iterator it = mapTimeStamp2CCG.begin();
		it != mapTimeStamp2CCG.end();it++)
	{
		m_vDataCCG.push_back(PointD(it->first,it->second));
	}

	for (map<__int64,double>::iterator it = mapTimeStamp2EHT.begin();
		it != mapTimeStamp2EHT.end();it++)
	{
		m_vDataHV.push_back(PointD(it->first,it->second));
	}

	for (map<__int64,double>::iterator it = mapTimeStamp2Fila.begin();
		it != mapTimeStamp2Fila.end();it++)
	{
		m_vDataFila.push_back(PointD(it->first,it->second));
	}

	for (map<__int64,double>::iterator it = mapTimeStamp2EMI.begin();
		it != mapTimeStamp2EMI.end();it++)
	{
		m_vDataEMI.push_back(PointD(it->first,it->second));
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


BOOL CDlgSEMLog_ShowHistoryTrend::OnInitDialog()
{
	CDialog::OnInitDialog();

	CTime timeNow = CTime::GetCurrentTime();
	CTimeSpan sp(-1,0,0,0);
	CTime timeYesterday = timeNow + sp;
	m_DayBegin.SetTime(&timeYesterday);

	m_btnPlayPrevious.SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_PlayPrevious)));
	m_btnPlayNext.SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_PlayNext)));
	m_chartContainer.SetContainerName(string_t(_T("LogData chart container")));

	//PostMessage(WM_MsgAsynLoadPageData,WPARAM(0),LPARAM(0));

	UpdateData(FALSE);
	Localize();
	return TRUE; 
}

void CDlgSEMLog_ShowHistoryTrend::_AddChart_Fila(V_CHARTDATAD& vData)
{
	USES_CONVERSION;
	if(vData.size() <= 0) return;

	float ftension = 0;
	Gdiplus::Color colChart(Color(255, 0, 128, 0));
	int	dashStyle = 0; 
	double precisionY = 1.0E-4;
	double fChMinX   = vData[0].X;
	double fChMaxX   = vData[vData.size() - 1].X;
	_SetTimeIvterval(fChMinX,fChMaxX);
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

	//横坐标划分为多少个区域,区域间隔60秒
	double dX = 0;
	for (dX = fChMinX;dX < fChMaxX; dX = dX + dDeltaX)
	{
		vAxisX.push_back(dX);
	}

	if (abs(fChMaxX - dX) <= 1.0E-15)
	{
	}
	else
	{
		vAxisX.push_back(dX);
	}


	for (double dY = YAxisMax;dY > YAxisMin; dY -= dDeltaY)
	{
		vAxisY.push_back(dY);
	}

	CAxis* pAxis = new CAxisFila;
	pAxis->SetGridXGroupCount(vAxisX.size() - 1);
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


void CDlgSEMLog_ShowHistoryTrend::_AddChart_CCG(V_CHARTDATAD& vData)
{
	USES_CONVERSION;
	if(vData.size() <= 0) return;

	float ftension = 0;
	Gdiplus::Color colChart(Color(255, 0, 0, 128));
	int	dashStyle = 0; 
	double precisionY = 1.0E-4;
	//float fChMinX   = GetRelativeTime(vData[0].X,true);
	//float fChMaxX   = GetRelativeTime(vData[vData.size() - 1].X,false);
	double fChMinX   = vData[0].X;
	double fChMaxX   = vData[vData.size() - 1].X;
	_SetTimeIvterval(fChMinX,fChMaxX);
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

	double dX = 0;
	for (dX = fChMinX;dX < fChMaxX; dX = dX + dDeltaX)
	{
		vAxisX.push_back(dX);
	}

	if (abs(fChMaxX - dX) <= 1.0E-15)
	{
	}
	else
	{
		vAxisX.push_back(dX);
	}

	for (double dY = YAxisMax;dY > YAxisMin * dDeltaY; dY *= dDeltaY)
	{
		vAxisY.push_back(dY);
	}

	CAxis* pAxis = new CAxisCCG;
	pAxis->SetGridXGroupCount(vAxisX.size() - 1);
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


void CDlgSEMLog_ShowHistoryTrend::_AddChart_EMI(V_CHARTDATAD& vData)
{
	USES_CONVERSION;
	if(vData.size() <= 0) return;

	float ftension = 0;
	Gdiplus::Color colChart(Color(255, 0, 128, 0));
	int	dashStyle = 0; 
	double precisionY = 1.0E-4;
	double fChMinX   = vData[0].X;
	double fChMaxX   = vData[vData.size() - 1].X;
	_SetTimeIvterval(fChMinX,fChMaxX);
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

    double dX = 0;
	for (dX = fChMinX;dX < fChMaxX; dX = dX + dDeltaX)
	{
		vAxisX.push_back(dX);
	}

	if (abs(fChMaxX - dX) <= 1.0E-15)
	{
	}
	else
	{
		vAxisX.push_back(dX);
	}


	for (double dY = YAxisMax;dY > YAxisMin; dY -= dDeltaY)
	{
		vAxisY.push_back(dY);
	}

	CAxis* pAxis = new CAxisEMI;
	pAxis->SetGridXGroupCount(vAxisX.size() - 1);
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


void CDlgSEMLog_ShowHistoryTrend::_AddChart_HV(V_CHARTDATAD& vData)
{
	USES_CONVERSION;
	if(vData.size() <= 0) return;

	float ftension = 0;
	Gdiplus::Color colChart(Color(255, 0, 128, 0));
	int	dashStyle = 0; 
	double precisionY = 1.0E-4;
	double fChMinX   = vData[0].X;
	double fChMaxX   = vData[vData.size() - 1].X;
	_SetTimeIvterval(fChMinX,fChMaxX);
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
    double dX = 0;
	for (dX = fChMinX;dX < fChMaxX; dX = dX + dDeltaX)
	{
		vAxisX.push_back(dX);
	}

	if (abs(fChMaxX - dX) <= 1.0E-15)
	{
	}
	else
	{
		vAxisX.push_back(dX);
	}


	for (double dY = YAxisMax;dY > YAxisMin; dY -= dDeltaY)
	{
		vAxisY.push_back(dY);
	}

	CAxis* pAxis = new CAxisHV;
	pAxis->SetGridXGroupCount(vAxisX.size() - 1);
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


void CDlgSEMLog_ShowHistoryTrend::_UpdateChartData(const string& strCharName,V_CHARTDATAD& vData)
{
	USES_CONVERSION;
	if(vData.size() <= 0) return;
	//float fChMinX   = GetRelativeTime(vData[0].X,true);
	//float fChMaxX   = GetRelativeTime(vData[vData.size() - 1].X,false);
	double fChMinX   = vData[0].X;
	double fChMaxX   = vData[vData.size() - 1].X;
	_SetTimeIvterval(fChMinX,fChMaxX);

	int nPntsNmb  = vData.size();      
	const double dDeltaX = 60.0;
	vector<double> vAxisX;           //X轴刻度值
	//横坐标划分为多少个区域,区域间隔60秒
	double dX = 0;
	for (dX = fChMinX;dX < fChMaxX; dX = dX + dDeltaX)
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

	if (abs(fChMaxX - dX) <= 1.0E-5)
	{
	}
	else
	{
		vAxisX.push_back(dX);
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
}

void CDlgSEMLog_ShowHistoryTrend::OnBnClickedQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	PostMessage(WM_MsgAsynLoadPageData,WPARAM(0),LPARAM(0));
}

void CDlgSEMLog_ShowHistoryTrend::OnBnClickedPreviousPage()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_nCurrentPage < 1) return;
	PostMessage(WM_MsgAsynLoadPageData,WPARAM(1),LPARAM(0));
}


void CDlgSEMLog_ShowHistoryTrend::OnBnClickedNextPage()
{
	// TODO: 在此添加控件通知处理程序代码
	PostMessage(WM_MsgAsynLoadPageData,WPARAM(2),LPARAM(0));
}

void CDlgSEMLog_ShowHistoryTrend::_SetTimeIvterval(float fMin,float fMax)
{
	time_t lTimeSatampMin = fMin;
	lTimeSatampMin += 28800;//GTM偏移8个时区得到北京时间
	tm stimeMin;
	gmtime_s(&stimeMin, &lTimeSatampMin);
	char szMin[64] = {0};
	strftime(szMin, 64, "%Y-%m-%d %H:%M:%S", &stimeMin);

	time_t lTimeSatampMax = fMax;
	lTimeSatampMax += 28800;//GTM偏移8个时区得到北京时间
	tm stimeMax;
	gmtime_s(&stimeMax, &lTimeSatampMax);
	char szMax[64] = {0};
	strftime(szMax, 64, "%Y-%m-%d %H:%M:%S", &stimeMax);

	m_strTimeInterval.Format("（%s 至 %s）",szMin,szMax);
}
