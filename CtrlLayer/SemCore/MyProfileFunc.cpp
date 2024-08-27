#include "StdAfx.h"
#include  <math.h>

#define	MYBUFSIZE	255
//四舍五入函数
int RoundFunc( double dNum )
{
	int nMin = (int)dNum;
	if( dNum < 0 )
		return ( (fabs(dNum-nMin) < 0.5) ? nMin : (nMin - 1) );
	else
		return ( (dNum-nMin < 0.5) ? nMin : (nMin + 1) );
}

//从注册表中读取参数值
BOOL GetMyProfileInt( char* szKey, char* szSubKey, int& result)
{
	char szBuffer[255];
	long nBuffersize = 255;
	HKEY hkKYKY;   
	HKEY hkKey;
	if( RegOpenKey( HKEY_CURRENT_CONFIG, "KYKY", &hkKYKY ) != ERROR_SUCCESS )
		return FALSE;
	if( RegOpenKey( hkKYKY, szKey, &hkKey ) != ERROR_SUCCESS )
	{
			RegCloseKey( hkKey );
			return FALSE;
	}
	if( RegQueryValue( hkKey, szSubKey, szBuffer,  &nBuffersize) == ERROR_SUCCESS )
		result = atoi( szBuffer );
	RegCloseKey( hkKey );
	RegCloseKey( hkKYKY );
	return TRUE;
}

BOOL GetMyProfileIntEx( LPCTSTR lpKey, LPCTSTR lpSubKey, int& value)
{
	char szBuf[MYBUFSIZE];
	DWORD dwSize = MYBUFSIZE;
	HKEY hk;
   	if( RegOpenKeyEx( HKEY_CURRENT_USER, 
					lpKey, 
					0, 
					KEY_ALL_ACCESS,
					&hk ) != ERROR_SUCCESS )
		return FALSE;
	if( RegQueryValueEx( hk, 
					lpSubKey,
					NULL,
					NULL,
					(LPBYTE)szBuf,
					&dwSize) == ERROR_SUCCESS )
		value = atoi( szBuf );
	RegCloseKey(hk);
	return TRUE;
}

BOOL GetMyProfileDouble( char* szKey, char* szSubKey, double& result)
{
	char szBuffer[255];
	long nBuffersize = 255;
	HKEY hkKYKY;   
	HKEY hkKey;
	if( RegOpenKey( HKEY_CURRENT_CONFIG, "KYKY", &hkKYKY ) != ERROR_SUCCESS )
		return FALSE;
	if( RegOpenKey( hkKYKY, szKey, &hkKey ) != ERROR_SUCCESS )
	{
			RegCloseKey( hkKey );
			return FALSE;
	}
	if( RegQueryValue( hkKey, szSubKey, szBuffer,  &nBuffersize) == ERROR_SUCCESS )
		result = atof( szBuffer );
	RegCloseKey( hkKey );
	RegCloseKey( hkKYKY );
	return TRUE;
}

BOOL GetMyProfileDoubleEx( LPCTSTR lpKey, LPCTSTR lpSubKey, double& value)
{
	char szBuf[MYBUFSIZE];
	DWORD dwSize = MYBUFSIZE;
	HKEY hk;
   	if( RegOpenKeyEx( HKEY_CURRENT_USER, 
					lpKey, 
					0, 
					KEY_ALL_ACCESS,
					&hk ) != ERROR_SUCCESS )
		return FALSE;
	if( RegQueryValueEx( hk, 
					lpSubKey,
					NULL,
					NULL,
					(LPBYTE)szBuf,
					&dwSize) == ERROR_SUCCESS )
		value = atof( szBuf );
	RegCloseKey(hk);
	return TRUE;
}

BOOL GetMyProfileLong( char* szKey, char* szSubKey, long& result)
{
	char szBuffer[255];
	long nBuffersize = 255;
	HKEY hkKYKY;   
	HKEY hkKey;
	if( RegOpenKey( HKEY_CURRENT_CONFIG, "KYKY", &hkKYKY ) != ERROR_SUCCESS )
		return FALSE;
	if( RegOpenKey( hkKYKY, szKey, &hkKey ) != ERROR_SUCCESS )
	{
			RegCloseKey( hkKey );
			return FALSE;
	}
	if( RegQueryValue( hkKey, szSubKey, szBuffer,  &nBuffersize) == ERROR_SUCCESS )
		result = atol( szBuffer );
	RegCloseKey( hkKey );
	RegCloseKey( hkKYKY );
	return TRUE;
}

BOOL GetMyProfileLongEx( LPCTSTR lpKey, LPCTSTR lpSubKey, long& value)
{
	char szBuf[MYBUFSIZE];
	DWORD dwSize = MYBUFSIZE;
	HKEY hk;
   	if( RegOpenKeyEx( HKEY_CURRENT_USER, 
					lpKey, 
					0, 
					KEY_ALL_ACCESS,
					&hk ) != ERROR_SUCCESS )
		return FALSE;
	if( RegQueryValueEx( hk, 
					lpSubKey,
					NULL,
					NULL,
					(LPBYTE)szBuf,
					&dwSize) == ERROR_SUCCESS )
		value = atol( szBuf );
	RegCloseKey(hk);
	return TRUE;
}

BOOL GetMyProfileString( char* szKey, char* szSubKey, CString& result)
{
	char szBuffer[255];
	long nBuffersize = 255;
	HKEY hkKYKY;   
	HKEY hkKey;
	if( RegOpenKey( HKEY_CURRENT_CONFIG, "KYKY", &hkKYKY ) != ERROR_SUCCESS )
		return FALSE;
	if( RegOpenKey( hkKYKY, szKey, &hkKey ) != ERROR_SUCCESS )
	{
			RegCloseKey( hkKey );
			return FALSE;
	}
	if( RegQueryValue( hkKey, szSubKey, szBuffer,  &nBuffersize) == ERROR_SUCCESS )
		result.Format( "%s", szBuffer );
	RegCloseKey( hkKey );
	RegCloseKey( hkKYKY );
	return TRUE;
}

BOOL GetMyProfileStringEx( LPCTSTR lpKey, LPCTSTR lpSubKey, CString& value)
{
	char szBuf[MYBUFSIZE];
	DWORD dwSize = MYBUFSIZE;
	HKEY hk;
   	if( RegOpenKeyEx( HKEY_CURRENT_USER, 
					lpKey, 
					0, 
					KEY_ALL_ACCESS,
					&hk ) != ERROR_SUCCESS )
		return FALSE;
	if( RegQueryValueEx( hk, 
					lpSubKey,
					NULL,
					NULL,
					(LPBYTE)szBuf,
					&dwSize) == ERROR_SUCCESS )
		value.Format( "%s", szBuf );
	RegCloseKey(hk);
	return TRUE;
}

//向注册表中写入参数值
BOOL SetMyProfileInt( char* szKey, char* szSubKey, const int value)
{
	char szBuffer[255];
	long nBuffersize = 255;
	sprintf( szBuffer, "%d", value );
	HKEY hkKYKY;   
	HKEY hkKey;
	HKEY hkSubKey;
	DWORD dwDisposition;
   	RegCreateKeyEx( HKEY_CURRENT_CONFIG, 
					"KYKY", 
					NULL, 
					"KYKY",
					REG_OPTION_NON_VOLATILE,
					KEY_ALL_ACCESS,
					NULL, 
					&hkKYKY,
					&dwDisposition );
   	RegCreateKeyEx( hkKYKY, 
					szKey, 
					NULL, 
					szKey,
					REG_OPTION_NON_VOLATILE,
					KEY_ALL_ACCESS,
					NULL, 
					&hkKey,
					&dwDisposition );
   	RegCreateKeyEx( hkKey, 
					szSubKey, 
					NULL, 
					szSubKey,
					REG_OPTION_NON_VOLATILE,
					KEY_ALL_ACCESS,
					NULL, 
					&hkSubKey,
					&dwDisposition );

//	RegOpenKey( HKEY_CURRENT_CONFIG, "KYKY", &hkKYKY );
//	RegOpenKey( hkKYKY, szKey, &hkKey );
	RegSetValue( hkKey, szSubKey, REG_SZ, szBuffer,  nBuffersize);
	RegCloseKey( hkSubKey );
	RegCloseKey( hkKey );
	RegCloseKey( hkKYKY );
	return TRUE;
}

BOOL SetMyProfileIntEx( LPCTSTR lpKey, LPCTSTR lpSubKey, const int value)
{
	char szBuf[255];
	sprintf( szBuf, "%d", value );

	HKEY hk;
	DWORD dwDisposition;
   	RegCreateKeyEx( HKEY_CURRENT_USER, 
					lpKey, 
					0, 
					NULL,
					REG_OPTION_NON_VOLATILE,
					KEY_ALL_ACCESS,
					NULL, 
					&hk,
					&dwDisposition );
	RegSetValueEx( hk,
					lpSubKey,
					0,
					REG_SZ,
					(LPBYTE) szBuf,
					strlen(szBuf) + 1 );
	RegCloseKey(hk); 
	return TRUE;
}

BOOL SetMyProfileDouble( char* szKey, char* szSubKey, const double value)
{
	char szBuffer[255];
	long nBuffersize = 255;
	sprintf( szBuffer, "%f", value );
	HKEY hkKYKY;   
	HKEY hkKey;
	HKEY hkSubKey;
	DWORD dwDisposition;
   	RegCreateKeyEx( HKEY_CURRENT_CONFIG, 
					"KYKY", 
					NULL, 
					"KYKY",
					REG_OPTION_NON_VOLATILE,
					KEY_ALL_ACCESS,
					NULL, 
					&hkKYKY,
					&dwDisposition );
   	RegCreateKeyEx( hkKYKY, 
					szKey, 
					NULL, 
					szKey,
					REG_OPTION_NON_VOLATILE,
					KEY_ALL_ACCESS,
					NULL, 
					&hkKey,
					&dwDisposition );
   	RegCreateKeyEx( hkKey, 
					szSubKey, 
					NULL, 
					szSubKey,
					REG_OPTION_NON_VOLATILE,
					KEY_ALL_ACCESS,
					NULL, 
					&hkSubKey,
					&dwDisposition );

//	RegOpenKey( HKEY_CURRENT_CONFIG, "KYKY", &hkKYKY );
//	RegOpenKey( hkKYKY, szKey, &hkKey );
	RegSetValue( hkKey, szSubKey, REG_SZ, szBuffer,  nBuffersize);
	RegCloseKey( hkSubKey );
	RegCloseKey( hkKey );
	RegCloseKey( hkKYKY );
	return TRUE;
}

BOOL SetMyProfileDoubleEx( LPCTSTR lpKey, LPCTSTR lpSubKey, const double value)
{
	char szBuf[255];
	sprintf( szBuf, "%.6f", value );

	HKEY hk;
	DWORD dwDisposition;
   	RegCreateKeyEx( HKEY_CURRENT_USER, 
					lpKey, 
					0, 
					NULL,
					REG_OPTION_NON_VOLATILE,
					KEY_ALL_ACCESS,
					NULL, 
					&hk,
					&dwDisposition );
	RegSetValueEx( hk,
					lpSubKey,
					0,
					REG_SZ,
					(LPBYTE) szBuf,
					strlen(szBuf) + 1 );
	RegCloseKey(hk); 
	return TRUE;
}

BOOL SetMyProfileString( char* szKey, char* szSubKey, CString& value )
{
	char szBuffer[255];
	long nBuffersize = 255;
	sprintf( szBuffer, "%s", value );
	HKEY hkKYKY;   
	HKEY hkKey;
	HKEY hkSubKey;
	DWORD dwDisposition;
   	RegCreateKeyEx( HKEY_CURRENT_CONFIG, 
					"KYKY", 
					NULL, 
					"KYKY",
					REG_OPTION_NON_VOLATILE,
					KEY_ALL_ACCESS,
					NULL, 
					&hkKYKY,
					&dwDisposition );
   	RegCreateKeyEx( hkKYKY, 
					szKey, 
					NULL, 
					szKey,
					REG_OPTION_NON_VOLATILE,
					KEY_ALL_ACCESS,
					NULL, 
					&hkKey,
					&dwDisposition );
   	RegCreateKeyEx( hkKey, 
					szSubKey, 
					NULL, 
					szSubKey,
					REG_OPTION_NON_VOLATILE,
					KEY_ALL_ACCESS,
					NULL, 
					&hkSubKey,
					&dwDisposition );

//	RegOpenKey( HKEY_CURRENT_CONFIG, "KYKY", &hkKYKY );
//	RegOpenKey( hkKYKY, szKey, &hkKey );
	RegSetValue( hkKey, szSubKey, REG_SZ, szBuffer,  nBuffersize);
	RegCloseKey( hkSubKey );
	RegCloseKey( hkKey );
	RegCloseKey( hkKYKY );
	return TRUE;
}

BOOL SetMyProfileStringEx( LPCTSTR lpKey, LPCTSTR lpSubKey, CString value)
{
	char szBuf[255];
	sprintf( szBuf, "%s", value );

	HKEY hk;
	DWORD dwDisposition;
   	RegCreateKeyEx( HKEY_CURRENT_USER, 
					lpKey, 
					0, 
					NULL,
					REG_OPTION_NON_VOLATILE,
					KEY_ALL_ACCESS,
					NULL, 
					&hk,
					&dwDisposition );
	RegSetValueEx( hk,
					lpSubKey,
					0,
					REG_SZ,
					(LPBYTE) szBuf,
					strlen(szBuf) + 1 );
	RegCloseKey(hk); 
	return TRUE;
}




////////////////////////////////////////////////////////////////
//Read data in one line in *.ini.
//In one line the data like ***,*****,*****, they are seperated by comma.

int ReadInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator)
{
	char szBuf1[255];
	char szBuf2[255];
	int* pint = (int*) pData;
	double* pdouble = (double*)pData;
	long* plong = (long*)pData;
	int i = 0;
	if(GetPrivateProfileString(Section, Entry, "", szBuf1, 255, path) != 0)
	{
		for(i=0; i<nTotal; i++)
		{
			strcpy(szBuf2, szBuf1);
			if(strchr(szBuf2, Separator) != NULL)
			{
				strcpy(szBuf1, strchr(szBuf2, Separator)+1);
				*strchr(szBuf2, Separator) = '\0';
				switch( uDataStyle )
				{
				case	INTSTYLE:
					pint[i] = atoi(szBuf2);
					break;
				case	DOUBLESTYLE:
					pdouble[i] = atof(szBuf2);
					break;
				case	LONGSTYLE:
					plong[i] = atol(szBuf2);
					break;
				default:
					return 0;
				}
			}
			else
			{
				if(i == nTotal-1)
				{
					// This is the last data.
					switch( uDataStyle )
					{
					case	INTSTYLE:
						pint[i] = atoi(szBuf2);
						break;
					case	DOUBLESTYLE:
						pdouble[i] = atof(szBuf2);
						break;
					case	LONGSTYLE:
						plong[i] = atol(szBuf2);
						break;
					default:
						return 0;
					}
					nTotal = i+1;
				}
				else
					nTotal = i;
				break;
			}
		}
	}
	return i+1;
}

int SaveInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator)
{
	char szBuf1[255];
	char szBuf2[255];
	*szBuf1 = '\0';
	int* pint = (int*) pData;
	double* pdouble = (double*)pData;
	long* plong = (long*)pData;
	int i = 0;
	for(i=0; i<nTotal; i++)
	{
		if(i == nTotal-1)
		{
			// This is the last data.
			switch( uDataStyle )
			{
			case	INTSTYLE:
				sprintf(szBuf2, "%d", pint[i]);
				break;
			case	DOUBLESTYLE:
				sprintf(szBuf2, "%f", pdouble[i]);
				break;
			case	LONGSTYLE:
				sprintf(szBuf2, "%ld", plong[i]);
				break;
			default:
				return 0;
			}
		}
		else
		{
			switch( uDataStyle )
			{
			case	INTSTYLE:
				sprintf(szBuf2, "%d%c", pint[i], Separator);
				break;
			case	DOUBLESTYLE:
				sprintf(szBuf2, "%f%c", pdouble[i], Separator);
				break;
			case	LONGSTYLE:
				sprintf(szBuf2, "%ld%c", plong[i], Separator);
				break;
			default:
				return 0;
			}
		}
		strcat(szBuf1, szBuf2);
	}
	WritePrivateProfileString(Section, Entry, szBuf1, path);
	return nTotal;
}
