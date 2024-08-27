// ImageDoc.cpp : implementation file
//
// LastModify : 2010.07.02

/////////////////////////////////////////////////////////////////////////////
// 09.11.18 对打开文件不存在的情况加以保护
// 09.11.19 将所有LoadString处均改为GetResString
// 10.02.08 对快照后的图片增加临时文件存储，并对文件存储做保护
// 10.07.02 增加优先存储格式
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XXX2.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#include <math.h>
#include <process.h>
#include "ximage.h"
#include "Quantize.h"

#include "ImageDoc.h"
#include "ImageView.h"
//#include "ScanView.h"
#include "ScanChildFrm.h"

#include "DlgRotate.h"
#include "DlgResample.h"
#include "DlgDecBpp.h"
#include "DlgIncBpp.h"
#include "DlgOptions.h"
#include "DlgDither.h"
#include "DlgThreshold.h"
#include "DlgColorize.h"
#include "DlgOpacity.h"
#include "DlgGamma.h"
#include "DlgPalette.h"
#include "DlgFFT.h"
#include "DlgRepair.h"
#include "DlgJpeg.h"
#include "DlgCustomFilter.h"
#include "DlgExpand.h"
#include "DlgFloodFill.h"
#include "DlgShadow.h"

#include "ImageItem.h"
#include "DlgPrompt.h"
#include "DlgInputAcq.h"
#include "ControlLayer.h"
#include "HVPowerDataManager.h"
#include "ImageDataManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL SaveSemCxImage(CFile &file, CxImage *pImage);
extern BOOL SaveSemParam(CFile &file, int nMag, int nHV);

// 05.06.23 图像缩放尺寸小于客户区大小时居中显示
double		g_dScaleImageTable[]	= { 0.125, 0.2, 0.4, 0.5, 0.7, 0.8, 1.0, 1.25, 1.5, 1.75, 2.0, 4.0 };
int			g_nScaleImageIndexMax	= sizeof(g_dScaleImageTable) / sizeof(double) - 1;
const int	g_nScaleImageIndexMin	= 0;
int			g_nScaleImageIndex		= 6;

/////////////////////////////////////////////////////////////////////////////
// CImageDoc

IMPLEMENT_DYNCREATE(CImageDoc, CSEMDocument)

CImageDoc::CImageDoc()
{
	image			= NULL;
	ImageLayer		= NULL;
	GraphLayer		= NULL;

	m_WaitingClick = stretchMode = FALSE;
	hThread = hProgress =0;

	for (int i=0;i<MAX_UNDO_LEVELS;i++) imageUndo[i]=NULL;
	QueryPerformanceFrequency(&m_swFreq);

	m_UndoLevel	= 0;
	m_dScale	= 1.;
	m_etime		= 0.0;
	m_NumSel	= 0;
	m_nHV		= 0;
	m_nMag		= 0;

	m_ac.strTime	= _T("");
	m_ac.nSpeed		= 0;
	m_ac.nIsSync	= 0;
	m_ac.nHV		= 0;
	m_ac.nHV2		= 0;
	m_ac.nFil		= 0;
	m_ac.nEMI		= 0;
	m_ac.strB		= "0";
	m_ac.strC		= "0";
	m_ac.strCL		= "0.0";
	m_ac.strOL		= "0.0";
	m_ac.strMag		= "150 X";
	m_ac.strWD		= "12 mm";
	m_ac.strSignal	= "SE";
	m_ac.nGain		= 0;
	m_ac.nGain2		= 0;
	m_ac.nFilter	= 0;
}

BOOL CImageDoc::OnNewDocument()
{
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString strTemp = timeCurrent.Format("%Y%m%d%H%M");
	CString csTitle;
	csTitle.Format( "SemImg%s",strTemp );

	SetTitle( csTitle );

	if (!CSEMDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	image = NULL;
	return TRUE;
}

CImageDoc::~CImageDoc()
{
	// stop the elaborations
	if (image) image->SetEscape(1);
	// stop the progress bar
	if (hProgress){
		ResumeThread(hProgress); //wake up!
		WaitForSingleObject(hProgress,INFINITE);
		CloseHandle(hProgress);
	}
	if (hThread){
		WaitForSingleObject(hThread,INFINITE);
		CloseHandle(hThread);
	}
	// free objects
	delete image;
	for (int i=0;i<MAX_UNDO_LEVELS;i++) delete imageUndo[i];

	/////////////////////////////////////////////////////
	if (ImageLayer != NULL)
	{
		ImageLayer->SetEscape(1);
		delete ImageLayer;
	}
	if (GraphLayer != NULL)
	{
		GraphLayer->SetEscape(1);
		delete GraphLayer;
	}
	/////////////////////////////////////////////////////
}

BEGIN_MESSAGE_MAP(CImageDoc, CSEMDocument)
	//{{AFX_MSG_MAP(CImageDoc)
	ON_COMMAND(ID_CXIMAGE_PHOTO, OnSemPhoto)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_PHOTO, OnUpdateSemPhoto)
	ON_COMMAND(ID_CXIMAGE_STRETCH_MODE, OnStretchMode)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_STRETCH_MODE, OnUpdateStretchMode)
	ON_COMMAND(ID_VIEW_ZOOM_IN, OnViewZoomIn)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_IN, OnUpdateViewZoomIn)
	ON_COMMAND(ID_VIEW_ZOOM_OUT, OnViewZoomOut)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_OUT, OnUpdateViewZoomOut)
	ON_COMMAND(ID_VIEW_ACTUAL_SIZE_IMAGE, OnViewActualSize)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ACTUAL_SIZE_IMAGE, OnUpdateViewActualSize)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_WINDOW_DUPLICATE, OnWindowDuplicate)
	ON_COMMAND(ID_CXIMAGE_MIRROR, OnCximageMirror)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_MIRROR, OnUpdateCximageMirror)
	ON_COMMAND(ID_CXIMAGE_FLIP, OnCximageFlip)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_FLIP, OnUpdateCximageFlip)
	ON_COMMAND(ID_CXIMAGE_NEGATIVE, OnCximageNegative)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_NEGATIVE, OnUpdateCximageNegative)
	ON_COMMAND(ID_CXIMAGE_GRAYSCALE, OnCximageGrayscale)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_GRAYSCALE, OnUpdateCximageGrayscale)
	ON_COMMAND(ID_CXIMAGE_ROTATEL, OnCximageRotatel)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_ROTATEL, OnUpdateCximageRotatel)
	ON_COMMAND(ID_CXIMAGE_ROTATER, OnCximageRotater)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_ROTATER, OnUpdateCximageRotater)
	ON_COMMAND(ID_CXIMAGE_ROTATE, OnCximageRotate)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_ROTATE, OnUpdateCximageRotate)
	ON_COMMAND(ID_CXIMAGE_RESAMPLE, OnCximageResample)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_RESAMPLE, OnUpdateCximageResample)
	ON_COMMAND(ID_CXIMAGE_DECREASEBPP, OnCximageDecreasebpp)
	ON_COMMAND(ID_CXIMAGE_INCREASEBPP, OnCximageIncreasebpp)
	ON_COMMAND(ID_CXIMAGE_THRESHOLD, OnCximageThreshold)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_THRESHOLD, OnUpdateCximageThreshold)
	ON_COMMAND(ID_CXIMAGE_COLORIZE, OnCximageColorize)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_COLORIZE, OnUpdateCximageColorize)
	ON_COMMAND(ID_CXIMAGE_LIGHTEN, OnCximageLighten)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_LIGHTEN, OnUpdateCximageLighten)
	ON_COMMAND(ID_CXIMAGE_DARKEN, OnCximageDarken)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_DARKEN, OnUpdateCximageDarken)
	ON_COMMAND(ID_CXIMAGE_CONTRAST, OnCximageContrast)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_CONTRAST, OnUpdateCximageContrast)
	ON_COMMAND(ID_CXIMAGE_LESSCONTRAST, OnCximageLessContrast)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_LESSCONTRAST, OnUpdateCximageLessContrast)
	ON_COMMAND(ID_CXIMAGE_ERODE, OnCximageErode)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_ERODE, OnUpdateCximageErode)
	ON_COMMAND(ID_CXIMAGE_DILATE, OnCximageDilate)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_DILATE, OnUpdateCximageDilate)
	ON_COMMAND(ID_CXIMAGE_BLUR, OnCximageBlur)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_BLUR, OnUpdateCximageBlur)
	ON_COMMAND(ID_CXIMAGE_SOFTEN, OnCximageSoften)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_SOFTEN, OnUpdateCximageSoften)
	ON_COMMAND(ID_CXIMAGE_SHARPEN, OnCximageSharpen)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_SHARPEN, OnUpdateCximageSharpen)
	ON_COMMAND(ID_CXIMAGE_EDGE, OnCximageEdge)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_EDGE, OnUpdateCximageEdge)
	ON_COMMAND(ID_CXIMAGE_EMBOSS, OnCximageEmboss)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_EMBOSS, OnUpdateCximageEmboss)
	ON_COMMAND(ID_CXIMAGE_DITHER, OnCximageDither)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_DITHER, OnUpdateCximageDither)
	ON_COMMAND(ID_CXIMAGE_OPTIONS, OnCximageOptions)
	ON_COMMAND(ID_CXIMAGE_SETTRANSPARENCY, OnCximageSettransparency)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_SETTRANSPARENCY, OnUpdateCximageSettransparency)
	ON_COMMAND(ID_CXIMAGE_REMOVETRANSPARENCY, OnCximageRemovetransparency)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_REMOVETRANSPARENCY, OnUpdateCximageRemovetransparency)
	ON_COMMAND(ID_CXIMAGE_SPLITHSL, OnCximageSplithsl)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_SPLITHSL, OnUpdateCximageSplithsl)
	ON_COMMAND(ID_CXIMAGE_SPLITRGB, OnCximageSplitrgb)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_SPLITRGB, OnUpdateCximageSplitrgb)
	ON_COMMAND(ID_CXIMAGE_SPLITYUV, OnCximageSplityuv)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_SPLITYUV, OnUpdateCximageSplityuv)
	ON_COMMAND(ID_CXIMAGE_PSEUDOCOLORS, OnCximagePseudocolors)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_PSEUDOCOLORS, OnUpdateCximagePseudocolors)
	ON_COMMAND(ID_CXIMAGE_OPACITY, OnCximageOpacity)
	ON_COMMAND(ID_CXIMAGE_REMOVEALPHACHANNEL, OnCximageRemovealphachannel)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_REMOVEALPHACHANNEL, OnUpdateCximageRemovealphachannel)
	ON_COMMAND(ID_CXIMAGE_ALPHASTRIP, OnCximageAlphastrip)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_ALPHASTRIP, OnUpdateCximageAlphastrip)
	ON_COMMAND(ID_CXIMAGE_INVETALPHA, OnCximageInvetalpha)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_INVETALPHA, OnUpdateCximageInvetalpha)
	ON_COMMAND(ID_CXIMAGE_ALPHAPALETTETOGGLE, OnCximageAlphapalettetoggle)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_ALPHAPALETTETOGGLE, OnUpdateCximageAlphapalettetoggle)
	ON_COMMAND(ID_CXIMAGE_GAMMA, OnCximageGamma)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_GAMMA, OnUpdateCximageGamma)
	ON_COMMAND(ID_CXIMAGE_FFT, OnCximageFft)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_FFT, OnUpdateCximageFft)
	ON_COMMAND(ID_CXIMAGE_Repair, OnCximageRepair)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_Repair, OnUpdateCximageRepair)
	ON_COMMAND(ID_CXIMAGE_Jpeg, OnCximageJpeg)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_Jpeg, OnUpdateCximageJpeg)
	ON_COMMAND(ID_CXIMAGE_Shadow, OnCximageShadow)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_Shadow, OnUpdateCximageShadow)
	ON_COMMAND(ID_TOOLS_ResoFactor, OnToolsResoFactor)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ResoFactor, OnUpdateToolsResoFactor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageDoc diagnostics

#ifdef _DEBUG
void CImageDoc::AssertValid() const
{
	CSEMDocument::AssertValid();
}

void CImageDoc::Dump(CDumpContext& dc) const
{
	CSEMDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageDoc serialization

void CImageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CImageDoc commands
//////////////////////////////////////////////////////////////////////////////

CView* CImageDoc::GetView(CRuntimeClass *prt)
{
	POSITION pos = GetFirstViewPosition();
	CView* pView;
	while( pos )
	{
		pView = GetNextView( pos );
		if( pView->IsKindOf( prt ) )
			return pView;
	}
	return NULL;		
}

void CImageDoc::UpdateScale()
{
	// 05.06.23 根据图像大小调整缩放因子
	CImageView* pView = (CImageView*)GetView( RUNTIME_CLASS(CImageView) );
	CRect rc;
	pView->GetClientRect(&rc);
	CxImage* ima = GetImage();
	int w = (int)(ima->GetWidth());
	int h = (int)(ima->GetHeight());

	double ratio_x = (double)rc.Width() / w;
	double ratio_y = (double)rc.Height() / h;
	double dScale = (ratio_x < ratio_y)?ratio_x:ratio_y;
	for( int i=g_nScaleImageIndexMin; i<=g_nScaleImageIndexMax; i++ )
	{
		if( dScale < g_dScaleImageTable[i] )
		{
			g_dScaleImageTable[i] = dScale;
			g_nScaleImageIndexMax = i;
			break;
		}
	}
	SetScale( dScale );//0.5 );

/*	int nZoom = (int)(m_dScale * 1000.0 + 0.5);
	for (int i=g_nScaleImageIndexMin; i<=g_nScaleImageIndexMax; i++)
	{
		if ( nZoom <= g_dScaleImageTable[i] )
		{
			g_nScaleImageIndex = i;		
			break;
		}
	}
	for( i=g_nScaleImageIndex; i>g_nScaleImageIndexMin; i-- )
	{
		if( (w * g_dScaleImageTable[i] * 0.001 < rc.Width())
			&& (h * g_dScaleImageTable[i] * 0.001 < rc.Height()) )
		{
			g_nScaleImageIndex = i;
			break;
		}
	}
	if( g_nScaleImageIndex < g_nScaleImageIndexMin )
		g_nScaleImageIndex = g_nScaleImageIndexMin;
	m_dScale = g_dScaleImageTable[g_nScaleImageIndex] * 0.001;
*/
}

void CImageDoc::SetScale( double dScale )
{
	BOOL bFound = FALSE;
	for( int i=g_nScaleImageIndexMin; i<=g_nScaleImageIndexMax; i++ )
	{
		if ( dScale == g_dScaleImageTable[i] )
		{
			g_nScaleImageIndex = i;
			bFound = TRUE;
			break;
		}
	}
	if( bFound )
		m_dScale = g_dScaleImageTable[g_nScaleImageIndex];
	else
	{
		g_nScaleImageIndex = g_nScaleImageIndexMax;
		m_dScale = dScale;
	}
}

CString CImageDoc::FindExtension(const CString& name)
{
	int len = name.GetLength();
	for (int i = len-1; i >= 0; i--){
		if (name[i] == '.'){
			return name.Mid(i+1);
		}
	}
	return CString("");
}

//////////////////////////////////////////////////////////////////////////////
void CImageDoc::SubmitUndo()
{
	if (m_UndoLevel>=MAX_UNDO_LEVELS){	// Max Undo reached
		delete imageUndo[0];			// discard the early undo
		for(int i=1;i<MAX_UNDO_LEVELS;i++){
			imageUndo[i-1]=imageUndo[i]; //shift the history
		}
		imageUndo[MAX_UNDO_LEVELS-1]=0;	// clear the undo slot
		m_UndoLevel=MAX_UNDO_LEVELS-1;  // reposition at last level
	}
	// we must clear the "redo" history when a new action is performed
	for (int i=m_UndoLevel;i<MAX_UNDO_LEVELS;i++){
		if (imageUndo[i]){
			delete imageUndo[i];
			imageUndo[i]=0;
		}
	}
	// save the actual image in the undo history
	if (image->IsValid()){
		imageUndo[m_UndoLevel] = new CxImage();
		imageUndo[m_UndoLevel]->Copy(*image);
		m_UndoLevel++;
	}
}

void CImageDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSEMDocument::SetPathName(lpszPathName, bAddToMRU);

	//07.06.22
	CString strName = lpszPathName;
	int nPos = strName.Find( '\\' );
	while( nPos != -1 )
	{
		strName = strName.Right( strName.GetLength() - nPos - 1 );
		nPos = strName.Find( '\\' );
	}
	SetTitle( strName );
}

//////////////////////////////////////////////////////////////////////////////
BOOL CImageDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CString filename(lpszPathName);
	CString ext(FindExtension(filename));
	ext.MakeLower();
	if (ext == "") return FALSE;

	int type = 0;
	if (ext == "bmp")					type = CXIMAGE_FORMAT_BMP;
#if CXIMAGE_SUPPORT_JPG
	else if (ext=="jpg"||ext=="jpeg")	type = CXIMAGE_FORMAT_JPG;
#endif
#if CXIMAGE_SUPPORT_GIF
	else if (ext == "gif")				type = CXIMAGE_FORMAT_GIF;
#endif
#if CXIMAGE_SUPPORT_PNG
	else if (ext == "png")				type = CXIMAGE_FORMAT_PNG;
#endif
#if CXIMAGE_SUPPORT_MNG
	else if (ext=="mng"||ext=="jng")	type = CXIMAGE_FORMAT_MNG;
#endif
#if CXIMAGE_SUPPORT_ICO
	else if (ext == "ico")				type = CXIMAGE_FORMAT_ICO;
#endif
#if CXIMAGE_SUPPORT_TIF
	else if (ext=="tiff"||ext=="tif")	type = CXIMAGE_FORMAT_TIF;
#endif
#if CXIMAGE_SUPPORT_TGA
	else if (ext=="tga")				type = CXIMAGE_FORMAT_TGA;
#endif
#if CXIMAGE_SUPPORT_PCX
	else if (ext=="pcx")				type = CXIMAGE_FORMAT_PCX;
#endif
#if SEMIMAGE_SUPPORT_SEM
	else if (ext=="sem")				type = SEMIMAGE_FORMAT_SEM;
#endif
	else return FALSE;

	Stopwatch(0);
	if (type < SEMIMAGE_FORMAT_SEM)
		image = new CxImage(filename, type);
	else
		OpenSemImage(filename, type);
	OpenParams(filename);
	Stopwatch(1);

	// 09.11.18对打开文件不存在的情况加以保护
	// 例如：单击Recent列表中一项，而该项刚好已被从硬盘该位置删除时的情况
	// 本次修改前代码副本
	/*------------------------------------------------- 
	if (!image->IsValid()){
		AfxMessageBox(image->GetLastError());
	-------------------------------------------------*/

	// 本次修改后代码
	//-------------------------------------------------
	if (!image || !image->IsValid()){
		CDlgPrompt dlg;
		dlg.m_byteType = (BYTE)0x01;
		if( image == NULL )
			dlg.m_strMsg = GetResString( IDS_SD_PROMPT_IMAGE_VALID );
		else
		{
			dlg.m_strMsg = image->GetLastError();
			if( dlg.m_strMsg.IsEmpty() )
				dlg.m_strMsg = GetResString( IDS_SD_PROMPT_IMAGE_VALID );
		}
		dlg.DoModal();
		delete image;
		image = NULL;
		return FALSE;
	}
	//-------------------------------------------------

/*	//07.04.26显示图像小样
	CImageItem* pImageItem = new CImageItem();
	pImageItem->m_pIImage->Copy( *GetImage() );
	pImageItem->m_strFileName = filename;
//	gpScanView->m_plistImageItem->AddTail( pImageItem );
//	gpScanView->UpdateItemRect();
	//07.04.26显示图像小样
*/
	if( ((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelThumb.m_hWnd != NULL )
		((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelThumb.AddItem( GetImage(), filename );//GetTitle() );

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
	UpdateScale();	// 05.06.23调整缩放尺寸

	//multiple images (TIFF/ICO)
	if (image->GetNumFrames()>1){
//		CString s;
//		s.Format("File with %d images. Read all?",image->GetNumFrames());
//		if (AfxMessageBox(s,MB_OKCANCEL)==IDOK)
		CDlgPrompt dlg;
		dlg.m_byteType = (BYTE)0x02;
		dlg.m_strMsg.Format("File with %d images. Read all?",image->GetNumFrames());
		if( dlg.DoModal() == IDOK )
		{
			int j; // points to the document name
			for(j=strlen(filename)-1;j>=0;j--){
				if (filename[j]=='\\'){	j++; break;	}
			}
			// create the documents for the other images
/*			for(int i=1;i<image->GetNumFrames();i++){
				CImageDoc *NewDoc=(CImageDoc*)theApp.m_pSemImageDocTemplate->OpenDocumentFile(NULL);
				if (NewDoc)	{
					CxImage *newImage = new CxImage();
					newImage->SetFrame(i);
					newImage->Load(filename,type);
					NewDoc->image = newImage;
					CString s;
					s.Format("%s (%d)",filename.Mid(j),i+1);
					NewDoc->SetTitle(s);
					NewDoc->UpdateAllViews(NULL,WM_USER_NEWIMAGE);
					UpdateScale();	// 05.06.23调整缩放尺寸
				}
			}
*/		}
	}
	
	// EXIF jpegs
	if (image->GetType() == CXIMAGE_FORMAT_JPG){
		FILE* hfile = _tfopen(filename,_T("rb"));
		m_exif.DecodeExif(hfile);
		fclose(hfile);
	}

	return TRUE;
}

void CImageDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	//07.04.26显示图像小样
	CString strTitle = GetTitle();
	CSEMDocument::OnCloseDocument();

//	gpScanView->DeleteItem( strTitle );
	if( ((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelThumb.m_hWnd != NULL )
		((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelThumb.DeleteItem( strTitle );
}

//////////////////////////////////////////////////////////////////////////////
BOOL CImageDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CString filename(lpszPathName);
	CString ext(FindExtension(filename));
	ext.MakeLower();
	if (ext == "") return FALSE;

	int type = 0;
	if (ext == "bmp")					type = CXIMAGE_FORMAT_BMP;
#if CXIMAGE_SUPPORT_JPG
	else if (ext=="jpg"||ext=="jpeg")	type = CXIMAGE_FORMAT_JPG;
#endif
#if CXIMAGE_SUPPORT_GIF
	else if (ext == "gif")				type = CXIMAGE_FORMAT_GIF;
#endif
#if CXIMAGE_SUPPORT_PNG
	else if (ext == "png")				type = CXIMAGE_FORMAT_PNG;
#endif
#if CXIMAGE_SUPPORT_MNG
	else if (ext == "mng")				type = CXIMAGE_FORMAT_MNG;
#endif
#if CXIMAGE_SUPPORT_ICO
	else if (ext == "ico")				type = CXIMAGE_FORMAT_ICO;
#endif
#if CXIMAGE_SUPPORT_TIF
	else if (ext=="tiff"||ext=="tif")	type = CXIMAGE_FORMAT_TIF;
#endif
#if CXIMAGE_SUPPORT_TGA
	else if (ext=="tga")				type = CXIMAGE_FORMAT_TGA;
#endif
#if SEMIMAGE_SUPPORT_SEM
	else if (ext=="sem")				type = SEMIMAGE_FORMAT_SEM;
#endif
	else return FALSE;

	bool retval;
	Stopwatch(0);
	if (type < SEMIMAGE_FORMAT_SEM)
	{
		/////////////////////////////////////////////////////
		// PixelsPerMeter = DPI * 10000.0 / 254.0;
		// DPI = PixelsPerMeter * 254.0 / 10000.0;
		// --------------------------------------------------
		// PixelsPerMeter = Pixels / PhotoSize (m)
		// PixelsPerMeter = Pixels * 1000 / PhotoSize (mm)
		// --------------------------------------------------
		// PhotoWidth  = 120 mm		<==>	ScreenWidth  = 1024 Pixel
		// PhotoHeight =  90 mm		<==>	ScreenHeight =  768 Pixel
		// --------------------------------------------------
		// DPI = Pixels * 25.4 / PhotoSize (mm);
		// --------------------------------------------------
		// DPI_X = 25.4 * 1024 / 127;	// 5寸照片尺寸为12.7cm x 8.9cm
		// DPI_Y = 25.4 *  768 /  90;
		const double dblDPI = 25.4 * image->GetWidth() / 127;
//		const double dblDPI_Y = 25.4 *  768 /  90;
		/////////////////////////////////////////////////////

		image->SetXDPI((long)dblDPI);
		image->SetYDPI((long)dblDPI);
		retval = image->Save(filename, type);
		if( retval )
		{
//			if( type == CXIMAGE_FORMAT_TIF )
				SaveParams(filename);
		}
	}
	else
		retval = SaveSemImage(filename, type);
	Stopwatch(1);
	UpdateStatusBar();

	if (retval)
	{
		CString	csTitle = GetTitle();
		theApp.DisplayMsg( csTitle );

		SetModifiedFlag(FALSE);

		//07.04.26显示图像小样
//		gpScanView->ChangeItemName( csTitle, filename );
		if( ((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelThumb.m_hWnd != NULL )
			((CMainFrame *)theApp.GetMainWnd())->m_wndImpPanelThumb.ChangeItemName( csTitle, filename );

		return TRUE;
	}

	if (type < SEMIMAGE_FORMAT_SEM)
	{
//		AfxMessageBox(image->GetLastError());
		CDlgPrompt dlg;
		dlg.m_byteType = (BYTE)0x01;
		dlg.m_strMsg = image->GetLastError();
		dlg.DoModal();
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////
BOOL CImageDoc::DoSave(LPCTSTR pszPathName, BOOL bReplace /*=TRUE*/)
{
	if (!image)	return FALSE;

	CString newName = pszPathName;
	BOOL bModified = IsModified();

	BOOL bSaveAs = FALSE;
	if (newName.IsEmpty())
		bSaveAs = TRUE;
	else if (!theApp.GetWritableType(image->GetType()))
		bSaveAs = TRUE;

	int nDocType = 0;

	if (bSaveAs)
	{
		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			int iBad = newName.FindOneOf(_T(" #%;/\\"));    // dubious filename
			if (iBad != -1)	//newName.ReleaseBuffer(iBad);
				newName = "UntitledImage";

			if( m_strPathName.IsEmpty()	)					//新文件 10.07.02 增加优先存储格式
			{
				newName += theApp.GetExtFromType( theApp.GetTypeFromIndex( theApp.m_nPriorSaveType, FALSE ) ).Mid(1,4);
			}
			else
			{
				if ( !IsSemImage() )
					// append the default suffix if there is one
					newName += theApp.GetExtFromType(image->GetType()).Mid(1,4);
				else
					newName += theApp.GetExtFromType(SEMIMAGE_FORMAT_SEM).Mid(1,4);
			}
		}

		// 10.07.02
		if( m_strPathName.IsEmpty() )
//			nDocType = theApp.m_nPriorSaveType;
			nDocType = theApp.GetTypeFromIndex( theApp.m_nPriorSaveType, FALSE );
		else
		{
			/////////////////////////////////////////////////////
			// 由于新打开的sem格式图像，其IsSemImage()返回值为FALSE,
			// 故加入对后缀的判断
			CString ext(FindExtension(m_strPathName));
			ext.MakeLower();
			if( !IsSemImage() && ext != "sem" )
				nDocType = image->GetType();
			else
				nDocType = SEMIMAGE_FORMAT_SEM;
			/////////////////////////////////////////////////////
		}

		CString strPrompt1, strPrompt2, strTitle;
		strPrompt1 = GetResString( IDS_SD_PROMPT_DOSAVE1 );
		strPrompt2 = GetResString( IDS_SD_PROMPT_DOSAVE2 );
		strTitle = GetResString( IDS_SD_PROMPT_DOSAVE_TITLE );

PROMPT_SAVE:
		if (!theApp.PromptForFileName(newName, 
			bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
			OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, &nDocType))
		{
			return FALSE;       // don't even try to save
		}

		if (CImageDataManager::Instance().SaveImageData())
		{
			int nPos = newName.ReverseFind('\\');
			CString strFolder = newName.Mid(0,nPos);
			CString strFileName = newName.Mid(nPos + 1,newName.GetLength()- nPos - 1 - 4);
			_SaveImageData2File(strFolder,strFileName);
		}

		if( m_strPathName.IsEmpty()							//新文件
			&& (nDocType != SEMIMAGE_FORMAT_SEM) )			//意图保存为标准格式
		{
//			if( MessageBox( NULL, strPrompt1, strTitle,
//						MB_YESNO | MB_ICONWARNING ) == IDNO )
			CDlgPrompt dlg;
			dlg.m_byteType = (BYTE)0x04;
			dlg.m_strMsg = strPrompt1;
			dlg.m_strTitle = strTitle;
//			if( dlg.DoModal() == IDCANCEL )
//				goto PROMPT_SAVE;
		}
		else if( m_strPathName.IsEmpty()
			&& (nDocType == SEMIMAGE_FORMAT_SEM) )
		{
			// 提示sem格式是否保存图形层
//			if( MessageBox( NULL, strPrompt2, strTitle,
//						MB_YESNO | MB_ICONWARNING ) == IDYES )
// 			CDlgPrompt dlg;
// 			dlg.m_byteType = (BYTE)0x04;
// 			dlg.m_strMsg = strPrompt2;
// 			dlg.m_strTitle = strTitle;
// 			if( dlg.DoModal() == IDYES )
// 				theApp.m_blSaveGraphLay = TRUE;
// 			else
				theApp.m_blSaveGraphLay = FALSE;
		}
		if( (!m_strPathName.IsEmpty()						//已保存文件
				&& (image->GetType() != SEMIMAGE_FORMAT_SEM)//标准格式
				&& (nDocType == SEMIMAGE_FORMAT_SEM) ) )	//另存为sem格式
		{
			//将标准格式另存为sem格式时提示输入加速电压、放大倍数
			CDlgInputAcq dlg;
			dlg.m_nHV	= m_nHV;
			dlg.m_nMag	= m_nMag;
			if( dlg.DoModal() == IDOK )
			{
				m_nHV	= dlg.m_nHV;
				m_nMag	= dlg.m_nMag;
			}
			else
				goto PROMPT_SAVE;
		}
		//======2006.09.19_图像保存========================
	}

	BeginWaitCursor();
	if (!OnSaveDocument(newName)){
		if (pszPathName == NULL){
			// be sure to delete the file
			TRY 
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs\n");
			}
			END_CATCH_ALL
		}
		EndWaitCursor();
		return FALSE;
	}

	EndWaitCursor();
	CString	csTitle = GetTitle();	//07.06.22
	if (bReplace)
	{
		// Reset the title and change the document name
		SetPathName(newName, TRUE);
		ASSERT(m_strPathName == newName); // must be set

		// 10.07.02 如果保存为标准图像，就将ImageLayer重新设置
		CString ext(FindExtension(newName));
		ext.MakeLower();
		if( ext != "sem" )
		{
			if (ImageLayer != NULL)
			{
				ImageLayer->SetEscape(1);
				delete ImageLayer;
				ImageLayer = NULL;
			}

			// 10.07.02 由于CxImage在Save中未能自动更新type，且没有SetType这样的函数进行操作
			// 导致文件正确保存或另存为为标准格式后，其type却不正确
			// 故手动生成新的CxImage，然后使用Copy
//			int nDocTypeCur = image->GetType();	// 可以监测到此时nDocTypeCur值为保存前的图像类型值
			CxImage* imagenew = new CxImage(newName, nDocType);
			image->Copy( *imagenew );
			delete imagenew;
		}
	}
	else // SaveCopyAs
	{
		SetModifiedFlag(bModified);
	}
	UpdateAllViews(NULL);

	//07.06.22
/*	int nPos = csTitle.Find( '.' );
	if( nPos == -1 )
		OnCloseDocument();
*/
	DeleteTempFile( csTitle );		// 10.02.08 如果顺利存储文件，就将临时文件删除
	return TRUE;        // success
}

// 09.03.16
HMENU CImageDoc::GetDefaultMenu()  // get menu depending on state
{
	return CSEMDocument::GetDefaultMenu();
}

// 10.02.08 保存临时文件
BOOL CImageDoc::SaveTempFile()
{
	char path[255];
	::GetModuleFileName( NULL, path, 255);
	char *ptemp = strrchr( path, '\\');
	ptemp[1] = 0;
	CString newName;
	newName.Format( "%s%s.tmpimg", path, GetTitle() );

	Stopwatch(0);

	const double dblDPI = 25.4 * image->GetWidth() / 127;
//	const double dblDPI_Y = 25.4 *  768 /  90;
	image->SetXDPI((long)dblDPI);
	image->SetYDPI((long)dblDPI);
	bool retval = image->Save(newName, CXIMAGE_FORMAT_JPG);

	Stopwatch(1);

	return retval;
}

// 10.02.08 删除临时文件
BOOL CImageDoc::DeleteTempFile( CString strFileName )
{
	char path[255];
	::GetModuleFileName( NULL, path, 255);
	char *ptemp = strrchr( path, '\\');
	ptemp[1] = 0;
	CString newName;
	newName.Format( "%s%s.tmpimg", path, strFileName );

	CFile file;
	if( file.Open( newName, CFile::modeCreate|CFile::modeReadWrite) )
	{
		file.Close();
		remove( newName );
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////

int CImageDoc::ComputePixel(float x, float y, float &x1, float &y1)
{
  double r, nn;

  if (x==0 && y==0) {
	 x1 = x;
	 y1 = y;
	 return 1;
  }

  nn = sqrt(x*x + y*y);
  r =  (fabs(x) > fabs(y)) ? fabs(nn/x): fabs(nn/y);

  x1 = (float)(r*x);
  y1 = (float)(r*y);

  return 1;
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::ComputeNewImage(void)
{
  CWaitCursor wait;
  DWORD x, y, x1, y1;
  float fx, fy, xmid, ymid, ar;
  CxImage *image2 = new CxImage(*image);

  xmid = (float) (image->GetWidth()/2.0);
  ymid = (float) (image->GetHeight()/2.0);
  ar = (float)(image->GetHeight())/(float)(image->GetWidth());
  for (y=0; y<image->GetHeight(); y++) {
	 for (x=0; x<image->GetWidth(); x++) {
		ComputePixel(ar*(x-xmid), y-ymid, fx, fy);
		x1 = (int)(xmid+fx/ar);
		y1 = (int)(ymid+fy);
		if (image->IsInside(x1, y1))
		  image2->SetPixelColor(x, y, image->GetPixelColor(x1, y1));
		else {
			if (image2->GetNumColors()==0){
				image2->SetPixelColor(x, y,RGB(255,255,255));
			} else {
				image2->SetPixelIndex(x, y,(BYTE)image2->GetTransIndex());
			}
		}
	 }
  }
  delete image;
  image = image2;
  UpdateAllViews(NULL);
}

//////////////////////////////////////////////////////////////////////////////
void CImageDoc::Stopwatch(int start0stop1)
{
	if (start0stop1==0)	QueryPerformanceCounter(&m_swStart);
	else {
		QueryPerformanceCounter(&m_swStop);
		if (m_swFreq.LowPart==0 && m_swFreq.HighPart==0) m_etime = -1;
		else {
			m_etime = (float)(m_swStop.LowPart - m_swStart.LowPart);
			if (m_etime < 0) m_etime += 2^32;
			m_etime /= (m_swFreq.LowPart+m_swFreq.HighPart * 2^32);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
void CImageDoc::UpdateStatusBar()
{
	if (image)
	{
		POSITION pos = GetFirstViewPosition();
		if( pos )
		{
			CImageView* pview = (CImageView*)(GetNextView(pos));
			CStatusBar& statusBar = ((CChildFrame *)(pview->GetParent()))->m_wndStatusBar;
			CString s;
			s.Format("(%dx%dx%d)",image->GetWidth(),image->GetHeight(),image->GetBpp());
			statusBar.SetPaneText(statusBar.CommandToIndex(ID_INDICATOR_IMAGE_PROPERTY), s);
			s.Format("Time (s): %.3f",m_etime);
			statusBar.SetPaneText(statusBar.CommandToIndex(ID_INDICATOR_OPEN_TIME), s);
			s.Format("%s; Speed=%dus/s, Sync=%d, Sig=%s; HV=%dV, Beam=%duA, Fil=%dmA, EMI=%duA; B=%s; C=%s; CL=%s; OL=%s; Mag=%s; WD=%s",
				m_ac.strTime, m_ac.nSpeed, m_ac.nIsSync, m_ac.strSignal,
				m_ac.nHV, m_ac.nHV2, m_ac.nFil, m_ac.nEMI,
				m_ac.strB, m_ac.strC, m_ac.strCL, m_ac.strOL, m_ac.strMag, m_ac.strWD );
			statusBar.SetPaneText(statusBar.CommandToIndex(ID_INDICATOR_IMAGE_INFO), s);

//			((CMainFrame *)theApp.GetMainWnd())->GetProgressBar().SetPos(0);
		}
	}
}


void  CImageDoc::SetData(int nRow,int nColumn,WORD dwData)
{
	m_ImageColor[nRow][nColumn] = dwData;
}

void CImageDoc::OnStretchMode() 
{
	// TODO: Add your command handler code here
	stretchMode = !stretchMode;
	UpdateAllViews(NULL);
	
}

void CImageDoc::OnUpdateStretchMode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(stretchMode);
	
}

//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnViewZoomIn() 
{
	SetZoom( 0 );
}

void CImageDoc::OnUpdateViewZoomIn(CCmdUI* pCmdUI) 
{
	if (stretchMode) pCmdUI->Enable(0);
	if (m_dScale == g_dScaleImageTable[g_nScaleImageIndexMax])
		pCmdUI->Enable(0);
}

void CImageDoc::OnViewZoomOut() 
{
	SetZoom( 1 );
}

void CImageDoc::OnUpdateViewZoomOut(CCmdUI* pCmdUI) 
{
	if (stretchMode) pCmdUI->Enable(0);
	if (m_dScale == g_dScaleImageTable[g_nScaleImageIndexMin]) pCmdUI->Enable(0);
}

void CImageDoc::OnViewActualSize() 
{
	SetZoom( 2 );
}

void CImageDoc::OnUpdateViewActualSize(CCmdUI* pCmdUI) 
{
	if (stretchMode) pCmdUI->Enable(0);
	if (m_dScale == 1) pCmdUI->Enable(0);
}

// 2009.03.20 合并缩放函数
void CImageDoc::SetZoom( int nStyle )
{
	switch( nStyle )
	{
	case	0:	// Zoom In
		// 05.06.23 根据图像大小调整缩放因子
		if (++g_nScaleImageIndex > g_nScaleImageIndexMax )
			g_nScaleImageIndex = g_nScaleImageIndexMax;
		m_dScale = g_dScaleImageTable[g_nScaleImageIndex];
		break;
	case	1:	// Zoom Out
		// 05.06.23 根据图像大小调整缩放因子
		if (--g_nScaleImageIndex < g_nScaleImageIndexMin )
			g_nScaleImageIndex = g_nScaleImageIndexMin;
		m_dScale = g_dScaleImageTable[g_nScaleImageIndex];
		break;
	case	2:	// Actual Size
	//	m_dScale = 1.;
		SetScale( 1.0 );
		break;
	default:
		break;
	}

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}

//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnWindowDuplicate() 
{
	CImageDoc *NewDoc=(CImageDoc*)theApp.m_pSemImageDocTemplate->OpenDocumentFile(NULL); 

	if (NewDoc)	{
		CxImage *newImage = new CxImage(*image);
		newImage->Copy(*image);

		NewDoc->image = newImage;

		CString s;
		s.Format("Copy %d of %s",theApp.m_nImageDocCount++,GetTitle());
		NewDoc->SetTitle(s);
		NewDoc->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	if (image==NULL) pCmdUI->Enable(0);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnEditCopy() 
{
	HANDLE hDIB=image->CopyToHandle();

	if (::OpenClipboard(AfxGetApp()->m_pMainWnd->GetSafeHwnd())) {
		if(::EmptyClipboard()) {
			if (::SetClipboardData(CF_DIB,hDIB) == NULL ) {
//				AfxMessageBox( "Unable to set Clipboard data" );
				CDlgPrompt dlg;
				dlg.m_byteType = (BYTE)0x01;
				dlg.m_strMsg = _T("Unable to set Clipboard data");
				dlg.DoModal();
			}
		}
	}
	CloseClipboard();
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	if(image==0 || hThread) pCmdUI->Enable(0);
	else pCmdUI->Enable(m_UndoLevel>0);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	if(image==0 || hThread) pCmdUI->Enable(0);
	else pCmdUI->Enable((m_UndoLevel<(MAX_UNDO_LEVELS))&&
				   (imageUndo[m_UndoLevel]!=0));
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnEditUndo() 
{
	m_UndoLevel--;
	CxImage* tmp = image;
	image=imageUndo[m_UndoLevel];
	imageUndo[m_UndoLevel]=tmp;

	UpdateAllViews(0,WM_USER_NEWIMAGE);
	UpdateStatusBar();
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnEditRedo() 
{
	CxImage* tmp = image;
	image=imageUndo[m_UndoLevel];
	imageUndo[m_UndoLevel]=tmp;
	m_UndoLevel++;

	UpdateAllViews(0,WM_USER_NEWIMAGE);
	UpdateStatusBar();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void /*unsigned long _stdcall*/ RunProgressThread(void *lpParam)
{
	CImageDoc *pDoc = (CImageDoc *)lpParam;
	POSITION pos;
	CView *pView;
	while(pDoc->hThread){
		Sleep(333);
		if(!pDoc->image) break;
		if(pDoc->image->GetEscape()) break;
		long n=pDoc->image->GetProgress();
		pos = pDoc->GetFirstViewPosition();
		pView = pDoc->GetNextView(pos);
		if (pView) SendMessage(pView->m_hWnd, WM_USER_PROGRESS,n,0);
	}
	pos = pDoc->GetFirstViewPosition();
	pView = pDoc->GetNextView(pos);
	if (pView) SendMessage(pView->m_hWnd, WM_USER_PROGRESS,100,0);
	Sleep(111);
	pos = pDoc->GetFirstViewPosition();
	pView = pDoc->GetNextView(pos);
	if (pView) SendMessage(pView->m_hWnd, WM_USER_NEWIMAGE,0,0);
	pDoc->hProgress = 0;
	_endthread();
	return;
}
//////////////////////////////////////////////////////////////////////////////
void /*unsigned long _stdcall*/ RunCxImageThread(void *lpParam)
{
	CImageDoc *pDoc = (CImageDoc *)lpParam;
	if (pDoc==NULL) return;
	if (pDoc->image==NULL){
		pDoc->hThread=0;
		return;
	}

	//prepare for elaboration
	pDoc->image->SetProgress(0);
	pDoc->image->SetEscape(0);

	pDoc->SubmitUndo();

	// auxilary thread for progress bar
	pDoc->hProgress = (HANDLE)_beginthread(RunProgressThread,0,pDoc);

	pDoc->Stopwatch(0);

	///////////////////////////////////////////
	long		i;
	LPBYTE		pImage, pImageLine;
	///////////////////////////////////////////
	bool status = true;

	switch (pDoc->m_MenuCommand)
	{
//	case ID_FILE_OPEN:
//		pDoc->image->ReadFile(*(CString*)pDoc->m_fp[0],(int)pDoc->m_fp[1]);
//		break;
	case ID_CXIMAGE_PHOTO:
		///////////////////////////////////////////
		pDoc->image->GrayScale();
		///////////////////////////////////////////
		if ( (pDoc->image->GetWidth() != 1024)
			|| (pDoc->image->GetHeight() != 768) )
		{
			pDoc->image->Resample(1024,768,1);
		}
		///////////////////////////////////////////
		pImage = pDoc->image->GetBits();
		for (i=0; i<768; i++)
		{
			pImageLine = &(pImage[(767-i)*1024]);
			CSemVirtualMCICard::Instance().Write( 
				pImageLine, 1024, CRect(0,i,1024,i+1) );
		}
		///////////////////////////////////////////
		break;
	case ID_CXIMAGE_FLIP:
		status = pDoc->image->Flip();
		break;
	case ID_CXIMAGE_MIRROR:
		status = pDoc->image->Mirror();
		break;
	case ID_CXIMAGE_NEGATIVE:
		status = pDoc->image->Negative();
		break;
	case ID_CXIMAGE_GRAYSCALE:
		status = pDoc->image->GrayScale();
		break;
	case ID_CXIMAGE_DITHER:
		status = pDoc->image->Dither(theApp.m_Filters.DitherMethod);
		break;
	case ID_CXIMAGE_THRESHOLD:
		if (theApp.m_Filters.ThreshPreserveColors){
			RGBQUAD c = {255,255,255,0};
			status = pDoc->image->Threshold2(theApp.m_Filters.ThreshLevel,true,c,true);
		} else {
			status = pDoc->image->Threshold(theApp.m_Filters.ThreshLevel);
		}
		break;
	case ID_CXIMAGE_COLORIZE:
		switch (theApp.m_Filters.ColorMode)
		{
		case 1:
			status = pDoc->image->Colorize(theApp.m_Filters.ColorHSL.rgbRed,theApp.m_Filters.ColorHSL.rgbGreen,theApp.m_Filters.ColorHSL.rgbBlue/100.0f);
			break;
		case 2:
			status = pDoc->image->Solarize(theApp.m_Filters.ColorSolarLevel,theApp.m_Filters.ColorSolarLink!=0);
			break;
		default:
			status = pDoc->image->ShiftRGB(theApp.m_Filters.ColorRed,theApp.m_Filters.ColorGreen,theApp.m_Filters.ColorBlue);
		}
		break;
	case ID_CXIMAGE_LIGHTEN:
		status = pDoc->image->Light(10);
		break;
	case ID_CXIMAGE_DARKEN:
		status = pDoc->image->Light(-10);
		break;
	case ID_CXIMAGE_CONTRAST:
		status = pDoc->image->Light((long)pDoc->m_fp[0],20);
		break;
	case ID_CXIMAGE_LESSCONTRAST:
		status = pDoc->image->Light(0,-10);
		break;
	case ID_CXIMAGE_DILATE:
		status = pDoc->image->Dilate(3);
		break;
	case ID_CXIMAGE_ERODE:
		status = pDoc->image->Erode(3);
		break;
	case ID_CXIMAGE_EMBOSS:
		{
			long kernel[]={0,0,-1,0,0,0,1,0,0};
			status = pDoc->image->Filter(kernel,3,-1,127);
			break;
		}
	case ID_CXIMAGE_BLUR:
		{
			long kernel[]={1,1,1,1,1,1,1,1,1};
			status = pDoc->image->Filter(kernel,3,9,0);
			break;
		}
	case ID_CXIMAGE_SOFTEN:
		{
			long kernel[]={1,1,1,1,8,1,1,1,1};
			status = pDoc->image->Filter(kernel,3,16,0);
			break;
		}
	case ID_CXIMAGE_SHARPEN:
		{
			long kernel[]={-1,-1,-1,-1,15,-1,-1,-1,-1};
			status = pDoc->image->Filter(kernel,3,7,0);
			break;
		}
	case ID_CXIMAGE_EDGE:
		{
			long kernel[]={-1,-1,-1,-1,8,-1,-1,-1,-1};
			status = pDoc->image->Filter(kernel,3,-1,255);
			break;
		}
	case ID_CXIMAGE_GAMMA:
		if (theApp.m_Filters.GammaLink){
			status = pDoc->image->GammaRGB(theApp.m_Filters.GammaR,theApp.m_Filters.GammaG,theApp.m_Filters.GammaB);
		} else {
			status = pDoc->image->Gamma(theApp.m_Filters.GammaLevel);
		}
		break;
	case ID_CXIMAGE_ROTATE:
		//***bd*** more rotation options
		CxImage::InterpolationMethod intm;
		CxImage::OverflowMethod overm;
		switch (theApp.m_Filters.RotateMethod) {
		case 0: intm=CxImage::IM_NEAREST_NEIGHBOUR; break;
		case 1: intm=CxImage::IM_BILINEAR; break;
		case 2: intm=CxImage::IM_BICUBIC; break;
		case 3: intm=CxImage::IM_BICUBIC2; break;
		case 4: intm=CxImage::IM_BSPLINE; break;
		case 5: intm=CxImage::IM_LANCZOS; break;
		case 6: intm=CxImage::IM_HERMITE; break;
		default: throw(0);
		}//switch
		switch (theApp.m_Filters.RotateOverflow) {
		case 0: overm=CxImage::OM_BACKGROUND; break;
		case 1: overm=CxImage::OM_BACKGROUND; break;
		case 2: overm=CxImage::OM_BACKGROUND; break;
		case 3: overm=CxImage::OM_WRAP; break;
		case 4: overm=CxImage::OM_REPEAT; break;
		case 5: overm=CxImage::OM_MIRROR; break;
		case 6: overm=CxImage::OM_TRANSPARENT; break;
		}//switch
		switch (theApp.m_Filters.RotateOverflow) {
		case 0: {
			RGBQUAD bkg = pDoc->image->GetPixelColor(0,0);
			status = pDoc->image->Rotate2(theApp.m_Filters.RotateAngle, 0, intm, overm, &bkg,true,theApp.m_Filters.RotateKeepsize!=0);
			break; }
		case 1: {
			RGBQUAD bkg = {0,0,0,0};
			status = pDoc->image->Rotate2(theApp.m_Filters.RotateAngle, 0, intm, overm, &bkg,true,theApp.m_Filters.RotateKeepsize!=0);
			break; }
		default:
			status = pDoc->image->Rotate2(theApp.m_Filters.RotateAngle, 0, intm, overm, 0,true,theApp.m_Filters.RotateKeepsize!=0);
		}
		break;
	case ID_CXIMAGE_ROTATEL:
		status = pDoc->image->RotateLeft();
		if (status) pDoc->RegionRotateLeft();
		break;
	case ID_CXIMAGE_ROTATER:
		status = pDoc->image->RotateRight();
		if (status) pDoc->RegionRotateRight();
		break;
	case ID_CXIMAGE_RESAMPLE:
		//***bd*** more resample options
		CxImage::InterpolationMethod rintm;
		switch (theApp.m_Filters.ResampleMethod) {
		case 0: rintm=CxImage::IM_NEAREST_NEIGHBOUR; break;
		case 1: rintm=CxImage::IM_BILINEAR; break;
		case 2: rintm=CxImage::IM_BILINEAR; break;
		case 3: rintm=CxImage::IM_BICUBIC; break;
		case 4: rintm=CxImage::IM_BICUBIC2; break;
		case 5: rintm=CxImage::IM_BSPLINE; break;
		case 6: rintm=CxImage::IM_LANCZOS; break;
		case 7: rintm=CxImage::IM_HERMITE; break;
		default: throw(0);
		}//switch
		switch (theApp.m_Filters.ResampleMethod) {
		case 0:
			status = pDoc->image->Resample(theApp.m_Filters.ResampleW,theApp.m_Filters.ResampleH,1);
			break;
		case 1:
			status = pDoc->image->Resample(theApp.m_Filters.ResampleW,theApp.m_Filters.ResampleH,0);
			break;
		case 2:
			if ((long)pDoc->image->GetWidth()>theApp.m_Filters.ResampleW && (long)pDoc->image->GetHeight()>theApp.m_Filters.ResampleH)
				status = pDoc->image->QIShrink(theApp.m_Filters.ResampleW,theApp.m_Filters.ResampleH);
			else
				status = pDoc->image->Resample2(theApp.m_Filters.ResampleW,theApp.m_Filters.ResampleH,rintm,CxImage::OM_REPEAT);
			break;
		default:
			status = pDoc->image->Resample2(theApp.m_Filters.ResampleW,theApp.m_Filters.ResampleH,rintm,CxImage::OM_REPEAT);
		}
		break;
	case ID_CXIMAGE_INCREASEBPP:
		status = pDoc->image->IncreaseBpp(theApp.m_Filters.IncBppBPP);
		break;
	case ID_CXIMAGE_DECREASEBPP:
		{
			long bit = theApp.m_Filters.DecBppBPP;
			long method = theApp.m_Filters.DecBppPalMethod;
			bool errordiffusion = theApp.m_Filters.DecBppErrDiff!=0;
			long colors = theApp.m_Filters.DecBppMaxColors;

			//pDoc->image->IncreaseBpp(24);
			RGBQUAD c = pDoc->image->GetTransColor();

			RGBQUAD* ppal = NULL;
			if (method==1){
				switch (bit){
				/*case 1:
					{
						CQuantizer q(2,8);
						q.ProcessImage(pDoc->image->GetDIB());
						ppal=(RGBQUAD*)calloc(2*sizeof(RGBQUAD),1);
						q.SetColorTable(ppal);
						break;
					}*/
				case 4:
					{
						CQuantizer q(colors,8);
						q.ProcessImage(pDoc->image->GetDIB());
						ppal=(RGBQUAD*)calloc(16*sizeof(RGBQUAD),1);
						q.SetColorTable(ppal);
						break;
					}
				case 8:
					{
						CQuantizer q(colors,(colors>16?7:8));
						q.ProcessImage(pDoc->image->GetDIB());
						ppal=(RGBQUAD*)calloc(256*sizeof(RGBQUAD),1);
						q.SetColorTable(ppal);
					}
				}
				status = pDoc->image->DecreaseBpp(bit,errordiffusion,ppal,colors);
			} else status = pDoc->image->DecreaseBpp(bit,errordiffusion,0);

			if (!pDoc->image->AlphaPaletteIsValid()) pDoc->image->AlphaPaletteEnable(0);

			if (pDoc->image->IsTransparent()){
				pDoc->image->SetTransIndex(pDoc->image->GetNearestIndex(c));
			}

			if (ppal) free(ppal);
			break;
		}
	}

	pDoc->Stopwatch(1);

	pDoc->image->SetProgress(100);

	pDoc->hThread=0;
	_endthread();
	return ;
}

//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnUpdateCximageFlip(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageGrayscale(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageMirror(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageNegative(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageResample(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageRotate(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageRotater(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageRotatel(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageThreshold(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageColorize(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageLighten(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageDarken(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageContrast(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageLessContrast(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageEmboss(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageBlur(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageDilate(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageEdge(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageErode(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageSharpen(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageSoften(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageDither(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageRemovetransparency(CCmdUI* pCmdUI) 
{	if(image==0 || hThread || image->GetTransIndex()<0) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageSplityuv(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageSplitrgb(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageSplithsl(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximagePseudocolors(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageRemovealphachannel(CCmdUI* pCmdUI) 
{	if(image==0 || hThread/* || !image->HasAlpha()*/) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageInvetalpha(CCmdUI* pCmdUI) 
{	if(image==0 || hThread/* || !image->HasAlpha()*/) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageAlphapalettetoggle(CCmdUI* pCmdUI) 
{	if(image==0 || hThread/* || !image->HasAlphaPalette()*/) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageAlphastrip(CCmdUI* pCmdUI) 
{	if(image==0 || hThread/* || (!image->HasAlpha() && !image->HasAlphaPalette())*/) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageFft(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageGamma(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageJpeg(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageRepair(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CImageDoc::OnUpdateCximageShadow(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}

//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageFlip() 
{
	m_MenuCommand=ID_CXIMAGE_FLIP;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageMirror() 
{
	m_MenuCommand=ID_CXIMAGE_MIRROR;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageNegative() 
{
	m_MenuCommand=ID_CXIMAGE_NEGATIVE;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageGrayscale() 
{
	m_MenuCommand=ID_CXIMAGE_GRAYSCALE;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}

//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageRotatel() 
{
	m_MenuCommand=ID_CXIMAGE_ROTATEL;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageRotater() 
{
	m_MenuCommand=ID_CXIMAGE_ROTATER;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageRotate() 
{
	if (image==NULL) return;
	CDlgRotate dlg;
	dlg.m_angle = theApp.m_Filters.RotateAngle;
	dlg.m_method = theApp.m_Filters.RotateMethod;
	dlg.m_overflow = theApp.m_Filters.RotateOverflow;
	dlg.m_keepsize = theApp.m_Filters.RotateKeepsize;
	if (dlg.DoModal()==IDOK){
		m_MenuCommand=ID_CXIMAGE_ROTATE;
		theApp.m_Filters.RotateAngle = dlg.m_angle;
		theApp.m_Filters.RotateMethod = dlg.m_method;
		theApp.m_Filters.RotateOverflow = dlg.m_overflow;
		theApp.m_Filters.RotateKeepsize = dlg.m_keepsize;
		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageResample() 
{
	if (image==NULL) return;
	
	CDlgResample dlg;
	dlg.m_w = image->GetWidth();
	dlg.m_h = image->GetHeight();
	dlg.m_ratio = ((float)image->GetWidth())/((float)image->GetHeight());
	
	dlg.m_sizemode = theApp.m_Filters.ResampleSizemode;
	dlg.m_factor = theApp.m_Filters.ResampleFactor;
	dlg.m_newwidth = theApp.m_Filters.ResampleW;
	dlg.m_newheight = theApp.m_Filters.ResampleH;
	dlg.m_bKeepRatio = theApp.m_Filters.ResampleKeepRatio;
	dlg.m_mode = theApp.m_Filters.ResampleMethod;
	if (dlg.m_bKeepRatio) dlg.m_newheight = (DWORD)(dlg.m_newwidth/dlg.m_ratio);
	
	if (dlg.DoModal()==IDOK){
		m_MenuCommand=ID_CXIMAGE_RESAMPLE;
		
		if (dlg.m_sizemode==1){
			dlg.m_newwidth = (DWORD)(dlg.m_w * fabs(dlg.m_factor));
			dlg.m_newheight = (DWORD)(dlg.m_h * fabs(dlg.m_factor));
		}
		
		theApp.m_Filters.ResampleSizemode = dlg.m_sizemode;
		theApp.m_Filters.ResampleFactor = dlg.m_factor;
		theApp.m_Filters.ResampleW = dlg.m_newwidth;
		theApp.m_Filters.ResampleH = dlg.m_newheight;
		theApp.m_Filters.ResampleKeepRatio = dlg.m_bKeepRatio;
		theApp.m_Filters.ResampleMethod = dlg.m_mode;
		
		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}
}

//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageDecreasebpp() 
{
	if (image==NULL) return;
	CDlgDecBpp dlg;
	dlg.m_bit=4;
	if (dlg.DoModal()==IDOK){
		m_MenuCommand=ID_CXIMAGE_DECREASEBPP;
		m_fp[0]=(void *)dlg.m_bit;
		m_fp[1]=(void *)dlg.m_method;
		m_fp[2]=(void *)dlg.m_errordiffusion;
		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageIncreasebpp() 
{
	if (image==NULL) return;
	CDlgIncBpp dlg;
	dlg.m_bit=24;
	if (dlg.DoModal()==IDOK){
		m_MenuCommand=ID_CXIMAGE_INCREASEBPP;
		m_fp[0]=(void *)dlg.m_bit;
		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageThreshold() 
{
	if (image==NULL) return;
	CDlgThreshold dlg;
	
	CxImage iContrastMask;
	iContrastMask.Copy(*image,true,false,false);
	if (!iContrastMask.IsValid()){
		AfxMessageBox(_T("cannot create ContrastMask")); 
		return;
	}
	iContrastMask.GrayScale();
	long edge[]={-1,-1,-1,-1,8,-1,-1,-1,-1};
	iContrastMask.Filter(edge,3,1,0);
	long blur[]={1,1,1,1,1,1,1,1,1};
	iContrastMask.Filter(blur,3,9,0);
	
	if (image->IsGrayScale()){
		dlg.m_thresh1 = (long)image->OptimalThreshold(0,0);
		dlg.m_thresh2 = (long)image->OptimalThreshold(0,0,&iContrastMask);
	} else {
		CxImage iGray;
		iGray.Copy(*image,true,false,false);
		iGray.GrayScale();
		dlg.m_thresh1 = (long)iGray.OptimalThreshold(0,0);
		dlg.m_thresh2 = (long)iGray.OptimalThreshold(0,0,&iContrastMask);
	}
	
	dlg.m_mean = (BYTE)image->Mean();
	dlg.m_bPreserve = theApp.m_Filters.ThreshPreserveColors;
	dlg.m_level = theApp.m_Filters.ThreshLevel;
	
	if (dlg.DoModal()==IDOK){
		m_MenuCommand=ID_CXIMAGE_THRESHOLD;
		theApp.m_Filters.ThreshLevel = dlg.m_level;
		theApp.m_Filters.ThreshPreserveColors = dlg.m_bPreserve;
		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}
}

void CImageDoc::OnCximageColorize() 
{
	// TODO: Add your command handler code here
	if (image==NULL) return;
	CDlgColorize dlg;

	dlg.m_bHSL = theApp.m_Filters.ColorMode;
	dlg.m_blend = theApp.m_Filters.ColorHSL.rgbBlue;
	dlg.m_sat = theApp.m_Filters.ColorHSL.rgbGreen;
	dlg.m_hue = theApp.m_Filters.ColorHSL.rgbRed;
	dlg.m_b = theApp.m_Filters.ColorBlue;
	dlg.m_g = theApp.m_Filters.ColorGreen;
	dlg.m_r = theApp.m_Filters.ColorRed;
	dlg.m_sol = theApp.m_Filters.ColorSolarLevel;
	dlg.m_bLinked = theApp.m_Filters.ColorSolarLink;

	if (dlg.DoModal()==IDOK){
		m_MenuCommand=ID_CXIMAGE_COLORIZE;

		theApp.m_Filters.ColorMode = dlg.m_bHSL;
		theApp.m_Filters.ColorHSL.rgbBlue = dlg.m_blend;
		theApp.m_Filters.ColorHSL.rgbGreen = dlg.m_sat;
		theApp.m_Filters.ColorHSL.rgbRed = dlg.m_hue;
		theApp.m_Filters.ColorBlue = dlg.m_b;
		theApp.m_Filters.ColorGreen = dlg.m_g;
		theApp.m_Filters.ColorRed = dlg.m_r;
		theApp.m_Filters.ColorSolarLevel = dlg.m_sol;
		theApp.m_Filters.ColorSolarLink = dlg.m_bLinked;

		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}
}

//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageDarken() 
{
	m_MenuCommand=ID_CXIMAGE_DARKEN;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageLighten() 
{
	m_MenuCommand=ID_CXIMAGE_LIGHTEN;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageContrast() 
{
	m_MenuCommand=ID_CXIMAGE_CONTRAST;
	m_fp[0]=(void *)(long)((128-image->Mean())/4);
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageEmboss() 
{
	m_MenuCommand=ID_CXIMAGE_EMBOSS;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageBlur() 
{
	m_MenuCommand=ID_CXIMAGE_BLUR;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageDilate() 
{
	m_MenuCommand=ID_CXIMAGE_DILATE;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageEdge() 
{
	m_MenuCommand=ID_CXIMAGE_EDGE;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageErode() 
{
	m_MenuCommand=ID_CXIMAGE_ERODE;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageSharpen() 
{
	m_MenuCommand=ID_CXIMAGE_SHARPEN;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageSoften() 
{
	m_MenuCommand=ID_CXIMAGE_SOFTEN;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////

void CImageDoc::OnCximageDither() 
{
	// TODO: Add your command handler code here
	if (image==NULL) return;
	
	CDlgDither dlg;
	dlg.m_method = theApp.m_Filters.DitherMethod;
	if (dlg.DoModal()==IDOK){
		m_MenuCommand=ID_CXIMAGE_DITHER;
		theApp.m_Filters.DitherMethod = dlg.m_method;
		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}
}

void CImageDoc::OnCximageOptions() 
{
	// TODO: Add your command handler code here
	if (image==NULL) return;
	
	CDlgOptions dlg;
	dlg.m_jpeg_quality = image->GetJpegQualityF();
	dlg.m_xres = image->GetXDPI();
	dlg.m_yres = image->GetYDPI();
	
#if CXIMAGE_SUPPORT_TIF
	dlg.m_Opt_tif = image->GetCodecOption(CXIMAGE_FORMAT_TIF);
#endif
#if CXIMAGE_SUPPORT_GIF
	dlg.m_Opt_gif = image->GetCodecOption(CXIMAGE_FORMAT_GIF);
#endif
#if CXIMAGE_SUPPORT_JPG
	dlg.m_Opt_jpg = image->GetCodecOption(CXIMAGE_FORMAT_JPG);
#endif
#if CXIMAGE_SUPPORT_PNG
	dlg.m_Opt_png = image->GetCodecOption(CXIMAGE_FORMAT_PNG);
#endif
#if CXIMAGE_SUPPORT_RAW
	dlg.m_Opt_raw = image->GetCodecOption(CXIMAGE_FORMAT_RAW);
#endif
	
	dlg.m_exif = &m_exif;
	if (dlg.DoModal()==IDOK){
		image->SetJpegQualityF(dlg.m_jpeg_quality);
		image->SetXDPI(dlg.m_xres);
		image->SetYDPI(dlg.m_yres);
		
#if CXIMAGE_SUPPORT_TIF
		image->SetCodecOption(dlg.m_Opt_tif, CXIMAGE_FORMAT_TIF);
#endif
#if CXIMAGE_SUPPORT_GIF
		image->SetCodecOption(dlg.m_Opt_gif, CXIMAGE_FORMAT_GIF);
#endif
#if CXIMAGE_SUPPORT_JPG
		image->SetCodecOption(dlg.m_Opt_jpg, CXIMAGE_FORMAT_JPG);
#endif
#if CXIMAGE_SUPPORT_PNG
		image->SetCodecOption(dlg.m_Opt_png, CXIMAGE_FORMAT_PNG);
#endif
#if CXIMAGE_SUPPORT_RAW
		image->SetCodecOption(dlg.m_Opt_raw, CXIMAGE_FORMAT_RAW);
#endif
		
#ifdef VATI_EXTENSIONS
		theApp.m_optJpegQuality = dlg.m_jpeg_quality;
		theApp.m_optJpegOptions = dlg.m_Opt_jpg;
		theApp.m_optRawOptions  = dlg.m_Opt_raw;
#endif
	}
}

void CImageDoc::OnCximageSettransparency() 
{
	// TODO: Add your command handler code here
	m_WaitingClick=TRUE;
}

void CImageDoc::OnUpdateCximageSettransparency(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (image && hThread==0) pCmdUI->Enable(1);
	else pCmdUI->Enable(0);
}

void CImageDoc::OnCximageRemovetransparency() 
{
	// TODO: Add your command handler code here
	SubmitUndo();
	if (image) image->SetTransIndex(-1);
	UpdateAllViews(NULL);
}

void CImageDoc::OnCximageSplithsl() 
{
	// TODO: Add your command handler code here
	if (image==NULL) return;

	CxImage *newr = new CxImage();
	CxImage *newg = new CxImage();
	CxImage *newb = new CxImage();

	image->SplitHSL(newr,newg,newb);
	
	CImageDoc *NewDocr=(CImageDoc*)theApp.m_pSemImageDocTemplate->OpenDocumentFile(NULL); 
	if (NewDocr)	{
		NewDocr->image = newr;
		CString s;
		s.Format("Hue Channel %d from %s",theApp.m_nImageDocCount++,GetTitle());
		NewDocr->SetTitle(s);
		NewDocr->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CImageDoc *NewDocg=(CImageDoc*)theApp.m_pSemImageDocTemplate->OpenDocumentFile(NULL); 
	if (NewDocg)	{
		NewDocg->image = newg;
		CString s;
		s.Format("Saturation Channel %d from %s",theApp.m_nImageDocCount++,GetTitle());
		NewDocg->SetTitle(s);
		NewDocg->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CImageDoc *NewDocb=(CImageDoc*)theApp.m_pSemImageDocTemplate->OpenDocumentFile(NULL); 
	if (NewDocb)	{
		NewDocb->image = newb;
		CString s;
		s.Format("Lightness Channel %d from %s",theApp.m_nImageDocCount++,GetTitle());
		NewDocb->SetTitle(s);
		NewDocb->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
}

void CImageDoc::OnCximageSplitrgb() 
{
	// TODO: Add your command handler code here
	if (image==NULL) return;

	CxImage *newr = new CxImage();
	CxImage *newg = new CxImage();
	CxImage *newb = new CxImage();

	image->SplitRGB(newr,newg,newb);
	
	CImageDoc *NewDocr=(CImageDoc*)theApp.m_pSemImageDocTemplate->OpenDocumentFile(NULL); 
	if (NewDocr)	{
		NewDocr->image = newr;
		CString s;
		s.Format("Red Channel %d from %s",theApp.m_nImageDocCount++,GetTitle());
		NewDocr->SetTitle(s);
		NewDocr->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CImageDoc *NewDocg=(CImageDoc*)theApp.m_pSemImageDocTemplate->OpenDocumentFile(NULL); 
	if (NewDocg)	{
		NewDocg->image = newg;
		CString s;
		s.Format("Green Channel %d from %s",theApp.m_nImageDocCount++,GetTitle());
		NewDocg->SetTitle(s);
		NewDocg->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CImageDoc *NewDocb=(CImageDoc*)theApp.m_pSemImageDocTemplate->OpenDocumentFile(NULL); 
	if (NewDocb)	{
		NewDocb->image = newb;
		CString s;
		s.Format("Blue Channel %d from %s",theApp.m_nImageDocCount++,GetTitle());
		NewDocb->SetTitle(s);
		NewDocb->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
}

void CImageDoc::OnCximageSplityuv() 
{
	// TODO: Add your command handler code here
	if (image==NULL) return;

	CxImage *newr = new CxImage();
	CxImage *newg = new CxImage();
	CxImage *newb = new CxImage();

	image->SplitYUV(newr,newg,newb);
	
	CImageDoc *NewDocr=(CImageDoc*)theApp.m_pSemImageDocTemplate->OpenDocumentFile(NULL); 
	if (NewDocr)	{
		NewDocr->image = newr;
		CString s;
		s.Format("Y Channel %d from %s",theApp.m_nImageDocCount++,GetTitle());
		NewDocr->SetTitle(s);
		NewDocr->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CImageDoc *NewDocg=(CImageDoc*)theApp.m_pSemImageDocTemplate->OpenDocumentFile(NULL); 
	if (NewDocg)	{
		NewDocg->image = newg;
		CString s;
		s.Format("U Channel %d from %s",theApp.m_nImageDocCount++,GetTitle());
		NewDocg->SetTitle(s);
		NewDocg->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CImageDoc *NewDocb=(CImageDoc*)theApp.m_pSemImageDocTemplate->OpenDocumentFile(NULL); 
	if (NewDocb)	{
		NewDocb->image = newb;
		CString s;
		s.Format("V Channel %d from %s",theApp.m_nImageDocCount++,GetTitle());
		NewDocb->SetTitle(s);
		NewDocb->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
}

void CImageDoc::OnCximagePseudocolors() 
{
	// TODO: Add your command handler code here
	if (image==NULL) return;

	SubmitUndo();

	if (!image->IsGrayScale()) image->GrayScale();
	image->HuePalette();

/*	POSITION pos = GetFirstViewPosition();
	CKYKYImageView* pView = (CKYKYImageView*)GetNextView(pos);
	HDC srcDC = ::GetDC(pView->GetSafeHwnd());
	HDC memDC = ::CreateCompatibleDC(srcDC);
	// copy the screen to the bitmap
	CSize sz(image->GetWidth(), image->GetHeight());
	int xshift = 0, yshift = 0;
	HBITMAP bm =::CreateCompatibleBitmap(srcDC, sz.cx, sz.cy);
	HBITMAP oldbm = (HBITMAP)::SelectObject(memDC,bm);
	::BitBlt(memDC, 0, 0, sz.cx, sz.cy, srcDC, xshift, yshift, SRCCOPY);
//	image->SetTransIndex(-1);
//	image->Draw(memDC);
	::TextOut(memDC,10,10,_T("test"),4);
	
	CxImage newima;
	newima.CreateFromHBITMAP(bm);
	image->Transfer(newima);
	// free objects
	SelectObject(memDC,oldbm);    
	DeleteObject(memDC);*/

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}

void CImageDoc::OnCximageOpacity() 
{
	// TODO: Add your command handler code here
	if (image==NULL) return;
	CDlgOpacity dlg;
	dlg.m_level=image->AlphaGetMax();
	if (dlg.DoModal()==IDOK){
		SubmitUndo();
		if (!image->AlphaIsValid()){
			image->AlphaCreate();
		}
		image->AlphaSetMax(dlg.m_level);
	}
	UpdateAllViews(NULL);
}

void CImageDoc::OnCximageRemovealphachannel() 
{
	// TODO: Add your command handler code here
	SubmitUndo();
	image->AlphaDelete();
	image->AlphaSetMax(255);
	UpdateAllViews(NULL);
}

void CImageDoc::OnCximageAlphastrip() 
{
	// TODO: Add your command handler code here
	SubmitUndo();
	SetCursor(LoadCursor(0,IDC_WAIT));
	Stopwatch(0);
	
	RGBQUAD c={255,255,255,0};
	image->SetTransColor(c);
	image->AlphaStrip();
	
	Stopwatch(1);
	SetCursor(LoadCursor(0,IDC_ARROW));
	UpdateStatusBar();
	UpdateAllViews(NULL,WM_USER_NEWIMAGE);	
}

void CImageDoc::OnCximageGamma() 
{
	if (image==NULL) return;
	CDlgGamma dlg;
	dlg.m_gamma = theApp.m_Filters.GammaLevel;
	dlg.m_gammaR = theApp.m_Filters.GammaR;
	dlg.m_gammaG = theApp.m_Filters.GammaG;
	dlg.m_gammaB = theApp.m_Filters.GammaB;
	dlg.m_bGammaMode = theApp.m_Filters.GammaLink;
	
	if (dlg.DoModal()==IDOK){
		SubmitUndo();
		m_MenuCommand=ID_CXIMAGE_GAMMA;
		theApp.m_Filters.GammaLevel = dlg.m_gamma;
		theApp.m_Filters.GammaR = dlg.m_gammaR;
		theApp.m_Filters.GammaG = dlg.m_gammaG;
		theApp.m_Filters.GammaB = dlg.m_gammaB;
		theApp.m_Filters.GammaLink = dlg.m_bGammaMode;
		
		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}
}

void CImageDoc::OnCximageInvetalpha() 
{
	// TODO: Add your command handler code here
	SubmitUndo();
	image->AlphaInvert();
	UpdateAllViews(NULL);
}

void CImageDoc::OnCximageAlphapalettetoggle() 
{
	// TODO: Add your command handler code here
	SubmitUndo();
	image->AlphaPaletteEnable(!image->AlphaPaletteIsEnabled());
	UpdateAllViews(NULL);	
}
//////////////////////////////////////////////////////////////////////////////
/*
void CImageDoc::OnCximageMedian() 
{
	m_MenuCommand=ID_CXIMAGE_MEDIAN;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageAddnoise() 
{
	m_MenuCommand=ID_CXIMAGE_ADDNOISE;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}

//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnUpdateViewPalette(CCmdUI* pCmdUI) 
{
	if(image==0 || hThread || image->GetNumColors()==0) pCmdUI->Enable(0);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnViewPalette() 
{
	if (image==NULL) return;
	CDlgPalette dlg;
	dlg.m_numcolors=image->GetNumColors();
	memcpy(dlg.m_pal,image->GetPalette(),dlg.m_numcolors*sizeof(RGBQUAD));
	if (dlg.DoModal()==IDOK){
		if (dlg.m_changed){
			SubmitUndo();
			switch (dlg.m_replace){
			case 1:
				{
					image->SetPalette(dlg.m_pal,dlg.m_numcolors);
					break;
				}
			case 2:
				{
					int bpp=image->GetBpp();
					image->IncreaseBpp(24);
					image->DecreaseBpp(bpp,false,dlg.m_pal);
					break;
				}
			case 3:
				{
					int bpp=image->GetBpp();
					image->IncreaseBpp(24);
					image->DecreaseBpp(bpp,true,dlg.m_pal);
					break;
				}
			}
			
			UpdateAllViews(NULL,WM_USER_NEWIMAGE);
		}
	}
}*/
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageFft() 
{
	if (image==NULL) return;
	CDlgFFT dlg;
	if (dlg.DoModal()==IDOK){
		SetCursor(LoadCursor(0,IDC_WAIT));
		Stopwatch(0);
		
		CxImage *srcr,*srci,*dstr,*dsti,tmp;
		
		srcr = (dlg.pDocReal) ? dlg.pDocReal->GetImage() : 0;
		srci = (dlg.pDocImag) ? dlg.pDocImag->GetImage() : 0;
		
		if (srcr==0 && srci==0) return;
		
		if (srcr) dstr = new CxImage(*srcr,true,false,false); else dstr=0;
		if (srci) dsti = new CxImage(*srci,true,false,false); else dsti=0;
		
		if (dstr==0){
			dstr = new CxImage(dsti->GetWidth(),dsti->GetHeight(),8);
			dstr->Clear(0);
			dstr->SetGrayPalette();
		}
		if (dsti==0){
			dsti = new CxImage(dstr->GetWidth(),dstr->GetHeight(),8);
			dsti->Clear(0);
			dsti->SetGrayPalette();
		}
		
		tmp.FFT2(dstr,dsti,0,0,dlg.bInverse,dlg.bForceFFT!=0,dlg.bMagnitude!=0);
		
		theApp.m_nImageDocCount++;
		CImageDoc *NewDoci=(CImageDoc*)(theApp.m_pSemImageDocTemplate)->OpenDocumentFile(NULL); 
		if (NewDoci)	{
			NewDoci->image = dsti;
			CString s;
			if (dlg.bMagnitude){
				s.Format(_T("FFT Phase %d"),theApp.m_nImageDocCount);
			} else {
				s.Format(_T("FFT Imag %d"),theApp.m_nImageDocCount);
			}
			NewDoci->SetTitle(s);
			NewDoci->UpdateAllViews(0,WM_USER_NEWIMAGE);
		}
		CImageDoc *NewDocr=(CImageDoc*)(theApp.m_pSemImageDocTemplate)->OpenDocumentFile(NULL); 
		if (NewDocr)	{
			NewDocr->image = dstr;
			CString s;
			if (dlg.bMagnitude){
				s.Format(_T("FFT Magnitude %d"),theApp.m_nImageDocCount);
			} else {
				s.Format(_T("FFT Real %d"),theApp.m_nImageDocCount);
			}
			NewDocr->SetTitle(s);
			NewDocr->UpdateAllViews(0,WM_USER_NEWIMAGE);
		}
		
		Stopwatch(1);
		UpdateStatusBar();
		SetCursor(LoadCursor(0,IDC_ARROW));
	}
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageRepair() 
{
	if (image==NULL) return;
	
	CDlgRepair dlg;
	dlg.m_iterations = 2;
	dlg.m_radius = (float)0.25;
	if (dlg.DoModal()==IDOK){
		SubmitUndo();
		SetCursor(LoadCursor(0,IDC_WAIT));
		Stopwatch(0);
		
		image->Repair(dlg.m_radius,dlg.m_iterations,dlg.m_ncs);
		
		Stopwatch(1);
		UpdateAllViews(NULL);
		UpdateStatusBar();
		SetCursor(LoadCursor(0,IDC_ARROW));
	}
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageLessContrast() 
{
	m_MenuCommand=ID_CXIMAGE_LESSCONTRAST;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageJpeg() 
{
	if (image==NULL) return;
	CDlgJpeg dlg;
	dlg.m_quality=50.0f;
	if (dlg.DoModal()==IDOK){
		
		SetCursor(LoadCursor(0,IDC_WAIT));
		Stopwatch(0);
		
		CxImage *tmp;
		tmp = new CxImage(*image);
		if (!tmp->IsGrayScale()) tmp->IncreaseBpp(24);
		tmp->SetTransIndex(-1);
		tmp->SetJpegQualityF(dlg.m_quality);
		
		DWORD imagetype = 0;
#if CXIMAGE_SUPPORT_JPG
		if (dlg.m_format==0) imagetype = CXIMAGE_FORMAT_JPG;
#endif
#if CXIMAGE_SUPPORT_JPC
		if (dlg.m_format==1) imagetype = CXIMAGE_FORMAT_JPC;
#endif
		
		CxMemFile tmpFile;
		tmpFile.Open();
		
		if (tmp->Encode(&tmpFile,imagetype)){
			
			tmpFile.Seek(0,SEEK_SET);
			if (tmp->Decode(&tmpFile,imagetype)){
				
				theApp.m_nImageDocCount++;
				CImageDoc *NewDoc=(CImageDoc*)(theApp.m_pSemImageDocTemplate)->OpenDocumentFile(NULL); 
				if (NewDoc)	{
					NewDoc->image = tmp;
					CString s;
					s.Format(_T("Jpeg compr. %d, q = %.3f, size = %d"),
						theApp.m_nImageDocCount, (double)dlg.m_quality, tmpFile.Size());
					NewDoc->SetTitle(s);
					NewDoc->UpdateAllViews(0,WM_USER_NEWIMAGE);
				}
			} else {
				CString s = tmp->GetLastError();
				AfxMessageBox(s);
				delete tmp;
			}
		} else {
			CString s = tmp->GetLastError();
			AfxMessageBox(s);
			delete tmp;
		}
		
		Stopwatch(1);
		UpdateStatusBar();
		SetCursor(LoadCursor(0,IDC_ARROW));
	}
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::RegionRotateLeft()
{
	long i,n;
	long h=image->GetHeight();
	for(i=0;i<m_NumSel;i++){
		n = m_Sel[i].x;
		m_Sel[i].x = m_Sel[i].y;
		m_Sel[i].y = h-1-n;
	}
	
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::RegionRotateRight()
{
	long i,n;
	long w=image->GetWidth();
	for(i=0;i<m_NumSel;i++){
		n = m_Sel[i].y;
		m_Sel[i].y = m_Sel[i].x;
		m_Sel[i].x = w-1-n;
	}
	
}
/*
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnUpdateCximageFlood(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_tool==5);
	CDlgFloodFill* pDlg = ((CMainFrame *)theApp.GetMainWnd())->m_pDlgFlood;
	if (pDlg && pDlg->GetSafeHwnd() && m_tool!=5){
		pDlg->ShowWindow(SW_HIDE);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageFlood() 
{
	m_tool=5;
	CDlgFloodFill* pDlg = ((CMainFrame *)theApp.GetMainWnd())->m_pDlgFlood;
	if (pDlg){
		if (pDlg->GetSafeHwnd()==0){
			pDlg->m_color = RGB(theApp.m_FloodColor.rgbRed, theApp.m_FloodColor.rgbGreen, theApp.m_FloodColor.rgbBlue);
			pDlg->m_tol = theApp.m_FloodTolerance;
			pDlg->m_opacity = theApp.m_FloodOpacity;
			pDlg->m_select = theApp.m_FloodSelect;
			pDlg->Create();
		} else {
			pDlg->ShowWindow(SW_SHOW);
		}
	}
}*/
//////////////////////////////////////////////////////////////////////////////
void CImageDoc::OnCximageShadow() 
{
	if (image==NULL) return;
	CDlgShadow dlg;
	
	dlg.m_x = theApp.m_Filters.ShadowX;
	dlg.m_y = theApp.m_Filters.ShadowY;
	dlg.m_radius = theApp.m_Filters.ShadowR;
	dlg.m_shadow = theApp.m_Filters.ShadowColor;
	dlg.m_bkg = theApp.m_Filters.ShadowBkg;
	dlg.m_intensity = theApp.m_Filters.ShadowIntensity;
	dlg.m_relative = theApp.m_Filters.ShadowRelative;
	
	if (dlg.DoModal()==IDOK){
		
		theApp.m_Filters.ShadowX = dlg.m_x;
		theApp.m_Filters.ShadowY = dlg.m_y;
		theApp.m_Filters.ShadowR = dlg.m_radius;
		theApp.m_Filters.ShadowColor = dlg.m_shadow;
		theApp.m_Filters.ShadowBkg = dlg.m_bkg;
		theApp.m_Filters.ShadowIntensity = dlg.m_intensity;
		theApp.m_Filters.ShadowRelative = dlg.m_relative;
		
		SetCursor(LoadCursor(0,IDC_WAIT));
		Stopwatch(0);
		
		RGBQUAD c0 = CxImage::RGBtoRGBQUAD(theApp.m_Filters.ShadowColor);
		RGBQUAD c1 = CxImage::RGBtoRGBQUAD(theApp.m_Filters.ShadowBkg);
		
		CxImage *mix = new CxImage(*image);
		mix->IncreaseBpp(24);
		mix->SelectionClear();
		mix->SelectionAddColor(c1);
		CxImage iShadow;
		mix->SelectionSplit(&iShadow);
		mix->SelectionDelete();
		
		if (theApp.m_Filters.ShadowRelative){
			CxImage gray(*image);
			gray.GrayScale();
			iShadow.Mix(gray,CxImage::OpOr);
		}
		
		iShadow.GaussianBlur(theApp.m_Filters.ShadowR);
		
		for (int n = 0; n<256; n++){
			BYTE r = (BYTE)(c1.rgbRed   + ((theApp.m_Filters.ShadowIntensity*n*((long)c0.rgbRed   - (long)c1.rgbRed))>>16));
			BYTE g = (BYTE)(c1.rgbGreen + ((theApp.m_Filters.ShadowIntensity*n*((long)c0.rgbGreen - (long)c1.rgbGreen))>>16));
			BYTE b = (BYTE)(c1.rgbBlue  + ((theApp.m_Filters.ShadowIntensity*n*((long)c0.rgbBlue  - (long)c1.rgbBlue))>>16));
			iShadow.SetPaletteColor((BYTE)(255-n),r,g,b);
		}
		
		mix->SetTransColor(c1);
		mix->SetTransIndex(0);
		mix->Mix(iShadow,CxImage::OpSrcCopy,theApp.m_Filters.ShadowX,theApp.m_Filters.ShadowY);
		//mix->Transfer(iShadow);
		mix->SetTransIndex(-1);
		
		CImageDoc *NewDocr=(CImageDoc*)(theApp.m_pSemImageDocTemplate)->OpenDocumentFile(NULL); 
		if (NewDocr)	{
			NewDocr->image = mix;
			CString s;
			s.Format(_T("NewImage%d"),theApp.m_nImageDocCount++);
			NewDocr->SetTitle(s);
			NewDocr->UpdateAllViews(0,WM_USER_NEWIMAGE);
		}
		
		Stopwatch(1);
		UpdateStatusBar();
		SetCursor(LoadCursor(0,IDC_ARROW));
	}
}
//////////////////////////////////////////////////////////////////////////////

void CImageDoc::OnUpdateSemPhoto(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//	BOOL boolPhotoScan = ((CMainFrame *)theApp.GetMainWnd())->
//			m_MainConsole.m_PageScan.IsPhoto();
	BOOL boolPhotoScan = FALSE;//gpScanView->IsPhoto();

	if (image==0 || hThread)
		pCmdUI->Enable( FALSE );
	else
		pCmdUI->Enable( !boolPhotoScan );	
}

void CImageDoc::OnSemPhoto() 
{
	// TODO: Add your command handler code here
	if (image==NULL) return;

/*	((CMainFrame *)theApp.GetMainWnd())->
			m_MainConsole.m_PageScan.ChangeScan( ST_Stop );
	((CMainFrame *)theApp.GetMainWnd())->
			m_MainConsole.m_PageScan.EnableLabel( FALSE );
*/
//	gpScanView->ChangeScan( ST_Stop );
//	gpScanView->SyncLabel( FALSE );
	///////////////////////////////////////////////////////
	m_MenuCommand=ID_CXIMAGE_PHOTO;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	///////////////////////////////////////////////////////
}

bool CImageDoc::OpenSemImage(const CString& filename, DWORD imagetype)
{
	ASSERT( !filename.IsEmpty() );

	if ( imagetype != SEMIMAGE_FORMAT_SEM )
		return FALSE;

	///////////////////////////////////////////////////////////////////////////////////
	CFile file;
	if( !file.Open(filename, CFile::modeRead) )
		return FALSE;

	///////////////////////////////////////////////////////////////////////////////////
	DWORD dwImageFileSize;										//图像文件的大小
	DWORD dwImageSize;											//图像的大小

	BITMAPFILEHEADER	bmpFileHeader;
	file.Read( &dwImageFileSize, sizeof( DWORD ) );

	file.Read( &bmpFileHeader, sizeof( BITMAPFILEHEADER ) );
	dwImageSize = dwImageFileSize - sizeof(BITMAPFILEHEADER);

	if( dwImageSize > 0 )
	{
		HANDLE hDIB = (HANDLE)GlobalAlloc(GHND, dwImageSize);
		if( hDIB == NULL )
		{
			file.Close();
			return FALSE;
		}
		
		BYTE* pGreyImageBuffer = (BYTE *)::GlobalLock( hDIB );
		file.Read( pGreyImageBuffer, dwImageSize );
		GlobalUnlock( hDIB );

		CxImage *newImage = new CxImage();
		newImage->CreateFromHANDLE( hDIB );
		image = newImage;

		GlobalFree( hDIB );
	}
	///////////////////////////////////////////////////////////////////////////////////

	file.Close();
	return TRUE;
}
/*
BOOL SaveSemCxImage(CFile &file, CxImage *pImage)
{
	///////////////////////////////////////////////////////////////////////////////////
	HANDLE hDIB = (HANDLE)pImage->GetDIB();
	if( hDIB == NULL )
	{
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////////
	DWORD dwImageSize;											//图像的大小
	DWORD dwImageFileSize;										//图像文件的大小
	dwImageSize		= pImage->GetSize();
	dwImageFileSize = dwImageSize + sizeof(BITMAPFILEHEADER);
	file.Write( &dwImageFileSize, sizeof( DWORD ) );
	if( dwImageSize > 0 )
	{
		BITMAPFILEHEADER	bmpFileHeader;
		bmpFileHeader.bfType		= 0x4D42; // BM
		bmpFileHeader.bfSize		= dwImageFileSize;
		bmpFileHeader.bfReserved1	= 0;
		bmpFileHeader.bfReserved2	= 0;
		bmpFileHeader.bfOffBits		=
			sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
		file.Write( &bmpFileHeader, sizeof( BITMAPFILEHEADER ) );

		BYTE* pGreyImageBuffer = (BYTE *)::GlobalLock( hDIB );

		//////////////////////////////////////////////////////
		BITMAPINFOHEADER	*pInfoHeader = (BITMAPINFOHEADER *)pGreyImageBuffer;
		// PixelsPerMeter = DPI * 10000.0 / 254.0;
		pInfoHeader->biXPelsPerMeter = (long)(pImage->GetXDPI() * 10000.0 / 254.0 + 0.5);
		pInfoHeader->biYPelsPerMeter = (long)(pImage->GetYDPI() * 10000.0 / 254.0 + 0.5);
		//////////////////////////////////////////////////////

		file.Write( pGreyImageBuffer, dwImageSize );
	}
	::GlobalUnlock( hDIB );  
	///////////////////////////////////////////////////////////////////////////////////

	return TRUE;
}

BOOL SaveSemParam(CFile &file, int nMag, int nHV)
{
	///////////////////////////////////////////////////////////////////////////////////
	struct ACQUIRECONDITIONSTRUCT
	{
		DWORD	dwDotTime;			// 2,5,10...us
		UINT	uFrames;			// default = 0
		UINT	uLinescanPoints;
		UINT	uResolution;		// 分辨率	1024
		UINT	uStepX;				// 
		UINT	uStepY;				// 
		UINT	ux0;
		UINT	uy0;
		UINT	ux1;
		UINT	uy1;
		UINT	uXPoint;
		UINT	uYPoint;
		UINT	uVolt;				//加速电压	KV	*
		UINT	uZoom;				//放大倍数	X	*
	};

	ACQUIRECONDITIONSTRUCT	AcquireCondition;
	ZeroMemory( &AcquireCondition, sizeof(ACQUIRECONDITIONSTRUCT) );
	AcquireCondition.uResolution	= 1024;
	AcquireCondition.uVolt			= nHV;
	AcquireCondition.uZoom			= nMag;
	BOOL	boolGrey				= TRUE;
	BYTE	byteMapLine				= 0;

	file.Write( &boolGrey, sizeof( BOOL ) );
	file.Write( &byteMapLine, sizeof( BYTE ) );
	file.Write( &AcquireCondition, sizeof( ACQUIRECONDITIONSTRUCT ) );	// 保存采集条件
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	int nNoteLen = 0;										// 保存注释字符串的长度
	file.Write( &nNoteLen, sizeof( int ) );
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	struct SETINFOSTRUCT
	{
		int		nType;				//设为0
		BOOL	bGreyImage;			//灰度图像为1，二值图像为0
		int		nItemNum;			//用于面分布和线扫描图像，灰度图像设为0
		int		nLineNum;			//测量线数目
		int		nTextNum;			//文本标记数目
		int		nReserved;			//预留，现设为0
		DWORD	wReserved;			//预留，现设为0
	};

	SETINFOSTRUCT	sisTemp;
	ZeroMemory( &sisTemp, sizeof(SETINFOSTRUCT) );
	sisTemp.bGreyImage	= TRUE;		//灰度图像为1，二值图像为0

	file.Write( &sisTemp, sizeof( SETINFOSTRUCT ) );			// 保存设置信息结构
	///////////////////////////////////////////////////////////////////////////////////

	return TRUE;
}
*/
bool CImageDoc::SaveSemImage(const CString& filename, DWORD imagetype)
{
	ASSERT( !filename.IsEmpty() );

	if ( imagetype != SEMIMAGE_FORMAT_SEM )
		return FALSE;

	//======2006.09.19_图像保存========================
	// 修改标示：2006.09.19_图像保存
	// 修改日期：2006.09.19星期二
	// 修改人  ：J.Y.Ma
	// 修改原因：存SEM格式时不正确，且存储分类不细

	// 本次修改前代码副本
	/*------------------------------------------------- 
	if( !IsSemImage() )	//06.09.12
		return FALSE;

	CFile file;
	///////////////////////////////////////////////////////////////////////////////////
	if( !file.Open( filename, CFile::modeCreate|CFile::modeReadWrite) )
		return FALSE;

	CxImage* pSaveImg = ImageLayer;

	if( theApp.m_blSaveGraphLay )
	{
		// 保存图形层
		OnCximageGrayscale();
		while( hThread != 0 )
		{
			Sleep(0);
		}
		pSaveImg = image;
	}
	-------------------------------------------------*/

	// 本次修改后代码
	//-------------------------------------------------
	CFile file;
	///////////////////////////////////////////////////////////////////////////////////
	if( !file.Open( filename, CFile::modeCreate|CFile::modeReadWrite) )
		return FALSE;

	CxImage* pSaveImg = ImageLayer;

	if( IsSemImage() && !theApp.m_blSaveGraphLay )
	{
		// 是新图像，且不保存图形层
		pSaveImg->GrayScale();
		Sleep(10 );
	}
	else
	{
		// 是新图像，且保存图形层
		// 否新图像，是/否保存图形层（所有标准图像另存为sem格式时）
		OnCximageGrayscale();
		while( hThread != 0 )
		{
			Sleep(0);
		}
		pSaveImg = image;
	}
	//======2006.09.19_图像保存========================

	/////////////////////////////////////////////////////
	// PixelsPerMeter = DPI * 10000.0 / 254.0;
	// DPI = PixelsPerMeter * 254.0 / 10000.0;
	// --------------------------------------------------
	// PixelsPerMeter = Pixels / PhotoSize (m)
	// PixelsPerMeter = Pixels * 1000 / PhotoSize (mm)
	// --------------------------------------------------
	// PhotoWidth  = 120 mm		<==>	ScreenWidth  = 1024 Pixel
	// PhotoHeight =  90 mm		<==>	ScreenHeight =  768 Pixel
	// --------------------------------------------------
	// DPI = Pixels * 25.4 / PhotoSize (mm);
	// --------------------------------------------------
	// DPI_X = 25.4 * 1024 / 127;	// 5寸照片尺寸为12.7cm x 8.9cm
	// DPI_Y = 25.4 *  768 /  90;
	const double dblDPI = 25.4 * pSaveImg->GetWidth() / 127;
//	const double dblDPI_Y = 25.4 *  768 /  90;
	/////////////////////////////////////////////////////

	pSaveImg->SetXDPI((long)dblDPI);
	pSaveImg->SetYDPI((long)dblDPI);

	if( !SaveSemCxImage(file, pSaveImg) )
	{
		file.Close();
		return FALSE;
	}

	SaveSemParam(file, m_nMag, m_nHV);

	file.Close();
	///////////////////////////////////////////////////////////////////////////////////

	//======2006.09.19_图像保存========================
	// 修改标示：2006.09.19_图像保存
	// 修改日期：2006.09.19星期二
	// 修改人  ：J.Y.Ma
	// 修改原因：存SEM格式时不正确，且存储分类不细

	// 本次修改前代码副本
	/*------------------------------------------------- 
	-------------------------------------------------*/

	// 本次修改后代码
	//-------------------------------------------------
	if( image != pSaveImg )
		image->Copy( *pSaveImg );
	//======2006.09.19_图像保存========================

	SaveParams( filename );
	return TRUE;
}
/*
bool CImageDoc::SaveSemImage(const CString& filename, DWORD imagetype)
{
	ASSERT( !filename.IsEmpty() );

	if ( imagetype != SEMIMAGE_FORMAT_SEM )
		return FALSE;
	///////////////////////////////////////////////////////////////////////////////////
	CFile file;
	if( !file.Open( filename, CFile::modeCreate|CFile::modeReadWrite) )
		return FALSE;

	if( !IsSemImage() )
	{
		file.Close();
		return FALSE;
	}

	HANDLE hDIB = (HANDLE)ImageLayer->GetDIB();
	if( hDIB == NULL )
	{
		file.Close();
		return FALSE;
	}
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	DWORD dwImageSize;											//图像的大小
	DWORD dwImageFileSize;										//图像文件的大小
	dwImageSize		= ImageLayer->GetSize();
	dwImageFileSize = dwImageSize + sizeof(BITMAPFILEHEADER);
	file.Write( &dwImageFileSize, sizeof( DWORD ) );
	if( dwImageSize > 0 )
	{
		BITMAPFILEHEADER	bmpFileHeader;
		bmpFileHeader.bfType		= 0x4D42; // BM
		bmpFileHeader.bfSize		= dwImageFileSize;
		bmpFileHeader.bfReserved1	= 0;
		bmpFileHeader.bfReserved2	= 0;
		bmpFileHeader.bfOffBits		=
			sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
		file.Write( &bmpFileHeader, sizeof( BITMAPFILEHEADER ) );

		BYTE* pGreyImageBuffer = (BYTE *)::GlobalLock( hDIB );

		//////////////////////////////////////////////////////
		BITMAPINFOHEADER	*pInfoHeader = (BITMAPINFOHEADER *)pGreyImageBuffer;
		// PixelsPerMeter = DPI * 10000.0 / 254.0;
		pInfoHeader->biXPelsPerMeter = (long)(ImageLayer->GetXDPI() * 10000.0 / 254.0 + 0.5);
		pInfoHeader->biYPelsPerMeter = (long)(ImageLayer->GetYDPI() * 10000.0 / 254.0 + 0.5);
		//////////////////////////////////////////////////////

		file.Write( pGreyImageBuffer, dwImageSize );
	}
	::GlobalUnlock( hDIB );  
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	struct ACQUIRECONDITIONSTRUCT
	{
		DWORD	dwDotTime;			// 2,5,10...us
		UINT	uFrames;			// default = 0
		UINT	uLinescanPoints;
		UINT	uResolution;		// 分辨率	1024
		UINT	uStepX;				// 
		UINT	uStepY;				// 
		UINT	ux0;
		UINT	uy0;
		UINT	ux1;
		UINT	uy1;
		UINT	uXPoint;
		UINT	uYPoint;
		UINT	uVolt;				//加速电压	KV	*
		UINT	uZoom;				//放大倍数	X	*
	};

	ACQUIRECONDITIONSTRUCT	AcquireCondition;
	ZeroMemory( &AcquireCondition, sizeof(ACQUIRECONDITIONSTRUCT) );
	AcquireCondition.uResolution	= 1024;
	AcquireCondition.uVolt			= m_nHV;
	AcquireCondition.uZoom			= m_nMag;
	BOOL	boolGrey				= TRUE;
	BYTE	byteMapLine				= 0;

	file.Write( &boolGrey, sizeof( BOOL ) );
	file.Write( &byteMapLine, sizeof( BYTE ) );
	file.Write( &AcquireCondition, sizeof( ACQUIRECONDITIONSTRUCT ) );	// 保存采集条件
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	int nNoteLen = 0;										// 保存注释字符串的长度
	file.Write( &nNoteLen, sizeof( int ) );
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	struct SETINFOSTRUCT
	{
		int		nType;				//设为0
		BOOL	bGreyImage;			//灰度图像为1，二值图像为0
		int		nItemNum;			//用于面分布和线扫描图像，灰度图像设为0
		int		nLineNum;			//测量线数目
		int		nTextNum;			//文本标记数目
		int		nReserved;			//预留，现设为0
		DWORD	wReserved;			//预留，现设为0
	};

	SETINFOSTRUCT	sisTemp;
	ZeroMemory( &sisTemp, sizeof(SETINFOSTRUCT) );
	sisTemp.bGreyImage	= TRUE;		//灰度图像为1，二值图像为0

	file.Write( &sisTemp, sizeof( SETINFOSTRUCT ) );			// 保存设置信息结构
	///////////////////////////////////////////////////////////////////////////////////

	file.Close();
	return TRUE;
}
*/
void CImageDoc::SaveParams( LPCTSTR lpszPathName )
{
	FILE* stream = NULL;
	if( (stream = fopen(lpszPathName, "ab+")) == NULL )
		return;

	// 写参数
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
		fprintf(stream, "%s; Speed=%dus/s, Sync=%d, Signal=%s, Gain=%d/%d, Filter=%d; HV=%dV, Bias=%dV, Fil=%dmA, EMI=%duA; B=%s; C=%s; CL=%s; OL=%s; Mag=%s; WD=%s",
				m_ac.strTime, m_ac.nSpeed, m_ac.nIsSync, m_ac.strSignal,
				m_ac.nGain, m_ac.nGain2, m_ac.nFilter,
				m_ac.nHV, m_ac.nHV2, m_ac.nFil, m_ac.nEMI,
				m_ac.strB, m_ac.strC, m_ac.strCL, m_ac.strOL, m_ac.strMag, m_ac.strWD );
	else
		fprintf(stream, "%s; Speed=%dus/s, Sync=%d, Signal=%s, Gain=%d/%d, Filter=%d; HV=%dV, Beam=%duA, Fil=%dmA, EMI=%duA; B=%s; C=%s; CL=%s; OL=%s; Mag=%s; WD=%s",
				m_ac.strTime, m_ac.nSpeed, m_ac.nIsSync, m_ac.strSignal,
				m_ac.nGain, m_ac.nGain2, m_ac.nFilter,
				m_ac.nHV, m_ac.nHV2, m_ac.nFil, m_ac.nEMI,
				m_ac.strB, m_ac.strC, m_ac.strCL, m_ac.strOL, m_ac.strMag, m_ac.strWD );
	// 写参数结束
	fclose(stream);

	stream = NULL;
}

void CImageDoc::OpenParams( LPCTSTR lpszPathName )
{
	FILE* stream = NULL;
	if( (stream = fopen(lpszPathName, "rb+")) == NULL )
		return;
	fseek( stream, -254, SEEK_END );

	int i, nStart = 0;
	char szBuf[256] = {0};
	fgets( szBuf, 255, stream );	// 因Info信息长度不固定，所以取一个大约数255
	for( i=0; i<256; i++ )
	{
		if( szBuf[i] == 0x3B )		// 找到第一个';'
		{
			// 如果关键字符均对得上，那么可以认为已读到Info最开始的日期部分 XXXX/XX/XX XX:XX:XX;
			if( szBuf[i-3] == 0x3A && szBuf[i-6] == 0x3A && szBuf[i-9] == 0x20 && szBuf[i-12] == 0x2F && szBuf[i-15] == 0x2F )
			{
				nStart = i -19;
				break;
			}
		}
	}
	if( i > 254 )
	{
		fclose(stream);
		stream = NULL;
		return;
	}
	char szBuf1[256], szBuf2[256];
	strcpy( szBuf1, szBuf +nStart );			// 得到所有Info
	// Time
	strcpy(szBuf2, szBuf1);
	if(strchr(szBuf2, ';') != NULL)
	{
		strcpy(szBuf1, strchr(szBuf2, ';')+1);	// szBuf1是剩余字符串
		*strchr(szBuf2, ';') = '\0';
		m_ac.strTime.Format( "%s", szBuf2 );
	}
	else
	{
		fclose(stream);
		stream = NULL;
		return;
	}
	// Speed
	if( OpenParams_FindKey( szBuf1, ',', 'u', szBuf2 ) )
		m_ac.nSpeed = atoi( szBuf2 );
	else
	{
		fclose(stream);
		stream = NULL;
		return;
	}
	// Sync
	if( OpenParams_FindKey( szBuf1, ',', ',', szBuf2 ) )
		m_ac.nIsSync = atoi( szBuf2 );
	else
	{
		fclose(stream);
		stream = NULL;
		return;
	}
	// Signal
	if( OpenParams_FindKey( szBuf1, ',', ',', szBuf2 ) )
		m_ac.strSignal.Format( "%s", szBuf2 );
	else
	{
		fclose(stream);
		stream = NULL;
		return;
	}
	// Gain
	if( OpenParams_FindKey( szBuf1, ',', ',', szBuf2 ) )
	{
		// szBuf2是X/X
		char szGain[10];
		strcpy( szGain, strchr(szBuf2, '//') +1 );	// szGain是Gain2
		*strchr( szBuf2, '//' ) = '\0';
		m_ac.nGain = atoi( szBuf2 );
		m_ac.nGain2 = atoi( szGain );
	}
	else
	{
		fclose(stream);
		stream = NULL;
		return;
	}
	// Filter
	if( OpenParams_FindKey( szBuf1, ';', ';', szBuf2 ) )
		m_ac.nFilter = atoi( szBuf2 );
	else
	{
		fclose(stream);
		stream = NULL;
		return;
	}
	// HV
	if( OpenParams_FindKey( szBuf1, ',', 'V', szBuf2 ) )
		m_ac.nHV = atoi( szBuf2 );
	else
	{
		fclose(stream);
		stream = NULL;
		return;
	}
	// Beam
	if( OpenParams_FindKey( szBuf1, ',', 'u', szBuf2 ) )
		m_ac.nHV2 = atoi( szBuf2 );
	else
	{
		fclose(stream);
		stream = NULL;
		return;
	}
	// Fil
	if( OpenParams_FindKey( szBuf1, ',', 'm', szBuf2 ) )
		m_ac.nFil = atoi( szBuf2 );
	else
	{
		fclose(stream);
		stream = NULL;
		return;
	}
	// EMI
	if( OpenParams_FindKey( szBuf1, ';', 'u', szBuf2 ) )
		m_ac.nEMI = atoi( szBuf2 );
	else
	{
		fclose(stream);
		stream = NULL;
		return;
	}
	// B
	if( OpenParams_FindKey( szBuf1, ';', '[', szBuf2 ) )
		m_ac.strB.Format( "%s", szBuf2 );
	else
	{
		fclose(stream);
		stream = NULL;
		return;
	}
	// C
	if( OpenParams_FindKey( szBuf1, ';', '[', szBuf2 ) )
		m_ac.strC.Format( "%s", szBuf2 );
	else
	{
		fclose(stream);
		stream = NULL;
		return;
	}
	// CL
	if( OpenParams_FindKey( szBuf1, ';', '[', szBuf2 ) )
		m_ac.strCL.Format( "%s", szBuf2 );
	else
	{
		fclose(stream);
		stream = NULL;
		return;
	}
	// OL
	if( OpenParams_FindKey2( szBuf1, ';', '[', szBuf2 ) )
		m_ac.strOL.Format( "%s", szBuf2 );
	else
	{
		fclose(stream);
		stream = NULL;
		return;
	}
	// Mag
	if( OpenParams_FindKey( szBuf1, ';', ';', szBuf2 ) )
		m_ac.strMag.Format( "%s", szBuf2 );
	else
	{
		fclose(stream);
		stream = NULL;
		return;
	}
	// WD
	m_ac.strWD.Format( "%s", szBuf1 );

	fclose(stream);
	stream = NULL;
}

BOOL CImageDoc::OpenParams_FindKey( char* infoSrc, char Separator, char Unit, char* infoKey )
{
	char szBuf1[255] = {0};
	char szBuf2[255] = {0};
	strcpy( szBuf1, infoSrc );
	strcpy( szBuf2, szBuf1 );
	if( strchr(szBuf2, Separator) == NULL )
		return FALSE;

	strcpy( szBuf1, strchr(szBuf2, Separator) +1 );	// szBuf1是剩余字符串
	strcpy( infoSrc, szBuf1 );

	*strchr( szBuf2, Separator ) = '\0';			// szBuf2是XXX=xx.xxUnit
	strcpy( szBuf1, strchr(szBuf2, '=') +1 );		// szBuf1是xx.xxUnit
	if( Separator != Unit )							// 有unit
	{
		if( strchr( szBuf1, Unit) != NULL )
		{
			strcpy( szBuf2, strchr(szBuf1, Unit) );	// szBuf2是Unit
			*strchr( szBuf1, Unit ) = '\0';
		}
	}
	strcpy( infoKey, szBuf1 );						// 找到关键字符串
	return TRUE;
}

BOOL CImageDoc::OpenParams_FindKey2( char* infoSrc, char Separator, char Unit, char* infoKey )
{
	// 只针对OL的调试模式：OL=[ XXXX ] XXX.XX + [XX]X.XXXX = XXX.XX
	char szBuf1[255] = {0};
	char szBuf2[255] = {0};
	strcpy( szBuf1, infoSrc );
	strcpy( szBuf2, szBuf1 );
	if( strchr(szBuf2, Separator) == NULL )
		return FALSE;

	strcpy( szBuf1, strchr(szBuf2, Separator) +1 );	// szBuf1是剩余字符串
	strcpy( infoSrc, szBuf1 );

	*strchr( szBuf2, Separator ) = '\0';			// szBuf2是OL字符串
	strcpy( szBuf1, strrchr(szBuf2, '=') +1 );		// szBuf1是最后的XXX.XX
	strcpy( infoKey, szBuf1 );						// 找到关键字符串
	return TRUE;
}

void CImageDoc::_SaveImageData2File(const CString &strFolder,  const CString& strFileName, BOOL bShowHeader )
{
	CString str;
	str.Format( "%s\\%s.csv", strFolder, strFileName );
	HANDLE hFile = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);

	if (bShowHeader)
	{
		//列标题
		CString sCol = "---,";
		for (int i = 0; i < ImageData_ColumnCount;i++)
		{
			CString sNum = "";
			sNum.Format("Col%d",i);
			if (ImageData_ColumnCount - 1 == i)
			{
				sCol = sCol + sNum;
			}
			else
			{
				sCol = sCol + sNum + ",";
			}
		}

		sCol = sCol + "\r\n";
		unsigned long cbRet;
		WriteFile( hFile, sCol.GetBuffer(), sCol.GetLength(), &cbRet, NULL );

		//逐行数据
		for (int i = 0; i < ImageData_RowCount;i++)
		{
			CString sColumnData = "";
			sColumnData.Format("Row%d,",i);
			for (int j = 0;j < ImageData_ColumnCount;j++)
			{
				DWORD dwData = m_ImageColor[i][j];
				CString sData;
				sData.Format("%d",dwData);
				if (ImageData_ColumnCount - 1 == i)
				{
					sColumnData = sColumnData + sData;
				}
				else
				{
					sColumnData = sColumnData + sData + ",";
				}
			}

			sColumnData = sColumnData + "\r\n";	
			WriteFile( hFile, sColumnData.GetBuffer(), sColumnData.GetLength(), &cbRet, NULL );
		}
	}
	else
	{
		//逐行数据
		for (int i = 0; i < ImageData_RowCount;i++)
		{
			CString sColumnData = "";
			for (int j = 0;j < ImageData_ColumnCount;j++)
			{
				DWORD dwData = m_ImageColor[i][j];
				CString sData;
				sData.Format("%d",dwData);
				if (ImageData_ColumnCount - 1 == i)
				{
					sColumnData = sColumnData + sData;
				}
				else
				{
					sColumnData = sColumnData + sData + ",";
				}
			}

			sColumnData = sColumnData + "\r\n";	
			unsigned long cbRet;
			WriteFile( hFile, sColumnData.GetBuffer(), sColumnData.GetLength(), &cbRet, NULL );
		}
	}

	CloseHandle(hFile);
}

#include "DlgInput.h"

void CImageDoc::OnToolsResoFactor() 
{
	if (image==NULL) return;
	DWORD dwType = image->GetType();
	// 	if( dwType != CXIMAGE_FORMAT_JPG )
	// 		return;

	long lx = image->GetXDPI();
	long ly = image->GetYDPI();
	DWORD dwWidth = image->GetWidth();
	DWORD dwHeight = image->GetHeight();
	double dMag = 0.0;
	if( m_ac.nHV == 0 )	// 无采集信息
	{
		CDlgInput dlg;
		dlg.m_nTitleIndex = IDS_SD_PROMPT_Input_Mag;
		if( dlg.DoModal() == IDOK )
			dMag = atof( dlg.m_strInput );
	}
	else
	{
		char buf[20], unit[10];
		sprintf( buf, "%s", m_ac.strMag );
		if( strchr( buf, ' ' ) != NULL )
		{
			strcpy( unit, strchr(buf, ' ') +1 );
			*strchr( buf, ' ') = '\0';
			dMag = atof( buf );
			if( strchr( unit, 'K') != NULL )
				dMag *= 1000;
		}
	}
	if( dMag > 0.0 )
	{
		// 图像打印尺寸 = （水平尺寸/水平分辨率） *25.4，单位mm
		double dPrintSize = 25.4 *dwWidth /lx ;
		// 分辨率图片水平方向的实际扫描场大小 = 图像打印尺寸/放大倍数
		double dAreaX = dPrintSize / dMag;
		CDlgInput dlg;
		dlg.m_nTitleIndex = IDS_SD_PROMPT_ResoFactor;
		dlg.m_strInput.Format( "%f nm", dAreaX *1000 );
		dlg.DoModal();
	}
}

void CImageDoc::OnUpdateToolsResoFactor(CCmdUI* pCmdUI) 
{
}
