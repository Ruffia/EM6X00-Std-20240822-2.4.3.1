// DlgLanguage.cpp : implementation file
//

#include "stdafx.h"
#include "DllTest.h"
#include "DlgLanguage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLanguage dialog


CDlgLanguage::CDlgLanguage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLanguage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLanguage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgLanguage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLanguage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLanguage, CDialog)
	//{{AFX_MSG_MAP(CDlgLanguage)
	ON_BN_CLICKED(IDC_BUTTON_Browse_hFile, OnBUTTONBrowsehFile)
	ON_BN_CLICKED(IDC_BUTTON_Browse_rcFile, OnBUTTONBrowsercFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLanguage message handlers

/////////////////////////////////////////////////////////////////////
//功能：从文本文件中取一行文本地函数 
//输入 
// fp: 文件指针，输入时保证非空 
// szLine: 返回行的指针，长度为MAX_LINE_LENGTH 
//返回值 该行的字符数
#define	MAX_LINE_LENGHT	255

BOOL GetLine(FILE* fp,char* szLine, int& nIndex)
{
	nIndex = 0;
	BOOL bEOF = FALSE;
	int ch = fgetc(fp);
	while(nIndex < MAX_LINE_LENGHT)
	{
		if(feof(fp) != 0)
		{
			bEOF = TRUE;
			break;
		}
		else if (ch == '\n')
			break;
		else
		{
			szLine[nIndex++] = (char)ch;
			ch = fgetc(fp);
		}
	}
	szLine[nIndex] = 0;
	return bEOF;
}

void CDlgLanguage::OnBUTTONBrowsehFile() 
{
	CFileDialog dlg(TRUE, "h", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, ".h文件 (*.h)|*.h||", NULL);
	if( dlg.DoModal() != IDOK )
		return;

	CString csFullPath = dlg.GetPathName();
	FILE *streamSrc, *streamCvt;	// source & convert
	streamSrc = fopen( (LPCSTR)csFullPath, "r" );
	if( streamSrc == NULL )
		return;

	char szBuf[255];
	sprintf( szBuf, "%s", csFullPath );
	char *exd = strrchr( szBuf, '.' );
	if(exd != NULL)
		*exd = '\0';
	strcat( szBuf, ".lang" );
	streamCvt = fopen( szBuf, "w" );
	if( streamCvt == NULL )
	{
		fclose( streamSrc );
		return;
	}
	sprintf( szBuf, "%s", csFullPath );
	GetDlgItem(IDC_EDIT_Path_hFile)->SetWindowText( szBuf );

	char szTemp[255], szBuf2[255];
	unsigned long cbRet = 0;
	sprintf( szBuf2, "%s\n", "[IDs]" );
	fwrite( szBuf2, sizeof( char ), strlen(szBuf2), streamCvt );

	int nIndex = 0;
	BOOL bEOF;
	do
	{
		bEOF = GetLine( streamSrc, szBuf, nIndex );
		if( bEOF || nIndex < 1 )
			break;
		// #define IDS_CHECK_Vacuum_VACUUM_SWITCH  2371
		strcpy(szTemp, szBuf);
		if(strchr(szTemp, ' ') != NULL)
		{
			strcpy(szBuf, strchr(szTemp, ' ')+1);	// IDS_CHECK_Vacuum_VACUUM_SWITCH  2371
//			*strchr(szTemp, ' ') = '\0';			// #define
			
			strcpy(szTemp, szBuf);
			if(strchr(szTemp, ' ') != NULL)
			{
				strcpy(szBuf, strchr(szTemp, ' ')+1);	//   2371
				*strchr(szTemp, ' ') = '\0';			// IDS_CHECK_Vacuum_VACUUM_SWITCH
				if( strchr(szBuf, 'x') != NULL )		// 0x8100
				{
					strcpy( szBuf2, strchr(szBuf,'x') +1 );	// 8100 (hex)
					int nID = 0;
					sscanf(szBuf2, "%04X", &nID);
					sprintf( szBuf2, "%d=%s\n", nID, szTemp );
				}
				else
					sprintf( szBuf2, "%d=%s\n", atoi(szBuf), szTemp );
				fwrite( szBuf2, sizeof( char ), strlen(szBuf2), streamCvt );
			}
		}
	}while( !bEOF );
	fclose( streamSrc );
	fclose( streamCvt );
}

void CDlgLanguage::OnBUTTONBrowsercFile() 
{
	CFileDialog dlg(TRUE, "rc", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, ".rc文件 (*.rc)|*.rc||", NULL);
	if( dlg.DoModal() != IDOK )
		return;

	CString csFullPath = dlg.GetPathName();
	FILE *streamSrc, *streamCvt;	// source & convert
	streamSrc = fopen( (LPCSTR)csFullPath, "r" );
	if( streamSrc == NULL )
		return;

	char szBuf[255];
	sprintf( szBuf, "%s", csFullPath );
	char *exd = strrchr( szBuf, '.' );
	if(exd != NULL)
		*exd = '\0';
	strcat( szBuf, ".lang" );
	streamCvt = fopen( szBuf, "w" );
	if( streamCvt == NULL )
	{
		fclose( streamSrc );
		return;
	}
	sprintf( szBuf, "%s", csFullPath );
	GetDlgItem(IDC_EDIT_Path_rcFile)->SetWindowText( szBuf );

	char szTemp[255], szBuf2[255];
	unsigned long cbRet = 0;
	sprintf( szBuf2, "%s\n", "[Strings]" );
	fwrite( szBuf2, sizeof( char ), strlen(szBuf2), streamCvt );

	int nIndex = 0;
	BOOL bEOF;
	do
	{
		bEOF = GetLine( streamSrc, szBuf, nIndex );
		if( bEOF || nIndex < 1 )
			break;
		// IDS_DLG_Colorize                "Colorize"
		if(strchr(szBuf, '"') != NULL)
		{
			strcpy(szBuf2, strchr(szBuf, '"')+1);	// Colorize"
			*strchr(szBuf, '"') = '\0';			// IDS_DLG_Colorize
			
			szBuf2[strlen(szBuf2) -1] = '\0';
			sprintf( szTemp, "%s=%s\n", szBuf, szBuf2 );
			fwrite( szTemp, sizeof( char ), strlen(szTemp), streamCvt );
		}
	}while( !bEOF );
	fclose( streamSrc );
	fclose( streamCvt );
}
