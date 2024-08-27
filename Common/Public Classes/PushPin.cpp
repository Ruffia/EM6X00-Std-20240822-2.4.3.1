// PushPin.cpp: implementation of the CPushPin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KYKY3200.h"
#include "PushPin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPushPin::CPushPin()
{
	m_bPinned = TRUE;
}

CPushPin::~CPushPin()
{

}

BEGIN_MESSAGE_MAP(CPushPin, CButtonST)
    //{{AFX_MSG_MAP(CPushPin)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPushPin::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
#ifdef ST_USE_MEMDC
  CDC  *pdrawDC = CDC::FromHandle(lpDIS->hDC);
  CMemDC memDC(pdrawDC);
  CDC  *pDC = &memDC;
#else	
  CDC* pDC = CDC::FromHandle(lpDIS->hDC);
#endif

  CPen *pOldPen;
  BOOL bIsPressed  = (lpDIS->itemState & ODS_SELECTED);
  BOOL bIsFocused  = (lpDIS->itemState & ODS_FOCUS);
  BOOL bIsDisabled = (lpDIS->itemState & ODS_DISABLED);

  BOOL bBtState  = bIsPressed || m_bPinned;

  CRect itemRect = lpDIS->rcItem;

  pDC->SetBkMode(TRANSPARENT);

  if (m_bIsFlat == FALSE)
  {
    if (bIsFocused || (GetDefault() == TRUE))
    {
      CBrush br(RGB(0,0,0));  
      pDC->FrameRect(&itemRect, &br);
      itemRect.DeflateRect(1, 1);
    }
  }

  // Prepare draw... paint button's area with background color
  COLORREF bgColor;
//  if ((m_MouseOnButton == TRUE) || (bIsPressed))
  if ((m_MouseOnButton == TRUE) || (bBtState))
    bgColor = GetActiveBgColor();
  else
    bgColor = GetInactiveBgColor();

	CBrush br(bgColor);
	// Draw transparent?
	if (m_bDrawTransparent == TRUE)
	{
		PaintBk(pDC);
	}
	else
	{
		pDC->FillRect(&itemRect, &br);
	}

	// Disegno lo sfondo del bottone
//CBrush br(GetSysColor(COLOR_BTNFACE));  
//pDC->FillRect(&itemRect, &br);

  // Draw pressed button
//  if (bIsPressed)
  if (bBtState)
  {
    if (m_bIsFlat == TRUE)
    {
      if (m_bDrawBorder == TRUE)
      {
		  pDC->Draw3dRect(itemRect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
/*
	    CPen penBtnHiLight(PS_SOLID, 0, GetSysColor(COLOR_BTNHILIGHT)); // Bianco
        CPen penBtnShadow(PS_SOLID, 0, GetSysColor(COLOR_BTNSHADOW));   // Grigio scuro

        // Disegno i bordi a sinistra e in alto
        // Dark gray line
        pOldPen = pDC->SelectObject(&penBtnShadow);
        pDC->MoveTo(itemRect.left, itemRect.bottom-1);
        pDC->LineTo(itemRect.left, itemRect.top);
        pDC->LineTo(itemRect.right, itemRect.top);
        // Disegno i bordi a destra e in basso
        // White line
        pDC->SelectObject(penBtnHiLight);
        pDC->MoveTo(itemRect.left, itemRect.bottom-1);
        pDC->LineTo(itemRect.right-1, itemRect.bottom-1);
        pDC->LineTo(itemRect.right-1, itemRect.top-1);
        //
        pDC->SelectObject(pOldPen);
*/
      }
    }
    else    
    {
      CBrush brBtnShadow(GetSysColor(COLOR_BTNSHADOW));
      pDC->FrameRect(&itemRect, &brBtnShadow);
    }
  }
  else // ...else draw non pressed button
  {
    CPen penBtnHiLight(PS_SOLID, 0, GetSysColor(COLOR_BTNHILIGHT)); // White
    CPen pen3DLight(PS_SOLID, 0, GetSysColor(COLOR_3DLIGHT));       // Light gray
    CPen penBtnShadow(PS_SOLID, 0, GetSysColor(COLOR_BTNSHADOW));   // Dark gray
    CPen pen3DDKShadow(PS_SOLID, 0, GetSysColor(COLOR_3DDKSHADOW)); // Black

    if (m_bIsFlat == TRUE)
    {
      if (m_MouseOnButton == TRUE && m_bDrawBorder == TRUE)
      {
		  pDC->Draw3dRect(itemRect, ::GetSysColor(COLOR_BTNHILIGHT), ::GetSysColor(COLOR_BTNSHADOW));
/*
  	    // Disegno i bordi a sinistra e in alto
        // White line
        pOldPen = pDC->SelectObject(&penBtnHiLight);
        pDC->MoveTo(itemRect.left, itemRect.bottom-1);
        pDC->LineTo(itemRect.left, itemRect.top);
        pDC->LineTo(itemRect.right, itemRect.top);
        // Disegno i bordi a destra e in basso
        // Dark gray line
        pDC->SelectObject(penBtnShadow);
        pDC->MoveTo(itemRect.left, itemRect.bottom-1);
        pDC->LineTo(itemRect.right-1, itemRect.bottom-1);
        pDC->LineTo(itemRect.right-1, itemRect.top-1);
        //
        pDC->SelectObject(pOldPen);
*/
      }
    }
    else
    {
      // Disegno i bordi a sinistra e in alto
      // White line
      pOldPen = pDC->SelectObject(&penBtnHiLight);
      pDC->MoveTo(itemRect.left, itemRect.bottom-1);
      pDC->LineTo(itemRect.left, itemRect.top);
      pDC->LineTo(itemRect.right, itemRect.top);
      // Light gray line
      pDC->SelectObject(pen3DLight);
      pDC->MoveTo(itemRect.left+1, itemRect.bottom-1);
      pDC->LineTo(itemRect.left+1, itemRect.top+1);
      pDC->LineTo(itemRect.right, itemRect.top+1);
      // Disegno i bordi a destra e in basso
      // Black line
      pDC->SelectObject(pen3DDKShadow);
      pDC->MoveTo(itemRect.left, itemRect.bottom-1);
      pDC->LineTo(itemRect.right-1, itemRect.bottom-1);
      pDC->LineTo(itemRect.right-1, itemRect.top-1);
      // Dark gray line
      pDC->SelectObject(penBtnShadow);
      pDC->MoveTo(itemRect.left+1, itemRect.bottom-2);
      pDC->LineTo(itemRect.right-2, itemRect.bottom-2);
      pDC->LineTo(itemRect.right-2, itemRect.top);
      //
      pDC->SelectObject(pOldPen);
    }
  }

  // Read the button's title
  CString sTitle;
  GetWindowText(sTitle);

  // If we don't want the title displayed
  if (m_bShowText == FALSE) sTitle.Empty();

  CRect captionRect = lpDIS->rcItem;

  // Draw the icon
  if (m_hIconIn != NULL)
  {
//    DrawTheIcon(pDC, &sTitle, &lpDIS->rcItem, &captionRect, bIsPressed, bIsDisabled);
	  DrawTheIcon(pDC, &sTitle, &lpDIS->rcItem, &captionRect, bBtState, bIsDisabled);
  }

  // Write the button title (if any)
  if (sTitle.IsEmpty() == FALSE)
  {
    // Disegno la caption del bottone
    // Se il bottone e' premuto muovo la captionRect di conseguenza
//    if (bIsPressed)
	if (bBtState)
      captionRect.OffsetRect(1, 1);
    
    // ONLY FOR DEBUG 
    // Evidenzia il rettangolo in cui verra' centrata la caption 
    //CBrush brBtnShadow(RGB(255, 0, 0));
    //pDC->FrameRect(&captionRect, &brBtnShadow);

#ifdef ST_USE_MEMDC
	// Get dialog's font
    CFont *pCurrentFont = GetFont(); 
    CFont *pOldFont = pDC->SelectObject(pCurrentFont);
#endif
//    if ((m_MouseOnButton == TRUE) || (bIsPressed)) 
    if ((m_MouseOnButton == TRUE) || (bBtState)) 
	{
      pDC->SetTextColor(GetActiveFgColor());
      pDC->SetBkColor(GetActiveBgColor());
    } 
	else 
	{
      pDC->SetTextColor(GetInactiveFgColor());
      pDC->SetBkColor(GetInactiveBgColor());
    }
    // Center text
    CRect centerRect = captionRect;
    pDC->DrawText(sTitle, -1, captionRect, DT_SINGLELINE|DT_CALCRECT);
    captionRect.OffsetRect((centerRect.Width() - captionRect.Width())/2, (centerRect.Height() - captionRect.Height())/2);
	/* RFU
    captionRect.OffsetRect(0, (centerRect.Height() - captionRect.Height())/2);
    captionRect.OffsetRect((centerRect.Width() - captionRect.Width())-4, (centerRect.Height() - captionRect.Height())/2);
	*/

	pDC->SetBkMode(TRANSPARENT);
    pDC->DrawState(captionRect.TopLeft(), captionRect.Size(), (LPCTSTR)sTitle, (bIsDisabled ? DSS_DISABLED : DSS_NORMAL), 
                   TRUE, 0, (CBrush*)NULL);
#ifdef ST_USE_MEMDC
    pDC->SelectObject(pOldFont);
#endif
  }

  if (m_bIsFlat == FALSE || (m_bIsFlat == TRUE && m_bDrawFlatFocus == TRUE))
  {
    // Draw the focus rect
    if (bIsFocused)
    {
      CRect focusRect = itemRect;
      focusRect.DeflateRect(3, 3);
      pDC->DrawFocusRect(&focusRect);
    }
  }
} // End of DrawItem

void CPushPin::OnClicked() 
{
	// TODO: Add your control notification handler code here
	m_bPinned = !m_bPinned;  //toggle the pinned option
	Invalidate();            //and force a redraw
}

#undef ST_USE_MEMDC
#undef ST_LIKEIE


