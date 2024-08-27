/*==========================================================================
 *
 *  Copyright (C) 1995-1997 Microsoft Corporation. All Rights Reserved.
 *
 *  File:       ddutil.cpp
 *  Content:    Routines for loading bitmap and palettes from resources
 *
 ***************************************************************************/
#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

#include "stdafx.h"
#include "ddutil.h"


//////////////////////////////////////////////////////////////////
#if !defined(DEBUG)

void __cdecl DebugMessage( LPSTR fmt, ... ) { }

#else

void __cdecl DebugMessage( LPSTR fmt, ... )
{
    char		sBuff[1024];
    va_list		va;

	// Initialize variable arguments.
    va_start(va, fmt);

    // format message with header
    lstrcpy( sBuff, "ImageCardAdapter:" );
    wvsprintf( &sBuff[ lstrlen(sBuff) ], fmt, va );
    lstrcat( sBuff, "\r\n" );

    // To the debugger unless we need to be quiet
//    if( !bStress )
    {
        OutputDebugString( sBuff );
    }

} // DebugMessage

#endif
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//  DDLoadPalette
//
//  Create a DirectDraw palette object from a bitmap handle
//
//  if the bitmap handle does not exist or NULL is passed create a
//  default palette.
//////////////////////////////////////////////////////////////////

LPDIRECTDRAWPALETTE DDLoadPalette(LPDIRECTDRAW pdd, HDIB hBmp)
{
    LPDIRECTDRAWPALETTE ddpal;
    int                 i;
    int                 n;
    LPBITMAPINFOHEADER  lpbi;
    PALETTEENTRY        ape[256];
    RGBQUAD *           prgb;

    //
    // build a gray palette as the default.
    //
    for (i=0; i<256; i++)
    {
		ape[i].peRed   = (BYTE)i;
		ape[i].peGreen = (BYTE)i;
		ape[i].peBlue  = (BYTE)i;
		ape[i].peFlags = (BYTE)0;
    }

    /*
	//
    // build a 332 palette as the default.
    //
    for (i=0; i<256; i++)
    {
		ape[i].peRed   = (BYTE)(((i >> 5) & 0x07) * 255 / 7);
		ape[i].peGreen = (BYTE)(((i >> 2) & 0x07) * 255 / 7);
		ape[i].peBlue  = (BYTE)(((i >> 0) & 0x03) * 255 / 3);
		ape[i].peFlags = (BYTE)0;
    }
	*/

    //
    // get a pointer to the bitmap
    //
    if ( hBmp != NULL )
    {
		lpbi = (LPBITMAPINFOHEADER)GlobalLock( (HGLOBAL)hBmp );
		if (!lpbi)
			OutputDebugString("lock hBmp failed\n");
		prgb = (RGBQUAD*)((BYTE*)lpbi + lpbi->biSize);

		if (lpbi == NULL || lpbi->biSize < sizeof(BITMAPINFOHEADER))
			n = 0;
		else if (lpbi->biBitCount > 8)
			n = 0;
		else if (lpbi->biClrUsed == 0)
			n = 1 << lpbi->biBitCount;
		else
			n = lpbi->biClrUsed;

		//
		//  a DIB color table has its colors stored BGR not RGB
		//  so flip them around.
		//
		for(i=0; i<n; i++ )
		{
			ape[i].peRed   = prgb[i].rgbRed;
			ape[i].peGreen = prgb[i].rgbGreen;
			ape[i].peBlue  = prgb[i].rgbBlue;
			ape[i].peFlags = 0;
		}
    }

    pdd->CreatePalette(DDPCAPS_8BIT, ape, &ddpal, NULL);

    return ddpal;
}

DWORD DDColorMatch(LPDIRECTDRAWSURFACE pdds, COLORREF rgb)
{
    COLORREF		rgbT;
    HDC				hdc;
    DWORD			dw = CLR_INVALID;
    DDSURFACEDESC	ddsd;
    HRESULT			hres;

    if (pdds->GetDC(&hdc) == DD_OK)
    {
        rgbT = GetPixel(hdc, 0, 0);
        SetPixel(hdc, 0, 0, rgb);
        pdds->ReleaseDC(hdc);
    }

    ddsd.dwSize = sizeof(ddsd);
    hres = pdds->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);

    if (hres == DD_OK)
    {
        dw  = *(DWORD *)ddsd.lpSurface;
	if(ddsd.ddpfPixelFormat.dwRGBBitCount != 32)
	    dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount)-1;
        pdds->Unlock(NULL);
    }
    else
    {
	pdds->GetSurfaceDesc(&ddsd);
	if(ddsd.ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE)
	    DebugMessage("Failed to lock Primary Surface!");
	else
	    DebugMessage("Failed to lock NON-PRIMARY Surface!");
    }

    if (pdds->GetDC(&hdc) == DD_OK)
    {
        SetPixel(hdc, 0, 0, rgbT);
        pdds->ReleaseDC(hdc);
    }

    return dw;
}
