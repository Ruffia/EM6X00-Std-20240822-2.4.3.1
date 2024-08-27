// ImageView.cpp : implementation file
//
// Last Modify : 2009.05.21

#include "stdafx.h"
#include "XXX2.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#include "memdc.h"
#include "ximage.h"

#include "ImageDoc.h"
#include "ImageView.h"
#include "GlobalParamSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageView

IMPLEMENT_DYNCREATE(CImageView, CScrollView)

CImageView::CImageView()
{
	SetScrollSizes(MM_TEXT, CSize(0, 0));
	VERIFY(m_brHatch.CreateHatchBrush(HS_DIAGCROSS, RGB(191, 191, 191)));
}

CImageView::~CImageView()
{
}


BEGIN_MESSAGE_MAP(CImageView, CScrollView)
	//{{AFX_MSG_MAP(CImageView)
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_VIEW_PANEL_3, OnViewPanel3)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PANEL_3, OnUpdateViewPanel3)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_MESSAGE(WM_USER_NEWIMAGE, OnNewImage)
	ON_MESSAGE(WM_USER_PROGRESS, OnNewProgress)
END_MESSAGE_MAP()


void CImageView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	/*
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
	*/
	Localize();
}

void CImageView::Localize(void)
{
	m_wLanguageID = CGlobalParamSetting::Instance().m_wLanguageID;

	// 获取menu_3200.mu的路径
	TCHAR path[255];
	::GetModuleFileName(0, path, 255);
	LPTSTR pszFileName = _tcsrchr(path, '\\') + 1;
	*pszFileName = '\0';
	strcat( path, g_csMenuFileName );

	TCHAR szBuf[20];

	// 获取根菜单数目
	TCHAR szBufRootSection[20];
	int nMenuRootCount = 0;
	sprintf(szBufRootSection, "Menu_Image_Root");

	if(GetPrivateProfileString(szBufRootSection, "Count", "0", szBuf, 255, path) != 0)
		nMenuRootCount = atoi(szBuf);
	else
		return;
	if( nMenuRootCount == 0 )
		return;

	// 分配菜单资源
	CMenu menuNew;
	menuNew.CreateMenu();
	CMenu* pmenu = new CMenu[nMenuRootCount];
	for(int i=0; i<nMenuRootCount; i++ )
		VERIFY( pmenu[i].CreatePopupMenu() );

	// 获取根菜单IDS
	CStringArray strRoot;
	strRoot.SetSize( nMenuRootCount, 20 );
	TCHAR szBufRootKey[20];
	for(int i=0; i<nMenuRootCount; i++ )
	{
		sprintf(szBufRootKey, "menu%d", i);
		if(GetPrivateProfileString(szBufRootSection, szBufRootKey, "0", szBuf, 255, path) != 0)
			strRoot[i].Format( "%s", szBuf );
	}

	// 依次获取子菜单
	int nMenuSubCount = 0;
	UINT nID = 0;
	for(int i=0; i<nMenuRootCount; i++ )
		theApp.MyAppendMenu( &(pmenu[i]), atoi(strRoot[i]), path );

	CString str;
	for(int i=0; i<nMenuRootCount; i++ )
	{
		nID = atoi( strRoot[i] );
		menuNew.AppendMenu(MF_POPUP, (UINT) pmenu[i].m_hMenu, GetResString(nID));
	}

	CImageDoc* pdoc = GetDocument();
	pdoc->m_DefaultMenu = menuNew.GetSafeHmenu();
	if (pdoc->m_DefaultMenu == NULL)
		return;
	CMDIFrameWnd* frame = ((CMDIChildWnd *) GetParent())->GetMDIFrame();
	CMenu* wmenu = CMenu::FromHandle(frame->GetWindowMenuPopup((CMenu::FromHandle(pdoc->m_DefaultMenu))->GetSafeHmenu()));
	frame->MDISetMenu(CMenu::FromHandle(pdoc->m_DefaultMenu), wmenu);
	frame->DrawMenuBar();

	// delete and detach
	for(int i=0; i<nMenuRootCount; i++ )
	{
		pmenu[i].Detach();
	}
	delete [] pmenu;
	menuNew.Detach();
}


BOOL CImageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CScrollView::PreCreateWindow(cs);
}

// CImageView drawing
void CImageView::OnDraw(CDC* pDC)
{
	CImageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	BOOL bPrinting = pDC->IsPrinting();

	CMemDC* pMemDC = NULL;
	if (!bPrinting) pDC = pMemDC = new CMemDC(pDC);

	if (!bPrinting && m_brHatch.m_hObject){
		CRect rect;
		GetClientRect(&rect);
		rect.right  = max(rect.right , m_totalDev.cx);
		rect.bottom = max(rect.bottom, m_totalDev.cy);
		m_brHatch.UnrealizeObject();
		CPoint pt(0, 0);
		pDC->LPtoDP(&pt);
		pt = pDC->SetBrushOrg(pt.x % 8, pt.y % 8);
		CBrush* old = pDC->SelectObject(&m_brHatch);
		pDC->FillRect(&rect, &m_brHatch);
		pDC->SelectObject(old);
	}

	CxImage* ima = pDoc->GetImage(); 
	if (ima) {
		if (bPrinting) {
			// get size of printer page (in pixels)
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			//int dcbpp = pDC->GetDeviceCaps(BITSPIXEL);
			//int dcnc = pDC->GetDeviceCaps(NUMCOLORS);
			//int dcp = pDC->GetDeviceCaps(PLANES);
			// get printer pixels per inch
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);
			// Best Fit case: create a rectangle which preserves the aspect ratio
			int cx=(ima->GetWidth()*cxInch)/ima->GetXDPI();
			int cy=(ima->GetHeight()*cyInch)/ima->GetYDPI();
			// print it!
			/*HDC TmpDC=CreateCompatibleDC(pDC->GetSafeHdc());
			HBITMAP bm =::CreateCompatibleBitmap(pDC->GetSafeHdc(), cx, cy);
			HBITMAP oldbm = (HBITMAP)::SelectObject(TmpDC,bm);
			BitBlt(TmpDC,0,0,cx,cy,0,0,0,WHITENESS);
			ima->Draw(TmpDC,CRect(0,0,cx,cy));
			BitBlt(pDC->GetSafeHdc(),100,100,cx,cy,TmpDC,0,0,SRCCOPY);
			DeleteObject(SelectObject(TmpDC,oldbm));
			DeleteDC(TmpDC);*/

			//======2004-5-12_图像视频打印功能=====================
			// 修改标示：2004-5-12_图像视频打印功能
			// 修改日期：2004年5月12日星期三
			// 修改人：罗明华
			// 修改原因：图像（视频）打印功能。
			
			// 本次修改前代码副本
			/*----------------------------------------------------- 
			CxImage tmp;
			tmp.Copy(*ima);
			RGBQUAD c={255,255,255,0};
			tmp.SetTransColor(c);
			tmp.AlphaStrip();
			tmp.Stretch(pDC->GetSafeHdc(), CRect(100,100,cx,cy));
			-----------------------------------------------------*/
			
			// 本次修改后代码
			//-----------------------------------------------------
			int cxStart, cyStart;
			double dfCriterionX = (double)cx / cxPage;
		
			if ( dfCriterionX >= 1.0 )  
			{   // 图像比打印纸大,需要缩小图像
				cxStart = 0;
				cx		= cxPage;
				cy		= (int)(cy / dfCriterionX);
			}
			else
			{   // 图像比打印纸小,需要调整图像位置
				cxStart = (cxPage - cx) / 2;
			}

			double dfCriterionY = (double)cy / cyPage;
				
			if ( dfCriterionY >= 1.0 )
			{
				cyStart = 0;
			}
			else if ( dfCriterionY > 0.5 )
			{
				cyStart = (cyPage - cy) / 2;
			}
			else
			{
				cyStart = (cyPage - 2*cy)/4;
			}

			if ( cxStart < 0 ) cxStart = 0;
			if ( cyStart < 0 ) cyStart = 0;

			CxImage tmp;
			tmp.Copy(*ima);
			RGBQUAD c={255,255,255,0};
			tmp.SetTransColor(c);
			tmp.AlphaStrip();
			tmp.Stretch(pDC->GetSafeHdc(), CRect(cxStart,cyStart,cxStart+cx,cyStart+cy));
			//-----------------------------------------------------
			//======修改日期：2004年5月12日星期三==================
		}
		else if (pDoc->GetStretchMode()) {
			CRect rect;
			GetClientRect(&rect);
			ima->Draw(pDC->GetSafeHdc(), rect);
		} else {
			// 05.06.23 图像缩放尺寸小于客户区大小时居中显示
			double zoom=pDoc->GetScale();
			int w = (int)(ima->GetWidth()*zoom);
			int h = (int)(ima->GetHeight()*zoom);
			CRect rc;
			GetClientRect(&rc);
			if( w < rc.Width() && h < rc.Height() )
				ima->Draw(pDC->GetSafeHdc(),
						CRect( rc.left + (rc.Width()-w)/2,
								rc.top + (rc.Height()-h)/2,
								rc.left + (rc.Width()-w)/2 + w,
								rc.top + (rc.Height()-h)/2 + h ) );
			else
			{
				if( zoom == 1 )
					ima->Draw(pDC->GetSafeHdc());
				else
					ima->Draw(pDC->GetSafeHdc(),
							CRect(0,0,(int)(ima->GetWidth()*zoom),(int)(ima->GetHeight()*zoom)));
			}
		}
	}

	delete pMemDC;
}

// CImageView printing
BOOL CImageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	pInfo->SetMaxPage(1);
	return DoPreparePrinting(pInfo);
}

void CImageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

// CImageView diagnostics
#ifdef _DEBUG
void CImageView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageDoc* CImageView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageDoc)));
	return (CImageDoc*)m_pDocument;
}
#endif //_DEBUG


void CImageView::GetImageXY(CImageDoc *pDoc, CxImage *ima, long *x, long *y)
{
	if (!ima || !pDoc) return;

	CPoint point=GetScrollPosition();
	double dx =(double)(*x + point.x);
	double dy =(double)(*y + point.y);

	// 05.06.23 图像缩放尺寸小于客户区大小时居中显示---Start
	double zoom=pDoc->GetScale();
	int w = (int)(ima->GetWidth()*zoom);
	int h = (int)(ima->GetHeight()*zoom);
	CRect rc;
	GetClientRect(&rc);
	if( w < rc.Width() && h < rc.Height() )
	{
		dx -= ( rc.left + (rc.Width()-w)/2 );
		dy -= ( rc.top + (rc.Height()-h)/2 );
	}
	// 05.06.23 图像缩放尺寸小于客户区大小时居中显示---End

	dx/=pDoc->GetScale();
	dy/=pDoc->GetScale();

	if (pDoc->GetStretchMode())	{
		CRect rect;
		GetClientRect(&rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		dx *= ima->GetWidth()/(float)width;
		dy *= ima->GetHeight()/(float)height;
	}

	*x = (long)dx;
	*y = (long)dy;
}

// CImageView message handlers
BOOL CImageView::OnEraseBkgnd(CDC* pDC) 
{
	return 1;
}

void CImageView::OnSetFocus(CWnd* pOldWnd) 
{
	CScrollView::OnSetFocus(pOldWnd);
	
	CImageDoc* pDoc = GetDocument();
	if (pDoc) pDoc->UpdateStatusBar();

	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( pfrm )
	{
		if( pfrm->m_MainPanelDebug.m_hWnd != NULL )
			pfrm->m_MainPanelDebug.ShowWindow( SW_HIDE );
		if( pfrm->m_wndScanCtrlBar.m_hWnd != NULL )
			pfrm->ShowControlBar(&(pfrm->m_wndScanCtrlBar), FALSE, FALSE);
		if( pfrm->m_HistoBar.m_hWnd != NULL )
			pfrm->ShowControlBar(&(pfrm->m_HistoBar), TRUE, FALSE);
		pfrm->RecalcLayout();
	}
}

void CImageView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (bActivate){
		((CMainFrame *)theApp.GetMainWnd())->m_HistoBar.Invalidate();
	}
	
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CImageView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	switch (lHint)
	{
	case WM_USER_NEWIMAGE:
		{
			CImageDoc* pDoc = GetDocument();
			CxImage*  ima  = pDoc->GetImage();
			if (ima) {
				int px=GetScrollPos(SB_HORZ);
				int py=GetScrollPos(SB_VERT);
				CSize sz(GetTotalSize());
				int x=(int)(ima->GetWidth()*pDoc->GetScale());
				int y=(int)(ima->GetHeight()*pDoc->GetScale());
				SetScrollSizes(MM_TEXT,	CSize(x,y));
				CSize sz2(GetTotalSize());

				CWnd* pFrame=GetParentFrame();
				RECT rClient;
				pFrame->GetClientRect(&rClient);

				if (sz.cx!=0 && sz.cy!=0){
					if (x>rClient.right) SetScrollPos(SB_HORZ,sz2.cx*px/sz.cx); else  SetScrollPos(SB_HORZ,0);
					if (y>rClient.bottom) SetScrollPos(SB_VERT,sz2.cy*py/sz.cy); else SetScrollPos(SB_VERT,0);
				}

				if (!ima->SelectionIsValid()) KillTimer(1);

#ifdef VATI_EXTENSIONS
				ima->SetJpegQualityF(theApp.m_optJpegQuality);
#if CXIMAGE_SUPPORT_JPG
				ima->SetCodecOption(theApp.m_optJpegOptions,CXIMAGE_FORMAT_JPG);
#endif
#if CXIMAGE_SUPPORT_RAW
				ima->SetCodecOption(theApp.m_optRawOptions,CXIMAGE_FORMAT_RAW);
#endif
#endif

				CMainFrame* pMain = (CMainFrame *)theApp.GetMainWnd();
				if (pMain->m_HistoBar.IsWindowVisible()){
					pDoc->m_hmax=ima->Histogram(pDoc->m_hr,pDoc->m_hg,pDoc->m_hb,pDoc->m_hgray);
					pMain->m_HistoBar.Invalidate();
				} else {
					pDoc->m_hmax = 0;
				}
			}
			
			break;
		}
	default:
		{
			CImageDoc* pDoc = GetDocument();
			if (pDoc){
				CxImage*  ima  = pDoc->GetImage();
				if (ima){
					if (pDoc->GetStretchMode()) SetScrollSizes(MM_TEXT,	CSize(0,0));
					else SetScrollSizes(MM_TEXT,CSize((int)(ima->GetWidth()*pDoc->GetScale()),
													  (int)(ima->GetHeight()*pDoc->GetScale())));
				}
			}
		}
	}
	CScrollView::OnUpdate(pSender, lHint, pHint);
}

LRESULT CImageView::OnNewImage(WPARAM wParam, LPARAM lParam)
{
	CImageDoc* pDoc = GetDocument();
	pDoc->UpdateAllViews(0,WM_USER_NEWIMAGE);
	pDoc->UpdateStatusBar();
	return 0;
}

LRESULT CImageView::OnNewProgress(WPARAM wParam, LPARAM lParam)
{
	CStatusBar& statusBar = ((CChildFrame *)GetParent())->m_wndStatusBar;
	CString s;
	s.Format("%d %%",(int)wParam);
	statusBar.SetPaneText(statusBar.CommandToIndex(ID_INDICATOR_OPEN_TIME), s);
	statusBar.Invalidate(1);

/*	CProgressCtrl& progressBar = ((CMainFrame *)theApp.GetMainWnd())->GetProgressBar();
	progressBar.SetPos((int)wParam);
	progressBar.Invalidate(1);*/

	return 0;
}

void CImageView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CImageDoc* pDoc = GetDocument();
	CxImage*  ima  = pDoc->GetImage();
	if (!ima)	return;

	// We'll get the RGB values at the point the user selects
	long x = point.x;
	long y = point.y;
	GetImageXY(pDoc, ima, &x,&y);

	TCHAR s[80];
	if (ima->IsInside(x,y))	{

		long yflip = ima->GetHeight() - y - 1;
		sprintf(s,"x: %d y: %d  idx: %d", x, y, ima->GetPixelIndex(x,yflip));
		RGBQUAD rgb=ima->GetPixelColor(x,yflip);
//		if (ima->HasAlpha()) rgb.rgbReserved=ima->AlphaGet(x,yflip);
		/*else */rgb.rgbReserved=ima->GetPaletteColor(ima->GetPixelIndex(x,yflip)).rgbReserved;
		sprintf(&s[strlen(s)],"  RGBA: (%d, %d, %d, %d)", rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue, rgb.rgbReserved);

		//Enable these lines if you want draw over the image	
		//if ((nFlags & MK_LBUTTON)==MK_LBUTTON){
		//	ima->SetPixelColor(x,yflip,RGB(rand()/(RAND_MAX/256),rand()/(RAND_MAX/256),rand()/(RAND_MAX/256)));
		//	Invalidate(0);
		//}

		if ((nFlags & MK_LBUTTON) && (nFlags & MK_SHIFT)){
			SetScrollPos(SB_HORZ,m_RefScroll.x - point.x + m_RefPoint.x);
			SetScrollPos(SB_VERT,m_RefScroll.y - point.y + m_RefPoint.y);
			Invalidate(0);
		}

	} else strcpy(s," ");
	
	CStatusBar& statusBar = ((CChildFrame *)GetParent())->m_wndStatusBar;
	statusBar.SetPaneText(statusBar.CommandToIndex(ID_INDICATOR_IMAGE_MOUSEMOVE), s);
	
	CScrollView::OnMouseMove(nFlags, point);
}

void CImageView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CImageDoc* pDoc = GetDocument();
	if (pDoc) {
		if (nFlags & MK_SHIFT){
			CxImage* ima = pDoc->GetImage();
			if (ima) {
				m_RefScroll = GetScrollPosition();
				m_RefPoint.x = point.x;
				m_RefPoint.y = point.y;
			}
		} else

		if (pDoc->GetWaitingClick()){
			pDoc->SetWaitingClick(0);
			CxImage* ima = pDoc->GetImage();
			if (ima) {
				long x = point.x;
				long y = point.y;
				GetImageXY(pDoc, ima, &x,&y);
				if (ima->IsInside(x,y))	{
					pDoc->SubmitUndo();
					long yflip = ima->GetHeight() - y - 1;
					ima->SetTransIndex(ima->GetPixelIndex(x,yflip));
					// <DP> RGB transparency
					ima->SetTransColor(ima->GetPixelColor(x,yflip));
					pDoc->UpdateAllViews(NULL);
				}
			}
		}
	}
	
	CScrollView::OnLButtonDown(nFlags, point);
}

// 2009.03.20 鼠标滚轮消息响应
BOOL CImageView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if( zDelta > 0 )		// Zoom In
	{
		CImageDoc* pDoc = GetDocument();
		pDoc->SetZoom( 0 );
	}
	else if( zDelta < 0 )	// Zoom Out
	{
		CImageDoc* pDoc = GetDocument();
		pDoc->SetZoom( 1 );
	}

	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CImageView::OnViewPanel3()
{
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( pfrm && pfrm->m_HistoBar.m_hWnd != NULL )
	{
		BOOL bVisible = ((pfrm->m_HistoBar.GetStyle() & WS_VISIBLE) != 0);
		pfrm->ShowControlBar(&(pfrm->m_HistoBar), !bVisible, FALSE);
		pfrm->RecalcLayout();
	}
}

void CImageView::OnUpdateViewPanel3(CCmdUI* pCmdUI)
{
	CMainFrame* pfrm = (CMainFrame *)theApp.GetMainWnd();
	if( pfrm && pfrm->m_HistoBar.m_hWnd != NULL )
	{
		BOOL bVisible = ((pfrm->m_HistoBar.GetStyle() & WS_VISIBLE) != 0);
		pCmdUI->SetCheck(bVisible);
	}
}