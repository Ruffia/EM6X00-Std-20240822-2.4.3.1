// SemCoreCtrlPanel_Stand4Ctl.cpp : Implementation of the CSemCoreCtrlPanel_Stand4Ctrl ActiveX Control class.

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand4.h"
#include "Digistring.h"
#include "SemCoreCtrlPanel_Stand4Ctl.h"
#include "SemCoreCtrlPanel_Stand4Ppg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_Stand4Ctrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_Stand4Ctrl, COleControl)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_Stand4Ctrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CSemCoreCtrlPanel_Stand4Ctrl, COleControl)
	//{{AFX_DISPATCH_MAP(CSemCoreCtrlPanel_Stand4Ctrl)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand4Ctrl, "Bind", Bind, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand4Ctrl, "Enable", Enable, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand4Ctrl, "SetRange", SetRange, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand4Ctrl, "SetPos", SetPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand4Ctrl, "SyncPos", SyncPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand4Ctrl, "SyncText", SyncText, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_VARIANT)
	DISP_STOCKPROP_CAPTION()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CSemCoreCtrlPanel_Stand4Ctrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CSemCoreCtrlPanel_Stand4Ctrl, COleControl)
	//{{AFX_EVENT_MAP(CSemCoreCtrlPanel_Stand4Ctrl)
	EVENT_CUSTOM("ReportPos", FireReportPos, VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSemCoreCtrlPanel_Stand4Ctrl, 1)
	PROPPAGEID(CSemCoreCtrlPanel_Stand4PropPage::guid)
END_PROPPAGEIDS(CSemCoreCtrlPanel_Stand4Ctrl)

/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_Stand4Ctrl, "SEMCORECTRLPANELSTAND4.SemCoreCtrlPanelStand4Ctrl.1",
	0x1fb180c2, 0xd2c8, 0x4029, 0xa6, 0x4c, 0x26, 0xfd, 0x6a, 0xa0, 0xe2, 0xaf)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CSemCoreCtrlPanel_Stand4Ctrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DSemCoreCtrlPanel_Stand4 =
		{ 0x4513f07b, 0xc2b1, 0x4d1c, { 0x84, 0x69, 0xdc, 0x6, 0xa7, 0x3f, 0xb7, 0x5f } };
const IID BASED_CODE IID_DSemCoreCtrlPanel_Stand4Events =
		{ 0xa4d9d351, 0x701f, 0x4a82, { 0xbf, 0xcb, 0xd4, 0xd9, 0xe, 0xc7, 0x74, 0x6f } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwSemCoreCtrlPanel_Stand4OleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSemCoreCtrlPanel_Stand4Ctrl, IDS_SEMCORECTRLPANEL_STAND4, _dwSemCoreCtrlPanel_Stand4OleMisc)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand4Ctrl::CSemCoreCtrlPanel_Stand4CtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_Stand4Ctrl

BOOL CSemCoreCtrlPanel_Stand4Ctrl::CSemCoreCtrlPanel_Stand4CtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_SEMCORECTRLPANEL_STAND4,
			IDB_SEMCORECTRLPANEL_STAND4,
			afxRegApartmentThreading,
			_dwSemCoreCtrlPanel_Stand4OleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand4Ctrl::CSemCoreCtrlPanel_Stand4Ctrl - Constructor

CSemCoreCtrlPanel_Stand4Ctrl::CSemCoreCtrlPanel_Stand4Ctrl()
{
	InitializeIIDs(&IID_DSemCoreCtrlPanel_Stand4, &IID_DSemCoreCtrlPanel_Stand4Events);

	// TODO: Initialize your control's instance data here.
	m_scfCtrlX.varMin.lVal = 0;
	m_scfCtrlX.varMax.lVal = 255;
	m_scfCtrlX.varPos.lVal = 0;
	
	m_scfCtrlY = m_scfCtrlX;
	
	m_strX = _T("-X-");
	m_strY = _T("-Y-");
	
	m_varPanelSerialNumber.lVal = -1;
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand4Ctrl::~CSemCoreCtrlPanel_Stand4Ctrl - Destructor

CSemCoreCtrlPanel_Stand4Ctrl::~CSemCoreCtrlPanel_Stand4Ctrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand4Ctrl::OnDraw - Drawing function

void CSemCoreCtrlPanel_Stand4Ctrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)));
	//pdc->Ellipse(rcBounds);

	//////////////////////////////////////////////////////
	CRect	rectClient, rectCtrl;
	CPoint	Center;
	rectClient = rcBounds;
	//////////////////////////////////////////////////////

	CString		csTextOut = InternalGetText();

	//////////////////////////////////////////////////////
	TEXTMETRIC	tm;
	pdc->GetTextMetrics(&tm);
	//////////////////////////////////////////////////////
	rectCtrl = rectClient;
	Center = rectCtrl.CenterPoint();
	Center.y += tm.tmHeight / 2;

	//////////////////////////////////////////////////////
	pdc->SetTextColor(RGB(255,255,255));
	pdc->SetBkMode(TRANSPARENT);
	pdc->SetTextAlign(TA_CENTER | TA_BOTTOM);
	pdc->ExtTextOut(Center.x, Center.y,
		ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);
	//////////////////////////////////////////////////////
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand4Ctrl::DoPropExchange - Persistence support

void CSemCoreCtrlPanel_Stand4Ctrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand4Ctrl::OnResetState - Reset control to default state

void CSemCoreCtrlPanel_Stand4Ctrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand4Ctrl::AboutBox - Display an "About" box to the user

void CSemCoreCtrlPanel_Stand4Ctrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_SEMCORECTRLPANEL_STAND4);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand4Ctrl message handlers

void CSemCoreCtrlPanel_Stand4Ctrl::Bind(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber) 
{
	// TODO: Add your dispatch handler code here
	m_varPanelClassID		= ClassID;
	m_varPanelSerialNumber	= SerialNumber;
}

void CSemCoreCtrlPanel_Stand4Ctrl::Enable(const VARIANT FAR& Enabled, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
}

void CSemCoreCtrlPanel_Stand4Ctrl::SetRange(const VARIANT FAR& Xmin, const VARIANT FAR& Xmax, const VARIANT FAR& Ymin, const VARIANT FAR& Ymax) 
{
	// TODO: Add your dispatch handler code here
	m_scfCtrlX.varMin = Xmin;
	m_scfCtrlX.varMax = Xmax;
	
	m_scfCtrlY.varMin = Ymin;
	m_scfCtrlY.varMax = Ymax;
}

void CSemCoreCtrlPanel_Stand4Ctrl::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;
	
	const long&	lXMin = m_scfCtrlX.varMin.lVal;
	const long&	lXMax = m_scfCtrlX.varMax.lVal;
	long&	lXPos = m_scfCtrlX.varPos.lVal;
	
	long	lXPosNew = Xpos.lVal;
	
	if (lXPosNew < lXMin)	lXPosNew = lXMin;
	if (lXPosNew > lXMax)	lXPosNew = lXMax;
	
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

void CSemCoreCtrlPanel_Stand4Ctrl::SyncPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;
	
	long&	lXPos = m_scfCtrlX.varPos.lVal;
	lXPos = Xpos.lVal;
	
	InvalidateControl();
}

void CSemCoreCtrlPanel_Stand4Ctrl::SyncText(LPCTSTR strX, LPCTSTR strY, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;
	
	m_strX = _T(strX);
	// m_strY;

	m_ctrlDigiMeter.SetText( m_strX );
	
	// force the control to redraw itself
	InvalidateControl();
}

int CSemCoreCtrlPanel_Stand4Ctrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CRect	rc(0, 0, lpCreateStruct->cx, lpCreateStruct->cy);
	DWORD	dwStyle = WS_CHILD | WS_VISIBLE | SS_SUNKEN;

	if ( !m_ctrlDigiMeter.Create(NULL, dwStyle, rc, this) )
		return -1;

	m_ctrlDigiMeter.SetWindowPos(NULL,
		rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
	m_ctrlDigiMeter.ShowWindow(SW_SHOW);

	m_ctrlDigiMeter.SetText( m_strX );
		
	return 0;
}

void CSemCoreCtrlPanel_Stand4Ctrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect	rc(0, 0, cx, cy);

	m_ctrlDigiMeter.SetWindowPos(NULL,
		rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
}
