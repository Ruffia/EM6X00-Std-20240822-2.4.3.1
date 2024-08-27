
#if !defined(MCICardParam__INCLUDE_)
#define MCICardParam__INCLUDE_

const DWORD	g_dwUSB_UpdateDelay				= 8;//13;

const CSize	g_szUSB_FrameImage				= CSize(8192,8192);	//CSize(2048,1536);
const CSize	g_szUSB_FrameGraph				= CSize(2048,2048);	//CSize(1024,768);
const CSize	g_szUSB_FrameLabel				= CSize(2048,64);	//CSize(1024,64);
const CSize	g_szUSB_FrameOscillograph		= CSize(2048,128);	//CSize(1024,128);

enum USB_FRAME_ZODER{
	USB_FrameDebugInfo,
	USB_FrameMobileUScale,
	USB_FrameLabel,
	USB_FrameEdit,
	USB_FrameGraphic,
	USB_FrameImage,
};

const COLORREF g_clrUSB_Transparent		= RGB(0,0,0);		// 0x00rrggbb
const COLORREF g_clrUSB_Background		= RGB(0,0,127);//RGB(0,0,1);//RGB(0,0,127);		// 0x00rrggbb

const COLORREF g_clrUSB_EditText		= RGB(155,255,255);
const COLORREF g_clrUSB_LabelText		= RGB(155,255,255);
const COLORREF g_clrUSB_LabelLogoText	= RGB(155,255,255);

const COLORREF g_clrUSB_MobileUScale	= RGB(255,255,155);//RGB(55, 146, 200);//
const COLORREF g_clrUSB_MobileUScaleLine= RGB(255,0,0);

//_TB _TN _NB _NN

//#define GRAPHIC_Transparent
#define GRAPHIC_BLACK

// 07.10.31显示模式
#define	NEW_SCANSCREEN_MODE

//======2004-8-2_加强图形层显示=======================
// 修改标示：2004-8-2_加强图形层显示
// 修改日期：2004年8月2日星期一
// 修改人：罗鸣华
// 修改原因：河北大学（保定）用户建议加强图形层显示
//		修改图形层字符颜色，全部改为白色。
//	    加大底标字体字号
//      加粗标尺
//======修改日期：2004年8月2日星期一==================


const int		Edit_nFontSize			= 32;
const CString	Edit_csFontFacename		= _T("黑体");//_T("宋体");

const int		Label_nFontSize			= 40;	//32
const CString	Label_csFontFacename	= _T("黑体");//_T("宋体");

const int		Logo_nFontSize			= 40;	//32
const CString	Logo_csFontFacename		= _T("黑体");//_T("Times New Roman");

const CString	UScale_csFontFacename	= _T("黑体");//_T("宋体");

// 1 inch = 25.4 mm
const double mm_TO_inch				= (double)0.0393700787401574803149606299212598;
const double SEM_dfStandImageWidth	= (double)120;		// mm
const double SEM_dfStandImageHeight	= (double)90;		// mm

#ifdef GRAPHIC_BLACK
const int	c_fnWeight	= FW_BLACK;		// font weight
#else
const int	c_fnWeight	= FW_NORMAL;	// font weight
#endif

#endif // MCICardParam__INCLUDE_

