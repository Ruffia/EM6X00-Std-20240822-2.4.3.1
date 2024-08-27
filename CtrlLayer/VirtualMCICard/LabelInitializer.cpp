#include "stdafx.h"
#include "LabelInitializer.h"
using namespace LabelInitializer;
#include "LabelControler.h"
#include "Factory.h"
#include <string>
#include "MCICardOption.h"

IMPLEMENT_FACTORY(IMCILabelInitializer,CLASS_InitLabelLay_0128,std::string,"InitLabelLay_0128")
void CLASS_InitLabelLay_0128::Initialize(CLabelControler* pLabelControler)
{
	/////////////////////////////////////////////////
	pLabelControler->m_csHV				= _T("0.0 KV");				// 高压显示
	pLabelControler->m_csMag				= _T("250 X");				// 放大倍数显示
	pLabelControler->m_csUScal			= _T("100 μm");			// 固定标尺显示
	pLabelControler->m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	pLabelControler->m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	pLabelControler->m_csWD				= _T("12.0 mm");
	/////////////////////////////////////////////////

	pLabelControler->m_nUScalePixelNum	= 40;

	/////////////////////////////////////////////////
	// 	m_ptHV				= CPoint(1,		10);		// 高压显示位置
	// 	m_ptMag				= CPoint(24,	10);		// 放大倍数显示位置
	// 	m_ptUScal			= CPoint(48,	10);		// 固定标尺显示位置
	// 	m_ptLogo			= CPoint(64,	10);		// 产品标识显示位置
	// 	m_ptPhotoSN			= CPoint(110,	10);		// 照相编号显示位置
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
	if (pLabelControler->m_hfontEditText != NULL)
		DeleteObject(pLabelControler->m_hfontEditText);

	if (pLabelControler->m_hfontLabelText != NULL)
		DeleteObject(pLabelControler->m_hfontLabelText);

	if (pLabelControler->m_hfontLabelLogoText != NULL)
		DeleteObject(pLabelControler->m_hfontLabelLogoText);

	pLabelControler->m_hfontEditText = CreateFont(
		8, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
		VARIABLE_PITCH,
		"Arial" );//pLabelControler->m_sysOpt.fName[SysOpt_nameEditText] );

	pLabelControler->m_hfontLabelText = CreateFont(
		8, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
		VARIABLE_PITCH,
		"Arial" );//m_sysOpt.fName[SysOpt_nameEditText] );

	pLabelControler->m_hfontLabelLogoText = CreateFont(
		8, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
		VARIABLE_PITCH,
		"Arial" );//m_sysOpt.fName[SysOpt_nameEditText] );
	/////////////////////////////////////////////////
}
	

IMPLEMENT_FACTORY(IMCILabelInitializer,CLASS_InitLabelLay_0256,std::string,"InitLabelLay_0256")
void CLASS_InitLabelLay_0256::Initialize(CLabelControler* pLabelControler)
{
		/////////////////////////////////////////////////
	pLabelControler->m_csHV				= _T("0.0 KV");				// 高压显示
	pLabelControler->m_csMag				= _T("250 X");				// 放大倍数显示
	pLabelControler->m_csUScal			= _T("100 μm");				// 固定标尺显示
	pLabelControler->m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	pLabelControler->m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	pLabelControler->m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	pLabelControler->m_csWD				= _T("12.0 mm");			// 工作距离显示
	pLabelControler->m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	pLabelControler->m_nUScalePixelNum	= 80;

/*	/////////////////////////////////////////////////
	m_ptHV				= CPoint(0,		1);
	m_ptMag				= CPoint(56,	1);
	m_ptWD				= CPoint(0,		16);
	m_ptSG				= CPoint(56,	16);
	m_ptUScal			= CPoint(110,	1);
	m_ptLogo			= CPoint(110,	16);
	m_ptTime			= CPoint(220,	1);
	m_ptNote			= CPoint(220,	16);
*/	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (pLabelControler->m_hfontEditText != NULL)
        DeleteObject(pLabelControler->m_hfontEditText);

    if (pLabelControler->m_hfontLabelText != NULL)
        DeleteObject(pLabelControler->m_hfontLabelText);

    if (pLabelControler->m_hfontLabelLogoText != NULL)
        DeleteObject(pLabelControler->m_hfontLabelLogoText);

    pLabelControler->m_hfontEditText = CreateFont(
        7, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameEditText] );

    pLabelControler->m_hfontLabelText = CreateFont(
        7, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );

    pLabelControler->m_hfontLabelLogoText = CreateFont(
        7, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}


IMPLEMENT_FACTORY(IMCILabelInitializer,CLASS_InitLabelLay_0480,std::string,"InitLabelLay_0480")
void CLASS_InitLabelLay_0480::Initialize(CLabelControler* pLabelControler)
{
		/////////////////////////////////////////////////
	pLabelControler->m_csHV				= _T("0.0 KV");				// 高压显示
	pLabelControler->m_csMag				= _T("250 X");				// 放大倍数显示
	pLabelControler->m_csUScal			= _T("100 μm");				// 固定标尺显示
	pLabelControler->m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	pLabelControler->m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	pLabelControler->m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	pLabelControler->m_csWD				= _T("12.0 mm");			// 工作距离显示
	pLabelControler->m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	pLabelControler->m_nUScalePixelNum	= 80;

/*	/////////////////////////////////////////////////
	m_ptHV				= CPoint(8,		1);
	m_ptMag				= CPoint(120,	1);
	m_ptWD				= CPoint(8,		16);
	m_ptSG				= CPoint(120,	16);
	m_ptUScal			= CPoint(220,	1);
	m_ptLogo			= CPoint(220,	16);
	m_ptTime			= CPoint(350,	1);
	m_ptNote			= CPoint(350,	16);
*/	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (pLabelControler->m_hfontEditText != NULL)
        DeleteObject(pLabelControler->m_hfontEditText);

    if (pLabelControler->m_hfontLabelText != NULL)
        DeleteObject(pLabelControler->m_hfontLabelText);

    if (pLabelControler->m_hfontLabelLogoText != NULL)
        DeleteObject(pLabelControler->m_hfontLabelLogoText);

    pLabelControler->m_hfontEditText = CreateFont(
        12, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameEditText] );

    pLabelControler->m_hfontLabelText = CreateFont(
        12, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );

    pLabelControler->m_hfontLabelLogoText = CreateFont(
        12, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}


IMPLEMENT_FACTORY(IMCILabelInitializer,CLASS_InitLabelLay_0512,std::string,"InitLabelLay_0512")
void CLASS_InitLabelLay_0512::Initialize(CLabelControler* pLabelControler)
{
		/////////////////////////////////////////////////
	pLabelControler->m_csHV				= _T("0.0 KV");				// 高压显示
	pLabelControler->m_csMag				= _T("250 X");				// 放大倍数显示
	pLabelControler->m_csUScal			= _T("100 μm");			// 固定标尺显示
	pLabelControler->m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	pLabelControler->m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	pLabelControler->m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	pLabelControler->m_csWD				= _T("12.0 mm");			// 工作距离显示
	pLabelControler->m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	pLabelControler->m_nUScalePixelNum	= 100;

/*	/////////////////////////////////////////////////
	m_ptHV				= CPoint(8,		2);
	m_ptMag				= CPoint(120,	2);
	m_ptWD				= CPoint(8,		24);
	m_ptSG				= CPoint(120,	24);
	m_ptUScal			= CPoint(232,	2);
	m_ptLogo			= CPoint(232,	24);
	m_ptTime			= CPoint(350,	2);
	m_ptNote			= CPoint(350,	24);
*/	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (pLabelControler->m_hfontEditText != NULL)
        DeleteObject(pLabelControler->m_hfontEditText);

    if (pLabelControler->m_hfontLabelText != NULL)
        DeleteObject(pLabelControler->m_hfontLabelText);

    if (pLabelControler->m_hfontLabelLogoText != NULL)
        DeleteObject(pLabelControler->m_hfontLabelLogoText);

    pLabelControler->m_hfontEditText = CreateFont(
        16, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameEditText] );

    pLabelControler->m_hfontLabelText = CreateFont(
        16, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );

    pLabelControler->m_hfontLabelLogoText = CreateFont(
        16, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}


IMPLEMENT_FACTORY(IMCILabelInitializer,CLASS_InitLabelLay_0640,std::string,"InitLabelLay_0640")
void CLASS_InitLabelLay_0640::Initialize(CLabelControler* pLabelControler)
{
		/////////////////////////////////////////////////
	pLabelControler->m_csHV				= _T("0.0 KV");				// 高压显示
	pLabelControler->m_csMag				= _T("250 X");				// 放大倍数显示
	pLabelControler->m_csUScal			= _T("100 μm");			// 固定标尺显示
	pLabelControler->m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	pLabelControler->m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	pLabelControler->m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	pLabelControler->m_csWD				= _T("12.0 mm");			// 工作距离显示
	pLabelControler->m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	pLabelControler->m_nUScalePixelNum	= 100;

/*	/////////////////////////////////////////////////
	m_ptHV				= CPoint(16,	2);
	m_ptMag				= CPoint(160,	2);
	m_ptWD				= CPoint(16,	22);
	m_ptSG				= CPoint(160,	22);
	m_ptUScal			= CPoint(300,	2);
	m_ptLogo			= CPoint(300,	22);
	m_ptTime			= CPoint(440,	2);
	m_ptNote			= CPoint(440,	22);
*/	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (pLabelControler->m_hfontEditText != NULL)
        DeleteObject(pLabelControler->m_hfontEditText);

    if (pLabelControler->m_hfontLabelText != NULL)
        DeleteObject(pLabelControler->m_hfontLabelText);

    if (pLabelControler->m_hfontLabelLogoText != NULL)
        DeleteObject(pLabelControler->m_hfontLabelLogoText);

    pLabelControler->m_hfontEditText = CreateFont(
        16, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameEditText] );

    pLabelControler->m_hfontLabelText = CreateFont(
        20, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );

    pLabelControler->m_hfontLabelLogoText = CreateFont(
        20, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}

IMPLEMENT_FACTORY(IMCILabelInitializer,CLASS_InitLabelLay_0800,std::string,"InitLabelLay_0800")
void CLASS_InitLabelLay_0800::Initialize(CLabelControler* pLabelControler)
{
		/////////////////////////////////////////////////
	pLabelControler->m_csHV				= _T("0.0 KV");				// 高压显示
	pLabelControler->m_csMag				= _T("250 X");				// 放大倍数显示
	pLabelControler->m_csUScal			= _T("100 μm");				// 固定标尺显示
	pLabelControler->m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	pLabelControler->m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	pLabelControler->m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	pLabelControler->m_csWD				= _T("12.0 mm");			// 工作距离显示
	pLabelControler->m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	pLabelControler->m_nUScalePixelNum	= 100;

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (pLabelControler->m_hfontEditText != NULL)
        DeleteObject(pLabelControler->m_hfontEditText);

    if (pLabelControler->m_hfontLabelText != NULL)
        DeleteObject(pLabelControler->m_hfontLabelText);

    if (pLabelControler->m_hfontLabelLogoText != NULL)
        DeleteObject(pLabelControler->m_hfontLabelLogoText);

    pLabelControler->m_hfontEditText = CreateFont(
        20, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameEditText] );

    pLabelControler->m_hfontLabelText = CreateFont(
        20, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );

    pLabelControler->m_hfontLabelLogoText = CreateFont(
        20, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}
	

IMPLEMENT_FACTORY(IMCILabelInitializer,CLASS_InitLabelLay_1024,std::string,"InitLabelLay_1024")
void CLASS_InitLabelLay_1024::Initialize(CLabelControler* pLabelControler)
{
		/////////////////////////////////////////////////
	pLabelControler->m_csHV				= _T("0.0 KV");				// 高压显示
	pLabelControler->m_csMag				= _T("250 X");				// 放大倍数显示
	pLabelControler->m_csUScal			= _T("100 μm");			// 固定标尺显示
	pLabelControler->m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	pLabelControler->m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	pLabelControler->m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	pLabelControler->m_csWD				= _T("12.0 mm");			// 工作距离显示
	pLabelControler->m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	pLabelControler->m_nUScalePixelNum	= 100;

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (pLabelControler->m_hfontEditText != NULL)
        DeleteObject(pLabelControler->m_hfontEditText);

    if (pLabelControler->m_hfontLabelText != NULL)
        DeleteObject(pLabelControler->m_hfontLabelText);

    if (pLabelControler->m_hfontLabelLogoText != NULL)
        DeleteObject(pLabelControler->m_hfontLabelLogoText);

    pLabelControler->m_hfontEditText = CreateFont(
        CMCICardOption::Instance().m_sysOpt.fSize[SysOpt_nameEditText], 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameEditText] );

    pLabelControler->m_hfontLabelText = CreateFont(
        CMCICardOption::Instance().m_sysOpt.fSize[SysOpt_nameLabelText]/*Label_nFontSize*/, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );

    pLabelControler->m_hfontLabelLogoText = CreateFont(
        CMCICardOption::Instance().m_sysOpt.fSize[SysOpt_nameLabelText]/*Logo_nFontSize*/, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}


IMPLEMENT_FACTORY(IMCILabelInitializer,CLASS_InitLabelLay_1440,std::string,"InitLabelLay_1440")
void CLASS_InitLabelLay_1440::Initialize(CLabelControler* pLabelControler)
{
		/////////////////////////////////////////////////
	pLabelControler->m_csHV				= _T("0.0 KV");				// 高压显示
	pLabelControler->m_csMag				= _T("250 X");				// 放大倍数显示
	pLabelControler->m_csUScal			= _T("100 μm");			// 固定标尺显示
	pLabelControler->m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	pLabelControler->m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	pLabelControler->m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	pLabelControler->m_csWD				= _T("12.0 mm");			// 工作距离显示
	pLabelControler->m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	pLabelControler->m_nUScalePixelNum	= 200;

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (pLabelControler->m_hfontEditText != NULL)
        DeleteObject(pLabelControler->m_hfontEditText);

    if (pLabelControler->m_hfontLabelText != NULL)
        DeleteObject(pLabelControler->m_hfontLabelText);

    if (pLabelControler->m_hfontLabelLogoText != NULL)
        DeleteObject(pLabelControler->m_hfontLabelLogoText);

    pLabelControler->m_hfontEditText = CreateFont(
        40, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameEditText] );

    pLabelControler->m_hfontLabelText = CreateFont(
        48, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );

    pLabelControler->m_hfontLabelLogoText = CreateFont(
        48, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}


IMPLEMENT_FACTORY(IMCILabelInitializer,CLASS_InitLabelLay_2048,std::string,"InitLabelLay_2048")
void CLASS_InitLabelLay_2048::Initialize(CLabelControler* pLabelControler)
{
		/////////////////////////////////////////////////
	pLabelControler->m_csHV				= _T("0.0 KV");				// 高压显示
	pLabelControler->m_csMag				= _T("250 X");				// 放大倍数显示
	pLabelControler->m_csUScal			= _T("100 μm");			// 固定标尺显示
	pLabelControler->m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	pLabelControler->m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	pLabelControler->m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	pLabelControler->m_csWD				= _T("12.0 mm");			// 工作距离显示
	pLabelControler->m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	pLabelControler->m_nUScalePixelNum	= 200;

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (pLabelControler->m_hfontEditText != NULL)
        DeleteObject(pLabelControler->m_hfontEditText);

    if (pLabelControler->m_hfontLabelText != NULL)
        DeleteObject(pLabelControler->m_hfontLabelText);

    if (pLabelControler->m_hfontLabelLogoText != NULL)
        DeleteObject(pLabelControler->m_hfontLabelLogoText);

    pLabelControler->m_hfontEditText = CreateFont(
        60, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameEditText] );

    pLabelControler->m_hfontLabelText = CreateFont(
        64, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );

    pLabelControler->m_hfontLabelLogoText = CreateFont(
        64, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}


IMPLEMENT_FACTORY(IMCILabelInitializer,CLASS_InitLabelLay_4096,std::string,"InitLabelLay_4096")
void CLASS_InitLabelLay_4096::Initialize(CLabelControler* pLabelControler)
{
		/////////////////////////////////////////////////
	pLabelControler->m_csHV				= _T("0.0 KV");				// 高压显示
	pLabelControler->m_csMag				= _T("250 X");				// 放大倍数显示
	pLabelControler->m_csUScal			= _T("100 μm");				// 固定标尺显示
	pLabelControler->m_csLogo			= _T("KYKY-EM6200");		// 产品标识显示
	pLabelControler->m_csPhotoSN			= _T("SN:0000");			// 照相编号显示
	pLabelControler->m_csTime			= _T("2017.08.22 17:05");	// 日期时间显示
	pLabelControler->m_csWD				= _T("12.0 mm");			// 工作距离显示
	pLabelControler->m_csNote			= _T("KYKY Sample");		// 备注显示
	/////////////////////////////////////////////////

	pLabelControler->m_nUScalePixelNum	= 400;

	/////////////////////////////////////////////////
	// Create Font
	/////////////////////////////////////////////////
    if (pLabelControler->m_hfontEditText != NULL)
        DeleteObject(pLabelControler->m_hfontEditText);

    if (pLabelControler->m_hfontLabelText != NULL)
        DeleteObject(pLabelControler->m_hfontLabelText);

    if (pLabelControler->m_hfontLabelLogoText != NULL)
        DeleteObject(pLabelControler->m_hfontLabelLogoText);

    pLabelControler->m_hfontEditText = CreateFont(
        100, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameEditText] );

    pLabelControler->m_hfontLabelText = CreateFont(
        120, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );

    pLabelControler->m_hfontLabelLogoText = CreateFont(
        120, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,	// NONANTIALIASED_QUALITY,
        VARIABLE_PITCH,
        CMCICardOption::Instance().m_sysOpt.fName[SysOpt_nameLabelText] );
	/////////////////////////////////////////////////
}



