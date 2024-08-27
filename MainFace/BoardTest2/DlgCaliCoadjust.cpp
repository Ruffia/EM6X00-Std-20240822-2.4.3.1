// DlgCaliCoadjust.cpp : implementation file
//

#include "stdafx.h"
#include "insample0.h"
#include "DlgCaliCoadjust.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void LeastSquares( int nCounts, double* pdC, double* pdK, double& dKa, double& dKb )
{
	double l, m, n, p;
	l = 0.0;
	m = 0.0;
	n = 0.0;
	p = 0.0;
	
	for( int i=0; i<nCounts; i++ )
	{
		l += pdC[i];
		m += pdK[i];
		n += pdC[i]*pdK[i];
		p += pdC[i]*pdC[i];
	}
	dKa = ( nCounts *n -l *m ) / ( nCounts *p -l *l );
	dKb = ( p *m -n *l ) / ( nCounts *p -l *l );
}

void LeastSquares2( int nCounts, double* pdC, double* pdK, double& dKa, double& dKb )
{
	double midx, midy;
	double tempMu, tempZi;
	int i;
	
	// 求平均值
	midx = 0.0;
	midy = 0.0;
	for( i=0; i<nCounts; i++ )
	{
		midx += ( 1.0 *pdC[i] /nCounts);
		midy += ( 1.0 *pdK[i] /nCounts);
	}
	
	// 求斜率
	tempMu	= 0.0;
	tempZi	= 0.0;
	for( i=0; i<nCounts; i++ )
	{
		tempMu	+= ( (pdC[i] -midx) * (pdC[i] -midx) );
		tempZi	+= ( (pdC[i] -midx) * (pdK[i] -midy) );
	}
	if( tempMu == 0 )
		printf( "Fail!\n" );
	dKa = tempZi /tempMu;
	
	// 求截距
	dKb = midy -dKa *midx;
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliCoadjust dialog

CDlgCaliCoadjust::CDlgCaliCoadjust(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCaliCoadjust::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCaliCoadjust)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dCo_Ka	= 0.0;
	m_dCo_Kb	= 0.0;
}


void CDlgCaliCoadjust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCaliCoadjust)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCaliCoadjust, CDialog)
	//{{AFX_MSG_MAP(CDlgCaliCoadjust)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Coadjust_1, OnBUTTONATCaliCoadjust1)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Coadjust_2, OnBUTTONATCaliCoadjust2)
	ON_BN_CLICKED(IDC_BUTTON_AT_Cali_Coadjust_3, OnBUTTONATCaliCoadjust3)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_CHECK_AT_Cali_Coadjust_1, IDC_CHECK_AT_Cali_Coadjust_10, OnCheckChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCaliCoadjust message handlers

BOOL CDlgCaliCoadjust::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateCaliData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCaliCoadjust::UpdateCaliData()
{
	CString str;
	str.Format( "%d", theApp.m_Cali.m_nCaliCoCounts );
	GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Counts )->SetWindowText( str );

	for( int i=0; i<10; i++ )
	{
		str.Format( "%.6f", theApp.m_Cali.m_dCaliCoC[i] );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Ctrl1 +i )->SetWindowText( str );
		str.Format( "%.6f", theApp.m_Cali.m_dCaliCoK[i] );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_K1 +i )->SetWindowText( str );

		if( theApp.m_Cali.m_bCaliCoSel[i] )
		{
			((CButton*)(GetDlgItem( IDC_CHECK_AT_Cali_Coadjust_1 + i)))->SetCheck( 1 );
			GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Ctrl1 +i )->EnableWindow( TRUE );
			GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_K1 +i )->EnableWindow( TRUE );
		}
		else
		{
			((CButton*)(GetDlgItem( IDC_CHECK_AT_Cali_Coadjust_1 + i)))->SetCheck( 0 );
			GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Ctrl1 +i )->EnableWindow( FALSE );
			GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_K1 +i )->EnableWindow( FALSE );
		}
	}
}

void CDlgCaliCoadjust::OnBUTTONATCaliCoadjust1() 
{
	// 获取最新数据
	CString str;
	GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Counts )->GetWindowText( str );
	theApp.m_Cali.m_nCaliCoCounts = atoi( str );
	for( int i=0; i<10; i++ )
	{
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Ctrl1 +i )->GetWindowText( str );
		theApp.m_Cali.m_dCaliCoC[i] = atof( str );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_K1 +i )->GetWindowText( str );
		theApp.m_Cali.m_dCaliCoK[i] = atof( str );
		theApp.m_Cali.m_bCaliCoSel[i] = ((CButton*)(GetDlgItem( IDC_CHECK_AT_Cali_Coadjust_1 + i)))->GetCheck();
	}

	// 数据个数是否符合要求
	if( theApp.m_Cali.m_nCaliCoCounts < 2 )
	{
		AfxMessageBox( "Counts Error !" );
		return;
	}

	// 用最小二乘法拟合直线方程
	double* pdC	= new double[theApp.m_Cali.m_nCaliCoCounts];
	double* pdK	= new double[theApp.m_Cali.m_nCaliCoCounts];
	int nIndex = 0;
	for( i=0; i<10; i++ )
	{
		if( theApp.m_Cali.m_bCaliCoSel[i] )
		{
			pdC[nIndex]	= theApp.m_Cali.m_dCaliCoC[i];
			pdK[nIndex]	= theApp.m_Cali.m_dCaliCoK[i];
			nIndex++;
		}
	}

	LeastSquares2( theApp.m_Cali.m_nCaliCoCounts, pdC, pdK, m_dCo_Ka, m_dCo_Kb );
	str.Format( "直线方程为：K = %.6f   x OLctrl     %+.6f", m_dCo_Ka, m_dCo_Kb );
	GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Equation )->SetWindowText( str );

	delete [] pdC;
	delete [] pdK;
}

void CDlgCaliCoadjust::OnBUTTONATCaliCoadjust2() 
{
	// 重置所有值为0，清除所有选中标志
	theApp.m_Cali.m_nCaliCoCounts = 0;
	GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Counts )->SetWindowText( "0" );

	for( int i=0; i<10; i++ )
	{
		theApp.m_Cali.m_dCaliCoC[i] = 0.0;
		theApp.m_Cali.m_dCaliCoK[i] = 0.0;
		theApp.m_Cali.m_bCaliCoSel[i] = FALSE;

		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Ctrl1 +i )->SetWindowText( "0.0" );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_K1 +i )->SetWindowText( "0.0" );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Ctrl1 +i )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_K1 +i )->EnableWindow( FALSE );
		((CButton*)(GetDlgItem( IDC_CHECK_AT_Cali_Coadjust_1 + i)))->SetCheck( 0 );
	}

	m_dCo_Ka = 0.0;
	m_dCo_Kb = 0.0;
	GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Equation )->SetWindowText( "" );
}

void CDlgCaliCoadjust::OnBUTTONATCaliCoadjust3() 
{
	// 将计算出的截距和斜率写入ini
	double* pdValue = new double[2];
	pdValue[0] = m_dCo_Ka;
	pdValue[1] = m_dCo_Kb;
	theApp.WriteINI( INI_Coadjust, INIIndex_OL, pdValue, 0, 0 );
	delete [] pdValue;

	// 通知SemCore的OL生效
	theApp.m_SemCtrl.m_SemCore.m_ObjectiveLens.SetParams( 9, m_dCo_Ka );
	theApp.m_SemCtrl.m_SemCore.m_ObjectiveLens.SetParams( 10, m_dCo_Kb );
}

void CDlgCaliCoadjust::OnCheckChanged(UINT nID)
{
	int nIndex = nID - IDC_CHECK_AT_Cali_Coadjust_1;
	int nCheck = ((CButton*)(GetDlgItem(nID)))->GetCheck();
	if( nCheck )
	{
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Ctrl1 +nIndex )->EnableWindow( TRUE );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_K1 +nIndex )->EnableWindow( TRUE );
	}
	else
	{
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Ctrl1 +nIndex )->EnableWindow( FALSE );
		GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_K1 +nIndex )->EnableWindow( FALSE );
	}

	theApp.m_Cali.m_bCaliCoSel[nIndex] = (BOOL)nCheck;
	int nCounts = 0;
	for( int i=0; i<10; i++ )
	{
		if( theApp.m_Cali.m_bCaliCoSel[i] )
			nCounts++;
	}
	theApp.m_Cali.m_nCaliCoCounts = nCounts;
	CString str;
	str.Format( "%d", nCounts );
	GetDlgItem( IDC_EDIT_AT_Cali_Coadjust_Counts )->SetWindowText( str );
}
