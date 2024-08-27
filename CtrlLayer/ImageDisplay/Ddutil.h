/*==========================================================================
 *
 *  Copyright (C) 1995 Microsoft Corporation. All Rights Reserved.
 *
 *  File:       ddutil.cpp
 *  Content:    Routines for loading bitmap and palettes from resources
 *
 ***************************************************************************/
#ifndef __DDUNIT__
#define __DDUNIT__

#include "dibapi.h"

void __cdecl DebugMessage( LPSTR fmt, ... );
LPDIRECTDRAWPALETTE DDLoadPalette(LPDIRECTDRAW pdd, HDIB hBmp);
DWORD DDColorMatch(LPDIRECTDRAWSURFACE pdds, COLORREF rgb);

#endif