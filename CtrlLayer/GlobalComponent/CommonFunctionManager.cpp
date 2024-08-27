#include "stdafx.h"
#include "CommonFunctionManager.h"

CCommonFunctionManager& CCommonFunctionManager::Instance()
{
	static CCommonFunctionManager Inst;
	return Inst;
}


CCommonFunctionManager::CCommonFunctionManager()
{	
}


BOOL CCommonFunctionManager::GetMonitorSizeFromEDID(double &dblPixelsPerMM_HORZ, double &dblPixelsPerMM_VERT)
{
	//read edid
	BOOL bRes = FALSE;
	DWORD dwPhysicalW, dwPhysicalH, dwWidth, dwHeight;
	dwPhysicalW = dwPhysicalH = dwWidth = dwHeight = 0;

	TCHAR str[255] = "SYSTEM\\CurrentControlSet\\Enum\\DISPLAY\\";
	HKEY hKey;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, str, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		DWORD i = 0;
		DWORD size = MAX_PATH;
		FILETIME ft;
		while(RegEnumKeyEx(hKey, i, str, &size, NULL, NULL, NULL, &ft) == ERROR_SUCCESS)
		{
			HKEY hKey2;
			if(RegOpenKeyEx(hKey, str, 0, KEY_READ, &hKey2) == ERROR_SUCCESS)
			{
				size = MAX_PATH;

				DWORD j = 0;
				FILETIME ftj;
				while(RegEnumKeyEx(hKey2, j, str, &size, NULL, NULL, NULL, &ftj) == ERROR_SUCCESS)
				{
					HKEY hKey2x;
					if( RegOpenKeyEx( hKey2, str, 0, KEY_READ, &hKey2x) == ERROR_SUCCESS)
					{
						HKEY hKey3;
						if(RegOpenKeyEx(hKey2x, "Control", 0, KEY_READ, &hKey3) == ERROR_SUCCESS)
						{
							RegCloseKey( hKey3 );
							if(RegOpenKeyEx(hKey2x, "Device Parameters", 0, KEY_READ, &hKey3) == ERROR_SUCCESS)
							{
								BYTE EDID[256];
								size = 256;
								if(RegQueryValueEx(hKey3, "EDID", NULL, NULL, (LPBYTE)&EDID, &size) == ERROR_SUCCESS)
								{
									dwPhysicalW	= EDID[21] * 10;	// 屏幕实际宽度尺寸mm
									dwPhysicalH	= EDID[22] * 10;	// 屏幕实际高度尺寸mm
									dwWidth		= (EDID[58] & 0xF0) << 4 | EDID[56];	// 分辨率X
									dwHeight	= (EDID[61] & 0xF0) << 4 | EDID[59];	// 分辨率Y
									dblPixelsPerMM_HORZ = 1.0 * dwWidth / dwPhysicalW;
									dblPixelsPerMM_VERT = 1.0 * dwHeight / dwPhysicalH;
									bRes	= TRUE;
								}
								RegCloseKey(hKey3);
							}
						}
						RegCloseKey(hKey2x);
					}
					j++;
				}
				RegCloseKey(hKey2);
			}
			i++;
		}
		RegCloseKey(hKey);
	}
	return bRes;
}


//Read data in one line in *.ini.
//In one line the data like ***,*****,*****, they are seperated by comma.
int	  CCommonFunctionManager::ReadInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator)
{
	char szBuf1[255] = {0};
	char szBuf2[255] = {0};
	int* pint = (int*) pData;
	double* pdouble = (double*)pData;
	long* plong = (long*)pData;
	CString* pstr = (CString*)pData;
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
				case	STRSTYLE:
					pstr[i] = szBuf2;
					break;
				default:
					return 0;
				}
			}
			else
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
				case	STRSTYLE:
					pstr[i] = szBuf2;
					break;
				default:
					return 0;
				}
				if(i == nTotal-1)
				{
					nTotal = i+1;
				}
				else
				{
					nTotal = i;
				}
				break;
			}
		}
	}
	return i+1;
}


int	  CCommonFunctionManager::SaveInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator)
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
