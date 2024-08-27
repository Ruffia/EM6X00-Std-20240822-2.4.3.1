// SEMFileDialog.cpp : implementation file
//
// Last Modify : 2011.04.28

/////////////////////////////////////////////////////////////////////////////
// 10.07.02 增加打开窗口的图像预览
// 11.04.28 更新CxImage相关项 
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XXX2.h"

#include "DLGS.h"
#include "SEMFileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// 前置声明，使用OFN_ENABLEHOOK时使用
BOOL NEAR CALLBACK HandleNotify(HWND hDlg, LPOFNOTIFY pofn);
LRESULT CALLBACK ComDlgPreviewProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

/////////////////////////////////////////////////////////////////////////////
// CSEMFileDialog

IMPLEMENT_DYNAMIC(CSEMFileDialog, CFileDialog)

CSEMFileDialog::CSEMFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd,DWORD dwSize,BOOL bVistaStyle) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd,dwSize,bVistaStyle)
{
	if( bOpenFileDialog )	// 只有打开对话框才显示预览
	{
		m_ofn.Flags			|= OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLETEMPLATE;// | OFN_ENABLEHOOK; 
		m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_DLG_HOOK_PREVIEW);
//		m_ofn.hInstance		= AfxGetInstanceHandle();
//		m_ofn.lpfnHook		=  (LPOFNHOOKPROC)ComDlgPreviewProc;
	}
	else
	{
		m_ofn.Flags			|= OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLETEMPLATE;// | OFN_ENABLEHOOK; 
		m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_DLG_HOOK_SAVESRC);
	}

	m_bOpenFileDialog	= bOpenFileDialog;
	image				= NULL;
}

CSEMFileDialog::~CSEMFileDialog()
{
	m_Font.DeleteObject();

	if (image)
	{
		image->SetEscape(1);
		image->Destroy();
	}

	// free objects
	delete image;
}


BEGIN_MESSAGE_MAP(CSEMFileDialog, CFileDialog)
	//{{AFX_MSG_MAP(CSEMFileDialog)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_CHECK_Preview_ShowPrev, OnCHECKPreviewShowPrev)
	ON_BN_CLICKED(IDC_CHECK_Save_Src, OnCHECKSaveSrc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSEMFileDialog

void CSEMFileDialog::Localize(void)
{
	if( m_hWnd == NULL )
		return;

	CString str;
	CWnd* pParent = GetParent();

	// Checkboxes
	CWnd* pChx = pParent->GetDlgItem(chx1);		//以只读方式打开(&R)
	if( pChx != NULL )
		pChx->GetWindowText( str );

	// Static text
	CWnd* pStc = pParent->GetDlgItem(stc1);
	if( pStc != NULL )
		pStc->GetWindowText( str );

	pStc = pParent->GetDlgItem(stc2);			//文件类型(&T): 或 保存类型(&T):
	if( pStc != NULL )
	{
		if( m_bOpenFileDialog )
			CommDlg_OpenSave_SetControlText( GetParent()->m_hWnd, stc2, (LPCSTR)GetResString(IDS_FileDialog_OpenType) );
		else
			CommDlg_OpenSave_SetControlText( GetParent()->m_hWnd, stc2, (LPCSTR)GetResString(IDS_FileDialog_SaveType) );
	}

	pStc = pParent->GetDlgItem(stc3);			//文件名(&N):
	if( pStc != NULL )
		CommDlg_OpenSave_SetControlText( GetParent()->m_hWnd, stc3, (LPCSTR)GetResString(IDS_FileDialog_FileName) );

	pStc = pParent->GetDlgItem(stc4);			//查找范围(&I): 或 保存在(&I):
	if( pStc != NULL )
	{
		if( m_bOpenFileDialog )
			CommDlg_OpenSave_SetControlText( GetParent()->m_hWnd, stc4, (LPCSTR)GetResString(IDS_FileDialog_LookIn) );
		else
			CommDlg_OpenSave_SetControlText( GetParent()->m_hWnd, stc4, (LPCSTR)GetResString(IDS_FileDialog_SaveIn) );
	}

	CWnd* pWnd = pParent->GetDlgItem(IDOK);		//打开(&O)或保存(&S)
	if( pWnd != NULL )
	{
		if( m_bOpenFileDialog )
			CommDlg_OpenSave_SetControlText( GetParent()->m_hWnd, IDOK, (LPCSTR)GetResString(IDS_FileDialog_Open) );
		else
			CommDlg_OpenSave_SetControlText( GetParent()->m_hWnd, IDOK, (LPCSTR)GetResString(IDS_FileDialog_Save) );
	}

	pWnd = pParent->GetDlgItem(IDCANCEL);		//取消
	if( pWnd != NULL )
		CommDlg_OpenSave_SetControlText( GetParent()->m_hWnd, IDCANCEL, (LPCSTR)GetResString(IDS_FileDialog_Cancel) );

	pWnd = GetDlgItem(IDC_CHECK_Preview_ShowPrev);	//是否显示预览
	if( pWnd != NULL )
		SetDlgItemText( IDC_CHECK_Preview_ShowPrev, (LPCSTR)GetResString(IDS_CHECK_Preview_ShowPrev) );

	pWnd = GetDlgItem(IDC_CHECK_Save_Src);		//是否保存图像原始数据
	if( pWnd != NULL )
		SetDlgItemText( IDC_CHECK_Save_Src, (LPCSTR)GetResString(IDS_CHECK_Save_Src) );
} 

BOOL CSEMFileDialog::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	Localize();

	m_Font.CreateFont(-12, 0, 0, 0, FW_NORMAL, 0, 0, 0,
					ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Microsoft Sans Serif");

	SetFont( &m_Font, TRUE );
	CWnd *pParent = GetParent();
	CWnd *pw = pParent->GetWindow(GW_CHILD);
	while(pw != NULL)
	{   
		pw->SetFont( &m_Font);
		pw = pw->GetWindow(GW_HWNDNEXT);
	};

	CButton* pBtn = (CButton*)(GetDlgItem(IDC_CHECK_Preview_ShowPrev));
	if( pBtn != NULL )
		pBtn->SetCheck( TRUE );

	theApp.m_nSaveSrc = 0;
	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSEMFileDialog::OnCHECKPreviewShowPrev() 
{
	// TODO: Add your control notification handler code here
	Invalidate( FALSE );
}

void CSEMFileDialog::OnCHECKSaveSrc() 
{
	// TODO: Add your control notification handler code here
	theApp.m_nSaveSrc = IsDlgButtonChecked(IDC_CHECK_Save_Src);
}

void CSEMFileDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if (image == NULL || !image->IsValid() )
		return;

	if( !IsDlgButtonChecked(IDC_CHECK_Preview_ShowPrev) )
		return;

	//获得画图区域和显示比例
	int nImageWidth = image->GetWidth();
	int nImageHeight = image->GetHeight();

	CRect rcClient = lpDrawItemStruct->rcItem;
	double dW, dH;
	dW = (double)rcClient.Width() / nImageWidth;
	dH = (double)rcClient.Height() / nImageHeight;
	double dZoom = 0.0;
	if ( dW > dH )
	{
		dZoom = dH;
	}
	else
	{
		dZoom = dW;
	}

	int w = (int)(nImageWidth*dZoom);
	int h = (int)(nImageHeight*dZoom);
		image->Draw(lpDrawItemStruct->hDC,
				CRect( rcClient.left + (rcClient.Width()-w)/2,
						rcClient.top + (rcClient.Height()-h)/2,
						rcClient.left + (rcClient.Width()-w)/2 +w,
						rcClient.top + (rcClient.Height()-h)/2 +h ) );

	
	CFileDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CSEMFileDialog::OnTypeChange()
{
	int nIndex = CFileDialog::m_ofn.nFilterIndex - 1;
	int nDocType = theApp.GetTypeFromIndex( nIndex, m_bOpenFileDialog );
	if( nDocType == -1 )
		return;

	CString csCurrentExt = theApp.GetExtFromType(nDocType);
	int nPosStar = csCurrentExt.Find( '.' );
	int nPosSemicolon = csCurrentExt.Find( ';' );
	if( nPosSemicolon == -1 )
		nPosSemicolon = csCurrentExt.GetLength();
	csCurrentExt = csCurrentExt.Mid( nPosStar +1, nPosSemicolon -nPosStar -1 );

	CString csFileExt = CFileDialog::GetFileExt();
	
	if( csFileExt != csCurrentExt )
	{
		CString csFileTitle = CFileDialog::GetFileTitle(); 
		if ( csFileTitle.IsEmpty() )	csFileTitle = _T("*");
		CString csFileName = csFileTitle + _T(".") + csCurrentExt;

		CWnd* pParent = GetParent();
		if (pParent)
		{
			CommDlg_OpenSave_SetControlText( pParent->m_hWnd, edt1, (LPCSTR)csFileName );
		}
	}
}

void CSEMFileDialog::OnFileNameChange()
{
	CFileDialog::OnFileNameChange();

	if( !m_bOpenFileDialog )
		return;

	// 准备预览图像
	CString filename(GetPathName());
	CString ext = filename.Right(3);
	ext.MakeLower();
	if (ext == "") return;

	int nType = CxImage::GetTypeIdFromName(ext);
	if (nType == CXIMAGE_FORMAT_UNKNOWN)
		nType = SEMIMAGE_FORMAT_SEM;

	if (image != NULL && image->IsValid())
	{
		delete image;
		image = NULL;
	}

	BeginWaitCursor();
	if (nType < SEMIMAGE_FORMAT_SEM)
		image = new CxImage(filename, nType);
	else
	{
		// 读取sem文件并生成图像
		CFile file;
		if( !file.Open(filename, CFile::modeRead) )
			return;

		///////////////////////////////////////////////////////////////////////////////////
		DWORD dwImageFileSize;										//图像文件的大小
		DWORD dwImageSize;											//图像的大小

		BITMAPFILEHEADER	bmpFileHeader;
		file.Read( &dwImageFileSize, sizeof( DWORD ) );

		file.Read( &bmpFileHeader, sizeof( BITMAPFILEHEADER ) );
		dwImageSize = dwImageFileSize - sizeof(BITMAPFILEHEADER);

		if( dwImageSize > 0 )
		{
			HANDLE hDIB = (HANDLE)GlobalAlloc(GHND, dwImageSize);
			if( hDIB == NULL )
			{
				file.Close();
				return;
			}
			
			BYTE* pGreyImageBuffer = (BYTE *)::GlobalLock( hDIB );
			file.Read( pGreyImageBuffer, dwImageSize );
			GlobalUnlock( hDIB );

			CxImage *newImage = new CxImage();
			newImage->CreateFromHANDLE( hDIB );
			image = newImage;

			GlobalFree( hDIB );
		}
		///////////////////////////////////////////////////////////////////////////////////

		file.Close();
	}
	EndWaitCursor();

	// 如果生成图像成功就刷新
	if (image != NULL && image->IsValid())
		Invalidate(FALSE);
	else
	{
		delete image;
		image = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//Hook function for the Comm Dlg
//在这里处理我们感兴趣的消息，想要哪个，截哪个!
LRESULT CALLBACK ComDlgPreviewProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:   //初始化对话框上的控件
		
		HWND hWnd;
		HWND hParent;
		hParent = GetParent( hDlg );

		// Static text
		hWnd = GetDlgItem(hParent, stc2);			//文件类型(&T): 或 保存类型(&T):
		if( hWnd != NULL )
		{
			::SetWindowText(hWnd, (LPCSTR)GetResString(IDS_FileDialog_OpenType));
		}

		hWnd = GetDlgItem(hParent, stc3);			//文件名(&N):
		if( hWnd != NULL )
			::SetWindowText( hWnd, (LPCSTR)GetResString(IDS_FileDialog_FileName) );

		hWnd = GetDlgItem(hParent, stc4);			//查找范围(&I): 或 保存在(&I):
		if( hWnd != NULL )
		{
			::SetWindowText( hWnd, (LPCSTR)GetResString(IDS_FileDialog_LookIn) );
		}

		hWnd = GetDlgItem(hParent, IDOK);			//打开(&O)或保存(&S)
		if( hWnd != NULL )
		{
			::SetWindowText( hWnd, (LPCSTR)GetResString(IDS_FileDialog_Open) );
		}

		hWnd = GetDlgItem(hParent, IDCANCEL);		//取消
		if( hWnd != NULL )
			::SetWindowText( hWnd, (LPCSTR)GetResString(IDS_FileDialog_Cancel) );

		hWnd = GetDlgItem(hDlg, IDC_CHECK_Preview_ShowPrev);
		if(SendDlgItemMessage(hDlg, IDC_CHECK_Preview_ShowPrev, BM_GETCHECK,0,0) == BST_UNCHECKED)
		{   
			hWnd = GetDlgItem(hDlg, IDC_BUTTON_Preview_Image);
			::SetWindowText(hWnd, "\n\n\n\n    点击图象文件进行预览！");
		}
		break;
	
	case WM_DESTROY:    //消毁对话框
		{
			LPOPENFILENAME lpOFN = (LPOPENFILENAME)GetWindowLong(hDlg, DWL_USER);
			
		}
		break;
		
	case WM_COMMAND:   //这里处理，IDC_CHECK_Preview_ShowPrev命令
		if ((HIWORD(wParam) == BN_CLICKED) && (LOWORD(wParam) == IDC_CHECK_Preview_ShowPrev))
		{
			if(SendDlgItemMessage(hDlg, IDC_CHECK_Preview_ShowPrev, BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				InvalidateRect(GetDlgItem(hDlg, IDC_BUTTON_Preview_Image), NULL, FALSE);
//				::SetDlgItemInt(hDlg, IDC_W, 0, FALSE); 
//				::SetDlgItemInt(hDlg, IDC_H, 0, FALSE); 
			}
		}
     
		//不选中，
		if(SendDlgItemMessage(hDlg, IDC_CHECK_Preview_ShowPrev, BM_GETCHECK,0,0) == BST_UNCHECKED)
		{   
			HWND hWnd;
			hWnd = GetDlgItem(hDlg, IDC_BUTTON_Preview_Image);
			::SetWindowText(hWnd, "\n\n\n\n    点击图象文件进行预览！");
		}
		
		//选中
		if(SendDlgItemMessage(hDlg, IDC_CHECK_Preview_ShowPrev, BM_GETCHECK,0,0) != BST_UNCHECKED)
		{
			HWND hWnd;
			hWnd = GetDlgItem(hDlg, IDC_BUTTON_Preview_Image);
			::SetWindowText(hWnd, "\n\n\n\n      处于非预览状态！");
		}
		
		if(LOWORD(wParam) == IDOK)
		{	
		}
		break;
		
	case WM_NOTIFY:
		HandleNotify(hDlg, (LPOFNOTIFY)lParam);
		
	default:		
		return FALSE;
	}
	return TRUE;
}

//这里处理notify 消息
BOOL NEAR CALLBACK HandleNotify(HWND hDlg, LPOFNOTIFY pofn)
{
	CSEMFileDialog dlgFile(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,NULL,NULL,0,FALSE);
	switch (pofn->hdr.code)
	{
	case CDN_SELCHANGE:
		{
			char szFile[MAX_PATH];
			// Get the path of the selected file.
			if (CommDlg_OpenSave_GetFilePath(GetParent(hDlg), szFile, sizeof(szFile)) <= sizeof(szFile))
			{

				if(GetFileAttributes(szFile) !=  FILE_ATTRIBUTE_DIRECTORY)
				{			
					//Should we load the Pic
//					if(SendDlgItemMessage(hDlg, IDC_CHECK_Preview_ShowPrev, BM_GETCHECK,0,0) == BST_UNCHECKED)
//						dlg.ShowImagepreview(hDlg, szFile);	
				}
			}
		}
		break;
		
	case CDN_FILEOK:
		return FALSE;
		break;
		
	}
	
	return(TRUE);
}