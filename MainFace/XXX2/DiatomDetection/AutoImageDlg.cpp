
// AutoImageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "XXX2.h"
#include "AutoImageDlg.h"
#include "afxdialogex.h"
#include "FillButton.h"
#include <vector>
using namespace std;
#include "WaferRatioRuler.h"

#define ButtonID_Base 10000
#define ButtonID_Min  10100
#define ButtonID_Max  10900


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CAutoImageDlg 对话框
CAutoImageDlg::CAutoImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoImageDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_mapID2ButtonClicked.clear();
	m_mapID2Button.clear();
	m_nLastClickedButtonID = 0;
}


CAutoImageDlg::~CAutoImageDlg()
{
	m_mapID2ButtonClicked.clear();
	for (map<int,CFillButton*>::iterator it = m_mapID2Button.begin();it != m_mapID2Button.end();it++)
	{
		CFillButton* pButton = it->second;
		if (pButton)
		{
			pButton->DestroyWindow();
			delete pButton;
			pButton = NULL;
		}
	}

	m_mapID2Button.clear();
}

void CAutoImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAutoImageDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND_RANGE(ButtonID_Min,ButtonID_Max,OnButtonClick)
END_MESSAGE_MAP()


// CAutoImageDlg 消息处理程序

BOOL CAutoImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	_CreateLayout();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAutoImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAutoImageDlg::OnPaint()
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
		_DrawCircle();

		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAutoImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAutoImageDlg::OnButtonClick(UINT uID)
{
	map<int,CFillButton*>:: iterator it = m_mapID2Button.find(uID);
	if(m_mapID2Button.end() == it) return;

	CFillButton* pButton = it->second;
	if(!pButton) return;

	m_mapID2ButtonClicked[uID] = pButton;
	pButton->setWordColor(RGB(255,0,255));

	_ChangeCliekedButtonColor(uID);

	pButton->ShowPostion();
	pButton->MoveStage();
}

void CAutoImageDlg::_CreateLayout()
{
	const float fButtonWidth = 28;
	const float fButtonHeight = 23;

	//按照晶圆形状，每行有多少个Fill
	const int nRow[Fill_RowCount] = {6,8,14,14,16,18,20,20,22,22,22,22,22,22,22,22,22,22,22,22,20,20,18,16,14,14};

	CRect rect;
	GetClientRect(&rect);

	//晶圆直径
	int nTotalWidth = min(rect.Width(),rect.Height()) - 20;

	//晶圆中心坐标(x,y)
	int nXCenter = nTotalWidth/2 - 22;
	int nYCenter = nTotalWidth/2 - 20;

	//Button 编号
	int nButtonSeq = 0;

	//逐行创建Button
	for (int i = 0; i < Fill_RowCount;i++)
	{
		int nButtonCount = nRow[i];
		//本行Y(高度)值
		float fRowY = nYCenter - (Fill_RowCount/2 - i)* fButtonHeight;

		int nColumnIndex = 0;

		//创建左半部分晶圆
		for (int j = nButtonCount/2 - 1; j >= 0;j--)
		{
			CRect rectButton;
			rectButton.left = nXCenter - j * fButtonWidth;
			rectButton.top = fRowY;
			rectButton.right = nXCenter - j * fButtonWidth + fButtonWidth;
			rectButton.bottom = fRowY + fButtonHeight;

			//根据 位置和ID 创建 Button
			CFillButton* btn = _CreateButton(rectButton,nButtonSeq);
			//设置在UI界面上所处的行，列
			btn->SetPosition(i,nColumnIndex);
			//计算对应的样品台坐标
			btn->CalculateStagePosition(CPoint(nXCenter,nYCenter));

			//CHvVAC_StageDlg* pParent = dynamic_cast<CHvVAC_StageDlg*>(GetParent());
			//if(pParent)
			//{
			//	btn->SetStageClient(&pParent->m_Client);
			//}
		
			nColumnIndex++;
			nButtonSeq++;
		}

		//创建右半部分晶圆
		for (int j = 0; j < nButtonCount/2;j++)
		{
			CRect rectButton;
			rectButton.left = nXCenter + (j+1) * fButtonWidth;
			rectButton.top = fRowY;
			rectButton.right = nXCenter + (j+1) * fButtonWidth + fButtonWidth;
			rectButton.bottom = fRowY + fButtonHeight;

			//根据 位置和ID 创建 Button
			CFillButton* btn = _CreateButton(rectButton,nButtonSeq);
			//设置在UI界面上所处的行，列
			btn->SetPosition(i,nColumnIndex);
			//计算对应的样品台坐标
			btn->CalculateStagePosition(CPoint(nXCenter,nYCenter));

			//CHvVAC_StageDlg* pParent = dynamic_cast<CHvVAC_StageDlg*>(GetParent());
			//if(pParent)
			//{
			//	btn->SetStageClient(&pParent->m_Client);
			//}

			nColumnIndex++;
			nButtonSeq++;
		}
	}
}

CFillButton*  CAutoImageDlg::_CreateButton(const CRect& rectButton,int nButtonSeq)
{
	CFillButton* btn = new CFillButton();
	CString strCaption = _T("");
	strCaption.Format(_T("%d"),nButtonSeq);

	const int nButtonID = ButtonID_Min + nButtonSeq;

	btn->Create(strCaption, WS_CHILD | WS_VISIBLE, rectButton, this, nButtonID);
	//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
	btn->ModifyStyle(0, BS_OWNERDRAW, 0);	
	//设置字体大小
	btn->setWordSize(80);

	btn->m_nShowID = nButtonSeq;

	m_mapID2Button[nButtonID] = btn;

	return btn;
}


void CAutoImageDlg::_DrawCircle()
{
	CRect rect;
	GetClientRect(&rect);
	int nWidth = min(rect.Width(),rect.Height()) - 20;
	int nXCenter = nWidth/2 - 22 + 28;
	int nYCenter = nWidth/2 - 20 + 23;

	CClientDC dcClient(this);
	dcClient.SelectStockObject(NULL_BRUSH);
	dcClient.Ellipse(10,10,nWidth,nWidth);
	//dcClient.MoveTo(159,rect.Height() - 102);
	//dcClient.LineTo(159 + 388,rect.Height() - 102);

	//CRect rectErase(159,rect.Height() - 101,159 + 400,rect.Height() - 36);
	//CBrush brush;
	//brush.CreateSolidBrush(RGB(240,240,240));
	//dcClient.FillRect(&rectErase,&brush); //用FillRect成员函数利用笔刷填充指定区域
	//brush.DeleteObject();

	CPen pen(PS_SOLID, 1, RGB(0, 0, 255));
	CPen *pOldPen = dcClient.SelectObject(&pen);

	dcClient.MoveTo(nXCenter,0);
	dcClient.LineTo(nXCenter,rect.Height() - 72);
	dcClient.MoveTo(nXCenter - 6,rect.Height() - 82);
	dcClient.LineTo(nXCenter,rect.Height() - 72);
	dcClient.MoveTo(nXCenter + 6,rect.Height() - 82);
	dcClient.LineTo(nXCenter,rect.Height() - 72);

	dcClient.TextOut(nXCenter - 20,rect.Height() - 97,"X");

	dcClient.MoveTo(0,nYCenter);
	dcClient.LineTo(rect.Width() - 2,nYCenter);
	dcClient.MoveTo(rect.Width() - 12,nYCenter - 6);
	dcClient.LineTo(rect.Width() - 2,nYCenter);
	dcClient.MoveTo(rect.Width() - 12,nYCenter + 6);
	dcClient.LineTo(rect.Width() - 2,nYCenter);

	dcClient.TextOut(rect.Width() - 17,nYCenter - 25,"Y");

	dcClient.SelectObject(&pOldPen);
}


void CAutoImageDlg::_ChangeCliekedButtonColor(UINT uID)
{
	map<int,CFillButton*>:: iterator it = m_mapID2ButtonClicked.find(m_nLastClickedButtonID);
	if(m_mapID2ButtonClicked.end() == it)
	{
	   m_nLastClickedButtonID = uID;
	   return;
	}
	
	CFillButton* pButton = it->second;
	if(pButton)
	{
		pButton->setWordColor(RGB(0,0,255));
		pButton->Invalidate(TRUE);
	}

	m_nLastClickedButtonID = uID;
}