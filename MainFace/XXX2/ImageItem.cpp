// FinderItem.cpp : implementation of the CFinderItem class
// Author : J.Y.Ma
// Last Modify : 2008.12.18

#include "stdafx.h"
#include "XXX2.h"
#include "ImageItem.h"

IMPLEMENT_DYNAMIC( CImageItem,		CObject );
//IMPLEMENT_DYNAMIC( CImageItem,		CFinderItem );


//灰度图像项目类构造函数
CImageItem::CImageItem()
{ 
	m_pIImage		= NULL;//new CxImage();								//为图像分配空间
	m_pImageLayer	= NULL;
	m_pGraphLayer	= NULL;

	New();
}

//灰度图像项目类带参数的构造函数
CImageItem::CImageItem( CxImage* pIImage )
{ 
	m_pIImage		= pIImage;										//传递图像指针
	m_pImageLayer	= pIImage;
	m_pGraphLayer	= pIImage;

	New();
}

void CImageItem::New()
{
	CTime	timeCurrent = CTime::GetCurrentTime();
	CString strTemp = timeCurrent.Format("%Y%m%d%H%M");
	CString csTitle;
	csTitle.Format( "SemImg%s",strTemp );

	m_strFileName = csTitle;//SetTitle( csTitle );

	m_nHV			= 0;
	m_nMag			= 0;

	m_bSel			= FALSE;
}

//灰度图像项目类析构函数
CImageItem::~CImageItem()
{
	if( m_pIImage )													//如果图像指针不为空，且是自动清除
	{
		if (m_pIImage) m_pIImage->SetEscape(1);
		delete m_pIImage;											//删除图像
		m_pIImage = NULL;											//将图像指针置为空
	}
	if (m_pImageLayer != NULL)
	{
		m_pImageLayer->SetEscape(1);
		delete m_pImageLayer;
		m_pImageLayer = NULL;
	}
	if (m_pGraphLayer != NULL)
	{
		m_pGraphLayer->SetEscape(1);
		delete m_pGraphLayer;
		m_pGraphLayer = NULL;
	}
}
//*************************************************************************************************
//用于小样显示
//参数:
//		CDC*	pDC		:设备环境类指针
//		CRect&	rect	:画图矩形区域地址
//		BOOL	bSel	:是否处于选中状态
//返回值:	void
//*************************************************************************************************
void CImageItem::Draw( CDC* pDC, CRect& rect, BOOL bSel/*, BOOL bFrame*/ )
{
//准备
	CRect rc = rect;
	//设置背景模式，并将位图区域设置为白色
	int nBkMode = pDC->SetBkMode( TRANSPARENT );
//	pDC->FillRect( rc, &CBrush( RGB( 255, 255, 255 ) ) );

//画图
	//非打印状态时画3d效果；打印时画边框
	if( !pDC->IsPrinting() )
	{
		if( m_bSel )//08.12.18
			pDC->FrameRect( rc, &CBrush( RGB(255,0,0) ) );
		else
			pDC->FrameRect( rc, &CBrush( RGB(255,255,255) ) );
	}
	
//画图像
	//缩小画图区域
	rc.DeflateRect( 2, 2 );
//	m_pIImage->DDraw( (long)pDC->GetSafeHdc(), rect.left+(rect.Width()-nWidth_rc)/2, rect.top+5, nWidth_rc, nHeight_rc );
	m_pIImage->Draw( pDC->GetSafeHdc(), rc );

//还原
	pDC->SetBkMode( nBkMode );
}

// 10.02.08 保存临时文件
BOOL CImageItem::SaveTempFile()
{
	char path[255];
	::GetModuleFileName( NULL, path, 255);
	char *ptemp = strrchr( path, '\\');
	ptemp[1] = 0;
	CString newName;
	newName.Format( "%s%s.tmpimg", path, m_strFileName );

	const double dblDPI = 25.4 * m_pIImage->GetWidth() / 127;
//	const double dblDPI_Y = 25.4 *  768 /  90;
	m_pIImage->SetXDPI((long)dblDPI);
	m_pIImage->SetYDPI((long)dblDPI);
	bool retval = m_pIImage->Save(newName, CXIMAGE_FORMAT_JPG);

	return retval;
}

// 10.02.08 删除临时文件
BOOL CImageItem::DeleteTempFile( CString strTitle )
{
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

BOOL CImageItem::DeleteTempFile()
{
	CString strTitle = m_strFileName;
	int nPos = strTitle.Find( '\\' );
	while( nPos != -1 )
	{
		strTitle = strTitle.Right( strTitle.GetLength() - nPos - 1 );
		nPos = strTitle.Find( '\\' );
	}
	return DeleteTempFile( strTitle );
}