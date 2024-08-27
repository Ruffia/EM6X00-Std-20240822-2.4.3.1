// FinderItem.cpp : implementation of the CFinderItem class
// Author : J.Y.Ma
// Last Modify : 2008.12.18

#include "stdafx.h"
#include "XXX2.h"
#include "ImageItem.h"

IMPLEMENT_DYNAMIC( CImageItem,		CObject );
//IMPLEMENT_DYNAMIC( CImageItem,		CFinderItem );


//�Ҷ�ͼ����Ŀ�๹�캯��
CImageItem::CImageItem()
{ 
	m_pIImage		= NULL;//new CxImage();								//Ϊͼ�����ռ�
	m_pImageLayer	= NULL;
	m_pGraphLayer	= NULL;

	New();
}

//�Ҷ�ͼ����Ŀ��������Ĺ��캯��
CImageItem::CImageItem( CxImage* pIImage )
{ 
	m_pIImage		= pIImage;										//����ͼ��ָ��
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

//�Ҷ�ͼ����Ŀ����������
CImageItem::~CImageItem()
{
	if( m_pIImage )													//���ͼ��ָ�벻Ϊ�գ������Զ����
	{
		if (m_pIImage) m_pIImage->SetEscape(1);
		delete m_pIImage;											//ɾ��ͼ��
		m_pIImage = NULL;											//��ͼ��ָ����Ϊ��
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
//����С����ʾ
//����:
//		CDC*	pDC		:�豸������ָ��
//		CRect&	rect	:��ͼ���������ַ
//		BOOL	bSel	:�Ƿ���ѡ��״̬
//����ֵ:	void
//*************************************************************************************************
void CImageItem::Draw( CDC* pDC, CRect& rect, BOOL bSel/*, BOOL bFrame*/ )
{
//׼��
	CRect rc = rect;
	//���ñ���ģʽ������λͼ��������Ϊ��ɫ
	int nBkMode = pDC->SetBkMode( TRANSPARENT );
//	pDC->FillRect( rc, &CBrush( RGB( 255, 255, 255 ) ) );

//��ͼ
	//�Ǵ�ӡ״̬ʱ��3dЧ������ӡʱ���߿�
	if( !pDC->IsPrinting() )
	{
		if( m_bSel )//08.12.18
			pDC->FrameRect( rc, &CBrush( RGB(255,0,0) ) );
		else
			pDC->FrameRect( rc, &CBrush( RGB(255,255,255) ) );
	}
	
//��ͼ��
	//��С��ͼ����
	rc.DeflateRect( 2, 2 );
//	m_pIImage->DDraw( (long)pDC->GetSafeHdc(), rect.left+(rect.Width()-nWidth_rc)/2, rect.top+5, nWidth_rc, nHeight_rc );
	m_pIImage->Draw( pDC->GetSafeHdc(), rc );

//��ԭ
	pDC->SetBkMode( nBkMode );
}

// 10.02.08 ������ʱ�ļ�
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

// 10.02.08 ɾ����ʱ�ļ�
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