// SemCoreCtrlPanel_Stand7Ctl.cpp : Implementation of the CSemCoreCtrlPanel_Stand7Ctrl ActiveX Control class.

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand7.h"
#include "SemCoreCtrlPanel_Stand7Ctl.h"
#include "SemCoreCtrlPanel_Stand7Ppg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_Stand7Ctrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_Stand7Ctrl, COleControl)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_Stand7Ctrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(nIDCheckX, OnCheckX)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CSemCoreCtrlPanel_Stand7Ctrl, COleControl)
	//{{AFX_DISPATCH_MAP(CSemCoreCtrlPanel_Stand7Ctrl)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand7Ctrl, "CaptionVisible", m_bCaptionVisible, OnCaptionVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand7Ctrl, "DebugInfoVisible", m_bDebugInfoVisible, OnDebugInfoVisibleChanged, VT_BOOL)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand7Ctrl, "Bind", Bind, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand7Ctrl, "Enable", Enable, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand7Ctrl, "SetRange", SetRange, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand7Ctrl, "SetPos", SetPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand7Ctrl, "SyncPos", SyncPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand7Ctrl, "SyncText", SyncText, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand7Ctrl, "SyncString", SyncString, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_STOCKPROP_BACKCOLOR()
	DISP_STOCKPROP_FORECOLOR()
	DISP_STOCKPROP_FONT()
	DISP_STOCKPROP_CAPTION()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CSemCoreCtrlPanel_Stand7Ctrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CSemCoreCtrlPanel_Stand7Ctrl, COleControl)
	//{{AFX_EVENT_MAP(CSemCoreCtrlPanel_Stand7Ctrl)
	EVENT_CUSTOM("ReportPos", FireReportPos, VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSemCoreCtrlPanel_Stand7Ctrl, 3)
	PROPPAGEID(CSemCoreCtrlPanel_Stand7PropPage::guid)
	PROPPAGEID(CLSID_CColorPropPage)
	PROPPAGEID(CLSID_CFontPropPage)
END_PROPPAGEIDS(CSemCoreCtrlPanel_Stand7Ctrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_Stand7Ctrl, "SEMCORECTRLPANELSTAND7.SemCoreCtrlPanelStand7Ctrl.1",
	0xa3b6c881, 0xd4e7, 0x401e, 0x86, 0x47, 0xd4, 0x53, 0x77, 0x33, 0x4a, 0x7b)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CSemCoreCtrlPanel_Stand7Ctrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DSemCoreCtrlPanel_Stand7 =
		{ 0x3c97f6f9, 0xce50, 0x40e4, { 0xab, 0xdb, 0xa4, 0x4c, 0xca, 0x20, 0x8d, 0xa } };
const IID BASED_CODE IID_DSemCoreCtrlPanel_Stand7Events =
		{ 0x71a2eea4, 0xcacc, 0x40c5, { 0xad, 0x25, 0x58, 0xd1, 0x1b, 0x71, 0x20, 0x63 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwSemCoreCtrlPanel_Stand7OleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSemCoreCtrlPanel_Stand7Ctrl, IDS_SEMCORECTRLPANEL_STAND7, _dwSemCoreCtrlPanel_Stand7OleMisc)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand7Ctrl::CSemCoreCtrlPanel_Stand7CtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_Stand7Ctrl

BOOL CSemCoreCtrlPanel_Stand7Ctrl::CSemCoreCtrlPanel_Stand7CtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_SEMCORECTRLPANEL_STAND7,
			IDB_SEMCORECTRLPANEL_STAND7,
			afxRegApartmentThreading,
			_dwSemCoreCtrlPanel_Stand7OleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand7Ctrl::CSemCoreCtrlPanel_Stand7Ctrl - Constructor

CSemCoreCtrlPanel_Stand7Ctrl::CSemCoreCtrlPanel_Stand7Ctrl()
{
	InitializeIIDs(&IID_DSemCoreCtrlPanel_Stand7, &IID_DSemCoreCtrlPanel_Stand7Events);

	// TODO: Initialize your control's instance data here.
	m_scfCtrlX.varMin.lVal = 0;
	m_scfCtrlX.varMax.lVal = 1;
	m_scfCtrlX.varPos.lVal = 0;
	
	m_scfCtrlY = m_scfCtrlX;
	
	m_strX = _T("-X-");
	m_strY = _T("-Y-");
	
	m_varPanelSerialNumber.lVal = -1;
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand7Ctrl::~CSemCoreCtrlPanel_Stand7Ctrl - Destructor

CSemCoreCtrlPanel_Stand7Ctrl::~CSemCoreCtrlPanel_Stand7Ctrl()
{
	// TODO: Cleanup your control's instance data here.
}

CString CSemCoreCtrlPanel_Stand7Ctrl::FormatDisplay()
{
	CString csTextOut;
	CString csTemp;
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

	//////////////////////////////////////////////////////
	// Draw Value
	csTextOut += m_strX;
	//////////////////////////////////////////////////////

	return csTextOut;
}

/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand7Ctrl::OnDraw - Drawing function

void CSemCoreCtrlPanel_Stand7Ctrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (m_ctrlCheckX.m_hWnd != NULL)
		return;

	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE)));
	
	//////////////////////////////////////////////////////
	// Draw Scrollbar Rectangle
	CRect	rc = rcBounds;
	
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
	

	TEXTMETRIC tm ;
	pdc->GetTextMetrics(&tm) ;
	UINT	nValueFontHeight = tm.tmHeight ;

	// determine the location for the value within
	// the rectangele based on the font height
	UINT	nValueBaseline = rc.top + (int)( rc.Height()/2.0 + nValueFontHeight/4.0 + 0.5 );
	UINT	nValueCenter   = rc.left + rc.Width()/2 ;
	//////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////
	CString csTextOut = FormatDisplay();
	//////////////////////////////////////////////////////
	// Draw Value
	if (!csTextOut.IsEmpty())
	{
		pdc->SetTextAlign(TA_CENTER | TA_BASELINE);
		pdc->TextOut(nValueCenter, nValueBaseline, csTextOut) ;
	}
	//////////////////////////////////////////////////////

	pdc->SelectObject(pOldFont);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand7Ctrl::DoPropExchange - Persistence support

void CSemCoreCtrlPanel_Stand7Ctrl::DoPropExchange(CPropExchange* pPX)
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
// CSemCoreCtrlPanel_Stand7Ctrl::OnResetState - Reset control to default state

void CSemCoreCtrlPanel_Stand7Ctrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand7Ctrl::AboutBox - Display an "About" box to the user

void CSemCoreCtrlPanel_Stand7Ctrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_SEMCORECTRLPANEL_STAND7);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand7Ctrl message handlers

void CSemCoreCtrlPanel_Stand7Ctrl::Bind(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber) 
{
	// TODO: Add your dispatch handler code here
	m_varPanelClassID		= ClassID;
	m_varPanelSerialNumber	= SerialNumber;
}

void CSemCoreCtrlPanel_Stand7Ctrl::Enable(const VARIANT FAR& Enabled, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;

	const BOOL& blEnabled = Enabled.boolVal;

	if (m_ctrlCheckX.m_hWnd != NULL)
		m_ctrlCheckX.EnableWindow( blEnabled );

}

void CSemCoreCtrlPanel_Stand7Ctrl::SetRange(const VARIANT FAR& Xmin, const VARIANT FAR& Xmax, const VARIANT FAR& Ymin, const VARIANT FAR& Ymax) 
{
	// TODO: Add your dispatch handler code here
	m_scfCtrlX.varMin = Xmin;
	m_scfCtrlX.varMax = Xmax;
	
	m_scfCtrlY.varMin = Ymin;
	m_scfCtrlY.varMax = Ymax;
	
	const long&	lXMin = m_scfCtrlX.varMin.lVal;
	const long&	lXMax = m_scfCtrlX.varMax.lVal;
}

void CSemCoreCtrlPanel_Stand7Ctrl::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
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

void CSemCoreCtrlPanel_Stand7Ctrl::SyncPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;
	
	long&	lXPos = m_scfCtrlX.varPos.lVal;
	lXPos = Xpos.lVal;

	m_ctrlCheckX.SetCheck( lXPos );

	m_ctrlCheckX.SetWindowText( FormatDisplay() );
	
	InvalidateControl((LPCRECT)m_ExpectInvalidRect);
}

void CSemCoreCtrlPanel_Stand7Ctrl::SyncText(LPCTSTR strX, LPCTSTR strY, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;
	
	m_strX = _T(strX);

	m_ctrlCheckX.SetWindowText( FormatDisplay() );
	
	// m_strY;
	
	// force the control to redraw itself
	InvalidateControl();
}

void CSemCoreCtrlPanel_Stand7Ctrl::SyncString(const VARIANT FAR& ResourceID, const VARIANT FAR& ResourceString) 
{
	// TODO: Add your dispatch handler code here
	switch( ResourceID.intVal )
	{
	case	0:
		{
			CString str(ResourceString.bstrVal);
//			SetWindowText( str );
			m_strX = str;
			m_ctrlCheckX.SetWindowText( FormatDisplay() );
			// force the control to redraw itself
			InvalidateControl();
		}
		break;
	default:
		break;
	}
}

int CSemCoreCtrlPanel_Stand7Ctrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_ctrlCheckX.m_hWnd != NULL)
		return FALSE;
	
	// TODO: Add your specialized creation code here
	CRect rc(0, 0, lpCreateStruct->cx, lpCreateStruct->cy);

	DWORD	dwStyle = WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_CENTER
		| BS_VCENTER | BS_PUSHLIKE | BS_MULTILINE | WS_TABSTOP;	//07.11.29∂‡––œ‘ æ
	DWORD	dwExStyle = WS_EX_STATICEDGE;
		
	const CString& strCaption = InternalGetText();

	m_ctrlCheckX.CreateEx(dwExStyle, _T("BUTTON"), strCaption, dwStyle, rc, this, nIDCheckX);
	m_ctrlCheckX.SetFont( CFont::FromHandle(m_font.GetFontHandle()) );
	m_ctrlCheckX.SetCheck( m_scfCtrlX.varPos.lVal );
	m_ctrlCheckX.SetWindowPos(NULL,
		rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER);
	m_ctrlCheckX.ShowWindow(SW_SHOW);

	m_ctrlCheckX.SetWindowText( FormatDisplay() );
	
	return 0;
}

void CSemCoreCtrlPanel_Stand7Ctrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rc(0, 0, cx, cy);
	
	if (m_ctrlCheckX.m_hWnd != NULL)
		m_ctrlCheckX.SetWindowPos(NULL,
		rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER);
}

void CSemCoreCtrlPanel_Stand7Ctrl::OnTextChanged() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	COleControl::OnTextChanged();

	if( m_ctrlCheckX.m_hWnd != NULL )
		m_ctrlCheckX.SetWindowText( FormatDisplay() );
	
	InvalidateControl((LPCRECT)m_ExpectInvalidRect);
}

void CSemCoreCtrlPanel_Stand7Ctrl::OnCaptionVisibleChanged() 
{
	// TODO: Add notification handler code
	if( m_ctrlCheckX.m_hWnd != NULL )
		m_ctrlCheckX.SetWindowText( FormatDisplay() );
	
	// force the control to redraw itself
	InvalidateControl();
	
	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand7Ctrl::OnDebugInfoVisibleChanged() 
{
	// TODO: Add notification handler code
	if( m_ctrlCheckX.m_hWnd != NULL )
		m_ctrlCheckX.SetWindowText( FormatDisplay() );
	
	// force the control to redraw itself
	InvalidateControl();
	
	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand7Ctrl::OnCheckX() 
{
	// TODO: Add your control notification handler code here
	UINT nCurrentPos = m_ctrlCheckX.GetCheck();

	if ( nCurrentPos != (UINT)(m_scfCtrlX.varPos.lVal) )
	{
		VARIANT	XPosNew, YPosNew, Flag;
		
		XPosNew.lVal	= nCurrentPos;
		YPosNew.lVal	= 0;
		Flag.ulVal		= FlagCtrlX;
		
		SetPos(XPosNew, YPosNew, Flag);
	}
}
