
// AutoImageDlg.cpp : ʵ���ļ�
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



// CAutoImageDlg �Ի���
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


// CAutoImageDlg ��Ϣ�������

BOOL CAutoImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	_CreateLayout();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CAutoImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAutoImageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		_DrawCircle();

		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	// ��ȡ��Ļ��Ⱥ͸߶�
	CRect rect;
	GetClientRect(&rect);
	int nWidth = min(rect.Width(),rect.Height()) - 20;
	int nXCenter = nWidth/2;
	int nYCenter = nWidth/2;

	const double screenRatio = 10; //��Ļ����

	const double circleRadius = 45; // 25mm ֱ��
	const double rectWidth = 4.5;     // 0.8mm ���
	const double rectHeight = 4.5;    // 0.8mm �߶�
	CPoint circleCenter(nWidth/2, nWidth/2); // Բ������Ļ�ϵ�λ��


	// �洢����λ�õı���
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


	// ���ƾ��β����
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
	//����ť�޸�ΪBS_OWNERDRAW���,����button�Ĳ����Ի�ģʽ
	btn->ModifyStyle(0, BS_OWNERDRAW, 0);	
	//���������С
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

	
	// ��ȡ��Ļ��Ⱥ͸߶�
	const double screenRatio = 10; //��Ļ����

	const double circleRadius = 45; // 25mm ֱ��
	const double rectWidth = 4.5;     // 0.8mm ���
	const double rectHeight = 4.5;    // 0.8mm �߶�
	CPoint circleCenter(nWidth/2, nWidth/2); // Բ������Ļ�ϵ�λ��

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

	// ��Բ
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