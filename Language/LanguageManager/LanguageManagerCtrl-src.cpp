// LanguageManagerCtrl.cpp : implementation file
//
// Last Modify : 2009.03.23

#include "stdafx.h"
#include "LanguageManagerCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


struct SLanguage {
	LANGID	lid;
	LPCTSTR pszLocale;
	BOOL	bSupported;
	LPCTSTR	pszISOLocale;
};


static SLanguage _aLanguages[] =
{
	{LANGID_AR_AE,	_T(""),				FALSE,	_T("ar_AE")},
	{LANGID_BG_BG,	_T("hun"),			FALSE,	_T("bg_BG")},
	{LANGID_HU_HU,	_T("hungarian"),	FALSE,	_T("hu_HU")},
	{LANGID_CA_ES,	_T(""),				FALSE,	_T("ca_ES")},
	{LANGID_ZH_CN,	_T("chs"),			TRUE,	_T("zh_CN")},
	{LANGID_ZH_TW,	_T("cht"),			TRUE,	_T("zh_TW")},
	{LANGID_DA_DK,	_T("danish"),		FALSE,	_T("da_DK")},
	{LANGID_NL_NL,	_T("dutch"),		FALSE,	_T("nl_NL")},
	{LANGID_EN_US,	_T("english"),		TRUE,	_T("en_US")},
	{LANGID_ET_EE,	_T(""),				FALSE,	_T("et_EE")},
	{LANGID_FI_FI,	_T("finnish"),		FALSE,	_T("fi_FI")},
	{LANGID_FR_FR,	_T("french"),		FALSE,	_T("fr_FR")},
//	{LANGID_GL_ES,	_T(""),				FALSE,	_T("gl_ES")},
	{LANGID_DE_DE,	_T("german"),		FALSE,	_T("de_DE")},
	{LANGID_EL_GR,	_T("greek"),		FALSE,	_T("el_GR")},
	{LANGID_IT_IT,	_T("italian"),		FALSE,	_T("it_IT")},
	{LANGID_KO_KR,	_T("korean"),		FALSE,	_T("ko_KR")},
	{LANGID_LV_LV,	_T(""),				FALSE,	_T("lv_LV")},
	{LANGID_LT_LT,	_T(""),				FALSE,	_T("lt_LT")},
	{LANGID_NB_NO,	_T("norwegian"),	FALSE,	_T("nb_NO")},
	{LANGID_PL_PL,	_T("polish"),		FALSE,	_T("pl_PL")},
	{LANGID_PT_PT,	_T("ptg"),			FALSE,	_T("pt_PT")},
	{LANGID_PT_BR,	_T("ptb"),			FALSE,	_T("pt_BR")},
	{LANGID_RU_RU,	_T("russian"),		FALSE,	_T("ru_RU")},
	{LANGID_SL_SI,	_T(""),				FALSE,	_T("sl_SI")},
	{LANGID_ES_ES_T,_T("spanish"),		FALSE,	_T("es_ES_T")},
	{LANGID_SV_SE,	_T("swedish"),		FALSE,	_T("sv_SE")},
	{LANGID_TR_TR,	_T("turkish"),		FALSE,	_T("tr_TR")},
	{LANGID_HE_IL,	_T(""),				FALSE,	_T("he_IL")},
	{LANGID_JP_JP,	_T(""),				FALSE,	_T("jp_JP")},
	{LANG_CZECH,	_T(""),				FALSE,	_T("cz_CZ")},
	{0, NULL, 0}
};


/////////////////////////////////////////////////////////////////////////////
// CLanguageManagerCtrl

CLanguageManagerCtrl::CLanguageManagerCtrl()
{
	m_hLangDLL		= NULL;
	m_wLanguageID	= 0;
	m_strLangDir	= _T("");
}

CLanguageManagerCtrl::~CLanguageManagerCtrl()
{
	FreeLangDLL();
}



CString CLanguageManagerCtrl::GetResString(/*HINSTANCE _hLangDLL, */UINT uStringID, WORD languageID)
{
	CString resString;
/*	if (_hLangDLL)
		resString.LoadString(_hLangDLL, uStringID, languageID);
	if (resString.IsEmpty())
		resString.LoadString(GetModuleHandle(NULL), uStringID, LANGID_EN_US);
*/
	char sz[256];
	if (m_hLangDLL)
	{
		LoadString(m_hLangDLL, uStringID, sz, sizeof(sz));
		resString.Format( "%s", sz );
	}
	if (resString.IsEmpty())
	{
		LoadString(GetModuleHandle(NULL), uStringID, sz, sizeof(sz));
		resString.Format( "%s", sz );
	}

	return resString;
}

CString CLanguageManagerCtrl::GetResString(/*HINSTANCE _hLangDLL, */UINT uStringID)
{
	CString resString;
/*	if (_hLangDLL)
		resString.LoadString(_hLangDLL, uStringID);
	if (resString.IsEmpty())
		resString.LoadString(GetModuleHandle(NULL), uStringID);
*/
	char sz[256];
	if (m_hLangDLL)
	{
		LoadString(m_hLangDLL, uStringID, sz, sizeof(sz));
		resString.Format( "%s", sz );
	}
	if (resString.IsEmpty())
	{
		LoadString(GetModuleHandle(NULL), uStringID, sz, sizeof(sz));
		resString.Format( "%s", sz );
	}

	return resString;
}

void CLanguageManagerCtrl::FreeLangDLL()
{
	if (m_hLangDLL != NULL && m_hLangDLL != GetModuleHandle(NULL)){
		VERIFY( FreeLibrary(m_hLangDLL) );
		m_hLangDLL = NULL;
	}
}

BOOL CLanguageManagerCtrl::CheckLangDLLVersion(const CString& rstrLangDLL)
{
	bool bResult = false;
/*	DWORD dwUnused;
	DWORD dwVerInfSize = GetFileVersionInfoSize(const_cast<LPTSTR>((LPCTSTR)rstrLangDLL), &dwUnused);
	if (dwVerInfSize != 0)
	{
		LPBYTE pucVerInf = (LPBYTE)calloc(dwVerInfSize, 1);
		if (pucVerInf)
		{
			if (GetFileVersionInfo(const_cast<LPTSTR>((LPCTSTR)rstrLangDLL), 0, dwVerInfSize, pucVerInf))
			{
				VS_FIXEDFILEINFO* pFileInf = NULL;
				UINT uLen = 0;
				if (VerQueryValue(pucVerInf, _T("\\"), (LPVOID*)&pFileInf, &uLen) && pFileInf && uLen)
				{
					bResult = (pFileInf->dwProductVersionMS == theApp.m_dwProductVersionMS &&
                               pFileInf->dwProductVersionLS == theApp.m_dwProductVersionLS);
				}
			}
			free(pucVerInf);
		}
	}
*/
	bResult = true;
	// no messagebox anymore since we just offer the user to download the new one
	/*if (!bResult){
		CString strError;
		// Don't try to load a localized version of that string! ;)
		strError.Format(_T("Language DLL \"%s\" is not for this eMule version. Please update the language DLL!"), rstrLangDLL);
		AfxMessageBox(strError, MB_ICONSTOP);
	}*/

	return bResult;
}

BOOL CLanguageManagerCtrl::LoadLangLib(const CString& rstrLangDir, LANGID lid)
{
/*
//	if( _hLangDLL != NULL )
		FreeLangDLL();//FreeLibrary(_hLangDLL);//

	bool bLoadedLib = false;
	CString strLangDLL = rstrLangDir;
	strLangDLL += pLangs->pszISOLocale;
//	if( lid == 8 )
//		strLangDLL += _T("Lang_En");
	strLangDLL += _T(".dll");
//	if (CheckLangDLLVersion(strLangDLL))
	{
		_hLangDLL = LoadLibrary(strLangDLL);
		if (_hLangDLL)
			bLoadedLib = true;
	}
	if (bLoadedLib)
	{
	//_tsetlocale(LC_ALL, pLangs->pszLocale);
	//SetThreadLocale(lid);
		return true;
	}

	return false;
*/
	const SLanguage* pLangs = _aLanguages;
	if (pLangs){
		while (pLangs->lid){
			if (pLangs->bSupported && pLangs->lid == lid){
				FreeLangDLL();

				bool bLoadedLib = false;
				if (pLangs->lid == LANGID_ZH_CN){
					m_hLangDLL = NULL;
					bLoadedLib = true;
				}
				else{
					CString strLangDLL = rstrLangDir;
					strLangDLL += pLangs->pszISOLocale;
					strLangDLL += _T(".dll");
					if (CheckLangDLLVersion(strLangDLL)){
						m_hLangDLL = LoadLibrary(strLangDLL);
						if (m_hLangDLL)
							bLoadedLib = true;
					}
				}
				if (bLoadedLib){
					//_tsetlocale(LC_ALL, pLangs->pszLocale);
					//SetThreadLocale(lid);
					return true;
				}
				break;
			}
			pLangs++;
		}
	}
	return false;
}







void CLanguageManagerCtrl::Init()
{
	InitLanguageDir();
	LANGID lidLocale = (LANGID)::GetThreadLocale();
	SetLanguageID( lidLocale );
	LoadLangLib( GetLangDir(), lidLocale );   
}

void CLanguageManagerCtrl::Close()
{
	FreeLangDLL();
}

void CLanguageManagerCtrl::InitLanguages(CString& rstrLangDir, bool bReInit)
{
	static BOOL _bInitialized = FALSE;
	if (_bInitialized && !bReInit)
		return;
	_bInitialized = TRUE;

	CFileFind ff;
	bool bEnd = !ff.FindFile(rstrLangDir + _T("*.dll"), 0);
	while (!bEnd)
	{
		bEnd = !ff.FindNextFile();
		if (ff.IsDirectory())
			continue;
		TCHAR szLandDLLFileName[MAX_PATH];
		_tsplitpath(ff.GetFileName(), NULL, NULL, szLandDLLFileName, NULL);

		SLanguage* pLangs = _aLanguages;
		if (pLangs){
			while (pLangs->lid){
				if (_tcsicmp(pLangs->pszISOLocale, szLandDLLFileName) == 0){
					pLangs->bSupported = TRUE;
					break;
				}
				pLangs++;
			}
		}
	}
	ff.Close();
}

void CLanguageManagerCtrl::InitLanguageDir()
{
	char buffer[490];
	::GetModuleFileName(0, buffer, 490);
	LPTSTR pszFileName = _tcsrchr(buffer, '\\') + 1;
	*pszFileName = '\0';

//	appdir = buffer;
//	configdir = appdir + CONFIGFOLDER;
//	m_strWebServerDir = appdir + _T("webserver\\");
//	m_strLangDir = appdir + _T("lang\\");
	m_strLangDir = buffer;
	m_strLangDir += _T("Language\\");
}

CString& CLanguageManagerCtrl::GetLangDir()
{
	return m_strLangDir;
}

WORD CLanguageManagerCtrl::GetLanguageID()
{
	return m_wLanguageID;
}

void CLanguageManagerCtrl::SetLanguageID(WORD lid)
{
	if( m_wLanguageID == lid )
		return;

	m_wLanguageID = lid;
	LoadLangLib( GetLangDir(), lid );   
}
