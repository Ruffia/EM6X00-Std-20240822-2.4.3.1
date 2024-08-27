// SemCoreCtrlPanel_StandACtl.cpp : Implementation of the CSemCoreCtrlPanel_StandACtrl ActiveX Control class.
//
// Last Modify : 2009.04.02

#include "stdafx.h"
#include "SemCoreCtrlPanel_StandA.h"
#include "SemCoreCtrlPanel_StandACtl.h"
#include "SemCoreCtrlPanel_StandAPpg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_StandACtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_StandACtrl, COleControl)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_StandACtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CSemCoreCtrlPanel_StandACtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CSemCoreCtrlPanel_StandACtrl)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_StandACtrl, "CaptionVisible", m_bCaptionVisible, OnCaptionVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_StandACtrl, "DebugInfoVisible", m_bDebugInfoVisible, OnDebugInfoVisibleChanged, VT_BOOL)
	DISP_FUNCTION(CSemCoreCtrlPanel_StandACtrl, "Bind", Bind, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_StandACtrl, "Enable", Enable, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_StandACtrl, "SetRange", SetRange, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_StandACtrl, "SetPos", SetPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_StandACtrl, "SyncPos", SyncPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_StandACtrl, "SyncText", SyncText, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_VARIANT)
	DISP_STOCKPROP_BACKCOLOR()
	DISP_STOCKPROP_FORECOLOR()
	DISP_STOCKPROP_FONT()
	DISP_STOCKPROP_CAPTION()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CSemCoreCtrlPanel_StandACtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CSemCoreCtrlPanel_StandACtrl, COleControl)
	//{{AFX_EVENT_MAP(CSemCoreCtrlPanel_StandACtrl)
	EVENT_CUSTOM("ReportPos", FireReportPos, VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSemCoreCtrlPanel_StandACtrl, 3)
	PROPPAGEID(CSemCoreCtrlPanel_StandAPropPage::guid)
	PROPPAGEID(CLSID_CColorPropPage)
	PROPPAGEID(CLSID_CFontPropPage)
END_PROPPAGEIDS(CSemCoreCtrlPanel_StandACtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_StandACtrl, "SEMCORECTRLPANELSTANDA.SemCoreCtrlPanelStandACtrl.1",
	0x6c185bd2, 0x30f0, 0x4524, 0xa9, 0x9c, 0x1b, 0x6c, 0xee, 0xd4, 0xed, 0x38)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CSemCoreCtrlPanel_StandACtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DSemCoreCtrlPanel_StandA =
		{ 0xab976132, 0x8c6, 0x4087, { 0x9a, 0x62, 0x9b, 0xa1, 0x82, 0x34, 0x50, 0x69 } };
const IID BASED_CODE IID_DSemCoreCtrlPanel_StandAEvents =
		{ 0x7077c5b3, 0xbcd5, 0x44af, { 0xa3, 0x1e, 0x9b, 0x84, 0x39, 0x7a, 0x6b, 0x8d } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwSemCoreCtrlPanel_StandAOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSemCoreCtrlPanel_StandACtrl, IDS_SEMCORECTRLPANEL_STANDA, _dwSemCoreCtrlPanel_StandAOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_StandACtrl::CSemCoreCtrlPanel_StandACtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_StandACtrl

BOOL CSemCoreCtrlPanel_StandACtrl::CSemCoreCtrlPanel_StandACtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_SEMCORECTRLPANEL_STANDA,
			IDB_SEMCORECTRLPANEL_STANDA,
			afxRegApartmentThreading,
			_dwSemCoreCtrlPanel_StandAOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_StandACtrl::CSemCoreCtrlPanel_StandACtrl - Constructor

CSemCoreCtrlPanel_StandACtrl::CSemCoreCtrlPanel_StandACtrl()
{
	InitializeIIDs(&IID_DSemCoreCtrlPanel_StandA, &IID_DSemCoreCtrlPanel_StandAEvents);

	// TODO: Initialize your control's instance data here.
	m_strX = _T("-X-");
	m_strY = _T("");
	m_strCaption = _T("");	//09.02.12
	
	m_varPanelSerialNumber.lVal = -1;
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_StandACtrl::~CSemCoreCtrlPanel_StandACtrl - Destructor

CSemCoreCtrlPanel_StandACtrl::~CSemCoreCtrlPanel_StandACtrl()
{
	// TODO: Cleanup your control's instance data here.
	OnSizeFont( NULL );
}

/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_StandACtrl::OnDraw - Drawing function

void CSemCoreCtrlPanel_StandACtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	/////////////////////////////////////////////////////////
	if (m_ctrlStatic.m_hWnd != NULL)
	{
		//09.02.12
/*		const CString& strCaption = InternalGetText();
		if( strCaption != m_strCaption )
		{
			m_ctrlStatic.SetWindowText( strCaption );
//			GetDisplayString();
		}
		//09.02.12
*/		return;
	}
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE)));

	/////////////////////////////////////////////////////////
	CRect		rc = rcBounds;

	/////////////////////////////////////////////////////////
	CPen*		pOldPen;
	CBrush*		pOldBrush;
	//CBrush*		pNewBrush = CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE));
	CBrush	brStaticBack;
	brStaticBack.CreateSolidBrush( TranslateColor(GetBackColor()) );

	pOldPen		= (CPen*)pdc->SelectStockObject(BLACK_PEN);
	//pOldBrush	= pdc->SelectObject(pNewBrush);
	pOldBrush	= pdc->SelectObject(&brStaticBack);

	/////////////////////////////////////////////////////////
	CFont*		pOldFont;
	//pdc->SetTextColor(TranslateColor(GetSysColor(COLOR_BTNTEXT)));
	//pdc->SetBkColor(TranslateColor(GetSysColor(COLOR_3DFACE)));
	pdc->SetTextColor(TranslateColor(GetForeColor()));
	pdc->SetBkColor(TranslateColor(GetBackColor()));
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
	rc = rcBounds;
	
	pdc->Rectangle(rc);

	/////////////////////////////////////////////////////////
	CString csTextOut = GetDisplayString();

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

		pdc->SetTextAlign(TA_CENTER | TA_BASELINE);
		pdc->TextOut(nValueCenter, nValueBaseline, csTextOut) ;

		//////////////////////////////////////////////////
		pdc->SelectObject( pOldFont );
		fontOnSize.DeleteObject();
	}
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	pdc->SelectObject(pOldPen);
	pdc->SelectObject(pOldBrush);
	brStaticBack.DeleteObject();
	/////////////////////////////////////////////////////////
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_StandACtrl::DoPropExchange - Persistence support

void CSemCoreCtrlPanel_StandACtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
	if (pPX->GetVersion() ==(DWORD)MAKELONG(_wVerMinor, _wVerMajor))
	{
		PX_Bool(pPX, _T("CaptionVisible"), m_bCaptionVisible, TRUE);
		PX_Bool(pPX, _T("DebugInfoVisible"), m_bDebugInfoVisible, TRUE);
	}
	else if (pPX->IsLoading())
	{
		// Skip over persistent data
		BOOL bDummy;
		PX_Bool(pPX, _T("CaptionVisible"), bDummy, FALSE);
		PX_Bool(pPX, _T("DebugInfoVisible"), bDummy, FALSE);
		
		// Force property values to these defaults
		m_bCaptionVisible = TRUE;
		m_bDebugInfoVisible = TRUE;
	}

}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_StandACtrl::OnResetState - Reset control to default state

void CSemCoreCtrlPanel_StandACtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_StandACtrl::AboutBox - Display an "About" box to the user

void CSemCoreCtrlPanel_StandACtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_SEMCORECTRLPANEL_STANDA);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_StandACtrl message handlers


void CSemCoreCtrlPanel_StandACtrl::Bind(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber) 
{
	// TODO: Add your dispatch handler code here
	m_varPanelClassID		= ClassID;
	m_varPanelSerialNumber	= SerialNumber;
}

void CSemCoreCtrlPanel_StandACtrl::Enable(const VARIANT FAR& Enabled, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here

}

void CSemCoreCtrlPanel_StandACtrl::SetRange(const VARIANT FAR& Xmin, const VARIANT FAR& Xmax, const VARIANT FAR& Ymin, const VARIANT FAR& Ymax) 
{
	// TODO: Add your dispatch handler code here

}

void CSemCoreCtrlPanel_StandACtrl::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	SyncPos( Xpos, Ypos, Flag );
	FireReportPos(m_varPanelClassID, m_varPanelSerialNumber, Xpos, Ypos, Flag);	
}

void CSemCoreCtrlPanel_StandACtrl::SyncPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
}

void CSemCoreCtrlPanel_StandACtrl::SyncText(LPCTSTR strX, LPCTSTR strY, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) == FlagCtrlX )
		m_strX = strX;

	if ( (Flag.ulVal & FlagCtrlY) == FlagCtrlY )
		m_strY = strY;
	
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

void CSemCoreCtrlPanel_StandACtrl::OnTextChanged() 
{
	// TODO: Add your specialized code here and/or call the base class
	COleControl::OnTextChanged();
	if (m_ctrlStatic.m_hWnd != NULL)
	{
		CString csTextOut = GetDisplayString();
		m_ctrlStatic.SetWindowText( csTextOut );
//		OnSizeFont( &m_ctrlStatic );
	}
}

void CSemCoreCtrlPanel_StandACtrl::OnCaptionVisibleChanged() 
{
	// TODO: Add notification handler code
	if (m_ctrlStatic.m_hWnd != NULL)
	{
		CString csTextOut = GetDisplayString();
		m_ctrlStatic.SetWindowText( csTextOut );
//		OnSizeFont( &m_ctrlStatic );
	}
	
	// force the control to redraw itself
	InvalidateControl();
	
	SetModifiedFlag();
}

void CSemCoreCtrlPanel_StandACtrl::OnDebugInfoVisibleChanged() 
{
	// TODO: Add notification handler code
	if (m_ctrlStatic.m_hWnd != NULL)
	{
		CString csTextOut = GetDisplayString();
		m_ctrlStatic.SetWindowText( csTextOut );
//		OnSizeFont( &m_ctrlStatic );
	}
	
	// force the control to redraw itself
	InvalidateControl();
	
	SetModifiedFlag();
}

int CSemCoreCtrlPanel_StandACtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	/////////////////////////////////////////////////////////
	CRect rcBounds(0, 0, lpCreateStruct->cx, lpCreateStruct->cy);
	CRect rc = rcBounds;
	/////////////////////////////////////////////////////////
	DWORD	dwStyle, dwExStyle;
	
	/////////////////////////////////////////////////////////
	// Create Static
	/////////////////////////////////////////////////////////
	rc = rcBounds;
	
	/////////////////////////////////////////////////////////
	dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE; //
	dwExStyle = WS_EX_STATICEDGE;
	
	/////////////////////////////////////////////////////////
	CString csTextOut = GetDisplayString();
	
	/////////////////////////////////////////////////////////
	m_ctrlStatic.CreateEx(dwExStyle, _T("STATIC"), csTextOut, dwStyle, rc, this, nIDStaticCtrl);
	if (m_ctrlStatic.m_hWnd != NULL)
	{
		m_ctrlStatic.SetWindowPos(NULL,	rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
//		OnSizeFont(&m_ctrlStatic);
		m_ctrlStatic.SetFont( CFont::FromHandle(m_font.GetFontHandle()) );	//09.04.02
		m_ctrlStatic.SetTextColor(TranslateColor(GetForeColor()));
		m_ctrlStatic.SetBkColor(TranslateColor(GetBackColor()));
	}
	
	return 0;
}

void CSemCoreCtrlPanel_StandACtrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	/////////////////////////////////////////////////////////
	CRect rcBounds(0, 0, cx, cy);
	CRect rc = rcBounds;
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// OnSize Static
	/////////////////////////////////////////////////////////
	if (m_ctrlStatic.m_hWnd != NULL)
	{
		m_ctrlStatic.SetWindowPos(NULL,	rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
//		OnSizeFont( &m_ctrlStatic );
	}
}

CString CSemCoreCtrlPanel_StandACtrl::GetDisplayString()
{
	CString csTextOut;
	const CString& strCaption = InternalGetText();
	
	csTextOut.Empty();
	/////////////////////////////////////////////////////////
	if (m_bCaptionVisible)
		csTextOut = strCaption + _T(" ");
	
	CString csTemp;
	if (m_bDebugInfoVisible)
	{
		const CString& csDisplayFormat = _T("[ID:%3Ld]");
		csTemp.Format( csDisplayFormat, m_varPanelSerialNumber.lVal );
		csTextOut += csTemp;
	}
	csTextOut += _T(" ") + m_strX;
	
	if (!m_strY.IsEmpty())
		csTextOut += _T(" ") + m_strY;

	/////////////////////////
	return csTextOut;
}

void CSemCoreCtrlPanel_StandACtrl::OnSizeFont(CWnd* pCtrl)
{
	// OnSize Font
	CRect				rcCtl;
	NONCLIENTMETRICS	nc;

	m_fontOnSize.DeleteObject();

	if ( pCtrl == NULL ) return;

	ZeroMemory(&nc, sizeof(nc));
	nc.cbSize = sizeof(nc);

	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &nc, 0);

	pCtrl->GetWindowRect( (LPRECT)rcCtl );

	const CString csPattern = GetDisplayString();
	CSize	sizePattern = pCtrl->GetDC()->GetTextExtent( csPattern );
	double  dblRateXY = (double)sizePattern.cx / sizePattern.cy;

	if( dblRateXY >= ( (double)rcCtl.Width() / rcCtl.Height() ) )
		nc.lfMenuFont.lfHeight = (int)( rcCtl.Width() / dblRateXY ) -5;
	else
		nc.lfMenuFont.lfHeight = (rcCtl.Height() - 8);

//	m_fontOnSize.CreateFontIndirect(&nc.lfMenuFont);
	m_fontOnSize.CreateFont(-nc.lfMenuFont.lfHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0,
					ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Microsoft Sans Serif");

	pCtrl->SetFont( &m_fontOnSize );

}
