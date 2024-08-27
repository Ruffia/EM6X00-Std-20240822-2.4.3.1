#include "stdafx.h"
#include "AppOption.h"


CAppOption& CAppOption::Instance()
{
	static CAppOption Inst;
	return Inst;
}


CAppOption::CAppOption()
{
	

}


BOOL CAppOption::SystemSettingsChanged(const SYS_Options& sysOpt)
{
	if( (m_sysOpt.color[SysOpt_clrBack] != sysOpt.color[SysOpt_clrBack])
		|| (m_sysOpt.color[SysOpt_clrEditText] != sysOpt.color[SysOpt_clrEditText])
		|| (m_sysOpt.color[SysOpt_clrLabelText] != sysOpt.color[SysOpt_clrLabelText])
		|| (m_sysOpt.color[SysOpt_clrLabelLogoText] != sysOpt.color[SysOpt_clrLabelLogoText])
		|| (m_sysOpt.color[SysOpt_clrMobileUScaleText] != sysOpt.color[SysOpt_clrMobileUScaleText])
		|| (m_sysOpt.color[SysOpt_clrMobileUScaleLine] != sysOpt.color[SysOpt_clrMobileUScaleLine])
		|| (m_sysOpt.color[SysOpt_clrOscillographGrid] != sysOpt.color[SysOpt_clrOscillographGrid])
		|| (m_sysOpt.color[SysOpt_clrOscillographGraph] != sysOpt.color[SysOpt_clrOscillographGraph])
		|| (m_sysOpt.fName[SysOpt_nameEditText] != sysOpt.fName[SysOpt_nameEditText])
		|| (m_sysOpt.fName[SysOpt_nameLabelText] != sysOpt.fName[SysOpt_nameLabelText])
		|| (m_sysOpt.fName[SysOpt_nameMobileUScaleText] != sysOpt.fName[SysOpt_nameMobileUScaleText])
		|| (m_sysOpt.nShowParams[SysOpt_showLabelTrans] != sysOpt.nShowParams[SysOpt_showLabelTrans])
		|| (m_sysOpt.nLineWidth != sysOpt.nLineWidth) )
	{
		return TRUE;
	}

	return FALSE;
}


int CAppOption::ReadFooterSettings( char* path )
{
	int i=0;
	for( i=0; i<11; i++)
		m_footerOpt.opt[i].bVisible	= FALSE;

	char szBuf[20], szBuf1[255], szBuf2[255], szBuf3[50];
	int nLines = 0;
	if(GetPrivateProfileString( "FooterSettings", "Lines", "", szBuf, 10, path) != 0 )
		nLines = atoi(szBuf);
	if( nLines < 1 )
		return -1;
	int nPos = 0;
	for( i=0; i<nLines; i++ )
	{
		sprintf(szBuf, "MacroLine%d", i+1);
		if(GetPrivateProfileString( "FooterSettings", szBuf, "", szBuf1, 255, path) != 0 )
		{
			strcpy(szBuf2, szBuf1);
			while(strchr(szBuf2, '}') != NULL)
			{
				strcpy(szBuf1, strchr(szBuf2, '}')+1);	// szBuf1中是该行剩余字符串
				*strchr(szBuf2, '}') = '\0';			// szBuf2中是字符串{HV;x=0
				strcpy(szBuf3, szBuf2+1);				// szBuf3中是当前字符串HV;x=0
				strcpy(szBuf2, strrchr(szBuf3, '=')+1);	// szBuf2中是当前字符串的位置
				nPos = atoi(szBuf2);
				*strchr(szBuf3, ';') = '\0';			// szBuf3中是带标识的当前字符串的内容
				strcpy(szBuf2, szBuf3+1);				// szBuf2中是当前字符串的内容
				switch(szBuf3[0])
				{
				case	't':	// Time
					m_footerOpt.opt[0].bVisible	= TRUE;
					m_footerOpt.opt[0].ptPos.x	= nPos;
					m_footerOpt.opt[0].ptPos.y	= i+1;
					_ReadFooter_TimeFormat(szBuf2);
					break;
				case	'h':	// HV
					m_footerOpt.opt[1].bVisible	= TRUE;
					m_footerOpt.opt[1].ptPos.x	= nPos;
					m_footerOpt.opt[1].ptPos.y	= i+1;
					m_footerOpt.opt[1].strDisp	= szBuf2;
					break;
				case	'w':	// WD
					m_footerOpt.opt[2].bVisible	= TRUE;
					m_footerOpt.opt[2].ptPos.x	= nPos;
					m_footerOpt.opt[2].ptPos.y	= i+1;
					m_footerOpt.opt[2].strDisp	= szBuf2;
					break;
				case	'm':	// Mag
					m_footerOpt.opt[3].bVisible	= TRUE;
					m_footerOpt.opt[3].ptPos.x	= nPos;
					m_footerOpt.opt[3].ptPos.y	= i+1;
					m_footerOpt.opt[3].strDisp	= szBuf2;
					break;
				case	'd':	// DET
					m_footerOpt.opt[4].bVisible	= TRUE;
					m_footerOpt.opt[4].ptPos.x	= nPos;
					m_footerOpt.opt[4].ptPos.y	= i+1;
					m_footerOpt.opt[4].strDisp	= szBuf2;
					break;
				case	'l':	// Logo
					m_footerOpt.opt[5].bVisible	= TRUE;
					m_footerOpt.opt[5].ptPos.x	= nPos;
					m_footerOpt.opt[5].ptPos.y	= i+1;
					m_footerOpt.opt[5].strDisp	= szBuf2;
					break;
				case	'u':	// uBar
					m_footerOpt.opt[6].bVisible	= TRUE;
					m_footerOpt.opt[6].ptPos.x	= nPos;
					m_footerOpt.opt[6].ptPos.y	= i+1;
					m_footerOpt.opt[6].strDisp	= szBuf2;
					break;
				case	'n':	// Note
					m_footerOpt.opt[7].bVisible	= TRUE;
					m_footerOpt.opt[7].ptPos.x	= nPos;
					m_footerOpt.opt[7].ptPos.y	= i+1;
					m_footerOpt.opt[7].strDisp	= szBuf2;
					break;
				case	'v':	// Vac
					m_footerOpt.opt[8].bVisible	= TRUE;
					m_footerOpt.opt[8].ptPos.x	= nPos;
					m_footerOpt.opt[8].ptPos.y	= i+1;
					m_footerOpt.opt[8].strDisp	= szBuf2;
					break;
				case	'e':	// EMI
					m_footerOpt.opt[9].bVisible	= TRUE;
					m_footerOpt.opt[9].ptPos.x	= nPos;
					m_footerOpt.opt[9].ptPos.y	= i+1;
					m_footerOpt.opt[9].strDisp	= szBuf2;
					break;
				case	's':	// Speed
					m_footerOpt.opt[10].bVisible	= TRUE;
					m_footerOpt.opt[10].ptPos.x	= nPos;
					m_footerOpt.opt[10].ptPos.y	= i+1;
					m_footerOpt.opt[10].strDisp	= szBuf2;
					break;
				}
				strcpy(szBuf2, szBuf1);
			}
		}
	}
	return 0;
}


void CAppOption::SetFooterSettingsDefault()
{
	// Time
	m_footerOpt.opt[0].bVisible	= TRUE;
	m_footerOpt.opt[0].ptPos.x	= 75;
	m_footerOpt.opt[0].ptPos.y	= 1;
	m_footerOpt.opt[0].strDisp	= "%Y.%m.%d %H:%M";
	// HV
	m_footerOpt.opt[1].bVisible	= TRUE;
	m_footerOpt.opt[1].ptPos.x	= 2;
	m_footerOpt.opt[1].ptPos.y	= 1;
	m_footerOpt.opt[1].strDisp	= "HV";
	// WD
	m_footerOpt.opt[2].bVisible	= TRUE;
	m_footerOpt.opt[2].ptPos.x	= 2;
	m_footerOpt.opt[2].ptPos.y	= 2;
	m_footerOpt.opt[2].strDisp	= "WD";
	// Mag
	m_footerOpt.opt[3].bVisible	= TRUE;
	m_footerOpt.opt[3].ptPos.x	= 25;
	m_footerOpt.opt[3].ptPos.y	= 1;
	m_footerOpt.opt[3].strDisp	= "MAG";
	// DET
	m_footerOpt.opt[4].bVisible	= TRUE;
	m_footerOpt.opt[4].ptPos.x	= 25;
	m_footerOpt.opt[4].ptPos.y	= 2;
	m_footerOpt.opt[4].strDisp	= "DET";
	// Logo
	m_footerOpt.opt[5].bVisible	= TRUE;
	m_footerOpt.opt[5].ptPos.x	= 50;
	m_footerOpt.opt[5].ptPos.y	= 2;
	m_footerOpt.opt[5].strDisp	= "Logo";
	// uBar
	m_footerOpt.opt[6].bVisible	= TRUE;
	m_footerOpt.opt[6].ptPos.x	= 50;
	m_footerOpt.opt[6].ptPos.y	= 1;
	m_footerOpt.opt[6].strDisp	= "uBar";
	// Note
	m_footerOpt.opt[7].bVisible	= TRUE;
	m_footerOpt.opt[7].ptPos.x	= 75;
	m_footerOpt.opt[7].ptPos.y	= 2;
	m_footerOpt.opt[7].strDisp	= "Note";
	// Vac
	m_footerOpt.opt[8].bVisible	= FALSE;
	m_footerOpt.opt[8].ptPos.x	= 0;
	m_footerOpt.opt[8].ptPos.y	= 0;
	m_footerOpt.opt[8].strDisp	= "Vac";
	// EMI
	m_footerOpt.opt[9].bVisible	= FALSE;
	m_footerOpt.opt[9].ptPos.x	= 0;
	m_footerOpt.opt[9].ptPos.y	= 0;
	m_footerOpt.opt[9].strDisp	= "EMI";
	// Speed
	m_footerOpt.opt[10].bVisible	= FALSE;
	m_footerOpt.opt[10].ptPos.x	= 0;
	m_footerOpt.opt[10].ptPos.y	= 0;
	m_footerOpt.opt[10].strDisp	= "Speed";
}


void CAppOption::_ReadFooter_TimeFormat( char* buf )
{
	// buf = "Time=Y/m/d H/M"
	char szBuf[100], szBuf1[20], szPer, ch;
	strcpy( szBuf1, buf );
	strcpy(szBuf, strchr(szBuf1, '=')+1);	// szBuf中是日期格式字符串

	szPer = '%';
	sprintf( szBuf1, "" );
	int len = strlen( szBuf );
	int index = 0;
	for( int i=0; i<len; i++ )
	{
		ch = szBuf[i];
		if(ch >='a' && ch<='z' ||  ch >='A' && ch<='Z')
		{
			szBuf1[index] = szPer;
			index++;
		}
		szBuf1[index] = ch;
		index++;
	}
	szBuf1[index] = '\0';
	m_footerOpt.opt[0].strDisp = szBuf1;
}
