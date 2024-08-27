// SemCoreCtrlPanel_Stand8Ctl.cpp : Implementation of the CSemCoreCtrlPanel_Stand8Ctrl ActiveX Control class.

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand8.h"
#include "SemCoreCtrlPanel_Stand8Ctl.h"
#include "SemCoreCtrlPanel_Stand8Ppg.h"
#include "DlgSetupTimer.h"
#include "DlgProgress.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

const double dfSpinButtonRectangle_Left		= 0.25;
const double dfSpinButtonRectangle_Right	= 0.75;
const double dfSpinButtonRectangle_Top		= 0.0;
const double dfSpinButtonRectangle_Bottom	= 1.0;


//////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_Stand8Ctrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_Stand8Ctrl, COleControl)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_Stand8Ctrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(UDN_DELTAPOS, nIDSpinCtrl, OnDeltaposSpin)
	ON_BN_CLICKED(nIDResetCtrl, OnButtonReset)
	ON_BN_CLICKED(nIDSaveCtrl, OnButtonSave)
	ON_BN_CLICKED(nIDLoadCtrl, OnButtonLoad)
	ON_BN_CLICKED(nIDParamCtrl, OnButtonParam)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CSemCoreCtrlPanel_Stand8Ctrl, COleControl)
	//{{AFX_DISPATCH_MAP(CSemCoreCtrlPanel_Stand8Ctrl)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand8Ctrl, "CaptionVisible", m_bCaptionVisible, OnCaptionVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand8Ctrl, "DebugInfoVisible", m_bDebugInfoVisible, OnDebugInfoVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand8Ctrl, "ProgressMessage", m_csProgressMessage, OnProgressMessageChanged, VT_BSTR)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand8Ctrl, "Bind", Bind, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand8Ctrl, "Enable", Enable, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand8Ctrl, "SetRange", SetRange, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand8Ctrl, "SetPos", SetPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand8Ctrl, "SyncPos", SyncPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand8Ctrl, "SyncText", SyncText, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_VARIANT)
	DISP_STOCKPROP_BACKCOLOR()
	DISP_STOCKPROP_FORECOLOR()
	DISP_STOCKPROP_FONT()
	DISP_STOCKPROP_CAPTION()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CSemCoreCtrlPanel_Stand8Ctrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CSemCoreCtrlPanel_Stand8Ctrl, COleControl)
	//{{AFX_EVENT_MAP(CSemCoreCtrlPanel_Stand8Ctrl)
	EVENT_CUSTOM("ReportPos", FireReportPos, VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSemCoreCtrlPanel_Stand8Ctrl, 3)
	PROPPAGEID(CSemCoreCtrlPanel_Stand8PropPage::guid)
	PROPPAGEID(CLSID_CColorPropPage)
	PROPPAGEID(CLSID_CFontPropPage)
END_PROPPAGEIDS(CSemCoreCtrlPanel_Stand8Ctrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_Stand8Ctrl, "SEMCORECTRLPANELSTAND8.SemCoreCtrlPanelStand8Ctrl.1",
	0xf515798a, 0xb3d0, 0x4b57, 0xb2, 0xfe, 0x89, 0xeb, 0xb3, 0x7f, 0x3a, 0x69)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CSemCoreCtrlPanel_Stand8Ctrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DSemCoreCtrlPanel_Stand8 =
		{ 0xba85f2, 0xcbfd, 0x49c6, { 0xaa, 0xa6, 0xc2, 0x9b, 0x81, 0xf0, 0x7f, 0x38 } };
const IID BASED_CODE IID_DSemCoreCtrlPanel_Stand8Events =
		{ 0xe6a338f7, 0xc0a3, 0x4982, { 0xa8, 0xf4, 0x7, 0x2c, 0xae, 0xf, 0xff, 0xe3 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwSemCoreCtrlPanel_Stand8OleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSemCoreCtrlPanel_Stand8Ctrl, IDS_SEMCORECTRLPANEL_STAND8, _dwSemCoreCtrlPanel_Stand8OleMisc)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand8Ctrl::CSemCoreCtrlPanel_Stand8CtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_Stand8Ctrl

BOOL CSemCoreCtrlPanel_Stand8Ctrl::CSemCoreCtrlPanel_Stand8CtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_SEMCORECTRLPANEL_STAND8,
			IDB_SEMCORECTRLPANEL_STAND8,
			afxRegApartmentThreading,
			_dwSemCoreCtrlPanel_Stand8OleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand8Ctrl::CSemCoreCtrlPanel_Stand8Ctrl - Constructor

CSemCoreCtrlPanel_Stand8Ctrl::CSemCoreCtrlPanel_Stand8Ctrl()
{
	InitializeIIDs(&IID_DSemCoreCtrlPanel_Stand8, &IID_DSemCoreCtrlPanel_Stand8Events);

	// TODO: Initialize your control's instance data here.
	m_scfCtrlX.varMin.lVal = 0;
	m_scfCtrlX.varMax.lVal = 255;
	m_scfCtrlX.varPos.lVal = 0;
	
	m_scfCtrlY = m_scfCtrlX;
	
	m_strX = _T("-X-");
	m_strY = _T("-Y-");
	
	m_varPanelSerialNumber.lVal = -1;

	m_nStepTimer = 150;
	m_lSavedPos = 0;
	m_csSettingSection = g_csSettingSection + _T("\\");
	m_szStepTimerEntry	= _T("StepTimer");
	m_szSavedPosEntry	= _T("SavedPos");
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand8Ctrl::~CSemCoreCtrlPanel_Stand8Ctrl - Destructor

CSemCoreCtrlPanel_Stand8Ctrl::~CSemCoreCtrlPanel_Stand8Ctrl()
{
	// TODO: Cleanup your control's instance data here.
	OnSizeFont( NULL );
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand8Ctrl::OnDraw - Drawing function

void CSemCoreCtrlPanel_Stand8Ctrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	/////////////////////////////////////////////////////////
	if (m_ctrlSpin.m_hWnd != NULL)
		return;
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// TODO: Replace the following code with your own drawing code.
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
	//pdc->SetTextColor(TranslateColor(GetForeColor()));
	pdc->SetTextColor(TranslateColor(GetSysColor(COLOR_BTNTEXT)));
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
	rc.right	= rcBounds.left + (long)((double)rcBounds.Width() * dfSpinButtonRectangle_Left - 1);
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
	/////////////////////////////////////////////////////////
	pOldFont = SelectStockFont(pdc);
	pdc->GetTextMetrics(&tm) ;
	nValueFontHeight = tm.tmHeight ;

	/////////////////////////////////////////////////////////
	// Draw Reset
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.left;
	rc.right	= rcBounds.left + (long)((double)rcBounds.Width() * dfSpinButtonRectangle_Left - 1);
	rc.top		= rcBounds.bottom - (long)((double)rcBounds.Height() / 2 );
	rc.bottom	= rcBounds.bottom;
	
	pdc->Rectangle(rc);

	csTextOut.LoadString(IDS_SEMCORECTRLPANEL_STAND8_SELFDEFINE_RESET);
	//////////////////////////////////////////////////////
	// determine the location for the value within
	// the rectangele based on the font height
	nValueBaseline = rc.top + (int)( rc.Height()/2.0 + nValueFontHeight / 4.0 + 0.5 );
	nValueCenter   = rc.left + rc.Width()/2 ;

	if (!csTextOut.IsEmpty())
	{
		pdc->SetTextAlign(TA_CENTER | TA_BASELINE);
		pdc->TextOut(nValueCenter, nValueBaseline, csTextOut) ;
	}
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// Draw Spin
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.left + (long)((double)rcBounds.Width() * dfSpinButtonRectangle_Left);
	rc.right	= rcBounds.left + (long)((double)rcBounds.Width() * dfSpinButtonRectangle_Right);
	rc.top		= rcBounds.top  + (long)((double)rcBounds.Height() * dfSpinButtonRectangle_Top);
	rc.bottom	= rcBounds.top  + (long)((double)rcBounds.Height() * dfSpinButtonRectangle_Bottom);
	
	pdc->Rectangle(rc);

	CPoint	p0, p1;
	p0.x = rc.left + (int)(rc.Width() / 2.0 + 0.5);
	p0.y = rc.top + 2;
	p1.x = p0.x;
	p1.y = rc.bottom - 2;

	pdc->MoveTo( p0 );
	pdc->LineTo( p1 );

	csTextOut = _T("¡¶");
	//////////////////////////////////////////////////////
	// determine the location for the value within
	// the rectangele based on the font height
	nValueBaseline = rc.top + (int)( rc.Height()/2.0 + nValueFontHeight / 4.0 + 0.5 );
	nValueCenter   = rc.left + rc.Width()/4 ;

	if (!csTextOut.IsEmpty())
	{
		pdc->SetTextAlign(TA_CENTER | TA_BASELINE);
		pdc->TextOut(nValueCenter, nValueBaseline, csTextOut) ;
	}
	/////////////////////////////////////////////////////////

	csTextOut = _T("¡·");
	//////////////////////////////////////////////////////
	// determine the location for the value within
	// the rectangele based on the font height
	nValueBaseline = rc.top + (int)( rc.Height()/2.0 + nValueFontHeight / 4.0 + 0.5 );
	nValueCenter   = rc.right - rc.Width()/4 ;

	if (!csTextOut.IsEmpty())
	{
		pdc->SetTextAlign(TA_CENTER | TA_BASELINE);
		pdc->TextOut(nValueCenter, nValueBaseline, csTextOut) ;
	}
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// Draw Save
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.right - (long)((double)rcBounds.Width() * (1 - dfSpinButtonRectangle_Right));
	rc.right	= rcBounds.right;
	rc.top		= rcBounds.top;
	rc.bottom	= rcBounds.top  + (long)((double)rcBounds.Height() / 2 );
	
	pdc->Rectangle(rc);

	csTextOut.LoadString(IDS_SEMCORECTRLPANEL_STAND8_SELFDEFINE_SAVE);
	//////////////////////////////////////////////////////
	// determine the location for the value within
	// the rectangele based on the font height
	nValueBaseline = rc.top + (int)( rc.Height()/2.0 + nValueFontHeight/4.0 + 0.5 );
	nValueCenter   = rc.left + rc.Width()/2 ;

	if (!csTextOut.IsEmpty())
	{
		pdc->SetTextAlign(TA_CENTER | TA_BASELINE);
		pdc->TextOut(nValueCenter, nValueBaseline, csTextOut) ;
	}
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// Draw Load
	/////////////////////////////////////////////////////////
	rc.top		= rcBounds.bottom - (long)((double)rcBounds.Height() / 2 );
	rc.bottom	= rcBounds.bottom;
	rc.left		= rcBounds.right - (long)((double)rcBounds.Width() * (1 - dfSpinButtonRectangle_Right));
	rc.right	= rcBounds.right - rc.Height();
	
	pdc->Rectangle(rc);

	csTextOut.LoadString(IDS_SEMCORECTRLPANEL_STAND8_SELFDEFINE_LOAD);
	//////////////////////////////////////////////////////
	// determine the location for the value within
	// the rectangele based on the font height
	nValueBaseline = rc.top + (int)( rc.Height()/2.0 + nValueFontHeight/4.0 + 0.5 );
	nValueCenter   = rc.left + rc.Width()/2 ;

	if (!csTextOut.IsEmpty())
	{
		pdc->SetTextAlign(TA_CENTER | TA_BASELINE);
		pdc->TextOut(nValueCenter, nValueBaseline, csTextOut) ;
	}
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// Draw Param
	/////////////////////////////////////////////////////////
	rc.top		= rcBounds.bottom - (long)((double)rcBounds.Height() / 2 );
	rc.bottom	= rcBounds.bottom;
	rc.left		= rcBounds.right - rc.Height();
	rc.right	= rcBounds.right;
	
	pdc->Rectangle(rc);

	csTextOut = _T(">>");
	//////////////////////////////////////////////////////
	// determine the location for the value within
	// the rectangele based on the font height
	nValueBaseline = rc.top + (int)( rc.Height()/2.0 + nValueFontHeight/4.0 + 0.5 );
	nValueCenter   = rc.left + rc.Width()/2 ;

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
// CSemCoreCtrlPanel_Stand8Ctrl::DoPropExchange - Persistence support

void CSemCoreCtrlPanel_Stand8Ctrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
	if (pPX->GetVersion() ==(DWORD)MAKELONG(_wVerMinor, _wVerMajor))
	{
		PX_Bool(pPX, _T("CaptionVisible"), m_bCaptionVisible, TRUE);
		PX_Bool(pPX, _T("DebugInfoVisible"), m_bDebugInfoVisible, TRUE);
		PX_String(pPX, _T("ProgressMessage"), m_csProgressMessage, _T(""));
	}
	else if (pPX->IsLoading())
	{
		// Skip over persistent data
		BOOL bDummy;
		CString strDummy;
		PX_Bool(pPX, _T("CaptionVisible"), bDummy, FALSE);
		PX_Bool(pPX, _T("DebugInfoVisible"), bDummy, FALSE);
		PX_String(pPX, _T("ProgressMessage"), strDummy, _T(""));
		
		// Force property values to these defaults
		m_bCaptionVisible = TRUE;
		m_bDebugInfoVisible = TRUE;
		m_csProgressMessage = _T("Loading......");
	}

}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand8Ctrl::OnResetState - Reset control to default state

void CSemCoreCtrlPanel_Stand8Ctrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand8Ctrl::AboutBox - Display an "About" box to the user

void CSemCoreCtrlPanel_Stand8Ctrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_SEMCORECTRLPANEL_STAND8);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand8Ctrl message handlers

void CSemCoreCtrlPanel_Stand8Ctrl::Bind(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber) 
{
	// TODO: Add your dispatch handler code here
	m_varPanelClassID		= ClassID;
	m_varPanelSerialNumber	= SerialNumber;
}

void CSemCoreCtrlPanel_Stand8Ctrl::Enable(const VARIANT FAR& Enabled, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;

	const BOOL& blEnabled = Enabled.boolVal;

	if (m_ctrlSpin.m_hWnd != NULL)
		m_ctrlSpin.EnableWindow( blEnabled );

//	if (m_ctrlReset.m_hWnd != NULL)
//		m_ctrlReset.EnableWindow( blEnabled );

	if (m_ctrlSave.m_hWnd != NULL)
		m_ctrlSave.EnableWindow( blEnabled );

	if (m_ctrlLoad.m_hWnd != NULL)
		m_ctrlLoad.EnableWindow( blEnabled );

//	if (m_ctrlParam.m_hWnd != NULL)
//		m_ctrlParam.EnableWindow( blEnabled );

}

void CSemCoreCtrlPanel_Stand8Ctrl::SetRange(const VARIANT FAR& Xmin, const VARIANT FAR& Xmax, const VARIANT FAR& Ymin, const VARIANT FAR& Ymax) 
{
	// TODO: Add your dispatch handler code here
	m_scfCtrlX.varMin = Xmin;
	m_scfCtrlX.varMax = Xmax;
	
	m_scfCtrlY.varMin = Ymin;
	m_scfCtrlY.varMax = Ymax;
}

void CSemCoreCtrlPanel_Stand8Ctrl::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
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

void CSemCoreCtrlPanel_Stand8Ctrl::SyncPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;
	
	/////////////////////////////////////////////////////////
	long&	lXPos = m_scfCtrlX.varPos.lVal;
	lXPos = Xpos.lVal;
	/////////////////////////////////////////////////////////

	CString csTextOut = GetDisplayString();

	/////////////////////////////////////////////////////////
	if (m_ctrlStatic.m_hWnd != NULL)
	{
		m_ctrlStatic.SetWindowText( csTextOut );
	}

//	InvalidateControl();
}

void CSemCoreCtrlPanel_Stand8Ctrl::SyncText(LPCTSTR strX, LPCTSTR strY, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;
	
	m_strX = _T(strX);
	
	// m_strY;
	
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

//////////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////////

void CSemCoreCtrlPanel_Stand8Ctrl::OnCaptionVisibleChanged() 
{
	// TODO: Add notification handler code
	
	// force the control to redraw itself
	InvalidateControl();
	
	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand8Ctrl::OnDebugInfoVisibleChanged() 
{
	// TODO: Add notification handler code
	
	// force the control to redraw itself
	InvalidateControl();
	
	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand8Ctrl::OnProgressMessageChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

int CSemCoreCtrlPanel_Stand8Ctrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	/////////////////////////////////////////////////////////
	CRect rcBounds(0, 0, lpCreateStruct->cx, lpCreateStruct->cy);
	CRect rc = rcBounds;
	/////////////////////////////////////////////////////////
	CString csTextOut;
	DWORD	dwStyle, dwExStyle;

	/////////////////////////////////////////////////////////
	// Create Static
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.left;
	rc.right	= rcBounds.left + (long)((double)rcBounds.Width() * dfSpinButtonRectangle_Left - 1);
	rc.top		= rcBounds.top;
	rc.bottom	= rcBounds.top  + (long)((double)rcBounds.Height() / 2 );
	
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
		OnSizeFont( &m_ctrlStatic );
		m_ctrlStatic.SetTextColor(TranslateColor(GetForeColor()));
		m_ctrlStatic.SetBkColor(TranslateColor(GetBackColor()));
	}
	
	/////////////////////////////////////////////////////////
	// Create Reset
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.left;
	rc.right	= rcBounds.left + (long)((double)rcBounds.Width() * dfSpinButtonRectangle_Left - 1);
	rc.top		= rcBounds.bottom - (long)((double)rcBounds.Height() / 2 );
	rc.bottom	= rcBounds.bottom;
	
	/////////////////////////////////////////////////////////
	dwStyle = WS_CHILD | WS_VISIBLE | BS_CENTER	| BS_VCENTER;
	dwExStyle = WS_EX_STATICEDGE;

	csTextOut.LoadString(IDS_SEMCORECTRLPANEL_STAND8_SELFDEFINE_RESET);
	/////////////////////////////////////////////////////////
	m_ctrlReset.CreateEx(dwExStyle, _T("BUTTON"), csTextOut, dwStyle, rc, this, nIDResetCtrl);
	if (m_ctrlReset.m_hWnd != NULL)
	{
		m_ctrlReset.SetWindowPos(NULL,
				rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);

		m_ctrlReset.SetFont( CFont::FromHandle(m_font.GetFontHandle()) );
	}

	/////////////////////////////////////////////////////////
	// Create Spin
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.left + (long)((double)rcBounds.Width() * dfSpinButtonRectangle_Left);
	rc.right	= rcBounds.left + (long)((double)rcBounds.Width() * dfSpinButtonRectangle_Right);
	rc.top		= rcBounds.top  + (long)((double)rcBounds.Height() * dfSpinButtonRectangle_Top);
	rc.bottom	= rcBounds.top  + (long)((double)rcBounds.Height() * dfSpinButtonRectangle_Bottom);
	
	/////////////////////////////////////////////////////////
	dwStyle = WS_CHILD | WS_VISIBLE | UDS_ARROWKEYS | UDS_HORZ | UDS_HOTTRACK;
	dwExStyle = WS_EX_STATICEDGE;

	/////////////////////////////////////////////////////////
	//	initialize common controls
	//	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_UPDOWN_REG));
	m_ctrlSpin.CreateEx(dwExStyle, dwStyle, rc, this, nIDSpinCtrl);
	if (m_ctrlSpin.m_hWnd != NULL)
	{
		m_ctrlSpin.SetWindowPos(NULL,
				rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
		m_ctrlSpin.SetRange( 0, 100 );
		m_ctrlSpin.SetPos( 50 );
		m_ctrlSpin.SetBase( 10 );
	}

	/////////////////////////////////////////////////////////
	// Create Save
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.right - (long)((double)rcBounds.Width() * (1 - dfSpinButtonRectangle_Right));
	rc.right	= rcBounds.right;
	rc.top		= rcBounds.top;
	rc.bottom	= rcBounds.top  + (long)((double)rcBounds.Height() / 2 );
	
	/////////////////////////////////////////////////////////
	dwStyle = WS_CHILD | WS_VISIBLE | BS_CENTER	| BS_VCENTER;
	dwExStyle = WS_EX_STATICEDGE;

	csTextOut.LoadString(IDS_SEMCORECTRLPANEL_STAND8_SELFDEFINE_SAVE);
	/////////////////////////////////////////////////////////
	m_ctrlSave.CreateEx(dwExStyle, _T("BUTTON"), csTextOut, dwStyle, rc, this, nIDSaveCtrl);
	if (m_ctrlSave.m_hWnd != NULL)
	{
		m_ctrlSave.SetWindowPos(NULL,
				rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);

		m_ctrlSave.SetFont( CFont::FromHandle(m_font.GetFontHandle()) );
	}

	/////////////////////////////////////////////////////////
	// Create Load
	/////////////////////////////////////////////////////////
	rc.top		= rcBounds.bottom - (long)((double)rcBounds.Height() / 2 );
	rc.bottom	= rcBounds.bottom;
	rc.left		= rcBounds.right - (long)((double)rcBounds.Width() * (1 - dfSpinButtonRectangle_Right));
	rc.right	= rcBounds.right - rc.Height();
	
	/////////////////////////////////////////////////////////
	dwStyle = WS_CHILD | WS_VISIBLE | BS_CENTER	| BS_VCENTER;
	dwExStyle = WS_EX_STATICEDGE;

	csTextOut.LoadString(IDS_SEMCORECTRLPANEL_STAND8_SELFDEFINE_LOAD);
	/////////////////////////////////////////////////////////
	m_ctrlLoad.CreateEx(dwExStyle, _T("BUTTON"), csTextOut, dwStyle, rc, this, nIDLoadCtrl);
	if (m_ctrlLoad.m_hWnd != NULL)
	{
		m_ctrlLoad.SetWindowPos(NULL,
				rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);

		m_ctrlLoad.SetFont( CFont::FromHandle(m_font.GetFontHandle()) );
	}

	/////////////////////////////////////////////////////////
	// Create Param
	/////////////////////////////////////////////////////////
	rc.top		= rcBounds.bottom - (long)((double)rcBounds.Height() / 2 );
	rc.bottom	= rcBounds.bottom;
	rc.left		= rcBounds.right - rc.Height();
	rc.right	= rcBounds.right;
	
	/////////////////////////////////////////////////////////
	dwStyle = WS_CHILD | WS_VISIBLE | BS_CENTER	| BS_VCENTER;
	dwExStyle = WS_EX_STATICEDGE;

	csTextOut = _T(">>");
	/////////////////////////////////////////////////////////
	m_ctrlParam.CreateEx(dwExStyle, _T("BUTTON"), csTextOut, dwStyle, rc, this, nIDParamCtrl);
	if (m_ctrlParam.m_hWnd != NULL)
	{
		m_ctrlParam.SetWindowPos(NULL,
				rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);

		m_ctrlParam.SetFont( CFont::FromHandle(m_font.GetFontHandle()) );
	}

	const CString& strCaption = InternalGetText();
	/////////////////////////////////////////////////////////	
	if ( !strCaption.IsEmpty() )
		m_csSettingSection += strCaption;
	else
		m_csSettingSection += _T("Stand8");

	m_nStepTimer = AfxGetApp()->GetProfileInt( m_csSettingSection, 
		m_szStepTimerEntry, 150 );

	m_lSavedPos = AfxGetApp()->GetProfileInt( m_csSettingSection, 
		m_szSavedPosEntry, 0 );
	/////////////////////////////////////////////////////////	
	
	return 0;
}

void CSemCoreCtrlPanel_Stand8Ctrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	/////////////////////////////////////////////////////////
	CRect	rcBounds;
	rcBounds.left	= 0;
	rcBounds.right	= cx;
	rcBounds.top	= 0;
	rcBounds.bottom	= cy;

	/////////////////////////////////////////////////////////
	CRect	rc = rcBounds;

	/////////////////////////////////////////////////////////
	// OnSize Static
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.left;
	rc.right	= rcBounds.left + (long)((double)rcBounds.Width() * dfSpinButtonRectangle_Left - 1);
	rc.top		= rcBounds.top;
	rc.bottom	= rcBounds.top  + (long)((double)rcBounds.Height() / 2 );
	
	if (m_ctrlStatic.m_hWnd != NULL)
	{
		m_ctrlStatic.SetWindowPos(NULL,
		rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);

		OnSizeFont( &m_ctrlStatic );
	}

	/////////////////////////////////////////////////////////
	// OnSize Reset
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.left;
	rc.right	= rcBounds.left + (long)((double)rcBounds.Width() * dfSpinButtonRectangle_Left - 1);
	rc.top		= rcBounds.bottom - (long)((double)rcBounds.Height() / 2 );
	rc.bottom	= rcBounds.bottom;
	
	if (m_ctrlReset.m_hWnd != NULL)
		m_ctrlReset.SetWindowPos(NULL,
		rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);	

	/////////////////////////////////////////////////////////
	// OnSize Spin
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.left + (long)((double)rcBounds.Width() * dfSpinButtonRectangle_Left);
	rc.right	= rcBounds.left + (long)((double)rcBounds.Width() * dfSpinButtonRectangle_Right);
	rc.top		= rcBounds.top  + (long)((double)rcBounds.Height() * dfSpinButtonRectangle_Top);
	rc.bottom	= rcBounds.top  + (long)((double)rcBounds.Height() * dfSpinButtonRectangle_Bottom);
	
	if (m_ctrlSpin.m_hWnd != NULL)
		m_ctrlSpin.SetWindowPos(NULL,
		rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);	

	/////////////////////////////////////////////////////////
	// OnSize Save
	/////////////////////////////////////////////////////////
	rc.left		= rcBounds.right - (long)((double)rcBounds.Width() * (1 - dfSpinButtonRectangle_Right));
	rc.right	= rcBounds.right;
	rc.top		= rcBounds.top;
	rc.bottom	= rcBounds.top  + (long)((double)rcBounds.Height() / 2 );
	
	if (m_ctrlSave.m_hWnd != NULL)
		m_ctrlSave.SetWindowPos(NULL,
		rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);	

	/////////////////////////////////////////////////////////
	// OnSize Load
	/////////////////////////////////////////////////////////
	rc.top		= rcBounds.bottom - (long)((double)rcBounds.Height() / 2 );
	rc.bottom	= rcBounds.bottom;
	rc.left		= rcBounds.right - (long)((double)rcBounds.Width() * (1 - dfSpinButtonRectangle_Right));
	rc.right	= rcBounds.right - rc.Height();
	
	if (m_ctrlLoad.m_hWnd != NULL)
		m_ctrlLoad.SetWindowPos(NULL,
		rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);	

	/////////////////////////////////////////////////////////
	// OnSize Param
	/////////////////////////////////////////////////////////
	rc.top		= rcBounds.bottom - (long)((double)rcBounds.Height() / 2 );
	rc.bottom	= rcBounds.bottom;
	rc.left		= rcBounds.right - rc.Height();
	rc.right	= rcBounds.right;
	
	if (m_ctrlParam.m_hWnd != NULL)
		m_ctrlParam.SetWindowPos(NULL,
		rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);	

	/////////////////////////////////////////////////////////
	
}

void CSemCoreCtrlPanel_Stand8Ctrl::SetCtrlPos( long lPos ) 
{
	// TODO: Add your dispatch handler code here
	VARIANT	XPosNew, YPosNew, Flag;
	
	XPosNew.lVal	= lPos;
	YPosNew.lVal	= 0;
	Flag.ulVal		= FlagCtrlX;

	SetPos(XPosNew, YPosNew, Flag);
}
	
void CSemCoreCtrlPanel_Stand8Ctrl::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	const long& lPosX = m_scfCtrlX.varPos.lVal;

	if (pNMUpDown->iDelta >= 1)
	{
		SetCtrlPos( lPosX + 1 );
	}
	if (pNMUpDown->iDelta <= -1)
	{
		SetCtrlPos( lPosX - 1 );
	}

	*pResult = 0;
}

void CSemCoreCtrlPanel_Stand8Ctrl::OnButtonReset() 
{
	// TODO: Add your control notification handler code here
	const long& lMin = m_scfCtrlX.varMin.lVal;

	SetCtrlPos( lMin );
}

void CSemCoreCtrlPanel_Stand8Ctrl::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	const long& lPosX = m_scfCtrlX.varPos.lVal;
	m_lSavedPos = lPosX;

	AfxGetApp()->WriteProfileInt( m_csSettingSection, 
		m_szSavedPosEntry, m_lSavedPos );
}

void CSemCoreCtrlPanel_Stand8Ctrl::OnButtonLoad() 
{
	// TODO: Add your control notification handler code here

	////////////////////////////////////////////////
	const long& lPosX = m_scfCtrlX.varPos.lVal;

	if (lPosX == m_lSavedPos)	return;
	if (lPosX > m_lSavedPos)
	{
		SetCtrlPos( m_lSavedPos );
		return;
	}

	////////////////////////////////////////////////
	CDlgProgress	dlg;
	dlg.Bind( this );

	dlg.m_iRangeFrom	= lPosX;
	dlg.m_iRangeTo		= m_lSavedPos;
	dlg.m_nTimerElapse	= m_nStepTimer;
	dlg.m_csMessage		= m_csProgressMessage;

	dlg.DoModal();
}

void CSemCoreCtrlPanel_Stand8Ctrl::OnButtonParam() 
{
	// TODO: Add your control notification handler code here
	CDlgSetupTimer	dlg;
	
	dlg.m_nTimer = m_nStepTimer;
	
	if ( dlg.DoModal() == IDOK )
	{
		m_nStepTimer = dlg.m_nTimer;

		AfxGetApp()->WriteProfileInt( m_csSettingSection, 
			m_szStepTimerEntry, m_nStepTimer );
	}
	
}

CString CSemCoreCtrlPanel_Stand8Ctrl::GetDisplayString()
{
	CString csTextOut;
	const CString& strCaption = InternalGetText();
	const long&	lXPos = m_scfCtrlX.varPos.lVal;

	csTextOut.Empty();
	/////////////////////////////////////////////////////////
	if (m_bCaptionVisible)
		csTextOut = strCaption + _T(" ");

	CString csTemp;
	if (m_bDebugInfoVisible)
	{
		//csTemp.Format("[ID:%3Ld]", m_varPanelSerialNumber.lVal);
		csTemp.Format("[ID:%3Ld] [%3Ld]", m_varPanelSerialNumber.lVal, lXPos);
		csTextOut += csTemp;
	}
	csTextOut += _T(" ") + m_strX;

//	if ( !m_strY.IsEmpty() )
//		csTextOut += _T(" ") + m_strY;

	return csTextOut;
}

void CSemCoreCtrlPanel_Stand8Ctrl::OnSizeFont(CWnd* pCtrl)
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

	m_strX = _T("-9999 mA-");
	const CString csPattern = GetDisplayString();

	m_strX = csBuffer;

	CSize	sizePattern = pCtrl->GetDC()->GetTextExtent( csPattern );
	double  dblRateXY = (double)sizePattern.cx / sizePattern.cy;


	if( dblRateXY >= ( (double)rcCtl.Width() / rcCtl.Height() ) )
	{
		nc.lfMenuFont.lfHeight = (int)( rcCtl.Width() / dblRateXY ) -5;
	}
	else
	{
		nc.lfMenuFont.lfHeight = rcCtl.Height() - 8;
	}

//	m_fontOnSize.CreateFontIndirect(&nc.lfMenuFont);
	m_fontOnSize.CreateFont(-nc.lfMenuFont.lfHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0,
					ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Microsoft Sans Serif");

	pCtrl->SetFont( &m_fontOnSize );

}
