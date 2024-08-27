// SemCoreCtrlPanel_Stand3Ctl.cpp : Implementation of the CSemCoreCtrlPanel_Stand3Ctrl ActiveX Control class.
//
// Last Modify : 2009.03.11

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand3.h"
#include "RoundSliderDlg.h"
#include "SemCoreCtrlPanel_Stand3Ctl.h"
#include "SemCoreCtrlPanel_Stand3Ppg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_Stand3Ctrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_Stand3Ctrl, COleControl)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_Stand3Ctrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CSemCoreCtrlPanel_Stand3Ctrl, COleControl)
	//{{AFX_DISPATCH_MAP(CSemCoreCtrlPanel_Stand3Ctrl)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand3Ctrl, "CaptionVisible", m_bCaptionVisible, OnCaptionVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand3Ctrl, "DebugInfoVisible", m_bDebugInfoVisible, OnDebugInfoVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand3Ctrl, "GradeVisible", m_bGradeVisible, OnGradeVisibleChanged, VT_BOOL)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand3Ctrl, "Bind", Bind, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand3Ctrl, "Enable", Enable, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand3Ctrl, "SetRange", SetRange, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand3Ctrl, "SetPos", SetPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand3Ctrl, "SyncPos", SyncPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand3Ctrl, "SyncText", SyncText, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand3Ctrl, "SyncResString", SyncResString, VT_EMPTY, VTS_VARIANT VTS_BSTR)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand3Ctrl, "SyncString", SyncString, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_STOCKPROP_BACKCOLOR()
	DISP_STOCKPROP_FORECOLOR()
	DISP_STOCKPROP_FONT()
	DISP_STOCKPROP_CAPTION()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CSemCoreCtrlPanel_Stand3Ctrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CSemCoreCtrlPanel_Stand3Ctrl, COleControl)
	//{{AFX_EVENT_MAP(CSemCoreCtrlPanel_Stand3Ctrl)
	EVENT_CUSTOM("ReportPos", FireReportPos, VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSemCoreCtrlPanel_Stand3Ctrl, 3)
	PROPPAGEID(CSemCoreCtrlPanel_Stand3PropPage::guid)
	PROPPAGEID(CLSID_CColorPropPage)
	PROPPAGEID(CLSID_CFontPropPage)
END_PROPPAGEIDS(CSemCoreCtrlPanel_Stand3Ctrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_Stand3Ctrl, "SEMCORECTRLPANELSTAND3.SemCoreCtrlPanelStand3Ctrl.1",
	0xa7fdf08d, 0x50e0, 0x4e4e, 0xb1, 0xc5, 0x21, 0xc9, 0x23, 0xb9, 0x85, 0x4)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CSemCoreCtrlPanel_Stand3Ctrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DSemCoreCtrlPanel_Stand3 =
		{ 0x94f7d89b, 0x2bc5, 0x4f43, { 0xa7, 0xee, 0x7d, 0x7a, 0xa6, 0xa9, 0x8c, 0x55 } };
const IID BASED_CODE IID_DSemCoreCtrlPanel_Stand3Events =
		{ 0x7ce808fa, 0x2c8c, 0x4c35, { 0xb6, 0xe2, 0xa6, 0xe7, 0x31, 0x3c, 0xdd, 0xe7 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwSemCoreCtrlPanel_Stand3OleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSemCoreCtrlPanel_Stand3Ctrl, IDS_SEMCORECTRLPANEL_STAND3, _dwSemCoreCtrlPanel_Stand3OleMisc)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand3Ctrl::CSemCoreCtrlPanel_Stand3CtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_Stand3Ctrl

BOOL CSemCoreCtrlPanel_Stand3Ctrl::CSemCoreCtrlPanel_Stand3CtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_SEMCORECTRLPANEL_STAND3,
			IDB_SEMCORECTRLPANEL_STAND3,
			afxRegApartmentThreading,
			_dwSemCoreCtrlPanel_Stand3OleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand3Ctrl::CSemCoreCtrlPanel_Stand3Ctrl - Constructor

CSemCoreCtrlPanel_Stand3Ctrl::CSemCoreCtrlPanel_Stand3Ctrl()
{
	InitializeIIDs(&IID_DSemCoreCtrlPanel_Stand3, &IID_DSemCoreCtrlPanel_Stand3Events);

	// TODO: Initialize your control's instance data here.
	m_scfCtrlX.varMin.lVal = 0;
	m_scfCtrlX.varMax.lVal = 20000 - 1;
	m_scfCtrlX.varPos.lVal = 5000;
	
	m_scfCtrlY = m_scfCtrlX;
	
	m_strX = _T("-X-");
	m_strY = _T("-Y-");
	
	m_varPanelSerialNumber.lVal = -1;
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand3Ctrl::~CSemCoreCtrlPanel_Stand3Ctrl - Destructor

CSemCoreCtrlPanel_Stand3Ctrl::~CSemCoreCtrlPanel_Stand3Ctrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand3Ctrl::OnDraw - Drawing function

void CSemCoreCtrlPanel_Stand3Ctrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	if ( m_dlgRoundSlider.m_hWnd != NULL )
		return;

	pdc->FillRect(rcBounds, CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE)));
	//pdc->Ellipse(rcBounds);

	//////////////////////////////////////////////////////
	CPoint	Center;
	//////////////////////////////////////////////////////

	TEXTMETRIC	tm;
	GetStockTextMetrics(&tm);
	CRect	rcTemp(0,0,32,tm.tmHeight + 4);

	CString		csTextOut;
	CFont*		pOldFont;
	//pdc->SetTextColor(TranslateColor(GetForeColor()));
	pdc->SetBkMode(TRANSPARENT);

	pOldFont = SelectStockFont(pdc);
	//////////////////////////////////////////////////////


	CRect	rectClient, rectCtrl;
	rectClient = rcBounds;

	if (!m_bGradeVisible)
	{		
		rectCtrl = rectClient;
	}
	else
	{		
		//////////////////////////////////////////////////////
		rectCtrl.left	= rectClient.left;
		rectCtrl.right	= rectClient.left + rectClient.Width()/3;

		rectCtrl.top	= rectClient.bottom - rcTemp.Height();
		rectCtrl.bottom = rectClient.bottom;
		
		pdc->FrameRect(rectCtrl, CBrush::FromHandle(GetSysColorBrush(COLOR_BTNTEXT)));
		//////////////////////////////////////////////////////
		csTextOut = _T("粗调");
		Center = rectCtrl.CenterPoint();
		Center.y += tm.tmHeight / 2;
		pdc->SetTextAlign(TA_CENTER | TA_BOTTOM);
		pdc->ExtTextOut(Center.x, Center.y,
			ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);
		//////////////////////////////////////////////////////
			
		//////////////////////////////////////////////////////
		rectCtrl.left	= rectClient.left + rectClient.Width()/3;
		rectCtrl.right	= rectClient.left + rectClient.Width()/3*2;

		rectCtrl.top	= rectClient.bottom - rcTemp.Height();
		rectCtrl.bottom = rectClient.bottom;

		pdc->FrameRect(rectCtrl, CBrush::FromHandle(GetSysColorBrush(COLOR_BTNTEXT)));
		//////////////////////////////////////////////////////
		csTextOut = _T("细调");
		Center = rectCtrl.CenterPoint();
		Center.y += tm.tmHeight / 2;
		pdc->SetTextAlign(TA_CENTER | TA_BOTTOM);
		pdc->ExtTextOut(Center.x, Center.y,
			ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);
		//////////////////////////////////////////////////////

		//////////////////////////////////////////////////////
		rectCtrl.left	= rectClient.left + rectClient.Width()/3*2;
		rectCtrl.right	= rectClient.right;

		rectCtrl.top	= rectClient.bottom - rcTemp.Height();
		rectCtrl.bottom = rectClient.bottom;

		pdc->FrameRect(rectCtrl, CBrush::FromHandle(GetSysColorBrush(COLOR_BTNTEXT)));
		//////////////////////////////////////////////////////
		csTextOut = _T("微调");
		Center = rectCtrl.CenterPoint();
		Center.y += tm.tmHeight / 2;
		pdc->SetTextAlign(TA_CENTER | TA_BOTTOM);
		pdc->ExtTextOut(Center.x, Center.y,
			ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);
		//////////////////////////////////////////////////////

		//////////////////////////////////////////////////////
		//  Ellipse
		//////////////////////////////////////////////////////
		rectCtrl.left	= rectClient.left;
		rectCtrl.right	= rectClient.right;

		rectCtrl.top	= rectClient.top;
		rectCtrl.bottom = rectClient.bottom - rcTemp.Height();
		//////////////////////////////////////////////////////

	}

	//////////////////////////////////////////////////////
	if ( rectCtrl.Width() > rectCtrl.Height() )
	{
		rectCtrl.left	+= ( rectCtrl.Width() - rectCtrl.Height() ) / 2;
		rectCtrl.right	-= ( rectCtrl.Width() - rectCtrl.Height() ) / 2;
	}
	else
	{
		rectCtrl.top	+= ( rectCtrl.Height() - rectCtrl.Width() ) / 2;
		rectCtrl.bottom	-= ( rectCtrl.Height() - rectCtrl.Width() ) / 2;
	}
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	CBrush*		pOldBrush;
	CPen*		pOldPen;
	
	pOldBrush = pdc->SelectObject( CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE)) );
	pOldPen = (CPen*)pdc->SelectStockObject(BLACK_PEN);

	pdc->Ellipse(rectCtrl);

	pdc->SelectObject(pOldPen);
	pdc->SelectObject(pOldBrush);
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	csTextOut = GetDisplayString();
	Center = rectCtrl.CenterPoint();
	Center.y += tm.tmHeight / 2;
	pdc->SetTextAlign(TA_CENTER | TA_BOTTOM);
	pdc->ExtTextOut(Center.x, Center.y,
		ETO_CLIPPED, rectCtrl, csTextOut, csTextOut.GetLength(), NULL);
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	pdc->SelectObject(pOldFont);

}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand3Ctrl::DoPropExchange - Persistence support

void CSemCoreCtrlPanel_Stand3Ctrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
	if (pPX->GetVersion() ==(DWORD)MAKELONG(_wVerMinor, _wVerMajor))
	{
		PX_Bool(pPX, _T("CaptionVisible"), m_bCaptionVisible, TRUE);
		PX_Bool(pPX, _T("DebugInfoVisible"), m_bDebugInfoVisible, TRUE);
		PX_Bool(pPX, _T("GradeVisible"), m_bGradeVisible, TRUE);
	}
	else if (pPX->IsLoading())
	{
		// Skip over persistent data
		BOOL bDummy;
		CString strDummy;
		PX_Bool(pPX, _T("CaptionVisible"), bDummy, FALSE);
		PX_Bool(pPX, _T("DebugInfoVisible"), bDummy, FALSE);
		PX_Bool(pPX, _T("GradeVisible"), bDummy, FALSE);
		
		// Force property values to these defaults
		m_bCaptionVisible = TRUE;
		m_bDebugInfoVisible = TRUE;
		m_bGradeVisible = TRUE;
	}

	//////////////////////////////////////////////////////////////////
	m_dlgRoundSlider.m_bGradeVisible		= m_bGradeVisible;
	m_dlgRoundSlider.m_bDebugInfoVisible	= m_bDebugInfoVisible;

	m_dlgRoundSlider.m_csRoundCtrl			= GetDisplayString();
	//////////////////////////////////////////////////////////////////
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand3Ctrl::OnResetState - Reset control to default state

void CSemCoreCtrlPanel_Stand3Ctrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand3Ctrl::AboutBox - Display an "About" box to the user

void CSemCoreCtrlPanel_Stand3Ctrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_SEMCORECTRLPANEL_STAND3);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand3Ctrl message handlers

void CSemCoreCtrlPanel_Stand3Ctrl::Bind(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber) 
{
	// TODO: Add your dispatch handler code here
	m_varPanelClassID		= ClassID;
	m_varPanelSerialNumber	= SerialNumber;
}

void CSemCoreCtrlPanel_Stand3Ctrl::Enable(const VARIANT FAR& Enabled, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;

	const BOOL& blEnabled = Enabled.boolVal;

	if (m_dlgRoundSlider.m_hWnd != NULL)
	{
		m_dlgRoundSlider.EnableWindow( blEnabled );
		m_dlgRoundSlider.m_ctrlRoundSlider.EnableWindow( blEnabled );
	}
}

void CSemCoreCtrlPanel_Stand3Ctrl::SetRange(const VARIANT FAR& Xmin, const VARIANT FAR& Xmax, const VARIANT FAR& Ymin, const VARIANT FAR& Ymax) 
{
	// TODO: Add your dispatch handler code here
	m_scfCtrlX.varMin = Xmin;
	m_scfCtrlX.varMax = Xmax;
	
	m_scfCtrlY.varMin = Ymin;
	m_scfCtrlY.varMax = Ymax;
	
	const long&	lXMin = m_scfCtrlX.varMin.lVal;
	const long&	lXMax = m_scfCtrlX.varMax.lVal;
	m_dlgRoundSlider.SetRange( lXMin, lXMax );
}

void CSemCoreCtrlPanel_Stand3Ctrl::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
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

void CSemCoreCtrlPanel_Stand3Ctrl::SyncPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;
	
	long&	lXPos = m_scfCtrlX.varPos.lVal;
	lXPos = Xpos.lVal;

	m_dlgRoundSlider.SetPos( lXPos );

	// TODO: Add your specialized creation code here
	InvalidateControl();
}

void CSemCoreCtrlPanel_Stand3Ctrl::SyncText(LPCTSTR strX, LPCTSTR strY, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;
	
	m_strX = _T(strX);
	//m_strY;
	
	// force the control to redraw itself
	InvalidateControl();
}

void CSemCoreCtrlPanel_Stand3Ctrl::SyncString(const VARIANT FAR& ResourceID, const VARIANT FAR& ResourceString) 
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

void CSemCoreCtrlPanel_Stand3Ctrl::OnTextChanged() 
{
	// TODO: Add your specialized code here and/or call the base class
	COleControl::OnTextChanged();
	if( m_dlgRoundSlider.m_hWnd != NULL )
	{
		m_dlgRoundSlider.m_csRoundCtrl			= GetDisplayString();
		m_dlgRoundSlider.SettingChanged( FALSE );
	}
}

void CSemCoreCtrlPanel_Stand3Ctrl::OnCaptionVisibleChanged() 
{
	// TODO: Add notification handler code
	
	// force the control to redraw itself
	InvalidateControl();
	if( m_dlgRoundSlider.m_hWnd != NULL )
	{
		m_dlgRoundSlider.m_bDebugInfoVisible	= m_bDebugInfoVisible;
		m_dlgRoundSlider.m_csRoundCtrl			= GetDisplayString();
		m_dlgRoundSlider.SettingChanged( FALSE );
	}
	
	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand3Ctrl::OnDebugInfoVisibleChanged() 
{
	// TODO: Add notification handler code
	
	// force the control to redraw itself
	InvalidateControl();
	if( m_dlgRoundSlider.m_hWnd != NULL )
	{
		m_dlgRoundSlider.m_bDebugInfoVisible	= m_bDebugInfoVisible;
		m_dlgRoundSlider.m_csRoundCtrl			= GetDisplayString();
		m_dlgRoundSlider.SettingChanged( FALSE );
	}
	
	SetModifiedFlag();
}

void CSemCoreCtrlPanel_Stand3Ctrl::OnGradeVisibleChanged() 
{
	// TODO: Add notification handler code

	// force the control to redraw itself
	InvalidateControl();
	if( m_dlgRoundSlider.m_hWnd != NULL )
	{
		m_dlgRoundSlider.m_bGradeVisible		= m_bGradeVisible;
		m_dlgRoundSlider.SettingChanged( TRUE );
	}

	SetModifiedFlag();
}

int CSemCoreCtrlPanel_Stand3Ctrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	//////////////////////////////////////////////////////////////////
	const long	lXMin = m_scfCtrlX.varMin.lVal;
	const long	lXMax = m_scfCtrlX.varMax.lVal;
	const long	lXPos = m_scfCtrlX.varPos.lVal;

	m_dlgRoundSlider.SetRange( lXMin, lXMax );
	m_dlgRoundSlider.SetPos( lXPos );
	m_dlgRoundSlider.m_csRoundCtrl		= GetDisplayString();
	//////////////////////////////////////////////////////////////////

	if ( !m_dlgRoundSlider.Bind( this ) )
		return -1;
	
	if ( !m_dlgRoundSlider.Create(CRoundSliderDlg::IDD, this) )
		return -1;
	m_dlgRoundSlider.m_ctrlRoundSlider.SetTextColor(TranslateColor(GetForeColor()));
	m_dlgRoundSlider.SetFont( CFont::FromHandle(m_font.GetFontHandle()) );
	m_dlgRoundSlider.ShowWindow( SW_SHOW );
	
	return 0;
}

void CSemCoreCtrlPanel_Stand3Ctrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_dlgRoundSlider.SetWindowPos( NULL, 0, 0, cx, cy, SWP_NOZORDER );
}

void CSemCoreCtrlPanel_Stand3Ctrl::ReportPos( int nX, int nY )
{
	VARIANT	Flag;
	long&	lXPos = m_scfCtrlX.varPos.lVal;

	Flag.ulVal = 0;
	if ( lXPos != nX )
	{
		lXPos = nX;
		Flag.ulVal |= FlagCtrlX;
	}

	if ( Flag.ulVal != 0 )
	{
		FireReportPos( m_varPanelClassID, m_varPanelSerialNumber,
			m_scfCtrlX.varPos, m_scfCtrlY.varPos, Flag );
	}
}

CString CSemCoreCtrlPanel_Stand3Ctrl::GetDisplayString()
{
	CString csTextOut;
	const CString& strCaption = InternalGetText();
	
	csTextOut.Empty();
	/////////////////////////////////////////////////////////
	if (m_bCaptionVisible)
		csTextOut = strCaption;
	else
		csTextOut = _T("");
	
	CString csTemp;
	if (m_bDebugInfoVisible)
	{
		const CString& csDisplayFormat = _T(" [ID:%3Ld]");
		csTemp.Format( csDisplayFormat, m_varPanelSerialNumber.lVal );
		csTextOut += csTemp;
	}

	return csTextOut;
}

extern CString csRoundCtrlGrade[];
// 09.02.16 添加方法以同步文字显示
void CSemCoreCtrlPanel_Stand3Ctrl::SyncResString(const VARIANT FAR& ResourceID, LPCTSTR strRes) 
{
	// TODO: Add your dispatch handler code here
	switch( ResourceID.intVal )
	{
	case	IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_COARSE:
		csRoundCtrlGrade[0] = _T(strRes);
		break;
	case	IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_FINE:
		csRoundCtrlGrade[1] = _T(strRes);
		break;
	case	IDC_DIALOG_SEMCORECTRLPANEL_STAND3_RADIO_TINY:
		csRoundCtrlGrade[2] = _T(strRes);
		break;
	default:
		break;
	}
}
