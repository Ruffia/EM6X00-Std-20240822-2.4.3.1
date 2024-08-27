// ThumbnailListCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
#include "ThumbnailListCtrlEx.h"
#include "ScanView.h"
#include "GlobalParamSetting.h"
#include "ControlLayer.h"
#include "HVPowerDataManager.h"
#include "ScanParameterManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern double	g_dHV, g_dHV2, g_dHV3;	// 分别记录高压、偏压（771束流设置值）、灯丝加热电流的设置值

/////////////////////////////////////////////////////////////////////////////
// CThumbnailListCtrlEx
IMPLEMENT_DYNAMIC( CThumbImageItem,		CObject );

CThumbImageItem::CThumbImageItem()
{ 
	m_pImageSrc		= NULL;//new CxImage();
	m_pImageThumb	= NULL;//new CxImage();
	m_pImageLayer	= NULL;

	// 2021.06 原始数据存储
	m_pPixZsrc		= NULL;
	m_pPixZsrc2		= NULL;

	New();
}

CThumbImageItem::CThumbImageItem( CxImage* pImage )
{ 
	m_pImageSrc		= pImage;

	New();
}

CThumbImageItem::~CThumbImageItem()
{
	if( m_pImageSrc )
	{
		if (m_pImageSrc) m_pImageSrc->SetEscape(1);
		delete m_pImageSrc;
		m_pImageSrc = NULL;
	}
	if( m_pImageThumb )
	{
		if (m_pImageThumb) m_pImageThumb->SetEscape(1);
		delete m_pImageThumb;
		m_pImageThumb = NULL;
	}
	if( m_pImageLayer )
	{
		if (m_pImageLayer) m_pImageLayer->SetEscape(1);
		delete m_pImageLayer;
		m_pImageLayer = NULL;
	}

	// 2021.06 原始数据存储
	if( m_pPixZsrc != NULL )
	{
		delete [] m_pPixZsrc;
		m_pPixZsrc = NULL;
	}
	if( m_pPixZsrc2 != NULL )
	{
		delete [] m_pPixZsrc2;
		m_pPixZsrc2 = NULL;
	}
}

void CThumbImageItem::New()
{
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString strTemp = timeCurrent.Format("%Y%m%d%H%M%S");
	CString csTitle;
	csTitle.Format( "SemImg%s",strTemp );

	m_strTitle		= csTitle;
	m_strTempName	= csTitle;
	m_strPathName	= _T("");

	m_bSel			= FALSE;

	m_nMag			= 150;
	m_nHV			= 20;

	m_ac.strTime	= _T("");

	m_ac.nSpeed		= 64;
	m_ac.nIsSync	= 1;

	m_ac.nHV		= 30000;
	m_ac.nHV2		= 0;
	m_ac.nFil		= 248;
	m_ac.nEMI		= 0;

	m_ac.strB		= "0";
	m_ac.strC		= "0";
	m_ac.strCL		= "420";
	m_ac.strOL		= "480";
	m_ac.strMag		= "150 X";
	m_ac.strWD		= "12 mm";
	m_ac.strSignal	= "SE";
	m_ac.nGain		= 5;
	m_ac.nGain2		= 3;
	m_ac.nFilter	= 3;
}

CxImage* CThumbImageItem::GetImage()
{
	return m_pImageSrc;
}

CString CThumbImageItem::GetDefaultTitle()
{
	return m_strTitle;
}

// 获取任意路径名的标题
CString CThumbImageItem::GetTitleFromPathName( CString strPathName )
{
	CString strTitle = strPathName;
	int nPos = strTitle.Find( '\\' );
	while( nPos != -1 )
	{
		strTitle = strTitle.Right( strTitle.GetLength() - nPos - 1 );
		nPos = strTitle.Find( '\\' );
	}
	return strTitle;
}

BOOL CThumbImageItem::IsSemImage()
{
	return (m_pImageLayer != NULL);
}

BOOL CThumbImageItem::SaveImage( CString strPathName, int nType, CString& strError )
{
	if (m_pImageSrc == NULL || !m_pImageSrc->IsValid())
		return FALSE;

	BOOL bRet = FALSE;

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
	// DPI_Y = 25.4 *  768 /  89;
	const double dblDPI = 25.4 * m_pImageSrc->GetWidth() / 127;
	/////////////////////////////////////////////////////

	m_pImageSrc->SetXDPI((long)dblDPI);
	m_pImageSrc->SetYDPI((long)dblDPI);

	if (nType < SEMIMAGE_FORMAT_SEM)
	{
		if( nType == CXIMAGE_FORMAT_PGX )			// only gray scale images
			m_pImageSrc->GrayScale();
		else if( nType == CXIMAGE_FORMAT_GIF		// better 8 bits or less
				|| nType == CXIMAGE_FORMAT_SKA )	// must be 8 bits or less
			m_pImageSrc->DecreaseBpp(8,0,0);

		bRet = m_pImageSrc->Save(strPathName, nType);
		if( !bRet )
			strError = m_pImageSrc->GetLastError();
		else
		{
//			if( nType == CXIMAGE_FORMAT_TIF )
				SaveParams(strPathName);
		}
	}
	else
		bRet = SaveSemImage(strPathName);

	// 保存成功后的刷新工作
	if( bRet )
	{
		// 更新文件路径名
		m_strPathName	= strPathName;

		// 重新获取标题名
		m_strTitle		= GetTitleFromPathName( strPathName );

		// 删除临时文件，并重设临时文件名
		DeleteTempFile();
		m_strTempName = _T("");

		// 如果保存为标准图像，就将ImageLayer重新设置
		CString ext(theApp.FindExtension(strPathName));
		ext.MakeLower();
		if( ext != "sem" )
		{
			if (m_pImageLayer != NULL)
			{
				m_pImageLayer->SetEscape(1);
				delete m_pImageLayer;
				m_pImageLayer = NULL;
			}

			// 10.07.02 由于CxImage在Save中未能自动更新type，且没有SetType这样的函数进行操作
			// 导致文件正确保存或另存为为标准格式后，其type却不正确
			// 故手动生成新的CxImage，然后使用Copy
//			int nDocTypeCur = m_pImageSrc->GetType();	// 可以监测到此时nDocTypeCur值为保存前的图像类型值
			CxImage* imagenew = new CxImage(strPathName, nType);
			m_pImageSrc->Copy( *imagenew );
			delete imagenew;
		}
	}

	return bRet;
}

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

void CThumbImageItem::SaveParams( CString strPathName )
{
	FILE* stream;
	if( (stream = fopen(strPathName, "ab+")) == NULL )
		return;

	CTime	timeCurrent = CTime::GetCurrentTime();
	m_ac.strTime = timeCurrent.Format("%Y/%m/%d %H:%M:%S");

	CScanView* pView = (CScanView*)theApp.GetScanView();
	if( pView != NULL )
	{
		if( pView->m_bStopPhoto )
		{
			m_ac.nSpeed = CScanParameterManager::Instance().m_nusbPhotoPixRate /2;
			m_ac.nIsSync	= 1;
		}
		else
		{
			m_ac.nSpeed = CScanParameterManager::Instance().m_nusbPixRate;
			m_ac.nIsSync	= 0;
		}
		if( pView->m_bSplit )
			m_ac.strSignal	= CControlLayer::Instance().m_BrdMCI.strChnName[0] +"," +CControlLayer::Instance().m_BrdMCI.strChnName[1];
		else if( pView->m_bOnlyDet2 )
			m_ac.strSignal	= CControlLayer::Instance().m_BrdMCI.strChnName[1];
		else
			m_ac.strSignal	= CControlLayer::Instance().m_BrdMCI.strChnName[0];
	}
	if( CHVPowerDataManager::Instance().m_dwHVtype < 1 )
	{
		m_ac.nHV	= (UINT)(CGlobalParamSetting::Instance().g_dHV *1000);
		m_ac.nHV2	= (UINT)CGlobalParamSetting::Instance().g_dHV2;
		m_ac.nFil	= (UINT)(CGlobalParamSetting::Instance().g_dHV3 *1000);
		m_ac.nEMI	= (UINT)(CHVPowerDataManager::Instance().m_CPS3603.dIoutA * 1e6);
		m_ac.strC	= CSemCoreAssemble::Instance().m_Contrast.FormatText();
	}
	else
	{
		if( CHVPowerDataManager::Instance().m_dwHVtype > 2 )
		{
			m_ac.nHV	= theApp.m_setWSM30KV2.fACLv *1000;
			m_ac.nHV2	= theApp.m_setWSM30KV2.fACLi;
			m_ac.nFil	= theApp.m_setWSM30KV2.fFILi *1000;
			m_ac.nEMI	= theApp.m_monWSM30KV2.monFB.fACLi;
		}
		else if( CHVPowerDataManager::Instance().m_dwHVtype > 1 )
		{
			m_ac.nHV	= theApp.m_setWSM30KV.fACLv *1000;
			m_ac.nHV2	= theApp.m_setWSM30KV.fACLi;
			m_ac.nFil	= theApp.m_setWSM30KV.fFILi *1000;
			m_ac.nEMI	= theApp.m_monWSM30KV.monFB.fACLi;
		}
		else
		{
			m_ac.nHV	= (UINT)(CHVPowerDataManager::Instance().m_Spellmans.pS.dVE *1000);
			m_ac.nHV2	= (UINT)CHVPowerDataManager::Instance().m_Spellmans.pS.dIB;
			m_ac.nFil	= (UINT)(CHVPowerDataManager::Instance().m_Spellmans.pS.dIH *1000);
			m_ac.nEMI	= (UINT)CHVPowerDataManager::Instance().m_Spellmans.pM.dII;
			m_ac.strC	= CSemCoreAssemble::Instance().m_SP_PMT.FormatText();
		}
	}
	m_ac.strB		= CSemCoreAssemble::Instance().m_Brightness.FormatText();
	m_ac.strCL		= CSemCoreAssemble::Instance().m_CondenserLens.FormatText();
	m_ac.strOL		= CSemCoreAssemble::Instance().m_ObjectiveLens.FormatText();
	m_ac.strMag		= CSemCoreAssemble::Instance().m_Magnifier.FormatText();
	m_ac.strWD		= CSemCoreAssemble::Instance().m_WD.FormatText();
	m_ac.nGain		= CControlLayer::Instance().m_BrdMCI.nGain[CControlLayer::Instance().m_BrdMCI.nDetPair];
	m_ac.nGain2		= CControlLayer::Instance().m_BrdMCI.nGain[CControlLayer::Instance().m_BrdMCI.nDetPair +1];
	m_ac.nFilter	= CControlLayer::Instance().m_BrdMCI.nFilter[CControlLayer::Instance().m_BrdMCI.nDetPair];

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
	fclose( stream );
	stream = NULL;
}

BOOL CThumbImageItem::SaveSemImage( CString strPathName )
{
	CFile file;
	///////////////////////////////////////////////////////////////////////////////////
	if( !file.Open( strPathName, CFile::modeCreate|CFile::modeReadWrite) )
		return FALSE;

	CxImage* pSaveImg = m_pImageLayer;

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
		m_pImageSrc->GrayScale();
		Sleep(10);
		pSaveImg = m_pImageSrc;
	}
	//======2006.09.19_图像保存========================

	if( !SaveSemCxImage(file, pSaveImg) )
	{
		file.Close();
		return FALSE;
	}

	SaveSemParam(file, m_nMag, m_nHV);

	file.Close();
	///////////////////////////////////////////////////////////////////////////////////

	if( m_pImageSrc != pSaveImg )
		m_pImageSrc->Copy( *pSaveImg );

	SaveParams( strPathName );
	return TRUE;
}

// 10.02.08 保存临时文件
BOOL CThumbImageItem::SaveTempFile()
{
	char path[255];
	::GetModuleFileName( NULL, path, 255);
	char *ptemp = strrchr( path, '\\');
	ptemp[1] = 0;
	m_strTempName.Format( "%s%s.tmpimg", path, m_strTitle );

	const double dblDPI = 25.4 * m_pImageSrc->GetWidth() / 127;
//	const double dblDPI_Y = 25.4 *  768 /  90;
	m_pImageSrc->SetXDPI((long)dblDPI);
	m_pImageSrc->SetYDPI((long)dblDPI);
	bool retval = m_pImageSrc->Save(m_strTempName, CXIMAGE_FORMAT_TIF);
	if( retval )
		SaveParams(m_strTempName);

	return retval;
}

// 10.02.08 删除临时文件
BOOL CThumbImageItem::DeleteTempFile( CString strTitle )
{
	if( strTitle.IsEmpty() )
		return FALSE;

	char path[255];
	::GetModuleFileName( NULL, path, 255);
	char *ptemp = strrchr( path, '\\');
	ptemp[1] = 0;
	CString newName;
	newName.Format( "%s%s.tmpimg", path, strTitle );

	CFile file;
	if( file.Open( newName, CFile::modeCreate|CFile::modeReadWrite) )
	{
		file.Close();
		remove( newName );
	}

	return TRUE;
}

BOOL CThumbImageItem::DeleteTempFile()
{
	if( m_strTempName.IsEmpty() )
		return FALSE;

	CFile file;
	if( file.Open( m_strTempName, CFile::modeCreate|CFile::modeReadWrite) )
	{
		file.Close();
		remove( m_strTempName );
	}

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CThumbnailListCtrlEx

CThumbnailListCtrlEx::CThumbnailListCtrlEx()
{
	m_plistThumbImageItem	= new ListThumbImageItem;
}

CThumbnailListCtrlEx::~CThumbnailListCtrlEx()
{
	POSITION pos;
	while( (pos=m_plistThumbImageItem->GetHeadPosition()) )
	{
		CThumbImageItem* pItem = m_plistThumbImageItem->GetAt( pos );
		m_plistThumbImageItem->RemoveHead();
		delete pItem;
	}
	delete m_plistThumbImageItem;
}


BEGIN_MESSAGE_MAP(CThumbnailListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CThumbnailListCtrlEx)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT ( NM_CUSTOMDRAW, OnCustomdraw )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThumbnailListCtrlEx message handlers

void CThumbnailListCtrlEx::OnCustomdraw ( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	*pResult = 0;

	// If this is the beginning of the control's paint cycle, request
	// notifications for each item.

	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		// This is the pre-paint stage for an item.  We need to make another
		// request to be notified during the post-paint stage.

		*pResult = CDRF_NOTIFYPOSTPAINT;
	}
	else if ( CDDS_ITEMPOSTPAINT  == pLVCD->nmcd.dwDrawStage )
	{
		// This is the beginning of an item's paint cycle.
		LVITEM	rItem;
		int		nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );
		CDC*	pDC   = CDC::FromHandle ( pLVCD->nmcd.hdc );
		CRect	rcItem, rcLabel, rcAll;
		CString	str;

		ZeroMemory ( &rItem, sizeof(LVITEM) );
		rItem.mask  = LVIF_IMAGE | LVIF_STATE;
		rItem.iItem = nItem;
		rItem.stateMask = LVIS_SELECTED | LVIS_FOCUSED;

		GetItem ( &rItem );
		GetItemRect ( nItem, &rcItem, LVIR_ICON );
		GetItemRect ( nItem, &rcLabel, LVIR_LABEL );
		rcItem.DeflateRect(WIDTH_STEP,0,WIDTH_STEP,HEIGHT_STEP); 
		rcLabel.left = rcItem.left;
		rcLabel.right= rcItem.right;
		rcAll.UnionRect( rcItem, rcLabel );

		CBrush brush, *pOldbrush;
		///////////////////////////////////////////////////////////////////////
		// Draw Rectangle
		if ( rItem.state & LVIS_SELECTED )
		{
			pDC->FillSolidRect( rcAll, RGB(255,255,0) );
			// Draw frame.
			brush.CreateSolidBrush( RGB(255, 0, 0) );
			pOldbrush = pDC->SelectObject( &brush );
			pDC->FrameRect( rcAll,&brush );
			pDC->SelectObject( pOldbrush );
			brush.DeleteObject();
		}
		else
		{
			pDC->FillSolidRect( rcAll, ::GetSysColor( COLOR_3DFACE ) );
		}
		///////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////
		// draw image
		IMAGEINFO imgInfo;
		ZeroMemory( &imgInfo, sizeof(IMAGEINFO) );
		CImageList* pimgList = GetImageList(LVSIL_NORMAL);
		pimgList->GetImageInfo( nItem, &imgInfo );

		CRect   TmpRect(0,   WIDTH_STEP,   THUMB_WIDTH,   THUMB_HEIGHT); 

		CPoint ptTopleft(0,0);
		ptTopleft.x   =   rcItem.left   +   ((rcItem.Width()-TmpRect.Width())/2); 
		ptTopleft.y   =   rcItem.top   +   ((rcItem.Height()-TmpRect.Height())/2); 

		pimgList->Draw ( pDC, rItem.iImage, ptTopleft, ILD_NORMAL );

/*		HDC   hMemDC   =   CreateCompatibleDC(pDC->m_hDC); 
		::SelectObject(hMemDC,   imgInfo.hbmImage); 
		StretchBlt(pDC->m_hDC,   ptTopleft.x,   ptTopleft.y,   TmpRect.Width(),   TmpRect.Height(), 
					hMemDC,   0,   0,   TmpRect.Width(),   TmpRect.Height(),   SRCCOPY); 
		::DeleteDC(hMemDC); 
*/
		///////////////////////////////////////////////////////////////////////

		// Draw SN
		CFont font, *pOldfont;
		str.Format( "%d", nItem +1 );
		if ( rItem.state & LVIS_SELECTED )
			pDC->SetTextColor( RGB(255,0,0) );
		else
			pDC->SetTextColor( RGB(255,255,255) );
		font.CreateFont(-18, 0, 0, 0, FW_NORMAL, 0, 0, 0,
					ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Microsoft Sans Serif");
		pOldfont = pDC->SelectObject( &font );
		pDC->TextOut(rcItem.left +20, rcItem.top +10, str);
		pDC->SelectObject( pOldfont );
		font.DeleteObject();

		// Draw label
		str = GetItemText( nItem, 0 );
		if ( rItem.state & LVIS_SELECTED )
			pDC->SetTextColor( RGB(0,0,255) );
		else
			pDC->SetTextColor( RGB(0,0,0) );
		pDC->DrawText( str, rcLabel, DT_CENTER | DT_VCENTER );
/*		font.CreateFont(-9, 0, 0, 0, FW_NORMAL, 0, 0, 0,
					ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Microsoft Sans Serif");
		pOldfont = pDC->SelectObject( &font );
		pDC->DrawText( str, rcLabel, DT_CENTER | DT_VCENTER );
		pDC->SelectObject( pOldfont );
		font.DeleteObject();
*/		///////////////////////////////////////////////////////////////////////

		*pResult = CDRF_SKIPDEFAULT;    // We've painted everything.
	} 
}
