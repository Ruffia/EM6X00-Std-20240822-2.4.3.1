// SemCoreCtrlPanel_Stand5Ctl.cpp : Implementation of the CSemCoreCtrlPanel_Stand5Ctrl ActiveX Control class.
//
// Last Modify : 2009.02.16

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand5.h"
#include "3DMeterCtrl.h"
#include "SemCoreCtrlPanel_Stand5Ctl.h"
#include "SemCoreCtrlPanel_Stand5Ppg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_Stand5Ctrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_Stand5Ctrl, COleControl)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_Stand5Ctrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CSemCoreCtrlPanel_Stand5Ctrl, COleControl)
	//{{AFX_DISPATCH_MAP(CSemCoreCtrlPanel_Stand5Ctrl)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand5Ctrl, "ControlStyle", m_nControlStyle, OnControlStyleChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand5Ctrl, "ScaleDecimals", m_nScaleDecimals, OnScaleDecimalsChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand5Ctrl, "ValueDecimals", m_nValueDecimals, OnValueDecimalsChanged, VT_I2)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand5Ctrl, "Bind", Bind, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand5Ctrl, "Enable", Enable, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand5Ctrl, "SetRange", SetRange, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand5Ctrl, "SetPos", SetPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand5Ctrl, "SyncPos", SyncPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand5Ctrl, "SyncText", SyncText, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_VARIANT)
	DISP_STOCKPROP_CAPTION()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CSemCoreCtrlPanel_Stand5Ctrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CSemCoreCtrlPanel_Stand5Ctrl, COleControl)
	//{{AFX_EVENT_MAP(CSemCoreCtrlPanel_Stand5Ctrl)
	EVENT_CUSTOM("ReportPos", FireReportPos, VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSemCoreCtrlPanel_Stand5Ctrl, 1)
	PROPPAGEID(CSemCoreCtrlPanel_Stand5PropPage::guid)
END_PROPPAGEIDS(CSemCoreCtrlPanel_Stand5Ctrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_Stand5Ctrl, "SEMCORECTRLPANELSTAND5.SemCoreCtrlPanelStand5Ctrl.1",
	0x96e0ed82, 0xf866, 0x44f0, 0x9b, 0xeb, 0xe0, 0x70, 0x19, 0x45, 0xfc, 0x1c)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CSemCoreCtrlPanel_Stand5Ctrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DSemCoreCtrlPanel_Stand5 =
		{ 0x23e28cb6, 0x7ac0, 0x4401, { 0x83, 0x3d, 0x3a, 0xde, 0xd9, 0xbf, 0xf7, 0x99 } };
const IID BASED_CODE IID_DSemCoreCtrlPanel_Stand5Events =
		{ 0x61b3bb3d, 0x9913, 0x4370, { 0x86, 0x97, 0x70, 0x4b, 0x1b, 0xf7, 0x7b, 0x93 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwSemCoreCtrlPanel_Stand5OleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSemCoreCtrlPanel_Stand5Ctrl, IDS_SEMCORECTRLPANEL_STAND5, _dwSemCoreCtrlPanel_Stand5OleMisc)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand5Ctrl::CSemCoreCtrlPanel_Stand5CtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_Stand5Ctrl

BOOL CSemCoreCtrlPanel_Stand5Ctrl::CSemCoreCtrlPanel_Stand5CtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_SEMCORECTRLPANEL_STAND5,
			IDB_SEMCORECTRLPANEL_STAND5,
			afxRegApartmentThreading,
			_dwSemCoreCtrlPanel_Stand5OleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand5Ctrl::CSemCoreCtrlPanel_Stand5Ctrl - Constructor

CSemCoreCtrlPanel_Stand5Ctrl::CSemCoreCtrlPanel_Stand5Ctrl()
{
	InitializeIIDs(&IID_DSemCoreCtrlPanel_Stand5, &IID_DSemCoreCtrlPanel_Stand5Events);

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
// CSemCoreCtrlPanel_Stand5Ctrl::~CSemCoreCtrlPanel_Stand5Ctrl - Destructor

CSemCoreCtrlPanel_Stand5Ctrl::~CSemCoreCtrlPanel_Stand5Ctrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand5Ctrl::OnDraw - Drawing function

void CSemCoreCtrlPanel_Stand5Ctrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	if( m_ctrl3DMeter.m_hWnd != NULL )
		return;

	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(COLOR_3DFACE)));

	//////////////////////////////////////////////////////
	CRect	rectClient, rectCtrl;
	rectClient = rcBounds;
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	// Draw Rectangle
	//////////////////////////////////////////////////////
	rectCtrl = rectClient;

	CBrush*		pOldBrush;
	CPen*		pOldPen;
	
	pOldBrush = pdc->SelectObject( CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE)) );
	pOldPen = (CPen*)pdc->SelectStockObject(BLACK_PEN);

	pdc->Rectangle(rectCtrl);

	pdc->SelectObject(pOldPen);
	pdc->SelectObject(pOldBrush);
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	// Draw Text
	//////////////////////////////////////////////////////
	rectCtrl = rectClient;

	TEXTMETRIC	tm;
	pdc->GetTextMetrics(&tm);
	//////////////////////////////////////////////////////

	CPoint	Center;
	Center = rectCtrl.CenterPoint();
	Center.y += tm.tmHeight / 2;

	CString		csTextOut = InternalGetText();

	//////////////////////////////////////////////////////
	pdc->SetBkMode(TRANSPARENT);
	pdc->SetTextAlign(TA_CENTER | TA_BOTTOM);
	pdc->ExtTextOut(Center.x, Center.y,
		ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);
	//////////////////////////////////////////////////////

	
	//////////////////////////////////////////////////////
	// Draw Label
	//////////////////////////////////////////////////////
	rectCtrl = rectClient;

	//////////////////////////////////////////////////////
	Center = rectCtrl.CenterPoint();
	Center.y = rectCtrl.bottom - (int)(tm.tmHeight * 0.25);

	csTextOut = _T("3DMeter");
	//////////////////////////////////////////////////////
	pdc->SetBkMode(TRANSPARENT);
	pdc->SetTextAlign(TA_CENTER | TA_BOTTOM);
	pdc->ExtTextOut(Center.x, Center.y,
		ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);
	//////////////////////////////////////////////////////
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand5Ctrl::DoPropExchange - Persistence support

void CSemCoreCtrlPanel_Stand5Ctrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
	if (pPX->GetVersion() ==(DWORD)MAKELONG(_wVerMinor, _wVerMajor))
	{
		PX_Short(pPX, _T("ControlStyle"), m_nControlStyle, 0);
		PX_Short(pPX, _T("ScaleDecimals"), m_nScaleDecimals, 0);
		PX_Short(pPX, _T("ValueDecimals"), m_nValueDecimals, 0);
	}
	else if (pPX->IsLoading())
	{
		// Skip over persistent data
		short nDummy;
		PX_Short(pPX, _T("ControlStyle"), nDummy, 0);
		PX_Short(pPX, _T("ScaleDecimals"), nDummy, 0);
		PX_Short(pPX, _T("ValueDecimals"), nDummy, 0);
		
		// Force property values to these defaults
		m_nControlStyle = 0;
		m_nScaleDecimals = 0;
		m_nValueDecimals = 0;
	}

	if ( m_nControlStyle != 0 )
	{
		m_scfCtrlX.varMin.dblVal = 0;
		m_scfCtrlX.varMax.dblVal = 100;
		m_scfCtrlX.varPos.dblVal = 10;
	}
	else
	{
		m_scfCtrlX.varMin.lVal = 0;
		m_scfCtrlX.varMax.lVal = 255;
		m_scfCtrlX.varPos.lVal = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand5Ctrl::OnResetState - Reset control to default state

void CSemCoreCtrlPanel_Stand5Ctrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand5Ctrl::AboutBox - Display an "About" box to the user

void CSemCoreCtrlPanel_Stand5Ctrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_SEMCORECTRLPANEL_STAND5);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand5Ctrl message handlers

void CSemCoreCtrlPanel_Stand5Ctrl::Bind(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber) 
{
	// TODO: Add your dispatch handler code here
	m_varPanelClassID		= ClassID;
	m_varPanelSerialNumber	= SerialNumber;
}

void CSemCoreCtrlPanel_Stand5Ctrl::Enable(const VARIANT FAR& Enabled, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
}

void CSemCoreCtrlPanel_Stand5Ctrl::SetRange(const VARIANT FAR& Xmin, const VARIANT FAR& Xmax, const VARIANT FAR& Ymin, const VARIANT FAR& Ymax) 
{
	// TODO: Add your dispatch handler code here
	m_scfCtrlX.varMin = Xmin;
	m_scfCtrlX.varMax = Xmax;
	
	m_scfCtrlY.varMin = Ymin;
	m_scfCtrlY.varMax = Ymax;

	if( m_ctrl3DMeter.m_hWnd == NULL )
		return;

	if ( m_nControlStyle != 0 )
	{
		const double&	dblXMin = m_scfCtrlX.varMin.dblVal;
		const double&	dblXMax = m_scfCtrlX.varMax.dblVal;

		m_ctrl3DMeter.SetRange( dblXMin, dblXMax );
	}
	else
	{
		const long&	lXMin = m_scfCtrlX.varMin.lVal;
		const long&	lXMax = m_scfCtrlX.varMax.lVal;

		m_ctrl3DMeter.SetRange( lXMin, lXMax );
	}

}

void CSemCoreCtrlPanel_Stand5Ctrl::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;

	VARIANT	XPosNew, YPosNew, FlagNew;

	if ( m_nControlStyle != 0 )
	{
		const double&	dblXMin = m_scfCtrlX.varMin.dblVal;
		const double&	dblXMax = m_scfCtrlX.varMax.dblVal;
		double&	dblXPos = m_scfCtrlX.varPos.dblVal;
		
		double	dblXPosNew = Xpos.dblVal;
		
		if (dblXPosNew < dblXMin)
			dblXPosNew = dblXMin;
		if (dblXPosNew > dblXMax)
			dblXPosNew = dblXMax;

		if (dblXPosNew != dblXPos)
		{
			XPosNew.dblVal	= dblXPosNew;
			YPosNew.dblVal	= 0;
			FlagNew.ulVal	= FlagCtrlX;

			SyncPos(XPosNew, YPosNew, FlagNew);
			FireReportPos(m_varPanelClassID, m_varPanelSerialNumber, XPosNew, YPosNew, FlagNew);
		}
	}
	else
	{
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
			XPosNew.lVal	= lXPosNew;
			YPosNew.lVal	= 0;
			FlagNew.ulVal	= FlagCtrlX;

			SyncPos(XPosNew, YPosNew, FlagNew);
			FireReportPos(m_varPanelClassID, m_varPanelSerialNumber, XPosNew, YPosNew, FlagNew);
		}
	}
	
}

void CSemCoreCtrlPanel_Stand5Ctrl::SyncPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;

	if( m_ctrl3DMeter.m_hWnd == NULL )
		return;

	if ( m_nControlStyle != 0 )
	{
		double&	dblXPos = m_scfCtrlX.varPos.dblVal;
		dblXPos = Xpos.dblVal;

		m_ctrl3DMeter.UpdateNeedle( dblXPos );
	}
	else
	{
		long&	lXPos = m_scfCtrlX.varPos.lVal;
		lXPos = Xpos.lVal;

		m_ctrl3DMeter.UpdateNeedle( lXPos );
	}
	
	InvalidateControl();
}

void CSemCoreCtrlPanel_Stand5Ctrl::SyncText(LPCTSTR strX, LPCTSTR strY, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;
	
	m_strX = _T(strX);
	// m_strY;

	// force the control to redraw itself
	InvalidateControl();
}

int CSemCoreCtrlPanel_Stand5Ctrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CRect	rc(0, 0, lpCreateStruct->cx, lpCreateStruct->cy);
	DWORD	dwStyle = WS_CHILD | WS_VISIBLE | SS_SUNKEN;

	if ( !m_ctrl3DMeter.Create(NULL, dwStyle, rc, this) )
		return -1;

	CString csUnits = InternalGetText();
	m_ctrl3DMeter.SetUnits( csUnits );

	if ( m_nControlStyle != 0 )
	{
		const double&	dblXMin = m_scfCtrlX.varMin.dblVal;
		const double&	dblXMax = m_scfCtrlX.varMax.dblVal;
		const double&	dblXPos = m_scfCtrlX.varPos.dblVal;

		m_ctrl3DMeter.SetValueDecimals( m_nValueDecimals );
		m_ctrl3DMeter.SetScaleDecimals( m_nScaleDecimals );
		m_ctrl3DMeter.SetRange( dblXMin, dblXMax );
		m_ctrl3DMeter.UpdateNeedle( dblXPos );
	}
	else
	{
		const long&	lXMin = m_scfCtrlX.varMin.lVal;
		const long&	lXMax = m_scfCtrlX.varMax.lVal;
		const long&	lXPos = m_scfCtrlX.varPos.lVal;

		m_ctrl3DMeter.SetValueDecimals( 0 );
		m_ctrl3DMeter.SetScaleDecimals( 0 );
		m_ctrl3DMeter.SetRange( lXMin, lXMax );
		m_ctrl3DMeter.UpdateNeedle( lXPos );
	}

	return 0;
}

void CSemCoreCtrlPanel_Stand5Ctrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if( m_ctrl3DMeter.m_hWnd == NULL )
		return;

	CRect	rc(0, 0, cx, cy);

	m_ctrl3DMeter.SetWindowPos(NULL,
		rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER);
	
}

void CSemCoreCtrlPanel_Stand5Ctrl::OnControlStyleChanged() 
{
	// TODO: Add notification handler code
	if ( m_nControlStyle != 0 )
	{
		m_scfCtrlX.varMin.dblVal = 0;
		m_scfCtrlX.varMax.dblVal = 100;
		m_scfCtrlX.varPos.dblVal = 10;
	}
	else
	{
		m_scfCtrlX.varMin.lVal = 0;
		m_scfCtrlX.varMax.lVal = 255;
		m_scfCtrlX.varPos.lVal = 0;
	}

	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand5Ctrl::OnScaleDecimalsChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand5Ctrl::OnValueDecimalsChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

// 09.02.16 添加消息响应以同步文字显示
void CSemCoreCtrlPanel_Stand5Ctrl::OnTextChanged() 
{
	// TODO: Add your specialized code here and/or call the base class
	COleControl::OnTextChanged();

	if( m_ctrl3DMeter.m_hWnd == NULL )
		return;
	CString csUnits = InternalGetText();
	m_ctrl3DMeter.SetUnits( csUnits );
}
