// SemCoreCtrlPanel_Stand2Ctl.cpp : Implementation of the CSemCoreCtrlPanel_Stand2Ctrl ActiveX Control class.
//
// Last Modify : 2009.02.17

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand2.h"
#include "MoveXYCtl.h"
#include "SemCoreCtrlPanel_Stand2Ctl.h"
#include "SemCoreCtrlPanel_Stand2Ppg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_Stand2Ctrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_Stand2Ctrl, COleControl)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_Stand2Ctrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CSemCoreCtrlPanel_Stand2Ctrl, COleControl)
	//{{AFX_DISPATCH_MAP(CSemCoreCtrlPanel_Stand2Ctrl)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand2Ctrl, "CaptionVisible", m_bCaptionVisible, OnCaptionVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand2Ctrl, "DebugInfoVisible", m_bDebugInfoVisible, OnDebugInfoVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand2Ctrl, "XMinString", m_csXMinString, OnXMinStringChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand2Ctrl, "XMaxString", m_csXMaxString, OnXMaxStringChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand2Ctrl, "YMinString", m_csYMinString, OnYMinStringChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand2Ctrl, "YMaxString", m_csYMaxString, OnYMaxStringChanged, VT_BSTR)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand2Ctrl, "Bind", Bind, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand2Ctrl, "Enable", Enable, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand2Ctrl, "SetRange", SetRange, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand2Ctrl, "SetPos", SetPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand2Ctrl, "SyncPos", SyncPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand2Ctrl, "SyncText", SyncText, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand2Ctrl, "SyncString", SyncString, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_STOCKPROP_BACKCOLOR()
	DISP_STOCKPROP_FORECOLOR()
	DISP_STOCKPROP_FONT()
	DISP_STOCKPROP_CAPTION()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CSemCoreCtrlPanel_Stand2Ctrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CSemCoreCtrlPanel_Stand2Ctrl, COleControl)
	//{{AFX_EVENT_MAP(CSemCoreCtrlPanel_Stand2Ctrl)
	EVENT_CUSTOM("ReportPos", FireReportPos, VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSemCoreCtrlPanel_Stand2Ctrl, 3)
	PROPPAGEID(CSemCoreCtrlPanel_Stand2PropPage::guid)
	PROPPAGEID(CLSID_CColorPropPage)
	PROPPAGEID(CLSID_CFontPropPage)
END_PROPPAGEIDS(CSemCoreCtrlPanel_Stand2Ctrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_Stand2Ctrl, "SEMCORECTRLPANELSTAND2.SemCoreCtrlPanelStand2Ctrl.1",
	0x2f96e150, 0x5951, 0x4817, 0x8c, 0xec, 0xac, 0x17, 0x29, 0x56, 0x91, 0x25)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CSemCoreCtrlPanel_Stand2Ctrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DSemCoreCtrlPanel_Stand2 =
		{ 0x1a6d957e, 0xbefd, 0x4ee8, { 0xa2, 0xba, 0x7b, 0x21, 0x6, 0x25, 0xa2, 0xce } };
const IID BASED_CODE IID_DSemCoreCtrlPanel_Stand2Events =
		{ 0x40b3cee8, 0x606f, 0x47b0, { 0xaf, 0x16, 0x4, 0xbb, 0xe0, 0xe4, 0x86, 0xe9 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwSemCoreCtrlPanel_Stand2OleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSemCoreCtrlPanel_Stand2Ctrl, IDS_SEMCORECTRLPANEL_STAND2, _dwSemCoreCtrlPanel_Stand2OleMisc)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand2Ctrl::CSemCoreCtrlPanel_Stand2CtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_Stand2Ctrl

BOOL CSemCoreCtrlPanel_Stand2Ctrl::CSemCoreCtrlPanel_Stand2CtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_SEMCORECTRLPANEL_STAND2,
			IDB_SEMCORECTRLPANEL_STAND2,
			afxRegApartmentThreading,
			_dwSemCoreCtrlPanel_Stand2OleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand2Ctrl::CSemCoreCtrlPanel_Stand2Ctrl - Constructor

CSemCoreCtrlPanel_Stand2Ctrl::CSemCoreCtrlPanel_Stand2Ctrl()
{
	InitializeIIDs(&IID_DSemCoreCtrlPanel_Stand2, &IID_DSemCoreCtrlPanel_Stand2Events);

	// TODO: Initialize your control's instance data here.
	m_scfCtrlX.varMin.lVal = -128;
	m_scfCtrlX.varMax.lVal = 127;
	m_scfCtrlX.varPos.lVal = 0;

	m_scfCtrlY = m_scfCtrlX;

	m_strX = _T("-X-");
	m_strY = _T("-Y-");

	m_varPanelSerialNumber.lVal = -1;	
}

/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand2Ctrl::~CSemCoreCtrlPanel_Stand2Ctrl - Destructor

CSemCoreCtrlPanel_Stand2Ctrl::~CSemCoreCtrlPanel_Stand2Ctrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand2Ctrl::OnDraw - Drawing function
#define SCROLLBAR_Width			16
#define EDIT_DISPLAY_Width		21
#define Interval_Width_Inside	1
#define Interval_Width_Outside	6


void CSemCoreCtrlPanel_Stand2Ctrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	const CString& m_Title = InternalGetText();

	m_MoveXYCtl.m_strCaption = m_Title;
	m_MoveXYCtl.m_bCaptionVisible = m_bCaptionVisible;
	if ( m_MoveXYCtl.m_hWnd != NULL)
		return;

	pdc->FillRect(rcBounds, CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE)));
	//pdc->Ellipse(rcBounds);

	//////////////////////////////////////////////////////
	CRect	 rectClient, rectCtrl;
	rectClient = rcBounds;

	//////////////////////////////////////////////////////
	// Draw IDC_STATIC_Frame
	//////////////////////////////////////////////////////
	rectCtrl.left	= rectClient.left + Interval_Width_Outside;
	rectCtrl.right	= rectClient.right - Interval_Width_Inside
						- SCROLLBAR_Width - Interval_Width_Outside;

	rectCtrl.top	= rectClient.top + Interval_Width_Outside;
	rectCtrl.bottom = rectClient.bottom - Interval_Width_Inside
						- SCROLLBAR_Width - Interval_Width_Inside
						- EDIT_DISPLAY_Width - Interval_Width_Outside;

	pdc->FillRect(rectCtrl, CBrush::FromHandle(GetSysColorBrush(COLOR_WINDOW)));
	pdc->FrameRect(rectCtrl, CBrush::FromHandle(GetSysColorBrush(COLOR_BTNTEXT)));
	//////////////////////////////////////////////////////

		CPen* pOldPen;
		CPen  TempPen;
		CPoint FrameCenter = rectCtrl.CenterPoint();

		//////////////////////////////////////////////////////
		// FrameCenter
		//////////////////////////////////////////////////////
		TempPen.CreatePen( PS_DASHDOT, 1, RGB(255,200,100) );
		pOldPen = pdc->SelectObject( &TempPen );
		pdc->MoveTo(rectCtrl.left, FrameCenter.y);
		pdc->LineTo(rectCtrl.right,FrameCenter.y);
		pdc->MoveTo(FrameCenter.x, rectCtrl.top);
		pdc->LineTo(FrameCenter.x, rectCtrl.bottom);
		pdc->SelectObject( pOldPen );
		TempPen.DeleteObject();
		//////////////////////////////////////////////////////

		//////////////////////////////////////////////////////
		// Draw Title
		//////////////////////////////////////////////////////
		if ( m_bCaptionVisible )
		{
			CSize TitleSize = pdc->GetTextExtent( m_Title );
			int TitleX, TitleY;
			TitleX = FrameCenter.x - TitleSize.cx / 2;
			TitleY = FrameCenter.y - TitleSize.cy / 2;		
			COLORREF BkColor, TextColor;
			int		 nBkMode;
			BkColor		= pdc->GetBkColor();
			TextColor	= pdc->GetTextColor();
			nBkMode		= pdc->SetBkMode( TRANSPARENT );

			pdc->SetBkColor( GetSysColor( COLOR_WINDOW ) );
			pdc->SetTextColor( RGB(0,0,255) );
				pdc->TextOut( TitleX, TitleY, m_Title );
			pdc->SetBkColor( BkColor );
			pdc->SetTextColor( TextColor );
			pdc->SetBkMode( nBkMode );
		}
		//////////////////////////////////////////////////////

		//////////////////////////////////////////////////////
		// Draw XMin & XMax String
		//////////////////////////////////////////////////////
		CString csTextOut;
		csTextOut = m_csXMinString;
		pdc->SetTextAlign(TA_LEFT | TA_TOP);
		pdc->ExtTextOut(rectCtrl.left+Interval_Width_Inside, FrameCenter.y,
			ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);

		csTextOut = m_csXMaxString;
		pdc->SetTextAlign(TA_RIGHT | TA_BOTTOM);
		pdc->ExtTextOut(rectCtrl.right-Interval_Width_Inside, FrameCenter.y,
			ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);

		csTextOut = m_csYMinString;
		pdc->SetTextAlign(TA_LEFT | TA_BOTTOM);
		pdc->ExtTextOut(FrameCenter.x, rectCtrl.bottom-Interval_Width_Inside,
			ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);

		csTextOut = m_csYMaxString;
		pdc->SetTextAlign(TA_RIGHT | TA_TOP);
		pdc->ExtTextOut(FrameCenter.x, rectCtrl.top+Interval_Width_Inside,
			ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);
		//////////////////////////////////////////////////////

		//////////////////////////////////////////////////////
		// MousePos
		//////////////////////////////////////////////////////
		CPoint m_MousePos(100, 100);
		TempPen.CreatePen( PS_SOLID, 1, RGB(0,0,255) );
		pOldPen = pdc->SelectObject( &TempPen );
		pdc->MoveTo(rectCtrl.left, m_MousePos.y);
		pdc->LineTo(rectCtrl.right,m_MousePos.y);
		pdc->MoveTo(m_MousePos.x, rectCtrl.top);
		pdc->LineTo(m_MousePos.x, rectCtrl.bottom);
		pdc->SelectObject( pOldPen );
		TempPen.DeleteObject();
		//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	// Draw IDC_SCROLLBAR_V
	//////////////////////////////////////////////////////
	rectCtrl.left	= rectClient.right - Interval_Width_Outside
						- SCROLLBAR_Width;
	rectCtrl.right	= rectClient.right - Interval_Width_Outside;


	rectCtrl.top	= rectClient.top + Interval_Width_Outside;
	rectCtrl.bottom = rectClient.bottom - Interval_Width_Inside
						- SCROLLBAR_Width - Interval_Width_Inside
						- EDIT_DISPLAY_Width - Interval_Width_Outside;
		
	pdc->FillRect(rectCtrl, CBrush::FromHandle(GetSysColorBrush(COLOR_3DSHADOW)));
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	// Draw IDC_SCROLLBAR_H
	//////////////////////////////////////////////////////
	rectCtrl.left	= rectClient.left + Interval_Width_Outside;
	rectCtrl.right	= rectClient.right - Interval_Width_Inside
						- SCROLLBAR_Width - Interval_Width_Outside;

	rectCtrl.top	= rectClient.bottom
						- SCROLLBAR_Width - Interval_Width_Inside
						- EDIT_DISPLAY_Width - Interval_Width_Outside;
	rectCtrl.bottom = rectClient.bottom	- Interval_Width_Inside
						- EDIT_DISPLAY_Width - Interval_Width_Outside;

	pdc->FillRect(rectCtrl, CBrush::FromHandle(GetSysColorBrush(COLOR_3DSHADOW)));
	//////////////////////////////////////////////////////


	//////////////////////////////////////////////////////
	// Draw IDC_STATIC_Display
	//////////////////////////////////////////////////////
	rectCtrl.left	= rectClient.left + Interval_Width_Outside;
	rectCtrl.right	= rectClient.right - Interval_Width_Outside;

	rectCtrl.top	= rectClient.bottom + 1
						- EDIT_DISPLAY_Width - Interval_Width_Outside;
	rectCtrl.bottom = rectClient.bottom - Interval_Width_Outside;
		
	pdc->FrameRect(rectCtrl, CBrush::FromHandle(GetSysColorBrush(COLOR_GRAYTEXT)));
	//////////////////////////////////////////////////////
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand2Ctrl::DoPropExchange - Persistence support

void CSemCoreCtrlPanel_Stand2Ctrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
	if (pPX->GetVersion() == (DWORD)MAKELONG(_wVerMinor, _wVerMajor))
	{
		PX_Bool(pPX, _T("CaptionVisible"), m_bCaptionVisible, TRUE);
		PX_Bool(pPX, _T("DebugInfoVisible"), m_bDebugInfoVisible, TRUE);
		PX_String(pPX, _T("XMinString"), m_csXMinString, _T(""));
		PX_String(pPX, _T("XMaxString"), m_csXMaxString, _T(""));
		PX_String(pPX, _T("YMinString"), m_csYMinString, _T(""));
		PX_String(pPX, _T("YMaxString"), m_csYMaxString, _T(""));
	}
	else if (pPX->IsLoading())
	{
		// Skip over persistent data
		BOOL bDummy;
		CString strDummy;
		PX_Bool(pPX, _T("CaptionVisible"), bDummy, FALSE);
		PX_Bool(pPX, _T("DebugInfoVisible"), bDummy, FALSE);
		PX_String(pPX, _T("XMinString"), strDummy, _T(""));
		PX_String(pPX, _T("XMaxString"), strDummy, _T(""));
		PX_String(pPX, _T("YMinString"), strDummy, _T(""));
		PX_String(pPX, _T("YMaxString"), strDummy, _T(""));

		// Force property values to these defaults
		m_bCaptionVisible = TRUE;
		m_bDebugInfoVisible = TRUE;
		m_csXMinString = _T("XMin");
		m_csXMaxString = _T("XMax");
		m_csYMinString = _T("YMin");
		m_csYMaxString = _T("YMax");
	}

	m_MoveXYCtl.m_bCaptionVisible	= m_bCaptionVisible;
	m_MoveXYCtl.m_bDebugInfoVisible = m_bDebugInfoVisible;
	m_MoveXYCtl.m_csXMinString = m_csXMinString;
	m_MoveXYCtl.m_csXMaxString = m_csXMaxString;
	m_MoveXYCtl.m_csYMinString = m_csYMinString;
	m_MoveXYCtl.m_csYMaxString = m_csYMaxString;
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand2Ctrl::OnResetState - Reset control to default state

void CSemCoreCtrlPanel_Stand2Ctrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand2Ctrl::AboutBox - Display an "About" box to the user

void CSemCoreCtrlPanel_Stand2Ctrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_SEMCORECTRLPANEL_STAND2);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand2Ctrl message handlers

int CSemCoreCtrlPanel_Stand2Ctrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if ( !m_MoveXYCtl.Bind( this ) )
		return -1;
	
	// 初始化 MoveXYCtl
	m_MoveXYCtl.SetRange(
		m_scfCtrlX.varMin.lVal,
		m_scfCtrlX.varMax.lVal,
		m_scfCtrlY.varMin.lVal,
		m_scfCtrlY.varMax.lVal );

	m_MoveXYCtl.SetPos(
		m_scfCtrlX.varPos.lVal,
		m_scfCtrlY.varPos.lVal );
	
	if ( m_MoveXYCtl.Create(CMoveXYCtl::IDD, this) != 0 )
	{
		m_MoveXYCtl.ShowWindow( SW_SHOW );
		m_MoveXYCtl.SetFont( CFont::FromHandle(m_font.GetFontHandle()) );
	}

	return 0;
}

void CSemCoreCtrlPanel_Stand2Ctrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_MoveXYCtl.SetWindowPos( NULL, 0, 0, cx, cy, SWP_NOZORDER );
	
}

void CSemCoreCtrlPanel_Stand2Ctrl::Bind(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber) 
{
	// TODO: Add your dispatch handler code here
	m_varPanelClassID		= ClassID;
	m_varPanelSerialNumber	= SerialNumber;

}

void CSemCoreCtrlPanel_Stand2Ctrl::Enable(const VARIANT FAR& Enabled, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;

	const BOOL& blEnabled = Enabled.boolVal;

	if (m_MoveXYCtl.m_hWnd != NULL)
		m_MoveXYCtl.EnableWindow( blEnabled );

}

void CSemCoreCtrlPanel_Stand2Ctrl::SetRange(const VARIANT FAR& Xmin, const VARIANT FAR& Xmax, const VARIANT FAR& Ymin, const VARIANT FAR& Ymax) 
{
	// TODO: Add your dispatch handler code here
	m_scfCtrlX.varMin = Xmin;
	m_scfCtrlX.varMax = Xmax;

	m_scfCtrlY.varMin = Ymin;
	m_scfCtrlY.varMax = Ymax;

	const long&	lXMin = m_scfCtrlX.varMin.lVal;
	const long&	lXMax = m_scfCtrlX.varMax.lVal;
	const long&	lYMin = m_scfCtrlY.varMin.lVal;
	const long&	lYMax = m_scfCtrlY.varMax.lVal;
	m_MoveXYCtl.SetRange(lXMin, lXMax, lYMin, lYMax);
}

void CSemCoreCtrlPanel_Stand2Ctrl::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	const long&	lXMin = m_scfCtrlX.varMin.lVal;
	const long&	lXMax = m_scfCtrlX.varMax.lVal;
	const long&	lYMin = m_scfCtrlY.varMin.lVal;
	const long&	lYMax = m_scfCtrlY.varMax.lVal;
	
	long&	lXPos = m_scfCtrlX.varPos.lVal;
	long&	lYPos = m_scfCtrlY.varPos.lVal;

	long	lXPosNew = Xpos.lVal;
	long	lYPosNew = Ypos.lVal;

	if( lXPosNew < lXMin )	lXPosNew = lXMin;
	if( lXPosNew > lXMax )	lXPosNew = lXMax;

	if( lYPosNew < lYMin )	lYPosNew = lYMin;
	if( lYPosNew > lYMax )	lYPosNew = lYMax;

	VARIANT	XPosNew, YPosNew, FlagNew;
	FlagNew.ulVal = 0;

	if ( (Flag.ulVal & FlagCtrlX) == FlagCtrlX )
	{
		if( lXPosNew != lXPos )
		{
			XPosNew.lVal	= lXPosNew;
			FlagNew.ulVal	|= FlagCtrlX;
		}
	}
	if ( (Flag.ulVal & FlagCtrlY) == FlagCtrlY )
	{
		if( lYPosNew != lYPos )
		{
			YPosNew.lVal	= lYPosNew;
			FlagNew.ulVal	|= FlagCtrlY;
		}
	}

	if( Flag.ulVal != 0 )
	{
		SyncPos( XPosNew, YPosNew, FlagNew );
		FireReportPos( m_varPanelClassID, m_varPanelSerialNumber, XPosNew, YPosNew, FlagNew );
	}
}

void CSemCoreCtrlPanel_Stand2Ctrl::SyncPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( Flag.ulVal == 0 )
		return;

	// 2008.12.17保证新值在范围内
	const long&	lXMin = m_scfCtrlX.varMin.lVal;
	const long&	lXMax = m_scfCtrlX.varMax.lVal;
	const long&	lYMin = m_scfCtrlY.varMin.lVal;
	const long&	lYMax = m_scfCtrlY.varMax.lVal;
	// 2008.12.17保证新值在范围内

	long&	lXPos = m_scfCtrlX.varPos.lVal;
	long&	lYPos = m_scfCtrlY.varPos.lVal;

	if ( (Flag.ulVal & FlagCtrlX) == FlagCtrlX )
		lXPos = Xpos.lVal;

	if ( (Flag.ulVal & FlagCtrlY) == FlagCtrlY )
		lYPos = Ypos.lVal;

	// 2008.12.17保证新值在范围内
	if( lXPos < lXMin )	lXPos = lXMin;
	if( lXPos > lXMax )	lXPos = lXMax;

	if( lYPos < lYMin )	lYPos = lYMin;
	if( lYPos > lYMax )	lYPos = lYMax;
	// 2008.12.17保证新值在范围内

	m_MoveXYCtl.SetPos( lXPos, lYPos );

	InvalidateControl();
}

void CSemCoreCtrlPanel_Stand2Ctrl::SyncText(LPCTSTR strX, LPCTSTR strY, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( Flag.ulVal == 0 )
		return;

	if ( (Flag.ulVal & FlagCtrlX) == FlagCtrlX )
		m_strX = _T(strX);

	if ( (Flag.ulVal & FlagCtrlY) == FlagCtrlY )
		m_strY = _T(strY);

	// force the control to redraw itself
	m_MoveXYCtl.Invalidate(FALSE);
	InvalidateControl();
}

void CSemCoreCtrlPanel_Stand2Ctrl::SyncString(const VARIANT FAR& ResourceID, const VARIANT FAR& ResourceString) 
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

// 09.02.16 添加消息响应以同步文字显示
void CSemCoreCtrlPanel_Stand2Ctrl::OnTextChanged() 
{
	// TODO: Add your specialized code here and/or call the base class
	COleControl::OnTextChanged();

	if( m_MoveXYCtl.m_hWnd == NULL )
		return;

	const CString& m_Title = InternalGetText();
	if( m_MoveXYCtl.m_strCaption != m_Title )
	{
		m_MoveXYCtl.m_strCaption = m_Title;
		m_MoveXYCtl.Invalidate(FALSE);
	}
}

void CSemCoreCtrlPanel_Stand2Ctrl::OnCaptionVisibleChanged() 
{
	// TODO: Add notification handler code
	if( m_MoveXYCtl.m_hWnd != NULL )
	{
		m_MoveXYCtl.m_bCaptionVisible	= m_bCaptionVisible;
		// 09.02.17
		m_MoveXYCtl.Invalidate(FALSE);
	}

	// force the control to redraw itself
	InvalidateControl();

	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand2Ctrl::OnDebugInfoVisibleChanged() 
{
	// TODO: Add notification handler code
	if( m_MoveXYCtl.m_hWnd != NULL )
	{
		m_MoveXYCtl.m_bDebugInfoVisible = m_bDebugInfoVisible;
		// 09.02.17
		m_MoveXYCtl.Invalidate(FALSE);
	}

	// force the control to redraw itself
	InvalidateControl();

	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand2Ctrl::OnXMinStringChanged() 
{
	// TODO: Add notification handler code
	if( m_MoveXYCtl.m_hWnd != NULL )
	{
		m_MoveXYCtl.m_csXMinString = m_csXMinString;
		// 09.02.17
		m_MoveXYCtl.Invalidate(FALSE);
	}

	// force the control to redraw itself
	InvalidateControl();

	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand2Ctrl::OnXMaxStringChanged() 
{
	// TODO: Add notification handler code
	if( m_MoveXYCtl.m_hWnd != NULL )
	{
		m_MoveXYCtl.m_csXMaxString = m_csXMaxString;
		// 09.02.17
		m_MoveXYCtl.Invalidate(FALSE);
	}
	
	// force the control to redraw itself
	InvalidateControl();

	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand2Ctrl::OnYMinStringChanged() 
{
	// TODO: Add notification handler code
	if( m_MoveXYCtl.m_hWnd != NULL )
	{
		m_MoveXYCtl.m_csYMinString = m_csYMinString;
		// 09.02.17
		m_MoveXYCtl.Invalidate(FALSE);
	}

	// force the control to redraw itself
	InvalidateControl();

	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand2Ctrl::OnYMaxStringChanged() 
{
	// TODO: Add notification handler code
	if( m_MoveXYCtl.m_hWnd != NULL )
	{
		m_MoveXYCtl.m_csYMaxString = m_csYMaxString;
		// 09.02.17
		m_MoveXYCtl.Invalidate(FALSE);
	}

	// force the control to redraw itself
	InvalidateControl();

	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand2Ctrl::ReportPos( int nX, int nY )
{
	VARIANT	Flag;
	long&	lXPos = m_scfCtrlX.varPos.lVal;
	long&	lYPos = m_scfCtrlY.varPos.lVal;

	Flag.ulVal = 0;
	if ( lXPos != nX )
	{
		lXPos = nX;
		Flag.ulVal |= FlagCtrlX;
	}
	if ( lYPos != nY )
	{
		lYPos = nY;
		Flag.ulVal |= FlagCtrlY;
	}

	if ( Flag.ulVal != 0 )
	{
		FireReportPos( m_varPanelClassID, m_varPanelSerialNumber, 
			m_scfCtrlX.varPos, m_scfCtrlY.varPos, Flag );
	}
}
