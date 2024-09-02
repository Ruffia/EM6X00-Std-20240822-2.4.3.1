
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
	// 获取屏幕宽度和高度
	CRect rect;
	GetClientRect(&rect);
	int nWidth = min(rect.Width(),rect.Height()) - 20;
	int nXCenter = nWidth/2;
	int nYCenter = nWidth/2;

	const double screenRatio = 10; //屏幕比例

	const double circleRadius = 45; // 25mm 直径
	const double rectWidth = 4.5;     // 0.8mm 宽度
	const double rectHeight = 4.5;    // 0.8mm 高度
	CPoint circleCenter(nWidth/2, nWidth/2); // 圆心在屏幕上的位置


	// 存储矩形位置的变量
	std::vector<CPoint> positions;

	double y = -circleRadius + rectHeight / 2;
	double x_start = -circleRadius + rectWidth / 2;

	while (y + rectHeight / 2 <= circleRadius)
	{
		double x = x_start;
		while (x + rectWidth / 2 <= circleRadius)
		{
			if (_IsRectInCircle(circleCenter, circleRadius * screenRatio, CPoint((int)(circleCenter.x + x * screenRatio), (int)(circleCenter.y + y * screenRatio)), (int)(rectWidth * screenRatio), (int)(rectHeight * screenRatio)))
			{
				positions.push_back(CPoint((int)(circleCenter.x + x * screenRatio), (int)(circleCenter.y + y * screenRatio)));
			}
			x += rectWidth;
		}
		y += rectHeight;
	}


	// 绘制矩形并编号
	for (size_t i = 0; i < positions.size(); ++i)
	{
		CRect rect(positions[i].x - (int)(rectWidth * 5),
			positions[i].y - (int)(rectHeight * 5),
			positions[i].x + (int)(rectWidth * 5),
			positions[i].y + (int)(rectHeight * 5));
		
		CFillButton* pBtn = _CreateButton(rect,i);
		//pBtn->SetStagePosition();
		//pBtn->CalculateStagePosition();

		double autoPlatform_X = ((positions[i].x- nWidth / 2) * 12.5) / nWidth;
		double autoPlatform_y = ((positions[i].y - nWidth / 2) * 12.5) / nWidth;
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


bool CAutoImageDlg::_IsRectInCircle(CPoint circleCenter, int circleRadius, CPoint rectCenter, int rectWidth, int rectHeight)
{
	CPoint rectCorners[4] = {
		CPoint(rectCenter.x - rectWidth / 2, rectCenter.y - rectHeight / 2),
		CPoint(rectCenter.x + rectWidth / 2, rectCenter.y - rectHeight / 2),
		CPoint(rectCenter.x - rectWidth / 2, rectCenter.y + rectHeight / 2),
		CPoint(rectCenter.x + rectWidth / 2, rectCenter.y + rectHeight / 2),
	};

	for (int i = 0; i < 4; ++i)
	{
		if (sqrt(pow((rectCorners[i].x - circleCenter.x), 2) + pow((rectCorners[i].y - circleCenter.y), 2)) > circleRadius)
		{
			return false;
		}
	}
	return true;
}


void CAutoImageDlg::_DrawCircle()
{
	CRect rect;
	GetClientRect(&rect);
	int nWidth = min(rect.Width(),rect.Height()) - 20;
	int nXCenter = nWidth/2 ;
	int nYCenter = nWidth/2 ;

	CClientDC dcClient(this);
	dcClient.SelectStockObject(NULL_BRUSH);

	CPen pen(PS_SOLID, 1, RGB(0, 0, 255));
	CPen *pOldPen = dcClient.SelectObject(&pen);

	
	// 获取屏幕宽度和高度
	const double screenRatio = 10; //屏幕比例

	const double circleRadius = 45; // 25mm 直径
	const double rectWidth = 4.5;     // 0.8mm 宽度
	const double rectHeight = 4.5;    // 0.8mm 高度
	CPoint circleCenter(nWidth/2, nWidth/2); // 圆心在屏幕上的位置

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

	// 画圆
	dcClient.Ellipse(circleCenter.x - (int)circleRadius * screenRatio, circleCenter.y - (int)circleRadius * screenRatio,
		circleCenter.x + (int)circleRadius * screenRatio, circleCenter.y + (int)circleRadius * screenRatio);
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