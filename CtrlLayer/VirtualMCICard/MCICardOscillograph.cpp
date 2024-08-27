// MCICard.cpp : implementation file
//

#include "stdafx.h"
#include "Ddutil.h"
#include "MCICard.h"
#include "RectConvertor.h"
#include "MCICardOption.h"
#include "MCICardCommonSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const double g_dGraphRange[] = {10,
								2.5,	// 0xFFF
								1.25,
								0.625,
								0.3125,
								0.15625,
								0.078125,
								0.0390625,
								0.01853125,
								0.009265625};

extern	TCHAR	g_exeFullPath[255];
extern	int		g_index;

////////////////////////////////////////////////////////////////////////////////////
// MCICard

