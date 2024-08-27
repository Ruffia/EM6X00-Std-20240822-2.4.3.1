// SemCtrl_ScanRotate.cpp: implementation of the CSemCtrl_ScanRotate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_ScanRotate.h"
#include <math.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSemCtrl_ScanRotate::CSemCtrl_ScanRotate()
{
	m_scfCtrl		= DefaultCtrlFigures_ERotate;
	m_bON			= FALSE;
	m_dAngleInit	= 0.0;
	m_dAngleComp	= 0.0;	// 物镜统调扫描旋转的补偿角度
}

CSemCtrl_ScanRotate::~CSemCtrl_ScanRotate()
{

}

BOOL CSemCtrl_ScanRotate::Bind( CSemCtrl_ObjectiveLens*	pOL )
{
	//CSemCtrl_WD * pWDSafety = DYNAMIC_DOWNCAST(CSemCtrl_WD, pWD);

	if ( pOL != NULL )
	{
		m_pOL		= pOL;
	}

	return TRUE;
}

BOOL CSemCtrl_ScanRotate::Flush()
{
	if ( !CSemBasisCtrl::Flush() )
		return FALSE;

	WORD				wSin;
	WORD				wCos;
	double				dAngle;
	const double		dfAngleToRadian = (double)PI/180.;

	USB_SEM_ControlMessage	usb_hostMSG;

	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;
//	wAngle = (WORD)(m_scfCtrl.varMax.lVal -lCtlIndex +1);	// 0 - 359 映射到 360 - 1

	lCtlIndex = lCtlIndex +(int)(m_dAngleInit *10);	// 以校准角度作为0度
	lCtlIndex = lCtlIndex +(int)(m_dAngleComp *10);
	if( lCtlIndex > m_scfCtrl.varMax.lVal +1 )
		lCtlIndex -= (m_scfCtrl.varMax.lVal +1);
	if( lCtlIndex < 0 )
		lCtlIndex += (m_scfCtrl.varMax.lVal +1);
	dAngle = ( m_scfCtrl.varMax.lVal -lCtlIndex +1 ) /10.0;	// 0 - 359 映射到 360 - 1
	BOOL	blRet	= FALSE;
	BOOL	bOn		= m_bON;
//	if( lCtlIndex == 0 )
	if( dAngle < 0.1 && dAngle > -0.1 )	//wAngle == 0 )		// 已实际输出角度为准进行判断
	{
		// Rotation OFF
		m_bON	= FALSE;
//		usb_hostMSG.bMessageID		= (BYTE)USB_RotationOFF;
		usb_hostMSG.bType			= (BYTE)0;
		usb_hostMSG.bCtlMSGLength	= (BYTE)4;
		usb_hostMSG.bData[0]		= (BYTE)USB_RotationOFF;	// opcode
		usb_hostMSG.bData[1]		= (BYTE)FALSE;				// no operand
		usb_hostMSG.bData[2]		= (BYTE)0;
		usb_hostMSG.bData[3]		= (BYTE)0;

		blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
		Sleep(10);
		blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
		Sleep(10);
		blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
	}
	else
	{
		m_bON	= TRUE;

		wSin = (WORD)( sin( dAngle * dfAngleToRadian ) * 0x7FFF );
		wCos = (WORD)( cos( dAngle * dfAngleToRadian ) * 0x7FFF );

		// set cos
		usb_hostMSG.bType			= (BYTE)0;
		usb_hostMSG.bCtlMSGLength	= (BYTE)4;
		usb_hostMSG.bData[0]		= (BYTE)USB_RotationSetCos;
		usb_hostMSG.bData[1]		= (BYTE)TRUE;
		usb_hostMSG.bData[2]		= (BYTE)( wCos & 0x00FF );
		usb_hostMSG.bData[3]		= (BYTE)( wCos >> 8 ) ;
		blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

		// set sin
		usb_hostMSG.bType			= (BYTE)0;
		usb_hostMSG.bCtlMSGLength	= (BYTE)4;
		usb_hostMSG.bData[0]		= (BYTE)USB_RotationSetSin;
		usb_hostMSG.bData[1]		= (BYTE)TRUE;
		usb_hostMSG.bData[2]		= (BYTE)( wSin & 0x00FF );
		usb_hostMSG.bData[3]		= (BYTE)( wSin >> 8 ) ;
		blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

		// update rotation
		usb_hostMSG.bType			= (BYTE)0;
		usb_hostMSG.bCtlMSGLength	= (BYTE)4;
		usb_hostMSG.bData[0]		= (BYTE)USB_UpdateRotation;
		usb_hostMSG.bData[1]		= (BYTE)FALSE;
		usb_hostMSG.bData[2]		= (BYTE)0;
		usb_hostMSG.bData[3]		= (BYTE)0;
		blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

		if( m_bON != bOn )
		{
			// scan rotation ON
			usb_hostMSG.bType			= (BYTE)0;
			usb_hostMSG.bCtlMSGLength	= (BYTE)4;
			usb_hostMSG.bData[0]		= (BYTE)USB_RotationON;
			usb_hostMSG.bData[1]		= (BYTE)FALSE;
			usb_hostMSG.bData[2]		= (BYTE)0;
			usb_hostMSG.bData[3]		= (BYTE)0;
			blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
			Sleep(100);
			blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
			Sleep(100);
			blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
		}
	}

	return blRet;
}

const CString& CSemCtrl_ScanRotate::FormatText()
{
	// GetPos
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	// Format Display string
	m_strText.Format( "%.1f +%.1f +%.1f deg", lCtlIndex /10.0, m_dAngleInit, m_dAngleComp );

	return m_strText;
}

const CString& CSemCtrl_ScanRotate::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	strText.Format( "%.1f +%.1f +%.1f deg", lCtlIndex /10.0, m_dAngleInit, m_dAngleComp );

	return strText;
}

void CSemCtrl_ScanRotate::SetParams( int nType, double dParam )
{
	switch( nType )
	{
	case	1:
		{
			m_dAngleInit = dParam;
			Flush();
		}
		break;
	case	2:
		{
			m_dAngleComp = dParam;
			Flush();
		}
		break;
	case	11:
		{
			VARIANT varPos;
			varPos.lVal = m_dAngleInit *10;
			SetPos( varPos );
			Flush();
		}
		break;
	}
}

void CSemCtrl_ScanRotate::GetParams( int nType, double& dParam )
{
	switch( nType )
	{
	case	1:
		{
			// 获取初始旋转角度
			dParam = m_dAngleInit;
		}
		break;
	case	2:
		{
			// 获取实际旋转角度
			VARIANT varPos = GetPos();
			long lCtlIndex = varPos.lVal;

			lCtlIndex = lCtlIndex +(int)(m_dAngleInit *10);	// 以校准角度作为0度
			lCtlIndex = lCtlIndex +(int)(m_dAngleComp *10);
			if( lCtlIndex > m_scfCtrl.varMax.lVal +1 )
				lCtlIndex -= (m_scfCtrl.varMax.lVal +1);
			if( lCtlIndex < 0 )
				lCtlIndex += (m_scfCtrl.varMax.lVal +1);
			dParam = lCtlIndex /10.0;
		}
		break;
	case	3:
		{
			dParam = m_dAngleComp;
		}
		break;
	}
}