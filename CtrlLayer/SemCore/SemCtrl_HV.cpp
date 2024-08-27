// SemCtrl_HV.cpp: implementation of the CSemCtrl_HV class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_HV.h"
#include "CoadjustParam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 2005-2-2_统调
/*
const WORD	CSemCtrl_HV::m_wHVCtlData[] =
{
//	0,		1,		2,		3,		4,		5,		6,		7,		8,		9,

	0,		1,		2,		3,		4,		5,		6,		7,		8,		9,		// 0
	10,		11,		12,		13,		14,		15,		16,		17,		18,		19,		// 1
	20,		21,		22,		23,		24,		25,		26,		27,		28,		29,		// 2
	30,		31,		32,		33,		34,		35,		36,		37,		38,		39,		// 3
	40,		41,		42,		43,		44,		45,		46,		47,		48,		49,		// 4
	50,		51,		52,		53,		54,		55,		56,		57,		58,		59,		// 5
	60,		61,		62,		63,		64,		65,		66,		67,		68,		69,		// 6
	70,		71,		72,		73,		74,		75,		76,		77,		78,		79,		// 7
	80,		81,		82,		83,		84,		85,		86,		87,		88,		89,		// 8
	90,		91,		92,		93,		94,		95,		96,		97,		98,		99,		// 9
	100,	110,	120,	130,	140,	150,	160,	170,	180,	190,	// 10
	200,	210,	220,	230,	240,	250,	260,	270,	280,	290,	// 11
	300																				// 12
};

const WORD	CSemCtrl_HV::m_wHVAdjustData[] =
{
//	0,		1,		2,		3,		4,		5,		6,		7,		8,		9,

	0x0000,	0x3B00,	0x5300,	0x6600,	0x7600,	0x8400,	0x9000,	0x9C00,	0xA700,	0xB100,		// 0
	0xBA00,	0xC400,	0xCC00,	0xD500,	0xDD00,	0xE400,	0xE800,	0xF700,	0xFA00,	0x0101,		// 1
	0x0801,	0x0E01,	0x1101,	0x1F01,	0x2101,	0x2701,	0x2901,	0x3701,	0x3801,	0x3E01,		// 2
	0x4301,	0x4901,	0x4E01,	0x5301,	0x5801,	0x5D01,	0x6201,	0x6701,	0x6C01,	0x7101,		// 3
	0x7501,	0x7A01,	0x7F01,	0x8301,	0x8801,	0x8C01,	0x9001,	0x9501,	0x9901,	0x9D01,		// 4
	0xA201,	0xA601,	0xAA01,	0xAE01,	0xB201,	0xB601,	0xBA01,	0xBE01,	0xC201,	0xC601,		// 5
	0xC901,	0xCD01,	0xD101,	0xD501,	0xD801,	0xDC01,	0xE001,	0xE301,	0xE701,	0xEB01,		// 6
	0xEE01,	0xF201,	0xF501,	0xF901,	0xFC01,	0xFF01,	0x0302,	0x0602,	0x0A02,	0x0D02,		// 7
	0x1002,	0x1402,	0x1702,	0x1A02,	0x1D02,	0x2102,	0x2402,	0x2702,	0x2A02,	0x2D02,		// 8
	0x3002,	0x3302,	0x3702,	0x3A02,	0x3D02,	0x4002,	0x4302,	0x4602,	0x4902,	0x4C02,		// 9
	0x4F02,	0x6C02,	0x8702,	0xA202,	0xBB02,	0xD402,	0xEB02,	0x0203,	0x1903,	0x2E03,		// 10
	0x4403,	0x5803,	0x6C03,	0x8003,	0x9303,	0xA603,	0xB903,	0xCB03,	0xDD03,	0xEE03,		// 11
	0xFF03																				// 12
};
*/

CSemCtrl_HV::CSemCtrl_HV()
{
	//WORD	nCtrlDataSize = 0;
	
	// 2005-2-2_统调
	WORD nCtrlDataSize = LoadSemHVAdjustParam( HVCoadjustParamFilename, 
		CtrlDataSizeMax_HV, m_wHVCtlData, m_wHVAdjustData );

	m_scfCtrl = DefaultCtrlFigures_HV;
	m_scfCtrl.varMax.vt = VT_UI4;
	m_scfCtrl.varMax.lVal = (long)(nCtrlDataSize-1);

	m_pCL	= NULL;
	m_pOL	= NULL;
}

CSemCtrl_HV::~CSemCtrl_HV()
{
}

BOOL CSemCtrl_HV::Flush()
{
	WORD				wusbCtl;
	USB_SEM_ControlMessage	usb_hostMSG;

	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	wusbCtl = (WORD)m_wHVCtlData[lCtlIndex];
	usb_hostMSG.bType			= (BYTE)1;
	usb_hostMSG.bCtlMSGLength	= (BYTE)4;
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_SPR_Legacy_HV_Power;
	usb_hostMSG.bData[1]		= (BYTE)USB_TM_SPR_Write_16;
	usb_hostMSG.bData[2]		= (BYTE)( wusbCtl & 0x00FF );
	usb_hostMSG.bData[3]		= (BYTE)( wusbCtl >> 8 );

	BOOL blRet = FALSE;
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

	return blRet;
}

const CString& CSemCtrl_HV::FormatText()
{
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	WORD				wusbCtl;
	wusbCtl = (WORD)m_wHVCtlData[lCtlIndex];

	// Format Display string
	if( m_wHVCtlData[ lCtlIndex ] < 100 )
	{
		m_strText.Format( "%2.1f KV [ %d ]", m_wHVCtlData[ lCtlIndex ]/10.0, wusbCtl );
	}
	else
	{
		m_strText.Format( "%d KV [ %d ]", m_wHVCtlData[ lCtlIndex ]/10, wusbCtl );
	}

	return m_strText;
}

const CString& CSemCtrl_HV::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;

	// Format Display string
	if( m_wHVCtlData[ lCtlIndex ] < 100 )
	{
		strText.Format( "%2.1f KV", m_wHVCtlData[ lCtlIndex ]/10.0 );
	}
	else
	{
		strText.Format( "%d KV", m_wHVCtlData[ lCtlIndex ]/10 );
	}	

	return strText;
}

BOOL CSemCtrl_HV::Bind( CSemCtrl_HVAccelerate*	pHVAccelerate )
{
	if ( pHVAccelerate != NULL )
	{
		m_pHVAccelerate	= pHVAccelerate;
		m_bReady				= TRUE;
	}

	return m_bReady;
}

BOOL CSemCtrl_HV::Bind( CSemCtrl_CondenserLens* pCL )
{
	if ( pCL != NULL )
	{
		m_pCL		= pCL;
		m_bReady	= TRUE;
	}
	return m_bReady;
}

BOOL CSemCtrl_HV::Bind( CSemCtrl_ObjectiveLens* pOL )
{
	if ( pOL != NULL )
	{
		m_pOL		= pOL;
		m_bReady	= TRUE;
	}
	return m_bReady;
}

VARIANT CSemCtrl_HV::SetPos( VARIANT varPos )
{
	CSemBasisCtrl::SetPos( varPos );

	if ( m_pHVAccelerate != NULL )
	{
		VARIANT varAcceleratePos;

		VARIANT varPos = GetPos();
		long lCtlIndex = varPos.lVal;

		varAcceleratePos.dblVal = m_wHVCtlData[ lCtlIndex ]/10.0;

		m_pHVAccelerate->SetPos( varAcceleratePos );

		// 新系统中高压统调要在软件上完成
		// 高压发生改变时，聚光镜电流、物镜电流和扫描线圈电流都要发生变化
		// 原来送到Z80的高压统调值要作为系数送至上述3个值
		// 30KV时该系数为1
		m_pCL->SetParams( SET_PARAMS_Rate, m_wHVCtlData[ lCtlIndex ] );
		m_pOL->SetParams( SET_PARAMS_Rate, m_wHVCtlData[ lCtlIndex ] );
		m_pOL->SetParams( 5, varAcceleratePos.dblVal );
	}

	return m_scfCtrl.varPos;
}

