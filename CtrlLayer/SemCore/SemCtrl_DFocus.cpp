// SemCtrl_DFocus.cpp: implementation of the CSemCtrl_DFocus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_DFocus.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const long DFocus_Range		= 16384;
//const long DFocus_Range_10	= DFocus_Range /10;
const long DFocus_Range_10	= DFocus_Range /16;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_DFocus::CSemCtrl_DFocus()
{
	m_scfCtrl			= DefaultCtrlFigures_DFocus;
	m_lSwitch			= 3;	// ����Focus Fine

	m_dScanHeight		= 1024.0;
	m_nValley			= (int)( 10000.0 / (8 *125) /4 +0.5) ;

	m_bPolarity_Atten	= 0x00;
	m_wusbCtl_Step		= 0x00;

	m_bDFforFF			= FALSE;
	m_nFFTotal			= 0;	// 0x8000

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
	if ( dwLength != 0 )
	{
		strcat(exeFullPath, m_strINIfileName);
		char szBuf[255];
		CString str = "";
		if(GetPrivateProfileString( "Options", "FFsteps", "11", szBuf, 255, exeFullPath) != 0)
			m_nDFforFFBase = atoi( szBuf );
	}
}

CSemCtrl_DFocus::~CSemCtrl_DFocus()
{
}

BOOL CSemCtrl_DFocus::Bind( CSemCtrl_ObjectiveLens*	pOL )
{
	if ( pOL != NULL )
	{
		m_pOL		= pOL;
	}

	return TRUE;
}

BOOL CSemCtrl_DFocus::Flush()
{
	if ( !CSemBasisCtrl::Flush() )
		return FALSE;
	
	WORD				wusbCtl;
	USB_SEM_ControlMessage	usb_hostMSG;

	/////////////////////////////////////////////////////////////////
	// (1)	Start point ����Y����������ʼֵ��
	//		����������Ʒ̨��б��������ﾵ���������������ƫ��ֵ
	// (2)	�ʼ����0x36 End Boundary (EB)Ϊһ����ֵ����0xFC00����ʼ��ʹ������positive
	// (3)	�������ǵ���Ʒ̨����������Ϊ����б�����ԣ������ϵĹ�����Ӧ����������0x34��ʼֵSP��
	//		��ʼֵ�ķ�Χ�޶���0x2000 - 0x8000
	// (4)	Ȼ���ȼ����0x32������ = (0x8000 - SP) / (Y/2)
	// (5)	Ȼ����ݵ�ǰ��pixrate����Valley�ĸ�����Ҫ����Valley��ʱ�䲻С��10us
	// (6)	���õ���ʼֵSP����������ֵ = (SP����ֵ - ST * Valley�ĸ���)
	/////////////////////////////////////////////////////////////////

	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	BYTE bPolarity_Atten = 0xFF;
	BOOL blRet = FALSE;

	///////////////////////////////////////////////////////
	// 2013.08.22 ֻʵ�鸴��ΪFocus Fine�Ĺ���
	///////////////////////////////////////////////////////
	if( m_bDFforFF )
	{
		// ����ΪFocus Fine
		bPolarity_Atten		= 0x05;		// 0b101: Bit2=1(pos ramp), Bits 1,0 = 01 (sets 10% range)
		m_bPolarity_Atten	= bPolarity_Atten;
		// ����������Χ������
		usb_hostMSG.bType			= (BYTE)1;
		usb_hostMSG.bCtlMSGLength	= (BYTE)4;
		usb_hostMSG.bData[0]		= (BYTE)USB_CID_LNS_ObjectiveLens_DynFocusPolarityAtten;
		usb_hostMSG.bData[1]		= (BYTE)USB_TM_LNS_Write_08;
		usb_hostMSG.bData[2]		= (BYTE)0;
		usb_hostMSG.bData[3]		= bPolarity_Atten;
		blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
		// ������
		m_wusbCtl_Step				= 0;
		usb_hostMSG.bType			= (BYTE)1;
		usb_hostMSG.bCtlMSGLength	= (BYTE)4;
		usb_hostMSG.bData[0]		= (BYTE)USB_CID_LNS_ObjectiveLens_DynFocusSteps;
		usb_hostMSG.bData[1]		= (BYTE)USB_TM_LNS_Write_16;
		usb_hostMSG.bData[2]		= (BYTE)0;
		usb_hostMSG.bData[3]		= (BYTE)0;
		blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
		return blRet;
	}
	else
	{
		if( m_lSwitch == 0 )
		{
			bPolarity_Atten = 0x00;
			// �ر�DF
			m_bPolarity_Atten = bPolarity_Atten;
			// ����������Χ������
			usb_hostMSG.bType			= (BYTE)1;
			usb_hostMSG.bCtlMSGLength	= (BYTE)4;
			usb_hostMSG.bData[0]		= (BYTE)USB_CID_LNS_ObjectiveLens_DynFocusPolarityAtten;
			usb_hostMSG.bData[1]		= (BYTE)USB_TM_LNS_Write_08;
			usb_hostMSG.bData[2]		= (BYTE)0;
			usb_hostMSG.bData[3]		= bPolarity_Atten;
			blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
			// ������
			m_wusbCtl_Step				= 0;
			usb_hostMSG.bType			= (BYTE)1;
			usb_hostMSG.bCtlMSGLength	= (BYTE)4;
			usb_hostMSG.bData[0]		= (BYTE)USB_CID_LNS_ObjectiveLens_DynFocusSteps;
			usb_hostMSG.bData[1]		= (BYTE)USB_TM_LNS_Write_16;
			usb_hostMSG.bData[2]		= (BYTE)0;
			usb_hostMSG.bData[3]		= (BYTE)0;
			blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
			return blRet;
		}
/*		else if( m_lSwitch == 1 )
		{
			// 10% range��DF
			// start point
			wusbCtl = (WORD)(m_scfCtrl.varMax.lVal -lCtlIndex +1);
			usb_hostMSG.bType			= (BYTE)1;
			usb_hostMSG.bCtlMSGLength	= (BYTE)4;
			usb_hostMSG.bData[0]		= (BYTE)USB_CID_LNS_ObjectiveLens_DnyFocusStartPoint;
			usb_hostMSG.bData[1]		= (BYTE)USB_TM_LNS_Write_16;
			usb_hostMSG.bData[2]		= (BYTE)( wusbCtl & 0x00FF );
			usb_hostMSG.bData[3]		= (BYTE)( wusbCtl >> 8 );
			if ( m_pMPComm != NULL )
				blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

			// ������
			m_wusbCtl_Step = (m_scfCtrl.varMax.lVal -lCtlIndex +1) / (m_dScanHeight / 2);
			usb_hostMSG.bType			= (BYTE)1;
			usb_hostMSG.bCtlMSGLength	= (BYTE)4;
			usb_hostMSG.bData[0]		= (BYTE)USB_CID_LNS_ObjectiveLens_DynFocusSteps;
			usb_hostMSG.bData[1]		= (BYTE)USB_TM_LNS_Write_16;
			usb_hostMSG.bData[2]		= (BYTE)( m_wusbCtl_Step & 0x00FF );
			usb_hostMSG.bData[3]		= (BYTE)( m_wusbCtl_Step >> 8 );
			if ( m_pMPComm != NULL )
				blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
			return blRet;
		}
*/		else
		{
			// DF
			// ������ step size (ST)
//			m_wusbCtl_Step = (WORD)((m_scfCtrl.varMax.lVal -lCtlIndex +1) / (m_dScanHeight / 2));
			m_wusbCtl_Step = (WORD)( (0x8000 - (lCtlIndex +0x2000)) / (m_dScanHeight / 2));
			usb_hostMSG.bType			= (BYTE)1;
			usb_hostMSG.bCtlMSGLength	= (BYTE)4;
			usb_hostMSG.bData[0]		= (BYTE)USB_CID_LNS_ObjectiveLens_DynFocusSteps;
			usb_hostMSG.bData[1]		= (BYTE)USB_TM_LNS_Write_16;
			usb_hostMSG.bData[2]		= (BYTE)( m_wusbCtl_Step & 0x00FF );
			usb_hostMSG.bData[3]		= (BYTE)( m_wusbCtl_Step >> 8 );
			blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

			// ��ʼ�� start point����ֵ
			// ʵ�ʷ�ΧֵΪ0 - 0x6000����������ֵ��ΧΪ0x2000 - 0x8000
			// SP����ֵ = SP��������ֵ - ST * Valley����
//			wusbCtl = (WORD)(m_scfCtrl.varMax.lVal -lCtlIndex +1);
			wusbCtl = (WORD)( (lCtlIndex + 0x2000) - m_wusbCtl_Step * m_nValley );
			usb_hostMSG.bType			= (BYTE)1;
			usb_hostMSG.bCtlMSGLength	= (BYTE)4;
			usb_hostMSG.bData[0]		= (BYTE)USB_CID_LNS_ObjectiveLens_DnyFocusStartPoint;
			usb_hostMSG.bData[1]		= (BYTE)USB_TM_LNS_Write_16;
			usb_hostMSG.bData[2]		= (BYTE)( wusbCtl & 0x00FF );
			usb_hostMSG.bData[3]		= (BYTE)( wusbCtl >> 8 );
			blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

			return blRet;
		}
	}
	return blRet;
	///////////////////////////////////////////////////////
/*
	if( lCtlIndex == m_scfCtrl.varMin.lVal || m_lSwitch == 0 )
		bPolarity_Atten	= 0x00;
	if( bPolarity_Atten == 0x00 )
	{
		// �ر�DF
		m_bPolarity_Atten = bPolarity_Atten;
		// ����������Χ������
		usb_hostMSG.bType			= (BYTE)1;
		usb_hostMSG.bCtlMSGLength	= (BYTE)4;
		usb_hostMSG.bData[0]		= (BYTE)USB_CID_LNS_ObjectiveLens_DynFocusPolarityAtten;
		usb_hostMSG.bData[1]		= (BYTE)USB_TM_LNS_Write_08;
		usb_hostMSG.bData[2]		= (BYTE)0;
		usb_hostMSG.bData[3]		= bPolarity_Atten;
		
		BOOL blRet = FALSE;
		if ( m_pMPComm != NULL )
		{
			blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
		}
		
		return blRet;
	}

	// ���㲽����
	double dStep = (lCtlIndex -m_scfCtrl.varMin.lVal) / m_dScanHeight / 2;
	// �жϲ�������ѡ��Χ
	if( dStep < (m_scfCtrl.varMax.lVal -m_scfCtrl.varMin.lVal+1)/16 )
	{
		// ȡ10%��Χ����������Ҫ������������
		bPolarity_Atten	= 0x01;			// CID 0x28, bit0 = 1, set 10% range
		dStep *= 16;
	}
	else
	{
		// ȡ100%��Χ������������
		bPolarity_Atten	= 0x02;			// CID 0x28, bit1 = 1, set full range
	}
	if( dStep > m_scfCtrl.varMax.lVal -m_scfCtrl.varMin.lVal )
		wusbCtl = (WORD)(m_scfCtrl.varMax.lVal -m_scfCtrl.varMin.lVal);
	else
		wusbCtl = (WORD)dStep;
	m_wusbCtl_Step = wusbCtl;

	// ���ü���
	if( lCtlIndex > m_scfCtrl.varMin.lVal )
		bPolarity_Atten	|= 0x04;		// CID 0x28, bit2 = 1, positive; otherwise, negtive
	m_bPolarity_Atten = bPolarity_Atten;
	
	BOOL blRet = FALSE;
	// ������
	usb_hostMSG.bType			= (BYTE)1;
	usb_hostMSG.bCtlMSGLength	= (BYTE)4;
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_LNS_ObjectiveLens_DynFocusSteps;
	usb_hostMSG.bData[1]		= (BYTE)USB_TM_LNS_Write_16;
	usb_hostMSG.bData[2]		= (BYTE)( wusbCtl & 0x00FF );
	usb_hostMSG.bData[3]		= (BYTE)( wusbCtl >> 8 );
	
	if ( m_pMPComm != NULL )
	{
		blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
	}

	// ����������Χ������
	usb_hostMSG.bType			= (BYTE)1;
	usb_hostMSG.bCtlMSGLength	= (BYTE)4;
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_LNS_ObjectiveLens_DynFocusPolarityAtten;
	usb_hostMSG.bData[1]		= (BYTE)USB_TM_LNS_Write_08;
	usb_hostMSG.bData[2]		= (BYTE)0;
	usb_hostMSG.bData[3]		= bPolarity_Atten;
	
	if ( m_pMPComm != NULL )
	{
		blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
	}

	return blRet;*/
}

void CSemCtrl_DFocus::SetSwitch( VARIANT varSwitch )
{
	// ������DFʱ��ֻ��close, 10%, full����״̬
	m_lSwitch++;
	if( m_lSwitch == 3 )
		m_lSwitch = 0;

	BOOL blRet = FALSE;
	if( m_lSwitch == 0 )
		m_bPolarity_Atten	= 0x00;		// close
	else if( m_lSwitch == 1 )
		m_bPolarity_Atten	= 0x05;		// 0b101: Bit2=1(pos ramp), Bits 1,0 = 01 (sets 10% range)
	else
		m_bPolarity_Atten	= 0x06;		// 0b110: Bit2=1(pos ramp), Bits 1,0 = 10 (sets full range)
	if( m_lSwitch > 0 )
	{
		// ����������Χ������
		USB_SEM_ControlMessage	usb_hostMSG;
		usb_hostMSG.bType			= (BYTE)1;
		usb_hostMSG.bCtlMSGLength	= (BYTE)4;
		usb_hostMSG.bData[0]		= (BYTE)USB_CID_LNS_ObjectiveLens_DynFocusPolarityAtten;
		usb_hostMSG.bData[1]		= (BYTE)USB_TM_LNS_Write_08;
		usb_hostMSG.bData[2]		= (BYTE)0;
		usb_hostMSG.bData[3]		= m_bPolarity_Atten;
		blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
	}
};

const CString& CSemCtrl_DFocus::FormatText()
{
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	// Format Display string
//	m_strText.Format( "%03.1f", 
//		(double)(lCtlIndex - m_scfCtrl.varMin.lVal) * 100.0 / (m_scfCtrl.varMax.lVal - m_scfCtrl.varMin.lVal) );

	//Format Display string
	m_strText.Format( "%03.3f[%X]  PA:[%x%x%x] ST:[0x%04X] sp:[0x%04X]", 
		(double)(lCtlIndex - m_scfCtrl.varMin.lVal) * 100.0 / (m_scfCtrl.varMax.lVal - m_scfCtrl.varMin.lVal),
		lCtlIndex + 0x2000,		// start point (SP) �û���������ֵ
		(m_bPolarity_Atten & 0x4 ) >> 2,
		(m_bPolarity_Atten & 0x2 ) >> 1,
		(m_bPolarity_Atten & 0x1 ),
		(WORD)((lCtlIndex + 0x2000) - m_wusbCtl_Step * m_nValley),
		(WORD)( (0x8000 - (lCtlIndex +0x2000)) / (m_dScanHeight / 2) ) );

	return m_strText;
}

const CString& CSemCtrl_DFocus::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	strText.Format( "%03.1f", 
		(double)(lCtlIndex - m_scfCtrl.varMin.lVal) * 100.0 / (m_scfCtrl.varMax.lVal - m_scfCtrl.varMin.lVal) );

	return strText;
}

const CString& CSemCtrl_DFocus::FormatSwitchText()
{
	// Format Display string
/*	if ( m_lSwitch != 0 )
		m_strSwitchText = m_pLanguageMan->GetResString( IDS_STATE_OPEN );
	else
		m_strSwitchText = m_pLanguageMan->GetResString( IDS_STATE_CLOSE );
*/
	CString str;
	if( m_lSwitch == 0 )
		m_strSwitchText = "Close";
	else if( m_lSwitch == 1 )
		m_strSwitchText = "10%";
	else if( m_lSwitch == 2 )
		m_strSwitchText = "Full";
	else
		m_strSwitchText = "FF";

	return m_strSwitchText;
}

const CString& CSemCtrl_DFocus::FormatSwitchText( VARIANT varSwitch )
{
	static CString strSwitchText;

	// Format Display string
	if ( varSwitch.lVal != 0 )
		strSwitchText = m_pLanguageMan->GetResString( IDS_STATE_OPEN );
	else
		strSwitchText = m_pLanguageMan->GetResString( IDS_STATE_CLOSE );

	return strSwitchText;
}

/*
const int SET_PARAMS_DFocus_Start		= 11;	// ���ö�̬�۽���ʼֵ
const int SET_PARAMS_DFocus_End			= 12;	// ���ö�̬�۽��յ�ֵ
#define	USB_CID_LNS_ObjectiveLens_DnyFocusStartPoint 0x34	// 16 bit	-			Objective Lens dynamic focus level at start of line, bit count (14 or 16) same as U51 DAC.
#define	USB_CID_LNS_ObjectiveLens_DnyFocusEndBound	0x36	// 16 bit	-			Objective Lens dynamic focus level boundary (either pos or neg direction), bit count (14 or 16) same as U51 DAC.
*/
void CSemCtrl_DFocus::SetParams( int nType, double dParam )// Type:���ͣ�Param:Ҫ���õĲ���ֵ
{
	USB_SEM_ControlMessage	usb_hostMSG;
	WORD	wusbCtl	= (WORD)dParam;
	BOOL	blRet	= FALSE;
	switch( nType )
	{
	case	SET_PARAMS_DFocus_Start:
		usb_hostMSG.bType			= (BYTE)1;
		usb_hostMSG.bCtlMSGLength	= (BYTE)4;
		usb_hostMSG.bData[0]		= (BYTE)USB_CID_LNS_ObjectiveLens_DnyFocusStartPoint;
		usb_hostMSG.bData[1]		= (BYTE)USB_TM_LNS_Write_16;
		usb_hostMSG.bData[2]		= (BYTE)( wusbCtl & 0x00FF );
		usb_hostMSG.bData[3]		= (BYTE)( wusbCtl >> 8 );

		blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
		break;

	case	SET_PARAMS_DFocus_End:
		usb_hostMSG.bType			= (BYTE)1;
		usb_hostMSG.bCtlMSGLength	= (BYTE)4;
		usb_hostMSG.bData[0]		= (BYTE)USB_CID_LNS_ObjectiveLens_DnyFocusEndBound;
		usb_hostMSG.bData[1]		= (BYTE)USB_TM_LNS_Write_16;
		usb_hostMSG.bData[2]		= (BYTE)( wusbCtl & 0x00FF );
		usb_hostMSG.bData[3]		= (BYTE)( wusbCtl >> 8 );

		blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
		break;

	case	13:
		if( m_dScanHeight != dParam )
		{
			// ���ɨ������߶ȱ���Ҫ���¼��㲽��Ȳ���
			m_dScanHeight = dParam;
			Flush();
		}
		break;

	case	14:	// ����DF���ܵ���̬
		m_lSwitch = (long)dParam;
		break;

	case	15:	// �Ƿ���ΪFF
		m_bDFforFF = (BOOL)dParam;
		break;

	case	16:
		// Ϊ����SP����ֵ����Ҫ֪����ǰ��pixrate���Ӷ������Valley�ĸ���
		// Ҫ����Valley��ʱ�䲻С��10us
		// ͨ��SetParams���ݵ���������pixrate = ��ͣ��ʱ�� /4
		m_nValley = (int)( 10000.0 / (dParam *125) /4 +0.5 );
		Flush();
		break;

	case	100:	// OprAdvance
		m_nOpr	= (int)dParam;
		break;

	default:
		break;
	}
}
