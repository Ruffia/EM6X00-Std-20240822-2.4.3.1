// xStaticText.cpp : implementation file
/* 15/03/2001
 * ing.davide.pizzolato@libero.it
 */
#include "stdafx.h"
#include "xStaticText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CxStaticText
CxStaticText::CxStaticText()
{
	m_ActualColor=m_TextColor=GetSysColor(COLOR_BTNTEXT); // default text color
	m_BlinkColor=GetSysColor(COLOR_3DLIGHT);		// default blink text color
	m_TimerID=0;
	m_BlinkCounter=-1;
	m_Style=SST_NORMAL;
}
/////////////////////////////////////////////////////////////////////////////
CxStaticText::~CxStaticText()
{
}
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CxStaticText, CStatic)
	//{{AFX_MSG_MAP(CxStaticText)
	ON_WM_PAINT()
	ON_WM_ENABLE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
void CxStaticText::OnPaint() 
{
	CPaintDC dc(this);				// device context for painting
	CString sCaption;
	GetWindowText(sCaption);		// get label text
	char* szCap=NULL;				// char buffer
	int iLen=sCaption.GetLength();	// buffer size
	szCap=(char*)malloc(2+iLen);	// allocate buffer
	if (szCap==NULL) return;		// allocation failure
	strcpy(szCap,sCaption);			// copy text to buffer
	szCap[iLen]=' ';				// ensure the last char is a space
	szCap[1+iLen]='\0';
	int iCharPos=0;					// current char position
	int iLastSpacePos=0;			// last space found;
	int i,nChar;
	int nLines=0;
	char* tokenpos;

	RECT rect;
	GetClientRect(&rect);			// get clip region
	dc.SetBkMode(TRANSPARENT);		// preserve background
	dc.SelectObject(GetStockObject(DEFAULT_GUI_FONT));	//get text font

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);			// get font metrics
	CSize te;						// storage for GetTextExtent

	while (iLastSpacePos<iLen){		//format text to wrap into clip rect
		i=iCharPos;
		while(1){
			tokenpos=strchr(&szCap[i],' ');
			if (tokenpos==NULL){
				if (i==iCharPos) iLastSpacePos=iLen;
				break;
			}
			i=tokenpos - szCap +1;
			te=dc.GetTextExtent(&szCap[iCharPos],tokenpos - &szCap[iCharPos]);
			if (te.cx<rect.right){
				iLastSpacePos=tokenpos - szCap;
			} else {
				break;
			}
		}
		nChar=iLastSpacePos-iCharPos;

		if (IsWindowEnabled()){			//Paint the text
			switch(m_Style){
			case SST_RAISED:
				dc.SetTextColor(GetSysColor(COLOR_3DHILIGHT));
				dc.ExtTextOut(-1,nLines-1,ETO_CLIPPED,&rect,&szCap[iCharPos],nChar,NULL);
				break;
			case SST_SUNKEN:
				dc.SetTextColor(GetSysColor(COLOR_3DSHADOW));
				dc.ExtTextOut(-1,nLines-1,ETO_CLIPPED,&rect,&szCap[iCharPos],nChar,NULL);
				break;
			}
			dc.SetTextColor(m_ActualColor);
			dc.ExtTextOut(0,nLines,ETO_CLIPPED,&rect,&szCap[iCharPos],nChar,NULL);
		} else {
			dc.SetTextColor(GetSysColor(COLOR_3DHILIGHT));
			dc.ExtTextOut(0,nLines,ETO_CLIPPED,&rect,&szCap[iCharPos],nChar,NULL);
			dc.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
			dc.ExtTextOut(-1,nLines-1,ETO_CLIPPED,&rect,&szCap[iCharPos],nChar,NULL);
		}

		iCharPos=iLastSpacePos+1;
		nLines+=tm.tmHeight;
	}
	free(szCap);
}
/////////////////////////////////////////////////////////////////////////////
void CxStaticText::OnEnable(BOOL bEnable)
/* called by the framework when the object state changes */
{
	// the background must be drawn by the object owner
	InvalidateParent();
	Invalidate(0);	//update screen
}
/////////////////////////////////////////////////////////////////////////////
COLORREF CxStaticText::SetTextColor(COLORREF new_color)
/* Set active text color; ex: m_static1.SetTextColor(RGB(0,0,255)); */
{
	COLORREF tmp_color=m_TextColor;
	m_ActualColor=m_TextColor=new_color;
	Invalidate(0);				//update screen
	return tmp_color;			//returns the previous color
}
/////////////////////////////////////////////////////////////////////////////
COLORREF CxStaticText::SetBlinkColor(COLORREF new_color)
/* Set blink text color; ex: m_static1.SetBlinkColor(RGB(0,255,0)); */
{
	COLORREF tmp_color=m_BlinkColor;
	m_BlinkColor=new_color;
	return tmp_color;			//returns the previous color
}
/////////////////////////////////////////////////////////////////////////////
void CxStaticText::SetBlink(BOOL bBlink, int blink_time, int blink_count)
{
	if (blink_count<0){
		m_BlinkCounter=0;
	} else {
		m_BlinkCounter=2*blink_count;
	}

	if(bBlink){
		if (m_TimerID==0) m_TimerID=SetTimer(1, blink_time,NULL);
	} else {
		if (m_TimerID!=0){
			KillTimer(m_TimerID);
			m_TimerID=0;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
void CxStaticText::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent==m_TimerID){
		if (m_BlinkCounter>0){
			m_BlinkCounter--;
			if (m_BlinkCounter==0){
				if (m_TimerID!=0){
					KillTimer(m_TimerID);
					m_TimerID=0;
				}
			}
		}
		if (m_ActualColor==m_TextColor)	m_ActualColor=m_BlinkColor;
		else	m_ActualColor=m_TextColor;
		Invalidate(0);
	}
	CStatic::OnTimer(nIDEvent);
}
/////////////////////////////////////////////////////////////////////////////
void CxStaticText::SetTextStyle(int style)
{	m_Style=style; }
/////////////////////////////////////////////////////////////////////////////
void CxStaticText::InvalidateParent()
{
	CWnd* pParent=GetParent();
	if (pParent){
		RECT rect;
		GetWindowRect(&rect);
		pParent->ScreenToClient(&rect);
		pParent->InvalidateRect(&rect,0);
	}
}
/////////////////////////////////////////////////////////////////////////////
void CxStaticText::SetWindowText(LPCTSTR lpszString)
{
	CStatic::SetWindowText(lpszString);
	Invalidate(0);
}
/////////////////////////////////////////////////////////////////////////////
