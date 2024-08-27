// SemCoreCtrlPanel_Stand6Ctl.cpp : Implementation of the CSemCoreCtrlPanel_Stand6Ctrl ActiveX Control class.
//
// Last Modify : 2009.02.16

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand6.h"
#include "SemCoreCtrlPanel_Stand6Ctl.h"
#include "SemCoreCtrlPanel_Stand6Ppg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
static const FONTDESC _fontdescHeading =
  { sizeof(FONTDESC), OLESTR("宋体"), FONTSIZE( 9 ), FW_REGULAR, 
     ANSI_CHARSET, FALSE, FALSE, FALSE };

const CString	csCtrlGrade[] = {"粗调", "细调", "微调"};
const long		lCtrlGradeMultiple[] = {32,8,1};//{64, 8, 1};
const long		lCtrlVernierScale[] = {1024,1024,1024};

const double dfHScrollbarRectangle_Left		= 0.0;
const double dfHScrollbarRectangle_Right	= 1.0;
const double dfHScrollbarRectangle_Top		= 0.3;
const double dfHScrollbarRectangle_Bottom	= 0.7;

const double dfVScrollbarRectangle_Left		= 0.1;
const double dfVScrollbarRectangle_Right	= 0.9;
const double dfVScrollbarRectangle_Top		= 0.0;
const double dfVScrollbarRectangle_Bottom	= 1.0;


IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_Stand6Ctrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_Stand6Ctrl, COleControl)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_Stand6Ctrl)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_COMMAND_RANGE(IDM_SEMCORECTRLPANEL_STAND9_MAG_1, IDM_SEMCORECTRLPANEL_STAND9_MAG_8, OnFastChanged)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CSemCoreCtrlPanel_Stand6Ctrl, COleControl)
	//{{AFX_DISPATCH_MAP(CSemCoreCtrlPanel_Stand6Ctrl)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand6Ctrl, "CaptionVisible", m_bCaptionVisible, OnCaptionVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand6Ctrl, "DebugInfoVisible", m_bDebugInfoVisible, OnDebugInfoVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand6Ctrl, "XMinString", m_csXMinString, OnXMinStringChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand6Ctrl, "XMaxString", m_csXMaxString, OnXMaxStringChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand6Ctrl, "GradeVisible", m_bGradeVisible, OnGradeVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand6Ctrl, "AutoVisible", m_bAutoVisible, OnAutoVisibleChanged, VT_BOOL)
	DISP_PROPERTY_EX(CSemCoreCtrlPanel_Stand6Ctrl, "GradeColor", GetGradeColor, SetGradeColor, VT_COLOR)
	DISP_PROPERTY_EX(CSemCoreCtrlPanel_Stand6Ctrl, "GradeFont", GetGradeFont, SetGradeFont, VT_FONT)
	DISP_PROPERTY_EX(CSemCoreCtrlPanel_Stand6Ctrl, "GradeState", GetGradeState, SetGradeState, VT_I2)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand6Ctrl, "Bind", Bind, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand6Ctrl, "Enable", Enable, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand6Ctrl, "SetRange", SetRange, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand6Ctrl, "SetPos", SetPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand6Ctrl, "SyncPos", SyncPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand6Ctrl, "SyncText", SyncText, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand6Ctrl, "SyncResString", SyncResString, VT_EMPTY, VTS_VARIANT VTS_BSTR)
	DISP_STOCKPROP_BACKCOLOR()
	DISP_STOCKPROP_FORECOLOR()
	DISP_STOCKPROP_FONT()
	DISP_STOCKPROP_CAPTION()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CSemCoreCtrlPanel_Stand6Ctrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CSemCoreCtrlPanel_Stand6Ctrl, COleControl)
	//{{AFX_EVENT_MAP(CSemCoreCtrlPanel_Stand6Ctrl)
	EVENT_CUSTOM("ReportPos", FireReportPos, VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT)
	EVENT_CUSTOM("ReportAction", FireReportAction, VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSemCoreCtrlPanel_Stand6Ctrl, 3)
	PROPPAGEID(CSemCoreCtrlPanel_Stand6PropPage::guid)
	PROPPAGEID(CLSID_CColorPropPage)
	PROPPAGEID(CLSID_CFontPropPage)
END_PROPPAGEIDS(CSemCoreCtrlPanel_Stand6Ctrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_Stand6Ctrl, "SEMCORECTRLPANELSTAND6.SemCoreCtrlPanelStand6Ctrl.1",
	0x957fc761, 0x96db, 0x40d2, 0x97, 0x52, 0x5a, 0x79, 0x2, 0x22, 0xc4, 0x98)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CSemCoreCtrlPanel_Stand6Ctrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DSemCoreCtrlPanel_Stand6 =
		{ 0xb6c04ba3, 0x1d44, 0x4f69, { 0x84, 0x1e, 0x5b, 0x6d, 0x87, 0x73, 0x5, 0x3c } };
const IID BASED_CODE IID_DSemCoreCtrlPanel_Stand6Events =
		{ 0x4add80a6, 0xf40a, 0x4c7a, { 0x90, 0x85, 0x75, 0x5c, 0x1c, 0xab, 0xb3, 0x4f } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwSemCoreCtrlPanel_Stand6OleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSemCoreCtrlPanel_Stand6Ctrl, IDS_SEMCORECTRLPANEL_STAND6, _dwSemCoreCtrlPanel_Stand6OleMisc)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand6Ctrl::CSemCoreCtrlPanel_Stand6CtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_Stand6Ctrl

BOOL CSemCoreCtrlPanel_Stand6Ctrl::CSemCoreCtrlPanel_Stand6CtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_SEMCORECTRLPANEL_STAND6,
			IDB_SEMCORECTRLPANEL_STAND6,
			afxRegApartmentThreading,
			_dwSemCoreCtrlPanel_Stand6OleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand6Ctrl::CSemCoreCtrlPanel_Stand6Ctrl - Constructor

CSemCoreCtrlPanel_Stand6Ctrl::CSemCoreCtrlPanel_Stand6Ctrl()
:m_fontGradeFont( &m_xFontNotification )

{
	InitializeIIDs(&IID_DSemCoreCtrlPanel_Stand6, &IID_DSemCoreCtrlPanel_Stand6Events);

	// TODO: Initialize your control's instance data here.
	m_scfCtrlX.varMin.lVal = 0;
	m_scfCtrlX.varMax.lVal = 32767;//65535;
	m_scfCtrlX.varPos.lVal = 16384;//32768;
	
	m_scfCtrlY = m_scfCtrlX;
	
	m_strX = _T("-X-");
	m_strY = _T("-Y-");

	m_blCtrlEnableX = TRUE;
	
	m_varPanelSerialNumber.lVal = -1;

	m_nCtrlGrade = 1;	//{0 "粗调", 1 "细调", 2 "微调"};
	ReadjustScollbarCtrlParam( TRUE );

	m_nFastTotal = 0;
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand6Ctrl::~CSemCoreCtrlPanel_Stand6Ctrl - Destructor

CSemCoreCtrlPanel_Stand6Ctrl::~CSemCoreCtrlPanel_Stand6Ctrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand6Ctrl::OnDraw - Drawing function

BOOL CSemCoreCtrlPanel_Stand6Ctrl::CreateScrollBarH(const CRect &rcBounds)
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
	//////////////////////////////////////////////////////////////
	if ( m_sbScrollBarX.m_hWnd != NULL )
	{
		m_sbScrollBarX.SetScrollRange( m_lVernierMin, m_lVernierMax );
		m_sbScrollBarX.SetScrollPos( m_lVernierPos );
		m_sbScrollBarX.SetWindowPos(NULL,
			rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
		m_sbScrollBarX.ShowWindow(SW_SHOW);
	}
	//////////////////////////////////////////////////////////////
	
	return TRUE;
}

void CSemCoreCtrlPanel_Stand6Ctrl::DrawHScroll(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE)));
	
	if (m_sbScrollBarX.m_hWnd == NULL)
	{
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
	}
	
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
		csTemp.Format(" [ID:%3Ld] [CTL:%5Ld]", m_varPanelSerialNumber.lVal, m_scfCtrlX.varPos.lVal);
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

	//////////////////////////////////////////////////////
	// Draw Value
	//csTemp.Format("[%5Ld]", m_scfCtrlX.varPos.lVal);
	//csTextOut = csTemp + m_strX;
	csTextOut = m_strX;
	
	pdc->SetTextAlign(TA_RIGHT | TA_TOP);
	pdc->ExtTextOut(rcBounds.right, rcBounds.top,
		ETO_CLIPPED, rcBounds, csTextOut, csTextOut.GetLength(), NULL);
	//////////////////////////////////////////////////////
		
	pdc->SelectObject(pOldFont);
	//////////////////////////////////////////////////////
	//////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////
	// Calculate Expect-Invalid-Rect
	TEXTMETRIC	tm;
	GetStockTextMetrics(&tm);
	CSize sizeText = pdc->GetTextExtent(csTextOut);
	m_ExpectInvalidRect			= rcBounds;
	if (m_bDebugInfoVisible)
		m_ExpectInvalidRect.left	= rcBounds.left + sizeText.cx;
	else
		m_ExpectInvalidRect.left	= rcBounds.right - sizeText.cx - tm.tmMaxCharWidth;
	m_ExpectInvalidRect.bottom	= rcBounds.top + sizeText.cy;
	//////////////////////////////////////////////////////
}

void CSemCoreCtrlPanel_Stand6Ctrl::DrawDlgGrade(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	CRect	rectClient, rectCtrl;
	rectCtrl = rectClient = rcBounds;

	//////////////////////////////////////////////////////
	CPoint	Center;
	//////////////////////////////////////////////////////

	CString		csTextOut;
	CFont*		pOldFont;
	//pdc->SetTextColor(TranslateColor(GetGradeColor()));
	pdc->SetTextColor(TranslateColor(GetSysColor(COLOR_BTNTEXT)));
	pdc->SetBkMode(TRANSPARENT);

	//pOldFont = SelectStockFont(pdc);
	pOldFont = SelectFontObject(pdc, m_fontGradeFont);

	TEXTMETRIC	tm;
	//GetStockTextMetrics(&tm);
	m_fontGradeFont.QueryTextMetrics(&tm);
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	rectCtrl.left	= rectClient.left;
	rectCtrl.right	= rectClient.left + rectClient.Width()/5;

	pdc->FrameRect(rectCtrl, CBrush::FromHandle(GetSysColorBrush(COLOR_BTNTEXT)));
	//////////////////////////////////////////////////////
	csTextOut = _T("粗调");
	Center = rectCtrl.CenterPoint();
	Center.y += (int)(tm.tmHeight / 2.0 + 0.5);
	pdc->SetTextAlign(TA_CENTER | TA_BOTTOM);
	pdc->ExtTextOut(Center.x, Center.y,
		ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);
	//////////////////////////////////////////////////////
		
	//////////////////////////////////////////////////////
	rectCtrl.left	= rectClient.left + rectClient.Width()/5;
	rectCtrl.right	= rectClient.left + rectClient.Width()/5*2;

	pdc->FrameRect(rectCtrl, CBrush::FromHandle(GetSysColorBrush(COLOR_BTNTEXT)));
	//////////////////////////////////////////////////////
	csTextOut = _T("细调");
	Center = rectCtrl.CenterPoint();
	Center.y += (int)(tm.tmHeight / 2.0 + 0.5);
	pdc->SetTextAlign(TA_CENTER | TA_BOTTOM);
	pdc->ExtTextOut(Center.x, Center.y,
		ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	rectCtrl.left	= rectClient.left + rectClient.Width()/5*2;
	rectCtrl.right	= rectClient.left + rectClient.Width()/5*3;

	pdc->FrameRect(rectCtrl, CBrush::FromHandle(GetSysColorBrush(COLOR_BTNTEXT)));
	//////////////////////////////////////////////////////
	csTextOut = _T("微调");
	Center = rectCtrl.CenterPoint();
	Center.y += (int)(tm.tmHeight / 2.0 + 0.5);
	pdc->SetTextAlign(TA_CENTER | TA_BOTTOM);
	pdc->ExtTextOut(Center.x, Center.y,
		ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	rectCtrl.left	= rectClient.left + rectClient.Width()/5*3;
	rectCtrl.right	= rectClient.left + rectClient.Width()/5*4;

	pdc->FrameRect(rectCtrl, CBrush::FromHandle(GetSysColorBrush(COLOR_BTNTEXT)));
	//////////////////////////////////////////////////////
	csTextOut = _T("消磁");
	Center = rectCtrl.CenterPoint();
	Center.y += (int)(tm.tmHeight / 2.0 + 0.5);
	pdc->SetTextAlign(TA_CENTER | TA_BOTTOM);
	pdc->ExtTextOut(Center.x, Center.y,
		ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	rectCtrl.left	= rectClient.left + rectClient.Width()/5*4;
	rectCtrl.right	= rectClient.right;

	pdc->FrameRect(rectCtrl, CBrush::FromHandle(GetSysColorBrush(COLOR_BTNTEXT)));
	//////////////////////////////////////////////////////
	csTextOut = _T("自动");
	Center = rectCtrl.CenterPoint();
	Center.y += (int)(tm.tmHeight / 2.0 + 0.5);
	pdc->SetTextAlign(TA_CENTER | TA_BOTTOM);
	pdc->ExtTextOut(Center.x, Center.y,
		ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	pdc->SelectObject(pOldFont);
}


int CSemCoreCtrlPanel_Stand6Ctrl::GetDlgHeight() 
{
	TEXTMETRIC	tm;
	//GetStockTextMetrics(&tm);
	m_fontGradeFont.QueryTextMetrics(&tm);

	int nDlgHeight = tm.tmHeight + 8;

	return nDlgHeight;
}

void CSemCoreCtrlPanel_Stand6Ctrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE)));

	if (!m_bGradeVisible)
	{		
		DrawHScroll(pdc, rcBounds, rcInvalid);
	}
	else
	{		
		int nDlgHeight = GetDlgHeight();

		CRect	rectCtrl;

		rectCtrl = rcBounds;
		rectCtrl.bottom -= nDlgHeight;
		DrawHScroll(pdc, rectCtrl, rcInvalid);
		
		rectCtrl = rcBounds;
		rectCtrl.top = rcBounds.bottom - nDlgHeight;
		if ( m_dlgGrade.m_hWnd == NULL )
			DrawDlgGrade(pdc, rectCtrl, rcInvalid);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand6Ctrl::DoPropExchange - Persistence support

void CSemCoreCtrlPanel_Stand6Ctrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);
	
	const OLE_COLOR clrDefault = TranslateColor( GetSysColor(COLOR_BTNTEXT) );

	// TODO: Call PX_ functions for each persistent custom property.
	if (pPX->GetVersion() ==(DWORD)MAKELONG(_wVerMinor, _wVerMajor))
	{
		PX_Bool(pPX, _T("CaptionVisible"), m_bCaptionVisible, TRUE);
		PX_Bool(pPX, _T("DebugInfoVisible"), m_bDebugInfoVisible, TRUE);
		PX_String(pPX, _T("XMinString"), m_csXMinString, _T(""));
		PX_String(pPX, _T("XMaxString"), m_csXMaxString, _T(""));
		PX_Bool(pPX, _T("GradeVisible"), m_bGradeVisible, TRUE);
		PX_Bool(pPX, _T("AutoVisible"), m_bAutoVisible, TRUE);
		PX_Font(pPX, _T("GradeFont"), m_fontGradeFont, &_fontdescHeading);
		PX_Color(pPX, _T("GradeColor"), m_clrGradeColor, clrDefault);
	}
	else if (pPX->IsLoading())
	{
		// Skip over persistent data
		BOOL bDummy;
		CString strDummy;
		CFontHolder	fontDummy( &m_xFontNotification );
		OLE_COLOR clrDummy;
		PX_Bool(pPX, _T("CaptionVisible"), bDummy, FALSE);
		PX_Bool(pPX, _T("DebugInfoVisible"), bDummy, FALSE);
		PX_String(pPX, _T("XMinString"), strDummy, _T(""));
		PX_String(pPX, _T("XMaxString"), strDummy, _T(""));
		PX_Bool(pPX, _T("GradeVisible"), bDummy, FALSE);
		PX_Bool(pPX, _T("AutoVisible"), bDummy, FALSE);
		PX_Font(pPX, _T("GradeFont"), fontDummy );
		PX_Color(pPX, _T("GradeColor"), clrDummy );
		
		// Force property values to these defaults
		m_bCaptionVisible = TRUE;
		m_bDebugInfoVisible = TRUE;
		m_csXMinString = _T("XMin");
		m_csXMaxString = _T("XMax");
		m_bGradeVisible = TRUE;
		m_bAutoVisible = TRUE;
		m_fontGradeFont.InitializeFont( &_fontdescHeading );
		m_clrGradeColor = clrDefault;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand6Ctrl::OnResetState - Reset control to default state

void CSemCoreCtrlPanel_Stand6Ctrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand6Ctrl::AboutBox - Display an "About" box to the user

void CSemCoreCtrlPanel_Stand6Ctrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_SEMCORECTRLPANEL_STAND6);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand6Ctrl message handlers
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
void CSemCoreCtrlPanel_Stand6Ctrl::Bind(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber) 
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
	if( ClassID.lVal == 8 )							// SemControlPanelClass_CondenserLens
		sprintf(stabuff, "Cond");
	else if( ClassID.lVal == 7 )					// SemControlPanelClass_ObjectiveLens
		sprintf(stabuff, "Obj");

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

void CSemCoreCtrlPanel_Stand6Ctrl::Enable(const VARIANT FAR& Enabled, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;

	m_blCtrlEnableX = Enabled.boolVal;

	if (m_sbScrollBarX.m_hWnd != NULL)
		m_sbScrollBarX.EnableWindow( m_blCtrlEnableX );

	if (m_dlgGrade.m_hWnd != NULL)
		m_dlgGrade.EnableWindow( m_blCtrlEnableX );

}

void CSemCoreCtrlPanel_Stand6Ctrl::SetRange(const VARIANT FAR& Xmin, const VARIANT FAR& Xmax, const VARIANT FAR& Ymin, const VARIANT FAR& Ymax) 
{
	// TODO: Add your dispatch handler code here
	m_scfCtrlX.varMin = Xmin;
	m_scfCtrlX.varMax = Xmax;
	
	m_scfCtrlY.varMin = Ymin;
	m_scfCtrlY.varMax = Ymax;

	ReadjustScollbarCtrlParam( TRUE );
	//////////////////////////////////////////////////////////////
	if ( m_sbScrollBarX.m_hWnd != NULL )
	{
		m_sbScrollBarX.SetScrollRange( m_lVernierMin, m_lVernierMax );
		m_sbScrollBarX.SetScrollPos( m_lVernierPos );
	}
	//////////////////////////////////////////////////////////////
}

void CSemCoreCtrlPanel_Stand6Ctrl::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
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
		
		XPosNew.dblVal	= lXPosNew;
		XPosNew.lVal	= lXPosNew;
		YPosNew.lVal	= 0;
		Flag.ulVal		= FlagCtrlX;
		
		SyncPos(XPosNew, YPosNew, Flag);
		FireReportPos(m_varPanelClassID, m_varPanelSerialNumber, XPosNew, YPosNew, Flag);
	}
}

void CSemCoreCtrlPanel_Stand6Ctrl::SyncPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;
	
	long&	lXPos = m_scfCtrlX.varPos.lVal;
	lXPos = Xpos.lVal;

	ReadjustScollbarCtrlParam();
	//////////////////////////////////////////////////////////////
	if ( m_sbScrollBarX.m_hWnd != NULL )
		m_sbScrollBarX.SetScrollPos( m_lVernierPos );
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	if (m_sbScrollBarX.m_hWnd != NULL)
		m_sbScrollBarX.EnableWindow( m_blCtrlEnableX );

	if (m_dlgGrade.m_hWnd != NULL)
		m_dlgGrade.EnableWindow( m_blCtrlEnableX );
	//////////////////////////////////////////////////////////////

	InvalidateControl();
//	InvalidateControl((LPCRECT)m_ExpectInvalidRect);	
}

void CSemCoreCtrlPanel_Stand6Ctrl::SyncText(LPCTSTR strX, LPCTSTR strY, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;
	
	m_strX = _T(strX);
	//m_strY;
	
	// force the control to redraw itself
	InvalidateControl();
}

void CSemCoreCtrlPanel_Stand6Ctrl::OnCaptionVisibleChanged() 
{
	// TODO: Add notification handler code
	
	// force the control to redraw itself
	InvalidateControl();
	
	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand6Ctrl::OnDebugInfoVisibleChanged() 
{
	// TODO: Add notification handler code
	
	// force the control to redraw itself
	InvalidateControl();
	
	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand6Ctrl::OnXMinStringChanged() 
{
	// TODO: Add notification handler code
	
	// force the control to redraw itself
	InvalidateControl();
	
	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand6Ctrl::OnXMaxStringChanged() 
{
	// TODO: Add notification handler code
	
	// force the control to redraw itself
	InvalidateControl();
	
	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand6Ctrl::OnGradeVisibleChanged() 
{
	// TODO: Add notification handler code

	// force the control to redraw itself
	InvalidateControl();

	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand6Ctrl::OnAutoVisibleChanged() 
{
	// TODO: Add notification handler code
	
	// force the control to redraw itself
	InvalidateControl();

	SetModifiedFlag();
}

int CSemCoreCtrlPanel_Stand6Ctrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	// Creat ScollbarCtrl
	//////////////////////////////////////////////////////////////////
	int nDlgHeight = GetDlgHeight();

	CRect rc(0, 0, lpCreateStruct->cx, lpCreateStruct->cy - nDlgHeight);

	ReadjustScollbarCtrlParam( TRUE );
	CreateScrollBarH(rc);
	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	// Creat DlgGrade
	//////////////////////////////////////////////////////////////////
	m_dlgGrade.m_nCtrlGrade = m_nCtrlGrade;
	m_dlgGrade.m_pFont = CFont::FromHandle( m_fontGradeFont.GetFontHandle() );
	m_dlgGrade.m_clrColor = TranslateColor( GetGradeColor() );
	m_dlgGrade.m_bAutoVisible = m_bAutoVisible;

	//////////////////////////////////////////////////////////////////
	if ( !m_dlgGrade.Bind( this ) )
		return -1;

	if ( !m_dlgGrade.Create(CGradeDlg::IDD, this) )
		return -1;

	if (m_bGradeVisible)
		m_dlgGrade.ShowWindow( SW_SHOW );
	else
		m_dlgGrade.ShowWindow( SW_HIDE );
	//////////////////////////////////////////////////////////////////

	return 0;
}

void CSemCoreCtrlPanel_Stand6Ctrl::OnSizeHScroll( const CRect &rcBounds )
{
	RECT	rc;
	
	rc.left		= rcBounds.left +(long)((double)rcBounds.Width() * dfHScrollbarRectangle_Left);
	rc.right	= rcBounds.left +(long)((double)rcBounds.Width() * dfHScrollbarRectangle_Right);
	rc.top		= rcBounds.top  +(long)((double)rcBounds.Height() * dfHScrollbarRectangle_Top);
	rc.bottom	= rcBounds.top  +(long)((double)rcBounds.Height() * dfHScrollbarRectangle_Bottom);
	
	if (m_sbScrollBarX.m_hWnd != NULL)
		m_sbScrollBarX.SetWindowPos(NULL,
		rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);	
}

void CSemCoreCtrlPanel_Stand6Ctrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect	rcHScroll(0,0,cx,cy);

	if (m_bGradeVisible)
	{
		int nDlgHeight = GetDlgHeight();

		m_dlgGrade.SetWindowPos( NULL, 0, cy - nDlgHeight, cx, nDlgHeight, SWP_NOZORDER );
		rcHScroll.bottom -= nDlgHeight;
	}

	OnSizeHScroll( rcHScroll );
}

void CSemCoreCtrlPanel_Stand6Ctrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	long	nMin = m_lVernierMin;
	long	nMax = m_lVernierMax;
	long	nCurrentPos	= m_lVernierPos;
	
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

	if (nCurrentPos != m_lVernierPos)
	{
		long lNewPos = m_lVernierBasise + ( nCurrentPos - m_lVernierMin ) * m_lVernierMultiple;

		VARIANT	XPosNew, YPosNew, Flag;
		XPosNew.lVal	= lNewPos;
		YPosNew.lVal	= 0;
		Flag.ulVal		= FlagCtrlX;

		SetPos(XPosNew, YPosNew, Flag);
	}
	
	COleControl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSemCoreCtrlPanel_Stand6Ctrl::ReportGradeChanged( int nGrade )
{
	m_nCtrlGrade = nGrade;

	ReadjustScollbarCtrlParam( TRUE );
	//////////////////////////////////////////////////////////////
	if ( m_sbScrollBarX.m_hWnd != NULL )
	{
		m_sbScrollBarX.SetScrollRange( m_lVernierMin, m_lVernierMax );
		m_sbScrollBarX.SetScrollPos( m_lVernierPos );
	}
	//////////////////////////////////////////////////////////////
}

//-----2008.05.09-----
void CSemCoreCtrlPanel_Stand6Ctrl::NotifyGradeChanged()
{
	//////////////////////////////////////////////////////////////
//	long lNewPos = m_lVernierBasise + ( nCurrentPos - m_lVernierMin ) * m_lVernierMultiple;
	VARIANT	XPosNew, YPosNew, Flag;
	XPosNew.lVal	= m_scfCtrlX.varPos.lVal;
	YPosNew.lVal	= 0;
	Flag.ulVal		= FlagCtrlX;

	FireReportPos(m_varPanelClassID, m_varPanelSerialNumber, XPosNew, YPosNew, Flag);
	//////////////////////////////////////////////////////////////
}
//-----2008.05.09-----

void CSemCoreCtrlPanel_Stand6Ctrl::NotifyActionChanged( int nIndex, int nCheck )
{
	VARIANT	XPosNew, YPosNew, Flag;
	XPosNew.lVal	= nIndex;
	YPosNew.lVal	= nCheck;
	Flag.ulVal		= FlagCtrlX;

	FireReportAction(m_varPanelClassID, m_varPanelSerialNumber, XPosNew, YPosNew, Flag);
}

void CSemCoreCtrlPanel_Stand6Ctrl::ReadjustScollbarCtrlParam( BOOL blResetVernier )
{
	//////////////////////////////////////////////////////////////////
	long lMin = m_scfCtrlX.varMin.lVal;
	long lMax = m_scfCtrlX.varMax.lVal;
	long lPos = m_scfCtrlX.varPos.lVal;
	//////////////////////////////////////////////////////////////////

	if (!blResetVernier)
	{
		long lVernierNewPos	= ( lPos - m_lVernierBasise ) / m_lVernierMultiple;
		if ( ( lVernierNewPos >= m_lVernierMin ) && ( lVernierNewPos <= m_lVernierMax ) )
		{
			m_lVernierPos = lVernierNewPos;
			return;
		}
	}

	if (m_bGradeVisible)
	{
		//////////////////////////////////////////////////////////////
		// Calculate m_lVernierMultiple
		switch (m_nCtrlGrade)
		{
			case 0:
			case 1:
			case 2:
				m_lVernierMultiple = lCtrlGradeMultiple[m_nCtrlGrade];
				m_lCtrlVernierScale = lCtrlVernierScale[m_nCtrlGrade];
				break;
			default:
				m_nCtrlGrade = 1;
				m_lVernierMultiple = lCtrlGradeMultiple[m_nCtrlGrade];
				m_lCtrlVernierScale = lCtrlVernierScale[m_nCtrlGrade];
				break;
		}
		//////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////
		// Calculate VernierScale parameters
		// Input:	lPos, lMin, lMax, m_lCtrlVernierScale
		// Output:	m_lVernierBasise, ~Min, ~Max, ~Pos
		//////////////////////////////////////////////////////////////
		m_lVernierMin		= 0;
		m_lVernierMax		= m_lCtrlVernierScale;
		long lScaleLength	= m_lCtrlVernierScale * m_lVernierMultiple;

		m_lVernierBasise	= lPos - lScaleLength / 2;
		if ( m_lVernierBasise < lMin )
			m_lVernierBasise = lMin;
		if ( m_lVernierBasise > lMax - lScaleLength )
			m_lVernierBasise = lMax - lScaleLength;

		m_lVernierPos		= m_lVernierMin + ( lPos - m_lVernierBasise ) / m_lVernierMultiple;
		m_lVernierBasise	= lPos - ( m_lVernierPos - m_lVernierMin) * m_lVernierMultiple;
		//lPos = m_lVernierBasise + ( m_lVernierPos - m_lVernierMin) * m_lVernierMultiple;
		//////////////////////////////////////////////////////////////
	}
	else
	{
		m_lVernierMultiple	= 1;

		m_lVernierBasise	= lMin;
		m_lVernierMin		= lMin;
		m_lVernierMax		= lMax;
		m_lVernierPos		= lPos;
	}
}

OLE_COLOR CSemCoreCtrlPanel_Stand6Ctrl::GetGradeColor() 
{
	// TODO: Add your property handler here

	return m_clrGradeColor;
}

void CSemCoreCtrlPanel_Stand6Ctrl::SetGradeColor(OLE_COLOR clrGradeColor) 
{
	// Is the property changing?
	if (m_clrGradeColor == clrGradeColor)
		return;

	if (FAILED(::OleTranslateColor(clrGradeColor, NULL, NULL)))
		ThrowError(CTL_E_INVALIDPROPERTYVALUE, AFX_IDP_E_INVALIDPROPERTYVALUE);

	if (!BoundPropertyRequestEdit(DISPID_FORECOLOR))
		SetNotPermitted();

	m_clrGradeColor = clrGradeColor;
	SetModifiedFlag();
	OnForeColorChanged();

	BoundPropertyChanged(DISPID_FORECOLOR);
}


LPFONTDISP CSemCoreCtrlPanel_Stand6Ctrl::GetGradeFont() 
{
	// TODO: Add your property handler here
	return m_fontGradeFont.GetFontDispatch();
}

void CSemCoreCtrlPanel_Stand6Ctrl::SetGradeFont(LPFONTDISP pFontDisp) 
{
	// TODO: Add your property handler here
	ASSERT((pFontDisp == NULL) ||
		   AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	m_fontGradeFont.InitializeFont( &_fontdescHeading, pFontDisp);

	SetModifiedFlag();
	OnFontChanged();

	BoundPropertyChanged(DISPID_FONT);
}

//-----2008.05.09-----
short CSemCoreCtrlPanel_Stand6Ctrl::GetGradeState() 
{
	// TODO: Add your property handler here
	return m_nCtrlGrade;
}

void CSemCoreCtrlPanel_Stand6Ctrl::SetGradeState(short nNewValue) 
{
	// TODO: Add your property handler here
	if( m_nCtrlGrade != nNewValue )
	{
		ReportGradeChanged( nNewValue );
		m_dlgGrade.SyncGradeChanged( nNewValue );
	}
	SetModifiedFlag();
}
//-----2008.05.09-----

// 09.02.16 添加方法以同步文字显示
void CSemCoreCtrlPanel_Stand6Ctrl::SyncResString(const VARIANT FAR& ResourceID, LPCTSTR strRes) 
{
	// TODO: Add your dispatch handler code here
/*	if( m_dlgGrade.m_hWnd != NULL )
		m_dlgGrade.SyncResString( IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_COARSE, ResourceID.bstrVal );
	return;
*/
/*	switch( ResourceID.intVal )
	{
	case	IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_COARSE:
	case	IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_FINE:
	case	IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_TINY:
	case	IDC_DIALOG_SEMCORECTRLPANEL_STAND6_CHECK_Rev:
	case	IDC_DIALOG_SEMCORECTRLPANEL_STAND6_BUTTON_Auto:
		if( m_dlgGrade.m_hWnd != NULL )
			m_dlgGrade.SyncResString( ResourceID.intVal, _T(strRes) );
		break;
	default:
		break;
	}
*/
	int nID = atoi(strRes);
	VARIANT varID;
	switch( nID )
	{
	case	1:
		varID.intVal = IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_COARSE;
		break;
	case	2:
		varID.intVal = IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_FINE;
		break;
	case	3:
		varID.intVal = IDC_DIALOG_SEMCORECTRLPANEL_STAND6_RADIO_TINY;
		break;
	case	4:
		varID.intVal = IDC_DIALOG_SEMCORECTRLPANEL_STAND6_CHECK_Rev;
		break;
	case	5:
		varID.intVal = IDC_DIALOG_SEMCORECTRLPANEL_STAND6_BUTTON_Auto;
		break;
	case	6:
		{
			varID.intVal = IDS_SEMCORECTRLPANELSTAND6CTRL_ObjectiveLens;
			CString str;
			str.Format( "%s", ResourceID.bstrVal );
			CSemCoreCtrlPanel_Stand6PropPage::m_strCaption = str;
		}
		break;
	}
	if( m_dlgGrade.m_hWnd != NULL && varID.intVal > 8000 )
		m_dlgGrade.SyncResString( varID.intVal, ResourceID.bstrVal );
}

void CSemCoreCtrlPanel_Stand6Ctrl::OnLButtonDown(UINT nFlags, CPoint point) 
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
void CSemCoreCtrlPanel_Stand6Ctrl::OnFastChanged(UINT nID)
{
	UINT nIndex = nID - IDM_SEMCORECTRLPANEL_STAND9_MAG_1;
	VARIANT	XPosNew, YPosNew, Flag;
	XPosNew.lVal	= nIndex;
	YPosNew.lVal	= 0;
	Flag.ulVal		= FlagCtrlX;

	FireReportAction(m_varPanelClassID, m_varPanelSerialNumber, XPosNew, YPosNew, Flag);
}
