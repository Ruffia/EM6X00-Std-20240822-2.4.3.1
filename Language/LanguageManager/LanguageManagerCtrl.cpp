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
/*	{LANGID_BG_BG,	_T("hun"),			FALSE,	_T("bg_BG")},
	{LANGID_HU_HU,	_T("hungarian"),	FALSE,	_T("hu_HU")},
	{LANGID_CA_ES,	_T(""),				FALSE,	_T("ca_ES")},
*/	{LANGID_ZH_CN,	_T("chs"),			TRUE,	_T("zh_CN")},
/*	{LANGID_ZH_TW,	_T("cht"),			FALSE,	_T("zh_TW")},
	{LANGID_DA_DK,	_T("danish"),		FALSE,	_T("da_DK")},
	{LANGID_NL_NL,	_T("dutch"),		FALSE,	_T("nl_NL")},
*/	{LANGID_EN_US,	_T("english"),		TRUE,	_T("en_US")},
/*	{LANGID_ET_EE,	_T(""),				FALSE,	_T("et_EE")},
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
*/	{LANGID_RU_RU,	_T("russian"),		TRUE,	_T("ru_RU")},
/*	{LANGID_SL_SI,	_T(""),				FALSE,	_T("sl_SI")},
	{LANGID_ES_ES_T,_T("spanish"),		FALSE,	_T("es_ES_T")},
	{LANGID_SV_SE,	_T("swedish"),		FALSE,	_T("sv_SE")},
	{LANGID_TR_TR,	_T("turkish"),		FALSE,	_T("tr_TR")},
	{LANGID_HE_IL,	_T(""),				FALSE,	_T("he_IL")},
	{LANGID_JP_JP,	_T(""),				FALSE,	_T("jp_JP")},
	{LANG_CZECH,	_T(""),				FALSE,	_T("cz_CZ")},
*/	{0, NULL, 0}
};

/*
std::string  utf8ToGBK( const std::string &strUtf8 )
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, NULL, 0);
	wchar_t * wszGBK = new wchar_t[len +1];
	memset(wszGBK, 0, len +1);
	MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len +1];
	memset(szGBK, 0, len +1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);

	std::string strGBK(szGBK);

	delete [] szGBK;
	delete [] wszGBK;
	return strGBK;
}

std::string gbkToUtf8(const std::string &strGBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	wchar_t * wszUtf8 = new wchar_t[len];
	memset(wszUtf8, 0, len);
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, wszUtf8, len);

	len = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = new char[len +1];
	memset(szUtf8, 0, len +1);
	WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, szUtf8, len, NULL, NULL);

	std::string strUTF8(szUtf8);

	delete [] szUtf8;
	delete [] wszUtf8;
	return strUTF8;
}
*/
// 在windows下打开一个记事本，保存文件，下面有四种编码选择。
// ANSI，也就是多字节字符集，在VC中也就是CHAR(char)字符串。
// Unicode，就是UTF16，在VC中也就是WCHAR(wchar_t)字符串。
// Unicode big endian，就是UTF32，这种编码用的比较少。
// UTF8，网页上几乎都是用UTF8，UTF8用1-4个字节来编码所有的字符，英文只需要1个字节，中文需要3-4个字节。
// 比起UTF16来说，UTF8这样可以尽可能的节省网络带宽，因为在网络上传输的字符，大部分以英文为主。
// UTF16至少是2个字节，部分字符4个字节。

// 如果我们写一个VC程序，从获取HTML网页数据，这些数据的编码是UTF8的，
// 获取到我们VC程序中的CHAR字符数组中时就会发现，英文可以正常显示，中文全部乱码了。
// 因为我们的CHAR型字符串用的是ANSI编码。

// 要想把UTF8转换为ANSI，一般有两种方法。
// 一种是手工写代码实现，百度上搜索可以发现很多资料，透彻了解这些字符集编码后，可以手工来实现转换，网上也有很多别人写好的转换函数。
// 一种方法就是借助第三方函数库。
// 由于我们在windows平台下编写程序，我们可以使用API函数来转换MultiByteToWideChar和WideCharToMultiByte。
// 使用这个函数，我们得进行两次转换：
// 先用MultiByteToWideChar把UTF8编码的CHAR字符串转换成WCHAR字符串，第一个参数要注明我们要转换的代码页为CP_UTF8，即UTF8的意思。
// 然后用WideCharToMultiByte吧WCHAR字符串转换成CHAR字符串，第一个参数使用936，936代码页的意思是简体中文。
// 有关代码页的知识可以百度百科一下。

// 下面贴出我写的两个ANSI与UTF8互转的函数。参数为MFC中的CString字符串，如果要传入C样式的字符数组型字符串，只需稍加修改即可。
void UTF8toANSI( CString& strUtf8 )
{
	// 获取转换为多字节后需要的缓冲区大小，创建多字节缓冲区
	int len = MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, NULL, 0);
	WCHAR * wszBuffer = new WCHAR[len +1];
	len = MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, wszBuffer, len);
	wszBuffer[len] = 0;

	len = WideCharToMultiByte(936, 0, wszBuffer, -1, NULL, 0, NULL, NULL);
	CHAR *szBuffer = new CHAR[len +1];
	len = WideCharToMultiByte(936, 0, wszBuffer, -1, szBuffer, len, NULL, NULL);
	szBuffer[len] = 0;

	strUtf8 = szBuffer;

	delete [] szBuffer;
	delete [] wszBuffer;
}

void ANSItoUTF8( CString& strAnsi )
{
	// 获取转换为宽字节后需要的缓冲区大小，创建宽字节缓冲区，936为简体中文GB2312代码页
	int len = MultiByteToWideChar(936, 0, strAnsi, -1, NULL, 0);
	WCHAR * wszBuffer = new WCHAR[len +1];
	MultiByteToWideChar(936, 0, strAnsi, -1, wszBuffer, len);
	wszBuffer[len] = 0;

	// 获取转为UTF8多字节后需要的缓冲区大小，创建多字节缓冲区
	len = WideCharToMultiByte(CP_UTF8, 0, wszBuffer, -1, NULL, 0, NULL, NULL);
	CHAR *szBuffer = new CHAR[len +1];
	WideCharToMultiByte(CP_UTF8, 0, wszBuffer, -1, szBuffer, len, NULL, NULL);
	szBuffer[len] = 0;

	strAnsi = szBuffer;

	delete [] szBuffer;
	delete [] wszBuffer;
}

// 值得注意的是，UTF8编码的字符串一般要将其保存在CHAR(char)型数组里，而不保存在WCHAR(wchar_t)型数组里。
// 为什么呢？
// 因为UTF8编码的字符串每个字符占1-4个字节，有的字符只占1个字节，应该用CHAR型数组来保存。
// 而WCHAR的话，一个WCHAR就占两个字节，对于只需要一个字符的字符，就会出问题。

/////////////////////////////////////////////////////////////////////////////
// CLanguageManagerCtrl

CLanguageManagerCtrl::CLanguageManagerCtrl()
{
	m_hLangDLL		= NULL;
	m_wLanguageID	= 0;
	m_strLangDir	= _T("");
	m_strLangIDs	= _T("");
	m_strLangIni	= _T("");
}

CLanguageManagerCtrl::~CLanguageManagerCtrl()
{
}



CString CLanguageManagerCtrl::GetResString(UINT uStringID, WORD languageID)
{
	CString resString;
	char sz[256];
	if (m_hLangDLL)
	{
		LoadString(m_hLangDLL, uStringID, sz, sizeof(sz));
		resString.Format( "%s", sz );
		UTF8toANSI( resString );
	}
	if (resString.IsEmpty())
	{
		LoadString(GetModuleHandle(NULL), uStringID, sz, sizeof(sz));
		resString.Format( "%s", sz );
	}

	return resString;
}

CString CLanguageManagerCtrl::GetResString(UINT uStringID)
{
	CString resString = _T("");

	// IDS_CHECK_Vacuum_VACUUM_SWITCH
	char sz[255], szIDname[255];
	sprintf( sz, "%d", uStringID );
	if( GetPrivateProfileString("IDs", sz, "", szIDname, 255, m_strLangIDs) != 0 )
	{
		if( GetPrivateProfileString("Strings", szIDname, "", sz, 255, m_strLangIni) != 0 )
			resString.Format( "%s", sz );
	}

	return resString;
}

BOOL CLanguageManagerCtrl::LoadLangLib(const CString& rstrLangDir, LANGID lid)
{
	const SLanguage* pLangs = _aLanguages;
	if (pLangs)
	{
		while (pLangs->lid)
		{
			if (pLangs->bSupported && pLangs->lid == lid)
			{
				bool bLoadedLib = false;
				CString strLangDLL = rstrLangDir;
				strLangDLL += pLangs->pszISOLocale;
				m_strLangIni = strLangDLL + _T(".lang");
				bLoadedLib = true;
				if (bLoadedLib)
				{
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
	LoadLangLib( m_strLangDir, lidLocale );   
}

void CLanguageManagerCtrl::Close()
{
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
		if (pLangs)
		{
			while (pLangs->lid)
			{
				if (_tcsicmp(pLangs->pszISOLocale, szLandDLLFileName) == 0)
				{
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

	m_strLangDir = buffer;
	m_strLangDir += _T("Language\\");
	m_strLangIDs = m_strLangDir + _T("all.lang");
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
	LoadLangLib( m_strLangDir, lid );   
}
