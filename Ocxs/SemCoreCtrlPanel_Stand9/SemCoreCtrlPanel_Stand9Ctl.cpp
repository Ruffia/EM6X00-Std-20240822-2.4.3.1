// SemCoreCtrlPanel_Stand9Ctl.cpp : Implementation of the CSemCoreCtrlPanel_Stand9Ctrl ActiveX Control class.
//
// Last Modify : 2009.05.11

#include "stdafx.h"
#include "SemCoreCtrlPanel_Stand9.h"
#include "SemCoreCtrlPanel_Stand9Ctl.h"
#include "SemCoreCtrlPanel_Stand9Ppg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

const double dfSpin2ButtonRectangle_Left	= 0.5;
const double dfSpin2ButtonRectangle_Right	= 0.75;
const double dfSpin2ButtonRectangle_Top		= 0.0;
const double dfSpin2ButtonRectangle_Bottom	= 1.0;

double& GetThirdSignificandWeight( double dblValue, int nPower = 1, BOOL boolIncrease = TRUE )
{
	static double dblSignificandWeight;

	if ( boolIncrease )
	{
		if (dblValue < 10.0)
			dblSignificandWeight = 0.01 * nPower;
		else if (dblValue < 100.0)
			dblSignificandWeight = 0.1 * nPower;
		else if (dblValue < 1000.0)
			dblSignificandWeight = 1.0 * nPower;
		else if (dblValue < 10000.0)
			dblSignificandWeight = 10.0 * nPower;
		else if (dblValue < 100000.0)
			dblSignificandWeight = 100.0 * nPower;
		else
			dblSignificandWeight = 1000.0 * nPower;
	}
	else
	{
		if (dblValue < 10.0 + 0.1 * nPower)
		{
			if (dblValue >= 10.0)
				dblSignificandWeight = dblValue - 9.99;
			else
				dblSignificandWeight = 0.01 * nPower;
		}
		else if (dblValue < 100.0 + 1.0 * nPower)
		{
			if (dblValue >= 100.0)
				dblSignificandWeight = dblValue - 99.9;
			else
				dblSignificandWeight = 0.1 * nPower;
		}
		else if (dblValue < 1000.0 + 10.0 * nPower)
		{
			if (dblValue >= 1000.0)
				dblSignificandWeight = dblValue - 999.0;
			else
				dblSignificandWeight = 1.0 * nPower;
		}
		else if (dblValue < 10000.0 + 100.0 * nPower)
		{
			if (dblValue >= 10000.0)
				dblSignificandWeight = dblValue - 9990.0;
			else
				dblSignificandWeight = 10.0 * nPower;
		}
		else if (dblValue < 100000.0 + 1000.0 * nPower)
		{
			if (dblValue >= 100000.0)
				dblSignificandWeight = dblValue - 99900.0;
			else
				dblSignificandWeight = 100.0 * nPower;
		}
		else
			dblSignificandWeight = 1000.0 * nPower;
	}

	return dblSignificandWeight;
}

//////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CSemCoreCtrlPanel_Stand9Ctrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSemCoreCtrlPanel_Stand9Ctrl, COleControl)
	//{{AFX_MSG_MAP(CSemCoreCtrlPanel_Stand9Ctrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(UDN_DELTAPOS, nIDSpin1Ctrl, OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, nIDSpin2Ctrl, OnDeltaposSpin2)
	ON_BN_CLICKED(nIDMultiply10Ctrl, OnButtonMultiply10)
	ON_BN_CLICKED(nIDDivide10Ctrl, OnButtonDivide10)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_COMMAND_RANGE(IDM_SEMCORECTRLPANEL_STAND9_MAG_1, IDM_SEMCORECTRLPANEL_STAND9_MAG_8, OnFastChanged)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CSemCoreCtrlPanel_Stand9Ctrl, COleControl)
	//{{AFX_DISPATCH_MAP(CSemCoreCtrlPanel_Stand9Ctrl)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand9Ctrl, "CaptionVisible", m_bCaptionVisible, OnCaptionVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand9Ctrl, "DebugInfoVisible", m_bDebugInfoVisible, OnDebugInfoVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSemCoreCtrlPanel_Stand9Ctrl, "NewDisplayMode", m_bNewDisplayMode, OnNewDisplayModeChanged, VT_BOOL)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand9Ctrl, "Bind", Bind, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand9Ctrl, "Enable", Enable, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand9Ctrl, "SetRange", SetRange, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand9Ctrl, "SetPos", SetPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand9Ctrl, "SyncPos", SyncPos, VT_EMPTY, VTS_VARIANT VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION(CSemCoreCtrlPanel_Stand9Ctrl, "SyncText", SyncText, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_VARIANT)
	DISP_STOCKPROP_BACKCOLOR()
	DISP_STOCKPROP_FORECOLOR()
	DISP_STOCKPROP_FONT()
	DISP_STOCKPROP_CAPTION()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CSemCoreCtrlPanel_Stand9Ctrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CSemCoreCtrlPanel_Stand9Ctrl, COleControl)
	//{{AFX_EVENT_MAP(CSemCoreCtrlPanel_Stand9Ctrl)
	EVENT_CUSTOM("ReportPos", FireReportPos, VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT)
	EVENT_CUSTOM("ReportAction", FireReportAction, VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT  VTS_VARIANT)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSemCoreCtrlPanel_Stand9Ctrl, 3)
	PROPPAGEID(CSemCoreCtrlPanel_Stand9PropPage::guid)
	PROPPAGEID(CLSID_CColorPropPage)
	PROPPAGEID(CLSID_CFontPropPage)
END_PROPPAGEIDS(CSemCoreCtrlPanel_Stand9Ctrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSemCoreCtrlPanel_Stand9Ctrl, "SEMCORECTRLPANELSTAND9.SemCoreCtrlPanelStand9Ctrl.1",
	0xe6ffa1c0, 0x7787, 0x41d9, 0x8b, 0xa6, 0xb4, 0xc5, 0x12, 0xaf, 0x9, 0xe3)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CSemCoreCtrlPanel_Stand9Ctrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DSemCoreCtrlPanel_Stand9 =
		{ 0x7783bc3e, 0x6657, 0x4717, { 0x9b, 0xff, 0x60, 0xfa, 0x2d, 0xfb, 0xb4, 0x1c } };
const IID BASED_CODE IID_DSemCoreCtrlPanel_Stand9Events =
		{ 0x78d88a88, 0xb2d0, 0x401b, { 0xad, 0x5c, 0x48, 0xd, 0x1a, 0x83, 0x27, 0x60 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwSemCoreCtrlPanel_Stand9OleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSemCoreCtrlPanel_Stand9Ctrl, IDS_SEMCORECTRLPANEL_STAND9, _dwSemCoreCtrlPanel_Stand9OleMisc)


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand9Ctrl::CSemCoreCtrlPanel_Stand9CtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSemCoreCtrlPanel_Stand9Ctrl

BOOL CSemCoreCtrlPanel_Stand9Ctrl::CSemCoreCtrlPanel_Stand9CtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_SEMCORECTRLPANEL_STAND9,
			IDB_SEMCORECTRLPANEL_STAND9,
			afxRegApartmentThreading,
			_dwSemCoreCtrlPanel_Stand9OleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand9Ctrl::CSemCoreCtrlPanel_Stand9Ctrl - Constructor

CSemCoreCtrlPanel_Stand9Ctrl::CSemCoreCtrlPanel_Stand9Ctrl()
{
	InitializeIIDs(&IID_DSemCoreCtrlPanel_Stand9, &IID_DSemCoreCtrlPanel_Stand9Events);

	// TODO: Initialize your control's instance data here.
	m_scfCtrlX.varMin.dblVal = 10.0;
	m_scfCtrlX.varMax.dblVal = 10000.0;
	m_scfCtrlX.varPos.dblVal = 100.0;
	
	m_scfCtrlY = m_scfCtrlX;
	
	m_strX = _T("-X-");
	m_strY = _T("-Y-");
	
	m_varPanelSerialNumber.lVal = -1;

	m_bMode1Line	= TRUE;
	m_bEnabled		= TRUE;

	m_nFastTotal = 0;
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand9Ctrl::~CSemCoreCtrlPanel_Stand9Ctrl - Destructor

CSemCoreCtrlPanel_Stand9Ctrl::~CSemCoreCtrlPanel_Stand9Ctrl()
{
	// TODO: Cleanup your control's instance data here.
	OnSizeFont( NULL );
	if( m_bmpMul10.m_hObject != NULL )
		m_bmpMul10.DeleteObject();
	if( m_bmpDiv10.m_hObject != NULL )
		m_bmpDiv10.DeleteObject();
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand9Ctrl::OnDraw - Drawing function

void CSemCoreCtrlPanel_Stand9Ctrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle(GetSysColorBrush(COLOR_3DFACE)));

	/////////////////////////////////////////////////////////
	if (m_ctrlSpin1.m_hWnd != NULL)
		return;
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// TODO: Replace the following code with your own drawing code.
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
// CSemCoreCtrlPanel_Stand9Ctrl::DoPropExchange - Persistence support

void CSemCoreCtrlPanel_Stand9Ctrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
	if (pPX->GetVersion() ==(DWORD)MAKELONG(_wVerMinor, _wVerMajor))
	{
		PX_Bool(pPX, _T("CaptionVisible"), m_bCaptionVisible, TRUE);
		PX_Bool(pPX, _T("DebugInfoVisible"), m_bDebugInfoVisible, TRUE);
		PX_Bool(pPX, _T("NewDisplayMode"), m_bNewDisplayMode, TRUE);
	}
	else if (pPX->IsLoading())
	{
		// Skip over persistent data
		BOOL bDummy;
		PX_Bool(pPX, _T("CaptionVisible"), bDummy, FALSE);
		PX_Bool(pPX, _T("DebugInfoVisible"), bDummy, FALSE);
		PX_Bool(pPX, _T("NewDisplayMode"), bDummy, FALSE);
		
		// Force property values to these defaults
		m_bCaptionVisible = TRUE;
		m_bDebugInfoVisible = TRUE;
		m_bNewDisplayMode = TRUE;
	}

}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand9Ctrl::OnResetState - Reset control to default state

void CSemCoreCtrlPanel_Stand9Ctrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand9Ctrl::AboutBox - Display an "About" box to the user

void CSemCoreCtrlPanel_Stand9Ctrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_SEMCORECTRLPANEL_STAND9);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CSemCoreCtrlPanel_Stand9Ctrl message handlers

void CSemCoreCtrlPanel_Stand9Ctrl::Bind(const VARIANT FAR& ClassID, const VARIANT FAR& SerialNumber) 
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
	char szBuf1[100], szBuf2[100];

	char Separator = ',';
	int i = 0;
	if(GetPrivateProfileString("Fast", "Mag", "", szBuf1, 100, path) != 0)
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

void CSemCoreCtrlPanel_Stand9Ctrl::Enable(const VARIANT FAR& Enabled, const VARIANT FAR& Flag) 
{
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;

	// TODO: Add your dispatch handler code here
	const BOOL& blEnabled = Enabled.boolVal;
	m_bEnabled = blEnabled;

	if (m_ctrlSpin1.m_hWnd != NULL)
	{
		m_ctrlSpin1.EnableWindow( blEnabled );
		if( m_bNewDisplayMode )
		{
			if( blEnabled )
				m_ctrlSpin1.SetBitmap( IDB_SEMCORECTRLPANEL_STAND9_BITMAP_ADD1, IDB_SEMCORECTRLPANEL_STAND9_BITMAP_DES1 );
			else
				m_ctrlSpin1.SetBitmap( IDB_SEMCORECTRLPANEL_STAND9_BITMAP_DIS_ADD1, IDB_SEMCORECTRLPANEL_STAND9_BITMAP_DIS_DES1 );
		}
	}

	if (m_ctrlSpin2.m_hWnd != NULL)
	{
		m_ctrlSpin2.EnableWindow( blEnabled );
		if( m_bNewDisplayMode )
		{
			if( blEnabled )
				m_ctrlSpin2.SetBitmap( IDB_SEMCORECTRLPANEL_STAND9_BITMAP_ADD10, IDB_SEMCORECTRLPANEL_STAND9_BITMAP_DES10 );
			else
				m_ctrlSpin2.SetBitmap( IDB_SEMCORECTRLPANEL_STAND9_BITMAP_DIS_ADD10, IDB_SEMCORECTRLPANEL_STAND9_BITMAP_DIS_DES10 );
		}
	}

	if (m_ctrlMultiply10.m_hWnd != NULL)
		m_ctrlMultiply10.EnableWindow( blEnabled );

	if (m_ctrlDivide10.m_hWnd != NULL)
		m_ctrlDivide10.EnableWindow( blEnabled );

}

void CSemCoreCtrlPanel_Stand9Ctrl::SetRange(const VARIANT FAR& Xmin, const VARIANT FAR& Xmax, const VARIANT FAR& Ymin, const VARIANT FAR& Ymax) 
{
	// TODO: Add your dispatch handler code here
	m_scfCtrlX.varMin = Xmin;
	m_scfCtrlX.varMax = Xmax;
	
	m_scfCtrlY.varMin = Ymin;
	m_scfCtrlY.varMax = Ymax;
}

void CSemCoreCtrlPanel_Stand9Ctrl::SetPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;
	
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
		VARIANT	XPosNew, YPosNew, Flag;
		
		XPosNew.dblVal	= dblXPosNew;
		YPosNew.dblVal	= 0.0;
		Flag.ulVal		= FlagCtrlX;
		
		SyncPos(XPosNew, YPosNew, Flag);
		FireReportPos(m_varPanelClassID, m_varPanelSerialNumber, XPosNew, YPosNew, Flag);	
	}
}

void CSemCoreCtrlPanel_Stand9Ctrl::SyncPos(const VARIANT FAR& Xpos, const VARIANT FAR& Ypos, const VARIANT FAR& Flag) 
{
	// TODO: Add your dispatch handler code here
	if ( (Flag.ulVal & FlagCtrlX) != FlagCtrlX )
		return;
	
	/////////////////////////////////////////////////////////
	m_scfCtrlX.varPos = Xpos;
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
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

void CSemCoreCtrlPanel_Stand9Ctrl::SyncText(LPCTSTR strX, LPCTSTR strY, const VARIANT FAR& Flag) 
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

// 09.02.17
void CSemCoreCtrlPanel_Stand9Ctrl::OnTextChanged() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	COleControl::OnTextChanged();

	CString csTextOut = GetDisplayString();
	if (m_ctrlStatic.m_hWnd != NULL)
		m_ctrlStatic.SetWindowText( csTextOut );
}

void CSemCoreCtrlPanel_Stand9Ctrl::OnCaptionVisibleChanged() 
{
	// TODO: Add notification handler code
	
	// force the control to redraw itself
	InvalidateControl();
	
	SetModifiedFlag();

	// 09.02.17
	CString csTextOut = GetDisplayString();
	if (m_ctrlStatic.m_hWnd != NULL)
		m_ctrlStatic.SetWindowText( csTextOut );
}

void CSemCoreCtrlPanel_Stand9Ctrl::OnDebugInfoVisibleChanged() 
{
	// TODO: Add notification handler code
	
	// force the control to redraw itself
	InvalidateControl();
	
	SetModifiedFlag();

	// 09.02.17
	CString csTextOut = GetDisplayString();
	if (m_ctrlStatic.m_hWnd != NULL)
		m_ctrlStatic.SetWindowText( csTextOut );
}

void CSemCoreCtrlPanel_Stand9Ctrl::OnNewDisplayModeChanged() 
{
	// TODO: Add notification handler code
	// 09.02.17
	ResetControl();
	CRect rc;
	GetClientRect( &rc );
	CreateControl( rc );
	// 09.02.17

	// force the control to redraw itself
	InvalidateControl();

	SetModifiedFlag();
}

int CSemCoreCtrlPanel_Stand9Ctrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CRect rcBounds(0, 0, lpCreateStruct->cx, lpCreateStruct->cy );
	return CreateControl( rcBounds );
}

// 09.02.17
int CSemCoreCtrlPanel_Stand9Ctrl::CreateControl( CRect rcBounds )
{
	CreateStatic( rcBounds );
	CreateMultiply10( rcBounds );
	CreateDivide10( rcBounds );
	CreateSpin1( rcBounds );
	CreateSpin2( rcBounds );
	return 0;
}

BOOL CSemCoreCtrlPanel_Stand9Ctrl::CreateStatic( CRect rcBounds )
{	
	CRect rc;
	CString csTextOut;
	DWORD	dwStyle, dwExStyle;

	/////////////////////////////////////////////////////////
	// Create Static
	/////////////////////////////////////////////////////////
	if( m_bNewDisplayMode )
	{
		rc.left		= rcBounds.left;
		rc.right	= rcBounds.right;
		rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 8 );
		rc.bottom	= rc.top + (long)((double)rcBounds.Height() / 3 );
	}
	else
	{
		rc.left		= rcBounds.left;
		rc.right	= rcBounds.left + (long)((double)rcBounds.Width() * dfSpin2ButtonRectangle_Left - 1);
		rc.top		= rcBounds.top;
		rc.bottom	= rcBounds.top  + (long)((double)rcBounds.Height() / 2 );
		if( m_bMode1Line )
		{
			rc.left		= rcBounds.left;
			rc.right	= rcBounds.left + (long)((double)rcBounds.Width() /2 );
			rc.top		= rcBounds.top;
			rc.bottom	= rcBounds.bottom;
		}
	}
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
		return TRUE;
	}
	return FALSE;
}

BOOL CSemCoreCtrlPanel_Stand9Ctrl::CreateMultiply10( CRect rcBounds )
{
	CRect rc;
	CString csTextOut;
	DWORD	dwStyle, dwExStyle;

	/////////////////////////////////////////////////////////
	// Create Multiply10			↑↓∧∨⑩▲▼△▽
	/////////////////////////////////////////////////////////
	if( m_bNewDisplayMode )
	{
		rc.left		= rcBounds.left;
		rc.right	= rcBounds.left + (long)((double)rcBounds.Width() / 3 );
		rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 2 );
		rc.bottom	= rcBounds.bottom - (long)((double)rcBounds.Height() / 4 );
	}
	else
	{
		rc.left		= rcBounds.left;
		rc.right	= rcBounds.left + (long)( ((double)rcBounds.Width() * dfSpin2ButtonRectangle_Left - 1)/2.0 + 1.5 );
		rc.top		= rcBounds.bottom - (long)((double)rcBounds.Height() / 2 );
		rc.bottom	= rcBounds.bottom;
		if( m_bMode1Line )
		{
			rc.left		= rcBounds.left + (long)((double)rcBounds.Width() /2 );
			rc.right	= rcBounds.left + (long)((double)rcBounds.Width() *3 /2 );
			rc.top		= rcBounds.top;
			rc.bottom	= rcBounds.top + (long)((double)rcBounds.Height() /2 );
		}
	}

	if( m_bNewDisplayMode )
		dwStyle = WS_CHILD | WS_VISIBLE | BS_CENTER	| BS_VCENTER | BS_BITMAP;
	else
		dwStyle = WS_CHILD | WS_VISIBLE | BS_CENTER | BS_VCENTER | BS_ICON;
	dwExStyle = WS_EX_STATICEDGE;

	m_ctrlMultiply10.CreateEx(dwExStyle, _T("BUTTON"), csTextOut, dwStyle, rc, this, nIDMultiply10Ctrl);
	if (m_ctrlMultiply10.m_hWnd != NULL)
	{
		m_ctrlMultiply10.SetWindowPos(NULL,
				rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);

		m_ctrlMultiply10.SetFont( CFont::FromHandle(m_font.GetFontHandle()) );
		if( m_bNewDisplayMode )
		{
			m_bmpMul10.LoadBitmap( IDB_SEMCORECTRLPANEL_STAND9_BITMAP_MUL10 );
			SendDlgItemMessage( nIDMultiply10Ctrl, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)((HBITMAP)m_bmpMul10) );
		}
		else
		{
			m_hIconMul10 = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SEMCORECTRLPANEL_STAND9_ICON_MUL10));
			SendDlgItemMessage( nIDMultiply10Ctrl, BM_SETIMAGE, IMAGE_ICON, (LPARAM)(m_hIconMul10) );
		}
		return TRUE;
	}
	//-----------------------------------------------------
	//======2008.11.27_按钮图标显示========================
	return FALSE;
}

BOOL CSemCoreCtrlPanel_Stand9Ctrl::CreateDivide10( CRect rcBounds )
{
	CRect rc;
	CString csTextOut;
	DWORD	dwStyle, dwExStyle;

	/////////////////////////////////////////////////////////
	// Create Divide10			↑↓∧∨⑩▲▼△▽
	/////////////////////////////////////////////////////////
	if( m_bNewDisplayMode )
	{
		rc.left		= rcBounds.left;
		rc.right	= rcBounds.left + (long)((double)rcBounds.Width() / 3 );
		rc.top		= rcBounds.bottom - (long)((double)rcBounds.Height() / 4 );
		rc.bottom	= rcBounds.bottom;
	}
	else
	{
		rc.left		= rcBounds.left + (long)( ((double)rcBounds.Width() * dfSpin2ButtonRectangle_Left - 1)/2.0 + 0.5 );
		rc.right	= rcBounds.left + (long)((double)rcBounds.Width() * dfSpin2ButtonRectangle_Left - 1);
		rc.top		= rcBounds.bottom - (long)((double)rcBounds.Height() / 2 );
		rc.bottom	= rcBounds.bottom;
		if( m_bMode1Line )
		{
			rc.left		= rcBounds.left + (long)((double)rcBounds.Width() /2 );
			rc.right	= rcBounds.left + (long)((double)rcBounds.Width() *3 /2 );
			rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 2 );
			rc.bottom	= rcBounds.bottom;
		}
	}

	if( m_bNewDisplayMode )
		dwStyle = WS_CHILD | WS_VISIBLE | BS_CENTER	| BS_VCENTER | BS_BITMAP;
	else
		dwStyle = WS_CHILD | WS_VISIBLE | BS_CENTER	| BS_VCENTER | BS_ICON;
	dwExStyle = WS_EX_STATICEDGE;

	m_ctrlDivide10.CreateEx(dwExStyle, _T("BUTTON"), csTextOut, dwStyle, rc, this, nIDDivide10Ctrl);
	if (m_ctrlDivide10.m_hWnd != NULL)
	{
		m_ctrlDivide10.SetWindowPos(NULL,
				rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);

		m_ctrlDivide10.SetFont( CFont::FromHandle(m_font.GetFontHandle()) );
		if( m_bNewDisplayMode )
		{
			m_bmpDiv10.LoadBitmap( IDB_SEMCORECTRLPANEL_STAND9_BITMAP_DIV10 );
			SendDlgItemMessage( nIDDivide10Ctrl, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)((HBITMAP)m_bmpDiv10) );
		}
		else
		{
			m_hIconDiv10 = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SEMCORECTRLPANEL_STAND9_ICON_DIV10));
			SendDlgItemMessage( nIDDivide10Ctrl, BM_SETIMAGE, IMAGE_ICON, (LPARAM)(m_hIconDiv10) );
		}
		return TRUE;
	}
	//-----------------------------------------------------
	//======2008.11.27_按钮图标显示========================
	return FALSE;
}

BOOL CSemCoreCtrlPanel_Stand9Ctrl::CreateSpin1( CRect rcBounds )
{
	CRect rc;
	CString csTextOut;
	DWORD	dwStyle, dwExStyle;

	/////////////////////////////////////////////////////////
	// Create Spin1
	/////////////////////////////////////////////////////////
	if( m_bNewDisplayMode )
	{
		rc.left		= rcBounds.right - (long)((double)rcBounds.Width() / 3 );
		rc.right	= rcBounds.right;
		rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 2 );
		rc.bottom	= rcBounds.bottom;
	}
	else
	{
		rc.left		= rcBounds.right - (long)((double)rcBounds.Width() * (1 - dfSpin2ButtonRectangle_Right));
		rc.right	= rcBounds.right;
		rc.top		= rcBounds.top  + (long)((double)rcBounds.Height() * dfSpin2ButtonRectangle_Top);
		rc.bottom	= rcBounds.top  + (long)((double)rcBounds.Height() * dfSpin2ButtonRectangle_Bottom);
		if( m_bMode1Line )
		{
			rc.left		= rcBounds.left + (long)((double)rcBounds.Width() *5 /6 );
			rc.right	= rcBounds.right;
			rc.top		= rcBounds.top;
			rc.bottom	= rcBounds.bottom;
		}
	}
	
	/////////////////////////////////////////////////////////
	dwStyle = WS_CHILD | WS_VISIBLE | UDS_ARROWKEYS | UDS_HOTTRACK;	// | UDS_HORZ
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
		if( m_bNewDisplayMode )
			m_ctrlSpin1.SetBitmap( IDB_SEMCORECTRLPANEL_STAND9_BITMAP_ADD1, IDB_SEMCORECTRLPANEL_STAND9_BITMAP_DES1 );
		else	//09.02.17
			m_ctrlSpin1.SetBitmap( 0, 0 );
		return TRUE;
	}
	return FALSE;
}

BOOL CSemCoreCtrlPanel_Stand9Ctrl::CreateSpin2( CRect rcBounds )
{
	CRect rc;
	CString csTextOut;
	DWORD	dwStyle, dwExStyle;

	/////////////////////////////////////////////////////////
	// Create Spin2
	/////////////////////////////////////////////////////////
	if( m_bNewDisplayMode )
	{
		rc.left		= rcBounds.left + (long)((double)rcBounds.Width() / 3 );
		rc.right	= rcBounds.right - (long)((double)rcBounds.Width() / 3 );
		rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 2 );
		rc.bottom	= rcBounds.bottom;
	}
	else
	{
		rc.left		= rcBounds.left + (long)((double)rcBounds.Width() * dfSpin2ButtonRectangle_Left);
		rc.right	= rcBounds.left + (long)((double)rcBounds.Width() * dfSpin2ButtonRectangle_Right);
		rc.top		= rcBounds.top  + (long)((double)rcBounds.Height() * dfSpin2ButtonRectangle_Top);
		rc.bottom	= rcBounds.top  + (long)((double)rcBounds.Height() * dfSpin2ButtonRectangle_Bottom);
		if( m_bMode1Line )
		{
			rc.left		= rcBounds.left + (long)((double)rcBounds.Width() *2 /3 );
			rc.right	= rcBounds.left + (long)((double)rcBounds.Width() *5 /6 );
			rc.top		= rcBounds.top;
			rc.bottom	= rcBounds.bottom;
		}
	}
	
	/////////////////////////////////////////////////////////
	dwStyle = WS_CHILD | WS_VISIBLE | UDS_ARROWKEYS | UDS_HOTTRACK;	// | UDS_HORZ
	dwExStyle = WS_EX_STATICEDGE;

	/////////////////////////////////////////////////////////
	//	initialize common controls
	//	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_UPDOWN_REG));
	m_ctrlSpin2.CreateEx(dwExStyle, dwStyle, rc, this, nIDSpin2Ctrl);
	if (m_ctrlSpin2.m_hWnd != NULL)
	{
		m_ctrlSpin2.SetWindowPos(NULL,
				rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
		m_ctrlSpin2.SetRange( 0, 100 );
		m_ctrlSpin2.SetPos( 50 );
		m_ctrlSpin2.SetBase( 10 );
		if( m_bNewDisplayMode )
			m_ctrlSpin2.SetBitmap( IDB_SEMCORECTRLPANEL_STAND9_BITMAP_ADD10, IDB_SEMCORECTRLPANEL_STAND9_BITMAP_DES10 );
		else	//09.02.17
			m_ctrlSpin2.SetBitmap( 0, 0 );
		return TRUE;
	}
	return FALSE;
}

// 09.02.17
void CSemCoreCtrlPanel_Stand9Ctrl::ResetControl()
{
	if( m_ctrlStatic.m_hWnd != NULL )
		m_ctrlStatic.DestroyWindow();
	if (m_ctrlMultiply10.m_hWnd != NULL)
		m_ctrlMultiply10.DestroyWindow();
	if (m_ctrlDivide10.m_hWnd != NULL)
		m_ctrlDivide10.DestroyWindow();
	if (m_ctrlSpin1.m_hWnd != NULL)
		m_ctrlSpin1.DestroyWindow();
	if (m_ctrlSpin2.m_hWnd != NULL)
		m_ctrlSpin2.DestroyWindow();

	if( m_bmpMul10.m_hObject != NULL )
		m_bmpMul10.DeleteObject();
	if( m_bmpDiv10.m_hObject != NULL )
		m_bmpDiv10.DeleteObject();
}

void CSemCoreCtrlPanel_Stand9Ctrl::OnSize(UINT nType, int cx, int cy) 
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
	if( m_bNewDisplayMode )
	{
		rc.left		= rcBounds.left;
		rc.right	= rcBounds.right;
		rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 8 );
		rc.bottom	= rc.top + (long)((double)rcBounds.Height() / 3 );
	}
	else
	{
		rc.left		= rcBounds.left;
		rc.right	= rcBounds.left + (long)((double)rcBounds.Width() * dfSpin2ButtonRectangle_Left - 1);
		rc.top		= rcBounds.top;
		rc.bottom	= rcBounds.top  + (long)((double)rcBounds.Height() / 2 );
		if( m_bMode1Line )
		{
			rc.left		= rcBounds.left;
			rc.right	= rcBounds.left + (long)((double)rcBounds.Width() /2 );
			rc.top		= rcBounds.top;
			rc.bottom	= rcBounds.bottom;
		}
	}
	if (m_ctrlStatic.m_hWnd != NULL)
	{
		m_ctrlStatic.SetWindowPos(NULL,	rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
		OnSizeFont( &m_ctrlStatic );
	}

	/////////////////////////////////////////////////////////
	// OnSize Multiply10			↑↓∧∨⑩▲▼△▽
	/////////////////////////////////////////////////////////
	if( m_bNewDisplayMode )
	{
		rc.left		= rcBounds.left;
		rc.right	= rcBounds.left + (long)((double)rcBounds.Width() / 3 );
		rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 2 );
		rc.bottom	= rcBounds.bottom - (long)((double)rcBounds.Height() / 4 );
	}
	else
	{
		rc.left		= rcBounds.left;
		rc.right	= rcBounds.left + (long)( ((double)rcBounds.Width() * dfSpin2ButtonRectangle_Left - 1)/2.0 + 1.5 );
		rc.top		= rcBounds.bottom - (long)((double)rcBounds.Height() / 2 );
		rc.bottom	= rcBounds.bottom;
		if( m_bMode1Line )
		{
			rc.left		= rcBounds.left + (long)((double)rcBounds.Width() /2 );
			rc.right	= rcBounds.left + (long)((double)rcBounds.Width() *2 /3 );
			rc.top		= rcBounds.top;
			rc.bottom	= rcBounds.top + (long)((double)rcBounds.Height() /2 );
		}
	}
	if (m_ctrlMultiply10.m_hWnd != NULL)
	{
		m_ctrlMultiply10.SetWindowPos(NULL,
				rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
	}

	/////////////////////////////////////////////////////////
	// OnSize Divide10			↑↓∧∨⑩▲▼△▽
	/////////////////////////////////////////////////////////
	if( m_bNewDisplayMode )
	{
		rc.left		= rcBounds.left;
		rc.right	= rcBounds.left + (long)((double)rcBounds.Width() / 3 );
		rc.top		= rcBounds.bottom - (long)((double)rcBounds.Height() / 4 );
		rc.bottom	= rcBounds.bottom;
	}
	else
	{
		rc.left		= rcBounds.left + (long)( ((double)rcBounds.Width() * dfSpin2ButtonRectangle_Left - 1)/2.0 + 0.5 );
		rc.right	= rcBounds.left + (long)((double)rcBounds.Width() * dfSpin2ButtonRectangle_Left - 1);
		rc.top		= rcBounds.bottom - (long)((double)rcBounds.Height() / 2 );
		rc.bottom	= rcBounds.bottom;
		if( m_bMode1Line )
		{
			rc.left		= rcBounds.left + (long)((double)rcBounds.Width() /2 );
			rc.right	= rcBounds.left + (long)((double)rcBounds.Width() *2 /3 );
			rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 2 );
			rc.bottom	= rcBounds.bottom;
		}
	}
	if (m_ctrlDivide10.m_hWnd != NULL)
	{
		m_ctrlDivide10.SetWindowPos(NULL,
				rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
	}

	/////////////////////////////////////////////////////////
	// OnSize Spin1
	/////////////////////////////////////////////////////////
	if( m_bNewDisplayMode )
	{
		rc.left		= rcBounds.right - (long)((double)rcBounds.Width() / 3 );
		rc.right	= rcBounds.right;
		rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 2 );
		rc.bottom	= rcBounds.bottom;
	}
	else
	{
		rc.left		= rcBounds.right - (long)((double)rcBounds.Width() * (1 - dfSpin2ButtonRectangle_Right));
		rc.right	= rcBounds.right;
		rc.top		= rcBounds.top  + (long)((double)rcBounds.Height() * dfSpin2ButtonRectangle_Top);
		rc.bottom	= rcBounds.top  + (long)((double)rcBounds.Height() * dfSpin2ButtonRectangle_Bottom);
		if( m_bMode1Line )
		{
			rc.left		= rcBounds.left + (long)((double)rcBounds.Width() *5 /6 );
			rc.right	= rcBounds.right;
			rc.top		= rcBounds.top;
			rc.bottom	= rcBounds.bottom;
		}
	}
	if (m_ctrlSpin1.m_hWnd != NULL)
	{
		m_ctrlSpin1.SetWindowPos(NULL,
				rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
	}

	/////////////////////////////////////////////////////////
	// OnSize Spin2
	/////////////////////////////////////////////////////////
	if( m_bNewDisplayMode )
	{
		rc.left		= rcBounds.left + (long)((double)rcBounds.Width() / 3 );
		rc.right	= rcBounds.right - (long)((double)rcBounds.Width() / 3 );
		rc.top		= rcBounds.top + (long)((double)rcBounds.Height() / 2 );
		rc.bottom	= rcBounds.bottom;
	}
	else
	{
		rc.left		= rcBounds.left + (long)((double)rcBounds.Width() * dfSpin2ButtonRectangle_Left);
		rc.right	= rcBounds.left + (long)((double)rcBounds.Width() * dfSpin2ButtonRectangle_Right);
		rc.top		= rcBounds.top  + (long)((double)rcBounds.Height() * dfSpin2ButtonRectangle_Top);
		rc.bottom	= rcBounds.top  + (long)((double)rcBounds.Height() * dfSpin2ButtonRectangle_Bottom);
		if( m_bMode1Line )
		{
			rc.left		= rcBounds.left + (long)((double)rcBounds.Width() *2 /3 );
			rc.right	= rcBounds.left + (long)((double)rcBounds.Width() *5 /6 );
			rc.top		= rcBounds.top;
			rc.bottom	= rcBounds.bottom;
		}
	}
	if (m_ctrlSpin2.m_hWnd != NULL)
	{
		m_ctrlSpin2.SetWindowPos(NULL,
				rc.left, rc.top, CRect(rc).Width(), CRect(rc).Height(), SWP_NOZORDER);
	}
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

void CSemCoreCtrlPanel_Stand9Ctrl::SetCtrlPos( double dblPos ) 
{
	// TODO: Add your dispatch handler code here
	VARIANT	XPosNew, YPosNew, Flag;
	
	XPosNew.dblVal	= dblPos;
	YPosNew.lVal	= 0;
	Flag.ulVal		= FlagCtrlX;

	SetPos(XPosNew, YPosNew, Flag);
}

void CSemCoreCtrlPanel_Stand9Ctrl::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_ctrlSpin1.m_bActiveSpinPressed = TRUE;
	const double& dblPosX = m_scfCtrlX.varPos.dblVal;
	double sblStep;

	if (pNMUpDown->iDelta >= 1)
	{
		sblStep = GetThirdSignificandWeight( dblPosX, 1 );
		SetCtrlPos( dblPosX + sblStep );
	}
	if (pNMUpDown->iDelta <= -1)
	{
		sblStep = GetThirdSignificandWeight( dblPosX, 1, FALSE );
		SetCtrlPos( dblPosX - sblStep );
	}

	*pResult = 0;
}

void CSemCoreCtrlPanel_Stand9Ctrl::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_ctrlSpin2.m_bActiveSpinPressed = TRUE;
	const double& dblPosX = m_scfCtrlX.varPos.dblVal;
	double sblStep;

	if (pNMUpDown->iDelta >= 1)
	{
		sblStep = GetThirdSignificandWeight( dblPosX, 10 );
		SetCtrlPos( dblPosX + sblStep );
	}
	if (pNMUpDown->iDelta <= -1)
	{
		sblStep = GetThirdSignificandWeight( dblPosX, 10, FALSE );
		SetCtrlPos( dblPosX - sblStep );
	}

	*pResult = 0;
}

void CSemCoreCtrlPanel_Stand9Ctrl::OnButtonMultiply10() 
{
	// TODO: Add your control notification handler code here
	const double& dblPosX = m_scfCtrlX.varPos.dblVal;

	SetCtrlPos( dblPosX * 10.0 );
}

void CSemCoreCtrlPanel_Stand9Ctrl::OnButtonDivide10() 
{
	// TODO: Add your control notification handler code here
	const double& dblPosX = m_scfCtrlX.varPos.dblVal;

	SetCtrlPos( dblPosX / 10.0 );
}

CString CSemCoreCtrlPanel_Stand9Ctrl::GetDisplayString()
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

void CSemCoreCtrlPanel_Stand9Ctrl::OnSizeFont(CWnd* pCtrl)
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

// 2009.05.08 在标签上单击可弹出放大倍数快速选择菜单列表
void CSemCoreCtrlPanel_Stand9Ctrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( !m_bEnabled || m_nFastTotal < 1 )
	{
		COleControl::OnLButtonDown(nFlags, point);
		return;
	}
	CPoint temppoint = point;

	CRect rc;
	m_ctrlStatic.GetClientRect( &rc );
	if( rc.PtInRect( temppoint ) )
	{
		CMenu menu;
		menu.CreatePopupMenu();
		for( int i=0; i<m_nFastTotal; i++ )
			menu.InsertMenu( IDM_SEMCORECTRLPANEL_STAND9_MAG_1 +i, MF_BYCOMMAND, IDM_SEMCORECTRLPANEL_STAND9_MAG_1 +i, m_strFast[i] );
		temppoint = CPoint( rc.left, rc.bottom );
		ClientToScreen( &temppoint );
		menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
							temppoint.x, temppoint.y, this, NULL);
	}

//	COleControl::OnLButtonDown(nFlags, point);
}

// 快速参数改变
void CSemCoreCtrlPanel_Stand9Ctrl::OnFastChanged(UINT nID)
{
	UINT nIndex = nID - IDM_SEMCORECTRLPANEL_STAND9_MAG_1;
/*	switch( nIndex )
	{
	case	0:
		SetCtrlPos( 15 );
		break;
	case	1:
		SetCtrlPos( 200 );
		break;
	case	2:
		SetCtrlPos( 500 );
		break;
	case	3:
		SetCtrlPos( 800 );
		break;
	case	4:
		SetCtrlPos( 1000 );
		break;
	case	5:
		SetCtrlPos( 1500 );
		break;
	case	6:
		SetCtrlPos( 2000 );
		break;
	case	7:
		SetCtrlPos( 5000 );
		break;
	default:
		break;
	}*/
	VARIANT	XPosNew, YPosNew, Flag;
	XPosNew.lVal	= nIndex;
	YPosNew.lVal	= 0;
	Flag.ulVal		= FlagCtrlX;

	FireReportAction(m_varPanelClassID, m_varPanelSerialNumber, XPosNew, YPosNew, Flag);
}
