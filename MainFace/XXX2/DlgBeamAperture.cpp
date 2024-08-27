// DlgBeamAperture.cpp : 实现文件
//

#include "stdafx.h"
#include "XXX2.h"
#include "DlgBeamAperture.h"
#include "afxdialogex.h"


// CDlgBeamAperture 对话框
IMPLEMENT_DYNAMIC(CDlgBeamAperture, CDialog)

CDlgBeamAperture::CDlgBeamAperture(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBeamAperture::IDD, pParent)
	, m_nSelectAper(0)
{
	m_nBeamAper = 30;
}

CDlgBeamAperture::~CDlgBeamAperture()
{
}

void CDlgBeamAperture::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Beam_AperSel, m_nSelectAper);
}


BEGIN_MESSAGE_MAP(CDlgBeamAperture, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgBeamAperture::OnBnClickedOk)
END_MESSAGE_MAP()

void CDlgBeamAperture::Localize(void)
{
	GetDlgItem( IDC_STATIC_Beam_AperSel )->SetWindowText(GetResString(IDC_STATIC_Beam_AperSel));
	GetDlgItem( IDC_STATIC_Beam_AperReal )->SetWindowText(GetResString(IDC_STATIC_Beam_AperReal));
	GetDlgItem(IDOK)->SetWindowText(GetResString(IDS_OK));
	SetWindowText(GetResString(IDC_STATIC_Beam_Aper));
}

// CDlgBeamAperture 消息处理程序
BOOL CDlgBeamAperture::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString str;
	int nValue = 30;
	int num=0;
	CComboBox* aAperture = ((CComboBox*)(GetDlgItem(IDC_COMBO_Beam_AperReal)));
	for(int i=0;i<5;i++)
	{
		nValue = 30 + 20*i;
		if(i >2)
			nValue = 100 * (i-2);
		str.Format( "%d", nValue );
		aAperture->AddString( str );
		m_nBeamAperNum[i] = nValue;
		if(nValue == m_nBeamAper)
			num = i;
	}
	aAperture->SetCurSel( num );

	m_nSelectAper = m_nBeamAper;
	UpdateData(FALSE);
	Localize();
	return TRUE;
}

void CDlgBeamAperture::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CComboBox* pComboAper = ((CComboBox*)(GetDlgItem(IDC_COMBO_Beam_AperReal)));
	if( pComboAper != NULL )
	{
		int nValue = 0;
		nValue = pComboAper->GetCurSel();
		m_nBeamAper = m_nBeamAperNum[nValue];
	}
	CDialog::OnOK();
}
