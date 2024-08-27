// SEMFileDialog.cpp : implementation file
//
// Last Modify : 2011.04.28

/////////////////////////////////////////////////////////////////////////////
// 10.07.02 ���Ӵ򿪴��ڵ�ͼ��Ԥ��
// 11.04.28 ����CxImage����� 
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


// ǰ��������ʹ��OFN_ENABLEHOOKʱʹ��
BOOL NEAR CALLBACK HandleNotify(HWND hDlg, LPOFNOTIFY pofn);
LRESULT CALLBACK ComDlgPreviewProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

/////////////////////////////////////////////////////////////////////////////
// CSEMFileDialog

IMPLEMENT_DYNAMIC(CSEMFileDialog, CFileDialog)

CSEMFileDialog::CSEMFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd,DWORD dwSize,BOOL bVistaStyle) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd,dwSize,bVistaStyle)
{
	if( bOpenFileDialog )	// ֻ�д򿪶Ի������ʾԤ��
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
	CWnd* pChx = pParent->GetDlgItem(chx1);		//��ֻ����ʽ��(&R)
	if( pChx != NULL )
		pChx->GetWindowText( str );

	// Static text
	CWnd* pStc = pParent->GetDlgItem(stc1);
	if( pStc != NULL )
		pStc->GetWindowText( str );

	pStc = pParent->GetDlgItem(stc2);			//�ļ�����(&T): �� ��������(&T):
	if( pStc != NULL )
	{
		if( m_bOpenFileDialog )
			CommDlg_OpenSave_SetControlText( GetParent()->m_hWnd, stc2, (LPCSTR)GetResString(IDS_FileDialog_OpenType) );
		else
			CommDlg_OpenSave_SetControlText( GetParent()->m_hWnd, stc2, (LPCSTR)GetResString(IDS_FileDialog_SaveType) );
	}

	pStc = pParent->GetDlgItem(stc3);			//�ļ���(&N):
	if( pStc != NULL )
		CommDlg_OpenSave_SetControlText( GetParent()->m_hWnd, stc3, (LPCSTR)GetResString(IDS_FileDialog_FileName) );

	pStc = pParent->GetDlgItem(stc4);			//���ҷ�Χ(&I): �� ������(&I):
	if( pStc != NULL )
	{
		if( m_bOpenFileDialog )
			CommDlg_OpenSave_SetControlText( GetParent()->m_hWnd, stc4, (LPCSTR)GetResString(IDS_FileDialog_LookIn) );
		else
			CommDlg_OpenSave_SetControlText( GetParent()->m_hWnd, stc4, (LPCSTR)GetResString(IDS_FileDialog_SaveIn) );
	}

	CWnd* pWnd = pParent->GetDlgItem(IDOK);		//��(&O)�򱣴�(&S)
	if( pWnd != NULL )
	{
		if( m_bOpenFileDialog )
			CommDlg_OpenSave_SetControlText( GetParent()->m_hWnd, IDOK, (LPCSTR)GetResString(IDS_FileDialog_Open) );
		else
			CommDlg_OpenSave_SetControlText( GetParent()->m_hWnd, IDOK, (LPCSTR)GetResString(IDS_FileDialog_Save) );
	}

	pWnd = pParent->GetDlgItem(IDCANCEL);		//ȡ��
	if( pWnd != NULL )
		CommDlg_OpenSave_SetControlText( GetParent()->m_hWnd, IDCANCEL, (LPCSTR)GetResString(IDS_FileDialog_Cancel) );

	pWnd = GetDlgItem(IDC_CHECK_Preview_ShowPrev);	//�Ƿ���ʾԤ��
	if( pWnd != NULL )
		SetDlgItemText( IDC_CHECK_Preview_ShowPrev, (LPCSTR)GetResString(IDS_CHECK_Preview_ShowPrev) );

	pWnd = GetDlgItem(IDC_CHECK_Save_Src);		//�Ƿ񱣴�ͼ��ԭʼ����
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

	//��û�ͼ�������ʾ����
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

	// ׼��Ԥ��ͼ��
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
		// ��ȡsem�ļ�������ͼ��
		CFile file;
		if( !file.Open(filename, CFile::modeRead) )
			return;

		///////////////////////////////////////////////////////////////////////////////////
		DWORD dwImageFileSize;										//ͼ���ļ��Ĵ�С
		DWORD dwImageSize;											//ͼ��Ĵ�С

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

	// �������ͼ��ɹ���ˢ��
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
//�����ﴦ�����Ǹ���Ȥ����Ϣ����Ҫ�ĸ������ĸ�!
LRESULT CALLBACK ComDlgPreviewProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:   //��ʼ���Ի����ϵĿؼ�
		
		HWND hWnd;
		HWND hParent;
		hParent = GetParent( hDlg );

		// Static text
		hWnd = GetDlgItem(hParent, stc2);			//�ļ�����(&T): �� ��������(&T):
		if( hWnd != NULL )
		{
			::SetWindowText(hWnd, (LPCSTR)GetResString(IDS_FileDialog_OpenType));
		}

		hWnd = GetDlgItem(hParent, stc3);			//�ļ���(&N):
		if( hWnd != NULL )
			::SetWindowText( hWnd, (LPCSTR)GetResString(IDS_FileDialog_FileName) );

		hWnd = GetDlgItem(hParent, stc4);			//���ҷ�Χ(&I): �� ������(&I):
		if( hWnd != NULL )
		{
			::SetWindowText( hWnd, (LPCSTR)GetResString(IDS_FileDialog_LookIn) );
		}

		hWnd = GetDlgItem(hParent, IDOK);			//��(&O)�򱣴�(&S)
		if( hWnd != NULL )
		{
			::SetWindowText( hWnd, (LPCSTR)GetResString(IDS_FileDialog_Open) );
		}

		hWnd = GetDlgItem(hParent, IDCANCEL);		//ȡ��
		if( hWnd != NULL )
			::SetWindowText( hWnd, (LPCSTR)GetResString(IDS_FileDialog_Cancel) );

		hWnd = GetDlgItem(hDlg, IDC_CHECK_Preview_ShowPrev);
		if(SendDlgItemMessage(hDlg, IDC_CHECK_Preview_ShowPrev, BM_GETCHECK,0,0) == BST_UNCHECKED)
		{   
			hWnd = GetDlgItem(hDlg, IDC_BUTTON_Preview_Image);
			::SetWindowText(hWnd, "\n\n\n\n    ���ͼ���ļ�����Ԥ����");
		}
		break;
	
	case WM_DESTROY:    //���ٶԻ���
		{
			LPOPENFILENAME lpOFN = (LPOPENFILENAME)GetWindowLong(hDlg, DWL_USER);
			
		}
		break;
		
	case WM_COMMAND:   //���ﴦ��IDC_CHECK_Preview_ShowPrev����
		if ((HIWORD(wParam) == BN_CLICKED) && (LOWORD(wParam) == IDC_CHECK_Preview_ShowPrev))
		{
			if(SendDlgItemMessage(hDlg, IDC_CHECK_Preview_ShowPrev, BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				InvalidateRect(GetDlgItem(hDlg, IDC_BUTTON_Preview_Image), NULL, FALSE);
//				::SetDlgItemInt(hDlg, IDC_W, 0, FALSE); 
//				::SetDlgItemInt(hDlg, IDC_H, 0, FALSE); 
			}
		}
     
		//��ѡ�У�
		if(SendDlgItemMessage(hDlg, IDC_CHECK_Preview_ShowPrev, BM_GETCHECK,0,0) == BST_UNCHECKED)
		{   
			HWND hWnd;
			hWnd = GetDlgItem(hDlg, IDC_BUTTON_Preview_Image);
			::SetWindowText(hWnd, "\n\n\n\n    ���ͼ���ļ�����Ԥ����");
		}
		
		//ѡ��
		if(SendDlgItemMessage(hDlg, IDC_CHECK_Preview_ShowPrev, BM_GETCHECK,0,0) != BST_UNCHECKED)
		{
			HWND hWnd;
			hWnd = GetDlgItem(hDlg, IDC_BUTTON_Preview_Image);
			::SetWindowText(hWnd, "\n\n\n\n      ���ڷ�Ԥ��״̬��");
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

//���ﴦ��notify ��Ϣ
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