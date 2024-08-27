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
/// ��������
/// </summary>
private string[] array =
new string[] { "k", "y", "s", "e", "m", "d", "j", "b", "r", "c" };
/// <summary>
/// ����ʱ���ʽ
/// </summary>
private string dateFormat = "yyyy-MM-dd";
/// <summary>
/// ����
/// </summary>
/// <param name="dt">����ǰʱ��</param>
/// <returns>���ܺ�ʱ��<</returns>
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
/// ����
/// </summary>
/// <param name="str">���ܺ�ʱ��</param>
/// <returns>����ǰʱ��</returns>
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
	// ����
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
	// ����
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
	// У��һ�£�����з���Կ�ַ�������Ϊ�����ڽ���
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
	// 1. �ļ�������(A) ע����������(B)
	//		A=0 && B=0	������Ϣ
	//		A=1 && B=0	��Ӳ���ļ���Ϣ�洢��ע���
	//		A=0 && B=1	��ע�����Ϣ�洢��Ӳ���ļ�
	//		A=1 && B=1	����Ƿ�һ��
	//					һ�£���д������Ϣ
	//					��һ�£��Ƚ���ʼ���ںͰ汾�ţ��Ծɵĸ����µ�
	//							�Ƚ�ʣ��ʱ�������µĸ��¾ɵ�
	/////////////////////////////////////////////////////////////////
	// ����ļ���ע������Ƿ����
	// Ӳ���ļ�
	BOOL bExist_File = TRUE;
	BOOL bExist_Reg = TRUE;
	char path[256] = "E:\\KYKYtime.dat";
	FILE *stream;
	stream = fopen( path, "r" );
	if( stream == NULL )
		bExist_File = FALSE;
	// ע�����
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
		// A=0 && B=0	������Ϣ
		// �������ڼ�¼�����洢�汾����
		sprintf( cTrial, "%s4137", strTime );	// ���ð����4137
		Trial_Encryption( cTrial, cTrialCvt );
		// ����ʣ��ʱ����ʼֵ
		m_nTrialTime = 30*24*60*60;	// 30*24*60*60;
		// Ҫȡ��[a,b]�����������ʹ��(rand() % (b-a+1))+ a;
		// ����1-999�������
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
			// ������������
			CFile file;
			CFileStatus rStatus;
			file.GetStatus( path, rStatus );
			rStatus.m_attribute = 0x02;	// hidden;
			file.SetStatus( path, rStatus );
		}

		// ����ע�������洢�汾��ʱ����Ϣ
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
		// A=1 && B=0	��Ӳ���ļ���Ϣ�洢��ע���
		// Set pointer to beginning of file:
		fseek( stream, 0L, SEEK_SET );
		// Read data back from file:
		fscanf( stream, "%s", cTrialCvt );			// �õ����ںͰ汾��Ϣ
		fscanf( stream, "%s", cTimeCvt );			// �õ�ʹ��ʱ����Ϣ
		fclose( stream );
		nRet = Trial_CheckValid( cTrialCvt, cTimeCvt );
		if( nRet > -1 )
		{
			m_bTrial_NormalUse = TRUE;
			strcat( cTrialCvt, cTimeCvt );
			// ����ע�������洢�汾��ʱ����Ϣ
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
		// A=0 && B=1	��ע�����Ϣ�洢��Ӳ���ļ�
		if( RegQueryValueEx( hkKey, cSubKey, NULL, NULL, (LPBYTE)cBuf, &dwSize ) == ERROR_SUCCESS )
		{
			// ������ע����л�õİ汾��Ϣ
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
					// ������������
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
		// A=1 && B=1	����Ƿ�һ��
		//				һ�£���д������Ϣ
		//				��һ�£��Ƚ���ʼ���ںͰ汾�ţ��Ծɵĸ����µ�
		//						�Ƚ�ʣ��ʱ�����Զ̵ĸ��³���
		// 1. �ȶ�Ӳ���ļ�
		// Set pointer to beginning of file:
		fseek( stream, 0L, SEEK_SET );
		// Read data back from file:
		fscanf( stream, "%s", cTrialCvt );			// �õ����ںͰ汾��Ϣ
		fscanf( stream, "%s", cTimeCvt );			// �õ�ʹ��ʱ����Ϣ
		fclose( stream );
		int nRet1 = Trial_CheckValid( cTrialCvt, cTimeCvt );
		int nTrialTimeFile = m_nTrialTime;
		// 2. ��ע���
		int nTrialTimeReg = 0;
		int nRet2 = 0;
		char cTrialCvt2[20] = "xxxxxxxx";
		char cTimeCvt2[20] = "xxxxxxxx";
		if( RegQueryValueEx( hkKey, cSubKey, NULL, NULL, (LPBYTE)cBuf, &dwSize ) == ERROR_SUCCESS )
		{
			// ������ע����л�õİ汾��Ϣ
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
		else if( (nRet1 != nRet2 && nRet1 > -1 && nRet2 > -1 && nRet1 > nRet2 )		// �汾��Ϣ��һ���Ҷ���Ч��ע����еĸ��ͼ�
			|| (nRet1 < 0 && nRet2 > -1)											// �汾��Ϣ��һ����ע����е���Ч
			|| (nRet1 == nRet2 && nRet1 > -1 && nTrialTimeFile > nTrialTimeReg) )	// �汾��Ϣһ����ע����е�ʱ����Ϣ����
		{
			// ע������ǵͼ�����Ϣ����ע���������Ч��Ϣ
			strcpy( cTrialCvt3, cTrialCvt2 );	// ��ע������汾��Ϣ
			strcpy( cTimeCvt3, cTimeCvt2 );		// ��ע�����ʱ����Ϣ
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
		else if( (nRet1 != nRet2 && nRet1 > -1 && nRet2 > -1 && nRet1 < nRet2)		// �汾��Ϣ��һ���Ҷ���Ч��Ӳ���еĸ��ͼ�
			|| (nRet1 > -1 && nRet2 < 0)											// �汾��Ϣ��һ����Ӳ���е���Ч
			|| (nRet1 == nRet2 && nRet1 > -1 && nTrialTimeFile < nTrialTimeReg) )	// �汾��Ϣһ����Ӳ���е�ʱ����Ϣ����
		{
			// Ӳ���ļ��ǵͼ�����Ϣ����Ӳ���ļ�������Ч��Ϣ
			strcpy( cTrialCvt3, cTrialCvt );	// ��Ӳ���ļ������汾��Ϣ
			strcpy( cTimeCvt3, cTimeCvt );		// ��Ӳ���ļ�����ʱ����Ϣ
			// �޸�ע�����
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
			// �����ǺϷ��İ汾���򶼱�Ϊ��Ч��Ϊ0�����ð�
			CString strPrompt = GetResStringProductVersion( IDS_SD_PROMPT_Trial_Prompt3 );
			MessageBox( NULL, strPrompt, "Warning", MB_OK|MB_ICONWARNING );
			m_nTrialTime = 0;
			char cTrial3[20];
			sprintf( cTrial3, "%s4137", strTime );	// ���ð����4137
			Trial_Encryption( cTrial3, cTrialCvt3 );
			Trial_Encryption( "00000000", cTimeCvt3 );
			// �޸�ע�����
			if( RegOpenKeyEx( HKEY_CLASSES_ROOT, cClsID, 0, KEY_ALL_ACCESS, &hkKey) == ERROR_SUCCESS )
			{
				strcpy( cBuf, cTrialCvt3 );
				strcat( cBuf, cTimeCvt3 );
				RegSetValueEx( hkKey, cSubKey, 0, REG_SZ, (LPBYTE) cBuf, strlen(cBuf) + 1 );
				RegCloseKey( hkKey );
			}
			// �޸�Ӳ���ļ�
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
				sprintf( cTrial, "%s5268", strTime );	// ��ʽ�����5268
				Trial_Encryption( cTrial, cTrialCvt );
				fwrite( cTrialCvt, sizeof( char ), strlen(cTrialCvt), stream );
				fseek( stream, 0L, SEEK_SET );
			}
			// Read data back from file:
			fscanf( stream, "%s", cTrial );	// �õ���һ����������
			fseek( stream, 8+1, SEEK_SET );	// �����ļ�д��λ��
			fwrite( cTimeCvt, sizeof( char ), strlen(cTimeCvt), stream );
			fclose( stream );
		}
	}

	// ע�����
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
				sprintf( cTrial, "%s5268", strTime );	// ��ʽ�����5268
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
	int nRet = -1;	// �汾��Ϣ��ʱ����Ϣ�ĳ��Ȳ�����Ҫ��
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
			// 4. �У���Ƚ��������Ƿ񳬳�
			char cVer[10] = "xxxx";
			strncpy( cVer, cTrial+4, 4 );		// �õ��汾��Ϣ
			int nCmp1 = strcmp( cVer, "4137" );
			int nCmp2 = strcmp( cVer, "5268" );
			m_nTrialTime = atoi( cTime );
			if( (nCmp1 != 0) && (nCmp2 != 0) )
				nRet = -4;		// �汾��Ϣ�Ȳ������ð�Ҳ������ʽ��
			else if( (nCmp1 == 0) && (m_nTrialTime > 30*24*60*60 +999) )
				nRet = -5;		// ���ð棬������ʱ������
			else if( (nCmp1 == 0) && (m_nTrialTime == 0) )
				nRet = 0;		// ��Ч�ڵ���
			else if( (nCmp2 == 0) || ( (nCmp1 == 0) && (m_nTrialTime > 0) ))
			{
				m_bTrial_NormalUse = TRUE;
				if( nCmp1 == 0 )
					nRet = 1;	// ��Ч���ڵ����ð�
				else
					nRet = 2;	// ��ʽ��
			}
		}
		else
		{
			if( !bTrial )
				nRet = -2;		// �汾��Ϣ���Ƿ��ַ�
			else if( !bTime )
				nRet = -3;		// ʱ����Ϣ���Ƿ��ַ�
		}
	}
	return nRet;
}


WORD CProductVersionManager::GetVersionInfo()
{
	WORD wRet = 0x0;
	// ��ȡExe�ļ��汾��Ϣ�ĺ�����ʹ��GetFileVersionInfo�õ�TFileVersionInfo�ṹ�壬����12�����ݣ�
/*	Type   TFileVersionInfo   =   Record   
		FixedInfo:TVSFixedFileInfo;		{�汾��Ϣ}   
		CompanyName:String;				{��˾����}   
		FileDescription:String;			{˵��}   
		FileVersion:String;				{�ļ��汾}   
		InternalName:String;			{�ڲ�����}   
		LegalCopyright:String;			{��Ȩ}   
		LegalTrademarks:String;			{�Ϸ��̱�}   
		OriginalFilename:String;		{Դ�ļ���}   
		ProductName:String;				{��Ʒ����}   
		ProductVersion:String;			{��Ʒ�汾}   
		Comments:String;				{��ע}   
		LocalPort:String;				{Local   UDP_Message   Port}   
	end;
*/
	// ��ȡ����·��
	char path[256];
	::GetModuleFileName(0, path, 255);
	// 1.  ǰ����������ʹ�ã�ΪVerQueryValue��׼��
	DWORD dwInfoSize, dwHandle;
//	dwInfoSize = ::GetFileVersionInfoSize(m_szAppFilePath, &dwHandle);
	dwInfoSize = ::GetFileVersionInfoSize(path, &dwHandle);
	if(dwInfoSize <= 0)
		ASSERT(FALSE);
	BYTE* pData = new BYTE[dwInfoSize];
	VERIFY(::GetFileVersionInfo(path, NULL,	dwInfoSize, (LPVOID)pData));

	// ��ѯʲô��Ҫ��VerQueryValue�ĵڶ������������ƣ�����ȡֵ���£�
	// "\" ��ȡ�ļ���VS_FIXEDFILEINFO�ṹ
	// "\VarFileInfo\Translation" ��ȡ�ļ��ķ����
	// "\StringFileInfo\languagecodepage\stringname" ��ȡ�ļ����ִ���Ϣ��languagecodepageָ�������Դ���ҳ��080404b0Ϊ���ģ�040904E4ΪӢ�ġ�

	// 2. ��ȡ���ΰ汾��
	LPVOID lpBuffer;
	UINT uLength;
	VERIFY(::VerQueryValue((LPVOID)pData, _T("\\"), &lpBuffer, &uLength));
	DWORD dwProductVersionMS = ((VS_FIXEDFILEINFO*)lpBuffer)->dwProductVersionMS;
	DWORD dwProductVersionLS = ((VS_FIXEDFILEINFO*)lpBuffer)->dwProductVersionLS;
// 	CString szVersion;
// 	szVersion.Format( "%x%x%x%x", HIWORD(dwProductVersionMS), LOWORD(dwProductVersionMS), HIWORD(dwProductVersionLS), LOWORD(dwProductVersionLS));
	wRet = HIWORD(dwProductVersionMS) *16*16*16 + LOWORD(dwProductVersionMS) *16*16 + HIWORD(dwProductVersionLS) *16 + LOWORD(dwProductVersionLS);

	// 3. ��ȡ��Ȩ��Ϣ
	VERIFY(::VerQueryValue((LPVOID)pData,
			_T("\\StringFileInfo\\080404b0\\LegalCopyright"),
			&lpBuffer, &uLength));
	CString szCopyright((LPCTSTR)lpBuffer, (int)uLength);

	// 4. ��ȡ��Ʒ����
	VERIFY(::VerQueryValue((LPVOID)pData,  
			_T("\\StringFileInfo\\080404b0\\ProductName"),
			&lpBuffer, &uLength));
	CString szProductName((LPCTSTR)lpBuffer, (int)uLength);

	// 5. ��ȡ��˾����
	VERIFY(::VerQueryValue((LPVOID)pData,
			_T("\\StringFileInfo\\080404b0\\CompanyName"),
			&lpBuffer, &uLength));
	CString szManufacture((LPCTSTR)lpBuffer, (int)uLength);

	delete [] pData;
	return wRet;
}