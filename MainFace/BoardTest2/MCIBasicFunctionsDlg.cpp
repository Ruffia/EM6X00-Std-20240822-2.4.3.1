// MCIBasicFunctionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "insample0.h"
#include "MCIBasicFunctionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMCIBasicFunctionsDlg dialog


CMCIBasicFunctionsDlg::CMCIBasicFunctionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMCIBasicFunctionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMCIBasicFunctionsDlg)
	m_strPortA = _T("");
	m_strPortB = _T("");
	m_strPortC = _T("");
	m_strPortD = _T("");
	m_nPortA = 1;
	m_nPortB = 0;
	m_nPortC = 1;
	m_nPortD = 0;
	//}}AFX_DATA_INIT
}


void CMCIBasicFunctionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMCIBasicFunctionsDlg)
	DDX_Control(pDX, IDC_BUTTON_ConfigEx, m_btnConfig);
	DDX_Control(pDX, IDC_UNABLE, m_staticUnable);
	DDX_Control(pDX, IDC_RED, m_staticRed);
	DDX_Control(pDX, IDC_GREEN, m_staticGreen);
	DDX_Control(pDX, IDC_STATIC31, m_staticIObitD7);
	DDX_Control(pDX, IDC_STATIC30, m_staticIObitD6);
	DDX_Control(pDX, IDC_STATIC29, m_staticIObitD5);
	DDX_Control(pDX, IDC_STATIC28, m_staticIObitD4);
	DDX_Control(pDX, IDC_STATIC27, m_staticIObitD3);
	DDX_Control(pDX, IDC_STATIC26, m_staticIObitD2);
	DDX_Control(pDX, IDC_STATIC25, m_staticIObitD1);
	DDX_Control(pDX, IDC_STATIC24, m_staticIObitD0);
	DDX_Control(pDX, IDC_STATIC23, m_staticIObitC7);
	DDX_Control(pDX, IDC_STATIC22, m_staticIObitC6);
	DDX_Control(pDX, IDC_STATIC21, m_staticIObitC5);
	DDX_Control(pDX, IDC_STATIC20, m_staticIObitC4);
	DDX_Control(pDX, IDC_STATIC19, m_staticIObitC3);
	DDX_Control(pDX, IDC_STATIC18, m_staticIObitC2);
	DDX_Control(pDX, IDC_STATIC17, m_staticIObitC1);
	DDX_Control(pDX, IDC_STATIC16, m_staticIObitC0);
	DDX_Control(pDX, IDC_STATIC15, m_staticIObitB7);
	DDX_Control(pDX, IDC_STATIC14, m_staticIObitB6);
	DDX_Control(pDX, IDC_STATIC13, m_staticIObitB5);
	DDX_Control(pDX, IDC_STATIC12, m_staticIObitB4);
	DDX_Control(pDX, IDC_STATIC11, m_staticIObitB3);
	DDX_Control(pDX, IDC_STATIC10, m_staticIObitB2);
	DDX_Control(pDX, IDC_STATIC9, m_staticIObitB1);
	DDX_Control(pDX, IDC_STATIC8, m_staticIObitB0);
	DDX_Control(pDX, IDC_STATIC7, m_staticIObitA7);
	DDX_Control(pDX, IDC_STATIC6, m_staticIObitA6);
	DDX_Control(pDX, IDC_STATIC5, m_staticIObitA5);
	DDX_Control(pDX, IDC_STATIC4, m_staticIObitA4);
	DDX_Control(pDX, IDC_STATIC3, m_staticIObitA3);
	DDX_Control(pDX, IDC_STATIC2, m_staticIObitA2);
	DDX_Control(pDX, IDC_STATIC1, m_staticIObitA1);
	DDX_Control(pDX, IDC_STATIC0, m_staticIObitA0);
	DDX_Radio(pDX, IDC_RADIO_PortA_Input, m_nPortA);
	DDX_Radio(pDX, IDC_RADIO_PortB_Input, m_nPortB);
	DDX_Radio(pDX, IDC_RADIO_PortC_Input, m_nPortC);
	DDX_Radio(pDX, IDC_RADIO_PortD_Input, m_nPortD);
	DDX_Text(pDX, IDC_EDIT_PortA, m_strPortA);
	DDV_MaxChars(pDX, m_strPortA, 2);
	DDX_Text(pDX, IDC_EDIT_PortB, m_strPortB);
	DDV_MaxChars(pDX, m_strPortB, 2);
	DDX_Text(pDX, IDC_EDIT_PortC, m_strPortC);
	DDV_MaxChars(pDX, m_strPortC, 2);
	DDX_Text(pDX, IDC_EDIT_PortD, m_strPortD);
	DDV_MaxChars(pDX, m_strPortD, 2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMCIBasicFunctionsDlg, CDialog)
	//{{AFX_MSG_MAP(CMCIBasicFunctionsDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON3, OnConfig3)
	ON_BN_CLICKED(IDC_BUTTON4, OnConfig4)
	ON_BN_CLICKED(IDC_BUTTON5, OnConfig5)
	ON_BN_CLICKED(IDC_BUTTON6, OnConfig6)
	ON_BN_CLICKED(IDC_BUTTON7, OnConfig7)
	ON_BN_CLICKED(IDC_BUTTON_Read_PortA, OnReadPortA)
	ON_BN_CLICKED(IDC_BUTTON_Write_PortA, OnWritePortA)
	ON_BN_CLICKED(IDC_BUTTON_Read_PortB, OnReadPortB)
	ON_BN_CLICKED(IDC_BUTTON_Write_PortB, OnWritePortB)
	ON_BN_CLICKED(IDC_BUTTON_Read_PortC, OnReadPortC)
	ON_BN_CLICKED(IDC_BUTTON_Write_PortC, OnWritePortC)
	ON_BN_CLICKED(IDC_BUTTON_Read_PortD, OnReadPortD)
	ON_BN_CLICKED(IDC_BUTTON_Write_PortD, OnWritePortD)
	ON_BN_CLICKED(IDC_BUTTON_ReadAll, OnReadAll)
	ON_BN_CLICKED(IDC_BUTTON_WriteAll, OnWriteAll)
	ON_BN_CLICKED(IDC_BUTTON_ConfigEx, OnConfigEx)
	ON_BN_CLICKED(IDC_RADIO_PortC_Input, OnRADIOPortCInput)
	ON_BN_CLICKED(IDC_RADIO_PortC_Output, OnRADIOPortCOutput)
	ON_BN_CLICKED(IDC_RADIO_PortD_Input, OnRADIOPortDInput)
	ON_BN_CLICKED(IDC_RADIO_PortD_Output, OnRADIOPortDOutput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMCIBasicFunctionsDlg message handlers

BOOL CMCIBasicFunctionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	///////////////////////////////////////////////////////////////////////////

	staticIObitsArray[0] = &m_staticIObitA0;
	staticIObitsArray[1] = &m_staticIObitA1;
	staticIObitsArray[2] = &m_staticIObitA2;
	staticIObitsArray[3] = &m_staticIObitA3;
	staticIObitsArray[4] = &m_staticIObitA4;
	staticIObitsArray[5] = &m_staticIObitA5;
	staticIObitsArray[6] = &m_staticIObitA6;
	staticIObitsArray[7] = &m_staticIObitA7;

	staticIObitsArray[8] = &m_staticIObitB0;
	staticIObitsArray[9] = &m_staticIObitB1;
	staticIObitsArray[10] = &m_staticIObitB2;
	staticIObitsArray[11] = &m_staticIObitB3;
	staticIObitsArray[12] = &m_staticIObitB4;
	staticIObitsArray[13] = &m_staticIObitB5;
	staticIObitsArray[14] = &m_staticIObitB6;
	staticIObitsArray[15] = &m_staticIObitB7;

	staticIObitsArray[16] = &m_staticIObitC0;
	staticIObitsArray[17] = &m_staticIObitC1;
	staticIObitsArray[18] = &m_staticIObitC2;
	staticIObitsArray[19] = &m_staticIObitC3;
	staticIObitsArray[20] = &m_staticIObitC4;
	staticIObitsArray[21] = &m_staticIObitC5;
	staticIObitsArray[22] = &m_staticIObitC6;
	staticIObitsArray[23] = &m_staticIObitC7;

	staticIObitsArray[24] = &m_staticIObitD0;
	staticIObitsArray[25] = &m_staticIObitD1;
	staticIObitsArray[26] = &m_staticIObitD2;
	staticIObitsArray[27] = &m_staticIObitD3;
	staticIObitsArray[28] = &m_staticIObitD4;
	staticIObitsArray[29] = &m_staticIObitD5;
	staticIObitsArray[30] = &m_staticIObitD6;
	staticIObitsArray[31] = &m_staticIObitD7;

	for( int i=0; i<32; i++ )
		bIObitsStateArray[i] = FALSE;
	///////////////////////////////////////////////////////////////////////////

	m_btnConfig.SetColor( g_crSYS );
	UpdateConfig();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMCIBasicFunctionsDlg::OnPaint() 
{
	CDC *pDC; //this is going to point to each of the "LEDS" contexts so they can be filled
	RECT rect; //this is going to hold the dimensions of each of the "LEDS"
	CBrush offBrush(RGB(0x00, 0xff, 0x00)); //the on brush is green
	CBrush onBrush(RGB(0xff, 0x00, 0x00)); //the off brush is red
	CBrush triBrush(RGB(0x7f, 0x7f, 0x7f)); // the tri brush is gray

	pDC = m_staticGreen.GetDC();
	m_staticGreen.GetClientRect(&rect);

	pDC->FillRect(&rect, &onBrush);

	pDC = m_staticRed.GetDC();
	m_staticRed.GetClientRect(&rect);

	pDC->FillRect(&rect, &offBrush);

	pDC = m_staticUnable.GetDC();
	m_staticUnable.GetClientRect(&rect);

	pDC->FillRect(&rect, &triBrush);

	for (int i = 0; i < 32; i++) //cycle through the inputs and outputs and set them to the right color
	{

		pDC = staticIObitsArray[i]->GetDC();
		staticIObitsArray[i]->GetClientRect(&rect);

		if( bIObitsStateArray[i] )
		{
			if ((IObitsMask >> i) % 2)
				pDC->FillRect(&rect, &onBrush);
			else
				pDC->FillRect(&rect, &offBrush);
		}
		else
			pDC->FillRect(&rect, &triBrush);
	}

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

void CMCIBasicFunctionsDlg::Output_AfterConfig( unsigned char OutMask )
{
	UpdatePortState( OutMask );
	UpdateListString_OutMask_Config( OutMask );
	UpdateListString_ReadEveryBit();
	UpdateListString_ReadEveryPort();
}

void CMCIBasicFunctionsDlg::Output_AfterWrite()
{
}

void CMCIBasicFunctionsDlg::UpdateConfig()
{
	UpdateData( TRUE );

	unsigned char OutMask = 0x00;
	OutMask |= m_nPortA;
	OutMask |= m_nPortB << 1;
	OutMask |= m_nPortC << 2;
	OutMask |= m_nPortD << 3;

	unsigned long DIOData = 0xFFFFFFFF;
	unsigned char TristateMask = 0x00;
	DIO_ConfigureEx(theApp.deviceIndex, &OutMask, &DIOData, &TristateMask);
	Output_AfterConfig( OutMask );
}

void CMCIBasicFunctionsDlg::UpdatePortState( unsigned char OutMask )
{
	BOOL bPortState = FALSE;

	///////////////////////////////////////////////////////////////////////////
	bPortState = ((OutMask & 0x01) ? 1 : 0);
	m_nPortA = bPortState;
	int j;
	for( j=0; j<8; j++ )
		bIObitsStateArray[j] = bPortState;

	bPortState = ((OutMask & 0x02) ? 1 : 0);
	m_nPortB = bPortState;
	for( j=8; j<16; j++ )
		bIObitsStateArray[j] = bPortState;

	bPortState = ((OutMask & 0x04) ? 1 : 0);
	m_nPortC = bPortState;
	for( j=16; j<24; j++ )
		bIObitsStateArray[j] = bPortState;

	bPortState = ((OutMask & 0x08) ? 1 : 0);
	m_nPortD = bPortState;
	for( j=24; j<32; j++ )
		bIObitsStateArray[j] = bPortState;

	UpdateData( FALSE );
}

void CMCIBasicFunctionsDlg::UpdateListString_OutMask_Config( unsigned char OutMask )
{
	BOOL bPortState = FALSE;
	CString strOutMask;
	strOutMask.Format( "Config OutMask = 0x%02X, means ", OutMask );

	///////////////////////////////////////////////////////////////////////////
	bPortState = ((OutMask & 0x01) ? 1 : 0);
	strOutMask += ( bPortState ? "A output;  " : "A input;  " );

	bPortState = ((OutMask & 0x02) ? 1 : 0);
	strOutMask += ( bPortState ? "B output;  " : "B input;  " );

	bPortState = ((OutMask & 0x04) ? 1 : 0);
	strOutMask += ( bPortState ? "C output;  " : "C input;  " );

	bPortState = ((OutMask & 0x08) ? 1 : 0);
	strOutMask += ( bPortState ? "D output;  " : "D input;  " );

//	m_listReport.AddString( strOutMask );
//	m_listReport.AddString( "" );
}

void CMCIBasicFunctionsDlg::UpdateButtonsState( BOOL bEnable )
{
//	(GetDlgItem( IDC_BUTTON1 ) )->EnableWindow( bEnable );
//	(GetDlgItem( IDC_BUTTON2 ) )->EnableWindow( bEnable );
	(GetDlgItem( IDC_BUTTON3 ) )->EnableWindow( bEnable );
	(GetDlgItem( IDC_BUTTON4 ) )->EnableWindow( bEnable );
	(GetDlgItem( IDC_BUTTON5 ) )->EnableWindow( bEnable );
	(GetDlgItem( IDC_BUTTON6 ) )->EnableWindow( bEnable );
	(GetDlgItem( IDC_BUTTON7 ) )->EnableWindow( bEnable );
}

void CMCIBasicFunctionsDlg::UpdateListString_OutMask_Write( unsigned long OutMask )
{
	CString strOutMask;
	strOutMask.Format( "Write OutMask = 0x%08X", OutMask );

//	m_listReport.AddString( strOutMask );
//	m_listReport.AddString( "" );
}

void CMCIBasicFunctionsDlg::UpdateListString_ReadEveryBit()
{
	CString str, str1, str2;
	unsigned char input;

	str1 = "";
	str2 = "";
	IObitsMask = 0x00000000;
	for( int i=31; i>=0; i-- )
	{
/*		str.Format( "%02d--", i );
		str1 += str;
		DIO_Read1(theApp.deviceIndex, i, &input);
		IObitsMask &= input << i;
		str.Format( "%1X---", input );
		str2 += str;
		if( i % 8 == 0 )
		{
			str1 += "***";
			str2 += "***";
		}
*/
		DIO_Read1(theApp.deviceIndex, i, &input);
		IObitsMask &= input << i;
		str.Format( "%1X", input );
		str2 += str;
		if( i % 8 == 0 )
			str2 += "; ";
		else if( i % 4 == 0 )
			str2 += " ";
		else
			str2 += "";
	}
//	m_listReport.AddString( str2 );
//	m_listReport.AddString( "" );
	Invalidate(0);
}

void CMCIBasicFunctionsDlg::UpdateListString_ReadEveryPort()
{
	CString str, str1, str2;
	unsigned char input;

	str1 = "";
	DIO_Read8(theApp.deviceIndex, 3, &input);
	m_strPortD.Format( "%02X", input );
	str.Format( "D: 0x%02X;   ", input );
	str1 += str;
	IObitsMask &= 0x00FFFFFF;
	IObitsMask |= input << 24;

	DIO_Read8(theApp.deviceIndex, 2, &input);
	m_strPortC.Format( "%02X", input );
	str.Format( "C: 0x%02X;   ", input );
	str1 += str;
	IObitsMask &= 0xFF00FFFF;
	IObitsMask |= input << 16;

	DIO_Read8(theApp.deviceIndex, 1, &input);
	m_strPortB.Format( "%02X", input );
	str.Format( "B: 0x%02X;   ", input );
	str1 += str;
	IObitsMask &= 0xFFFF00FF;
	IObitsMask |= input << 8;

	DIO_Read8(theApp.deviceIndex, 0, &input);
	m_strPortA.Format( "%02X", input );
	str.Format( "A: 0x%02X;   ", input );
	str1 += str;
	IObitsMask &= 0xFFFFFF00;
	IObitsMask |= input;

//	m_listReport.AddString( str1 );
//	m_listReport.AddString( "" );

	UpdateData( FALSE );
	Invalidate(0);
}

void CMCIBasicFunctionsDlg::OnReadPortA() 
{
	// TODO: Add your control notification handler code here
	unsigned char input;

	DIO_Read8(theApp.deviceIndex, 0, &input);

	m_strPortA.Format( "%02X", input );
	UpdateData( FALSE );

	IObitsMask &= 0xFFFFFF00;
	IObitsMask |= input;

	Invalidate(0);
}

void CMCIBasicFunctionsDlg::OnWritePortA() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	CString str;
	if( m_nPortA == 0 )
	{
//		m_listReport.AddString( "Port A is input, can NOT write data!" );
//		m_listReport.AddString( "" );
		return;
	}

	str = m_strPortA;
	unsigned char output;
	char *buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output);
	DIO_Write8(theApp.deviceIndex, 0, output);

	IObitsMask &= 0xFFFFFF00;
	IObitsMask |= output;
	Invalidate(0);

	str.Format( "outMask = 0x%08X", IObitsMask );
//	m_listReport.AddString( str );
//	m_listReport.AddString( "" );
}

void CMCIBasicFunctionsDlg::OnReadPortB() 
{
	// TODO: Add your control notification handler code here
	unsigned char input;

	DIO_Read8(theApp.deviceIndex, 1, &input);

	m_strPortB.Format( "%02X", input );
	UpdateData( FALSE );

	IObitsMask &= 0xFFFF00FF;
	IObitsMask |= input << 8;

	Invalidate(0);
}

void CMCIBasicFunctionsDlg::OnWritePortB() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	CString str;
	if( m_nPortB == 0 )
	{
//		m_listReport.AddString( "Port B is input, can NOT write data!" );
//		m_listReport.AddString( "" );
		return;
	}

	str = m_strPortB;
	unsigned char output;
	char *buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output);
	DIO_Write8(theApp.deviceIndex, 1, output);

	IObitsMask &= 0xFFFF00FF;
	IObitsMask |= output << 8;
	Invalidate(0);

	str.Format( "outMask = 0x%08X", IObitsMask );
//	m_listReport.AddString( str );
//	m_listReport.AddString( "" );
}

void CMCIBasicFunctionsDlg::OnReadPortC() 
{
	// TODO: Add your control notification handler code here
	unsigned char input;

	DIO_Read8(theApp.deviceIndex, 2, &input);

	m_strPortC.Format( "%02X", input );
	UpdateData( FALSE );

	IObitsMask &= 0xFF00FFFF;
	IObitsMask |= input << 16;

	Invalidate(0);
}

void CMCIBasicFunctionsDlg::OnWritePortC() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	CString str;
	if( m_nPortC == 0 )
	{
//		m_listReport.AddString( "Port C is input, can NOT write data!" );
//		m_listReport.AddString( "" );
		return;
	}

	str = m_strPortC;
	unsigned char output;
	char *buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output);
	DIO_Write8(theApp.deviceIndex, 2, output);

	IObitsMask &= 0xFF00FFFF;
	IObitsMask |= output << 16;
	Invalidate(0);

	str.Format( "outMask = 0x%08X", IObitsMask );
//	m_listReport.AddString( str );
//	m_listReport.AddString( "" );
}

void CMCIBasicFunctionsDlg::OnReadPortD() 
{
	// TODO: Add your control notification handler code here
	unsigned char input;

	DIO_Read8(theApp.deviceIndex, 3, &input);

	m_strPortD.Format( "%02X", input );
	UpdateData( FALSE );

	IObitsMask &= 0x00FFFFFF;
	IObitsMask |= input << 24;

	Invalidate(0);
}

void CMCIBasicFunctionsDlg::OnWritePortD() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	CString str;
	if( m_nPortD == 0 )
	{
//		m_listReport.AddString( "Port D is input, can NOT write data!" );
//		m_listReport.AddString( "" );
		return;
	}

	str = m_strPortD;
	unsigned char output;
	char *buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output);
	DIO_Write8(theApp.deviceIndex, 3, output);

	IObitsMask &= 0x00FFFFFF;
	IObitsMask |= output << 24;
	Invalidate(0);

	str.Format( "outMask = 0x%08X", IObitsMask );
//	m_listReport.AddString( str );
//	m_listReport.AddString( "" );
}

void CMCIBasicFunctionsDlg::OnReadAll() 
{
	// TODO: Add your control notification handler code here
	ReadAll();
}

void CMCIBasicFunctionsDlg::ReadAll()
{
	unsigned char input[4];
	DIO_ReadAll(theApp.deviceIndex, &input);
	m_strPortA.Format( "%02X", input[0] );
	m_strPortB.Format( "%02X", input[1] );
	m_strPortC.Format( "%02X", input[2] );
	m_strPortD.Format( "%02X", input[3] );

	IObitsMask = 0x00000000;
	IObitsMask |= input[0];
	IObitsMask |= input[1] << 8;
	IObitsMask |= input[2] << 16;
	IObitsMask |= input[3] << 24;

	UpdateData( FALSE );
	Invalidate(0);
}

void CMCIBasicFunctionsDlg::OnWriteAll() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );

	CString str = "";
	char *buf;
	unsigned char output[4];
	unsigned long outMask = 0x00000000;
	IObitsMask = 0x00000000;

	str = m_strPortA;
	buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output[0]);
	str.ReleaseBuffer();
	outMask |= output[0];
	IObitsMask |= output[0];

	str = m_strPortB;
	buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output[1]);
	str.ReleaseBuffer();
	outMask |= output[1] << 8;
	IObitsMask |= output[1] << 8;

	str = m_strPortC;
	buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output[2]);
	str.ReleaseBuffer();
	outMask |= output[2] << 16;
	IObitsMask |= output[2] << 16;

	str = m_strPortD;
	buf = str.GetBuffer(3);
	sscanf(buf, "%x", &output[3]);
	str.ReleaseBuffer();
	outMask |= output[3] << 24;
	IObitsMask |= output[3] << 24;

	DIO_WriteAll(theApp.deviceIndex, output);
	UpdateListString_OutMask_Write( outMask );
	Invalidate(0);
}

void CMCIBasicFunctionsDlg::OnConfigEx() 
{
	// TODO: Add your control notification handler code here
	UpdateConfig();
}

void CMCIBasicFunctionsDlg::OnConfig3() 
{
	// TODO: Add your control notification handler code here
	unsigned char OutMask = 0x08; //A, B, and C input; D is opposite C, so it's output.
	unsigned long DIOData = 0xFFFFFFFF;
	unsigned char TristateMask = 0x00;
	DIO_ConfigureEx(theApp.deviceIndex, &OutMask, &DIOData, &TristateMask);

	UpdatePortState( OutMask );
	UpdateListString_OutMask_Config( OutMask );
	UpdateListString_ReadEveryBit();
	UpdateListString_ReadEveryPort();

	unsigned long outMask = 0x04000000;
	DIO_WriteAll(theApp.deviceIndex, &outMask);

	UpdateListString_OutMask_Write( outMask );
	UpdateListString_ReadEveryBit();
	UpdateListString_ReadEveryPort();
}

void CMCIBasicFunctionsDlg::OnConfig4() 
{
	// TODO: Add your control notification handler code here
	///////////////////////////////////////////////////////////////////////////
	unsigned char OutMask = 0x07; //A, B, and C output; D is opposite C, so it's input.
	unsigned long DIOData = 0xFFFFFFFF;
	unsigned char TristateMask = 0x00;
	DIO_ConfigureEx(theApp.deviceIndex, &OutMask, &DIOData, &TristateMask);

	UpdatePortState( OutMask );
	UpdateListString_OutMask_Config( OutMask );
	UpdateListString_ReadEveryBit();
	UpdateListString_ReadEveryPort();

	unsigned long outMask = 0x0103060F;
	DIO_WriteAll(theApp.deviceIndex, &outMask);

	UpdateListString_OutMask_Write( outMask );
	UpdateListString_ReadEveryBit();
	UpdateListString_ReadEveryPort();
}

void CMCIBasicFunctionsDlg::OnConfig5() 
{
	// TODO: Add your control notification handler code here
	///////////////////////////////////////////////////////////////////////////
	unsigned char OutMask = 0x0A; //A, C input; B, D output.
	unsigned long DIOData = 0xFFFFFFFF;
	unsigned char TristateMask = 0x00;
	DIO_ConfigureEx(theApp.deviceIndex, &OutMask, &DIOData, &TristateMask);

	UpdatePortState( OutMask );
	UpdateListString_OutMask_Config( OutMask );
	UpdateListString_ReadEveryBit();
	UpdateListString_ReadEveryPort();

	unsigned long outMask = 0xAAAAAAAA;
	DIO_WriteAll(theApp.deviceIndex, &outMask);

	UpdateListString_OutMask_Write( outMask );
	UpdateListString_ReadEveryBit();
	UpdateListString_ReadEveryPort();
}

void CMCIBasicFunctionsDlg::OnConfig6() 
{
	// TODO: Add your control notification handler code here
	///////////////////////////////////////////////////////////////////////////
	unsigned char OutMask = 0x05; //A, C output; B, D input.
	unsigned long DIOData = 0xFFFFFFFF;
	unsigned char TristateMask = 0x00;
	DIO_ConfigureEx(theApp.deviceIndex, &OutMask, &DIOData, &TristateMask);

	UpdatePortState( OutMask );
	UpdateListString_OutMask_Config( OutMask );
	UpdateListString_ReadEveryBit();
	UpdateListString_ReadEveryPort();

	unsigned long outMask = 0xAAAAAAAA;
	DIO_WriteAll(theApp.deviceIndex, &outMask);

	UpdateListString_OutMask_Write( outMask );
	UpdateListString_ReadEveryBit();
	UpdateListString_ReadEveryPort();
}

void CMCIBasicFunctionsDlg::OnConfig7() 
{
	// TODO: Add your control notification handler code here
	///////////////////////////////////////////////////////////////////////////
	unsigned char OutMask = 0x09; //A, D output; B, C input.
	unsigned long DIOData = 0xFFFFFFFF;
	unsigned char TristateMask = 0x00;
	DIO_ConfigureEx(theApp.deviceIndex, &OutMask, &DIOData, &TristateMask);

	UpdatePortState( OutMask );
	UpdateListString_OutMask_Config( OutMask );
	UpdateListString_ReadEveryBit();
	UpdateListString_ReadEveryPort();

	unsigned long outMask = 0xAAAAAAAA;
	DIO_WriteAll(theApp.deviceIndex, &outMask);

	UpdateListString_OutMask_Write( outMask );
	UpdateListString_ReadEveryBit();
	UpdateListString_ReadEveryPort();
}

void CMCIBasicFunctionsDlg::OnRADIOPortCInput() 
{
	// TODO: Add your control notification handler code here
	CButton* pbtn = (CButton*)( GetDlgItem(IDC_RADIO_PortD_Input) );
	if( pbtn != NULL )
		pbtn->SetCheck( FALSE );
	pbtn = (CButton*)( GetDlgItem(IDC_RADIO_PortD_Output) );
	if( pbtn != NULL )
		pbtn->SetCheck( TRUE );
}

void CMCIBasicFunctionsDlg::OnRADIOPortCOutput() 
{
	// TODO: Add your control notification handler code here
	CButton* pbtn = (CButton*)( GetDlgItem(IDC_RADIO_PortD_Input) );
	if( pbtn != NULL )
		pbtn->SetCheck( TRUE );
	pbtn = (CButton*)( GetDlgItem(IDC_RADIO_PortD_Output) );
	if( pbtn != NULL )
		pbtn->SetCheck( FALSE );
}

void CMCIBasicFunctionsDlg::OnRADIOPortDInput() 
{
	// TODO: Add your control notification handler code here
	CButton* pbtn = (CButton*)( GetDlgItem(IDC_RADIO_PortC_Input) );
	if( pbtn != NULL )
		pbtn->SetCheck( FALSE );
	pbtn = (CButton*)( GetDlgItem(IDC_RADIO_PortC_Output) );
	if( pbtn != NULL )
		pbtn->SetCheck( TRUE );
}

void CMCIBasicFunctionsDlg::OnRADIOPortDOutput() 
{
	// TODO: Add your control notification handler code here
	CButton* pbtn = (CButton*)( GetDlgItem(IDC_RADIO_PortC_Input) );
	if( pbtn != NULL )
		pbtn->SetCheck( TRUE );
	pbtn = (CButton*)( GetDlgItem(IDC_RADIO_PortC_Output) );
	if( pbtn != NULL )
		pbtn->SetCheck( FALSE );
}
