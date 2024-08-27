// DlgDriverInit.cpp : implementation file
//

#include "stdafx.h"
#include "Options_StageMC6600Wnd.h"
#include "DlgDriverInit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern unsigned char SerialNum(short anxi);

/////////////////////////////////////////////////////////////////////////////
// CDlgDriverInit dialog


CDlgDriverInit::CDlgDriverInit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDriverInit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDriverInit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nAxial    = -1;
	m_nAddress  = -1;
}

CDlgDriverInit::~CDlgDriverInit()
{
}


void CDlgDriverInit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDriverInit)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDriverInit, CDialog)
	//{{AFX_MSG_MAP(CDlgDriverInit)
	ON_BN_CLICKED(IDC_BUTTON_AASeting, OnBnClickedButtonAaseting)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_P, OnBnClickedButtonP)
	ON_BN_CLICKED(IDC_BUTTON_I, OnBnClickedButtonI)
	ON_BN_CLICKED(IDC_BUTTON_D, OnBnClickedButtonD)
	ON_BN_CLICKED(IDC_BUTTON_Default, OnBnClickedButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_Slimit, OnBnClickedButtonSlimit)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDriverInit message handlers

void CDlgDriverInit::Localize()
{
	char szBuf[255];
	// 界面元素
	if(GetPrivateProfileString("Label", "CapDrive", "", szBuf, 50, m_cPathLocalize) != 0)
		SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "SlaveSetGroup", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_Slave)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "SlaveAxial", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_Axia)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "SlaveAddress", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_Address)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "DrvSetSlave", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_BUTTON_AASeting)->SetWindowText( szBuf );

	if(GetPrivateProfileString("Label", "SoftLimit", "", szBuf, 50, m_cPathLocalize) != 0)
	{
		GetDlgItem(IDC_STATIC_SoftLimit)->SetWindowText( szBuf );
		GetDlgItem(IDC_CHECK_SoftLimit)->SetWindowText( szBuf );
	}
	if(GetPrivateProfileString("Label", "DrvSetSoft", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_BUTTON_Slimit)->SetWindowText( szBuf );

	if(GetPrivateProfileString("Label", "SetWizard", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_SetWizard)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "MotorType", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_MotorType)->SetWindowText( szBuf );
		if(GetPrivateProfileString("Label", "MotorStatus", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_MotorStatus)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "EncoderLines", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_EncoderLines)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "ControlSignal", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_ControlSignal)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "ControlMode", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_ControlMode)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "MaxCurrent", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_MaxCurrent)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "DrvSetWizard", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDOK)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "PIDGroup", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_STATIC_PID)->SetWindowText( szBuf );
	if(GetPrivateProfileString("Label", "DrvDefaultPID", "", szBuf, 50, m_cPathLocalize) != 0)
		GetDlgItem(IDC_BUTTON_Default)->SetWindowText( szBuf );
}

BOOL CDlgDriverInit::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str;
	CComboBox* aAxial = ((CComboBox*)(GetDlgItem(IDC_COMBO_AxialSetting)));
	aAxial->AddString("X");
	aAxial->AddString("Y");
	aAxial->AddString("Z");
	aAxial->AddString("R");
	aAxial->AddString("T");
	aAxial->SetCurSel( 0 );

	int i, m;
	CComboBox* aAddress = ((CComboBox*)(GetDlgItem(IDC_COMBO_AddreSetting)));
	if( aAddress != NULL )
	{
		for( i=0; i<9; i++ )
		{
			str.Format( "0x0%d", i+1 );
			aAddress->AddString( str );
		}
		aAddress->SetCurSel( m_nAddress -1 );
	}
// 	aAddress->AddString("0x01");
// 	aAddress->AddString("0x02");
// 	aAddress->SetCurSel( 0 );

	char szBuf[255];
	//设置向导-电机类型
	for( i=0; i<5; i++ )
	{
		CComboBox* aMotorT = ((CComboBox*)(GetDlgItem(IDC_COMBO_XMotorT +i)));
		if( aMotorT != NULL )
		{
			if(GetPrivateProfileString("Label", "MotorType1", "", szBuf, 50, m_cPathLocalize) != 0)
				aMotorT->AddString(szBuf);
			if(GetPrivateProfileString("Label", "MotorType2", "", szBuf, 50, m_cPathLocalize) != 0)
				aMotorT->AddString(szBuf);
			aMotorT->SetCurSel( 1 );
		}
	}
	//设置向导-电机状态
	for( i=0; i<5; i++ )
	{
		CComboBox* aMotorS = ((CComboBox*)(GetDlgItem(IDC_COMBO_XMotorS +i)));
		if( aMotorS != NULL )
		{
			if(GetPrivateProfileString("Label", "MotorStatus1", "", szBuf, 50, m_cPathLocalize) != 0)
				aMotorS->AddString(szBuf);
			if(GetPrivateProfileString("Label", "MotorStatus2", "", szBuf, 50, m_cPathLocalize) != 0)
				aMotorS->AddString(szBuf);
			aMotorS->SetCurSel( 1 );
		}
	}
	//设置向导-编码器线数
	for( i=0; i<5; i++ )
	{
		CComboBox* eEncoderLines = ((CComboBox*)(GetDlgItem(IDC_COMBO_XEncoderL +i)));
		if( eEncoderLines != NULL )
		{
			for( int j=0; j<5; j++ )
			{
				m = 500*(j+1);
				str.Format( "%d", m );
				eEncoderLines->AddString( str );
			}
		}
	}
	for( i=0; i<5; i++ )
	{
		CComboBox* eEncoderLines = ((CComboBox*)(GetDlgItem(IDC_COMBO_XEncoderL +i)));
		if( eEncoderLines != NULL )
			eEncoderLines->SetCurSel(m_nEncoderLines[i]-1);
	}

	//设置向导-输入信号
	for( i=0; i<5; i++ )
	{
		CComboBox* aConSignal = ((CComboBox*)(GetDlgItem(IDC_COMBO_XControlS +i)));
		if( aConSignal != NULL )
		{
			if(GetPrivateProfileString("Label", "CtlSignal1", "", szBuf, 50, m_cPathLocalize) != 0)
				aConSignal->AddString(szBuf);
			if(GetPrivateProfileString("Label", "CtlSignal2", "", szBuf, 50, m_cPathLocalize) != 0)
				aConSignal->AddString(szBuf);
			if(GetPrivateProfileString("Label", "CtlSignal3", "", szBuf, 50, m_cPathLocalize) != 0)
				aConSignal->AddString(szBuf);
			aConSignal->SetCurSel( 0 );
		}
	}

	//设置向导-控制方式
	for( i=0; i<5; i++ )
	{
		CComboBox* aConMode = ((CComboBox*)(GetDlgItem(IDC_COMBO_XControlM +i)));
		if( aConMode != NULL )
		{
			if(GetPrivateProfileString("Label", "CtlMode1", "", szBuf, 50, m_cPathLocalize) != 0)
				aConMode->AddString(szBuf);
			if(GetPrivateProfileString("Label", "CtlMode2", "", szBuf, 50, m_cPathLocalize) != 0)
				aConMode->AddString(szBuf);
			if(GetPrivateProfileString("Label", "CtlMode3", "", szBuf, 50, m_cPathLocalize) != 0)
				aConMode->AddString(szBuf);
			if(GetPrivateProfileString("Label", "CtlMode4", "", szBuf, 50, m_cPathLocalize) != 0)
				aConMode->AddString(szBuf);
			aConMode->SetCurSel( 3 );
		}
	}

	//设置向导-最大电流
	for( i=0; i<5; i++ )
	{
		CComboBox* mMaxCurrent = ((CComboBox*)(GetDlgItem(IDC_COMBO_XMaxCur +i)));
		if( mMaxCurrent != NULL )
		{
			for( m=0; m<10; m++ )
			{
				str.Format( "%d", m+1 );
				mMaxCurrent->AddString( str );
			}
			mMaxCurrent->SetCurSel( m_nMaxCurrent[i]-1  );
		}
	}

	Localize();

	if( m_bSoftLimit )
		((CButton*)(GetDlgItem(IDC_CHECK_SoftLimit)))->SetCheck(1);

	//SetTimer( 1, 200, NULL );
	//显示PID当前值
	for(i=0;i<15;i++)
	{
		if(i<5)
			str.Format( "%.3f", m_fCurP[i] );
		else if(i<10)
			str.Format( "%.3f", m_fCurI[i-5] );
		else
			str.Format( "%.3f", m_fCurD[i-10] );
		GetDlgItem(IDC_EDIT_PX+i)->SetWindowText(str);
	}
	return TRUE; 
	
}

void CDlgDriverInit::OnBnClickedButtonAaseting()
{
	// 地址值设置,目前只支持单次设置
    CComboBox* aAddress = ((CComboBox*)(GetDlgItem(IDC_COMBO_AddreSetting)));
	if( aAddress != NULL )
		m_nAddress = aAddress->GetCurSel() +1;
	BYTE  Addr[8];
	for(int i=0;i<1;i++)
	{
		Addr[0]= 0xFF;Addr[1]= 0x06;
		Addr[2]= 0x00;Addr[3]= 0x14;
		Addr[4]= 0x00;Addr[5]= m_nAddress;
		// 计算CRC校验位
		unsigned short tmp = 0xffff;
		unsigned short ret1 = 0;
		for(int n = 0; n < 6; n++)
		{
			// 此处的6 -- 要校验的位数为6个
			tmp = Addr[n] ^ tmp;
			for(int i = 0;i < 8;i++)
			{  
				// 此处的8 -- 指每一个char类型又8bit，每bit都要处理
				if(tmp & 0x01)
				{
					tmp = tmp >> 1;
					tmp = tmp ^ 0xa001;
				}
				else
				{
					tmp = tmp >> 1;
				}
			}
		}
		// CRC校验后的值
//		printf("%X\n",tmp);
		// 将CRC校验的高低位对换位置
		ret1 = tmp >> 8;
		ret1 = ret1 | (tmp << 8); 
		Addr[6] = tmp;
		Addr[7] = ret1;

		theApp.m_StageMC6600.SlaveAddrSet(Addr, sizeof(Addr));
		Sleep(100);
//		int nlenRecvRH = g_MC6600.ReadData((BYTE*)Recvdata, 200);
	}
}

void CDlgDriverInit::SetMotorType(unsigned char ucAxis,BYTE M)
{
	theApp.m_StageMC6600.SetWizard(ucAxis,M);
}

void CDlgDriverInit::SetEncoderLines(unsigned char ucAxis,int M)
{
	theApp.m_StageMC6600.SetEncoderLines(ucAxis,M);
}

void CDlgDriverInit::SetControlSignal(unsigned char ucAxis,BYTE M)
{
	theApp.m_StageMC6600.SetControlSignal(ucAxis,M);
}

void CDlgDriverInit::SetControlMode(  unsigned char ucAxis ,BYTE M )
{
	theApp.m_StageMC6600.SetControlMode(ucAxis,M);
}

void CDlgDriverInit::SetMaxCurrent(  unsigned char ucAxis ,int M )
{
	theApp.m_StageMC6600.SetMaxCurrent(ucAxis,M);
}

void CDlgDriverInit::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CComboBox* aAxial = ((CComboBox*)(GetDlgItem(IDC_COMBO_AxialSetting)));
	if( aAxial != NULL )
		m_nAxial = (char)aAxial->GetCurSel();

	CComboBox* aAddress = ((CComboBox*)(GetDlgItem(IDC_COMBO_AddreSetting)));
	if( aAddress != NULL )
		m_nAddress = aAddress->GetCurSel() +1;

	int i, nM;
	BYTE byteM;
	unsigned char ucAxis;
	//设置向导-电机类型
	for( i=0; i<5; i++ )
	{
		CComboBox* aMotorT = ((CComboBox*)(GetDlgItem(IDC_COMBO_XMotorT +i)));
		if( aMotorT != NULL )
		{
			ucAxis = SerialNum(i);
			m_nSetMotorType[i] = aMotorT->GetCurSel() +1;
			if(m_nSetMotorType[i] == 1)
				byteM = 0x01;
			else
				byteM = 0x04;
			SetMotorType(ucAxis, byteM);
		}
	}
	//设置向导-记录电机状态
	int Motor = m_nAxial;
	CComboBox* aMotorS = ((CComboBox*)(GetDlgItem(IDC_COMBO_XMotorS +Motor)));
	if( aMotorS != NULL )
		m_bRecordMotorStatus[Motor] = aMotorS->GetCurSel();
			
	//设置向导-编码器线数
	for( i=0; i<5; i++ )
	{
		CComboBox* aEncoderL = ((CComboBox*)(GetDlgItem(IDC_COMBO_XEncoderL +i)));
		if( aEncoderL != NULL )
		{
			ucAxis = SerialNum(i);
			m_nEncoderLines[i] = aEncoderL->GetCurSel() +1; 
			nM = m_nEncoderLines[i] *500;
			SetEncoderLines(ucAxis, nM);
		}
	}
	//设置向导-输入信号
	for( i=0; i<5; i++ )
	{
		CComboBox* aConS = ((CComboBox*)(GetDlgItem(IDC_COMBO_XControlS +i)));
		if( aConS != NULL )
		{
			ucAxis = SerialNum(i);
			m_nControlSignal[i] = aConS->GetCurSel() +1;
			byteM = (BYTE)( m_nControlSignal[i] -1 );
			SetControlSignal(ucAxis, byteM);
		}
	}
	//设置向导-控制方式
	for( i=0; i<5; i++ )
	{
		CComboBox* aConM = ((CComboBox*)(GetDlgItem(IDC_COMBO_XControlM +i)));
		if( aConM != NULL )
		{
			ucAxis = SerialNum(i);
			m_nControlMode[i] = aConM->GetCurSel() +1;
			switch( m_nControlMode[i] )
			{
			case	1:
				byteM = 0x00;
				break;
			case	2:
				byteM = 0x01;
				break;
			case	3:
				byteM = 0x03;
				break;
			default:
				byteM = 0x04;
				break;
			}
			SetControlMode(ucAxis, byteM);
		}
	}
	//设置向导-最大电流
	for( i=0; i<5; i++ )
	{
		CComboBox* mMaxC = ((CComboBox*)(GetDlgItem(IDC_COMBO_XMaxCur +i)));
		if( mMaxC != NULL )
		{
			ucAxis = SerialNum(i);
			m_nMaxCurrent[i] = mMaxC->GetCurSel() +1; 
			nM = m_nMaxCurrent[i]*100;
			SetMaxCurrent(ucAxis, nM);
		}
	}
//	m_bSoftLimit = ((CButton*)(GetDlgItem(IDC_CHECK_SoftLimit)))->GetCheck();
	CDialog::OnOK();
}

BOOL CDlgDriverInit::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		CString str = "";
		int nID = GetFocus()->GetDlgCtrlID();
		switch( nID )
		{
		case    IDC_EDIT_PX:
		case    IDC_EDIT_PY:
		case    IDC_EDIT_PZ:
		case    IDC_EDIT_PR:
		case    IDC_EDIT_PT:
            ModifyP(nID -IDC_EDIT_PX);
			break;
        case    IDC_EDIT_IX:
		case    IDC_EDIT_IY:
		case    IDC_EDIT_IZ:
		case    IDC_EDIT_IR:
		case    IDC_EDIT_IT:
            ModifyI(nID -IDC_EDIT_IX);
			break;
		case    IDC_EDIT_DX:
		case    IDC_EDIT_DY:
		case    IDC_EDIT_DZ:
		case    IDC_EDIT_DR:
		case    IDC_EDIT_DT:
            ModifyD(nID -IDC_EDIT_DX);
			break;
		default:
			break;
		}
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgDriverInit::OnBnClickedButtonP()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	float value = 0.0;
	for(int i=0;i<5;i++)
	{
		value = m_fCurP[i];	
		str.Format( "%.4f", value );
		GetDlgItem(IDC_EDIT_PX +i)->SetWindowText(str);
	}
}

void CDlgDriverInit::OnBnClickedButtonI()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	float value;
	for(int i=0;i<5;i++)
	{
		value = m_fCurI[i];
		str.Format( "%.4f", value );
		GetDlgItem(IDC_EDIT_IX +i)->SetWindowText(str);
	}
}

void CDlgDriverInit::OnBnClickedButtonD()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	float value;
	for(int i=0;i<5;i++)
	{
		value = m_fCurD[i];
		str.Format( "%.4f", value );
		GetDlgItem(IDC_EDIT_DX +i)->SetWindowText(str);
	}
}

void CDlgDriverInit::ModifyP(short anxi)
{
	if( m_Style.bOld && (anxi==m_Style.nAxis) )
		return;
	CString str;
	GetDlgItem(IDC_EDIT_PX +anxi)->GetWindowText(str);	// mm
	float fValue = (float)atof(str);
	unsigned char ucAxis = SerialNum( anxi );
	theApp.m_StageMC6600.PIDModify(1, ucAxis, fValue);
	m_fCurP[anxi] = fValue;
}

void CDlgDriverInit::ModifyI(short anxi)
{
	CString str;
	GetDlgItem(IDC_EDIT_IX +anxi)->GetWindowText(str);	// mm
	float fValue = (float)atof(str);
	unsigned char ucAxis = SerialNum( anxi );
	if( m_Style.bOld && (anxi==m_Style.nAxis) )
		theApp.m_StageMC6600.ItoModify(m_Style.ucAxis, fValue*1000);
	else
		theApp.m_StageMC6600.PIDModify(2, ucAxis, fValue);
	m_fCurI[anxi] = fValue;
}

void CDlgDriverInit::ModifyD(short anxi)
{
	CString str;
	GetDlgItem(IDC_EDIT_DX +anxi)->GetWindowText(str);	// mm
	float fValue = (float)atof(str);
	unsigned char ucAxis = SerialNum( anxi );
	if( m_Style.bOld && (anxi==m_Style.nAxis) )
		theApp.m_StageMC6600.DtoModify(m_Style.ucAxis, fValue*1000);
	else
		theApp.m_StageMC6600.PIDModify(3, ucAxis, fValue);
	m_fCurD[anxi] = fValue;
}

void CDlgDriverInit::SetDefaultPID(int ID,short anxi)
{
	CString str;
	unsigned char ucAxis = SerialNum( anxi );
	if(ID == 0)
	{
		if( m_Style.bOld && (anxi==m_Style.nAxis) )
			return;
		theApp.m_StageMC6600.PIDModify(1, ucAxis, m_fDefaultP[anxi]);
		str.Format( "%-04.4f", m_fDefaultP[anxi] );
		GetDlgItem(IDC_EDIT_PX +anxi)->SetWindowText(str);
		m_fCurP[anxi] = m_fDefaultP[anxi] ;
	}
	else if(ID == 1)
	{
		if( m_Style.bOld && (anxi==m_Style.nAxis) )
			theApp.m_StageMC6600.ItoModify(m_Style.ucAxis, m_fDefaultI[anxi]*1000);
		else
			theApp.m_StageMC6600.PIDModify(2, ucAxis, m_fDefaultI[anxi]);
		str.Format( "%-04.4f", m_fDefaultI[anxi] );
		GetDlgItem(IDC_EDIT_IX +anxi)->SetWindowText(str);
		m_fCurI[anxi] = m_fDefaultI[anxi] ;
	}
	else
	{
		if( m_Style.bOld && (anxi==m_Style.nAxis) )
			theApp.m_StageMC6600.DtoModify(m_Style.ucAxis, m_fDefaultD[anxi]*1000);
		else
			theApp.m_StageMC6600.PIDModify(3, ucAxis, m_fDefaultD[anxi]);
		str.Format( "%-04.4f", m_fDefaultD[anxi] );
		GetDlgItem(IDC_EDIT_DX +anxi)->SetWindowText(str);
		m_fCurD[anxi] = m_fDefaultD[anxi] ;
	}
}

void CDlgDriverInit::OnBnClickedButtonDefault()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<5;i++)
	{
		SetDefaultPID(0,i);
		SetDefaultPID(1,i);
		SetDefaultPID(2,i);
	}
}

void CDlgDriverInit::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialog::OnCancel();
}

void CDlgDriverInit::OnBnClickedButtonSlimit()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bSoftLimit = ((CButton*)(GetDlgItem(IDC_CHECK_SoftLimit)))->GetCheck();
	EndDialog(IDC_BUTTON_Slimit);
}

void CDlgDriverInit::OnTimer(UINT nIDEvent) 
{
	//读取当前位置PID：需时较长，在界面显示出来后再调用
	KillTimer(1);
	OnBnClickedButtonP();
	OnBnClickedButtonI();
	OnBnClickedButtonD();
	CDialog::OnTimer(nIDEvent);
}
