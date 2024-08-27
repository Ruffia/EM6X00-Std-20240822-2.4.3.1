// MCICard.cpp : implementation file
//

#include "stdafx.h"
#include "Ddutil.h"
#include "MCICard.h"
#include "MCICardCommonSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMCICard

//卷积(模板操作)
BOOL CMCICard::Convolution( double *Kernel, CSize KernelSize, int nX1, int nY1, int nX2, int nY2, LPSTR lpDIBBits)
{
	CSize& m_szImage = CMCICardCommonSetting::Instance().m_szImage;
	int nOffX = KernelSize.cx/2+1;
	int nOffY = KernelSize.cy/2+1;
	int nWidth = m_szImage.cx;
	int nHeight = m_szImage.cy;
	if( nX1 < nOffX ) nX1 = nOffX;
	if( nY1 < nOffY ) nY1 = nOffY;
	if( nX2 >= nWidth - nOffX ) nX2 = nWidth - nOffX;
	if( nY2 >= nHeight - nOffY ) nY2 = nHeight - nOffY;

	LONG GrayTotal;
	unsigned char Data;

	unsigned char *pNewBuffer, *pOldBits, *pNewBits, *pOldTemp, *pNewTemp;
//	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
//	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
//	RGBQUAD *pOldPalette, *pNewPalette;
	int nWidthBytes, x, y, j, l;
/*
	pOldBuffer = (unsigned char *) m_pImageObject->GetDIBPointer( &nWidthBytes, m_pImageObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	nNumColors = m_pImageObject->GetNumColors();
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+nNumColors*sizeof(RGBQUAD)];
*/
	nWidthBytes = nWidth;
	pOldBits = (unsigned char *)lpDIBBits;

	DWORD dwNewSize;
	HGLOBAL hNewDib;

	dwNewSize = /*sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + nNumColors * sizeof( RGBQUAD ) + */nWidthBytes * nHeight;
	hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	if( hNewDib == NULL )
	{
		return( FALSE );
	}

	pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNewBuffer == NULL )
	{
		::GlobalFree( hNewDib );
		return( FALSE );
	}

/*	pNewBFH = (BITMAPFILEHEADER *) pNewBuffer;
	pNewBIH = (BITMAPINFOHEADER *) &pNewBuffer[sizeof(BITMAPFILEHEADER)];
	pNewPalette = (RGBQUAD *) &pNewBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	pNewBits = (unsigned char *) &pNewBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+nNumColors*sizeof(RGBQUAD)];

	*pNewBFH = *pOldBFH;
	*pNewBIH = *pOldBIH;
	memcpy( pNewPalette, pOldPalette, nNumColors*sizeof(RGBQUAD) );
*/
	pNewBits = pNewBuffer;
	memcpy( pNewBits, pOldBits, nWidthBytes * nHeight );

//	((LPBYTE)lpDIBBits)[ (m_szImage.cy - pdwPixY[i] - 1) * lImageLineOffset + pdwPixX[i] ]  = (BYTE)pdwPixZ[i];
	for( y=nY1; y<=nY2; y++ )
	{
		pOldTemp = pOldBits;
		pOldTemp += ( ( nHeight - 1 - y - 1 ) * nWidthBytes );
		pOldTemp += ( nX1 - 1 );
		pNewTemp = pNewBits;
		pNewTemp += ( ( nHeight - 1 - y ) * nWidthBytes );
		pNewTemp += nX1;
		l = 0;
		for( x=nX1; x<=nX2; x++ )
		{
			//遍历模板中所有像素的灰度值，求加权总和
			j = 0;
			GrayTotal = 0;
			for( int k=0; k<KernelSize.cy; k++ )
			{
				for(int i=0; i<KernelSize.cx; i++ )
				{
					Data = pOldTemp[i+l+k*nWidthBytes];
					GrayTotal += ( (LONG)(Data * Kernel[i+k*KernelSize.cx]) );
				}
			}
			if( GrayTotal < 0 )
				GrayTotal = 0;
			if( GrayTotal > 255 )
				GrayTotal = 255;
			//根据新的R,G,B值得到最接近的调色板索引值
			*pNewTemp++ = (unsigned char) GrayTotal;
			l++;
		}
	}
	memcpy( pNewBits, pOldBits, nWidth * nHeight );

	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	return( TRUE );
}

/**********************************************************
* 低通滤波
* 遍历3x3模板中所有像素的灰度值，求加权平均值
*			 1　┏1   1   1┓
*			━　┃1   1· 1┃
*			 9  ┗1   1   1┛
**********************************************************/
BOOL CMCICard::LowpassFilter( DWORD *dwFact, int nX1, int nY1, int nX2, int nY2, LPSTR lpDIBBits )
{
	double kernel3[9] = { 1, 1, 1, 1, 1, 1, 1, 1, 1};
	for( int i=0; i<9; i++ )
		kernel3[i] /= 9.;
	int KernelSize = 3;
	return Convolution(kernel3, CSize(KernelSize,KernelSize), nX1, nY1, nX2, nY2, lpDIBBits);
}

/**********************************************************
* 高斯滤波平滑 Page268
* 遍历3x3模板中所有像素的灰度值，求加权平均值
*			 1 ┏1   2   1┓
*			━ ┃2   4· 2┃
*			16 ┗1   2   1┛
**********************************************************/
BOOL CMCICard::Gauss(UINT uSize, int nX1, int nY1, int nX2, int nY2, LPSTR lpDIBBits)
{
	double kernel3[9] = { 1,2,1,2,4,2,1,2,1};
	for( int i=0; i<9; i++ )
		kernel3[i] /= 16.;
	int KernelSize = 3;
	return Convolution(kernel3, CSize(KernelSize,KernelSize), nX1, nY1, nX2, nY2, lpDIBBits);
}
