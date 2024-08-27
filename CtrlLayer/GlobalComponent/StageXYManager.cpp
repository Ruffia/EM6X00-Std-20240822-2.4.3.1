#include "stdafx.h"
#include "StageXYManager.h"
#include "Message.h"
#include "DlgPrompt.h"
#include "DebugMessageDispatcher.h"
#include "LanguageManagerCtrl.h"

static const	unsigned	short	g_nAutoStageController			= 1;

CStageXYManager& CStageXYManager::Instance()
{
	static CStageXYManager Inst;
	return Inst;
}

CStageXYManager::CStageXYManager()
{
	// 15.11 自动台
	m_Stage.nHasAutoStage		= 0;
	m_Stage.xy.bReady			= FALSE;
	m_plistStagePos				= new ListStagePos;
	m_Stage.xy.ptCurPosAb.x		= 0;
	m_Stage.xy.ptCurPosAb.y		= 0;
	m_Stage.xy.nGratingX		= 500;
	m_Stage.xy.nGratingY		= 500;
	m_Stage.xy.nRangeX			= 50;
	m_Stage.xy.nRangeY			= 50;

	m_pLanguageManagerCtrl = NULL;
}


void CStageXYManager::Attach(HWND hWnd,CLanguageManagerCtrl* pLanguageManagerCtrl)
{
	m_hWnd = hWnd;
	m_pLanguageManagerCtrl = pLanguageManagerCtrl;
}

void CStageXYManager::Release()
{
	///////////////////////////////////////////////////////////////////////////////
	// 15.10 XY自动台自动归零
	m_DMCWin.Close();

	POSITION pos;
	while( (pos=m_plistStagePos->GetHeadPosition()) )
	{
		structStagePos* pItem = m_plistStagePos->GetAt( pos );
		m_plistStagePos->RemoveHead();
		delete pItem;
	}
	delete m_plistStagePos;
	///////////////////////////////////////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////////////////
// 15.10 自动台
///////////////////////////////////////////////////////////////////////////////

void CStageXYManager::Stage_XY_Init()
{
	// Initialize the DMCWin object
	m_DMCWin.SetController(g_nAutoStageController);	// Controller 1 in the Windows registry
	m_DMCWin.SethWnd(m_hWnd);	   			        // Our hWnd;

	// Open a connection to the controller 
	LONG rc = m_DMCWin.Open();
	if( rc != DMCNOERROR )
	{
		CString str;
		str.Format( "rc = %ld", rc );
		CDlgPrompt dlg(m_pLanguageManagerCtrl);
		dlg.m_byteType = (BYTE)0x01;
		dlg.m_strMsg = str;
		dlg.DoModal();
	}

	//
	// NOTE: We could have omitted the calls to SetController and SethWnd and
	//       called Open with arguments:
	//
	//    m_DMCWin.Open(nController, GetSafeHwnd());
	//

	if (m_DMCWin.IsConnected())
	{
/*		// Display the controller version information
		char szVersion[128];

		if (m_DMCWin.GetVersion(szVersion, sizeof(szVersion)) == 0)
			pEdit->SetWindowText(szVersion);*/
		// 机械复位
/*		CString str;
		m_DMCWin.Command( "HM;BG", szBuffer, sizeof(szBuffer) );
		// 移动到中心：50mm的台子，就是移动到25mm中心
		str.Format( "PA %ld,%ld;BG", g_nCenter /g_nGrating, g_nCenter /g_nGrating );
		m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
*/
		m_DMCWin.Command( "MO" );
		m_DMCWin.Command( "SHXY" );
		m_DMCWin.Command( "AC 1000000,1000000" );
		m_DMCWin.Command( "DC 16000000,16000000" );
		m_DMCWin.Command( "BL -2147483648,-2147483648" );
		m_DMCWin.Command( "FL 2147483647,2147483647" );
		m_Stage.xy.ptCurPosAb = CPoint(10000000,10000000);

		m_Stage.xy.bReady = TRUE;
	}
	else
	{
/*		// Report error
		char szMessage[128];
		CString TempString;

		m_DMCWin.GetErrorText(m_DMCWin.GetLastError(), szMessage, sizeof(szMessage));
		TempString = "Could not connect to controller. ";
		TempString += szMessage;
		AfxMessageBox(TempString);

		pEdit->SetWindowText("Not connected with controller");*/
		CDlgPrompt dlg(m_pLanguageManagerCtrl);
		dlg.m_byteType = (BYTE)0x01;
		dlg.m_strMsg = m_pLanguageManagerCtrl->GetResString(IDS_Stage_Device_NOT_Found);
		dlg.DoModal();
		m_Stage.xy.bReady = FALSE;
	}

	// 获取工作路径
	char path[256];
	::GetModuleFileName(0, path, 255);
	char *exd = strrchr(path, (int)'\\');
	if(exd != NULL)
		*exd = '\0';
	strcat(path, "\\KYKYcfg.ini");

	char	szBuf1[50], szBuf2[50], szBufRootSection[20];
	sprintf(szBufRootSection, "StageCfg");
	// 光栅尺
	if(GetPrivateProfileString(szBufRootSection, "Gratings", "0", szBuf1, 255, path) != 0)
	{
		strcpy(szBuf2, szBuf1);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf1, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_Stage.xy.nGratingX = atoi(szBuf2);
			m_Stage.xy.nGratingY = atoi(szBuf1);
		}
	}
	// 行程
	if(GetPrivateProfileString(szBufRootSection, "Ranges", "0", szBuf1, 255, path) != 0)
	{
		strcpy(szBuf2, szBuf1);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf1, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_Stage.xy.nRangeX = atoi(szBuf2);
			m_Stage.xy.nRangeY = atoi(szBuf1);
		}
	}
	// 找原点方法
	m_Stage.xy.nMtd = GetPrivateProfileInt( szBufRootSection, "Mtd", 0, path );
	// XY是否反向
	m_Stage.xy.bRev = GetPrivateProfileInt( szBufRootSection, "XY", 0, path );
	// X/Y的方向
	if(GetPrivateProfileString(szBufRootSection, "Dir", "0", szBuf1, 255, path) != 0)
	{
		strcpy(szBuf2, szBuf1);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf1, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			if( atol(szBuf2) > 0 )
			{
				m_Stage.xy.bRevX = FALSE;
				m_Stage.xy.sXLD	= "-";	// 向左为-
				m_Stage.xy.sXRD	= "+";	// 向右为+
			}
			else
			{
				m_Stage.xy.bRevX = TRUE;
				m_Stage.xy.sXLD	= "+";
				m_Stage.xy.sXRD	= "-";
			}
			if( atol(szBuf1) > 0 )
			{
				m_Stage.xy.bRevY = FALSE;
				m_Stage.xy.sYLD	= "-";	// 向上为-
				m_Stage.xy.sYRD	= "+";	// 向下为+
			}
			else
			{
				m_Stage.xy.bRevY = TRUE;
				m_Stage.xy.sYLD	= "+";
				m_Stage.xy.sYRD	= "-";
			}
		}
	}
	else
	{
		m_Stage.xy.bRevX = FALSE;
		m_Stage.xy.bRevY = FALSE;
		m_Stage.xy.sXLD	= "-";
		m_Stage.xy.sXRD	= "+";
		m_Stage.xy.sYLD	= "-";
		m_Stage.xy.sYRD	= "+";
	}

/*	if(GetPrivateProfileString(szBufRootSection, "Dir2", "0", szBuf1, 255, path) != 0)
	{
		strcpy(szBuf2, szBuf1);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf1, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			m_Stage.bIndex_XD = atol(szBuf2);
			m_Stage.bIndex_YD = atol(szBuf1);
		}
	}
	else
	{
		m_Stage.bIndex_XD = 1;
		m_Stage.bIndex_YD = 1;
	}
*/	// 找原点第一步命令，并据此得出软件应判断哪个标志位
	if(GetPrivateProfileString(szBufRootSection, "Cmd1", "0", szBuf1, 255, path) != 0)
	{
		m_Stage.xy.sCmdFI1.Format( "%s", szBuf1 );
		strcpy(szBuf2, szBuf1);
		if(strchr(szBuf2, ',') != NULL)
		{
			strcpy(szBuf1, strchr(szBuf2, ',')+1);
			*strchr(szBuf2, ',') = '\0';
			if( atol(szBuf2) > 0 )
			{
				m_Stage.xy.sCmdLX = "MG_LFX";	// X forward limit switch
				m_Stage.xy.sCmdLX2= "MG_LRX";
			}
			else
			{
				m_Stage.xy.sCmdLX = "MG_LRX";	// X reverse limit switch
				m_Stage.xy.sCmdLX2= "MG_LFX";
			}

//			strcpy(szBuf2, strchr(szBuf1, ',')+1);
			if( atol(szBuf2) > 0 )
			{
				m_Stage.xy.sCmdLY = "MG_LFY";	// Y forward limit switch
				m_Stage.xy.sCmdLY2= "MG_LRY";
			}
			else
			{
				m_Stage.xy.sCmdLY = "MG_LRY";	// Y reverse limit switch
				m_Stage.xy.sCmdLY2= "MG_LFY";
			}
		}
	}
	else
	{
		m_Stage.xy.sCmdFI1	= "-5000,5000";
		m_Stage.xy.sCmdLX	= "MG_LRX";
		m_Stage.xy.sCmdLY	= "MG_LFY";
		m_Stage.xy.sCmdLX2	= "MG_LFX";
		m_Stage.xy.sCmdLY2	= "MG_LRY";
	}
	// 找原点第二步命令1（速度）
	if(GetPrivateProfileString(szBufRootSection, "Cmd21", "0", szBuf1, 255, path) != 0)
		m_Stage.xy.sCmdFI21.Format( "%s", szBuf1 );
	// 找原点第二步命令2（跳转）
	if(GetPrivateProfileString(szBufRootSection, "Cmd22", "0", szBuf1, 255, path) != 0)
		m_Stage.xy.sCmdFI22.Format( "%s", szBuf1 );
// 	{
// 		strcpy(szBuf2, szBuf1);
// 		if(strchr(szBuf2, ',') != NULL)
// 		{
// 			strcpy(szBuf1, strchr(szBuf2, ',')+1);
// 			*strchr(szBuf2, ',') = '\0';
// 			m_Stage.xy.sCmdFI22.Format( "%ld,,%ld", atol(szBuf2), atol(szBuf1) );
// 		}
// 	}
	// 找原点第三步命令
	if(GetPrivateProfileString(szBufRootSection, "Cmd3", "0", szBuf1, 255, path) != 0)
		m_Stage.xy.sCmdFI3.Format( "%s", szBuf1 );
	// 软件限位命令
	if(GetPrivateProfileString(szBufRootSection, "Cmd41", "0", szBuf1, 255, path) != 0)
		m_Stage.xy.sCmdLimitX.Format( "%s", szBuf1 );
	if(GetPrivateProfileString(szBufRootSection, "Cmd42", "0", szBuf1, 255, path) != 0)
		m_Stage.xy.sCmdLimitY.Format( "%s", szBuf1 );
}


void CStageXYManager::Stage_XY_FindIndex()
{
	if( m_Stage.nHasAutoStage > 0 && m_Stage.xy.bReady )
	{
		CString str;
		str.Format("Grating: X = %d nm, Y = %d nm", m_Stage.xy.nGratingX, m_Stage.xy.nGratingY );
		CDebugMessageDispatcher::Instance().DisplayMsg( str );
		str.Format("Stage reverse: %d", m_Stage.xy.bRev );
		CDebugMessageDispatcher::Instance().DisplayMsg( str );
		str.Format("Stage XLD=%s, XRD=%s, YLD=%s, YRD=%s", m_Stage.xy.sXLD, m_Stage.xy.sXRD, m_Stage.xy.sYLD, m_Stage.xy.sYRD );
		CDebugMessageDispatcher::Instance().DisplayMsg( str );
		str.Format("Stage CMD1 = %s, LX = %s, LY = %s", m_Stage.xy.sCmdFI1, m_Stage.xy.sCmdLX, m_Stage.xy.sCmdLY );
		CDebugMessageDispatcher::Instance().DisplayMsg( str );
		str.Format("Stage CMD21 (SP) = %s", m_Stage.xy.sCmdFI21 );
		CDebugMessageDispatcher::Instance().DisplayMsg( str );
		str.Format("Stage CMD22 (PR) = %s", m_Stage.xy.sCmdFI22 );
		CDebugMessageDispatcher::Instance().DisplayMsg( str );
		str.Format("Stage CMD3 (JG OF FIXY) = %s", m_Stage.xy.sCmdFI3 );
		CDebugMessageDispatcher::Instance().DisplayMsg( str );
		// 启动找Index
		::PostMessage(m_hWnd,WM_USER_KYKYComm_AutoStageXY_FI_Init,0,0);
	}
}


int CStageXYManager::Stage_XY_GetPosition( long& lX, long& lY, BOOL bDisplay)
{
	char szBuf1[64], szBuf2[64];
	m_DMCWin.Command( "TP", szBuf1, sizeof(szBuf1) );

	strcpy(szBuf2, szBuf1);
	if(strchr(szBuf2, ',') != NULL)
	{
		strcpy(szBuf1, strchr(szBuf2, ',')+1);
		*strchr(szBuf2, ',') = '\0';
		lX = atol(szBuf2);

		lY = atol(szBuf1);
/*		if(strchr(szBuf1, ',') != NULL)
		{
			strcpy(szBuf2, strchr(szBuf1, ',')+1);
			*strchr(szBuf1, ',') = '\0';
			if(strchr(szBuf2, ',') != NULL)
			{
				strcpy(szBuf1, strchr(szBuf2, ',')+1);
				*strchr(szBuf2, ',') = '\0';
				lY = atol(szBuf2);
			}
		}*/
	}

	CString str;
	str.Format( "pos X=%ld, Y=%ld", lX, lY );
	CDebugMessageDispatcher::Instance().DisplayMsg( str );

	if( bDisplay )
	{
		CString str;
		str.Format( "pos X=%ld, Y=%ld", lX, lY );
		CDebugMessageDispatcher::Instance().DisplayMsg( str );
	}

	return 0;
}

void CStageXYManager::Stage_XY_Command( CString strCmd, int nValue, BOOL bIsX, BOOL bDir )
{
	// 一个轴的运动
	CString str;
	if( bIsX )
	{
		if( bDir )	// X，且向右移动，例如：在面板上单击向右箭头
		{
			if( m_Stage.xy.bRev )	// XY反向
				str.Format( "SHY;%s ,%s%ld;BGY", strCmd, m_Stage.xy.sYRD, nValue );
			else
				str.Format( "SHX;%s %s%ld;BGX", strCmd, m_Stage.xy.sXRD, nValue );
		}
		else		// X，且向左移动，例如：在面板上单击向左箭头
		{
			if( m_Stage.xy.bRev)	// XY反向
				str.Format( "SHY;%s ,%s%ld;BGY", strCmd, m_Stage.xy.sYLD, nValue );
			else
				str.Format( "SHX;%s %s%ld;BGX", strCmd, m_Stage.xy.sXLD, nValue );
		}
	}
	else
	{
		if( bDir )	// Y，且向下移动，例如：在面板上单击向下箭头
		{
			if( m_Stage.xy.bRev )	// XY反向
				str.Format( "SHX;%s %s%ld;BGX", strCmd, m_Stage.xy.sXRD, nValue );
			else
				str.Format( "SHY;%s ,%s%ld;BGY", strCmd, m_Stage.xy.sYRD, nValue );
		}
		else		// Y，且向上移动，例如：在面板上单击向上箭头
		{
			if( m_Stage.xy.bRev)	// XY反向
				str.Format( "SHX;%s %s%ld;BGX", strCmd, m_Stage.xy.sXLD, nValue );
			else
				str.Format( "SHY;%s ,%s%ld;BGY", strCmd, m_Stage.xy.sYLD, nValue );
		}
	}

	char szBuffer[64];
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	Stage_XY_GetPosition( m_Stage.xy.ptCurPosAb.x, m_Stage.xy.ptCurPosAb.y );
}

void CStageXYManager::Stage_XY_Command2( CString strCmd, int nValueX, int nValueY, BOOL bDirX, BOOL bDirY, BOOL bSerial )
{
	// 两个轴同时运动
	CString strTemp, str;
	if( m_Stage.xy.bRev )	// XY反向
		strTemp.Format( "%s %s%ld,%s%ld", strCmd, bDirY ? m_Stage.xy.sXRD : m_Stage.xy.sXLD, nValueY,
											bDirX ? m_Stage.xy.sYRD : m_Stage.xy.sYLD, nValueX );
	else
		strTemp.Format( "%s %s%ld,%s%ld", strCmd, bDirX ? m_Stage.xy.sXRD : m_Stage.xy.sXLD, nValueX,
												bDirY ? m_Stage.xy.sYRD : m_Stage.xy.sYLD, nValueY );
// 	if( bSerial )
// 		str.Format( "SHXY;%s;BGXY", strTemp );
// 	else
// 		str = strTemp;
	str.Format( "SHXY;%s;BGXY", strTemp );
	CDebugMessageDispatcher::Instance().DisplayMsg( str );

	char szBuffer[64];
	m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	Stage_XY_GetPosition( m_Stage.xy.ptCurPosAb.x, m_Stage.xy.ptCurPosAb.y );
}
