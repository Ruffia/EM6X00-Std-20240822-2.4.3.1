
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
	const float fButtonWidth = 28;
	const float fButtonHeight = 23;

	//���վ�Բ��״��ÿ���ж��ٸ�Fill
	const int nRow[Fill_RowCount] = {6,8,14,14,16,18,20,20,22,22,22,22,22,22,22,22,22,22,22,22,20,20,18,16,14,14};

	CRect rect;
	GetClientRect(&rect);

	//��Բֱ��
	int nTotalWidth = min(rect.Width(),rect.Height()) - 20;

	//��Բ��������(x,y)
	int nXCenter = nTotalWidth/2 - 22;
	int nYCenter = nTotalWidth/2 - 20;

	//Button ���
	int nButtonSeq = 0;

	//���д���Button
	for (int i = 0; i < Fill_RowCount;i++)
	{
		int nButtonCount = nRow[i];
		//����Y(�߶�)ֵ
		float fRowY = nYCenter - (Fill_RowCount/2 - i)* fButtonHeight;

		int nColumnIndex = 0;

		//������벿�־�Բ
		for (int j = nButtonCount/2 - 1; j >= 0;j--)
		{
			CRect rectButton;
			rectButton.left = nXCenter - j * fButtonWidth;
			rectButton.top = fRowY;
			rectButton.right = nXCenter - j * fButtonWidth + fButtonWidth;
			rectButton.bottom = fRowY + fButtonHeight;

			//���� λ�ú�ID ���� Button
			CFillButton* btn = _CreateButton(rectButton,nButtonSeq);
			//������UI�������������У���
			btn->SetPosition(i,nColumnIndex);
			//�����Ӧ����Ʒ̨����
			btn->CalculateStagePosition(CPoint(nXCenter,nYCenter));

			//CHvVAC_StageDlg* pParent = dynamic_cast<CHvVAC_StageDlg*>(GetParent());
			//if(pParent)
			//{
			//	btn->SetStageClient(&pParent->m_Client);
			//}
		
			nColumnIndex++;
			nButtonSeq++;
		}

		//�����Ұ벿�־�Բ
		for (int j = 0; j < nButtonCount/2;j++)
		{
			CRect rectButton;
			rectButton.left = nXCenter + (j+1) * fButtonWidth;
			rectButton.top = fRowY;
			rectButton.right = nXCenter + (j+1) * fButtonWidth + fButtonWidth;
			rectButton.bottom = fRowY + fButtonHeight;

			//���� λ�ú�ID ���� Button
			CFillButton* btn = _CreateButton(rectButton,nButtonSeq);
			//������UI�������������У���
			btn->SetPosition(i,nColumnIndex);
			//�����Ӧ����Ʒ̨����
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
	//����ť�޸�ΪBS_OWNERDRAW���,����button�Ĳ����Ի�ģʽ
	btn->ModifyStyle(0, BS_OWNERDRAW, 0);	
	//���������С
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
	//dcClient.FillRect(&rectErase,&brush); //��FillRect��Ա�������ñ�ˢ���ָ������
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