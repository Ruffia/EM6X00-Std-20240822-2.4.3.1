// MCICard.cpp : implementation file
//

#include "stdafx.h"
#include "Ddutil.h"
#include "MCICard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMCICard

BOOL CMCICard::ShowLabel(BOOL boolShow)
{
	if ( m_boolShowLabel == boolShow )
		return m_boolShowLabel;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	CRect rcGrapic = CRect(0, m_szGraph.cy - m_szLabel.cy,
							m_szGraph.cx, m_szGraph.cy);
	CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );

	if ( m_boolShowLabel == TRUE )
	{	
		m_UpdateRectsImage.Add( rcImage );
	}
	else
	{
		m_UpdateRectsLabel.Add( rcImage );
	}

	m_boolShowLabel = boolShow;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////

	return m_boolShowLabel;
}

BOOL CMCICard::ShowTime(BOOL boolShow)
{
	if ( m_boolShowTime == boolShow )
		return m_boolShowTime;
	m_boolShowTime = boolShow;
	if( m_boolShowLabel )
		RedrawLabelLay3();
	return TRUE;
}

BOOL CMCICard::ShowNote(BOOL boolShow)
{
	if ( m_boolShowNote == boolShow )
		return m_boolShowNote;
	m_boolShowNote = boolShow;
	if( m_boolShowLabel )
		RedrawLabelLay3();
	return TRUE;
}

BOOL CMCICard::InitLabelLay()
{
	if( m_sizeReso.cx < 128+1 )
		InitLabelLay_0128();
	else if( m_sizeReso.cx < 384+1 )
		InitLabelLay_0256();
	else if( m_sizeReso.cx < 480+1 )
		InitLabelLay_0480();
	else if( m_sizeReso.cx < 600+1 )
		InitLabelLay_0512();
	else if( m_sizeReso.cx < 640+1 )
		InitLabelLay_0640();
	else if( m_sizeReso.cx < 800+1 )
		InitLabelLay_0800();
	else if( m_sizeReso.cx < 1280+1 )
		InitLabelLay_1024();
	else if( m_sizeReso.cx < 1536+1 )
		InitLabelLay_1440();
	else if( m_sizeReso.cx < 2048+1 )
		InitLabelLay_2048();
	else
		InitLabelLay_4096();
	RedrawLabelLay3();

	return TRUE;
}

void CMCICard::InitLabelLay_0128()
{
	/////////////////////////////////////////////////
	m_csHV				= _T("0.0 KV");				// 高压显示
	m_csMag				= _T("250 X");				// 放大倍数显示
	m_csUScal			= _T("100 μm");			// 固定标尺显示
	m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	m_csWD				= _T("12.0 mm");
	/////////////////////////////////////////////////

	m_nUScalePixelNum	= 40;

	/////////////////////////////////////////////////
	m_ptHV				= CPoint(1,		10);		// 高压显示位置
	m_ptMag				= CPoint(24,	10);		// 放大倍数显示位置
	m_ptUScal			= CPoint(48,	10);		// 固定标尺显示位置
	m_ptLogo			= CPoint(64,	10);		// 产品标识显示位置
	m_ptPhotoSN			= CPoint(110,	10);		// 照相编号显示位置
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (m_hfontEditText != NULL)
        DeleteObject(m_hfontEditText);

    if (m_hfontLabelText != NULL)
        DeleteObject(m_hfontLabelText);

    if (m_hfontLabelLogoText != NULL)
        DeleteObject(m_hfontLabelLogoText);

    m_hfontEditText = CreateFont(
        8, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        "Arial" );//m_sysOpt.fName[SysOpt_nameEditText] );

    m_hfontLabelText = CreateFont(
        8, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        "Arial" );//m_sysOpt.fName[SysOpt_nameEditText] );

    m_hfontLabelLogoText = CreateFont(
        8, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        "Arial" );//m_sysOpt.fName[SysOpt_nameEditText] );
	/////////////////////////////////////////////////
}

void CMCICard::InitLabelLay_0256()
{
	/////////////////////////////////////////////////
	m_csHV				= _T("0.0 KV");				// 高压显示
	m_csMag				= _T("250 X");				// 放大倍数显示
	m_csUScal			= _T("100 μm");				// 固定标尺显示
	m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	m_csWD				= _T("12.0 mm");			// 工作距离显示
	m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	m_nUScalePixelNum	= 80;

	/////////////////////////////////////////////////
// 	m_ptHV				= CPoint( 4,		16);		// 高压显示位置
// 	m_ptMag				= CPoint(48,		16);		// 放大倍数显示位置
// 	m_ptWD				= CPoint(0, 16);
// 	m_ptSG				= CPoint(0, 16);
// 	m_ptUScal			= CPoint(104,		16);		// 固定标尺显示位置
// 	m_ptLogo			= CPoint(128,		16);		// 产品标识显示位置
// 	m_ptPhotoSN			= CPoint(210,		16);		// 照相编号显示位置
	/////////////////////////////////////////////////
	/////////////////////////////////////////////////
	m_ptHV				= CPoint(0,		1);
	m_ptMag				= CPoint(56,	1);
	m_ptWD				= CPoint(0,		16);
	m_ptSG				= CPoint(56,	16);
	m_ptUScal			= CPoint(110,	1);
	m_ptLogo			= CPoint(110,	16);
	m_ptTime			= CPoint(220,	1);
	m_ptNote			= CPoint(220,	16);
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (m_hfontEditText != NULL)
        DeleteObject(m_hfontEditText);

    if (m_hfontLabelText != NULL)
        DeleteObject(m_hfontLabelText);

    if (m_hfontLabelLogoText != NULL)
        DeleteObject(m_hfontLabelLogoText);

    m_hfontEditText = CreateFont(
        7, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameEditText] );

    m_hfontLabelText = CreateFont(
        7, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );

    m_hfontLabelLogoText = CreateFont(
        7, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}

void CMCICard::InitLabelLay_0384()
{
	/////////////////////////////////////////////////
	m_csHV				= _T("0.0 KV");				// 高压显示
	m_csMag				= _T("250 X");				// 放大倍数显示
	m_csUScal			= _T("100 μm");				// 固定标尺显示
	m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	m_csWD				= _T("12.0 mm");			// 工作距离显示
	m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	m_nUScalePixelNum	= 80;

	/////////////////////////////////////////////////
// 	m_ptHV				= CPoint( 4,		16);		// 高压显示位置
// 	m_ptMag				= CPoint(48,		16);		// 放大倍数显示位置
// 	m_ptWD				= CPoint(0, 16);
// 	m_ptSG				= CPoint(0, 16);
// 	m_ptUScal			= CPoint(104,		16);		// 固定标尺显示位置
// 	m_ptLogo			= CPoint(128,		16);		// 产品标识显示位置
// 	m_ptPhotoSN			= CPoint(210,		16);		// 照相编号显示位置
	/////////////////////////////////////////////////
	/////////////////////////////////////////////////
	m_ptHV				= CPoint(4,		1);
	m_ptMag				= CPoint(90,	1);
	m_ptWD				= CPoint(4,		16);
	m_ptSG				= CPoint(90,	16);
	m_ptUScal			= CPoint(175,	1);
	m_ptLogo			= CPoint(175,	16);
	m_ptTime			= CPoint(350,	1);
	m_ptNote			= CPoint(350,	16);
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (m_hfontEditText != NULL)
        DeleteObject(m_hfontEditText);

    if (m_hfontLabelText != NULL)
        DeleteObject(m_hfontLabelText);

    if (m_hfontLabelLogoText != NULL)
        DeleteObject(m_hfontLabelLogoText);

    m_hfontEditText = CreateFont(
        12, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameEditText] );

    m_hfontLabelText = CreateFont(
        12, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );

    m_hfontLabelLogoText = CreateFont(
        12, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}
void CMCICard::InitLabelLay_0480()
{
	/////////////////////////////////////////////////
	m_csHV				= _T("0.0 KV");				// 高压显示
	m_csMag				= _T("250 X");				// 放大倍数显示
	m_csUScal			= _T("100 μm");				// 固定标尺显示
	m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	m_csWD				= _T("12.0 mm");			// 工作距离显示
	m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	m_nUScalePixelNum	= 80;

	/////////////////////////////////////////////////
// 	m_ptHV				= CPoint( 4,		16);		// 高压显示位置
// 	m_ptMag				= CPoint(48,		16);		// 放大倍数显示位置
// 	m_ptWD				= CPoint(0, 16);
// 	m_ptSG				= CPoint(0, 16);
// 	m_ptUScal			= CPoint(104,		16);		// 固定标尺显示位置
// 	m_ptLogo			= CPoint(128,		16);		// 产品标识显示位置
// 	m_ptPhotoSN			= CPoint(210,		16);		// 照相编号显示位置
	/////////////////////////////////////////////////
	/////////////////////////////////////////////////
	m_ptHV				= CPoint(8,		1);
	m_ptMag				= CPoint(120,	1);
	m_ptWD				= CPoint(8,		16);
	m_ptSG				= CPoint(120,	16);
	m_ptUScal			= CPoint(220,	1);
	m_ptLogo			= CPoint(220,	16);
	m_ptTime			= CPoint(350,	1);
	m_ptNote			= CPoint(350,	16);
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (m_hfontEditText != NULL)
        DeleteObject(m_hfontEditText);

    if (m_hfontLabelText != NULL)
        DeleteObject(m_hfontLabelText);

    if (m_hfontLabelLogoText != NULL)
        DeleteObject(m_hfontLabelLogoText);

    m_hfontEditText = CreateFont(
        12, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameEditText] );

    m_hfontLabelText = CreateFont(
        12, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );

    m_hfontLabelLogoText = CreateFont(
        12, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}

void CMCICard::InitLabelLay_0512()
{
	/////////////////////////////////////////////////
	m_csHV				= _T("0.0 KV");				// 高压显示
	m_csMag				= _T("250 X");				// 放大倍数显示
	m_csUScal			= _T("100 μm");			// 固定标尺显示
	m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	m_csWD				= _T("12.0 mm");			// 工作距离显示
	m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	m_nUScalePixelNum	= 100;

	/////////////////////////////////////////////////
// 	m_ptHV				= CPoint( 32-16,		20);		// 高压显示位置
// 	m_ptMag				= CPoint(96+16,			20);		// 放大倍数显示位置
// 	m_ptWD				= CPoint(0, 20);
// 	m_ptSG				= CPoint(0, 20);
// 	m_ptUScal			= CPoint(256-16,		20);		// 固定标尺显示位置
// 	m_ptLogo			= CPoint(320-32,		20);		// 产品标识显示位置
// 	m_ptPhotoSN			= CPoint(460-32,		20);		// 照相编号显示位置
	/////////////////////////////////////////////////
	/////////////////////////////////////////////////
	m_ptHV				= CPoint(8,		2);
	m_ptMag				= CPoint(120,	2);
	m_ptWD				= CPoint(8,		24);
	m_ptSG				= CPoint(120,	24);
	m_ptUScal			= CPoint(232,	2);
	m_ptLogo			= CPoint(232,	24);
	m_ptTime			= CPoint(350,	2);
	m_ptNote			= CPoint(350,	24);
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (m_hfontEditText != NULL)
        DeleteObject(m_hfontEditText);

    if (m_hfontLabelText != NULL)
        DeleteObject(m_hfontLabelText);

    if (m_hfontLabelLogoText != NULL)
        DeleteObject(m_hfontLabelLogoText);

    m_hfontEditText = CreateFont(
        16, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameEditText] );

    m_hfontLabelText = CreateFont(
        16, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );

    m_hfontLabelLogoText = CreateFont(
        16, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}

void CMCICard::InitLabelLay_0640()
{
	/////////////////////////////////////////////////
	m_csHV				= _T("0.0 KV");				// 高压显示
	m_csMag				= _T("250 X");				// 放大倍数显示
	m_csUScal			= _T("100 μm");			// 固定标尺显示
	m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	m_csWD				= _T("12.0 mm");			// 工作距离显示
	m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	m_nUScalePixelNum	= 100;

	/////////////////////////////////////////////////
// 	m_ptHV				= CPoint( 32-16,		20);		// 高压显示位置
// 	m_ptMag				= CPoint(96+16,			20);		// 放大倍数显示位置
// 	m_ptWD				= CPoint(0, 20);
// 	m_ptSG				= CPoint(0, 20);
// 	m_ptUScal			= CPoint(256-16,		20);		// 固定标尺显示位置
// 	m_ptLogo			= CPoint(320,			20);		// 产品标识显示位置
// 	m_ptPhotoSN			= CPoint(460+32,		20);		// 照相编号显示位置
	/////////////////////////////////////////////////
	/////////////////////////////////////////////////
	m_ptHV				= CPoint(16,	2);
	m_ptMag				= CPoint(160,	2);
	m_ptWD				= CPoint(16,	22);
	m_ptSG				= CPoint(160,	22);
	m_ptUScal			= CPoint(300,	2);
	m_ptLogo			= CPoint(300,	22);
	m_ptTime			= CPoint(440,	2);
	m_ptNote			= CPoint(440,	22);
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (m_hfontEditText != NULL)
        DeleteObject(m_hfontEditText);

    if (m_hfontLabelText != NULL)
        DeleteObject(m_hfontLabelText);

    if (m_hfontLabelLogoText != NULL)
        DeleteObject(m_hfontLabelLogoText);

    m_hfontEditText = CreateFont(
        16, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameEditText] );

    m_hfontLabelText = CreateFont(
        20, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );

    m_hfontLabelLogoText = CreateFont(
        20, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}

void CMCICard::InitLabelLay_0800()
{
	/////////////////////////////////////////////////
	m_csHV				= _T("0.0 KV");				// 高压显示
	m_csMag				= _T("250 X");				// 放大倍数显示
	m_csUScal			= _T("100 μm");				// 固定标尺显示
	m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	m_csWD				= _T("12.0 mm");			// 工作距离显示
	m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	m_nUScalePixelNum	= 100;

	/////////////////////////////////////////////////
// 	m_ptHV				= CPoint( 32-16,		20);		// 高压显示位置
// 	m_ptMag				= CPoint(96+48,			20);		// 放大倍数显示位置
// 	m_ptWD				= CPoint(0, 20);
// 	m_ptSG				= CPoint(0, 20);
// 	m_ptUScal			= CPoint(256+32+32,		20);		// 固定标尺显示位置
// 	m_ptLogo			= CPoint(320+64+32,		20);		// 产品标识显示位置
// 	m_ptPhotoSN			= CPoint(600+32,		20);		// 照相编号显示位置
	/////////////////////////////////////////////////
	/////////////////////////////////////////////////
	m_ptHV				= CPoint(16,	2);
	m_ptMag				= CPoint(200,	2);
	m_ptWD				= CPoint(16,	24);
	m_ptSG				= CPoint(200,	24);
	m_ptUScal			= CPoint(400,	2);
	m_ptLogo			= CPoint(400,	24);
	m_ptTime			= CPoint(580,	2);
	m_ptNote			= CPoint(580,	24);
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (m_hfontEditText != NULL)
        DeleteObject(m_hfontEditText);

    if (m_hfontLabelText != NULL)
        DeleteObject(m_hfontLabelText);

    if (m_hfontLabelLogoText != NULL)
        DeleteObject(m_hfontLabelLogoText);

    m_hfontEditText = CreateFont(
        20, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameEditText] );

    m_hfontLabelText = CreateFont(
        20, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );

    m_hfontLabelLogoText = CreateFont(
        20, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}

void CMCICard::InitLabelLay_1024()
{
	/////////////////////////////////////////////////
	m_csHV				= _T("0.0 KV");				// 高压显示
	m_csMag				= _T("250 X");				// 放大倍数显示
	m_csUScal			= _T("100 μm");			// 固定标尺显示
	m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	m_csWD				= _T("12.0 mm");			// 工作距离显示
	m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	m_nUScalePixelNum	= 100;

	/////////////////////////////////////////////////
//	m_ptHV				= CPoint( 64,24);		// 高压显示位置
//	m_ptMag				= CPoint(192+16,24);	// 放大倍数显示位置
//	m_ptUScal			= CPoint(512-64-16,24);	// 固定标尺显示位置
//	m_ptLogo			= CPoint(640-64-16,24);	// 产品标识显示位置
//	m_ptPhotoSN			= CPoint(896-32-16,24);	// 照相编号显示位置
	/////////////////////////////////////////////////
	/////////////////////////////////////////////////
// 	m_ptHV				= CPoint( 64-16,		20);		// 高压显示位置
// 	m_ptMag				= CPoint(192+16-16,		20);		// 放大倍数显示位置
// 	m_ptUScal			= CPoint(512-64-32,		20);		// 固定标尺显示位置
// 	m_ptLogo			= CPoint(640/*-64-40*/,		20);	// 产品标识显示位置
// 	m_ptPhotoSN			= CPoint(896/*-32*/-8,		20);	// 照相编号显示位置
	/////////////////////////////////////////////////
	/////////////////////////////////////////////////
// 	m_ptHV				= CPoint(16, 20);
// 	m_ptMag				= CPoint(120, 20);
// 	m_ptWD				= CPoint(240, 20);
// 	m_ptSG				= CPoint(360+16, 20);
// 	m_ptUScal			= CPoint(512, 20);
// 	m_ptLogo			= CPoint(640, 20);
// 	m_ptPhotoSN			= CPoint(880, 20);
	/////////////////////////////////////////////////
	/////////////////////////////////////////////////
	m_ptHV				= CPoint(16,	2);
	m_ptMag				= CPoint(240,	2);
	m_ptWD				= CPoint(16,	34);
	m_ptSG				= CPoint(240,	34);
	m_ptUScal			= CPoint(480,	2);
	m_ptLogo			= CPoint(480,	34);
	m_ptTime			= CPoint(736,	2);
	m_ptNote			= CPoint(736,	34);
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (m_hfontEditText != NULL)
        DeleteObject(m_hfontEditText);

    if (m_hfontLabelText != NULL)
        DeleteObject(m_hfontLabelText);

    if (m_hfontLabelLogoText != NULL)
        DeleteObject(m_hfontLabelLogoText);

    m_hfontEditText = CreateFont(
        m_sysOpt.fSize[SysOpt_nameEditText], 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameEditText] );

    m_hfontLabelText = CreateFont(
        m_sysOpt.fSize[SysOpt_nameLabelText]/*Label_nFontSize*/, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );

    m_hfontLabelLogoText = CreateFont(
        m_sysOpt.fSize[SysOpt_nameLabelText]/*Logo_nFontSize*/, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}

void CMCICard::InitLabelLay_1440()
{
	/////////////////////////////////////////////////
	m_csHV				= _T("0.0 KV");				// 高压显示
	m_csMag				= _T("250 X");				// 放大倍数显示
	m_csUScal			= _T("100 μm");			// 固定标尺显示
	m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	m_csWD				= _T("12.0 mm");			// 工作距离显示
	m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	m_nUScalePixelNum	= 200;

	/////////////////////////////////////////////////
// 	m_ptHV				= CPoint( 64-16,		30);		// 高压显示位置
// 	m_ptMag				= CPoint(256,			30);		// 放大倍数显示位置
// 	m_ptWD				= CPoint(0, 30);
// 	m_ptSG				= CPoint(0, 30);
// 	m_ptUScal			= CPoint(640-64,		30);		// 固定标尺显示位置
// 	m_ptLogo			= CPoint(768-80,		30);		// 产品标识显示位置
// 	m_ptPhotoSN			= CPoint(1024+16,		30);		// 照相编号显示位置
	/////////////////////////////////////////////////
	/////////////////////////////////////////////////
	m_ptHV				= CPoint(24,	2);
	m_ptMag				= CPoint(340,	2);
	m_ptWD				= CPoint(24,	48);
	m_ptSG				= CPoint(340,	48);
	m_ptUScal			= CPoint(680,	2);
	m_ptLogo			= CPoint(680,	48);
	m_ptTime			= CPoint(940,	2);
	m_ptNote			= CPoint(940,	48);
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (m_hfontEditText != NULL)
        DeleteObject(m_hfontEditText);

    if (m_hfontLabelText != NULL)
        DeleteObject(m_hfontLabelText);

    if (m_hfontLabelLogoText != NULL)
        DeleteObject(m_hfontLabelLogoText);

    m_hfontEditText = CreateFont(
        40, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameEditText] );

    m_hfontLabelText = CreateFont(
        48, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );

    m_hfontLabelLogoText = CreateFont(
        48, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}

void CMCICard::InitLabelLay_2048()
{
	/////////////////////////////////////////////////
	m_csHV				= _T("0.0 KV");				// 高压显示
	m_csMag				= _T("250 X");				// 放大倍数显示
	m_csUScal			= _T("100 μm");			// 固定标尺显示
	m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	m_csWD				= _T("12.0 mm");			// 工作距离显示
	m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	m_nUScalePixelNum	= 200;

	/////////////////////////////////////////////////
// 	m_ptHV				= CPoint( 64-16,		30);		// 高压显示位置
// 	m_ptMag				= CPoint(384+16-16,		30);		// 放大倍数显示位置
// 	m_ptWD				= CPoint(0, 30);
// 	m_ptSG				= CPoint(0, 30);
// 	m_ptUScal			= CPoint(1024-64-64-96,	30);		// 固定标尺显示位置
// 	m_ptLogo			= CPoint(1280-64-128,	30);		// 产品标识显示位置
// 	m_ptPhotoSN			= CPoint(1792-32-16,	30);		// 照相编号显示位置
	/////////////////////////////////////////////////
	/////////////////////////////////////////////////
	m_ptHV				= CPoint(48,	5);
	m_ptMag				= CPoint(500,	5);
	m_ptWD				= CPoint(48,	64);
	m_ptSG				= CPoint(500,	64);
	m_ptUScal			= CPoint(1000,	5);
	m_ptLogo			= CPoint(1000,	64);
	m_ptTime			= CPoint(1500,	5);
	m_ptNote			= CPoint(1500,	64);
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (m_hfontEditText != NULL)
        DeleteObject(m_hfontEditText);

    if (m_hfontLabelText != NULL)
        DeleteObject(m_hfontLabelText);

    if (m_hfontLabelLogoText != NULL)
        DeleteObject(m_hfontLabelLogoText);

    m_hfontEditText = CreateFont(
        60, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameEditText] );

    m_hfontLabelText = CreateFont(
        64, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );

    m_hfontLabelLogoText = CreateFont(
        64, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}

void CMCICard::InitLabelLay_4096()
{
	/////////////////////////////////////////////////
	m_csHV				= _T("0.0 KV");				// 高压显示
	m_csMag				= _T("250 X");				// 放大倍数显示
	m_csUScal			= _T("100 μm");				// 固定标尺显示
	m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	m_csWD				= _T("12.0 mm");			// 工作距离显示
	m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	m_nUScalePixelNum	= 400;

	/////////////////////////////////////////////////
// 	m_ptHV				= CPoint( 128,			80);		// 高压显示位置
// 	m_ptMag				= CPoint(768+16-16,		80);		// 放大倍数显示位置
// 	m_ptWD				= CPoint(0, 80);
// 	m_ptSG				= CPoint(0, 80);
// 	m_ptUScal			= CPoint(2048-128-64-64,80);		// 固定标尺显示位置
// 	m_ptLogo			= CPoint(2560-128-64,	80);		// 产品标识显示位置
// 	m_ptPhotoSN			= CPoint(3584-64-64,	80);		// 照相编号显示位置
	/////////////////////////////////////////////////
	/////////////////////////////////////////////////
	m_ptHV				= CPoint(40,	5);
	m_ptMag				= CPoint(1000,	5);
	m_ptWD				= CPoint(40,	120);
	m_ptSG				= CPoint(1000,	120);
	m_ptUScal			= CPoint(2000,	5);
	m_ptLogo			= CPoint(2000,	120);
	m_ptTime			= CPoint(3000,	5);
	m_ptNote			= CPoint(3000,	120);
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (m_hfontEditText != NULL)
        DeleteObject(m_hfontEditText);

    if (m_hfontLabelText != NULL)
        DeleteObject(m_hfontLabelText);

    if (m_hfontLabelLogoText != NULL)
        DeleteObject(m_hfontLabelLogoText);

    m_hfontEditText = CreateFont(
        100, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameEditText] );

    m_hfontLabelText = CreateFont(
        120, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );

    m_hfontLabelLogoText = CreateFont(
        120, 0, 0, 0, c_fnWeight, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}

long CMCICard::RedrawLabelLay()
{
	HDC				hdc;
    HRESULT			ddrval;

	/////////////////////////////////////////////////////////////////
	if ( m_lpDDSLabelLay == NULL ) return DD_OK;
	ddrval = m_lpDDSLabelLay->GetDC( &hdc );
	if( ddrval != DD_OK )
		return ddrval;
	/////////////////////////////////////////////////////////////////
	COLORREF clrBack;
	if( m_sysOpt.nShowParams[SysOpt_showLabelTrans] )
		clrBack = g_clrUSB_Transparent;
	else
		clrBack	= m_sysOpt.color[SysOpt_clrBack];
	HBRUSH brBackBrush = CreateSolidBrush( clrBack );
	HPEN hPen, hPenOld;
	int nLineWidth;
	switch( m_sizeReso.cx )
	{
	case	512:
	case	1024:
		nLineWidth	= 1;
		break;
	case	2048:
		nLineWidth	= 5;
		break;
	case	4096:
		nLineWidth	= 10;
		break;
	default:
		nLineWidth	= 1;
		break;
	}
	hPen = CreatePen(PS_SOLID, nLineWidth, m_sysOpt.color[SysOpt_clrLabelText]);
	/////////////////////////////////////////////////////////////////

	CRect rcFill = CRect(0,0,m_szLabel.cx,m_szLabel.cy);
	FillRect( hdc, (LPRECT)rcFill, brBackBrush );
	int nOffsetX, nValidX;
	if( m_bSplit )
	{
		nOffsetX	= m_szGraph.cx /2;
		nValidX		= m_szGraph.cx /2;
	}
	else
	{
		nOffsetX	= 0;
		nValidX		= m_szGraph.cx;
	}

	/////////////////////////////////////////////////////////////////
	if(m_hfontLabelText != NULL)
	{
		SelectObject(hdc, m_hfontLabelText);
		SetTextColor(hdc, m_sysOpt.color[SysOpt_clrLabelText]);
		SetBkColor(hdc, clrBack);

		SetTextAlign(hdc, TA_TOP | TA_LEFT);
		TextOut(hdc, m_ptHV.x, m_ptHV.y, (LPCSTR)m_csHV, m_csHV.GetLength());
		TextOut(hdc, m_ptMag.x, m_ptMag.y, (LPCSTR)m_csMag, m_csMag.GetLength());
		TextOut(hdc, m_ptPhotoSN.x, m_ptPhotoSN.y, (LPCSTR)m_csPhotoSN, m_csPhotoSN.GetLength());
		TextOut(hdc, m_ptWD.x, m_ptWD.y, (LPCSTR)m_csWD, m_csWD.GetLength());
		TextOut(hdc, m_ptSG.x, m_ptSG.y, (LPCSTR)m_csSG1, m_csSG1.GetLength());
		if( m_bSplit )
		{
			TextOut(hdc, m_ptHV.x +nOffsetX, m_ptHV.y, (LPCSTR)m_csHV, m_csHV.GetLength());
			TextOut(hdc, m_ptMag.x +nOffsetX, m_ptMag.y, (LPCSTR)m_csMag, m_csMag.GetLength());
			TextOut(hdc, m_ptPhotoSN.x +nOffsetX, m_ptPhotoSN.y, (LPCSTR)m_csPhotoSN, m_csPhotoSN.GetLength());
			TextOut(hdc, m_ptWD.x +nOffsetX, m_ptWD.y, (LPCSTR)m_csWD, m_csWD.GetLength());
			TextOut(hdc, m_ptSG.x +nOffsetX, m_ptSG.y, (LPCSTR)m_csSG2, m_csSG2.GetLength());
		}

		SetTextAlign(hdc, TA_TOP | TA_CENTER);
		TextOut(hdc, m_ptUScal.x, m_ptUScal.y, (LPCSTR)m_csUScal, m_csUScal.GetLength());
		if( m_bSplit )
			TextOut(hdc, m_ptUScal.x +nOffsetX, m_ptUScal.y, (LPCSTR)m_csUScal, m_csUScal.GetLength());
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if(m_hfontLabelLogoText != NULL)
	{
		SelectObject(hdc, m_hfontLabelLogoText);
		SetTextColor(hdc, m_sysOpt.color[SysOpt_clrLabelLogoText]);
		SetBkColor(hdc, clrBack);

		SetTextAlign(hdc, TA_TOP | TA_LEFT);
		TextOut(hdc, m_ptLogo.x, m_ptLogo.y, (LPCSTR)m_csLogo, m_csLogo.GetLength());
		if( m_bSplit )
			TextOut(hdc, m_ptLogo.x +nOffsetX, m_ptLogo.y, (LPCSTR)m_csLogo, m_csLogo.GetLength());
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	hPenOld = (HPEN)SelectObject(hdc, hPen);

	CPoint ptPos0(1,12);
	CPoint ptPos1(1,12);
	switch( m_sizeReso.cx )
	{
	case	128:
		ptPos0.y = 5;
		ptPos1.y = 5;
		break;
	case	256:
		ptPos0.y = 10;
		ptPos1.y = 10;
		break;
	case	512:
	case	1024:
		ptPos0.y = 12;
		ptPos1.y = 12;
		break;
	case	2048:
		ptPos0.y = 24;
		ptPos1.y = 24;
		break;
	case	4096:
		ptPos0.y = 48;
		ptPos1.y = 48;
		break;
	default:
		ptPos0.y = 10;
		ptPos1.y = 10;
		break;
	}

	long nStart = (long)( (nValidX - m_nUScalePixelNum) / 2.0 + 0.5);
	ptPos0.x = nStart;
	ptPos1.x = ptPos0.x + m_nUScalePixelNum;

#ifdef GRAPHIC_BLACK
	MoveToEx ( hdc, ptPos0.x, ptPos0.y-2, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y-2 );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y-1, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y-1 );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y+1, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y+1 );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y+2, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y+2 );

	MoveToEx ( hdc, ptPos0.x-1, ptPos0.y-6, NULL );
	LineTo ( hdc, ptPos0.x-1, ptPos0.y+7 );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y-6, NULL );
	LineTo ( hdc, ptPos0.x, ptPos0.y+7 );
	MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-6, NULL );
	LineTo ( hdc, ptPos0.x+1, ptPos0.y+7 );

	MoveToEx ( hdc, ptPos1.x-1, ptPos1.y-6, NULL );
	LineTo ( hdc, ptPos1.x-1, ptPos1.y+7 );
	MoveToEx ( hdc, ptPos1.x, ptPos1.y-6, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y+7 );
	MoveToEx ( hdc, ptPos1.x+1, ptPos1.y-6, NULL );
	LineTo ( hdc, ptPos1.x+1, ptPos1.y+7 );

	if( m_bSplit )
	{
		ptPos0.x += nOffsetX;
		ptPos1.x += nOffsetX;

		MoveToEx ( hdc, ptPos0.x, ptPos0.y-2, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y-2 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y-1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y-1 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y+1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+1 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y+2, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+2 );

		MoveToEx ( hdc, ptPos0.x-1, ptPos0.y-6, NULL );
		LineTo ( hdc, ptPos0.x-1, ptPos0.y+7 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y-6, NULL );
		LineTo ( hdc, ptPos0.x, ptPos0.y+7 );
		MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-6, NULL );
		LineTo ( hdc, ptPos0.x+1, ptPos0.y+7 );

		MoveToEx ( hdc, ptPos1.x-1, ptPos1.y-6, NULL );
		LineTo ( hdc, ptPos1.x-1, ptPos1.y+7 );
		MoveToEx ( hdc, ptPos1.x, ptPos1.y-6, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+7 );
		MoveToEx ( hdc, ptPos1.x+1, ptPos1.y-6, NULL );
		LineTo ( hdc, ptPos1.x+1, ptPos1.y+7 );
	}

#else
	MoveToEx ( hdc, ptPos0.x, ptPos0.y-1, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y-1 );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y+1, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y+1 );

	MoveToEx ( hdc, ptPos0.x, ptPos0.y-3, NULL );
	LineTo ( hdc, ptPos0.x, ptPos0.y+4 );
	MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-3, NULL );
	LineTo ( hdc, ptPos0.x+1, ptPos0.y+4 );

	MoveToEx ( hdc, ptPos1.x, ptPos1.y-3, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y+4 );
	MoveToEx ( hdc, ptPos1.x+1, ptPos1.y-3, NULL );
	LineTo ( hdc, ptPos1.x+1, ptPos1.y+4 );

	if( m_bSplit )
	{
		ptPos0.x += nOffsetX;
		ptPos1.x += nOffsetX;

		MoveToEx ( hdc, ptPos0.x, ptPos0.y-1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y-1 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y+1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+1 );

		MoveToEx ( hdc, ptPos0.x, ptPos0.y-3, NULL );
		LineTo ( hdc, ptPos0.x, ptPos0.y+4 );
		MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-3, NULL );
		LineTo ( hdc, ptPos0.x+1, ptPos0.y+4 );

		MoveToEx ( hdc, ptPos1.x, ptPos1.y-3, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+4 );
		MoveToEx ( hdc, ptPos1.x+1, ptPos1.y-3, NULL );
		LineTo ( hdc, ptPos1.x+1, ptPos1.y+4 );
	}
#endif

	double dblStep = m_nUScalePixelNum / 5.0;
	for (int i=1; i<5; i++)
	{
#ifdef GRAPHIC_BLACK
		ptPos0.x = (int)(nStart + dblStep * i + 0.5);

		MoveToEx ( hdc, ptPos0.x-1, ptPos0.y-5, NULL );
		LineTo ( hdc, ptPos0.x-1, ptPos0.y+1 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y-5, NULL );
		LineTo ( hdc, ptPos0.x, ptPos0.y+1 );
		MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-5, NULL );
		LineTo ( hdc, ptPos0.x+1, ptPos0.y+1 );
#else
		ptPos0.x = (int)(nStart + dblStep * i + 0.5);

		MoveToEx ( hdc, ptPos0.x, ptPos0.y-2, NULL );
		LineTo ( hdc, ptPos0.x, ptPos0.y );
		MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-2, NULL );
		LineTo ( hdc, ptPos0.x+1, ptPos0.y );
#endif
	}
	if( m_bSplit )
	{
		for (int i=1; i<5; i++)
		{
#ifdef GRAPHIC_BLACK
			ptPos0.x = (int)(nStart + dblStep * i + 0.5) +nOffsetX;

			MoveToEx ( hdc, ptPos0.x-1, ptPos0.y-5, NULL );
			LineTo ( hdc, ptPos0.x-1, ptPos0.y+1 );
			MoveToEx ( hdc, ptPos0.x, ptPos0.y-5, NULL );
			LineTo ( hdc, ptPos0.x, ptPos0.y+1 );
			MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-5, NULL );
			LineTo ( hdc, ptPos0.x+1, ptPos0.y+1 );
#else
			ptPos0.x = (int)(nStart + dblStep * i + 0.5) +nOffsetX;

			MoveToEx ( hdc, ptPos0.x, ptPos0.y-2, NULL );
			LineTo ( hdc, ptPos0.x, ptPos0.y );
			MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-2, NULL );
			LineTo ( hdc, ptPos0.x+1, ptPos0.y );
#endif
		}
	}

	SelectObject(hdc, hPenOld);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	DeleteObject( hPen );
	DeleteObject( brBackBrush );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( m_lpDDSLabelLay == NULL ) return DD_OK;
	m_lpDDSLabelLay->ReleaseDC(hdc);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( m_boolShowLabel )
	{
		CRect rcGrapic = CRect(0, m_szGraph.cy - m_szLabel.cy,
			m_szGraph.cx, m_szGraph.cy);
		CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );

		m_UpdateRectsImage.Add( rcImage );
	}
	/////////////////////////////////////////////////////////////////

	return DD_OK;
}

long CMCICard::RedrawLabelLay2()
{
	HDC				hdc;
    HRESULT			ddrval;

	/////////////////////////////////////////////////////////////////
	if ( m_lpDDSLabelLay == NULL ) return DD_OK;
	ddrval = m_lpDDSLabelLay->GetDC( &hdc );
	if( ddrval != DD_OK )
		return ddrval;
	/////////////////////////////////////////////////////////////////
	COLORREF clrBack;
	if( m_sysOpt.nShowParams[SysOpt_showLabelTrans] )
		clrBack = g_clrUSB_Transparent;
	else
		clrBack	= m_sysOpt.color[SysOpt_clrBack];
	HBRUSH brBackBrush = CreateSolidBrush( clrBack );
	HPEN hPen, hPenOld;
	int nLineWidth, nOffsetY;
	switch( m_sizeReso.cx )
	{
	case	256:
	case	384:
	case	480:
	case	512:
		nOffsetY	= 5;
		nLineWidth	= 1;
		break;
	case	600:
	case	640:
	case	768:
	case	800:
		nOffsetY	= 8;
		nLineWidth	= 1;
		break;
	case	1024:
		nLineWidth	= 1;
		nOffsetY	= 20;
		break;
	case	1280:
	case	1440:
	case	1536:
		nLineWidth	= 2;
		nOffsetY	= 20;
		break;
	case	2048:
		nLineWidth	= 5;
		nOffsetY	= 20;
		break;
	case	4096:
		nLineWidth	= 10;
		nOffsetY	= 20;
		break;
	default:
		nLineWidth	= 1;
		break;
	}
	hPen = CreatePen(PS_SOLID, nLineWidth, m_sysOpt.color[SysOpt_clrLabelText]);
	/////////////////////////////////////////////////////////////////

	CRect rcFill = CRect(0,0,m_szLabel.cx,m_szLabel.cy);
	FillRect( hdc, (LPRECT)rcFill, brBackBrush );
	int nOffsetX, nValidX;
	if( m_bSplit )
	{
		nOffsetX	= m_szGraph.cx /2;
		nValidX		= m_szGraph.cx /2 /4;
	}
	else
	{
		nOffsetX	= 0;
		nValidX		= m_szGraph.cx /4;
	}

	/////////////////////////////////////////////////////////////////
	hPenOld = (HPEN)SelectObject(hdc, hPen);

	CPoint ptPos0(1,12);
	CPoint ptPos1(1,12);
	CPoint ptPosBak(1,12);
	switch( m_sizeReso.cx )
	{
	case	128:
		ptPos0.y = 5;
		ptPos1.y = 5;
		break;
	case	256:
		ptPos0.y = 10;
		ptPos1.y = 10;
		break;
	case	512:
	case	1024:
		ptPos0.y = 12;
		ptPos1.y = 12;
		break;
	case	2048:
		ptPos0.y = 24;
		ptPos1.y = 24;
		break;
	case	4096:
		ptPos0.y = 48;
		ptPos1.y = 48;
		break;
	default:
		ptPos0.y = 10;
		ptPos1.y = 10;
		break;
	}

//	long nStart = m_sizeReso.cx -nValidX -20 +(long)( (nValidX - m_nUScalePixelNum) / 2.0 + 0.5);
	long nStart = m_sizeReso.cx -m_nUScalePixelNum -nOffsetY;
	ptPos0.x = nStart;
	ptPos1.x = ptPos0.x + m_nUScalePixelNum;
	ptPosBak = ptPos0;

#ifdef GRAPHIC_BLACK
	MoveToEx ( hdc, ptPos0.x, ptPos0.y-2, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y-2 );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y-1, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y-1 );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y+1, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y+1 );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y+2, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y+2 );

	MoveToEx ( hdc, ptPos0.x-1, ptPos0.y-6, NULL );
	LineTo ( hdc, ptPos0.x-1, ptPos0.y+7 );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y-6, NULL );
	LineTo ( hdc, ptPos0.x, ptPos0.y+7 );
	MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-6, NULL );
	LineTo ( hdc, ptPos0.x+1, ptPos0.y+7 );

	MoveToEx ( hdc, ptPos1.x-1, ptPos1.y-6, NULL );
	LineTo ( hdc, ptPos1.x-1, ptPos1.y+7 );
	MoveToEx ( hdc, ptPos1.x, ptPos1.y-6, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y+7 );
	MoveToEx ( hdc, ptPos1.x+1, ptPos1.y-6, NULL );
	LineTo ( hdc, ptPos1.x+1, ptPos1.y+7 );

	if( m_bSplit )
	{
		ptPos0.x += nOffsetX;
		ptPos1.x += nOffsetX;

		MoveToEx ( hdc, ptPos0.x, ptPos0.y-2, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y-2 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y-1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y-1 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y+1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+1 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y+2, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+2 );

		MoveToEx ( hdc, ptPos0.x-1, ptPos0.y-6, NULL );
		LineTo ( hdc, ptPos0.x-1, ptPos0.y+7 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y-6, NULL );
		LineTo ( hdc, ptPos0.x, ptPos0.y+7 );
		MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-6, NULL );
		LineTo ( hdc, ptPos0.x+1, ptPos0.y+7 );

		MoveToEx ( hdc, ptPos1.x-1, ptPos1.y-6, NULL );
		LineTo ( hdc, ptPos1.x-1, ptPos1.y+7 );
		MoveToEx ( hdc, ptPos1.x, ptPos1.y-6, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+7 );
		MoveToEx ( hdc, ptPos1.x+1, ptPos1.y-6, NULL );
		LineTo ( hdc, ptPos1.x+1, ptPos1.y+7 );
	}

#else
	MoveToEx ( hdc, ptPos0.x, ptPos0.y-1, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y-1 );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y+1, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y+1 );

	MoveToEx ( hdc, ptPos0.x, ptPos0.y-3, NULL );
	LineTo ( hdc, ptPos0.x, ptPos0.y+4 );
	MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-3, NULL );
	LineTo ( hdc, ptPos0.x+1, ptPos0.y+4 );

	MoveToEx ( hdc, ptPos1.x, ptPos1.y-3, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y+4 );
	MoveToEx ( hdc, ptPos1.x+1, ptPos1.y-3, NULL );
	LineTo ( hdc, ptPos1.x+1, ptPos1.y+4 );

	if( m_bSplit )
	{
		ptPos0.x += nOffsetX;
		ptPos1.x += nOffsetX;

		MoveToEx ( hdc, ptPos0.x, ptPos0.y-1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y-1 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y+1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+1 );

		MoveToEx ( hdc, ptPos0.x, ptPos0.y-3, NULL );
		LineTo ( hdc, ptPos0.x, ptPos0.y+4 );
		MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-3, NULL );
		LineTo ( hdc, ptPos0.x+1, ptPos0.y+4 );

		MoveToEx ( hdc, ptPos1.x, ptPos1.y-3, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+4 );
		MoveToEx ( hdc, ptPos1.x+1, ptPos1.y-3, NULL );
		LineTo ( hdc, ptPos1.x+1, ptPos1.y+4 );
	}
#endif

	double dblStep = m_nUScalePixelNum / 5.0;
	for (int i=1; i<5; i++)
	{
#ifdef GRAPHIC_BLACK
		ptPos0.x = (int)(nStart + dblStep * i + 0.5);

		MoveToEx ( hdc, ptPos0.x-1, ptPos0.y-5, NULL );
		LineTo ( hdc, ptPos0.x-1, ptPos0.y+1 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y-5, NULL );
		LineTo ( hdc, ptPos0.x, ptPos0.y+1 );
		MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-5, NULL );
		LineTo ( hdc, ptPos0.x+1, ptPos0.y+1 );
#else
		ptPos0.x = (int)(nStart + dblStep * i + 0.5);

		MoveToEx ( hdc, ptPos0.x, ptPos0.y-2, NULL );
		LineTo ( hdc, ptPos0.x, ptPos0.y );
		MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-2, NULL );
		LineTo ( hdc, ptPos0.x+1, ptPos0.y );
#endif
	}
	if( m_bSplit )
	{
		for (int i=1; i<5; i++)
		{
#ifdef GRAPHIC_BLACK
			ptPos0.x = (int)(nStart + dblStep * i + 0.5) +nOffsetX;

			MoveToEx ( hdc, ptPos0.x-1, ptPos0.y-5, NULL );
			LineTo ( hdc, ptPos0.x-1, ptPos0.y+1 );
			MoveToEx ( hdc, ptPos0.x, ptPos0.y-5, NULL );
			LineTo ( hdc, ptPos0.x, ptPos0.y+1 );
			MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-5, NULL );
			LineTo ( hdc, ptPos0.x+1, ptPos0.y+1 );
#else
			ptPos0.x = (int)(nStart + dblStep * i + 0.5) +nOffsetX;

			MoveToEx ( hdc, ptPos0.x, ptPos0.y-2, NULL );
			LineTo ( hdc, ptPos0.x, ptPos0.y );
			MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-2, NULL );
			LineTo ( hdc, ptPos0.x+1, ptPos0.y );
#endif
		}
	}
	SelectObject(hdc, hPenOld);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	ptPos0.x -= nOffsetX;
	SetTextAlign(hdc, TA_TOP | TA_LEFT);
	SIZE szText;
/*	if(m_hfontLabelText != NULL)
	{
		SelectObject(hdc, m_hfontLabelText);
		SetTextColor(hdc, m_sysOpt.color[SysOpt_clrLabelText]);
		SetBkColor(hdc, clrBack);

		TextOut(hdc, m_ptHV.x, m_ptHV.y, (LPCSTR)m_csHV, m_csHV.GetLength());
		TextOut(hdc, m_ptMag.x, m_ptMag.y, (LPCSTR)m_csMag, m_csMag.GetLength());
		TextOut(hdc, m_ptWD.x, m_ptWD.y, (LPCSTR)m_csWD, m_csWD.GetLength());
		TextOut(hdc, m_ptSG.x, m_ptSG.y, (LPCSTR)m_csSG1, m_csSG1.GetLength());
//		TextOut(hdc, m_ptPhotoSN.x, m_ptPhotoSN.y, (LPCSTR)m_csPhotoSN, m_csPhotoSN.GetLength());
		GetTextExtentPoint( hdc, (LPCSTR)m_csUScal, m_csUScal.GetLength(), &szText );
//		TextOut(hdc, ptPosBak.x +(m_nUScalePixelNum -szText.cx)/2, m_ptUScal.y, (LPCSTR)m_csUScal, m_csUScal.GetLength());
		TextOut(hdc, m_sizeReso.cx -szText.cx -20, m_ptUScal.y, (LPCSTR)m_csUScal, m_csUScal.GetLength());
		GetTextExtentPoint( hdc, (LPCSTR)m_csSG1, m_csSG1.GetLength(), &szText );
		ptPos0.x = m_ptSG.x +szText.cx +20;
		TextOut(hdc, ptPos0.x, m_ptPhotoSN.y, (LPCSTR)m_csPhotoSN, m_csPhotoSN.GetLength());

		if( m_bSplit )
		{
			TextOut(hdc, m_ptHV.x +nOffsetX, m_ptHV.y, (LPCSTR)m_csHV, m_csHV.GetLength());
			TextOut(hdc, m_ptMag.x +nOffsetX, m_ptMag.y, (LPCSTR)m_csMag, m_csMag.GetLength());
			TextOut(hdc, m_ptWD.x +nOffsetX, m_ptWD.y, (LPCSTR)m_csWD, m_csWD.GetLength());
			TextOut(hdc, m_ptSG.x +nOffsetX, m_ptSG.y, (LPCSTR)m_csSG2, m_csSG2.GetLength());
//			TextOut(hdc, m_ptPhotoSN.x +nOffsetX, m_ptPhotoSN.y, (LPCSTR)m_csPhotoSN, m_csPhotoSN.GetLength());
			GetTextExtentPoint( hdc, (LPCSTR)m_csUScal, m_csUScal.GetLength(), &szText );
//			TextOut(hdc, ptPosBak.x +(m_nUScalePixelNum -szText.cx)/2 +nOffsetX, m_ptUScal.y, (LPCSTR)m_csUScal, m_csUScal.GetLength());
			TextOut(hdc, m_sizeReso.cx -szText.cx -20 +nOffsetX, m_ptUScal.y, (LPCSTR)m_csUScal, m_csUScal.GetLength());
			GetTextExtentPoint( hdc, (LPCSTR)m_csSG2, m_csSG2.GetLength(), &szText );
			ptPos1.x = m_ptSG.x +szText.cx +20;
			TextOut(hdc, ptPos1.x +nOffsetX, m_ptPhotoSN.y, (LPCSTR)m_csPhotoSN, m_csPhotoSN.GetLength());
		}

//		TextOut(hdc, m_ptUScal.x, m_ptUScal.y, (LPCSTR)m_csUScal, m_csUScal.GetLength());
//		if( m_bSplit )
//			TextOut(hdc, m_ptUScal.x +nOffsetX, m_ptUScal.y, (LPCSTR)m_csUScal, m_csUScal.GetLength());
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if(m_hfontLabelLogoText != NULL)
	{
		SelectObject(hdc, m_hfontLabelLogoText);
		SetTextColor(hdc, m_sysOpt.color[SysOpt_clrLabelLogoText]);
		SetBkColor(hdc, clrBack);

// 		TextOut(hdc, m_ptLogo.x, m_ptLogo.y, (LPCSTR)m_csLogo, m_csLogo.GetLength());
// 		if( m_bSplit )
// 			TextOut(hdc, m_ptLogo.x +nOffsetX, m_ptLogo.y, (LPCSTR)m_csLogo, m_csLogo.GetLength());
 		GetTextExtentPoint( hdc, (LPCSTR)m_csPhotoSN, m_csPhotoSN.GetLength(), &szText );
		TextOut(hdc, ptPos0.x +szText.cx +20, m_ptLogo.y, (LPCSTR)m_csLogo, m_csLogo.GetLength());
		if( m_bSplit )
			TextOut(hdc, ptPos1.x +szText.cx +20 +nOffsetX, m_ptLogo.y, (LPCSTR)m_csLogo, m_csLogo.GetLength());
// 		GetTextExtentPoint( hdc, (LPCSTR)m_csLogo, m_csLogo.GetLength(), &szText );
// 		TextOut(hdc, ptPosBak.x +(m_nUScalePixelNum -szText.cx)/2, m_ptLogo.y +10, (LPCSTR)m_csLogo, m_csLogo.GetLength());
// 		if( m_bSplit )
// 			TextOut(hdc, ptPosBak.x +(m_nUScalePixelNum -szText.cx)/2 +nOffsetX, m_ptLogo.y +10, (LPCSTR)m_csLogo, m_csLogo.GetLength());
	}
	/////////////////////////////////////////////////////////////////
*/
	if(m_hfontLabelText != NULL)
	{
		SelectObject(hdc, m_hfontLabelText);
		SetTextColor(hdc, m_sysOpt.color[SysOpt_clrLabelText]);
		SetBkColor(hdc, clrBack);

		TextOut(hdc, m_ptHV.x, m_ptHV.y, (LPCSTR)m_csHV, m_csHV.GetLength());

		GetTextExtentPoint( hdc, (LPCSTR)m_csHV, m_csHV.GetLength(), &szText );
		m_ptMag.x = m_ptHV.x +szText.cx +nOffsetY;
		TextOut(hdc, m_ptMag.x, m_ptMag.y, (LPCSTR)m_csMag, m_csMag.GetLength());

		GetTextExtentPoint( hdc, (LPCSTR)m_csMag, m_csMag.GetLength(), &szText );
		m_ptWD.x = m_ptMag.x +szText.cx +nOffsetY;
		TextOut(hdc, m_ptWD.x, m_ptWD.y, (LPCSTR)m_csWD, m_csWD.GetLength());

		GetTextExtentPoint( hdc, (LPCSTR)m_csWD, m_csWD.GetLength(), &szText );
		m_ptSG.x = m_ptWD.x +szText.cx +nOffsetY;
		if( m_bDet2nd )	// 只看第二通道时
		{
			TextOut(hdc, m_ptSG.x, m_ptSG.y, (LPCSTR)m_csSG2, m_csSG2.GetLength());
			GetTextExtentPoint( hdc, (LPCSTR)m_csSG2, m_csSG2.GetLength(), &szText );
		}
		else
		{
			TextOut(hdc, m_ptSG.x, m_ptSG.y, (LPCSTR)m_csSG1, m_csSG1.GetLength());
			GetTextExtentPoint( hdc, (LPCSTR)m_csSG1, m_csSG1.GetLength(), &szText );
		}
		m_ptLogo.x = m_ptSG.x +szText.cx +nOffsetY;

		GetTextExtentPoint( hdc, (LPCSTR)m_csLogo, m_csLogo.GetLength(), &szText );
		m_ptPhotoSN.x = m_ptLogo.x +szText.cx +nOffsetY;
		TextOut(hdc, m_ptPhotoSN.x, m_ptPhotoSN.y, (LPCSTR)m_csPhotoSN, m_csPhotoSN.GetLength());

		GetTextExtentPoint( hdc, (LPCSTR)m_csUScal, m_csUScal.GetLength(), &szText );
		m_ptUScal.x = m_sizeReso.cx -szText.cx -nOffsetY;
		TextOut(hdc, m_ptUScal.x, m_ptUScal.y, (LPCSTR)m_csUScal, m_csUScal.GetLength());

		if( m_bSplit )
		{
			TextOut(hdc, m_ptHV.x +nOffsetX, m_ptHV.y, (LPCSTR)m_csHV, m_csHV.GetLength());
			TextOut(hdc, m_ptMag.x +nOffsetX, m_ptMag.y, (LPCSTR)m_csMag, m_csMag.GetLength());
			TextOut(hdc, m_ptWD.x +nOffsetX, m_ptWD.y, (LPCSTR)m_csWD, m_csWD.GetLength());
			TextOut(hdc, m_ptSG.x +nOffsetX, m_ptSG.y, (LPCSTR)m_csSG2, m_csSG2.GetLength());

			GetTextExtentPoint( hdc, (LPCSTR)m_csSG2, m_csSG2.GetLength(), &szText );
			ptPos0.x = m_ptSG.x +szText.cx +nOffsetY;	// m_ptLogo.x

			GetTextExtentPoint( hdc, (LPCSTR)m_csLogo, m_csLogo.GetLength(), &szText );
			TextOut(hdc, ptPos0.x +szText.cx +nOffsetY +nOffsetX, m_ptPhotoSN.y, (LPCSTR)m_csPhotoSN, m_csPhotoSN.GetLength());

			GetTextExtentPoint( hdc, (LPCSTR)m_csUScal, m_csUScal.GetLength(), &szText );
			TextOut(hdc, m_ptUScal.x +nOffsetX, m_ptUScal.y, (LPCSTR)m_csUScal, m_csUScal.GetLength());
		}
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if(m_hfontLabelLogoText != NULL)
	{
		SelectObject(hdc, m_hfontLabelLogoText);
		SetTextColor(hdc, m_sysOpt.color[SysOpt_clrLabelLogoText]);
		SetBkColor(hdc, clrBack);

		TextOut(hdc, m_ptLogo.x, m_ptLogo.y, (LPCSTR)m_csLogo, m_csLogo.GetLength());
		if( m_bSplit )
			TextOut(hdc, ptPos0.x +nOffsetX, m_ptLogo.y, (LPCSTR)m_csLogo, m_csLogo.GetLength());
	}
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	DeleteObject( hPen );
	DeleteObject( brBackBrush );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( m_lpDDSLabelLay == NULL ) return DD_OK;
	m_lpDDSLabelLay->ReleaseDC(hdc);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( m_boolShowLabel )
	{
		CRect rcGrapic = CRect(0, m_szGraph.cy - m_szLabel.cy,
			m_szGraph.cx, m_szGraph.cy);
		CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );

		m_UpdateRectsImage.Add( rcImage );
	}
	/////////////////////////////////////////////////////////////////

	return DD_OK;
}

long CMCICard::RedrawLabelLay3()
{
	HDC				hdc;
    HRESULT			ddrval;

	/////////////////////////////////////////////////////////////////
	if ( m_lpDDSLabelLay == NULL ) return DD_OK;
	ddrval = m_lpDDSLabelLay->GetDC( &hdc );
	if( ddrval != DD_OK )
		return ddrval;
	/////////////////////////////////////////////////////////////////
	COLORREF clrBack;
	if( m_sysOpt.nShowParams[SysOpt_showLabelTrans] )
		clrBack = g_clrUSB_Transparent;
	else
		clrBack	= m_sysOpt.color[SysOpt_clrBack];
	HBRUSH brBackBrush = CreateSolidBrush( clrBack );
	HPEN hPen, hPenOld;
	int nLineWidth, nOffsetY;
	switch( m_sizeReso.cx )
	{
	case	256:
	case	384:
	case	480:
	case	512:
		nOffsetY	= 5;
		nLineWidth	= 1;
		break;
	case	600:
	case	640:
	case	768:
	case	800:
		nOffsetY	= 8;
		nLineWidth	= 1;
		break;
	case	1024:
		nLineWidth	= 1;
		nOffsetY	= 16;
		break;
	case	1280:
	case	1440:
	case	1536:
		nLineWidth	= 2;
		nOffsetY	= 20;
		break;
	case	2048:
		nLineWidth	= 5;
		nOffsetY	= 20;
		break;
	case	4096:
		nLineWidth	= 10;
		nOffsetY	= 20;
		break;
	default:
		nLineWidth	= 1;
		break;
	}
	hPen = CreatePen(PS_SOLID, nLineWidth, m_sysOpt.color[SysOpt_clrLabelText]);
	/////////////////////////////////////////////////////////////////

	CRect rcFill = CRect(0,0,m_szLabel.cx,m_szLabel.cy);
	FillRect( hdc, (LPRECT)rcFill, brBackBrush );
	int nOffsetX, nValidX;
	if( m_bSplit )
	{
		nOffsetX	= m_szGraph.cx /2;
		nValidX		= m_szGraph.cx /2 /4;
	}
	else
	{
		nOffsetX	= 0;
		nValidX		= m_szGraph.cx /4;
	}

	/////////////////////////////////////////////////////////////////
	hPenOld = (HPEN)SelectObject(hdc, hPen);

	CPoint ptPos0(1,12);
	CPoint ptPos1(1,12);
	CPoint ptPosBak(1,12);
	switch( m_sizeReso.cx )
	{
	case	128:
		ptPos0.y = 5;
		ptPos1.y = 5;
		break;
	case	256:
		ptPos0.y = 10;
		ptPos1.y = 10;
		break;
	case	512:
	case	1024:
		ptPos0.y = 16;
		ptPos1.y = 16;
		break;
	case	2048:
		ptPos0.y = 24;
		ptPos1.y = 24;
		break;
	case	4096:
		ptPos0.y = 48;
		ptPos1.y = 48;
		break;
	default:
		ptPos0.y = 10;
		ptPos1.y = 10;
		break;
	}

//	long nStart = m_sizeReso.cx -nValidX -20 +(long)( (nValidX - m_nUScalePixelNum) / 2.0 + 0.5);
	long nStart = m_ptUScal.x;	//m_sizeReso.cx -m_nUScalePixelNum -nOffsetY;
	ptPos0.x = nStart;
	ptPos1.x = ptPos0.x + m_nUScalePixelNum;
	ptPosBak = ptPos0;

#ifdef GRAPHIC_BLACK
//	MoveToEx ( hdc, ptPos0.x, ptPos0.y-2, NULL );
//	LineTo ( hdc, ptPos1.x, ptPos1.y-2 );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y-1, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y-1 );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y+1, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y+1 );
//	MoveToEx ( hdc, ptPos0.x, ptPos0.y+2, NULL );
//	LineTo ( hdc, ptPos1.x, ptPos1.y+2 );

	MoveToEx ( hdc, ptPos0.x-1, ptPos0.y-6, NULL );
	LineTo ( hdc, ptPos0.x-1, ptPos0.y+7 );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y-6, NULL );
	LineTo ( hdc, ptPos0.x, ptPos0.y+7 );
//	MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-6, NULL );
//	LineTo ( hdc, ptPos0.x+1, ptPos0.y+7 );

//	MoveToEx ( hdc, ptPos1.x-1, ptPos1.y-6, NULL );
//	LineTo ( hdc, ptPos1.x-1, ptPos1.y+7 );
	MoveToEx ( hdc, ptPos1.x, ptPos1.y-6, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y+7 );
	MoveToEx ( hdc, ptPos1.x+1, ptPos1.y-6, NULL );
	LineTo ( hdc, ptPos1.x+1, ptPos1.y+7 );

	if( m_bSplit )
	{
		ptPos0.x += nOffsetX;
		ptPos1.x += nOffsetX;

//		MoveToEx ( hdc, ptPos0.x, ptPos0.y-2, NULL );
//		LineTo ( hdc, ptPos1.x, ptPos1.y-2 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y-1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y-1 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y+1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+1 );
//		MoveToEx ( hdc, ptPos0.x, ptPos0.y+2, NULL );
//		LineTo ( hdc, ptPos1.x, ptPos1.y+2 );

		MoveToEx ( hdc, ptPos0.x-1, ptPos0.y-6, NULL );
		LineTo ( hdc, ptPos0.x-1, ptPos0.y+7 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y-6, NULL );
		LineTo ( hdc, ptPos0.x, ptPos0.y+7 );
//		MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-6, NULL );
//		LineTo ( hdc, ptPos0.x+1, ptPos0.y+7 );

//		MoveToEx ( hdc, ptPos1.x-1, ptPos1.y-6, NULL );
//		LineTo ( hdc, ptPos1.x-1, ptPos1.y+7 );
		MoveToEx ( hdc, ptPos1.x, ptPos1.y-6, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+7 );
		MoveToEx ( hdc, ptPos1.x+1, ptPos1.y-6, NULL );
		LineTo ( hdc, ptPos1.x+1, ptPos1.y+7 );
	}

#else
	MoveToEx ( hdc, ptPos0.x, ptPos0.y-1, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y-1 );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y );
	MoveToEx ( hdc, ptPos0.x, ptPos0.y+1, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y+1 );

	MoveToEx ( hdc, ptPos0.x, ptPos0.y-3, NULL );
	LineTo ( hdc, ptPos0.x, ptPos0.y+4 );
	MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-3, NULL );
	LineTo ( hdc, ptPos0.x+1, ptPos0.y+4 );

	MoveToEx ( hdc, ptPos1.x, ptPos1.y-3, NULL );
	LineTo ( hdc, ptPos1.x, ptPos1.y+4 );
	MoveToEx ( hdc, ptPos1.x+1, ptPos1.y-3, NULL );
	LineTo ( hdc, ptPos1.x+1, ptPos1.y+4 );

	if( m_bSplit )
	{
		ptPos0.x += nOffsetX;
		ptPos1.x += nOffsetX;

		MoveToEx ( hdc, ptPos0.x, ptPos0.y-1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y-1 );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y );
		MoveToEx ( hdc, ptPos0.x, ptPos0.y+1, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+1 );

		MoveToEx ( hdc, ptPos0.x, ptPos0.y-3, NULL );
		LineTo ( hdc, ptPos0.x, ptPos0.y+4 );
		MoveToEx ( hdc, ptPos0.x+1, ptPos0.y-3, NULL );
		LineTo ( hdc, ptPos0.x+1, ptPos0.y+4 );

		MoveToEx ( hdc, ptPos1.x, ptPos1.y-3, NULL );
		LineTo ( hdc, ptPos1.x, ptPos1.y+4 );
		MoveToEx ( hdc, ptPos1.x+1, ptPos1.y-3, NULL );
		LineTo ( hdc, ptPos1.x+1, ptPos1.y+4 );
	}
#endif

	SelectObject(hdc, hPenOld);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	ptPos0.x -= nOffsetX;
	SetTextAlign(hdc, TA_TOP | TA_LEFT);
	SIZE szText;
	CString str;
	if(m_hfontLabelText != NULL)
	{
		SelectObject(hdc, m_hfontLabelText);
		SetTextColor(hdc, m_sysOpt.color[SysOpt_clrLabelText]);
		SetBkColor(hdc, clrBack);

		str.Format( "HV:%s", m_csHV );
		TextOut(hdc, m_ptHV.x, m_ptHV.y, (LPCSTR)str, str.GetLength());		//m_csHV, m_csHV.GetLength());
		str.Format( "MAG:%s", m_csMag );
		TextOut(hdc, m_ptMag.x, m_ptMag.y, (LPCSTR)str, str.GetLength());	//m_csMag, m_csMag.GetLength());
		str.Format( "WD:%s", m_csWD );
		TextOut(hdc, m_ptWD.x, m_ptWD.y, (LPCSTR)str, str.GetLength());		//m_csWD, m_csWD.GetLength());

		if( m_bDet2nd )	// 只看第二通道时
		{
			str.Format( "DET:%s", m_csSG2 );
			TextOut(hdc, m_ptSG.x, m_ptSG.y, (LPCSTR)str, str.GetLength());	//m_csSG2, m_csSG2.GetLength());
		}
		else
		{
			str.Format( "DET:%s", m_csSG1 );
			TextOut(hdc, m_ptSG.x, m_ptSG.y, (LPCSTR)str, str.GetLength());	//m_csSG1, m_csSG1.GetLength());
		}
		GetTextExtentPoint( hdc, (LPCSTR)m_csUScal, m_csUScal.GetLength(), &szText );
		TextOut(hdc, m_ptUScal.x +(m_nUScalePixelNum -szText.cx)/2, m_ptUScal.y, (LPCSTR)m_csUScal, m_csUScal.GetLength());

		if( m_boolShowTime )
		{
			str = "XXXXXXXXXXXXXXXX";
			GetTextExtentPoint( hdc, (LPCSTR)str, str.GetLength(), &szText );
			m_ptTime.x = m_sizeReso.cx -szText.cx -nOffsetY;
			TextOut(hdc, m_ptTime.x, m_ptTime.y, (LPCSTR)m_csTime, m_csTime.GetLength());
		}

		if( m_boolShowNote )
		{
			str = "XXXXXXXXXXXXXXXX";
			GetTextExtentPoint( hdc, (LPCSTR)str, str.GetLength(), &szText );
			m_ptNote.x = m_sizeReso.cx -szText.cx -nOffsetY;
			TextOut(hdc, m_ptNote.x, m_ptNote.y, (LPCSTR)m_csNote, m_csNote.GetLength());
		}

		if( m_bSplit )
		{
			str.Format( "HV:%s", m_csHV );
			TextOut(hdc, m_ptHV.x +nOffsetX, m_ptHV.y, (LPCSTR)str, str.GetLength());	//m_csHV, m_csHV.GetLength());
			str.Format( "MAG:%s", m_csMag );
			TextOut(hdc, m_ptMag.x +nOffsetX, m_ptMag.y, (LPCSTR)str, str.GetLength());	//m_csMag, m_csMag.GetLength());
			str.Format( "WD:%s", m_csWD );
			TextOut(hdc, m_ptWD.x +nOffsetX, m_ptWD.y, (LPCSTR)str, str.GetLength());	//m_csWD, m_csWD.GetLength());
			str.Format( "DET:%s", m_csSG2 );
			TextOut(hdc, m_ptSG.x +nOffsetX, m_ptSG.y, (LPCSTR)str, str.GetLength());	//m_csSG2, m_csSG2.GetLength());
			GetTextExtentPoint( hdc, (LPCSTR)m_csUScal, m_csUScal.GetLength(), &szText );
			TextOut(hdc, m_ptUScal.x +nOffsetX +(m_nUScalePixelNum -szText.cx)/2, m_ptUScal.y-4, (LPCSTR)m_csUScal, m_csUScal.GetLength());
			if( m_boolShowTime )
				TextOut(hdc, m_ptTime.x +nOffsetX, m_ptTime.y, (LPCSTR)m_csTime, m_csTime.GetLength());
			if( m_boolShowNote )
				TextOut(hdc, m_ptNote.x +nOffsetX, m_ptNote.y, (LPCSTR)m_csNote, m_csNote.GetLength());
		}
	}
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if(m_hfontLabelLogoText != NULL)
	{
		SelectObject(hdc, m_hfontLabelLogoText);
		SetTextColor(hdc, m_sysOpt.color[SysOpt_clrLabelLogoText]);
		SetBkColor(hdc, clrBack);

		TextOut(hdc, m_ptLogo.x, m_ptLogo.y, (LPCSTR)m_csLogo, m_csLogo.GetLength());
		if( m_bSplit )
			TextOut(hdc, m_ptLogo.x +nOffsetX, m_ptLogo.y, (LPCSTR)m_csLogo, m_csLogo.GetLength());
	}
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	DeleteObject( hPen );
	DeleteObject( brBackBrush );
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( m_lpDDSLabelLay == NULL ) return DD_OK;
	m_lpDDSLabelLay->ReleaseDC(hdc);
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	if ( m_boolShowLabel )
	{
		CRect rcGrapic = CRect(0, m_szGraph.cy - m_szLabel.cy,
			m_szGraph.cx, m_szGraph.cy);
		CRect rcImage = LayWindowMapClip_Graph_to_Image( rcGrapic );

		m_UpdateRectsImage.Add( rcImage );
	}
	/////////////////////////////////////////////////////////////////

	return DD_OK;
}

void CMCICard::UpdateHV(CString	csHV)
{	
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_csHV = csHV;
	m_boolRedrawLabel = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}

void CMCICard::UpdateWD(CString	csWD)
{	
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_csWD = csWD;
	m_boolRedrawLabel = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}

void CMCICard::UpdatePhotoSN(CString	csPhotoSN)
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_csPhotoSN = csPhotoSN;
	m_boolRedrawLabel = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}

void CMCICard::UpdateTime(CString	csTime)
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_csTime = csTime;
	m_boolRedrawLabel = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}

void CMCICard::UpdateNote(CString	csNote)
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_csNote = csNote;
	m_boolRedrawLabel = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}

void CMCICard::UpdateMAG(
	CString		csMag,					// 放大倍数显示字符
	int			nUScalePixelNum,		// 固定标尺显示长度
	CString		csUScal,				// 固定标尺显示字符
	CString		csMobileUScaleText )	// 移动标尺显示字符
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_csMag					= csMag;
	m_nUScalePixelNum		= nUScalePixelNum;
	m_csUScal				= csUScal;
	m_csMobileUScaleText	= csMobileUScaleText;
	m_boolRedrawUScal = TRUE;
	m_boolRedrawLabel = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}

// 16.06.12 多移动标尺实验
void CMCICard::UpdateMAGArray( int nIndex,
	CString		csMobileUScaleText )	// 移动标尺显示字符
{
	///////////////////////////////////////////////////////
	m_mutexLayAll.Lock();
	///////////////////////////////////////////////////////

	m_csMobileUScaleTextArray[nIndex]	= csMobileUScaleText;
	m_boolRedrawLabel = TRUE;

	///////////////////////////////////////////////////////
	m_mutexLayAll.Unlock();
	///////////////////////////////////////////////////////
}
// 16.06.12 多移动标尺实验

