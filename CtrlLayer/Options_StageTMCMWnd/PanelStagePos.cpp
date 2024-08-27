#include "StdAfx.h"
#include "Options_StageTMCMWnd.h"
#include "PanelStage.h"
#include "DlgInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define	NUM_COLUMNS		6



void CPanelStage::List_SetHead( int nIndex, CString strName )
{
	m_strPosListHead[nIndex] = strName;
}

void CPanelStage::List_Init()
{
	m_nPosColWidth[0] = 10;
	m_nPosColWidth[1] = 10;
	m_nPosColWidth[2] = 10;
	m_nPosColWidth[3] = 10;
	m_nPosColWidth[4] = 10;
	m_nPosColWidth[5] = 10;

	m_plistTMCMPos	= new ListTMCMPos;
	// Enable the full row selection and the drag drop of headers.
	DWORD styles = LVS_EX_FULLROWSELECT;
	// Use macro since this is new and not in MFC.
	ListView_SetExtendedListViewStyleEx(m_listPos.m_hWnd, styles, styles );

	CString str;
	char szLabel[10];
	TEXTMETRIC tm;
	//��ȡ�ͻ���CDC
	CClientDC dc(this);
	//������ѡ��
	CFont* pOldFont=dc.SelectObject(GetFont());
	//���TEXTMETRIC����
	dc.GetTextMetrics(&tm);
	//������ָ�
	dc.SelectObject(pOldFont);
	//�����б�����
	LV_COLUMN lvc;
	lvc.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	//���б���������
	for(int n=0; n<NUM_COLUMNS; n++)
	{
		lvc.iSubItem=n;
		sprintf_s( szLabel, "%s", m_strPosListHead[n] );
		lvc.pszText = szLabel;
		lvc.cx = tm.tmAveCharWidth *( m_nPosColWidth[n] );
		if( n== 0 )
			lvc.fmt = LVCFMT_LEFT;
		else
			lvc.fmt = LVCFMT_RIGHT;
		m_listPos.InsertColumn(n,&lvc);
	}

	List_Load();
	List_Update();
}

void CPanelStage::List_Load()
{
	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\Stage\\StageTMCM.ini");

    m_nPosTotal = GetPrivateProfileInt( "StageTMCMPos", "Total", 0, path );
	if( m_nPosTotal == 0 )
		return;

	int nTotal = m_nPosTotal;
//	LV_ITEM lvi;
	char	szBuf1[255], szBuf2[255], szBufRootSection[20], szBufRootKey[20];
	sprintf_s(szBufRootSection, "StageTMCMPos");
	for( int i=0; i<nTotal; i++ )
	{
		sprintf_s(szBufRootKey, "pos%d", i);
		if(GetPrivateProfileString(szBufRootSection, szBufRootKey, "", szBuf1, 255, path) != 0)
		{
			structTMCMPos* pPos = new structTMCMPos();
			strcpy_s(szBuf2, szBuf1);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy_s(szBuf1, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				pPos->strName = szBuf2;
			}
			strcpy_s(szBuf2, szBuf1);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy_s(szBuf1, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				pPos->fPos[0] = (float)atof(szBuf2);
			}
			strcpy_s(szBuf2, szBuf1);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy_s(szBuf1, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				pPos->fPos[1] = (float)atof(szBuf2);
			}
			strcpy_s(szBuf2, szBuf1);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy_s(szBuf1, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				pPos->fPos[2] = (float)atof(szBuf2);
			}
			strcpy_s(szBuf2, szBuf1);
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy_s(szBuf1, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				pPos->fPos[3] = (float)atof(szBuf2);
			}
			pPos->fPos[4] = (float)atof(szBuf1);
			m_plistTMCMPos->AddTail( pPos );

/*			ZeroMemory(&lvi,sizeof(LV_ITEM));
			lvi.mask = LVIF_TEXT;
			lvi.iItem = m_listPos.GetItemCount();
			lvi.iSubItem = 0;
			(LPCTSTR&)lvi.pszText = pPos->strName;
			lvi.cchTextMax=strlen(lvi.pszText);
			m_listPos.InsertItem(&lvi);
			m_listPos.SetItem(&lvi);*/
		}
	}
}

void CPanelStage::List_ChangeName( int nItem, CString strName )
{
	CString strOldName = m_listPos.GetItemText(nItem, 0);
	//�����б�����
	LV_ITEM lvi;
	ZeroMemory(&lvi,sizeof(LV_ITEM));
	lvi.mask = LVIF_TEXT;
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
//	pItem->GetName( string );
	(LPCTSTR&)lvi.pszText = strName;
	lvi.cchTextMax=strlen(lvi.pszText);
	m_listPos.SetItem(&lvi);

	// �ҵ�ָ��λ�ò����б���ɾ��
	int nCounts = 0;
	POSITION pos = m_plistTMCMPos->GetHeadPosition();
	while( pos )
	{
		structTMCMPos* pPos = m_plistTMCMPos->GetAt( pos );
		if( nCounts == nItem && pPos->strName == strOldName )
		{
			pPos->strName = strName;
			break;
		}
		m_plistTMCMPos->GetNext( pos );
		nCounts++;
	}
}

void CPanelStage::OnBnClickedButtonListadd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	structTMCMPos* pPos = new structTMCMPos();
	CString str;
	str.Format( "Pos %d", ++m_nPosTotal );
	pPos->strName = str;
	pPos->fPos[0] = (float)(m_fCurStep[0] /m_dEquivalent[0] *m_paramDriveDir.nDir[0]);
	pPos->fPos[1] = (float)(m_fCurStep[1] /m_dEquivalent[1] *m_paramDriveDir.nDir[1]);
	pPos->fPos[2] = (float)(m_fCurStep[2] /m_dEquivalent[2] *m_paramDriveDir.nDir[2]);
	pPos->fPos[3] = (float)(m_fCurStep[3] /m_dEquivalent[3] *m_paramDriveDir.nDir[3]);
	pPos->fPos[4] = (float)(m_fCurStep[4] /m_dEquivalent[4] *m_paramDriveDir.nDir[4]);
	m_plistTMCMPos->AddTail( pPos );
	List_Update();
}

void CPanelStage::OnBnClickedButtonListdel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ��û��ѡ��λ��
	POSITION pos = m_listPos.GetFirstSelectedItemPosition();
	if( pos == NULL )
		return;

	// ��ȡָ��λ�����ֲ�ɾ��
	CString str;
	int nItem = -1;
	while( pos )
		nItem = m_listPos.GetNextSelectedItem(pos);
	str = m_listPos.GetItemText( nItem, 0 );
	m_listPos.DeleteItem( nItem );
	// �ҵ�ָ��λ�ò����б���ɾ��
	pos = m_plistTMCMPos->GetHeadPosition();
	while( pos )
	{
		structTMCMPos* pPos = m_plistTMCMPos->GetAt( pos );
		if( pPos->strName == str )
		{
			m_plistTMCMPos->RemoveAt( pos );
			delete pPos;
			break;
		}
		m_plistTMCMPos->GetNext( pos );
	}

	m_nPosTotal = m_plistTMCMPos->GetCount();
	List_Update();
}

void CPanelStage::OnBnClickedButtonListchange() 
{
	// TODO: Add your control notification handler code here
	int nCounts = m_listPos.GetSelectedCount();
	if( nCounts > 0 )
	{
		int nItem = -1;
		nItem = m_listPos.GetNextItem( nItem, LVNI_SELECTED );
		ASSERT(nItem != -1);
		CDlgInput dlg;
		dlg.m_strInput = m_listPos.GetItemText( nItem, 0 );
		strcpy_s( dlg.m_cPathLocalize, m_cPathLocalize );
		if( dlg.DoModal() == IDOK )
		{
			List_ChangeName( nItem, dlg.m_strInput );
			List_Update();
		}
	}
}

void CPanelStage::List_Update()
{
	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat_s(path, "\\Stage\\StageTMCM.ini");

	// ����λ������
	int nTotal = m_nPosTotal;
	CString str;
	str.Format( "%d", nTotal );
	WritePrivateProfileString( "StageTMCMPos", "Total", str, path );  //��CString�ͱ�������Cfg.ini�ļ�������ֶ������м�����StageTMCMPos��

	m_listPos.DeleteAllItems();
	//��������ӵ�POS�ĸ��������б������Item
	POSITION pos = m_plistTMCMPos->GetHeadPosition();
	LV_ITEM lvi;
	int i=0;
	CString strPos;
	while( pos )
	{
		structTMCMPos* pPos = m_plistTMCMPos->GetNext( pos );

		// �������
		ZeroMemory(&lvi,sizeof(LV_ITEM));
		lvi.mask = LVIF_TEXT;
		lvi.iItem = m_listPos.GetItemCount();
/*
		lvi.iSubItem = 0;							//���
		string.Format( "%d", lvi.iItem +10 );
		(LPCTSTR&)lvi.pszText = string;
		lvi.cchTextMax = strlen(lvi.pszText);
		m_listPos.InsertItem(&lvi);
		m_listPos.SetItem(&lvi);
*/
		lvi.iSubItem = 0;							//��ǩ
		(LPCTSTR&)lvi.pszText = pPos->strName;
		lvi.cchTextMax = strlen(lvi.pszText);
		m_listPos.InsertItem(&lvi);
		m_listPos.SetItem(&lvi);

		lvi.iSubItem = 1;							//X
		str.Format( "%.6f", pPos->fPos[0]);
		(LPCTSTR&)lvi.pszText = str;
		lvi.cchTextMax = strlen(lvi.pszText);
		m_listPos.SetItem(&lvi);

		lvi.iSubItem = 2;							//Y
		str.Format("%.6f", pPos->fPos[1]);
		(LPCTSTR&)lvi.pszText = str;
		lvi.cchTextMax = strlen(lvi.pszText);
		m_listPos.SetItem(&lvi);

		lvi.iSubItem = 3;							//Z
		str.Format("%.6f", pPos->fPos[2]);
		(LPCTSTR&)lvi.pszText = str;
		lvi.cchTextMax = strlen(lvi.pszText);
		m_listPos.SetItem(&lvi);

		lvi.iSubItem = 4;							//R
		str.Format("%.1f", pPos->fPos[3]);
		(LPCTSTR&)lvi.pszText = str;
		lvi.cchTextMax = strlen(lvi.pszText);
		m_listPos.SetItem(&lvi);

		lvi.iSubItem = 5;							//T
		str.Format("%.1f", pPos->fPos[4]);
		(LPCTSTR&)lvi.pszText = str;
		lvi.cchTextMax = strlen(lvi.pszText);
		m_listPos.SetItem(&lvi);


		// д�������ļ�
		str.Format( "pos%d", i++ );
		strPos.Format( "%s,%.6f,%.6f,%.2f,%.1f,%.1f,", pPos->strName, pPos->fPos[0], pPos->fPos[1], pPos->fPos[2], pPos->fPos[3], pPos->fPos[4]);
		WritePrivateProfileString( "StageTMCMPos", str, strPos, path );
	}
}

void CPanelStage::OnDblclkListPosition(NMHDR *pNMHDR, LRESULT *pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	List_Goto( pDispInfo->item.mask );

	*pResult = 0;
}

void CPanelStage::List_Goto( int nItem )
{
	// ��ȡָ��λ������
	CString strName, str;
	strName = m_listPos.GetItemText( nItem, 0 );

	// �ҵ�ָ��λ�ò���ת
	POSITION pos = m_plistTMCMPos->GetHeadPosition();
	while( pos )
	{
		structTMCMPos* pPos = m_plistTMCMPos->GetAt( pos );
		if( pPos->strName == strName )
		{
			Message("Goto defined pos");
			//m_bRelativeBak = m_bRelative;
			m_bRelative = FALSE;	// �����Ծ���λ����ת
			Move5Axis(pPos->fPos[0]*m_paramDriveDir.nDir[0], pPos->fPos[1]*m_paramDriveDir.nDir[1], pPos->fPos[2]*m_paramDriveDir.nDir[2], pPos->fPos[3]*m_paramDriveDir.nDir[3], pPos->fPos[4]*m_paramDriveDir.nDir[4]);
			// ��λ��λ��
			break;
		}
		m_plistTMCMPos->GetNext( pos );
	}
}
