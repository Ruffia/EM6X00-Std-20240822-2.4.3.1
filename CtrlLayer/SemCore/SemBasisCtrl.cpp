// SemBasisCtrl.cpp: implementation of the CSemBasisCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemBasisCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemBasisCtrl::CSemBasisCtrl()
{
	m_strText				= _T("CSemBasisCtrl");

	m_lastTickCount = 0;
	m_nOpr			= 0;
	m_nType			= 0;
}

CSemBasisCtrl::~CSemBasisCtrl()
{

}

//09.03.23
BOOL CSemBasisCtrl::BindLanguageManager( CLanguageManagerCtrl* pLanguageManCtrl )
{
	m_pLanguageMan = pLanguageManCtrl;
	if( m_pLanguageMan == NULL )
		return FALSE;
	else
		return TRUE;
}

BOOL CSemBasisCtrl::CoherenceCheck()
{
	BOOL blRet = TRUE;
	
	if ( m_scfCtrl.varMax.lVal < m_scfCtrl.varMin.lVal )
	{
		m_scfCtrl.varMax.lVal = m_scfCtrl.varMin.lVal;
		blRet = FALSE;
	}

	if ( m_scfCtrl.varPos.lVal > m_scfCtrl.varMax.lVal )
	{
		m_scfCtrl.varPos.lVal = m_scfCtrl.varMax.lVal;
		blRet = FALSE;
	}

	if ( m_scfCtrl.varPos.lVal < m_scfCtrl.varMin.lVal )
	{
		m_scfCtrl.varPos.lVal = m_scfCtrl.varMin.lVal;
		blRet = FALSE;
	}

	return blRet;
}

void CSemBasisCtrl::SetRange( VARIANT varMin, VARIANT varMax )
{
	CoherenceCheck();
	
	m_scfCtrl.varMin = varMin;
	m_scfCtrl.varMax = varMax;
}

VARIANT CSemBasisCtrl::SetPos( VARIANT varPos )
{
	m_scfCtrl.varPos = varPos;

	CoherenceCheck();

	return m_scfCtrl.varPos;
}

long CSemBasisCtrl::GetPosByValue(const double dValue)
{
	return 0L;
}

VARIANT CSemBasisCtrl::Plus( VARIANT verDelt )
{
	m_scfCtrl.varPos.lVal += verDelt.lVal;
	
	CoherenceCheck();

	return m_scfCtrl.varPos;
}

VARIANT CSemBasisCtrl::Minus( VARIANT verDelt )
{
	m_scfCtrl.varPos.lVal -= verDelt.lVal;

	CoherenceCheck();

	return m_scfCtrl.varPos;
}

BOOL CSemBasisCtrl::Flush()
{
	DWORD thisTickCount = GetTickCount();
	if ((thisTickCount - m_lastTickCount) < 10)	// 10ms响应一次
		return FALSE;

	m_lastTickCount = thisTickCount;
	return TRUE;
}

int ReadInOneLine(char *path, char *Section, char *Entry, UINT uDataStyle, int nTotal, void* pData, char Separator = ',');

BOOL CSemBasisCtrl::ReadINI( int nType, int nIndex, double* pdValue )	// 读取配置文件，获取参数
{
	TCHAR exeFullPath[MAX_PATH];
	DWORD i, dwLength;

	dwLength = GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	for ( i=dwLength-1; i > 0; i-- )
	{
		if ( exeFullPath[i] == '\\' )
		{
			exeFullPath[i+1] = '\0';
			break;
		}
	}
	
	if ( dwLength == 0 )
		return FALSE;

	strcat(exeFullPath, m_strINIfileName);
	char szBuf[255];
	CString str = "";
	if( nType == INI_Ratio )
	{
		if( nIndex == INIIndex_OL )
			str = "OLmap";
		else if( nIndex == INIIndex_MagK )
		{
			if( ReadInOneLine(exeFullPath, "Ratioes", "MagK", 2, 10, pdValue) == 10 )
				return TRUE;
			else
				return FALSE;
		}
		if(GetPrivateProfileString("Ratioes", str, "", szBuf, 15, exeFullPath) != 0)
		{
			*pdValue = atof( szBuf );
			return TRUE;
		}
	}
	else if( nType == INI_CaliOffset )
	{
		if( nIndex == INIIndex_ShiftX )
			str = "ShiftX";
		else if( nIndex == INIIndex_ShiftY )
			str = "ShiftY";
		else if( nIndex == INIIndex_StigX )
			str = "StigX";
		else if( nIndex == INIIndex_StigY )
			str = "StigY";
		else if( nIndex == INIIndex_AlignX )
			str = "AlignX";
		else if( nIndex == INIIndex_AlignY )
			str = "AlignY";
		if(GetPrivateProfileString("Calibrate_Offset", str, "", szBuf, 15, exeFullPath) != 0)
		{
			*pdValue = atof( szBuf );
			return TRUE;
		}
	}
	else if( nType == INI_Debug )
	{
		if( nIndex == INIIndex_Start )
			str = "Start";
		if(GetPrivateProfileString("Debug", str, "", szBuf, 15, exeFullPath) != 0)
		{
			*pdValue = atof( szBuf );
			return TRUE;
		}
	}
	else if( nType == INI_DFforFF )
	{
		str = "FFsteps";
		if(GetPrivateProfileString("Options", str, "", szBuf, 15, exeFullPath) != 0)
		{
			*pdValue = atof( szBuf );
			return TRUE;
		}
	}
	else if( nType == INI_Coadjust )
	{
		if( nIndex == INIIndex_OL )
		{
			if( ReadInOneLine(exeFullPath, "Coadjust", "Equation", 2, 2, pdValue) == 2 )
				return TRUE;
			else
				return FALSE;
		}
		else if( nIndex == INIIndex_WD )
		{
			if( ReadInOneLine(exeFullPath, "Coadjust", "WD", 2, 2, pdValue) == 2 )
				return TRUE;
			else
				return FALSE;
		}
		else if( nIndex == INIIndex_Factors )
		{
			if( ReadInOneLine(exeFullPath, "Coadjust", "Factors", 2, 3, pdValue) == 3 )
				return TRUE;
			else
				return FALSE;
		}
		else if( nIndex == INIIndex_Factors2 )
		{
			if( ReadInOneLine(exeFullPath, "Coadjust", "Factors2", 2, 3, pdValue) == 3 )
				return TRUE;
			else
				return FALSE;
		}
		else if( nIndex == INIIndex_FactorMin )
		{
			if( ReadInOneLine(exeFullPath, "Coadjust", "FactorsWDmin", 2, 2, pdValue) == 2 )
				return TRUE;
			else
				return FALSE;
		}
		else if( nIndex == INIIndex_MagChange )
		{
			if( ReadInOneLine(exeFullPath, "Coadjust", "MagChange", 2, 5, pdValue) == 5 )
				return TRUE;
			else
				return FALSE;
		}
		else if( nIndex == INIIndex_HV )
		{
			if(ReadInOneLine(exeFullPath, "Coadjust", "FactorsHV", 2, 6, pdValue) == 6 )
				return TRUE;
			else
				return FALSE;
		}
		else if( nIndex == INIIndex_Rotate )
		{
			if(ReadInOneLine(exeFullPath, "Coadjust", "FactorsRotate", 2, 2, pdValue) == 2 )
				return TRUE;
			else
				return FALSE;
		}
	}

	return FALSE;
}
