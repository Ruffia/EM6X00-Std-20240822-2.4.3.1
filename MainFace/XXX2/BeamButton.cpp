#include "stdafx.h"
#include "resource.h"
#include "BeamButton.h"

int	nButtonState = 0;   //正常

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
	dc.Attach(lpDrawItemStruct->hDC);//得到绘制的设备环境CDC
	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);
	CString strText = m_strCaption;
	
	SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);//透明
    // 根据m_state绘制按钮
	COLORREF clrInner, clrPen, clrBG;
	int nLeft = 3, nTop = 3, nRight = 3, nBottom = 3;
	switch (m_state)
	{
	case STATE_NORMAL:
		// 绘制正常状态
		{
			clrInner	= RGB(128,128,128);	// 内背景画刷色
			clrPen		= RGB(128,128,128);	// 边框画笔色
			clrBG		= RGB(128,128,128);	// 大背景画刷色
			nLeft = nTop = 4;
		}
		break;
	case STATE_DETECTION:
		{
			// 绘制按钮按下状态
			clrInner	= RGB(50,205,50);	// 内背景画刷色
			clrPen		= RGB(20,20,20);	// 边框画笔色
			clrBG		= RGB(50,205,50);	// 大背景画刷色
		}
		break;
	case STATE_CONTRAL:
		{
			clrInner	= RGB(0,160,230);	// 内背景画刷色
			clrPen		= RGB(80,80,80);	// 边框画笔色
			clrBG		= RGB(0,0,250);		// 大背景画刷色
		}
		break;
	}
	CBrush brushInner(clrInner);	//内背景画刷
	CPen pen(0,1,clrPen);			//边框画笔
	CBrush brushBG(clrBG);			//大背景画刷
	dc.FillRect(&(lpDrawItemStruct->rcItem), &brushBG);	//利用画刷brush，填充大矩形框	
	CRect rect = lpDrawItemStruct->rcItem;
	CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	POINT pt;
	//画按钮的外边框，它是一个半径为10的圆角矩形
	pt.x = 10;
	pt.y = 10;
	CPen* hOldPen = pDC->SelectObject(&pen);
	pDC->RoundRect(&rect, pt);
	pDC->SelectObject(hOldPen);
	rect.DeflateRect(3, 3, 3, 3);	//缩进
	CBrush *pOldBrush = pDC->SelectObject(&brushBG);  
	pDC->Rectangle(rect);			//画矩形
	pDC->SelectObject(pOldBrush);
	pDC->FillRect(rect,&brushInner );	//填充内矩形
	//因为这里进行了重绘,所以文字也要重绘
	DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(),&lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);		
	dc.Detach();
/*
	switch (m_state)
    {
    case STATE_NORMAL:
        // 绘制正常状态
		{
			CBrush brush(RGB(128, 128, 128));    //内背景画刷
			CPen m_BoundryPen(0,1,RGB(128,128,128));   //边框画笔
			CBrush m_BackgroundBrush=RGB(128,128,128);    //大背景画刷
			dc.FillRect(&(lpDrawItemStruct->rcItem), &m_BackgroundBrush);//利用画刷brush，填充矩形框	
			CRect rect = lpDrawItemStruct->rcItem;
			CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
			POINT pt;
			//画按钮的外边框，它是一个半径为5的圆角矩形
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
			//因为这里进行了重绘,所以文字也要重绘
			DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(),&lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			break;
		}
    case STATE_DETECTION:
		{
			// 绘制按钮按下状态
			CBrush brush(RGB(50,205,50));    //内背景画刷
			CPen m_BoundryPen(0,1,RGB(20,20,20));   //边框画笔
			CBrush m_BackgroundBrush=RGB(50,205,50);    //大背景画刷
			dc.FillRect(&(lpDrawItemStruct->rcItem), &m_BackgroundBrush);//利用画刷brush，填充大矩形框	
			CRect rect = lpDrawItemStruct->rcItem;
			CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
			POINT pt;
			//画按钮的外边框，它是一个半径为10的圆角矩形
			pt.x = 10;
			pt.y = 10;
			CPen* hOldPen = pDC->SelectObject(&m_BoundryPen);
			pDC->RoundRect(&rect, pt);
			pDC->SelectObject(hOldPen);
			rect.DeflateRect(3, 3, 3, 3);   //缩进
			CBrush *pOldBrush = pDC->SelectObject(&m_BackgroundBrush);  
			pDC->Rectangle(rect);    //画矩形
			pDC->SelectObject(pOldBrush);
			pDC->FillRect(rect,&brush );    //填充内矩形
			//因为这里进行了重绘,所以文字也要重绘
			DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(),&lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);		
			break;
		}
	case STATE_CONTRAL:
		{
			CBrush brush(RGB(0, 160, 230));    //内背景画刷
			CPen m_BoundryPen(0,1,RGB(80,80,80));   //边框画笔
			CBrush m_BackgroundBrush=RGB(0,0,250);    //大背景画刷
			dc.FillRect(&(lpDrawItemStruct->rcItem), &m_BackgroundBrush);//利用画刷brush，填充大矩形框	
			CRect rect = lpDrawItemStruct->rcItem;
			CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
			POINT pt;
			//画按钮的外边框，它是一个半径为10的圆角矩形
			pt.x = 10;
			pt.y = 10;
			CPen* hOldPen = pDC->SelectObject(&m_BoundryPen);
			pDC->RoundRect(&rect, pt);
			pDC->SelectObject(hOldPen);
			rect.DeflateRect(3, 3, 3, 3);   //缩进
			CBrush *pOldBrush = pDC->SelectObject(&m_BackgroundBrush);  
			pDC->Rectangle(rect);    //画矩形
			pDC->SelectObject(pOldBrush);
			pDC->FillRect(rect,&brush );    //填充内矩形
			//因为这里进行了重绘,所以文字也要重绘
			DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(),&lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			break;
		}
    }
	dc.Detach();*/
}
 
void CBeamButton::OnLButtonDown(int nButtonState)
{
    // 鼠标按下
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
 