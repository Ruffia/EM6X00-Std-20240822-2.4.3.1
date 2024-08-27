// DlgImpPanelThumb.cpp : implementation file
//
// Last Modify : 2010.09.25

#include "stdafx.h"
#include "XXX2.h"
#include "DlgImpPanelThumb.h"

#include "MainFrm.h"
#include "ScanView.h"
#include "DlgInputAcq.h"
#include "DlgPrompt.h"	//09.04.20
#include "ImageDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelThumb dialog


CDlgImpPanelThumb::CDlgImpPanelThumb(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImpPanelThumb::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgImpPanelThumb)
	//}}AFX_DATA_INIT
}

CDlgImpPanelThumb::~CDlgImpPanelThumb()
{
	m_Font.DeleteObject();
}

void CDlgImpPanelThumb::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgImpPanelThumb)
	DDX_Control(pDX, IDC_LIST_IMP_PANEL_Thumb, m_ListCtrlThumb);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgImpPanelThumb, CDialog)
	//{{AFX_MSG_MAP(CDlgImpPanelThumb)
	ON_BN_CLICKED(IDC_BUTTON_IMP_PANEL_Thumb_Save, OnThumbSave)
	ON_BN_CLICKED(IDC_BUTTON_IMP_PANEL_Thumb_Delete, OnThumbDelete)
	ON_BN_CLICKED(IDC_BUTTON_IMP_PANEL_Thumb_View, OnThumbView)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_IMP_PANEL_Thumb, OnDblclkLISTThumb)
	ON_BN_CLICKED(IDC_CHECK_IMP_PANEL_Thumb_SelAll, OnThumbSelAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgImpPanelThumb message handlers

void CDlgImpPanelThumb::Localize(void)
{ 
	if( m_hWnd == NULL )
		return;

	GetDlgItem(IDC_BUTTON_IMP_PANEL_Thumb_Save)->SetWindowText(GetResString(IDS_BUTTON_IMP_PANEL_Thumb_Save)); 
	GetDlgItem(IDC_BUTTON_IMP_PANEL_Thumb_Delete)->SetWindowText(GetResString(IDS_BUTTON_IMP_PANEL_Thumb_Delete)); 
	GetDlgItem(IDC_BUTTON_IMP_PANEL_Thumb_View)->SetWindowText(GetResString(IDS_BUTTON_IMP_PANEL_Thumb_View)); 
	GetDlgItem(IDC_CHECK_IMP_PANEL_Thumb_SelAll)->SetWindowText(GetResString(IDS_CHECK_IMP_PANEL_Thumb_SelAll)); 

	SetWindowText(GetResString(IDS_STATIC_IMP_PANEL_Group_Thumb)); 
} 

BOOL CDlgImpPanelThumb::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ImageListThumb.Create(THUMB_WIDTH, THUMB_HEIGHT, ILC_COLOR24, 0, 1);
	m_ListCtrlThumb.SetImageList(&m_ImageListThumb, LVSIL_NORMAL);
	m_ListCtrlThumb.SetImageList(&m_ImageListThumb, LVSIL_SMALL);
	m_ListCtrlThumb.SetBkColor( ::GetSysColor(COLOR_3DFACE) );

	m_Font.CreateFont(-9, 0, 0, 0, FW_NORMAL, 0, 0, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Microsoft Sans Serif");
	m_ListCtrlThumb.SetFont( &m_Font );

	Localize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgImpPanelThumb::AdjustCtrlsPos()
{
//	if( m_hWnd == NULL )
//		return;

	//根据屏幕分辨率动态调整对话框布局
	CRect rc, wndRect;
	GetClientRect( &rc );

	int nLeft = rc.left +20;
	int nTop = rc.top +5;//( rc.Height() /6 *3 - 8*2 ) /2;
	int nWidth = rc.Width() /7;
	int nHeight = rc.Height() /5;
	//全选复选框
	CButton* pSelect = (CButton*)( GetDlgItem( IDC_CHECK_IMP_PANEL_Thumb_SelAll ) );
	if( pSelect != NULL )
		pSelect->MoveWindow( nLeft, nTop, nWidth, nHeight, TRUE );

	nTop += (nHeight +5);
	//保存按钮
	CButton* pSave = (CButton*)( GetDlgItem( IDC_BUTTON_IMP_PANEL_Thumb_Save ) );
	if( pSave != NULL )
		pSave->MoveWindow( nLeft, nTop, nWidth, nHeight, TRUE );

	nTop += (nHeight +5);
	//删除按钮
	CButton* pDelete = (CButton*)( GetDlgItem( IDC_BUTTON_IMP_PANEL_Thumb_Delete ) );
	if( pDelete != NULL )
		pDelete->MoveWindow( nLeft, nTop, nWidth, nHeight, TRUE );

	nTop += (nHeight +5);
	//查看按钮
	CButton* pView = (CButton*)( GetDlgItem( IDC_BUTTON_IMP_PANEL_Thumb_View ) );
	if( pView != NULL )
		pView->MoveWindow( nLeft, nTop, nWidth, nHeight, TRUE );

	if( nWidth == 100 )
		nLeft = rc.left + 150;
	else
		nLeft = rc.left + rc.Width() /5;
	if( rc.Height() -6 > ITEM_HEIGHT +30 )
	{
		nHeight	= ITEM_HEIGHT +30;
		nTop	= rc.top + (rc.Height() -nHeight) /2;
	}
	else
	{
		nHeight = rc.Height() -6;
		nTop	= rc.top +3;
	}
	//列表控件
	CListCtrl* pList = (CListCtrl*)( GetDlgItem( IDC_LIST_IMP_PANEL_Thumb ) );
	if( pList != NULL )
		pList->MoveWindow( nLeft, nTop, rc.right -nLeft -3, nHeight, TRUE );
}

void CDlgImpPanelThumb::AddItem( CxImage *pImage, CString strName )
{
	// hold the window update to avoid flicking
	m_ListCtrlThumb.SetRedraw(FALSE);

	CThumbImageItem* pThumbImageItem = new CThumbImageItem();
//	pThumbImageItem->m_pImageSrc = pImage;//->Copy( *pImage );
//	pThumbImageItem->m_pImageThumb = pImage;//->Copy( *pImage );
	pThumbImageItem->m_pImageSrc = new CxImage();
	pThumbImageItem->m_pImageSrc->Copy( *pImage );
	pThumbImageItem->m_pImageThumb = new CxImage();
	pThumbImageItem->m_pImageThumb->Copy( *pImage );

	pThumbImageItem->m_strPathName = strName;
	pThumbImageItem->m_strTitle = pThumbImageItem->GetTitleFromPathName( strName );
	pThumbImageItem->SaveTempFile();

	RGBQUAD canvas = {0,0,0,0};
	pThumbImageItem->m_pImageThumb->Thumbnail( THUMB_WIDTH, THUMB_HEIGHT, canvas);
	m_ListCtrlThumb.m_plistThumbImageItem->AddTail( pThumbImageItem );

	Reset_ImageList_ListCtrl();
	Update_ImageList_ListCtrl( m_ListCtrlThumb.m_plistThumbImageItem->GetCount() );

	m_ListCtrlThumb.SetRedraw(TRUE);
}

void CDlgImpPanelThumb::AddItem( CThumbImageItem* pThumbImageItem )
{
	m_ListCtrlThumb.SetRedraw(FALSE);

	// 保存临时文件
	pThumbImageItem->SaveTempFile();

	// 生成缩略图
	RGBQUAD canvas = {0,0,0,0};
	pThumbImageItem->m_pImageThumb = new CxImage();
	pThumbImageItem->m_pImageThumb->Copy( *pThumbImageItem->m_pImageSrc );
	pThumbImageItem->m_pImageThumb->Thumbnail( THUMB_WIDTH, THUMB_HEIGHT, canvas);
	m_ListCtrlThumb.m_plistThumbImageItem->AddTail( pThumbImageItem );

	// 刷新
	Reset_ImageList_ListCtrl();
	Update_ImageList_ListCtrl( m_ListCtrlThumb.m_plistThumbImageItem->GetCount() );

	m_ListCtrlThumb.SetRedraw(TRUE);
}

void CDlgImpPanelThumb::Reset_ImageList_ListCtrl()
{
	UINT uCount = m_ImageListThumb.GetImageCount();
	// reset our image list
	for( UINT i = 0; i<uCount; i++ )
		m_ImageListThumb.Remove(i);	
	// remove all items from list view
	if( m_ListCtrlThumb.GetItemCount() != 0 )
		m_ListCtrlThumb.DeleteAllItems();
}

void CDlgImpPanelThumb::Update_ImageList_ListCtrl( UINT uCount )
{
	// set the size of the image list
	m_ImageListThumb.SetImageCount( uCount );

	// set the length of the space between thumbnails
	// you can also calculate and set it based on the length of your list control
	int nGap	= 10;
	CPaintDC	cdc(this);
	CBitmap*    pBmp = NULL;
	HBITMAP		hBmp = NULL;
	POINT		pt;

	POSITION pos = m_ListCtrlThumb.m_plistThumbImageItem->GetHeadPosition();

	for( UINT i=0; i<uCount; i++ )
	{
		// load the bitmap
		CThumbImageItem* pitem = m_ListCtrlThumb.m_plistThumbImageItem->GetNext( pos );
		hBmp = pitem->m_pImageThumb->MakeBitmap(m_ListCtrlThumb.GetDC()->m_hDC);

		// attach the thumbnail bitmap handle to an CBitmap object
		pBmp = new CBitmap();		 
		pBmp->Attach(hBmp);

		// add bitmap to our image list
		m_ImageListThumb.Replace(i, pBmp, NULL);

		// put item to display
		// set the image file name as item text
		m_ListCtrlThumb.InsertItem(i, pitem->m_strTitle, i);

		// get current item position	 
		m_ListCtrlThumb.GetItemPosition(i, &pt);	 
	  
		// shift the thumbnail to desired position
		pt.x = nGap + i*(ITEM_WIDTH + nGap);
		pt.y = 0;
		m_ListCtrlThumb.SetItemPosition(i, pt);
			
		delete pBmp;
		if (hBmp) DeleteObject(hBmp);
	}
}

void CDlgImpPanelThumb::OnThumbSave() 
{
	// TODO: Add your control notification handler code here
	UINT uCount = m_ListCtrlThumb.GetSelectedCount();
	if (uCount == 0)
		return;

	POSITION posListCtrl	= m_ListCtrlThumb.GetFirstSelectedItemPosition();
	POSITION posListItem	= NULL;
	int nItem = -1;

	// hold the window update to avoid flicking
	m_ListCtrlThumb.SetRedraw(FALSE);

	// 找到第一个ListThumbImageItem中被选中的项目
	while( posListCtrl )
	{
		nItem = m_ListCtrlThumb.GetNextSelectedItem(posListCtrl);
		posListItem = m_ListCtrlThumb.m_plistThumbImageItem->FindIndex( nItem );
		if( posListItem != NULL )
		{
			CThumbImageItem* pitem = m_ListCtrlThumb.m_plistThumbImageItem->GetAt( posListItem );
			pitem->m_bSel = TRUE;

			DoSave( pitem );
//			break;
		}
	}

	Reset_ImageList_ListCtrl();
	Update_ImageList_ListCtrl( m_ListCtrlThumb.m_plistThumbImageItem->GetCount() );

	// let's show the new thumbnails
	m_ListCtrlThumb.SetRedraw(TRUE);
	((CButton*)(GetDlgItem(IDC_CHECK_IMP_PANEL_Thumb_SelAll)))->SetCheck(0);
}

BOOL CDlgImpPanelThumb::DoSave( CThumbImageItem* pImageItem )
{
	// 找到对应的ImageDoc文档
	BOOL bHasImageWindow = FALSE;
	CDocument* pDoc = NULL;
	CString strdocTitle = pImageItem->GetDefaultTitle();
	POSITION Pos = theApp.m_pSemImageDocTemplate->GetFirstDocPosition();
	while( Pos )
	{
		pDoc = theApp.m_pSemImageDocTemplate->GetNextDoc( Pos );
		if( pDoc->GetTitle() == strdocTitle )
		{
			bHasImageWindow = TRUE;
			break;
		}
	}
	if( bHasImageWindow && pDoc != NULL )
	{
		if( pDoc->DoSave("") )
		{
			pImageItem->m_bSel = FALSE;
			return TRUE;
		}
		else
			return FALSE;
	}

	int nDocType;
	UINT nIDSTitle;
	CString strNewName, strDocTitle;

	///////////////////////////////////////////////////////////////////////////
	// 存储前的文件名及文件类型初始化
	if( pImageItem->m_strPathName.IsEmpty() )
	{
		// 对话框标题
		nIDSTitle = AFX_IDS_SAVEFILE;

		// 文件存储类型
		nDocType = theApp.GetTypeFromIndex( theApp.m_nPriorSaveType, FALSE );

		// 文件名
		CString csExt = theApp.GetExtFromType(nDocType);
		int nPosStar = csExt.Find( '.' );
		int nPosSemicolon = csExt.Find( ';' );
		if( nPosSemicolon == -1 )
			nPosSemicolon = csExt.GetLength();
		csExt = csExt.Mid( nPosStar +1, nPosSemicolon -nPosStar -1 );

		strNewName.Format( "%s.%s", pImageItem->GetDefaultTitle(), csExt );
	}
	else
	{	
		// 对话框标题
		nIDSTitle = AFX_IDS_SAVEFILECOPY;

		// 文件存储类型
		// 由于新打开的sem格式图像，其IsSemImage()返回值为FALSE,
		// 故加入对后缀的判断
		CString ext(theApp.FindExtension(pImageItem->m_strPathName));
		ext.MakeLower();
		if( !(pImageItem->IsSemImage()) && ext != "sem" )
			nDocType = pImageItem->m_pImageSrc->GetType();
		else
			nDocType = SEMIMAGE_FORMAT_SEM;

		// 文件名
		strNewName = pImageItem->GetDefaultTitle();
	}

	///////////////////////////////////////////////////////////////////////////
	// 存储提示
	CString strPrompt1, strPrompt2, strTitle;
	strPrompt1 = GetResString( IDS_SD_PROMPT_DOSAVE1 );
	strPrompt2 = GetResString( IDS_SD_PROMPT_DOSAVE2 );
	strTitle = GetResString( IDS_SD_PROMPT_DOSAVE_TITLE );

PROMPT_SAVE:
	if (!theApp.PromptForFileName(strNewName, nIDSTitle,
		OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, &nDocType))
	{
		pImageItem->m_bSel = FALSE;
		return FALSE;       // don't even try to save
	}
	if( pImageItem->m_strPathName.IsEmpty()				//新文件
		&& (nDocType != SEMIMAGE_FORMAT_SEM) )			//意图保存为标准格式
	{
		CDlgPrompt dlg;
		dlg.m_byteType = (BYTE)0x04;
		dlg.m_strMsg = strPrompt1;
		dlg.m_strTitle = strTitle;
//		if( dlg.DoModal() == IDCANCEL )
//			goto PROMPT_SAVE;
	}
	else if( pImageItem->m_strPathName.IsEmpty()
		&& (nDocType == SEMIMAGE_FORMAT_SEM) )
	{
		// 提示sem格式是否保存图形层
// 		CDlgPrompt dlg;
// 		dlg.m_byteType = (BYTE)0x04;
// 		dlg.m_strMsg = strPrompt2;
// 		dlg.m_strTitle = strTitle;
// 		if( dlg.DoModal() == IDYES )
// 			theApp.m_blSaveGraphLay = TRUE;
// 		else
			theApp.m_blSaveGraphLay = FALSE;
	}
	if( (!(pImageItem->m_strPathName.IsEmpty())			//已保存文件
			&& (pImageItem->m_pImageSrc->GetType() != SEMIMAGE_FORMAT_SEM)//标准格式
			&& (nDocType == SEMIMAGE_FORMAT_SEM) ) )	//另存为sem格式
	{
		//将标准格式另存为sem格式时提示输入加速电压、放大倍数
		CDlgInputAcq dlg;
		if( dlg.DoModal() == IDOK )
		{
			pImageItem->m_nHV	= dlg.m_nHV;
			pImageItem->m_nMag	= dlg.m_nMag;
		}
		else
			goto PROMPT_SAVE;
	}
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	// 实际存储过程
	BeginWaitCursor();
	CString filename(strNewName);
	CString ext(theApp.FindExtension(filename));
	ext.MakeLower();
	if (ext == "") 
	{
		pImageItem->m_bSel = FALSE;
		return FALSE;
	}

	int nType = CxImage::GetTypeIdFromName(ext);
	if (nType == CXIMAGE_FORMAT_UNKNOWN)
		nType = SEMIMAGE_FORMAT_SEM;

	CString strError = _T("");
	BOOL bRet = pImageItem->SaveImage( filename, nType, strError );
	if( bRet )
	{
		if( bHasImageWindow  && pDoc != NULL )
		{
			pDoc->SetPathName( pImageItem->m_strPathName );
//			pdoc->SetTitle( pImageItem->GetDefaultTitle() );
			pDoc->SetModifiedFlag(FALSE);
		}
	}
	else
	{
		CDlgPrompt dlg;
		dlg.m_byteType = (BYTE)0x01;
		dlg.m_strMsg = strError;
		dlg.DoModal();
	}
	SaveImageDataSrc( pImageItem );

	EndWaitCursor();
	///////////////////////////////////////////////////////////////////////////

	pImageItem->m_bSel = FALSE;
	return TRUE;
}

void CDlgImpPanelThumb::SaveImageDataSrc( CThumbImageItem* pImageItem )
{
	// 2021.06 原始数据存储
	if( theApp.m_nSaveSrc > 0 )
	{
		// 将原始数据存储为同名的csv格式文件
		/////////////////////////////////////////////
		// 设置原始数据存储文件
		TCHAR exeFullPath[255];
		sprintf( exeFullPath, "%s", pImageItem->m_strPathName );
		char *ptemp = strrchr( exeFullPath, '.');
		ptemp[1] = 0;
		strcat( exeFullPath, "csv" );
		HANDLE hFileSrc = CreateFile(exeFullPath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		if( hFileSrc != NULL )
		{
			unsigned long cbRet;
			char buf[255], bufPix[64]= {0}, bufLine[4096*7] = {0};	// 1个数据最多由4个字符+，+空格组成
			// 头
			if( pImageItem->m_pPixZsrc2 != NULL )
				sprintf( buf, "X\tY\tZa\tZb\r\n" );
			else
				sprintf( buf, "X\tY\tZ\r\n" );
			WriteFile( hFileSrc, buf, strlen(bufLine), &cbRet, NULL );
			int nWidth = pImageItem->m_pImageSrc->GetWidth();
			if( pImageItem->m_pPixZsrc2 != NULL )	// 分割图像要注意图像宽度
				nWidth /= 2;
			int nHeight = pImageItem->m_pImageSrc->GetHeight();
			int i,j;
			for( i=0; i<nHeight; i++ )
			{
				bufLine[0] = '\0';
				// 一行数据写一次
				for( j=0; j<nWidth; j++ )
				{
					sprintf( bufPix, "%d, ", pImageItem->m_pPixZsrc[ i*nWidth +j ] );
					strcat( bufLine, bufPix );
				}
				WriteFile( hFileSrc, bufLine, strlen(bufLine), &cbRet, NULL );
				// 如果是分割像，一行在结束第一通道后，再接上第二通道数据
				if( pImageItem->m_pPixZsrc2 != NULL )
				{
					bufLine[0] = '\0';
					for( j=0; j<nWidth; j++ )
					{
						sprintf( bufPix, "%d, ", pImageItem->m_pPixZsrc2[ i*nWidth +j ] );
						strcat( bufLine, bufPix );
					}
					WriteFile( hFileSrc, bufLine, strlen(bufLine), &cbRet, NULL );
				}
				// 换行
				sprintf( buf, "\r\n" );
				WriteFile( hFileSrc, buf, strlen(buf), &cbRet, NULL );
			}
			CloseHandle( hFileSrc );
		}
		/////////////////////////////////////////////
	}
}

void CDlgImpPanelThumb::OnThumbDelete() 
{
	// TODO: Add your control notification handler code here
	UINT uCount = m_ListCtrlThumb.GetSelectedCount();
	if (uCount == 0)
		return;

	POSITION posListCtrl	= m_ListCtrlThumb.GetFirstSelectedItemPosition();
	POSITION posListItem	= NULL;
	int nItem = -1;

	// hold the window update to avoid flicking
	m_ListCtrlThumb.SetRedraw(FALSE);

	// 找到ListThumbImageItem中被选中的项目
	while( posListCtrl )
	{
		nItem = m_ListCtrlThumb.GetNextSelectedItem(posListCtrl);
		posListItem = m_ListCtrlThumb.m_plistThumbImageItem->FindIndex( nItem );
		if( posListItem != NULL )
		{
			CThumbImageItem* pitem = m_ListCtrlThumb.m_plistThumbImageItem->GetAt( posListItem );
			pitem->m_bSel = TRUE;
		}
	}
	// 删除ListThumbImageItem中被选中的项目并关闭相应的图像窗口
	CString strName;
	int nPos = -1;
	posListItem = m_ListCtrlThumb.m_plistThumbImageItem->GetHeadPosition();
	while( posListItem )
	{
		// 保存老位置
		POSITION oldpos = posListItem;
		// 获取项目指针
		CThumbImageItem* pitem = m_ListCtrlThumb.m_plistThumbImageItem->GetNext( posListItem );
		if( pitem->m_bSel )
		{
			if( !((CMainFrame *)theApp.GetMainWnd())->DestroyMDIChild( pitem->m_strTitle ) )
			{
				// 没有相应的图像窗口
				pitem->DeleteTempFile();
				m_ListCtrlThumb.m_plistThumbImageItem->RemoveAt( oldpos );
				delete pitem;
			}
		}
	}


/*	// 删除ImageList和ListCtrl中选中的项目
	// 由于之前判断了uCount不为0，故使用do...while
	do
	{
		// 由于GetNextSelectedItem不因之后的DeleteItem而更新，
		// 故pos需手动更新，否则会因pos不对而报错
		posListCtrl = m_ListCtrlThumb.GetFirstSelectedItemPosition();
		nItem = m_ListCtrlThumb.GetNextSelectedItem(posListCtrl);
		m_ListCtrlThumb.DeleteItem(nItem);
		m_ImageListThumb.Remove(nItem);
	} while( posListCtrl );

	// Update position: shift the thumbnail to desired position
	int nGap = 10;
	CPoint pt(0,0);
	uCount = m_ListCtrlThumb.GetItemCount();
	for (UINT i=0; i<uCount; i++)
	{
		pt.x = nGap + i*(ITEM_WIDTH + nGap);
		pt.y = 0;
		m_ListCtrlThumb.SetItemPosition(i, pt);
	}
*/
/*	// another way to delete mutiple items.
	int nItem = -1;
	do
	{
		nItem = m_ListCtrlThumb.GetNextItem( -1, LVNI_SELECTED );
		m_ListCtrlThumb.DeleteItem( nItem );
		m_ImageListThumb.Remove( nItem );
	} while( m_ListCtrlThumb.GetSelectedCount() != 0 );
*/
	Reset_ImageList_ListCtrl();
	Update_ImageList_ListCtrl( m_ListCtrlThumb.m_plistThumbImageItem->GetCount() );
	// let's show the new thumbnails
	m_ListCtrlThumb.SetRedraw(TRUE);
	((CButton*)(GetDlgItem(IDC_CHECK_IMP_PANEL_Thumb_SelAll)))->SetCheck(0);
}

void CDlgImpPanelThumb::DeleteItem( CString strTitle )
{
	// hold the window update to avoid flicking
	m_ListCtrlThumb.SetRedraw(FALSE);

	//获得项目列表头位置
	POSITION pos = m_ListCtrlThumb.m_plistThumbImageItem->GetHeadPosition();
	while( pos )
	{
		//保存老位置
		POSITION oldpos = pos;
		//获取项目指针
		CThumbImageItem* pitem = m_ListCtrlThumb.m_plistThumbImageItem->GetNext( pos );

		CString strItemTitle = pitem->GetDefaultTitle();
		if( strItemTitle == strTitle )
		{
			// 删除临时文件
			pitem->DeleteTempFile();
			// 从项目列表中删除该项目
			m_ListCtrlThumb.m_plistThumbImageItem->RemoveAt( oldpos );
			delete pitem;
			break;
		}
	}

	Reset_ImageList_ListCtrl();
	Update_ImageList_ListCtrl( m_ListCtrlThumb.m_plistThumbImageItem->GetCount() );

	// let's show the new thumbnails
	m_ListCtrlThumb.SetRedraw(TRUE);
}

void CDlgImpPanelThumb::ChangeItemName( CString strOldName, CString strNewName )
{
	// 改变项目列表中对应项的名字
	CString strTitle;
	int nItem = 0;
	POSITION pos = m_ListCtrlThumb.m_plistThumbImageItem->GetHeadPosition();
	while( pos )
	{
		CThumbImageItem* pitem = m_ListCtrlThumb.m_plistThumbImageItem->GetNext( pos );

		strTitle = pitem->GetDefaultTitle();
		if( strTitle == strOldName )
		{
			pitem->m_strPathName = strNewName;
			strTitle = pitem->GetTitleFromPathName( strNewName );
			pitem->m_strTitle = strTitle;
			// 2021.06 原始数据存储
			SaveImageDataSrc( pitem );
			break;
		}
		nItem++;
	}

	// 改变ListCtrl中对应项的名字
	m_ListCtrlThumb.SetItemText( nItem, 0, strTitle );
}

void CDlgImpPanelThumb::OnThumbView() 
{
	// TODO: Add your control notification handler code here
	int nItem = -1;
	POSITION pos = m_ListCtrlThumb.GetFirstSelectedItemPosition();
	if( pos )
	{
		nItem = m_ListCtrlThumb.GetNextSelectedItem(pos);
		pos = m_ListCtrlThumb.m_plistThumbImageItem->FindIndex( nItem );
		if( pos != NULL )
		{
			CThumbImageItem* pitem = m_ListCtrlThumb.m_plistThumbImageItem->GetAt( pos );

			CString strName = pitem->m_strPathName;
			if( strName.IsEmpty() )
				strName = pitem->m_strTitle;
			else
			{
				int nPos = strName.Find( '\\' );
				while( nPos != -1 )
				{
					strName = strName.Right( strName.GetLength() - nPos - 1 );
					nPos = strName.Find( '\\' );
				}
			}

			if( !((CMainFrame *)theApp.GetMainWnd())->ActivateMDIChild( strName ) )
				ShowItem( pitem );
			pitem->m_bSel = FALSE;

//			theApp.LinkImageProcess( pitem );
		}
	}
	((CButton*)(GetDlgItem(IDC_CHECK_IMP_PANEL_Thumb_SelAll)))->SetCheck(0);
}

void CDlgImpPanelThumb::OnDblclkLISTThumb(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nItem = m_ListCtrlThumb.GetNextItem(-1, LVNI_FOCUSED);
	if( nItem > -1 )
	{
		POSITION pos = m_ListCtrlThumb.m_plistThumbImageItem->FindIndex( nItem );
		if( pos != NULL )
		{
			CThumbImageItem* pitem = m_ListCtrlThumb.m_plistThumbImageItem->GetAt( pos );

			CString strName = pitem->m_strPathName;
			int nPos = strName.Find( '\\' );
			while( nPos != -1 )
			{
				strName = strName.Right( strName.GetLength() - nPos - 1 );
				nPos = strName.Find( '\\' );
			}

			if( !((CMainFrame *)theApp.GetMainWnd())->ActivateMDIChild( strName ) )
				ShowItem( pitem );
			pitem->m_bSel = FALSE;
		}
	}
	
	*pResult = 0;
}

void CDlgImpPanelThumb::ShowItem( CThumbImageItem* pThumbImageItem )
{
	CImageDoc *NewDoc=(CImageDoc*)theApp.m_pSemImageDocTemplate->OpenDocumentFile(NULL);
	BeginWaitCursor();
	if (NewDoc)
	{
		NewDoc->m_nMag	= pThumbImageItem->m_nMag;
		NewDoc->m_nHV	= pThumbImageItem->m_nHV;
		NewDoc->m_ac	= pThumbImageItem->m_ac;
		const double dblDPI = 25.4 * pThumbImageItem->m_pImageSrc->GetWidth() / 127;
		
		if( pThumbImageItem->m_pImageLayer != NULL )
		{
			NewDoc->ImageLayer	= new CxImage();
			NewDoc->ImageLayer->Copy( *pThumbImageItem->m_pImageLayer );
			NewDoc->ImageLayer->SetXDPI( (long)dblDPI );
			NewDoc->ImageLayer->SetYDPI( (long)dblDPI );
		}
		NewDoc->image		= new CxImage();
		NewDoc->image->Copy( *pThumbImageItem->m_pImageSrc );
		NewDoc->image->SetXDPI( (long)dblDPI );
		NewDoc->image->SetYDPI( (long)dblDPI );
		NewDoc->SetTitle( pThumbImageItem->GetDefaultTitle() );
		if( !pThumbImageItem->m_strPathName.IsEmpty() )
			NewDoc->SetPathName( pThumbImageItem->m_strPathName );
		
		NewDoc->SetModifiedFlag(FALSE);
		NewDoc->UpdateAllViews(0,WM_USER_NEWIMAGE);
		NewDoc->UpdateScale();
		NewDoc->UpdateStatusBar();
	}
	EndWaitCursor();
}

void CDlgImpPanelThumb::OnThumbSelAll() 
{
	// TODO: Add your control notification handler code here
	int nCheck = ((CButton*)(GetDlgItem(IDC_CHECK_IMP_PANEL_Thumb_SelAll)))->GetCheck();
	int nCount = m_ListCtrlThumb.GetItemCount();
	if( nCount == 0 )
	{
		if( nCheck )
			((CButton*)(GetDlgItem(IDC_CHECK_IMP_PANEL_Thumb_SelAll)))->SetCheck(0);
		return;
	}

	m_ListCtrlThumb.SetRedraw(FALSE);
	if( nCheck )
	{
		for(int i=0; i<nCount; i++ )
			m_ListCtrlThumb.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		theApp.DisplayMsg( "Press select All" );
	}
	else
	{
		for(int i=0; i<nCount; i++ )
			m_ListCtrlThumb.SetItemState(i, ~LVIS_SELECTED, LVIS_SELECTED);
		theApp.DisplayMsg( "Press unselect All" );
	}

	m_ListCtrlThumb.SetRedraw(TRUE);
	POSITION pos = m_ListCtrlThumb.GetFirstSelectedItemPosition();
}
