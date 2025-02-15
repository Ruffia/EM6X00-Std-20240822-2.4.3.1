// DlgOptions.cpp : implementation file
//

#include "stdafx.h"
#include "XXX2.h"
#include "DlgOptions.h"

#include "ximajpg.h"
#include "ximaraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptions dialog


CDlgOptions::CDlgOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptions)
	m_jpeg_quality = 0.0f;
	m_xres = 0;
	m_yres = 0;
	//}}AFX_DATA_INIT
}


void CDlgOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptions)
	DDX_Control(pDX, IDC_CHECK5, m_chkJpgOpt);
	DDX_Control(pDX, IDEXIF, m_info);
	DDX_Control(pDX, IDC_RADIO21,m_rEnc21);
	DDX_Control(pDX, IDC_RADIO20,m_rEnc20);
	DDX_Control(pDX, IDC_RADIO19,m_rEnc19);
	DDX_Control(pDX, IDC_RADIO18,m_rEnc18);
	DDX_Control(pDX, IDC_RADIO17,m_rEnc17);
	DDX_Control(pDX, IDC_RADIO16,m_rEnc16);
	DDX_Control(pDX, IDC_RADIO15,m_rEnc15);
	DDX_Control(pDX, IDC_RADIO14,m_rEnc14);
	DDX_Control(pDX, IDC_RADIO13,m_rEnc13);
	DDX_Control(pDX, IDC_RADIO12,m_rEnc12);
	DDX_Control(pDX, IDC_RADIO11,m_rEnc11);
	DDX_Control(pDX, IDC_RADIO10,m_rEnc10);
	DDX_Control(pDX, IDC_RADIO9, m_rEnc9);
	DDX_Control(pDX, IDC_RADIO8, m_rEnc8);
	DDX_Control(pDX, IDC_RADIO7, m_rEnc7);
	DDX_Control(pDX, IDC_RADIO6, m_rEnc5);
	DDX_Control(pDX, IDC_RADIO5, m_rEnc4);
	DDX_Control(pDX, IDC_RADIO4, m_rEnc3);
	DDX_Control(pDX, IDC_RADIO3, m_rEnc2);
	DDX_Control(pDX, IDC_RADIO2, m_rEnc1);
	DDX_Control(pDX, IDC_RADIO1, m_rEnc0);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Text(pDX, IDC_EDIT1, m_jpeg_quality);
	DDV_MinMaxFloat(pDX, m_jpeg_quality, 0.0f, 100.0f);
	DDX_Text(pDX, IDC_EDIT2, m_xres);
	DDV_MinMaxLong(pDX, m_xres, 0, 10000);
	DDX_Text(pDX, IDC_EDIT3, m_yres);
	DDV_MinMaxDWord(pDX, m_yres, 0, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptions, CDialog)
	//{{AFX_MSG_MAP(CDlgOptions)
	ON_BN_CLICKED(IDEXIF, OnExif)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOptions message handlers

BOOL CDlgOptions::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);
	m_info.SetIcon(IDI_B,BS_LEFT);

	m_info.EnableWindow(m_exif && m_exif->m_exifinfo.IsExif);

	switch(m_Opt_tif)
	{
	case 1:
		m_rEnc1.SetCheck(1);
		break;
	case 2:
		m_rEnc2.SetCheck(1);
		break;
	case 3:
	case 4:
		m_rEnc3.SetCheck(1);
		break;
	case 5:
		m_rEnc7.SetCheck(1);
		break;
	case 32773:
		m_rEnc4.SetCheck(1);
		break;
	case 7:
		m_rEnc5.SetCheck(1);
		break;
	default:
		m_rEnc0.SetCheck(1);
	}

	switch(m_Opt_gif)
	{
	case 1:
		m_rEnc9.SetCheck(1);
		break;
	case 2:
		m_rEnc10.SetCheck(1);
		break;
	default:
		m_rEnc8.SetCheck(1);
	}

	switch(m_Opt_png)
	{
	case 1:
		m_rEnc12.SetCheck(1);
		break;
	default:
		m_rEnc11.SetCheck(1);
	}

	if (m_Opt_jpg & CxImageJPG::ENCODE_PROGRESSIVE){
		m_rEnc14.SetCheck(1);
	} else {
		m_rEnc13.SetCheck(1);
	}
	if (m_Opt_jpg & CxImageJPG::ENCODE_OPTIMIZE){
		m_chkJpgOpt.SetCheck(1);
	} else {
		m_chkJpgOpt.SetCheck(0);
	}
	if (m_Opt_jpg & CxImageJPG::ENCODE_SUBSAMPLE_444){
		m_rEnc17.SetCheck(1);
	} else if (m_Opt_jpg & CxImageJPG::ENCODE_SUBSAMPLE_422){
		m_rEnc16.SetCheck(1);
	} else {
		m_rEnc15.SetCheck(1);
	}

	switch(m_Opt_raw)
	{
	case 1:
		m_rEnc19.SetCheck(1);
		break;
	case 2:
		m_rEnc20.SetCheck(1);
		break;
	case 3:
		m_rEnc21.SetCheck(1);
		break;
	default:
		m_rEnc18.SetCheck(1);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOptions::OnOK() 
{
	if (m_rEnc1.GetCheck()) m_Opt_tif = 1;
	if (m_rEnc2.GetCheck()) m_Opt_tif = 2;
	if (m_rEnc3.GetCheck()) m_Opt_tif = 4;
	if (m_rEnc4.GetCheck()) m_Opt_tif = 32773;
	if (m_rEnc5.GetCheck()) m_Opt_tif = 7;
	if (m_rEnc7.GetCheck()) m_Opt_tif = 5;
	if (m_rEnc0.GetCheck()) m_Opt_tif = 0;

	if (m_rEnc8.GetCheck())  m_Opt_gif = 0;
	if (m_rEnc9.GetCheck())  m_Opt_gif = 1;
	if (m_rEnc10.GetCheck()) m_Opt_gif = 2;

	if (m_rEnc11.GetCheck()) m_Opt_png = 0;
	if (m_rEnc12.GetCheck()) m_Opt_png = 1;

#if CXIMAGE_SUPPORT_JPG
	if (m_rEnc13.GetCheck()) m_Opt_jpg = 0;
	if (m_rEnc14.GetCheck()) m_Opt_jpg = CxImageJPG::ENCODE_PROGRESSIVE;
	if (m_chkJpgOpt.GetCheck()) m_Opt_jpg |= CxImageJPG::ENCODE_OPTIMIZE;

	m_Opt_jpg &= ~(CxImageJPG::ENCODE_SUBSAMPLE_444 | CxImageJPG::ENCODE_SUBSAMPLE_422);
	if (m_rEnc17.GetCheck()) m_Opt_jpg |= CxImageJPG::ENCODE_SUBSAMPLE_444;
	if (m_rEnc16.GetCheck()) m_Opt_jpg |= CxImageJPG::ENCODE_SUBSAMPLE_422;
#endif

#if CXIMAGE_SUPPORT_RAW
	if (m_rEnc18.GetCheck()) m_Opt_raw = CxImageRAW::DECODE_QUALITY_LIN;
	if (m_rEnc19.GetCheck()) m_Opt_raw = CxImageRAW::DECODE_QUALITY_VNG;
	if (m_rEnc20.GetCheck()) m_Opt_raw = CxImageRAW::DECODE_QUALITY_PPG;
	if (m_rEnc21.GetCheck()) m_Opt_raw = CxImageRAW::DECODE_QUALITY_AHD;
#endif

	CDialog::OnOK();
}

void CDlgOptions::OnExif() 
{
	CString s,t;
	s=_T("");

    if (m_exif->m_exifinfo.CameraMake[0]){
        t.Format(_T("Camera make  : %s\n"),m_exif->m_exifinfo.CameraMake); s+=t;
        t.Format(_T("Camera model : %s\n"),m_exif->m_exifinfo.CameraModel); s+=t;
    }
    if (m_exif->m_exifinfo.DateTime[0]){
        t.Format(_T("Date/Time    : %s\n"),m_exif->m_exifinfo.DateTime); s+=t;
    }
    if (m_exif->m_exifinfo.Version[0]){
        t.Format(_T("Exif version : %s\n"),m_exif->m_exifinfo.Version); s+=t;
    }

    t.Format(_T("Width x Height   : %d x %d\n"),m_exif->m_exifinfo.Width, m_exif->m_exifinfo.Height); s+=t;

	if ((m_exif->m_exifinfo.ResolutionUnit!=0)&&(m_exif->m_exifinfo.Xresolution!=0)){
		t.Format(_T("X resolution (dpi) : %5.1f\n"),m_exif->m_exifinfo.Xresolution/m_exif->m_exifinfo.ResolutionUnit); s+=t;
	}
	if ((m_exif->m_exifinfo.ResolutionUnit!=0)&&(m_exif->m_exifinfo.Yresolution!=0)){
		t.Format(_T("Y resolution (dpi) : %5.1f\n"),m_exif->m_exifinfo.Yresolution/m_exif->m_exifinfo.ResolutionUnit); s+=t;
	}

    if (m_exif->m_exifinfo.Orientation > 1){
        t.Format(_T("Orientation  : %s\n"), m_exif->m_exifinfo.Orientation); s+=t;
    }
    if (m_exif->m_exifinfo.IsColor == 0){
        t.Format(_T("Color/bw     : Black and white\n")); s+=t;
    }
    if (m_exif->m_exifinfo.FlashUsed >= 0){
        t.Format(_T("Flash used   : %s\n"),m_exif->m_exifinfo.FlashUsed ? _T("Yes") : _T("No")); s+=t;
    }
    if (m_exif->m_exifinfo.FocalLength){
        t.Format(_T("Focal length : %4.1fmm"),(double)m_exif->m_exifinfo.FocalLength); s+=t;
        if (m_exif->m_exifinfo.CCDWidth){
            t.Format(_T("  (35mm equivalent: %dmm)"),
                        (int)(m_exif->m_exifinfo.FocalLength/m_exif->m_exifinfo.CCDWidth*36 + 0.5)); s+=t;
        }
        t.Format(_T("\n")); s+=t;
    }

    if (m_exif->m_exifinfo.CCDWidth){
        t.Format(_T("CCD width    : %4.2fmm\n"),(double)m_exif->m_exifinfo.CCDWidth); s+=t;
    }

    if (m_exif->m_exifinfo.ExposureTime){
        t.Format(_T("Exposure time:%6.3f s "),(double)m_exif->m_exifinfo.ExposureTime); s+=t;
        if (m_exif->m_exifinfo.ExposureTime <= 0.5){
            t.Format(_T(" (1/%d)"),(int)(0.5 + 1/m_exif->m_exifinfo.ExposureTime)); s+=t;
        }
        t.Format(_T("\n")); s+=t;
    }
	if (m_exif->m_exifinfo.Brightness){
		t.Format(_T("Brightness : %6.3f\n"),m_exif->m_exifinfo.Brightness); s+=t;
	}
    if (m_exif->m_exifinfo.ApertureFNumber){
        t.Format(_T("Aperture     : f/%3.1f\n"),(double)m_exif->m_exifinfo.ApertureFNumber); s+=t;
    }
    if (m_exif->m_exifinfo.Distance){
        if (m_exif->m_exifinfo.Distance < 0){
            t.Format(_T("Focus dist.  : Infinite\n")); s+=t;
        }else{
            t.Format(_T("Focus dist.  : %4.2fm\n"),(double)m_exif->m_exifinfo.Distance); s+=t;
        }
    }


    if (m_exif->m_exifinfo.ISOequivalent){ // 05-jan-2001 vcs
        t.Format(_T("ISO equiv.   : %2d\n"),(int)m_exif->m_exifinfo.ISOequivalent); s+=t;
    }
    if (m_exif->m_exifinfo.ExposureBias){ // 05-jan-2001 vcs
        t.Format(_T("Exposure bias:%4.2f\n"),(double)m_exif->m_exifinfo.ExposureBias); s+=t;
    }
        
    if (m_exif->m_exifinfo.Whitebalance){ // 05-jan-2001 vcs
        switch(m_exif->m_exifinfo.Whitebalance) {
        case 1:
            t.Format(_T("Whitebalance : sunny\n")); s+=t;
            break;
        case 2:
            t.Format(_T("Whitebalance : fluorescent\n")); s+=t;
            break;
        case 3:
            t.Format(_T("Whitebalance : incandescent\n")); s+=t;
            break;
        default:
            t.Format(_T("Whitebalance : cloudy\n")); s+=t;
        }
    }
    if (m_exif->m_exifinfo.MeteringMode){ // 05-jan-2001 vcs
        switch(m_exif->m_exifinfo.MeteringMode) {
        case 2:
            t.Format(_T("Metering Mode: center weight\n")); s+=t;
            break;
        case 3:
            t.Format(_T("Metering Mode: spot\n")); s+=t;
            break;
        case 5:
            t.Format(_T("Metering Mode: matrix\n")); s+=t;
            break;
        }
    }
    if (m_exif->m_exifinfo.ExposureProgram){ // 05-jan-2001 vcs
        switch(m_exif->m_exifinfo.ExposureProgram) {
        case 2:
            t.Format(_T("Exposure     : program (auto)\n")); s+=t;
            break;
        case 3:
            t.Format(_T("Exposure     : aperture priority (semi-auto)\n")); s+=t;
            break;
        case 4:
            t.Format(_T("Exposure     : shutter priority (semi-auto)\n")); s+=t;
            break;
        }
    }
    if (m_exif->m_exifinfo.CompressionLevel){ // 05-jan-2001 vcs
        switch(m_exif->m_exifinfo.CompressionLevel) {
        case 1:
            t.Format(_T("Jpeg Quality : basic\n")); s+=t;
            break;
        case 2:
            t.Format(_T("Jpeg Quality : normal\n")); s+=t;
            break;
        case 4:
            t.Format(_T("Jpeg Quality : fine\n")); s+=t;
            break;
       }
    }

    t.Format(_T("Encoding      : ")); s+=t;
	switch(m_exif->m_exifinfo.Process){
	case 0xC0: //M_SOF0
		t.Format(_T("Baseline\n")); s+=t;
		break;
	case 0xC1: //M_SOF1
		t.Format(_T("Extended sequential\n")); s+=t;
		break;
	case 0xC2: //M_SOF2
		t.Format(_T("Progressive\n")); s+=t;
		break;
	case 0xC3: //M_SOF3
		t.Format(_T("Lossless\n")); s+=t;
		break;
	case 0xC5: //M_SOF5
		t.Format(_T("Differential sequential\n")); s+=t;
		break;
	case 0xC6: //M_SOF6
		t.Format(_T("Differential progressive\n")); s+=t;
		break;
	case 0xC7: //M_SOF7
		t.Format(_T("Differential lossless\n")); s+=t;
		break;
	case 0xC9: //M_SOF9
		t.Format(_T("Extended sequential, arithmetic coding\n")); s+=t;
		break;
	case 0xCA: //M_SOF10
		t.Format(_T("Progressive, arithmetic coding\n")); s+=t;
		break;
	case 0xCB: //M_SOF11
		t.Format(_T("Lossless, arithmetic coding\n")); s+=t;
		break;
	case 0xCD: //M_SOF13
		t.Format(_T("Differential sequential, arithmetic coding\n")); s+=t;
		break;
	case 0xCE: //M_SOF14
		t.Format(_T("Differential progressive, arithmetic coding\n")); s+=t;
		break;
	case 0xCF: //M_SOF0
		t.Format(_T("Differential lossless, arithmetic coding\n")); s+=t;
		break;
	default:
		t.Format(_T("Unknown\n")); s+=t;
		break;
	}

    // Print the comment. Print 'Comment:' for each new line of comment.
    if (m_exif->m_exifinfo.Comments[0]){
        int a;
        char c;
        t.Format(_T("Comment      : ")); s+=t;
        for (a=0;a<MAX_COMMENT;a++){
            c = m_exif->m_exifinfo.Comments[a];
            if (c == '\0') break;
            if (c == '\n'){
                // Do not start a new line if the string ends with a carriage return.
                if (m_exif->m_exifinfo.Comments[a+1] != '\0'){
                    t.Format(_T("\nComment      : ")); s+=t;
                }else{
                    t.Format(_T("\n")); s+=t;
                }
            }else{
                s+=c;
            }
        }
    }

    t.Format(_T("\n")); s+=t;
		
	MessageBox(s,_T("EXIF"),MB_OK|MB_ICONQUESTION);
}
