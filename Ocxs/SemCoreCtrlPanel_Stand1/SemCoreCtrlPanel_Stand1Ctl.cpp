// SemCoreCtrlPanel_Stand1Ctl.cpp : Implementation of the CSemCoreCtrlPanel_Stand1Ctrl ActiveX Control class.
//
// Last Modify : 2008.12.03

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand1.h"
#include "SemCoreCtrlPanel_Stand1Ctl.h"
#include "SemCoreCtrlPanel_Stand1Ppg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

const double dfHScrollbarRectangle_Left		= 0.0;
const double dfHScrollbarRectangle_Right	= 1.0;
const double dfHScrollbarRectangle_Top		= 0.5;	//0.3;
const double dfHScrollbarRectangle_Bottom	= 1.0;	//0.7;

const double dfVScrollbarRectangle_Left		= 0.1;
const double dfVScrollbarRectangle_Right	= 0.9;
const double dfVScrollbarRectangle_Top		= 0.0;
const double dfVScrollbarRectangle_Bottom	= 1.0;

const double dfSpin2ButtonRectangle_Left	= 0.5;
const double dfSpin2ButtonRectangle_Right	= 0.75;
const double dfSpin2ButtonRectangle_Top		= 0.0;
const double dfSpin2ButtonRectangle_Bottom	= 1.0;

IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_Stand1Ctrl, COleControl)


long GetNewPos( long lValue, int nBlock )
{
	long lPos = lValue;
	switch( nBlock )
	{
	case 0:
		lPos += 1;
		break;
	case 1:
		if( lValue + 10 >= 100 )
		{
			if( lValue < 100 )
				lPos = 100;
			else
				lPos += 5;
		}
		else
			lPos += 10;
		break;
	case 2:
		lPos -= 1;
		break;
	case 3:
		if( lValue -10 < 100 )
		{
			if( lValue > 105 )
				lPos -= 5;
			else if( lValue > 100 )
				lPos = 100;
			else
				lPos -= 10;
		}
		else
			lPos -= 5;
		break;
	}
	return lPos;
}

/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_Stand1Ctrl, COleControl)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_Stand1Ctrl)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_NOTIFY(UDN_DELTAPOS, nIDSpin1Ctrl, OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, nIDSpin2Ctrl, OnDeltaposSpin2)
	ON_BN_CLICKED(nIDMultiply10Ctrl, OnButtonMultiply10)
	ON_BN_CLICKED(nIDDivide10Ctrl, OnButtonDivide10)
	ON_CBN_SELCHANGE(nIDComboCtrl, OnSelchangeCombo)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_COMMAND_RANGE(IDM_SEMCORECTRLPANEL_STAND9_MAG_1, IDM_SEMCORECTRLPANEL_STAND9_MAG_8, OnFastChanged)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CSemCoreCtrlPanel_Stand1Ctrl, COleControl)
	//{{AFX_DISPATCH_MAP(CSemCoreCtrlPanel_Stand1Ctrl)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand1Ctrl, "CaptionVisible", m_bCaptionVisible, OnCaptionVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand1Ctrl, "DebugInfoVisible", m_bDebugInfoVisible, OnDebugInfoVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand1Ctrl, "XMinString", m_csXMinString, OnXMinStringChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand1Ctrl, "XMaxString", m_csXMaxString, OnXMaxStringChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand1Ctrl, "ControlStyle", m_nControlStyle, OnControlStyleChanged, VT_I2)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand1Ctrl, "Bind", Bind, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand1Ctrl, "Enable", Enable, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand1Ctrl, "SetRange", SetRange, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand1Ctrl, "SetPos", SetPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand1Ctrl, "SyncPos", SyncPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand1Ctrl, "SyncText", SyncText, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand1Ctrl, "SyncString", SyncString, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_STOCKPROP_BACKCOLOR()
	DISP_STOCKPROP_FORECOLOR()
	DISP_STOCKPROP_FONT()
	DISP_STOCKPROP_CAPTION()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CSemCoreCtrlPanel_Stand1Ctrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CSemCoreCtrlPanel_Stand1Ctrl, COleControl)
	//{{AFX_EVENT_MAP(CSemCoreCtrlPanel_Stand1Ctrl)
	EVENT_CUSTOM("ReportPos", FireReportPos, VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT)
	EVENT_CUSTOM("ReportAction", FireReportAction, VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSemCoreCtrlPanel_Stand1Ctrl, 3)
	PROPPAGEID(CSemCoreCtrlPanel_Stand1PropPage::guid)
	PROPPAGEID(CLSID_CColorPropPage)
	PROPPAGEID(CLSID_CFontPropPage)
END_PROPPAGEIDS(CSemCoreCtrlPanel_Stand1Ctrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_Stand1Ctrl, "SEMCORECTRLPANELSTAND1.SemCoreCtrlPanelStand1Ctrl.1",
	0xa34817ad, 0x1937, 0x4d50, 0xab, 0xc0, 0x7, 0x20, 0x90, 0xd2, 0xd1, 0xbf)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CSemCoreCtrlPanel_Stand1Ctrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DSemCoreCtrlPanel_Stand1 =
{ 0x997ed4b7, 0xaefa, 0x46e4, { 0x95, 0x59, 0x7e, 0x8b, 0xea, 0xd2, 0xcd, 0x4b } };
const IID BASED_CODE IID_DSemCoreCtrlPanel_Stand1Events =
{ 0xc696cdd4, 0x4806, 0x437d, { 0x9e, 0x74, 0x85, 0x4a, 0x3d, 0x8d, 0x96, 0x44 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwSemCoreCtrlPanel_Stand1OleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSemCoreCtrlPanel_Stand1Ctrl, IDS_SEMCORECTRLPANEL_STAND1, _dwSemCoreCtrlPanel_Stand1OleMisc)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand1Ctrl::CSemCoreCtrlPanel_Stand1CtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_Stand1Ctrl

BOOL CSemCoreCtrlPanel_Stand1Ctrl::CSemCoreCtrlPanel_Stand1CtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.
	
	if (bRegister)
		return AfxOleRegisterControlClass(
		AfxGetInstanceHandle(),
		m_clsid,
		m_lpszProgID,
		IDS_SEMCORECTRLPANEL_STAND1,
		IDB_SEMCORECTRLPANEL_STAND1,
		afxRegApartmentThreading,
		_dwSemCoreCtrlPanel_Stand1OleMisc,
		_tlid,
		_wVerMajor,
		_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand1Ctrl::CSemCoreCtrlPanel_Stand1Ctrl - Constructor

CSemCoreCtrlPanel_Stand1Ctrl::CSemCoreCtrlPanel_Stand1Ctrl()
{
	InitializeIIDs(&IID_DSemCoreCtrlPanel_Stand1, &IID_DSemCoreCtrlPanel_Stand1Events);
	
	// TODO: Initialize your control's instance data here.
	m_scfCtrlX.varMin.lVal = 0;
	m_scfCtrlX.varMax.lVal = 255;
	m_scfCtrlX.varPos.lVal = 0;
	
	m_scfCtrlY = m_scfCtrlX;
	
	m_strX = _T("-X-");
	m_strY = _T("-Y-");

	m_blCtrlEnableX = TRUE;
	
	m_varPanelSerialNumber.lVal = -1;
	m_nFastTotal = 0;
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand1Ctrl::~CSemCoreCtrlPanel_Stand1Ctrl - Destructor

CSemCoreCtrlPanel_Stand1Ctrl::~CSemCoreCtrlPanel_Stand1Ctrl()
{
	// TODO: Cleanup your control's instance data here.
	OnSizeFont( NULL );
	if( m_bmpMul10.m_hObject != NULL )
		m_bmpMul10.DeleteObject();
	if( m_bmpDiv10.m_hObject != NULL )
		m_bmpDiv10.DeleteObject();
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand1Ctrl::OnDraw - Drawing function

void CSemCoreCtrlPanel_Stand1Ctrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	switch (m_nControlStyle)
	{
	case 0:
		DrawHScroll(pdc, rcBounds, rcInvalid);
		break;
	case 1:
		DrawVScroll(pdc, rcBounds, rcInvalid);
		break;
	case 2:
		DrawBtnAndLabel(pdc, rcBounds, rcInvalid);
		break;
	}
}

void CSemCoreCtrlPanel_Stand1Ctrl::DrawHScroll(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE)));
	
	//////////////////////////////////////////////////////
	// Draw Scrollbar Rectangle
	CRect	rc = rcBounds;
	rc.left		= rcBounds.left +(long)((double)rcBounds.Width() * dfHScrollbarRectangle_Left);
	rc.right	= rcBounds.left +(long)((double)rcBounds.Width() * dfHScrollbarRectangle_Right);
	rc.top		= rcBounds.top  +(long)((double)rcBounds.Height() * dfHScrollbarRectangle_Top);
	rc.bottom	= rcBounds.top  +(long)((double)rcBounds.Height() * dfHScrollbarRectangle_Bottom);
	
	CBrush*		pOldBrush;
	CBrush		bkBrush(TranslateColor(GetBackColor()));
	CPen*		pOldPen;
	
	pOldBrush = pdc->SelectObject(&bkBrush);
	pOldPen = (CPen*)pdc->SelectStockObject(BLACK_PEN);
	pdc->Rectangle(rc);
	pdc->SelectObject(pOldPen);
	pdc->SelectObject(pOldBrush);
	//////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////
	//////////////////////////////////////////////////////
	CFont*		pOldFont;
	pdc->SetTextColor(TranslateColor(GetForeColor()));
	pdc->SetBkMode(TRANSPARENT);
	pOldFont = SelectStockFont(pdc);
	
	CString csTextOut, csTemp;
	
	//////////////////////////////////////////////////////
	// Draw Caption & Debug Info
	const CString& strCaption = InternalGetText();
	
	csTextOut.Empty();
	if (m_bCaptionVisible)
	{
		csTextOut = strCaption;
	}
	if (m_bDebugInfoVisible)
	{
		csTemp.Format(" [ID:%3Ld] [CTL:%3Ld]", m_varPanelSerialNumber.lVal, m_scfCtrlX.varPos.lVal);
		csTextOut += csTemp;
	}
	if (!csTextOut.IsEmpty())
	{
		pdc->SetTextAlign(TA_LEFT | TA_TOP);
		pdc->ExtTextOut(rcBounds.left, rcBounds.top,
			ETO_CLIPPED, rcBounds, csTextOut, csTextOut.GetLength(), NULL);
	}
	//////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////
	// Draw Value
	//csTemp.Format("[%3Ld]", m_scfCtrlX.varPos.lVal);
	//csTextOut = csTemp + m_strX;
	csTextOut = m_strX;
	
	pdc->SetTextAlign(TA_RIGHT | TA_TOP);
	pdc->ExtTextOut(rcBounds.right, rcBounds.top,
		ETO_CLIPPED, rcBounds, csTextOut, csTextOut.GetLength(), NULL);
	//////////////////////////////////////////////////////
/*	
	//////////////////////////////////////////////////////
	// Draw XMin & XMax String
	csTextOut = m_csXMinString;
	pdc->SetTextAlign(TA_LEFT | TA_BOTTOM);
	pdc->ExtTextOut(rcBounds.left, rcBounds.bottom,
		ETO_CLIPPED, rcBounds, csTextOut, csTextOut.GetLength(), NULL);
	
	csTextOut = m_csXMaxString;
	pdc->SetTextAlign(TA_RIGHT | TA_BOTTOM);
	pdc->ExtTextOut(rcBounds.right, rcBounds.bottom,
		ETO_CLIPPED, rcBounds, csTextOut, csTextOut.GetLength(), NULL);
	//////////////////////////////////////////////////////
*/	
	pdc->SelectObject(pOldFont);
	//////////////////////////////////////////////////////
	//////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////
	// Calculate Expect-Invalid-Rect
	TEXTMETRIC	tm;
	GetStockTextMetrics(&tm);
	CSize sizeText = pdc->GetTextExtent(strCaption);
	m_ExpectInvalidRect			= rcBounds;
	//m_ExpectInvalidRect.left	= rcBounds.right - sizeText.cx - tm.tmMaxCharWidth;
	m_ExpectInvalidRect.left	= rcBounds.left + sizeText.cx;
	m_ExpectInvalidRect.bottom	= rcBounds.top + sizeText.cy;
	//////////////////////////////////////////////////////
}

void CSemCoreCtrlPanel_Stand1Ctrl::DrawVScroll(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE)));
	
	//////////////////////////////////////////////////////
	// Calculate TextMetrics
	TEXTMETRIC	tm;
	GetStockTextMetrics(&tm);
	//////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////
	// Draw Scrollbar Rectangle
	CRect	rc = rcBounds;
	rc.left		= rcBounds.left +(long)((double)rcBounds.Width() * dfVScrollbarRectangle_Left);
	rc.right	= rcBounds.left +(long)((double)rcBounds.Width() * dfVScrollbarRectangle_Right);
	rc.top		= rcBounds.top  +(long)((double)rcBounds.Height() * dfVScrollbarRectangle_Top);
	rc.bottom	= rcBounds.top  +(long)((double)rcBounds.Height() * dfVScrollbarRectangle_Bottom);
	
	rc.top		+= 2 * tm.tmHeight;
	rc.bottom	-= tm.tmHeight;
	
	if (m_bCaptionVisible)
		rc.bottom	-= tm.tmHeight;
	if (m_bDebugInfoVisible)
		rc.bottom	-= tm.tmHeight;
	
	CBrush*		pOldBrush;
	CBrush		bkBrush(TranslateColor(GetBackColor()));
	CPen*		pOldPen;
	
	pOldBrush = pdc->SelectObject(&bkBrush);
	pOldPen = (CPen*)pdc->SelectStockObject(BLACK_PEN);
	pdc->Rectangle(rc);
	pdc->SelectObject(pOldPen);
	pdc->SelectObject(pOldBrush);
	//////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////
	CFont*		pOldFont;
	pdc->SetTextColor(TranslateColor(GetForeColor()));
	pdc->SetBkMode(TRANSPARENT);
	pOldFont = SelectStockFont(pdc);
	//////////////////////////////////////////////////////
	
	CString csTextOut, csTemp;
	int		nCenter = (rcBounds.left + rcBounds.right)/2;
	rc = rcBounds;
	//////////////////////////////////////////////////////
	// Draw Caption & Debug Info
	const CString& strCaption = InternalGetText();
	
	if (m_bDebugInfoVisible)
	{
		csTemp.Format("[ID:%3Ld] [CTL:%3Ld]", m_varPanelSerialNumber.lVal, m_scfCtrlX.varPos.lVal);
		csTextOut = csTemp;
		
		pdc->SetTextAlign(TA_CENTER | TA_BOTTOM);
		pdc->ExtTextOut(nCenter, rc.bottom,
			ETO_CLIPPED, rcBounds, csTextOut, csTextOut.GetLength(), NULL);
		rc.bottom	-= tm.tmHeight;
	}
	if (m_bCaptionVisible)
	{
		csTextOut = strCaption;
		
		pdc->SetTextAlign(TA_CENTER | TA_BOTTOM);
		pdc->ExtTextOut(nCenter, rc.bottom,
			ETO_CLIPPED, rcBounds, csTextOut, csTextOut.GetLength(), NULL);
		rc.bottom	-= tm.tmHeight;
	}
	//////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////
	// Draw Value
	//csTemp.Format("[%3Ld]", m_scfCtrlX.varPos.lVal);
	//csTextOut = csTemp + m_strX;
	csTextOut = m_strX;
	
	pdc->SetTextAlign(TA_CENTER | TA_TOP);
	pdc->ExtTextOut(nCenter, rc.top,
		ETO_CLIPPED, rcBounds, csTextOut, csTextOut.GetLength(), NULL);
	rc.top		+= tm.tmHeight;
	//////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////
	// Draw XMin & XMax String
	csTextOut = m_csXMinString;
	pdc->SetTextAlign(TA_CENTER | TA_BOTTOM);
	pdc->ExtTextOut(nCenter, rc.bottom,
		ETO_CLIPPED, rcBounds, csTextOut, csTextOut.GetLength(), NULL);
	rc.bottom	-= tm.tmHeight;
	
	csTextOut = m_csXMaxString;
	pdc->SetTextAlign(TA_CENTER | TA_TOP);
	pdc->ExtTextOut(nCenter, rc.top,
		ETO_CLIPPED, rcBounds, csTextOut, csTextOut.GetLength(), NULL);
	rc.top		+= tm.tmHeight;
	//////////////////////////////////////////////////////
	
	pdc->SelectObject(pOldFont);
	//////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////
	// Calculate Expect-Invalid-Rect
	m_ExpectInvalidRect			= rcBounds;
	m_ExpectInvalidRect.bottom	= rcBounds.top + tm.tmHeight;
	//////////////////////////////////////////////////////
}

void CSemCoreCtrlPanel_Stand1Ctrl::DrawBtnAndLabel(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	/////////////////////////////////////////////////////////
	if (m_ctrlSpin1.m_hWnd != NULL)
		return;

	pdc->FillRect(rcBounds, CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE)));

	/////////////////////////////////////////////////////////
	CString csTextOut;

	/////////////////////////////////////////////////////////
	CRect		rc = rcBounds;

	/////////////////////////////////////////////////////////
	CPen*		pOldPen;
	CBrush*		pOldBrush;
	CBrush*		pNewBrush = CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE));

	pOldPen		= (CPen*)pdc->SelectStockObject(BLACK_PEN);
	pOldBrush	= pdc->SelectObject(pNewBrush);

	/////////////////////////////////////////////////////////
	CFont*		pOldFont;
	pdc->SetTextColor(TranslateColor(GetSysColor(COLOR_BTNTEXT)));
	//pdc->SetBkColor(TranslateColor(GetSysColor(COLOR_3DFACE)));
	//pdc->SetTextColor(TranslateColor(GetForeColor()));
	//pdc->SetBkColor(TranslateColor(GetBackColor()));
	pdc->SetBkMode(TRANSPARENT);

	/////////////////////////////////////////////////////////
	TEXTMETRIC tm ;
	UINT	nValueFontHeight;
	UINT	nValueBaseline;
	UINT	nValueCenter;
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// Draw Static
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.left;
	rc.right	= rcBounds.left + (long)((double)rcBounds.Width() * dfSpin2ButtonRectangle_Left - 1);
	rc.top		= rcBounds.top;
	rc.bottom	= rcBounds.top  + (long)((double)rcBounds.Height() / 2 );
	
	/////////////////////////////////////////////////////////
	CBrush StaticBrush;
	StaticBrush.CreateSolidBrush( TranslateColor(GetBackColor()) );
	pdc->SelectObject( &StaticBrush );
	pdc->Rectangle(rc);
	pdc->SelectObject(pNewBrush);
	StaticBrush.DeleteObject();
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	csTextOut = GetDisplayString();

	if (!csTextOut.IsEmpty())
	{
		NONCLIENTMETRICS	nc;
		CFont				fontOnSize;
		ZeroMemory(&nc, sizeof(nc));
		nc.cbSize = sizeof(nc);
		SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &nc, 0);
		nc.lfMenuFont.lfHeight = rc.Height() - 8;
		fontOnSize.CreateFontIndirect(&nc.lfMenuFont);

		pOldFont = pdc->SelectObject( &fontOnSize );
		//////////////////////////////////////////////////

		pdc->GetTextMetrics(&tm) ;
		nValueFontHeight = tm.tmHeight ;
		//////////////////////////////////////////////////////
		// determine the location for the value within
		// the rectangele based on the font height
		nValueBaseline = rc.top + (int)( rc.Height()/2.0 + nValueFontHeight / 4.0 + 0.5 );
		nValueCenter   = rc.left + rc.Width()/2 ;

		COLORREF crColor = pdc->SetTextColor(TranslateColor(GetForeColor()));
		pdc->SetTextAlign(TA_CENTER | TA_BASELINE);
		pdc->TextOut(nValueCenter, nValueBaseline, csTextOut) ;
		pdc->SetTextColor( crColor );

		//////////////////////////////////////////////////
		pdc->SelectObject( pOldFont );
		fontOnSize.DeleteObject();
	}
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	//pdc->SetTextColor(TranslateColor(GetSysColor(COLOR_BTNTEXT)));
	//pdc->SetBkColor(TranslateColor(GetSysColor(COLOR_3DFACE)));
	//pdc->SetBkMode(TRANSPARENT);

	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////
	pOldFont = SelectStockFont(pdc);
	pdc->GetTextMetrics(&tm) ;
	nValueFontHeight = tm.tmHeight ;

	/////////////////////////////////////////////////////////
	// Draw Multiply10			↑↓∧∨⑩▲▼△▽
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.left;
	rc.right	= rcBounds.left + (long)( ((double)rcBounds.Width() * dfSpin2ButtonRectangle_Left - 1)/2.0 + 1.5 );
	rc.top		= rcBounds.bottom - (long)((double)rcBounds.Height() / 2 );
	rc.bottom	= rcBounds.bottom;
	
	pdc->Rectangle(rc);

	csTextOut = _T("▲");
	//////////////////////////////////////////////////////
	// determine the location for the value within
	// the rectangele based on the font height
	nValueBaseline = rc.top + (int)( rc.Height()/2.0 + nValueFontHeight / 4.0 + 1.5 );
	nValueCenter   = rc.left + rc.Width()/2 ;

	if (!csTextOut.IsEmpty())
	{
		pdc->SetTextAlign(TA_CENTER | TA_BASELINE);
		pdc->TextOut(nValueCenter, nValueBaseline, csTextOut) ;
	}
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// Draw Divide10			↑↓∧∨⑩▲▼△▽
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.left + (long)( ((double)rcBounds.Width() * dfSpin2ButtonRectangle_Left - 1)/2.0 + 0.5 );
	rc.right	= rcBounds.left + (long)((double)rcBounds.Width() * dfSpin2ButtonRectangle_Left - 1);
	rc.top		= rcBounds.bottom - (long)((double)rcBounds.Height() / 2 );
	rc.bottom	= rcBounds.bottom;
	
	pdc->Rectangle(rc);

	csTextOut = _T("▼");
	//////////////////////////////////////////////////////
	// determine the location for the value within
	// the rectangele based on the font height
	nValueBaseline = rc.top + (int)( rc.Height()/2.0 + nValueFontHeight / 3.0 + 1.5 );
	nValueCenter   = rc.left + rc.Width()/2 ;

	if (!csTextOut.IsEmpty())
	{
		pdc->SetTextAlign(TA_CENTER | TA_BASELINE);
		pdc->TextOut(nValueCenter, nValueBaseline, csTextOut) ;
	}
	/////////////////////////////////////////////////////////

	CPoint	p0, p1;
	/////////////////////////////////////////////////////////
	// Draw Spin1
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.left + (long)((double)rcBounds.Width() * dfSpin2ButtonRectangle_Left);
	rc.right	= rcBounds.left + (long)((double)rcBounds.Width() * dfSpin2ButtonRectangle_Right);
	rc.top		= rcBounds.top  + (long)((double)rcBounds.Height() * dfSpin2ButtonRectangle_Top);
	rc.bottom	= rcBounds.top  + (long)((double)rcBounds.Height() * dfSpin2ButtonRectangle_Bottom);
	
	pdc->Rectangle(rc);

	p0.x = rc.left + 2;
	p0.y = rc.top + (int)(rc.Height() / 2.0 + 0.5);
	p1.x = rc.right - 2;
	p1.y = p0.y;

	pdc->MoveTo( p0 );
	pdc->LineTo( p1 );

	csTextOut = _T("▲");
	//////////////////////////////////////////////////////
	// determine the location for the value within
	// the rectangele based on the font height
	nValueBaseline = rc.top + (int)( rc.Height()/4.0 + nValueFontHeight / 4.0 + 1.5 );
	nValueCenter   = rc.left + rc.Width()/2;

	if (!csTextOut.IsEmpty())
	{
		pdc->SetTextAlign(TA_CENTER | TA_BASELINE);
		pdc->TextOut(nValueCenter, nValueBaseline, csTextOut) ;
	}
	/////////////////////////////////////////////////////////

	csTextOut = _T("▼");
	//////////////////////////////////////////////////////
	// determine the location for the value within
	// the rectangele based on the font height
	nValueBaseline = rc.top + (int)( rc.Height()*3 /4.0 + nValueFontHeight / 3.0 + 1.5 );
	nValueCenter   = rc.right - rc.Width()/2;

	if (!csTextOut.IsEmpty())
	{
		pdc->SetTextAlign(TA_CENTER | TA_BASELINE);
		pdc->TextOut(nValueCenter, nValueBaseline, csTextOut) ;
	}
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// Draw Spin2
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.right - (long)((double)rcBounds.Width() * (1 - dfSpin2ButtonRectangle_Right));
	rc.right	= rcBounds.right;
	rc.top		= rcBounds.top  + (long)((double)rcBounds.Height() * dfSpin2ButtonRectangle_Top);
	rc.bottom	= rcBounds.top  + (long)((double)rcBounds.Height() * dfSpin2ButtonRectangle_Bottom);
	
	pdc->Rectangle(rc);

	p0.x = rc.left + 2;
	p0.y = rc.top + (int)(rc.Height() / 2.0 + 0.5);
	p1.x = rc.right - 2;
	p1.y = p0.y;

	pdc->MoveTo( p0 );
	pdc->LineTo( p1 );

	csTextOut = _T("▲");
	//////////////////////////////////////////////////////
	// determine the location for the value within
	// the rectangele based on the font height
	nValueBaseline = rc.top + (int)( rc.Height()/4.0 + nValueFontHeight / 4.0 + 1.5 );
	nValueCenter   = rc.left + rc.Width()/2;

	if (!csTextOut.IsEmpty())
	{
		pdc->SetTextAlign(TA_CENTER | TA_BASELINE);
		pdc->TextOut(nValueCenter, nValueBaseline, csTextOut) ;
	}
	/////////////////////////////////////////////////////////

	csTextOut = _T("▼");
	//////////////////////////////////////////////////////
	// determine the location for the value within
	// the rectangele based on the font height
	nValueBaseline = rc.top + (int)( rc.Height()*3 /4.0 + nValueFontHeight / 3.0 + 1.5 );
	nValueCenter   = rc.right - rc.Width()/2;

	if (!csTextOut.IsEmpty())
	{
		pdc->SetTextAlign(TA_CENTER | TA_BASELINE);
		pdc->TextOut(nValueCenter, nValueBaseline, csTextOut) ;
	}
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	pdc->SelectObject(pOldFont);
	pdc->SelectObject(pOldPen);
	pdc->SelectObject(pOldBrush);
	/////////////////////////////////////////////////////////
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand1Ctrl::DoPropExchange - Persistence support

void CSemCoreCtrlPanel_Stand1Ctrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);
	
	// TODO: Call PX_ functions for each persistent custom property.
	if (pPX->GetVersion() ==(DWORD)MAKELONG(_wVerMinor, _wVerMajor))
	{
		PX_Bool(pPX, _T("CaptionVisible"), m_bCaptionVisible, TRUE);
		PX_Bool(pPX, _T("DebugInfoVisible"), m_bDebugInfoVisible, TRUE);
		PX_String(pPX, _T("XMinString"), m_csXMinString, _T(""));
		PX_String(pPX, _T("XMaxString"), m_csXMaxString, _T(""));
		PX_Short(pPX, _T("ControlStyle"), m_nControlStyle, 0);
	}
	else if (pPX->IsLoading())
	{
		// Skip over persistent data
		BOOL bDummy;
		short nDummy;
		CString strDummy;
		PX_Bool(pPX, _T("CaptionVisible"), bDummy, FALSE);
		PX_Bool(pPX, _T("DebugInfoVisible"), bDummy, FALSE);
		PX_String(pPX, _T("XMinString"), strDummy, _T(""));
		PX_String(pPX, _T("XMaxString"), strDummy, _T(""));
		PX_Short(pPX, _T("ControlStyle"), nDummy, 0);
		
		// Force property values to these defaults
		m_bCaptionVisible = TRUE;
		m_bDebugInfoVisible = TRUE;
		m_csXMinString = _T("XMin");
		m_csXMaxString = _T("XMax");
		m_nControlStyle = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand1Ctrl::OnResetState - Reset control to default state

void CSemCoreCtrlPanel_Stand1Ctrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange
	
	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand1Ctrl::AboutBox - Display an "About" box to the user

void CSemCoreCtrlPanel_Stand1Ctrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_SEMCORECTRLPANEL_STAND1);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand1Ctrl message handlers

void CSemCoreCtrlPanel_Stand1Ctrl::OnCaptionVisibleChanged() 
{
	// TODO: Add notification handler code
	
	// force the control to redraw itself
	InvalidateControl();
	
	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand1Ctrl::OnDebugInfoVisibleChanged() 
{
	// TODO: Add notification handler code
	
	// force the control to redraw itself
	InvalidateControl();
	
	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand1Ctrl::OnXMinStringChanged() 
{
	// TODO: Add notification handler code
	
	// force the control to redraw itself
	InvalidateControl();
	
	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand1Ctrl::OnXMaxStringChanged() 
{
	// TODO: Add notification handler code
	
	// force the control to redraw itself
	InvalidateControl();
	
	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand1Ctrl::OnControlStyleChanged() 
{
	// TODO: Add notification handler code
	
	// force the control to redraw itself
	InvalidateControl();
	
	SetModifiedFlag();
}
/*
enum SemControlPanelClassID{
		SemControlPanelClass_NULL,						// 0
		SemControlPanelClass_HV,						// 01
		SemControlPanelClass_ScanShift,					// 02
		SemControlPanelClass_BeemAlignment,				// 03
		SemControlPanelClass_Brightness,				// 04
		SemControlPanelClass_Contrast,					// 05
		SemControlPanelClass_Stigmator,					// 06
		SemControlPanelClass_ObjectiveLens,				// 07
		SemControlPanelClass_CondenserLens,				// 08
		SemControlPanelClass_FilamentCurrent,			// 09
		SemControlPanelClass_GunCurrent,				// 10
		SemControlPanelClass_Magnifier,					// 11
		SemControlPanelClass_WorkDistance,				// 12
		SemControlPanelClass_ObjectiveLensReverse,		// 13
		SemControlPanelClass_CondenserLensReverse,		// 14
		SemControlPanelClass_AuxiliaryStigmation,		// 15
		SemControlPanelClass_DCondenserLens,			// 16
		SemControlPanelClass_ScanRotate,				// 17
		SemControlPanelClass_SACP,						// 18
		SemControlPanelClass_DFocus,					// 19
		SemControlPanelClass_DFocusSwitch,				// 20
		SemControlPanelClass_DObjectiveLens,			// 21
		SemControlPanelClass_DObjectiveLensSwitch,		// 22
		SemControlPanelClass_RobinsonDetectorSwitch,	// 23
		SemControlPanelClass_Signal2Brightness,			// 24
		SemControlPanelClass_Signal2Contrast,			// 25
		SemControlPanelClass_YAdjust,					// 26
		SemControlPanelClass_ImageReverse,				// 27
		SemControlPanelClass_GunCurrentAccelerate,		// 28
		SemControlPanelClass_HVAccelerate,				// 29
		SemControlPanelClass_Channel2AB,				// 30
		SemControlPanelClass_Channel2C,					// 31
		SemControlPanelClass_Channel2D,					// 32
		SemControlPanelClass_Channel2E,					// 33
		SemControlPanelClass_BseBySeDetectorSwitch,		// 34
		// CPS 3601
		SemControlPanelClass_CPS_HV,					// 35
		SemControlPanelClass_CPS_Bias,					// 36
		SemControlPanelClass_CPS_FilamentCurrent,		// 37
		// CPS 3601
		// Spellman EBM30N12/771
		SemControlPanelClass_SP_HV,						// 38
		SemControlPanelClass_SP_Bias,					// 39
		SemControlPanelClass_SP_Heater,					// 40
		SemControlPanelClass_SP_PMT,					// 41
		// Spellman EBM30N12/771
};
*/
void CSemCoreCtrlPanel_Stand1Ctrl::Bind(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber) 
{
	// TODO: Add your dispatch handler code here
	m_varPanelClassID		= ClassID;
	m_varPanelSerialNumber	= SerialNumber;

	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");
	char szBuf1[100], szBuf2[100], stabuff[20];
	if( ClassID.lVal == 35 || ClassID.lVal == 38 )		// SemControlPanelClass_CPS_HV || SemControlPanelClass_SP_HV
		sprintf(stabuff, "HV");
	else if( ClassID.lVal == 36 )						// SemControlPanelClass_CPS_Bias
		sprintf(stabuff, "Bias");
	else if( ClassID.lVal == 39 )						// SemControlPanelClass_SP_Bias
		sprintf(stabuff, "Beam");
	else if( ClassID.lVal == 37 || ClassID.lVal == 40 )	// SemControlPanelClass_CPS_FilamentCurrent || SemControlPanelClass_SP_Heater
		sprintf(stabuff, "Fila");
	else if( ClassID.lVal == 4 )						// SemControlPanelClass_Brightness
		sprintf(stabuff, "Brt");
	else if( ClassID.lVal == 24 )						// SemControlPanelClass_Signal2Brightness
		sprintf(stabuff, "Brt2");
	else if( ClassID.lVal == 5 || ClassID.lVal == 41 )	// SemControlPanelClass_Contrast || SemControlPanelClass_SP_PMT
		sprintf(stabuff, "Cst");

	char Separator = ',';
	int i = 0;
	if(GetPrivateProfileString("Fast", stabuff, "", szBuf1, 100, path) != 0)
	{
		do 
		{
			strcpy(szBuf2, szBuf1);
			if(strchr(szBuf2, Separator) != NULL)
			{
				strcpy(szBuf1, strchr(szBuf2, Separator)+1);
				*strchr(szBuf2, Separator) = '\0';
				m_strFast[i] = szBuf2;
				i++;
			}
			else
			{
				m_strFast[i] = szBuf2;
				m_nFastTotal = i +1;
				break;
			}
		} while (i<8 && szBuf2 != NULL);
	}
}

void CSemCoreCtrlPanel_Stand1Ctrl::Enable(const VARIANT FAR& Enabled, const VARIANT FAR& Flag) 
{
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;

	// TODO: Add your dispatch handler code here
	m_blCtrlEnableX = Enabled.boolVal;

	if (m_sbScrollBarX.m_hWnd != NULL)
		m_sbScrollBarX.EnableWindow( m_blCtrlEnableX );
	if (m_ctrlStatic.m_hWnd != NULL )
		m_ctrlStatic.EnableWindow( m_blCtrlEnableX );
	if (m_ctrlCombo.m_hWnd != NULL )
		m_ctrlCombo.EnableWindow( m_blCtrlEnableX );
	if (m_ctrlMultiply10.m_hWnd != NULL )
		m_ctrlMultiply10.EnableWindow( m_blCtrlEnableX );
	if (m_ctrlDivide10.m_hWnd != NULL )
		m_ctrlDivide10.EnableWindow( m_blCtrlEnableX );
	if (m_ctrlSpin1.m_hWnd != NULL )
	{
		m_ctrlSpin1.EnableWindow( m_blCtrlEnableX );
	}
}

void CSemCoreCtrlPanel_Stand1Ctrl::SetRange(const VARIANT FAR& Xmin, const VARIANT FAR& Xmax, const VARIANT FAR& Ymin, const VARIANT FAR& Ymax) 
{
	// TODO: Add your dispatch handler code here
	m_scfCtrlX.varMin = Xmin;
	m_scfCtrlX.varMax = Xmax;
	
	m_scfCtrlY.varMin = Ymin;
	m_scfCtrlY.varMax = Ymax;
	
	const long&	lXMin = m_scfCtrlX.varMin.lVal;
	const long&	lXMax = m_scfCtrlX.varMax.lVal;

	if (m_sbScrollBarX.m_hWnd != NULL)
	{
		m_sbScrollBarX.SetScrollRange(lXMin, lXMax);
		m_sbScrollBarX.EnableWindow( m_blCtrlEnableX );
	}
}

void CSemCoreCtrlPanel_Stand1Ctrl::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;
	
	const long&	lXMin = m_scfCtrlX.varMin.lVal;
	const long&	lXMax = m_scfCtrlX.varMax.lVal;
	long&	lXPos = m_scfCtrlX.varPos.lVal;
	
	long	lXPosNew = Xpos.lVal;
	
	if (lXPosNew < lXMin)
		lXPosNew = lXMin;
	if (lXPosNew > lXMax)
		lXPosNew = lXMax;
	
	if (lXPosNew != lXPos)
	{
		VARIANT	XPosNew, YPosNew, Flag;
		
		XPosNew.lVal	= lXPosNew;
		YPosNew.lVal	= 0;
		Flag.ulVal		= FlagCtrlX;
		
		SyncPos(XPosNew, YPosNew, Flag);
		FireReportPos(m_varPanelClassID, m_varPanelSerialNumber, XPosNew, YPosNew, Flag);	
	}
}

void CSemCoreCtrlPanel_Stand1Ctrl::SyncPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;
	
	long&	lXPos = m_scfCtrlX.varPos.lVal;
	lXPos = Xpos.lVal;

	if( m_nControlStyle == 2 )
	{
		CString csTextOut, csTemp;

		/////////////////////////////////////////////////////////
		csTextOut = GetDisplayString();

		/////////////////////////////////////////////////////////
		if (m_ctrlStatic.m_hWnd != NULL)
		{
			m_ctrlStatic.SetWindowText( csTextOut );
		}

	//	InvalidateControl();
	}
	else
	{
		if (m_nControlStyle == 0)
		{
			if (m_sbScrollBarX.m_hWnd != NULL)
				m_sbScrollBarX.SetScrollPos(lXPos);
		}
		else
		{
			long lScrollPos = m_scfCtrlX.varMax.lVal + m_scfCtrlX.varMin.lVal - lXPos;

			if (m_sbScrollBarX.m_hWnd != NULL)
				m_sbScrollBarX.SetScrollPos(lXPos);
		}
		
		InvalidateControl((LPCRECT)m_ExpectInvalidRect);
	}
}

void CSemCoreCtrlPanel_Stand1Ctrl::SyncText(LPCTSTR strX, LPCTSTR strY, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;
	
	m_strX = _T(strX);
	
	// m_strY;

	if( m_nControlStyle == 2 )
	{
		/////////////////////////////////////////////////////////
		CString csTextOut = GetDisplayString();

		/////////////////////////////////////////////////////////
		if (m_ctrlStatic.m_hWnd != NULL)
		{
			m_ctrlStatic.SetWindowText( csTextOut );
		}

		// force the control to redraw itself
	//	InvalidateControl();
	}
	else
	{
		if (m_sbScrollBarX.m_hWnd != NULL)
			m_sbScrollBarX.EnableWindow( m_blCtrlEnableX );
		
		// force the control to redraw itself
		InvalidateControl();
	}
}

void CSemCoreCtrlPanel_Stand1Ctrl::SyncString(const VARIANT FAR& ResourceID, const VARIANT FAR& ResourceString) 
{
	// TODO: Add your dispatch handler code here
	switch( ResourceID.intVal )
	{
	case	0:
		{
			CString str(ResourceString.bstrVal);
			SetWindowText( str );
		}
		break;
	default:
		break;
	}
}

int CSemCoreCtrlPanel_Stand1Ctrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CRect rc(0, 0, lpCreateStruct->cx, lpCreateStruct->cy);
	
	switch (m_nControlStyle )
	{
	case 0:
		CreateScrollBarH(rc);
		break;
	case 1:
		CreateScrollBarV(rc);
		break;
	case 2:
		CreateButtonAndLabel(rc);
		break;
	}
	
	return 0;
}

BOOL CSemCoreCtrlPanel_Stand1Ctrl::CreateScrollBarH(const CRect &rcBounds)
{
	if (m_sbScrollBarX.m_hWnd != NULL)
		return FALSE;
	
	DWORD	dwStyle = WS_CHILD | WS_VISIBLE | SBS_HORZ | SBS_TOPALIGN;
	RECT	rc;
	
	rc.left		= rcBounds.left +(long)((double)rcBounds.Width() * dfHScrollbarRectangle_Left);
	rc.right	= rcBounds.left +(long)((double)rcBounds.Width() * dfHScrollbarRectangle_Right);
	rc.top		= rcBounds.top  +(long)((double)rcBounds.Height() * dfHScrollbarRectangle_Top);
	rc.bottom	= rcBounds.top  +(long)((double)rcBounds.Height() * dfHScrollbarRectangle_Bottom);
	
	m_sbScrollBarX.Create(dwStyle, rc, this, nIDScrollBarX);
	m_sbScrollBarX.SetScrollRange(m_scfCtrlX.varMin.lVal, m_scfCtrlX.varMax.lVal);
	m_sbScrollBarX.SetScrollPos(m_scfCtrlX.varPos.lVal);
	m_sbScrollBarX.SetWindowPos(NULL,
		rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
	m_sbScrollBarX.ShowWindow(SW_SHOW);
	
	return TRUE;
}

BOOL CSemCoreCtrlPanel_Stand1Ctrl::CreateScrollBarV(const CRect &rcBounds)
{
	if (m_sbScrollBarX.m_hWnd != NULL)
		return FALSE;
	
	DWORD	dwStyle = WS_CHILD | WS_VISIBLE | SBS_VERT | SBS_RIGHTALIGN;
	RECT	rc;
	
	//////////////////////////////////////////////////////
	// Calculate TextMetrics
	TEXTMETRIC	tm;
	GetStockTextMetrics(&tm);
	//////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////
	// Draw Scrollbar Rectangle
	rc.left		= rcBounds.left +(long)((double)rcBounds.Width() * dfVScrollbarRectangle_Left);
	rc.right	= rcBounds.left +(long)((double)rcBounds.Width() * dfVScrollbarRectangle_Right);
	rc.top		= rcBounds.top  +(long)((double)rcBounds.Height() * dfVScrollbarRectangle_Top);
	rc.bottom	= rcBounds.top  +(long)((double)rcBounds.Height() * dfVScrollbarRectangle_Bottom);
	
	rc.top		+= 2 * tm.tmHeight;
	rc.bottom	-= tm.tmHeight;
	
	if (m_bCaptionVisible)
		rc.bottom	-= tm.tmHeight;
	if (m_bDebugInfoVisible)
		rc.bottom	-= tm.tmHeight;

	long lScrollPos = m_scfCtrlX.varMax.lVal + m_scfCtrlX.varMin.lVal - m_scfCtrlX.varPos.lVal;
	
	m_sbScrollBarX.Create(dwStyle, rc, this, nIDScrollBarX);
	m_sbScrollBarX.SetScrollRange(m_scfCtrlX.varMin.lVal, m_scfCtrlX.varMax.lVal);
	m_sbScrollBarX.SetScrollPos(lScrollPos);
	m_sbScrollBarX.SetWindowPos(NULL,
		rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
	m_sbScrollBarX.ShowWindow(SW_SHOW);
	
	return TRUE;
}

BOOL CSemCoreCtrlPanel_Stand1Ctrl::CreateButtonAndLabel(const CRect &rcBounds)
{
	if( m_ctrlSpin1.m_hWnd != NULL )
		return FALSE;

	CRect rc = rcBounds;
	/////////////////////////////////////////////////////////
	CString csTextOut;
	DWORD	dwStyle, dwExStyle;
	
	/////////////////////////////////////////////////////////
	// Create Static
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.left + (long)((double)rcBounds.Width() / 7 );
	rc.right	= rcBounds.right - (long)((double)rcBounds.Width() / 7 );
/*	rc.top		= rcBounds.top  +(long)((double)rcBounds.Height() * dfVScrollbarRectangle_Top);
//	rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 6 );
	rc.bottom	= rc.top + (long)((double)rcBounds.Height() / 3 );
*/	rc.top		= rcBounds.top;
	rc.bottom	= rcBounds.bottom;
	/////////////////////////////////////////////////////////
	dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE; //
	dwExStyle = WS_EX_STATICEDGE;
	
	/////////////////////////////////////////////////////////
	csTextOut = GetDisplayString();

	/////////////////////////////////////////////////////////
	m_ctrlStatic.CreateEx(dwExStyle, _T("STATIC"), csTextOut, dwStyle, rc, this, nIDStaticCtrl);
	if (m_ctrlStatic.m_hWnd != NULL)
	{
		m_ctrlStatic.SetWindowPos(NULL,	rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
//		OnSizeFont( &m_ctrlStatic );
		m_ctrlStatic.SetTextColor(TranslateColor(GetForeColor()));
		m_ctrlStatic.SetBkColor(TranslateColor(GetBackColor()));
	}

/*	/////////////////////////////////////////////////////////
	// Create ComboBox
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.left + (long)((double)rcBounds.Width() / 4 );
	rc.right	= rcBounds.right - (long)((double)rcBounds.Width() / 4 );
	rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 2 );
	rc.bottom	= rc.top + (long)((double)rcBounds.Height() );
	/////////////////////////////////////////////////////////
	dwStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_SORT;
	dwExStyle = WS_EX_STATICEDGE;
	
	/////////////////////////////////////////////////////////
	csTextOut = GetDisplayString();

	/////////////////////////////////////////////////////////
	m_ctrlCombo.CreateEx(dwExStyle, _T("COMBOBOX"), NULL, dwStyle, rc, this, nIDComboCtrl);
	if (m_ctrlCombo.m_hWnd != NULL)
	{
		m_ctrlCombo.SetWindowPos(NULL,	rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height()+80, SWP_NOZORDER);
		m_ctrlCombo.AddString( "0" );
		m_ctrlCombo.AddString( "10" );
		m_ctrlCombo.AddString( "20" );
		m_ctrlCombo.AddString( "30" );
		m_ctrlCombo.SetCurSel( 0 );
	}
*/
	/////////////////////////////////////////////////////////
	// Create Multiply10			↑↓∧∨⑩▲▼△▽
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.left + 2;
	rc.right	= rcBounds.left + (long)((double)rcBounds.Width() / 7 - 2 );
	rc.top		= rcBounds.top  +(long)((double)rcBounds.Height() * dfVScrollbarRectangle_Top);
//	rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 6 );
	rc.bottom	= rcBounds.top + (long)((double)rcBounds.Height() / 2 );

	dwStyle = WS_CHILD | WS_VISIBLE | BS_CENTER | BS_VCENTER | BS_ICON;
	dwExStyle = WS_EX_STATICEDGE;

	m_ctrlMultiply10.CreateEx(dwExStyle, _T("BUTTON"), csTextOut, dwStyle, rc, this, nIDMultiply10Ctrl);
	if (m_ctrlMultiply10.m_hWnd != NULL)
	{
		m_ctrlMultiply10.SetWindowPos(NULL,
				rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);

		m_ctrlMultiply10.SetFont( CFont::FromHandle(m_font.GetFontHandle()) );
	}
	m_ctrlMultiply10.SetIcon( ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SEMCORECTRLPANEL_STAND1_ICON_MUL10)) );

	/////////////////////////////////////////////////////////
	// Create Divide10			↑↓∧∨⑩▲▼△▽
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.left + 2;
	rc.right	= rcBounds.left + (long)((double)rcBounds.Width() / 7 - 2 );
	rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 2 );
	rc.bottom	= rcBounds.top  +(long)((double)rcBounds.Height() * dfVScrollbarRectangle_Bottom);
//	rc.bottom	= rcBounds.bottom - (long)((double)rcBounds.Height() / 6 );

	dwStyle = WS_CHILD | WS_VISIBLE | BS_CENTER	| BS_VCENTER | BS_ICON;
	dwExStyle = WS_EX_STATICEDGE;

	m_ctrlDivide10.CreateEx(dwExStyle, _T("BUTTON"), csTextOut, dwStyle, rc, this, nIDDivide10Ctrl);
	if (m_ctrlDivide10.m_hWnd != NULL)
	{
		m_ctrlDivide10.SetWindowPos(NULL,
				rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);

		m_ctrlDivide10.SetFont( CFont::FromHandle(m_font.GetFontHandle()) );
	}
	m_ctrlDivide10.SetIcon( ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SEMCORECTRLPANEL_STAND1_ICON_DIV10)) );

	/////////////////////////////////////////////////////////
	// Create Spin1
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.right - (long)((double)rcBounds.Width() / 7 - 2 );
	rc.right	= rcBounds.right - 2;
	rc.top		= rcBounds.top  +(long)((double)rcBounds.Height() * dfVScrollbarRectangle_Top);
	rc.bottom	= rcBounds.top  +(long)((double)rcBounds.Height() * dfVScrollbarRectangle_Bottom);
//	rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 6 );
//	rc.bottom	= rcBounds.bottom + (long)((double)rcBounds.Height() / 6 );
	
	/////////////////////////////////////////////////////////
	dwStyle = WS_CHILD | WS_VISIBLE | UDS_ARROWKEYS | UDS_HOTTRACK;
	dwExStyle = WS_EX_STATICEDGE;

	/////////////////////////////////////////////////////////
	//	initialize common controls
	//	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_UPDOWN_REG));
	m_ctrlSpin1.CreateEx(dwExStyle, dwStyle, rc, this, nIDSpin1Ctrl);
	if (m_ctrlSpin1.m_hWnd != NULL)
	{
		m_ctrlSpin1.SetWindowPos(NULL,
				rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
		m_ctrlSpin1.SetRange( 0, 100 );
		m_ctrlSpin1.SetPos( 50 );
		m_ctrlSpin1.SetBase( 10 );
	}

	return TRUE;
}

void CSemCoreCtrlPanel_Stand1Ctrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	RECT	rc;
	CRect	rcBounds;
	
	rcBounds.left	= 0;
	rcBounds.right	= cx;
	rcBounds.top	= 0;
	rcBounds.bottom	= cy;
	
	if( m_nControlStyle == 2 )
	{
		/////////////////////////////////////////////////////////
		// OnSize Static
		/////////////////////////////////////////////////////////
		rc.left		= rcBounds.left + (long)((double)rcBounds.Width() / 7 );
		rc.right	= rcBounds.right - (long)((double)rcBounds.Width() / 7 );
/*		rc.top		= rcBounds.top  +(long)((double)rcBounds.Height() * dfVScrollbarRectangle_Top);
//		rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 6 );
		rc.bottom	= rc.top + (long)((double)rcBounds.Height() / 2 - 2);
*/		rc.top		= rcBounds.top;
		rc.bottom	= rcBounds.bottom;
		if (m_ctrlStatic.m_hWnd != NULL)
		{
			m_ctrlStatic.SetWindowPos(NULL,	rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
//			OnSizeFont( &m_ctrlStatic );
		}

/*		/////////////////////////////////////////////////////////
		// OnSize ComboBox
		/////////////////////////////////////////////////////////
		rc.left		= rcBounds.left + (long)((double)rcBounds.Width() / 4 );
		rc.right	= rcBounds.right - (long)((double)rcBounds.Width() / 4 );
		rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 2 );
		rc.bottom	= rc.top + (long)((double)rcBounds.Height() );
		if (m_ctrlCombo.m_hWnd != NULL)
		{
			m_ctrlCombo.SetWindowPos(NULL,
					rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height()+80, SWP_NOZORDER);
		}
*/
		/////////////////////////////////////////////////////////
		// OnSize Multiply10			↑↓∧∨⑩▲▼△▽
		/////////////////////////////////////////////////////////
		rc.left		= rcBounds.left + 2;
		rc.right	= rcBounds.left + (long)((double)rcBounds.Width() / 7 - 2 );
		rc.top		= rcBounds.top  +(long)((double)rcBounds.Height() * dfVScrollbarRectangle_Top);
//		rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 6 );
		rc.bottom	= rcBounds.top + (long)((double)rcBounds.Height() / 2 );
		if (m_ctrlMultiply10.m_hWnd != NULL)
		{
			m_ctrlMultiply10.SetWindowPos(NULL,
					rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
		}

		/////////////////////////////////////////////////////////
		// OnSize Divide10			↑↓∧∨⑩▲▼△▽
		/////////////////////////////////////////////////////////
		rc.left		= rcBounds.left + 2;
		rc.right	= rcBounds.left + (long)((double)rcBounds.Width() / 7 - 2 );
		rc.top		= rc.bottom;//rcBounds.top + (long)((double)rcBounds.Height() / 2 );
		rc.bottom	= rcBounds.top  +(long)((double)rcBounds.Height() * dfVScrollbarRectangle_Bottom);
//		rc.bottom	= rcBounds.bottom - (long)((double)rcBounds.Height() / 6 );
		if (m_ctrlDivide10.m_hWnd != NULL)
		{
			m_ctrlDivide10.SetWindowPos(NULL,
					rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
		}

		/////////////////////////////////////////////////////////
		// OnSize Spin1
		/////////////////////////////////////////////////////////
		rc.left		= rcBounds.right - (long)((double)rcBounds.Width() / 7 - 2 );
		rc.right	= rcBounds.right - 2;
		rc.top		= rcBounds.top  +(long)((double)rcBounds.Height() * dfVScrollbarRectangle_Top);
		rc.bottom	= rcBounds.top  +(long)((double)rcBounds.Height() * dfVScrollbarRectangle_Bottom);
//		rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 6 + 2 );
//		rc.bottom	= rcBounds.bottom - (long)((double)rcBounds.Height() / 6 + 2 );
		if (m_ctrlSpin1.m_hWnd != NULL)
		{
			m_ctrlSpin1.SetWindowPos(NULL,
					rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
		}
	}
	else
	{
		if (m_nControlStyle == 0)
		{
			rc.left		= rcBounds.left +(long)((double)rcBounds.Width() * dfHScrollbarRectangle_Left);
			rc.right	= rcBounds.left +(long)((double)rcBounds.Width() * dfHScrollbarRectangle_Right);
			rc.top		= rcBounds.top  +(long)((double)rcBounds.Height() * dfHScrollbarRectangle_Top);
			rc.bottom	= rcBounds.top  +(long)((double)rcBounds.Height() * dfHScrollbarRectangle_Bottom);
		}
		else
		{
			//////////////////////////////////////////////////////
			// Calculate TextMetrics
			TEXTMETRIC	tm;
			GetStockTextMetrics(&tm);
			//////////////////////////////////////////////////////
			
			//////////////////////////////////////////////////////
			// Draw Scrollbar Rectangle
			rc.left		= rcBounds.left +(long)((double)rcBounds.Width() * dfVScrollbarRectangle_Left);
			rc.right	= rcBounds.left +(long)((double)rcBounds.Width() * dfVScrollbarRectangle_Right);
			rc.top		= rcBounds.top  +(long)((double)rcBounds.Height() * dfVScrollbarRectangle_Top);
			rc.bottom	= rcBounds.top  +(long)((double)rcBounds.Height() * dfVScrollbarRectangle_Bottom);
			
			rc.top		+= 2 * tm.tmHeight;
			rc.bottom	-= tm.tmHeight;
			
			if (m_bCaptionVisible)
				rc.bottom	-= tm.tmHeight;
			if (m_bDebugInfoVisible)
				rc.bottom	-= tm.tmHeight;
		}
		
		if (m_sbScrollBarX.m_hWnd != NULL)
		{
			m_sbScrollBarX.SetWindowPos(NULL,
				rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);	
		}
	}
}

void CSemCoreCtrlPanel_Stand1Ctrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	long	nMin = m_scfCtrlX.varMin.lVal;
	long	nMax = m_scfCtrlX.varMax.lVal;
	long	nCurrentPos	= m_scfCtrlX.varPos.lVal;
	
	switch (nSBCode)
	{
		case SB_LEFT :				// Scroll to far left.
			// case SB_TOP :
			nCurrentPos = nMin;
			break;
			
		case SB_RIGHT :				//  Scroll to far right.
			// case SB_BOTTOM :
			nCurrentPos = nMax;
			break;
			
		case SB_LINELEFT :			//  Scroll left.
			// case SB_LINEUP :		//  Scroll up.
			nCurrentPos--;
			break;
			
		case SB_LINERIGHT :			//  Scroll right.
			// case SB_LINEDOWN :	//  Scroll down.
			nCurrentPos++;
			break;
			
		case SB_PAGELEFT :			// Scroll one page left.
			// case SB_PAGEUP :		//  Scroll one page up.
			nCurrentPos -= 8;
			break;
			
		case SB_PAGERIGHT :			//  Scroll one page right.
			// case SB_PAGEDOWN :	// Scroll one page down.
			nCurrentPos += 8;
			break;
			
		case SB_ENDSCROLL :			//  End scroll.
			break;
			
		case SB_THUMBPOSITION :		//  Scroll to absolute position. The current position is specified by the nPos parameter.
			break;
			
		case SB_THUMBTRACK :		//  Drag scroll box to specified position. The current position is specified by the nPos parameter.
			nCurrentPos = nPos;
			break;
			
		default:
			break;
	}

	if (nCurrentPos != m_scfCtrlX.varPos.lVal)
	{
		VARIANT	XPosNew, YPosNew, Flag;
		
		XPosNew.lVal	= nCurrentPos;
		YPosNew.lVal	= 0;
		Flag.ulVal		= FlagCtrlX;
		
		SetPos(XPosNew, YPosNew, Flag);
	}
	
	COleControl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSemCoreCtrlPanel_Stand1Ctrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	long	nMin = m_scfCtrlX.varMin.lVal;
	long	nMax = m_scfCtrlX.varMax.lVal;
	long	nCurrentPos	= m_scfCtrlX.varMax.lVal + m_scfCtrlX.varMin.lVal - m_scfCtrlX.varPos.lVal;

	switch (nSBCode)
	{
			// case SB_RIGHT :		//  Scroll to far right.
		case SB_TOP :
			nCurrentPos = nMin;
			break;
			
			// case SB_LEFT :		// Scroll to far left.
		case SB_BOTTOM :
			nCurrentPos = nMax;
			break;
			
			// case SB_LINELEFT :	//  Scroll left.
		case SB_LINEUP :			//  Scroll up.
			nCurrentPos--;
			break;
			
			// case SB_LINERIGHT :	//  Scroll right.
		case SB_LINEDOWN :			//  Scroll down.
			nCurrentPos++;
			break;
			
			// case SB_PAGELEFT :	// Scroll one page left.
		case SB_PAGEUP :			// Scroll one page up.
			nCurrentPos -= 8;
			break;
			
			// case SB_PAGERIGHT :	//  Scroll one page right.
		case SB_PAGEDOWN :			//  Scroll one page down.
			nCurrentPos += 8;
			break;
			
		case SB_ENDSCROLL :			//  End scroll.
			break;
			
		case SB_THUMBPOSITION :		//  Scroll to absolute position. The current position is specified by the nPos parameter.
			break;
			
		case SB_THUMBTRACK :		//  Drag scroll box to specified position. The current position is specified by the nPos parameter.
			nCurrentPos = nPos;
			break;
			
		default:
			break;
	}

	long lNewPos = m_scfCtrlX.varMax.lVal + m_scfCtrlX.varMin.lVal - nCurrentPos;	
	if (lNewPos != m_scfCtrlX.varPos.lVal)
	{
		VARIANT	XPosNew, YPosNew, Flag;
		
		XPosNew.lVal	= lNewPos;
		YPosNew.lVal	= 0;
		Flag.ulVal		= FlagCtrlX;
		
		SetPos(XPosNew, YPosNew, Flag);
	}
	
	COleControl::OnVScroll(nSBCode, nPos, pScrollBar);
}




/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

void CSemCoreCtrlPanel_Stand1Ctrl::SetCtrlPos( long lPos ) 
{
	// TODO: Add your dispatch handler code here
	VARIANT	XPosNew, YPosNew, Flag;
	
	XPosNew.lVal	= lPos;
	YPosNew.lVal	= 0;
	Flag.ulVal		= FlagCtrlX;

	SetPos(XPosNew, YPosNew, Flag);
}

void CSemCoreCtrlPanel_Stand1Ctrl::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	//2008.12.03
//	m_ctrlSpin1.m_bActiveSpinPressed = TRUE;
	//2008.12.03
	long lXPos = m_scfCtrlX.varPos.lVal;

	if (pNMUpDown->iDelta >= 1)
	{
		lXPos = GetNewPos( lXPos, 0 );
		SetCtrlPos( lXPos );
	}
	if (pNMUpDown->iDelta <= -1)
	{
		lXPos = GetNewPos( lXPos, 2 );
		SetCtrlPos( lXPos );
	}

	*pResult = 0;
}

void CSemCoreCtrlPanel_Stand1Ctrl::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	long lXPos = m_scfCtrlX.varPos.lVal;

	if (pNMUpDown->iDelta >= 1)
	{
		lXPos = GetNewPos( lXPos, 1 );
		SetCtrlPos( lXPos );
	}
	if (pNMUpDown->iDelta <= -1)
	{
		lXPos = GetNewPos( lXPos, 3 );
		SetCtrlPos( lXPos );
	}

	*pResult = 0;
}

void CSemCoreCtrlPanel_Stand1Ctrl::OnButtonMultiply10() 
{
	// TODO: Add your control notification handler code here
	long lXPos = m_scfCtrlX.varPos.lVal;

	lXPos = GetNewPos( lXPos, 1 );
	SetCtrlPos( lXPos );
}

void CSemCoreCtrlPanel_Stand1Ctrl::OnButtonDivide10() 
{
	// TODO: Add your control notification handler code here
	long lXPos = m_scfCtrlX.varPos.lVal;

	lXPos = GetNewPos( lXPos, 3 );
	SetCtrlPos( lXPos );
}

void CSemCoreCtrlPanel_Stand1Ctrl::OnSelchangeCombo() 
{
	CString str;
	int nIndex = m_ctrlCombo.GetCurSel();
	switch( nIndex )
	{
	case 0:
		SetCtrlPos( 0 );
		break;
	case 1:
		SetCtrlPos( 100 );
		break;
	case 2:
		SetCtrlPos( 110 );
		break;
	case 3:
		SetCtrlPos( 120 );
		break;
	}
}

CString CSemCoreCtrlPanel_Stand1Ctrl::GetDisplayString()
{
	CString csTextOut;
	const CString& strCaption = InternalGetText();
	const double& dblPosX = m_scfCtrlX.varPos.dblVal;

	csTextOut.Empty();
	/////////////////////////////////////////////////////////
	if (m_bCaptionVisible)
		csTextOut = strCaption + _T(" ");

	CString csTemp;
	if (m_bDebugInfoVisible)
	{
		//csTemp.Format("[ID:%3Ld]", m_varPanelSerialNumber.lVal);
		csTemp.Format("[ID:%3Ld] %4.2e", m_varPanelSerialNumber.lVal, dblPosX);
		csTextOut += csTemp;
	}
	csTextOut += _T(" ") + m_strX;

//	if ( !m_strY.IsEmpty() )
//		csTextOut += _T(" ") + m_strY;

	return csTextOut;
}

void CSemCoreCtrlPanel_Stand1Ctrl::OnSizeFont(CWnd* pCtrl)
{
	// OnSize Font
	CRect				rcCtl;
	NONCLIENTMETRICS	nc;

	m_fontOnSize.DeleteObject();

	if (pCtrl == NULL) return;

	ZeroMemory(&nc, sizeof(nc));
	nc.cbSize = sizeof(nc);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &nc, 0);

	pCtrl->GetWindowRect( (LPRECT)rcCtl );

	CString csBuffer = m_strX;

	m_strX = _T("-99.9 K-");
	const CString csPattern = GetDisplayString();

	m_strX = csBuffer;
	csBuffer = _T("-99.9 K-");

	CSize	sizePattern = pCtrl->GetDC()->GetTextExtent( csPattern );
	double  dblRateXY = (double)sizePattern.cx / sizePattern.cy;


	if( dblRateXY >= ( (double)rcCtl.Width() / rcCtl.Height() ) )
	{
		nc.lfMenuFont.lfHeight = (int)( rcCtl.Width() / dblRateXY ) -5;
	}
	else
	{
		nc.lfMenuFont.lfHeight = (rcCtl.Height() - 8);
	}

//	m_fontOnSize.CreateFontIndirect(&nc.lfMenuFont);
	m_fontOnSize.CreateFont(-nc.lfMenuFont.lfHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0,
					ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Microsoft Sans Serif");

	pCtrl->SetFont( &m_fontOnSize );

}

void CSemCoreCtrlPanel_Stand1Ctrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( !m_blCtrlEnableX || m_nFastTotal < 1 )
	{
		COleControl::OnLButtonDown(nFlags, point);
		return;
	}
	CPoint temppoint = point;

	CRect rc;
	GetClientRect( &rc );
	int nWidth = rc.Width();
	rc.left		= rc.left + nWidth / 5 *2;
	rc.right	= rc.right - nWidth / 5 *2;
	if( rc.PtInRect( temppoint ) )
	{
		CMenu menu;
		menu.CreatePopupMenu();
		for( int i=0; i<m_nFastTotal; i++ )
			menu.InsertMenu( IDM_SEMCORECTRLPANEL_STAND9_MAG_1 +i, MF_BYCOMMAND, IDM_SEMCORECTRLPANEL_STAND9_MAG_1 +i, m_strFast[i] );
//		temppoint = CPoint( rc.left, rc.bottom );
		ClientToScreen( &temppoint );
		menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
			temppoint.x, temppoint.y, this, NULL);
	}

//	COleControl::OnLButtonDown(nFlags, point);
}

// 快速参数改变
void CSemCoreCtrlPanel_Stand1Ctrl::OnFastChanged(UINT nID)
{
	UINT nIndex = nID - IDM_SEMCORECTRLPANEL_STAND9_MAG_1;
	VARIANT	XPosNew, YPosNew, Flag;
	XPosNew.lVal	= nIndex;
	YPosNew.lVal	= 0;
	Flag.ulVal		= FlagCtrlX;

	FireReportAction(m_varPanelClassID, m_varPanelSerialNumber, XPosNew, YPosNew, Flag);
}

void CSemCoreCtrlPanel_Stand1Ctrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( !m_blCtrlEnableX )
	{
		COleControl::OnRButtonDown(nFlags, point);
		return;
	}
	CPoint temppoint = point;

	CRect rc;
	GetClientRect( &rc );
	int nWidth = rc.Width();
	rc.left		= rc.left + nWidth / 4;
	rc.right	= rc.right - nWidth / 4;
	if( rc.PtInRect( temppoint ) )
	{
		VARIANT	XPosNew, YPosNew, Flag;
		XPosNew.lVal	= 0;
		YPosNew.lVal	= 1;
		Flag.ulVal		= FlagCtrlX;
		
		FireReportAction(m_varPanelClassID, m_varPanelSerialNumber, XPosNew, YPosNew, Flag);
	}

//	COleControl::OnRButtonDown(nFlags, point);
}
