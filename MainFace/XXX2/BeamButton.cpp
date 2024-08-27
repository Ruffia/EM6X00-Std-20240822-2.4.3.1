#include "stdafx.h"
#include "resource.h"
#include "BeamButton.h"

int	nButtonState = 0;   //����

BEGIN_MESSAGE_MAP(CBeamButton, CButton)
END_MESSAGE_MAP()
 
CBeamButton::CBeamButton() : m_state(STATE_NORMAL)
{
}
 
void CBeamButton::SetButtonState(ButtonState state)
{
    m_state = state;
    RedrawWindow();
}
 
void CBeamButton::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);//�õ����Ƶ��豸����CDC
	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);
	CString strText = m_strCaption;
	
	SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);//͸��
    // ����m_state���ư�ť
	COLORREF clrInner, clrPen, clrBG;
	int nLeft = 3, nTop = 3, nRight = 3, nBottom = 3;
	switch (m_state)
	{
	case STATE_NORMAL:
		// ��������״̬
		{
			clrInner	= RGB(128,128,128);	// �ڱ�����ˢɫ
			clrPen		= RGB(128,128,128);	// �߿򻭱�ɫ
			clrBG		= RGB(128,128,128);	// �󱳾���ˢɫ
			nLeft = nTop = 4;
		}
		break;
	case STATE_DETECTION:
		{
			// ���ư�ť����״̬
			clrInner	= RGB(50,205,50);	// �ڱ�����ˢɫ
			clrPen		= RGB(20,20,20);	// �߿򻭱�ɫ
			clrBG		= RGB(50,205,50);	// �󱳾���ˢɫ
		}
		break;
	case STATE_CONTRAL:
		{
			clrInner	= RGB(0,160,230);	// �ڱ�����ˢɫ
			clrPen		= RGB(80,80,80);	// �߿򻭱�ɫ
			clrBG		= RGB(0,0,250);		// �󱳾���ˢɫ
		}
		break;
	}
	CBrush brushInner(clrInner);	//�ڱ�����ˢ
	CPen pen(0,1,clrPen);			//�߿򻭱�
	CBrush brushBG(clrBG);			//�󱳾���ˢ
	dc.FillRect(&(lpDrawItemStruct->rcItem), &brushBG);	//���û�ˢbrush��������ο�	
	CRect rect = lpDrawItemStruct->rcItem;
	CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	POINT pt;
	//����ť����߿�����һ���뾶Ϊ10��Բ�Ǿ���
	pt.x = 10;
	pt.y = 10;
	CPen* hOldPen = pDC->SelectObject(&pen);
	pDC->RoundRect(&rect, pt);
	pDC->SelectObject(hOldPen);
	rect.DeflateRect(3, 3, 3, 3);	//����
	CBrush *pOldBrush = pDC->SelectObject(&brushBG);  
	pDC->Rectangle(rect);			//������
	pDC->SelectObject(pOldBrush);
	pDC->FillRect(rect,&brushInner );	//����ھ���
	//��Ϊ����������ػ�,��������ҲҪ�ػ�
	DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(),&lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);		
	dc.Detach();
/*
	switch (m_state)
    {
    case STATE_NORMAL:
        // ��������״̬
		{
			CBrush brush(RGB(128, 128, 128));    //�ڱ�����ˢ
			CPen m_BoundryPen(0,1,RGB(128,128,128));   //�߿򻭱�
			CBrush m_BackgroundBrush=RGB(128,128,128);    //�󱳾���ˢ
			dc.FillRect(&(lpDrawItemStruct->rcItem), &m_BackgroundBrush);//���û�ˢbrush�������ο�	
			CRect rect = lpDrawItemStruct->rcItem;
			CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
			POINT pt;
			//����ť����߿�����һ���뾶Ϊ5��Բ�Ǿ���
			pt.x = 10;
			pt.y = 10;
			CPen* hOldPen = pDC->SelectObject(&m_BoundryPen);
			pDC->RoundRect(&rect, pt);
			pDC->SelectObject(hOldPen);
			rect.DeflateRect(4, 4, 3, 3);   
			CBrush *pOldBrush = pDC->SelectObject(&m_BackgroundBrush);  
			pDC->Rectangle(rect);   
			pDC->SelectObject(pOldBrush);
			pDC->FillRect(rect,&brush );
			//��Ϊ����������ػ�,��������ҲҪ�ػ�
			DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(),&lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			break;
		}
    case STATE_DETECTION:
		{
			// ���ư�ť����״̬
			CBrush brush(RGB(50,205,50));    //�ڱ�����ˢ
			CPen m_BoundryPen(0,1,RGB(20,20,20));   //�߿򻭱�
			CBrush m_BackgroundBrush=RGB(50,205,50);    //�󱳾���ˢ
			dc.FillRect(&(lpDrawItemStruct->rcItem), &m_BackgroundBrush);//���û�ˢbrush��������ο�	
			CRect rect = lpDrawItemStruct->rcItem;
			CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
			POINT pt;
			//����ť����߿�����һ���뾶Ϊ10��Բ�Ǿ���
			pt.x = 10;
			pt.y = 10;
			CPen* hOldPen = pDC->SelectObject(&m_BoundryPen);
			pDC->RoundRect(&rect, pt);
			pDC->SelectObject(hOldPen);
			rect.DeflateRect(3, 3, 3, 3);   //����
			CBrush *pOldBrush = pDC->SelectObject(&m_BackgroundBrush);  
			pDC->Rectangle(rect);    //������
			pDC->SelectObject(pOldBrush);
			pDC->FillRect(rect,&brush );    //����ھ���
			//��Ϊ����������ػ�,��������ҲҪ�ػ�
			DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(),&lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);		
			break;
		}
	case STATE_CONTRAL:
		{
			CBrush brush(RGB(0, 160, 230));    //�ڱ�����ˢ
			CPen m_BoundryPen(0,1,RGB(80,80,80));   //�߿򻭱�
			CBrush m_BackgroundBrush=RGB(0,0,250);    //�󱳾���ˢ
			dc.FillRect(&(lpDrawItemStruct->rcItem), &m_BackgroundBrush);//���û�ˢbrush��������ο�	
			CRect rect = lpDrawItemStruct->rcItem;
			CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
			POINT pt;
			//����ť����߿�����һ���뾶Ϊ10��Բ�Ǿ���
			pt.x = 10;
			pt.y = 10;
			CPen* hOldPen = pDC->SelectObject(&m_BoundryPen);
			pDC->RoundRect(&rect, pt);
			pDC->SelectObject(hOldPen);
			rect.DeflateRect(3, 3, 3, 3);   //����
			CBrush *pOldBrush = pDC->SelectObject(&m_BackgroundBrush);  
			pDC->Rectangle(rect);    //������
			pDC->SelectObject(pOldBrush);
			pDC->FillRect(rect,&brush );    //����ھ���
			//��Ϊ����������ػ�,��������ҲҪ�ػ�
			DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(),&lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			break;
		}
    }
	dc.Detach();*/
}
 
void CBeamButton::OnLButtonDown(int nButtonState)
{
    // ��갴��
	switch(nButtonState)
	{
	case 0:
		m_state =  STATE_NORMAL;
		break;
	case 1:
		m_state =  STATE_DETECTION;
		break;
	case 2:
		m_state =  STATE_CONTRAL;
		break;
	}
	Invalidate(FALSE);
}
 