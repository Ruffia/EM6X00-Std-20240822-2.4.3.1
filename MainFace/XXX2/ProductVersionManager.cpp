#include "stdafx.h"
#include "ProductVersionManager.h"
#include "Resource.h"

#define	GetResStringProductVersion(uStringID)		m_pLanguageManagerCtrl->GetResString(uStringID)

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {66E4C113-AB95-4138-8BB6-1E890A36E54F}
static const CLSID clsid =
{ 0x66e4c113, 0xab95, 0x4138, { 0x8b, 0xb6, 0x1e, 0x89, 0xa, 0x36, 0xe5, 0x4f } };


//char cTrial_Array[] = { 'k', 'y', 's', 'e', 'm', 'd', 'j', 'b', 'r', 'c' };
char cTrial_Array[] = "kysemdjbrc";	// "0123456789"

CProductVersionManager& CProductVersionManager::Instance()
{
	static CProductVersionManager Inst;
	return Inst;
}

CProductVersionManager::CProductVersionManager()
{
	// 17.08
	m_nTrialTime				= 2592000;
	m_bTrial_NormalUse			= FALSE;
	m_pLanguageManagerCtrl = NULL;
}

void CProductVersionManager::Initialize(CLanguageManagerCtrl& LanguageCtrl)
{
	m_pLanguageManagerCtrl = &LanguageCtrl;
}


/*
/// <summary>
/// 加密数组
/// </summary>
private string[] array =
new string[] { "k", "y", "s", "e", "m", "d", "j", "b", "r", "c" };
/// <summary>
/// 加密时间格式
/// </summary>
private string dateFormat = "yyyy-MM-dd";
/// <summary>
/// 加密
/// </summary>
/// <param name="dt">加密前时间</param>
/// <returns>加密后时间<</returns>
public string JiaMi(DateTime dt)
{
	string dtStr = dt.ToString(dateFormat);
	dtStr = dtStr.Replace("-", "");
	char[] charArray = dtStr.ToCharArray();
	dtStr = string.Empty;
	for (int i = 0; i < charArray.Length; i++)
	{
		dtStr += array[Convert.ToInt32(charArray[i].ToString())];
	}
	dtStr = StringConvert(dtStr, 3, 5);
	dtStr = StringConvert(dtStr, 2, 7);
	return dtStr;
}
/// <summary>
/// 解密
/// </summary>
/// <param name="str">加密后时间</param>
/// <returns>加密前时间</returns>
public DateTime JieMi(string str)
{
	DateTime dt = DateTime.Now;
	char[] charArray = str.ToCharArray();
	string dtStr = string.Empty;
	for (int j = 0; j < charArray.Length; j++)
	{
		for (int i = 0; i < array.Length; i++)
		{
			if (array[i] == charArray[j].ToString())
			{
				dtStr += i;
				break;
			}
		}
	}
	dtStr = StringConvert(dtStr, 2, 7);
	dtStr = StringConvert(dtStr, 3, 5);
	dtStr = dtStr.Insert(4, "-");
	dtStr = dtStr.Insert(7, "-");
	dt = DateTime.Parse(dtStr);
	return dt;
}encryption and decryption
*/


void CProductVersionManager::Trial_Encryption( char* cTimeSrc, char* cTimeCvt )
{
	char ch;
	// 加密
	int nLength = strlen( cTimeSrc );
	for( int i=0; i<nLength; i++ )
	{
		ch = cTimeSrc[i];
		cTimeCvt[i] = cTrial_Array[atoi(&ch)];
	}
	// swap 2,5
	ch = cTimeCvt[1];
	cTimeCvt[1] = cTimeCvt[4];
	cTimeCvt[4] = ch;
	if( nLength > 6 )
	{
		// swap 3,7
		ch = cTimeCvt[2];
		cTimeCvt[2] = cTimeCvt[6];
		cTimeCvt[6] = ch;
	}
}


BOOL CProductVersionManager::Trial_Decryption( char* cTimeCvt, char* cTimeSrc )
{
	char ch;
	// 解密
	int nLength = strlen( cTimeCvt );
	int nLengthA = strlen( cTrial_Array );
	for( int i=0; i<nLength; i++ )
	{
		ch = cTimeCvt[i];
		for( int j=0; j<nLengthA; j++ )
		{
			if( ch == cTrial_Array[j] )
			{
				cTimeSrc[i] = '0' +j;
				break;
			}
		}
	}
	// swap 2,5
	if( nLength > 4 )
	{
		ch = cTimeSrc[1];
		cTimeSrc[1] = cTimeSrc[4];
		cTimeSrc[4] = ch;
	}
	if( nLength > 6 )
	{
		// swap 3,7
		ch = cTimeSrc[2];
		cTimeSrc[2] = cTimeSrc[6];
		cTimeSrc[6] = ch;
	}

	BOOL bLegal= TRUE;
	// 校验一下，如果有非密钥字符，则认为试用期结束
	nLength = strlen( cTimeSrc );
	for(int i=0; i<nLength; i++ )
	{
		if( cTimeSrc[i] < 0x30 || cTimeSrc[i] > 0x39 )
		{
			bLegal = FALSE;
			break;
		}
	}
	return bLegal;
}


BOOL CProductVersionManager::Trial_GetFirstTime()
{
	CString strTime;
	CTime	timeCurrent = CTime::GetCurrentTime();
	strTime = timeCurrent.Format(_T("%m%d"));
	char cTrial[20] = {0}, cTrialCvt[20] = "xxxxxxxx";
	char cTime[20] = {0}, cTimeCvt[20] = "xxxxxxxx";

	/////////////////////////////////////////////////////////////////
	// 1. 文件存在吗？(A) 注册表项存在吗？(B)
	//		A=0 && B=0	建立信息
	//		A=1 && B=0	将硬盘文件信息存储到注册表
	//		A=0 && B=1	将注册表信息存储到硬盘文件
	//		A=1 && B=1	检查是否一致
	//					一致：则都写入新信息
	//					不一致：比较起始日期和版本号，以旧的更新新的
	//							比较剩余时长，以新的更新旧的
	/////////////////////////////////////////////////////////////////
	// 检查文件和注册表项是否存在
	// 硬盘文件
	BOOL bExist_File = TRUE;
	BOOL bExist_Reg = TRUE;
	char path[256] = "E:\\KYKYtime.dat";
	FILE *stream;
	stream = fopen( path, "r" );
	if( stream == NULL )
		bExist_File = FALSE;
	// 注册表项
	// HKEY_CLASSES_ROOT\Wow6432Node\CLSID\{66e4c113-ab95-4138-8bb6-1e890a36e54f}\VerInfo
	CString strClsID;
	strClsID.Format("{%x-%x-%x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
		clsid.Data1, clsid.Data2, clsid.Data3,
		clsid.Data4[0], clsid.Data4[1], clsid.Data4[2], clsid.Data4[3],
		clsid.Data4[4], clsid.Data4[5], clsid.Data4[6], clsid.Data4[7] );
	char cClsID[255], cSubKey[20];
	sprintf( cClsID, "CLSID\\%s", strClsID );
	sprintf( cSubKey, "%s", "skkbkcyy" );	// 20170901 skybkcky
	char cBuf[255];
	DWORD dwSize = 255;
	HKEY hkKey;
	if( RegOpenKeyEx( HKEY_CLASSES_ROOT, cClsID, 0, KEY_ALL_ACCESS, &hkKey) != ERROR_SUCCESS )
		bExist_Reg = FALSE;
	int nRet = -1;

	if( !bExist_File && !bExist_Reg )
	{
		// A=0 && B=0	建立信息
		// 建立日期记录，并存储版本代号
		sprintf( cTrial, "%s4137", strTime );	// 试用版代号4137
		Trial_Encryption( cTrial, cTrialCvt );
		// 设置剩余时长初始值
		m_nTrialTime = 30*24*60*60;	// 30*24*60*60;
		// 要取得[a,b]的随机整数，使用(rand() % (b-a+1))+ a;
		// 产生1-999的随机数
		m_nTrialTime = m_nTrialTime +(rand() % (999 -1 +1)) +1;
		sprintf( cTime, "%08d", m_nTrialTime );
		Trial_Encryption( cTime, cTimeCvt );
		strcpy( cBuf, cTrialCvt );
		strcat( cBuf, cTimeCvt );

		stream = fopen( path, "wb" );
		if( stream != NULL )
		{
			strcat( cTrialCvt, "\n" );
			fwrite( cTrialCvt, sizeof( char ), strlen(cTrialCvt), stream );
			fwrite( cTimeCvt, sizeof( char ), strlen(cTimeCvt), stream );
			fclose( stream );
			// 设置隐藏属性
			CFile file;
			CFileStatus rStatus;
			file.GetStatus( path, rStatus );
			rStatus.m_attribute = 0x02;	// hidden;
			file.SetStatus( path, rStatus );
		}

		// 创建注册表项，并存储版本和时长信息
		DWORD dwDisposition;
		if( RegCreateKeyEx( HKEY_CLASSES_ROOT, cClsID, NULL, cClsID,
			REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, 
			&hkKey, &dwDisposition ) == ERROR_SUCCESS )
		{
			RegSetValueEx( hkKey, cSubKey, 0, REG_SZ, (LPBYTE) cBuf, strlen(cBuf) + 1 );
			RegCloseKey( hkKey );
		}
		m_bTrial_NormalUse = TRUE;
	}
	else if( bExist_File && !bExist_Reg )
	{
		// A=1 && B=0	将硬盘文件信息存储到注册表
		// Set pointer to beginning of file:
		fseek( stream, 0L, SEEK_SET );
		// Read data back from file:
		fscanf( stream, "%s", cTrialCvt );			// 得到日期和版本信息
		fscanf( stream, "%s", cTimeCvt );			// 得到使用时长信息
		fclose( stream );
		nRet = Trial_CheckValid( cTrialCvt, cTimeCvt );
		if( nRet > -1 )
		{
			m_bTrial_NormalUse = TRUE;
			strcat( cTrialCvt, cTimeCvt );
			// 创建注册表项，并存储版本和时长信息
			DWORD dwDisposition;
			if( RegCreateKeyEx( HKEY_CLASSES_ROOT, cClsID, NULL, cClsID,
				REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, 
				&hkKey, &dwDisposition ) == ERROR_SUCCESS )
			{
				RegSetValueEx( hkKey, cSubKey, 0, REG_SZ, (LPBYTE) cTrialCvt, strlen(cTrialCvt) + 1 );
				RegCloseKey( hkKey );
			}
		}
		else
		{
			m_nTrialTime = 0;
			m_bTrial_NormalUse = FALSE;
		}
	}
	else if( !bExist_File && bExist_Reg )
	{
		// A=0 && B=1	将注册表信息存储到硬盘文件
		if( RegQueryValueEx( hkKey, cSubKey, NULL, NULL, (LPBYTE)cBuf, &dwSize ) == ERROR_SUCCESS )
		{
			// 分析从注册表中获得的版本信息
			strncpy( cTrialCvt, cBuf, 8 );
			strncpy( cTimeCvt, cBuf+8, 8 );
			nRet = Trial_CheckValid( cTrialCvt, cTimeCvt );
			if( nRet > -1 )
			{
				m_bTrial_NormalUse = TRUE;
				stream = fopen( path, "wb" );
				if( stream != NULL )
				{
					strcat( cTrialCvt, "\n" );
					fwrite( cTrialCvt, sizeof( char ), strlen(cTrialCvt), stream );
					fwrite( cTimeCvt, sizeof( char ), strlen(cTimeCvt), stream );
					fclose( stream );
					// 设置隐藏属性
					CFile file;
					CFileStatus rStatus;
					file.GetStatus( path, rStatus );
					rStatus.m_attribute = 0x02;	// hidden;
					file.SetStatus( path, rStatus );
				}
			}
		}
		else
		{
			m_nTrialTime = 0;
			m_bTrial_NormalUse = FALSE;
		}
		RegCloseKey( hkKey );
	}
	else
	{
		// A=1 && B=1	检查是否一致
		//				一致：则都写入新信息
		//				不一致：比较起始日期和版本号，以旧的更新新的
		//						比较剩余时长，以短的更新长的
		// 1. 先读硬盘文件
		// Set pointer to beginning of file:
		fseek( stream, 0L, SEEK_SET );
		// Read data back from file:
		fscanf( stream, "%s", cTrialCvt );			// 得到日期和版本信息
		fscanf( stream, "%s", cTimeCvt );			// 得到使用时长信息
		fclose( stream );
		int nRet1 = Trial_CheckValid( cTrialCvt, cTimeCvt );
		int nTrialTimeFile = m_nTrialTime;
		// 2. 读注册表
		int nTrialTimeReg = 0;
		int nRet2 = 0;
		char cTrialCvt2[20] = "xxxxxxxx";
		char cTimeCvt2[20] = "xxxxxxxx";
		if( RegQueryValueEx( hkKey, cSubKey, NULL, NULL, (LPBYTE)cBuf, &dwSize ) == ERROR_SUCCESS )
		{
			// 分析从注册表中获得的版本信息
			strncpy( cTrialCvt2, cBuf, 8 );
			strncpy( cTimeCvt2, cBuf+8, 8 );
			nRet2 = Trial_CheckValid( cTrialCvt2, cTimeCvt2 );
			nTrialTimeReg = m_nTrialTime;
		}
		RegCloseKey( hkKey );

		char cTrialCvt3[20] = "xxxxxxxx";
		char cTimeCvt3[20] = "xxxxxxxx";
		if( nTrialTimeFile < 0 )
			nTrialTimeFile = 0;
		if( nTrialTimeReg < 0 )
			nTrialTimeReg = 0;

		if( nRet1 == nRet2 && nRet1 > -1 && nTrialTimeFile == nTrialTimeReg )
		{
			if( nRet1 > 0 )
			{
				m_nTrialTime = nTrialTimeFile;
				m_bTrial_NormalUse = TRUE;
				return TRUE;
			}
		}
		else if( (nRet1 != nRet2 && nRet1 > -1 && nRet2 > -1 && nRet1 > nRet2 )		// 版本信息不一致且都有效且注册表中的更低级
			|| (nRet1 < 0 && nRet2 > -1)											// 版本信息不一致且注册表中的有效
			|| (nRet1 == nRet2 && nRet1 > -1 && nTrialTimeFile > nTrialTimeReg) )	// 版本信息一致且注册表中的时长信息更短
		{
			// 注册表中是低级别信息，或注册表中是有效信息
			strcpy( cTrialCvt3, cTrialCvt2 );	// 从注册表拷贝版本信息
			strcpy( cTimeCvt3, cTimeCvt2 );		// 从注册表拷贝时长信息
			stream = fopen( path, "rb+" );
			if( stream != NULL )
			{
				strcat( cTrialCvt3, "\n" );
				fwrite( cTrialCvt3, sizeof( char ), strlen(cTrialCvt3), stream );
				fwrite( cTimeCvt3, sizeof( char ), strlen(cTimeCvt3), stream );
				fclose( stream );
			}
			m_nTrialTime = nTrialTimeReg;
		}
		else if( (nRet1 != nRet2 && nRet1 > -1 && nRet2 > -1 && nRet1 < nRet2)		// 版本信息不一致且都有效且硬盘中的更低级
			|| (nRet1 > -1 && nRet2 < 0)											// 版本信息不一致且硬盘中的有效
			|| (nRet1 == nRet2 && nRet1 > -1 && nTrialTimeFile < nTrialTimeReg) )	// 版本信息一致且硬盘中的时长信息更短
		{
			// 硬盘文件是低级别信息，或硬盘文件中是有效信息
			strcpy( cTrialCvt3, cTrialCvt );	// 从硬盘文件拷贝版本信息
			strcpy( cTimeCvt3, cTimeCvt );		// 从硬盘文件拷贝时长信息
			// 修改注册表项
			if( RegOpenKeyEx( HKEY_CLASSES_ROOT, cClsID, 0, KEY_ALL_ACCESS, &hkKey) == ERROR_SUCCESS )
			{
				strcat( cTrialCvt3, cTimeCvt3 );
				RegSetValueEx( hkKey, cSubKey, 0, REG_SZ, (LPBYTE) cTrialCvt3, strlen(cTrialCvt3) + 1 );
				RegCloseKey( hkKey );
			}
			m_nTrialTime = nTrialTimeFile;
		}
		else
		{
			// 都不是合法的版本，则都变为有效期为0的试用版
			CString strPrompt = GetResStringProductVersion( IDS_SD_PROMPT_Trial_Prompt3 );
			MessageBox( NULL, strPrompt, "Warning", MB_OK|MB_ICONWARNING );
			m_nTrialTime = 0;
			char cTrial3[20];
			sprintf( cTrial3, "%s4137", strTime );	// 试用版代号4137
			Trial_Encryption( cTrial3, cTrialCvt3 );
			Trial_Encryption( "00000000", cTimeCvt3 );
			// 修改注册表项
			if( RegOpenKeyEx( HKEY_CLASSES_ROOT, cClsID, 0, KEY_ALL_ACCESS, &hkKey) == ERROR_SUCCESS )
			{
				strcpy( cBuf, cTrialCvt3 );
				strcat( cBuf, cTimeCvt3 );
				RegSetValueEx( hkKey, cSubKey, 0, REG_SZ, (LPBYTE) cBuf, strlen(cBuf) + 1 );
				RegCloseKey( hkKey );
			}
			// 修改硬盘文件
			stream = fopen( path, "rb+" );
			if( stream != NULL )
			{
				strcat( cTrialCvt3, "\n" );
				fwrite( cTrialCvt3, sizeof( char ), strlen(cTrialCvt3), stream );
				fwrite( cTimeCvt3, sizeof( char ), strlen(cTimeCvt3), stream );
				fclose( stream );
			}
		}
		if( m_nTrialTime > 0 )
			m_bTrial_NormalUse = TRUE;
		else
		{
			m_bTrial_NormalUse = FALSE;
			CString strPrompt = GetResStringProductVersion( IDS_SD_PROMPT_Trial_Prompt4 );
			MessageBox( NULL, strPrompt, "Warning", MB_OK|MB_ICONWARNING );
		}
	}
	return TRUE;
}


void CProductVersionManager::Trial_SaveTime( BOOL bRegister )
{
	CString strTime;
	CTime	timeCurrent = CTime::GetCurrentTime();
	strTime = timeCurrent.Format("%m%d");
	char cTrial[20], cTrialCvt[20] = "08314137";
	char cTime[20], cTimeCvt[20] = "432000";

	if( bRegister )
		m_nTrialTime = 0;
	else
	{
		m_nTrialTime -= 5*60;
		if( m_nTrialTime < 0 )
			m_nTrialTime = 0;
	}
	sprintf( cTime, "%08d", m_nTrialTime );
	Trial_Encryption( cTime, cTimeCvt );

	char path[256] = "E:\\KYKYtime.dat";
	FILE *stream;
	stream = fopen( path, "r" );
	if( stream != NULL )
	{
		fclose( stream );
		stream = fopen( path, "rb+" );
		if( stream != NULL )
		{
			// Set pointer to beginning of file:
			fseek( stream, 0L, SEEK_SET );
			if( bRegister )
			{
				sprintf( cTrial, "%s5268", strTime );	// 正式版代号5268
				Trial_Encryption( cTrial, cTrialCvt );
				fwrite( cTrialCvt, sizeof( char ), strlen(cTrialCvt), stream );
				fseek( stream, 0L, SEEK_SET );
			}
			// Read data back from file:
			fscanf( stream, "%s", cTrial );	// 得到第一次运行日期
			fseek( stream, 8+1, SEEK_SET );	// 调整文件写入位置
			fwrite( cTimeCvt, sizeof( char ), strlen(cTimeCvt), stream );
			fclose( stream );
		}
	}

	// 注册表项
	// HKEY_CLASSES_ROOT\Wow6432Node\CLSID\{66e4c113-ab95-4138-8bb6-1e890a36e54f}\VerInfo
	CString strClsID;
	strClsID.Format("{%x-%x-%x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
		clsid.Data1, clsid.Data2, clsid.Data3,
		clsid.Data4[0], clsid.Data4[1], clsid.Data4[2], clsid.Data4[3],
		clsid.Data4[4], clsid.Data4[5], clsid.Data4[6], clsid.Data4[7] );
	char cClsID[255], cSubKey[20];
	sprintf( cClsID, "CLSID\\{%x-%x-%x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
		clsid.Data1, clsid.Data2, clsid.Data3,
		clsid.Data4[0], clsid.Data4[1], clsid.Data4[2], clsid.Data4[3],
		clsid.Data4[4], clsid.Data4[5], clsid.Data4[6], clsid.Data4[7] );
	sprintf( cSubKey, "%s", "skkbkcyy" );	// 20170901 skybkcky
	char cBuf[255];
	DWORD dwSize = 255;
	HKEY hkKey;
	if( RegOpenKeyEx( HKEY_CLASSES_ROOT, cClsID, 0, KEY_ALL_ACCESS, &hkKey) == ERROR_SUCCESS )
	{
		if( RegQueryValueEx( hkKey, cSubKey, NULL, NULL, (LPBYTE)cBuf, &dwSize ) == ERROR_SUCCESS )
		{
			if( bRegister )
			{
				sprintf( cTrial, "%s5268", strTime );	// 正式版代号5268
				Trial_Encryption( cTrial, cTrialCvt );
				strncpy( cBuf, cTrialCvt, 8 );
			}
			strncpy( cBuf+8, cTimeCvt, 8 );
			RegSetValueEx( hkKey, cSubKey, 0, REG_SZ, (LPBYTE) cBuf, strlen(cBuf) + 1 );
			RegCloseKey( hkKey );
		}
	}
}


int CProductVersionManager::Trial_CheckValid( char* cTrialCvt, char* cTimeCvt )
{
	int nRet = -1;	// 版本信息或时长信息的长度不符合要求
	int nLengTrial = strlen( cTrialCvt );
	int nLengTime = strlen( cTimeCvt );
	if( nLengTrial == 8 && nLengTime == 8 )
	{
		char cTime[20]	= "xxxxxxxx";
		char cTrial[20]	= "xxxxxxxx";
		BOOL bTrial = Trial_Decryption( cTrialCvt, cTrial );
		BOOL bTime = Trial_Decryption( cTimeCvt, cTime );
		if( bTrial && bTime )
		{
			// 4. 有，则比较试用期是否超出
			char cVer[10] = "xxxx";
			strncpy( cVer, cTrial+4, 4 );		// 得到版本信息
			int nCmp1 = strcmp( cVer, "4137" );
			int nCmp2 = strcmp( cVer, "5268" );
			m_nTrialTime = atoi( cTime );
			if( (nCmp1 != 0) && (nCmp2 != 0) )
				nRet = -4;		// 版本信息既不是试用版也不是正式版
			else if( (nCmp1 == 0) && (m_nTrialTime > 30*24*60*60 +999) )
				nRet = -5;		// 试用版，但试用时长超出
			else if( (nCmp1 == 0) && (m_nTrialTime == 0) )
				nRet = 0;		// 有效期到期
			else if( (nCmp2 == 0) || ( (nCmp1 == 0) && (m_nTrialTime > 0) ))
			{
				m_bTrial_NormalUse = TRUE;
				if( nCmp1 == 0 )
					nRet = 1;	// 有效期内的试用版
				else
					nRet = 2;	// 正式版
			}
		}
		else
		{
			if( !bTrial )
				nRet = -2;		// 版本信息含非法字符
			else if( !bTime )
				nRet = -3;		// 时长信息含非法字符
		}
	}
	return nRet;
}


WORD CProductVersionManager::GetVersionInfo()
{
	WORD wRet = 0x0;
	// 获取Exe文件版本信息的函数（使用GetFileVersionInfo得到TFileVersionInfo结构体，包含12项内容）
/*	Type   TFileVersionInfo   =   Record   
		FixedInfo:TVSFixedFileInfo;		{版本信息}   
		CompanyName:String;				{公司名称}   
		FileDescription:String;			{说明}   
		FileVersion:String;				{文件版本}   
		InternalName:String;			{内部名称}   
		LegalCopyright:String;			{版权}   
		LegalTrademarks:String;			{合法商标}   
		OriginalFilename:String;		{源文件名}   
		ProductName:String;				{产品名称}   
		ProductVersion:String;			{产品版本}   
		Comments:String;				{备注}   
		LocalPort:String;				{Local   UDP_Message   Port}   
	end;
*/
	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	// 1.  前两个函数的使用，为VerQueryValue做准备
	DWORD dwInfoSize, dwHandle;
//	dwInfoSize = ::GetFileVersionInfoSize(m_szAppFilePath, &dwHandle);
	dwInfoSize = ::GetFileVersionInfoSize(path, &dwHandle);
	if(dwInfoSize <= 0)
		ASSERT(FALSE);
	BYTE* pData = new BYTE[dwInfoSize];
	VERIFY(::GetFileVersionInfo(path, NULL,	dwInfoSize, (LPVOID)pData));

	// 查询什么主要有VerQueryValue的第二个参数来控制，它的取值如下：
	// "\" 获取文件的VS_FIXEDFILEINFO结构
	// "\VarFileInfo\Translation" 获取文件的翻译表
	// "\StringFileInfo\languagecodepage\stringname" 获取文件的字串信息，languagecodepage指的是语言代码页【080404b0为中文，040904E4为英文】

	// 2. 获取主次版本号
	LPVOID lpBuffer;
	UINT uLength;
	VERIFY(::VerQueryValue((LPVOID)pData, _T("\\"), &lpBuffer, &uLength));
	DWORD dwProductVersionMS = ((VS_FIXEDFILEINFO*)lpBuffer)->dwProductVersionMS;
	DWORD dwProductVersionLS = ((VS_FIXEDFILEINFO*)lpBuffer)->dwProductVersionLS;
// 	CString szVersion;
// 	szVersion.Format( "%x%x%x%x", HIWORD(dwProductVersionMS), LOWORD(dwProductVersionMS), HIWORD(dwProductVersionLS), LOWORD(dwProductVersionLS));
	wRet = HIWORD(dwProductVersionMS) *16*16*16 + LOWORD(dwProductVersionMS) *16*16 + HIWORD(dwProductVersionLS) *16 + LOWORD(dwProductVersionLS);

	// 3. 获取版权信息
	VERIFY(::VerQueryValue((LPVOID)pData,
			_T("\\StringFileInfo\\080404b0\\LegalCopyright"),
			&lpBuffer, &uLength));
	CString szCopyright((LPCTSTR)lpBuffer, (int)uLength);

	// 4. 获取产品名称
	VERIFY(::VerQueryValue((LPVOID)pData,  
			_T("\\StringFileInfo\\080404b0\\ProductName"),
			&lpBuffer, &uLength));
	CString szProductName((LPCTSTR)lpBuffer, (int)uLength);

	// 5. 获取公司名称
	VERIFY(::VerQueryValue((LPVOID)pData,
			_T("\\StringFileInfo\\080404b0\\CompanyName"),
			&lpBuffer, &uLength));
	CString szManufacture((LPCTSTR)lpBuffer, (int)uLength);

	delete [] pData;
	return wRet;
}