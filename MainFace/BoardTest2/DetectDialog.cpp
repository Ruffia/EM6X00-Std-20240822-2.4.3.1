// DetectDialog.cpp : implementation file
//

#include "stdafx.h"
#include "insample0.h"
#include "DetectDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DetectDialog dialog

DetectDialog::DetectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(DetectDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(DetectDialog)
	m_message = _T("");
	//}}AFX_DATA_INIT
	bFoundFT232	= false;
	bFoundDI16A	= false;
}

void DetectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DetectDialog)
	DDX_Control(pDX, IDC_LIST1, m_deviceList);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_message);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(DetectDialog, CDialog)
	//{{AFX_MSG_MAP(DetectDialog)
	ON_BN_CLICKED(IDC_DETECT_BTN, OnDetectBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DetectDialog message handlers

void DetectDialog::OnDetectBtn() 
{
	CString str, strTemp;
	m_deviceList.ResetContent();
	///////////////////////////////////////////////////////////////////////////
	// ÕÒFT232Ä£¿é
	theApp.ft232Index = -1;
	m_deviceList.AddString( "FT232 device(s):" );

	int numDevs = theApp.m_232Device.Serial_ListDevicesCounts();
	if( numDevs > -1 )
	{
		str.Format("%d FT232 device(s) attached:", numDevs);
		m_deviceList.AddString(str);

		if( numDevs > 0 )
		{
			for( int i=0; i<numDevs; i++)
			{
				str.Format("   #%d Serial Number: %s ", i, theApp.m_232Device.Serial_GetDeviceSerialNumber(i) );
				m_deviceList.AddString(str);
				FT_HANDLE ftHandle;
				FT_DEVICE ftDevice;
				FT_STATUS ftStatus;
				DWORD deviceID;
				char SerialNumber[16];
				char Description[64];
				ftStatus = FT_Open(i, &ftHandle);
				if(ftStatus != FT_OK)
				{
					// FT_Open failed return;
					continue;
				}
				ftStatus = FT_GetDeviceInfo( ftHandle, &ftDevice, &deviceID, SerialNumber, Description, NULL );
				if (ftStatus == FT_OK)
				{
					if (ftDevice == FT_DEVICE_232H)
					{
						// device is FT232H
						bFoundFT232	= true;
						theApp.ft232Index = i;
						FT_Close(ftHandle);
						break;
					}
				}
				FT_Close(ftHandle);
			}
		}
		else
		{
			m_deviceList.AddString("do not found FT232 device!");
		}
	}
	else
		m_deviceList.AddString("FT_ListDevices failed");
	m_deviceList.AddString(" ");
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	// ÕÒUSB-DI16AÄ£¿é
	m_deviceList.AddString( "USB-DI16A device(s):" );
	unsigned long DeviceMask;
	unsigned long PID;
	char *buf;

	DeviceMask = GetDevices();
	if (DeviceMask)
	{
		QueryDeviceInfo(0, &PID, NULL, NULL, NULL, NULL);
		
		switch(PID)
		{
		case	0x8008:
		case	0x800A:
		case	0x800D:
			str.LoadString( IDS_DETECT_USBTYPE_16);
			bFoundDI16A = true;
			index = diOnly;
			break;
		default:
			buf = str.GetBuffer(5);
			itoa(PID, buf, 16);
			str.ReleaseBuffer();
			strTemp.LoadString( IDS_DETECT_USBTYPE_Default );
			str += strTemp;
			break;
		}
	}
	else
	{
		str.LoadString( IDS_DETECT_Not_Found );//IDS_DETECT_No_Device );
	}
	m_deviceList.AddString(str);

// 	if (bFoundDI16A)
// 	{
// 		m_message.LoadString( IDS_DETECT_Found );
// 	}
// 	else
// 	{
// 		m_message.LoadString( IDS_DETECT_Not_Found );
// 	}
	///////////////////////////////////////////////////////////////////////////

	UpdateData(FALSE);

}

BOOL DetectDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	OnDetectBtn();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DetectDialog::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}
