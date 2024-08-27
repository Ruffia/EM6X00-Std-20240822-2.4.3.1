// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__C969A442_F547_4AA6_8152_232F4CC08237__INCLUDED_)
#define AFX_STDAFX_H__C969A442_F547_4AA6_8152_232F4CC08237__INCLUDED_
#define WM_USER_MC6600_Update	(WM_USER+100)

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#include <afxdocob.h>
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions
#include <afxmt.h>

#include <stdlib.h>
#include <tchar.h>
#include <cmath>
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <tuple>
#include <random>
#include "gdiplus.h"
using namespace Gdiplus;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#define AFX_ZERO_INIT_OBJECT(base_class) \
	memset(((base_class*)this)+1, 0, sizeof(*this) - sizeof(class base_class)); 

#endif // !defined(AFX_STDAFX_H__C969A442_F547_4AA6_8152_232F4CC08237__INCLUDED_)