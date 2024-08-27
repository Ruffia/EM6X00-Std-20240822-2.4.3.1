// FilmPhotoData.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
//#include "ImageDoc.h"
//#include "ImageCard.h"

#include "FilmPhotoData.h"
#include "ControlLayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


///////////////////////////////////////////////////////////////////
const BYTE g_bGraphFlagTrans = 0;
const BYTE g_bGraphFlagBackground = 3;
const BYTE g_bGraphFlagForeground = 2;

///////////////////////////////////////////////////////////////////
const long FilmPhoto_Width		= 1024;	// g_szFrameGraph.cx;
const long FilmPhoto_Height		= 768;	// g_szFrameGraph.cy;
const long FilmPhoto_BUFFSIZE	= FilmPhoto_Width * FilmPhoto_Height;
///////////////////////////////////////////////////////////////////
BYTE g_cFilmPhotoData[ FilmPhoto_BUFFSIZE ];
///////////////////////////////////////////////////////////////////

LPBYTE FormatGraphDataByBmp_Bit4( CBitmap& bmpGraph );
LPBYTE FormatGraphDataByBmp_Bit8( CBitmap& bmpGraph );
LPBYTE FormatGraphDataByBmp_Bit16( CBitmap& bmpGraph );
LPBYTE FormatGraphDataByBmp_Bit24( CBitmap& bmpGraph );
LPBYTE FormatGraphDataByBmp_Bit32( CBitmap& bmpGraph );


///////////////////////////////////////////////////////////////////

LPBYTE MakeFilmPhotoDataByGraphLay()
{
	LPBYTE		lpBuff = NULL;
	CBitmap		bmpGraphLay;

	if ( !CSemVirtualMCICard::Instance().GetGraphBmp( bmpGraphLay ) )
		return lpBuff;

	////////////////////////////////////////////////////
	int wBpp;
	////////////////////////////////////////////////////
	BITMAP GraphBitMap;
	bmpGraphLay.GetBitmap( &GraphBitMap );
	wBpp = GraphBitMap.bmBitsPixel;
	////////////////////////////////////////////////////
    if		(wBpp <= 1)		wBpp = 1;
    else if (wBpp <= 4)		wBpp = 4;
    else if (wBpp <= 8)		wBpp = 8;
    else if (wBpp <= 16)	wBpp = 16;
    else if (wBpp <= 24)	wBpp = 24;
    else					wBpp = 32;
	////////////////////////////////////////////////////

	switch (wBpp)
	{
	case 4:
		lpBuff = FormatGraphDataByBmp_Bit4( bmpGraphLay );
		break;
	case 8:
		lpBuff = FormatGraphDataByBmp_Bit8( bmpGraphLay );
		break;
	case 16:
		lpBuff = FormatGraphDataByBmp_Bit16( bmpGraphLay );
		break;
	case 24:
		lpBuff = FormatGraphDataByBmp_Bit24( bmpGraphLay );
		break;
	case 32:
		lpBuff = FormatGraphDataByBmp_Bit32( bmpGraphLay );
		break;
	default:
		break;
	}

	bmpGraphLay.DeleteObject();

	return lpBuff;
}

///////////////////////////////////////////////////////////////////

LPBYTE FormatGraphDataByBmp_Bit4( CBitmap& bmpGraph )
{
	return NULL;
}

LPBYTE FormatGraphDataByBmp_Bit8( CBitmap& bmpGraph )
{
	return NULL;
}

LPBYTE FormatGraphDataByBmp_Bit16( CBitmap& bmpGraph )
{
	return NULL;
}

LPBYTE FormatGraphDataByBmp_Bit24( CBitmap& bmpGraph )
{
	typedef struct tagBMPRGB { // rgb
		BYTE    rgbBlue; 
		BYTE    rgbGreen; 
		BYTE    rgbRed; 
	} BMPRGB;
	typedef BMPRGB FAR *	LPBMPRGB;


	return NULL;
}

// 0x00rrggbb --> 0x00bbggrr
COLORREF From0RGBto0BGR( COLORREF clr )
{
	union tagBMPCLR{
		COLORREF	clr;
		BYTE		bData[4];
	}clrRGB,clrBGR;
	clrRGB.clr = clr;
	
	clrBGR.bData[0] = clrRGB.bData[2];
	clrBGR.bData[1] = clrRGB.bData[1];
	clrBGR.bData[2] = clrRGB.bData[0];
	clrBGR.bData[3] = clrRGB.bData[3];

	return clrBGR.clr;
}


LPBYTE FormatGraphDataByBmp_Bit32( CBitmap& bmpGraph )
{
	////////////////////////////////////////////////////////////
	const COLORREF clrTrans = From0RGBto0BGR( g_clrUSB_Transparent );
	const COLORREF clrBack	= From0RGBto0BGR( g_clrUSB_Background );
	const long	 nBytePP	= sizeof(COLORREF);	// Byte Per Pixel
	////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	LPBYTE pBuffer = new BYTE[ FilmPhoto_BUFFSIZE * nBytePP ];
	if ( pBuffer == NULL ) return NULL;
	///////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////
	bmpGraph.GetBitmapBits( FilmPhoto_BUFFSIZE * nBytePP, pBuffer );
	///////////////////////////////////////////////////////////////////

	LPCOLORREF	pBmpLine;
	LPBYTE		pGraphLine;

	COLORREF	colorRGB;
	for ( int j=0; j<FilmPhoto_Height; j++ )
	{
		pBmpLine = (LPCOLORREF)&(pBuffer[ j * FilmPhoto_Width * nBytePP ]);
		pGraphLine = (LPBYTE)&(g_cFilmPhotoData[ j * FilmPhoto_Width ]);

		for ( int i=0; i<FilmPhoto_Width; i++ )
		{
			colorRGB	= (COLORREF)pBmpLine[i];

			if ( colorRGB == clrTrans )
				pGraphLine[i] = g_bGraphFlagTrans;
			else if ( colorRGB == clrBack )
				pGraphLine[i] = g_bGraphFlagBackground;
			else
				pGraphLine[i] = g_bGraphFlagForeground;
		}
	}

	///////////////////////////////////////////////////////////////////
	delete pBuffer;
	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////

	return g_cFilmPhotoData;
}

LPBYTE MakeFilmPhotoDataByImageLay()
{
	if ( MakeFilmPhotoDataByGraphLay() == NULL )
		return NULL;

	const BYTE clrText	= theApp.m_nDPhotoForeground;
	const BYTE clrBack	= theApp.m_nDPhotoBackground;

	CRect	rcFrameDestWindow(0,0,FilmPhoto_Width,FilmPhoto_Height);
	BYTE	cLineBuff[ FilmPhoto_Width ], color;
	LPBYTE	pGraphLine;

	for ( int j=0; j<FilmPhoto_Height; j++ )
	{
		rcFrameDestWindow.top		= j;
		rcFrameDestWindow.bottom	= j+1;
		CSemVirtualMCICard::Instance().Read( cLineBuff, FilmPhoto_Width, rcFrameDestWindow );

		pGraphLine = (LPBYTE)&(g_cFilmPhotoData[ j * FilmPhoto_Width ]);

		for ( int i=0; i<FilmPhoto_Width; i++ )
		{
			color		= pGraphLine[i];

			if ( color == g_bGraphFlagTrans )
				pGraphLine[i] = cLineBuff[i];
			else if ( color == g_bGraphFlagBackground )
				pGraphLine[i] = clrBack;
			else
				pGraphLine[i] = clrText;
		}
	}

	return g_cFilmPhotoData;
}
