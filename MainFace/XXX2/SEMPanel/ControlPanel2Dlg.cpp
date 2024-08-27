
// ControlPanel2Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "XXX2.h"
#include "ControlPanel2Dlg.h"
#include "afxdialogex.h"
#include "Tools/InputEdit.h"
#include "LibUSBCommandButton.h"
#define ButtonID_Min 10300
#define ButtonID_Max 10310
static const int nControlID = 10100;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CControlPanel2Dlg 对话框

const string  CControlPanel2Dlg::m_strFunction[8] = {
	"libusb_init",
	"get_device_descriptor",
	"get_device_endpoint",
	"libusb_open_device_with_vid_pid",
	"libusb_claim_interface",
    "libusb_bulk_transfer_send",
	"libusb_bulk_transfer_receive",
	""};

const string  CControlPanel2Dlg::m_strButtomCaption[8] = {
	"init",
	"get_descriptor",
	"get_endpoint",
	"open_device",
	"claim_interface",
	"send",
	"receive",
	""};

CControlPanel2Dlg::CControlPanel2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CControlPanel2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	ZeroMemory(m_pEdit,64,0);
	ZeroMemory(m_pStatic,8,0);
	ZeroMemory(m_DataSend,64);
}


CControlPanel2Dlg::~CControlPanel2Dlg()
{
	for (int i = 0; i < 64;i++)
	{
		if (m_pEdit[i])
		{
			if (IsWindow(m_pEdit[i]->m_hWnd))
			{
				m_pEdit[i]->DestroyWindow();
			}		
			delete m_pEdit[i];
		}
	}

	for (int i = 0; i < 8;i++)
	{
		if (m_pStatic[i])
		{
			m_pStatic[i]->DestroyWindow();
			delete m_pStatic[i];
		}
	}

	if (m_pFont)
	{
		delete m_pFont;
		m_pFont = NULL;
	}

	_UnInitUSB();
}

void CControlPanel2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CControlPanel2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND_RANGE(ButtonID_Min,ButtonID_Max,OnButtonClick)
END_MESSAGE_MAP()


// CControlPanel2Dlg 消息处理程序

BOOL CControlPanel2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	int nRet = _InitUSB();
	if(nRet < 0)
	{
		MessageBox("Initialize USB Failed!");
	}

	_CreateLayout();

	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CControlPanel2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CControlPanel2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CControlPanel2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CControlPanel2Dlg::OnButtonClick(UINT uID)
{
	UpdateData(TRUE);

	map<int,CCommandButton*>:: iterator it = m_mapID2Button.find(uID);
	if(m_mapID2Button.end() == it) return;

	CCommandButton* pButton = it->second;
	if(!pButton) return;

	if (pButton->GetDlgCtrlID() < ButtonID_Min + 5)
	{
		pButton->ExecuteComand(m_DataSend,m_DataRecv);
	}
	else if (pButton->GetDlgCtrlID() == ButtonID_Min + 5)
	{
		for (int i = 0; i < 64; i++)
		{
			m_pEdit[i]->UpdateData(TRUE);
			CString strValue;
			GetDlgItemText(nControlID + i,strValue);
			byte byteData = 0;
			sscanf(strValue,"%02X",&byteData);
			m_DataSend[i] = byteData;
		}
		pButton->ExecuteComand(m_DataSend,m_DataRecv);
	}
	else if (pButton->GetDlgCtrlID() == ButtonID_Min + 6)
	{
		pButton->ExecuteComand(m_DataSend,m_DataRecv);
		CString strDataRecv = "";
		for (int i = 0; i < 64; i++)
		{
			CString strByte = "";
			strByte.Format("%02X  ",m_DataRecv[i]);
			strDataRecv = strDataRecv + strByte;
			if (i % 16 == 15)
			{
				strDataRecv += "\r\n";
			}
		}
		SetDlgItemText(m_pRecvEdit->GetDlgCtrlID() ,strDataRecv);
		m_pRecvEdit->UpdateData(FALSE);
	}
}

void CControlPanel2Dlg::_CreateLayout()
{
	_CreateFont();

	CRect rcBase(40,80,80,36);

	for (int i = 0; i < 5;i++)
	{
		CRect rc;
		rc.left = rcBase.left + i * 120;
		rc.top = rcBase.top - 50;
		rc.right = rc.left + 100;
		rc.bottom = rc.top + 20;
		_CreateButton(rc,i);
	}
	
		
	//创建 4(行) * 16列 控件
	const int nRowCount = 4;
	const int nColumnCount = 16;
	const int nEditWidth = 30;
	
	int K = 0;
	for (int i = 0; i < nRowCount; i++)
	{
		CRect rc;
		rc.top = rcBase.top + i * 30;
		rc.bottom = rc.top + 20;

		CStatic* pStaticFront = new CStatic;
		CString strStaticFront = "0X ";
		CRect rcStaticFront(rcBase.left - 20,rcBase.top + i * 30,rcBase.left ,rcBase.top + i * 30 + 20);
		pStaticFront->Create(strStaticFront,WS_CHILD|WS_VISIBLE|SS_CENTER,rcStaticFront,this,10100 + 100 + i - 1);
		pStaticFront->SetFont(m_pFont);
		m_pStatic[K++] = pStaticFront;
		pStaticFront->ShowWindow(SW_SHOW);

		CStatic* pStaticBack = new CStatic;
		CString strStaticBack;
		strStaticBack.Format("(%d--%d)",i*nColumnCount,i*nColumnCount + 15);
		CRect rcStatic(rcBase.left + nColumnCount * 40,rcBase.top + i * 30,rcBase.left + nColumnCount * 40 + 50,rcBase.top + i * 30 + 20);
		pStaticBack->Create(strStaticBack,WS_CHILD|WS_VISIBLE|SS_CENTER,rcStatic,this,10100 + 100 + i);
		pStaticBack->SetFont(m_pFont);
		m_pStatic[K++] = pStaticBack;
		pStaticBack->ShowWindow(SW_SHOW);

		for (int j = 0; j < nColumnCount;j++)
		{
			rc.left = rcBase.left + j * 40;
			rc.right = rc.left + nEditWidth;
			InputEdit* pEdit = new InputEdit;
			pEdit->Create(WS_CHILD|WS_VISIBLE|SS_CENTER,rc,this, (nControlID + i* 16 + j) );
			pEdit->SetFont(m_pFont);
			pEdit->SetLimitText(2);
			CString strText;
			strText.Format("%02X",i*16 + j);
			pEdit->SetWindowTextA(strText);
			m_pEdit[i* 16 + j] = pEdit;
			pEdit->ShowWindow(SW_SHOW);
		}

	}

	CRect rcSend;
	rcSend.left = rcBase.left + 40 * 16 + 60;
	rcSend.top = rcBase.top + 30;
	rcSend.right = rcSend.left + 60;
	rcSend.bottom = rcSend.top + 50;
	_CreateButton(rcSend,5);

	CRect rcRecv(rcBase.left,rcBase.top + 3 * 50,rcBase.left + 16 * 40,rcBase.top + 3 * 50 + 120);
	m_pRecvEdit = new CEdit;
	m_pRecvEdit->Create(WS_CHILD|WS_VISIBLE|SS_CENTER,rcRecv,this,10300);
	m_pRecvEdit->SetFont(m_pFont);
	m_pRecvEdit->ShowWindow(SW_SHOW);


	CRect rcRecvButton;
	rcRecvButton.left = rcBase.left + 40 * 16 + 60;
	rcRecvButton.top = rcBase.top + 180;
	rcRecvButton.right = rcRecvButton.left + 60;
	rcRecvButton.bottom = rcRecvButton.top + 50;
	_CreateButton(rcRecvButton,6);

}

void CControlPanel2Dlg::_CreateFont()
{
	m_pFont = new CFont;
	m_pFont->CreateFont(18, // nHeight 
		6, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_NORMAL, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("微软雅黑")); // lpszFac
}


CCommandButton*  CControlPanel2Dlg::_CreateButton(const CRect& rectButton,int nButtonSeq)
{
	CCommandButton* btn = new CCommandButton();
	CString strCaption(m_strButtomCaption[nButtonSeq].c_str());

	const int nButtonID = ButtonID_Min + nButtonSeq;

	btn->Create(strCaption, WS_CHILD | WS_VISIBLE, rectButton, this, nButtonID);
	//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
	btn->ModifyStyle(0, BS_OWNERDRAW, 0);	
	//设置字体大小
	btn->setWordSize(80);
	btn->Initialize(usb_handle,&udev,&udev_desc);
	string strFunction = m_strFunction[nButtonSeq];
	btn->SetCommand(strFunction);

	m_mapID2Button[nButtonID] = btn;

	return btn;
}


int CControlPanel2Dlg::_InitUSB()
{
	usb_handle = NULL;
	//1. load user data.
	udev.vid = VID;
	udev.pid = PID;
	udev.bInterfaceClass = LIBUSB_CLASS_PER_INTERFACE;
	udev.bInterfaceSubClass = LIBUSB_CLASS_PER_INTERFACE;
	udev.bmAttributes = LIBUSB_TRANSFER_TYPE_BULK;
	udev.dev = NULL;

	//2. init libusb.
	int ret = libusb_init(NULL);
	if(ret < 0)
	{
		return -1;
	}

	//3. search for specified usb device.
	ret = get_device_descriptor(&udev_desc, &udev);
	if(ret < 0) {
		return -2;
	}

	ret = get_device_endpoint(&udev_desc, &udev);
	if(ret < 0) {
		return -3;
	}

	/*4.open device and start communication by usb*/
	//open the usb device
	usb_handle = libusb_open_device_with_vid_pid(NULL, udev.vid, udev.pid);
	if(usb_handle == NULL) {
		return -4;
	}

	ret = libusb_claim_interface(usb_handle, udev.bInterfaceNumber);
	if(ret < 0) {
		ret = libusb_detach_kernel_driver(usb_handle, udev.bInterfaceNumber);
		if(ret < 0) {
			return -5;
		}
		ret = libusb_claim_interface(usb_handle, udev.bInterfaceNumber);
		if(ret < 0)
		{
			return -6;
		}
	}

	return ret;
}


void CControlPanel2Dlg::_UnInitUSB()
{
	//6，注销
	libusb_close(usb_handle);
	libusb_release_interface(usb_handle, udev.bInterfaceNumber);
	libusb_free_device_list(udev.devs, 1);
	libusb_exit(NULL);
	usb_handle = NULL;
}
