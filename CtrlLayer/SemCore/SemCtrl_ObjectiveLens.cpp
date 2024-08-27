// SemCtrl_ObjectiveLens.cpp: implementation of the CSemCtrl_ObjectiveLens class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemCtrl_WD.h"
#include "SemCtrl_ObjectiveLens.h"

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

CSemCtrl_ObjectiveLens::CSemCtrl_ObjectiveLens()
{	
	m_scfCtrl = DefaultCtrlFigures_ObjectiveLens;

	m_bReady	= FALSE;
	m_pWD		= NULL;
	m_pRotate	= NULL;

	m_dRateC	= 0.0;
	m_dRateM	= 1.0;
	m_bDebug	= TRUE;
	m_bReverse	= FALSE;

	/////////////////////////////////////////////////////////////
	// 15.05.14 韩勇说在拟合新统调公式时，3200的拟合效果偏差较大
	// 所以有可能6200还使用之前的统调公式
	// 故将m_nType做调整，原来为0代表6200,1代表6900
	// 现改为：	0代表6200且使用原统调公式
	//			1代表6200且使用新统调公式(用原3200统调表，用新公式拟合出固定系数)
	//			11代表6200且使用新标高样品动态拟合新公式的系数
	//			12代表6900(使用新标高样品动态拟合新公式的系数)
	// 判断电镜型号时除原有注册表项目，暂时使用ini中的[Debug]/Start来区分6200的新老统调公式类型
	// 该ini项目原来用于物镜显示值的6X00-->3X00的映射，早已不用
	/////////////////////////////////////////////////////////////
/*	double dValue = 512.0;
	if( CSemBasisCtrl::ReadINI( INI_Debug, INIIndex_Start, &dValue ) )
		m_bDebug = (BOOL)dValue;
	if( !m_bDebug )	// 非调试模式再读取映射比例系数
	{
		dValue = 512.0;
		if( CSemBasisCtrl::ReadINI( INI_Ratio, INIIndex_OL, &dValue ) )
			m_dRateM = 512.0 / dValue;
	}*/

	// DF for FF
	double dValue = 13;
	if( CSemBasisCtrl::ReadINI( INI_DFforFF, INIIndex_DF, &dValue ) )
		m_nDFforFFBase = (int)dValue;
	m_bDFforFF	= FALSE;
	m_lCtrlDF	= 0;

	// Coadjust
	// 物镜统调工作距离
	double*		pdouble	= new double[6];
	if( CSemBasisCtrl::ReadINI( INI_Coadjust, INIIndex_OL, pdouble ) )
	{
/*		int nCtrl1 = (int)pdouble[0];	// 12mm标高时看清楚像，此时的OL控制值对应K=750
		int nCtrl2 = (int)pdouble[1];	// 24mm标高时看清楚像，此时的OL控制值对应K=590
		// OLctrl = nCtrl1,	K = 750;
		// OLctrl = nCtrl2,	K = 590;
		m_dCo_Ka	= (750.0 -590.0) /(nCtrl1 - nCtrl2);
		m_dCo_Kb	= 750.0 - nCtrl1 * m_dCo_Ka;
*/		m_dCo_Ka	= pdouble[0];
		m_dCo_Kb	= pdouble[1];
	}
	else
	{
		m_dCo_Ka	= 1.0;
		m_dCo_Kb	= 0.0;
	}
	delete [] pdouble;

	UpdateCoadjust_HV_WD();
	UpdateCoadjust_OL_Rotate();

// 	for( int i=0; i<100000; i++ )
// 		m_lCtrlData[i] = 0;
// 	m_lCtrlCount = 0;
}

void CSemCtrl_ObjectiveLens::UpdateCoadjust_HV_WD()
{
	// 高压统调工作距离
	double*		pdouble	= new double[6];
	int i;
	for( i=0; i<6; i++ )
		pdouble[i] = -1.1;
	if( CSemBasisCtrl::ReadINI( INI_Coadjust, INIIndex_HV, pdouble ) )
	{
		// 读到了6个值，但还要看是否是正常值
		BOOL bNotValid = FALSE;
		for( i=0; i<6; i++ )
		{
			m_dCo_HV[i] = pdouble[i];
			if( m_dCo_HV[i] < 0.4 )
			{
				// 有非正常值，则即使读到的数目正确，也还是要进入高压统调模式
				bNotValid = TRUE;
				break;
			}
		}
		m_bCoadjustHVMode = bNotValid;
	}
	else
	{
		BOOL bValid = FALSE;
		for( i=0; i<6; i++ )
		{
			if( pdouble[i] > -1.0 )
			{
				bValid = TRUE;	// 有FactorsHV条目
				break;
			}
		}
		if( bValid )
		{
			// 有FactorsHV条目，但系数总数不对，则要进入高压统调模式
			m_bCoadjustHVMode = TRUE;
		}
		else
		{
			// 没有FactorsHV条目，则使用默认系数值，不进入高压统调模式
			for( i=0; i<6; i++ )
				m_dCo_HV[i] = 0.5;
			m_bCoadjustHVMode = FALSE;
		}
	}
	delete [] pdouble;
}

void CSemCtrl_ObjectiveLens::UpdateCoadjust_HV_WD_RadioC( double dParam )
{
	if( m_bCoadjustHVMode )
		m_dRateC = 1.0;	// 在进行高压统调，系数始终为1.0
	else
	{
		double dWD = 0.0;
		m_pWD->GetParams( 1, dWD );
		if( dWD < 7.5 )
			m_dRateC = pow( dParam /300.0, m_dCo_HV[0] );
		else if( dWD < 12.5 )
			m_dRateC = pow( dParam /300.0, m_dCo_HV[1] );
		else if( dWD < 17.5 )
			m_dRateC = pow( dParam /300.0, m_dCo_HV[2] );
		else if( dWD < 22.5 )
			m_dRateC = pow( dParam /300.0, m_dCo_HV[3] );
		else if( dWD < 27.5 )
			m_dRateC = pow( dParam /300.0, m_dCo_HV[4] );
		else
			m_dRateC = pow( dParam /300.0, m_dCo_HV[5] );
	}
}

void CSemCtrl_ObjectiveLens::UpdateCoadjust_OL_Rotate()
{
	m_bCoadjustRotateMode = FALSE;
	// 物镜统调扫描旋转
	double*		pdouble	= new double[2];
	int i;
	for( i=0; i<2; i++ )
		pdouble[i] = -360.1;
	if( CSemBasisCtrl::ReadINI( INI_Coadjust, INIIndex_Rotate, pdouble ) )
	{
		m_dCo_Rotate[0]	= pdouble[0];
		m_dCo_Rotate[1]	= pdouble[1];
		m_bCoadjustRotateMode = FALSE;
	}
	else
	{
		BOOL bValid = FALSE;
		for( i=0; i<2; i++ )
		{
			if( pdouble[i] > -360 )
			{
				bValid = TRUE;	// 有FactorsRotate条目
				break;
			}
		}
		if( bValid )
		{
			// 有FactorsHV条目，但系数总数不对，则要进入扫描旋转统调模式
			m_bCoadjustRotateMode = TRUE;
		}
		else
		{
			// 没有FactorsHV条目，则使用默认系数值，不进入扫描旋转统调模式
			m_dCo_Rotate[0] = 0.0;
			m_dCo_Rotate[1] = 0.0;
			m_bCoadjustRotateMode = FALSE;
		}
	}
	delete [] pdouble;
}

CSemCtrl_ObjectiveLens::~CSemCtrl_ObjectiveLens()
{
/*	TCHAR exeFullPath[255];
	::GetModuleFileName( NULL, exeFullPath, 255 );
	char *ptemp = strrchr( exeFullPath, '\\');
	ptemp[1] = 0;
	
	CString str, strTemp;
	CTime	timeCurrent = CTime::GetCurrentTime();
	strTemp = timeCurrent.Format("%Y%m%d%H%M");
	str.Format( "%szOL%s.txt", exeFullPath, strTemp );
	HANDLE h = CreateFile(str, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
	if( h != NULL )
	{
		unsigned long cbRet;
		char Buf[255];
		for( int i=0; i<m_lCtrlCount; i++ )
		{
			str.Format( "%04X\t%ld", m_lCtrlData[i], m_lCtrlData[i] );
			sprintf(Buf, "%s\n", str );
			WriteFile(h, Buf, strlen(Buf), &cbRet, NULL);
		}
	}
	CloseHandle( h );*/
}

BOOL CSemCtrl_ObjectiveLens::Bind( CSemCtrl_WD*	pWD )
{
	//CSemCtrl_WD * pWDSafety = DYNAMIC_DOWNCAST(CSemCtrl_WD, pWD);

	if ( pWD != NULL )
	{
		m_pWD		= pWD;
		m_bReady	= TRUE;
		m_pWD->Bind( this );
	}

	return m_bReady;
}

BOOL CSemCtrl_ObjectiveLens::Bind( CSemCtrl_ScanRotate*	pRotate )
{
	//CSemCtrl_WD * pWDSafety = DYNAMIC_DOWNCAST(CSemCtrl_ScanRotate, pRotate);

	if ( pRotate != NULL )
	{
		m_pRotate	= pRotate;
		m_bReady	= TRUE;
		m_pRotate->Bind( this );
	}

	return m_bReady;
}

VARIANT CSemCtrl_ObjectiveLens::SetPos( VARIANT varPos )
{
	VARIANT varNewPos = CSemBasisCtrl::SetPos( varPos );

//	if( m_bReady )
//		m_pWD->LookupPos( (WORD)(varNewPos.lVal) );

	return varNewPos;
}

BOOL CSemCtrl_ObjectiveLens::Flush()
{
	if ( !CSemBasisCtrl::Flush() )
		return FALSE;

	WORD				wusbCtl;
	USB_SEM_ControlMessage	usb_hostMSG;

	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;

	// 新系统中高压统调要在软件上完成
	// 高压发生改变时，聚光镜电流、物镜电流和扫描线圈电流都要发生变化
	// 原来送到Z80的高压统调值要作为系数送至上述3个值
	// 30KV时该系数为1
	if( m_bReverse )
	{
		// 0 - 65535 映射到 32768 - 0
//		wusbCtl = (WORD)( (m_scfCtrl.varMax.lVal +1 -lCtlIndex) /2 * m_dRateC);
		// 0 - 32767 映射到 32768 - 0
		wusbCtl = (WORD)( 32768 -lCtlIndex *m_dRateC );
	}
	else
	{
		// 0 - 65535 映射到 32768 - 65535
//		wusbCtl = (WORD)( (m_scfCtrl.varMax.lVal +1 +lCtlIndex) /2 * m_dRateC);
		// 0 - 32767 映射到 32768 - 65535
		wusbCtl = (WORD)( 32768 +lCtlIndex *m_dRateC );
	}

//	usb_hostMSG.bMessageID		= (BYTE)USB_CID_LNS_ObjectiveLens_Static;
	usb_hostMSG.bType			= (BYTE)1;
	usb_hostMSG.bCtlMSGLength	= (BYTE)4;
	usb_hostMSG.bData[0]		= (BYTE)USB_CID_LNS_ObjectiveLens_Static;	// Serial Comm operand low byte (X/Y Stig DAC circuit address)
	usb_hostMSG.bData[1]		= (BYTE)USB_TM_LNS_Write_16;				// Serial Comm operand high byte (16 bit write to SCN board)
	usb_hostMSG.bData[2]		= (BYTE)( wusbCtl & 0x00FF );
	usb_hostMSG.bData[3]		= (BYTE)( wusbCtl >> 8 );

	BOOL blRet = FALSE;
	blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );

	// 物镜统调扫描旋转
	if( !m_bCoadjustRotateMode && m_dRateC > 0.0 && (m_pRotate!= NULL) )
	{
//		int nAngleComp = m_dCo_Rotate[0] *(lCtlIndex *m_dRateC) + m_dCo_Rotate[1];
		int nAngleComp = m_dCo_Rotate[0] *lCtlIndex + m_dCo_Rotate[1];
		if( nAngleComp < 0 )
			nAngleComp += 360;
		if( nAngleComp > 360 )
			nAngleComp -= 360;
		m_pRotate->SetParams( 2, nAngleComp );
	}

	// 当DF复用为FF时计算小数部分的差值，并转换为DF应设置的控制量
	double dDiff = varPos.dblVal -varPos.lVal;
	long lDFCtlIndex = 0;
	if( m_bDFforFF && (fabs(dDiff) > 1.0/m_nDFforFFBase) )
	{
		for(int i=0; i<m_nDFforFFBase; i++ )
		{
			if( fabs(dDiff) < (i+1) *1.0 /m_nDFforFFBase )
			{
				lDFCtlIndex = i;
				break;
			}
		}
	}
	m_lCtrlDF = lDFCtlIndex;
	if( m_bDFforFF )
	{
		// start point
		wusbCtl = (WORD)(m_lCtrlDF +32768);
		usb_hostMSG.bType			= (BYTE)1;
		usb_hostMSG.bCtlMSGLength	= (BYTE)4;
		usb_hostMSG.bData[0]		= (BYTE)USB_CID_LNS_ObjectiveLens_DnyFocusStartPoint;
		usb_hostMSG.bData[1]		= (BYTE)USB_TM_LNS_Write_16;
		usb_hostMSG.bData[2]		= (BYTE)( wusbCtl & 0x00FF );
		usb_hostMSG.bData[3]		= (BYTE)( wusbCtl >> 8 );
		blRet = CMP::Instance().USB_PostMSGEx( usb_hostMSG );
	}

	// 物镜统调工作距离
	/////////////////////////////////////////////////////////////
	// 15.05.14 韩勇说在拟合新统调公式时，3200的拟合效果偏差较大
	// 所以有可能6200还使用之前的统调公式
	/////////////////////////////////////////////////////////////
	if( m_nType < 1 )
	{
		// 原6200
		double dK = m_dCo_Ka * (32768 +lCtlIndex) + m_dCo_Kb;
		if( (dK > 0) && (fabs( m_dK-dK) > 0.00001) )
		{
			m_dK = dK;
			// 负向时暂时不计算
			double dM = 0.0;
			// M = 4 * K * SQRT( N / HV ) - CNT
			dM = 4 * dK * sqrt( double(Co_6200_N / 300.0) ) - Co_6200_CNT;
			if( dM > 0 )
				m_pWD->Co_UpdatePosByM( dM );
		}
	}
	else
	{
		if( m_nType < 2 )
		{
			// 新6200
			double dOL = m_dCo_Ka * (lCtlIndex * 1.0 / 32.0 * m_dRateM) + m_dCo_Kb;
			if( dOL > 0 )
				m_pWD->Co_UpdatePosByOL( dOL );
		}
		else
		{
			// 6200 & 6900
			double dOL = (lCtlIndex +1.0*m_lCtrlDF/m_nDFforFFBase) * (1.0 / 32.0) * m_dRateM;
			if( dOL > 0 )
				m_pWD->Co_UpdatePosByOL( dOL );
		}
	}

	return blRet;
}

const CString& CSemCtrl_ObjectiveLens::FormatText()
{
	WORD			wusbCtl;
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;
//	const double dfToDisplay = (double)1.0/64.0;
	const double dfToDisplay = (double)1.0/32.0;

	if( m_bReverse )
	{
		// 0 - 65535 映射到 32768 - 0
//		wusbCtl = (WORD)( (m_scfCtrl.varMax.lVal +1 -lCtlIndex) /2 * m_dRateC);
		// 0 - 32767 映射到 32768 - 0
		wusbCtl = (WORD)( 32768 -lCtlIndex *m_dRateC );//( (32768 -lCtlIndex) * m_dRateC);
	}
	else
	{
		// 0 - 65535 映射到 32768 - 65535
//		wusbCtl = (WORD)( (m_scfCtrl.varMax.lVal +1 +lCtlIndex) /2 * m_dRateC);
		// 0 - 32767 映射到32768 - 65535
		wusbCtl = (WORD)( 32768 +lCtlIndex * m_dRateC );//((lCtlIndex +32768) * m_dRateC);
	}

	// Format Display string
	double dOL = lCtlIndex * dfToDisplay * m_dRateM;
	double dFF = 1.0*m_lCtrlDF/m_nDFforFFBase * dfToDisplay * m_dRateM;
	double dTotal = (lCtlIndex +1.0*m_lCtrlDF/m_nDFforFFBase) * dfToDisplay * m_dRateM;
	if( m_nOpr > 0 )
		m_strText.Format( "[ %04X ] %4.2f + [%02d]%.4f = %4.2f", wusbCtl, dOL, m_lCtrlDF, dFF, dTotal );
	else
		m_strText.Format( "%4.2f", dTotal );

	return m_strText;
}


long CSemCtrl_ObjectiveLens::GetPosByValue(const double dValue)
{
	const double dfToDisplay = (double)1.0/32.0;
	long lCtrlIndex = dValue/(dfToDisplay * m_dRateM) - 1.0*m_lCtrlDF/m_nDFforFFBase;
	return lCtrlIndex;
}


const CString& CSemCtrl_ObjectiveLens::FormatText( VARIANT varPos )
{
	static CString strText;
	long lCtlIndex = varPos.lVal;
//	const double dfToDisplay = (double)1.0/64.0;
	const double dfToDisplay = (double)1.0/32.0;

	double dTotal = (lCtlIndex +1.0*m_lCtrlDF/m_nDFforFFBase) * dfToDisplay * m_dRateM;
	strText.Format( "%4.6f", dTotal );

	return strText;
}

/*
const int SET_PARAMS_DebugMode			= 1;	// 设置调试模式
const int SET_PARAMS_Reverse			= 2;	// 设置反转
const int SET_PARAMS_DetPair			= 3;	// 设置探测器组
const int SET_PARAMS_Rate				= 4;	// 设置统调比例系数
const int SET_PARAMS_Det2nd				= 5;	// 设置探测器组中的第二通道
*/

void CSemCtrl_ObjectiveLens::SetParams( int nType, double dParam )// Type:类型；Param:要设置的参数值
{
	switch( nType )
	{
	case	SET_PARAMS_DebugMode:
		// 设置调试模式，该模式下滚动条显示值不乘以映射比例系数
		m_bDebug = (BOOL)dParam;
		if( m_bDebug )
			m_dRateM = 1.0;
		else
		{
			double dValue = 512.0;
			if( CSemBasisCtrl::ReadINI( INI_Ratio, INIIndex_OL, &dValue ) )
				m_dRateM = 512.0 / dValue;
		}
		break;

	case	SET_PARAMS_Reverse:
		m_bReverse = (BOOL)dParam;
		Flush();
		break;

	case	SET_PARAMS_Rate:
		{
// 			if( dParam < 1 )
// 				m_dRateC = 1.0;
// 			else
//				m_dRateC = sqrt( dParam /300.0 );
//			m_dRateC = pow( dParam /300.0, m_dCo_HV );
			UpdateCoadjust_HV_WD_RadioC( dParam );
			if( !m_bCoadjustHVMode )
			{
				m_pWD->SetParams( 201, dParam );
				char szBuf[255] = {0};
				sprintf( szBuf, "\r\nRate of HV coadjust: original = %.6f, new = %.6f\r\n", sqrt( dParam /300.0 ), m_dRateC );
				OutputDebugString( szBuf );
			}
			Flush();
		}
		break;

/*	case	5:
		// 这其实和传递m_dRateC是一样的，根据m_dRateC可以得出目前的高压值
		// 但因为实验阶段m_dRateC都是从CPS_HV送1.0，另设一个参数保存高压值
		// 以后可合并
		Flush();
		break;
*/
	case	9:		// 统调校准后，当时就可以生效
		m_dCo_Ka = dParam;
		break;
	case	10:		// 统调校准后，Ka和Kb必须成对送下，然后调用Flush生效
		m_dCo_Kb = dParam;
		Flush();
		break;

	case	11:		// 是否使用DF复用FF
		m_bDFforFF = (BOOL)dParam;
		if( !m_bDFforFF )
			m_lCtrlDF = 0;
		Flush();
		break;

	case	21:
		// 高压统调工作距离
		if( dParam > 0.0 )
		{
			// 有生效的系数，就再读取一次ini
			UpdateCoadjust_HV_WD();
		}
		else
		{
			m_bCoadjustHVMode = TRUE;
			m_dRateC = 1.0;
			Flush();
		}
		break;

	case	22:
		{
			// 物镜统调扫描旋转
			if( dParam > 0 )
			{
				m_bCoadjustRotateMode = TRUE;
				m_pRotate->SetParams( 2, 0 );
			}
			else
			{
				UpdateCoadjust_OL_Rotate();
				Flush();
			}
		}
		break;

	case	100:	// OprAdvance
		m_nOpr	= (int)dParam;
		break;
	case	101:	// SemType
		m_nType	= (int)dParam;
		m_pWD->m_nType = m_nType;
		if( m_nType > 1 )	// 6200 & 6900
		{
			m_dCo_Ka = 1.0;
			m_dCo_Kb = 0.0;
		}
		break;

	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////
// 2024.03 统调实验时使用
void CSemCtrl_ObjectiveLens::GetParams( int nType, double& dParam )
{
	VARIANT varPos = GetPos();
	long lCtlIndex = varPos.lVal;
	const double dfToDisplay = (double)1.0/32.0;
	switch( nType )
	{
	case	0:	// lCtrlIndex
		dParam = lCtlIndex;
		break;
	case	1:	// wusbCtl
		{
			WORD			wusbCtl;
			//	const double dfToDisplay = (double)1.0/64.0;
			const double dfToDisplay = (double)1.0/32.0;

			if( m_bReverse )
			{
				// 0 - 65535 映射到 32768 - 0
//				wusbCtl = (WORD)( (m_scfCtrl.varMax.lVal +1 -lCtlIndex) /2 * m_dRateC);
				// 0 - 32767 映射到 32768 - 0
				wusbCtl = (WORD)( 32768 -lCtlIndex *m_dRateC );//( (32768 -lCtlIndex) * m_dRateC);
			}
			else
			{
				// 0 - 65535 映射到 32768 - 65535
//				wusbCtl = (WORD)( (m_scfCtrl.varMax.lVal +1 +lCtlIndex) /2 * m_dRateC);
				// 0 - 32767 映射到32768 - 65535
				wusbCtl = (WORD)( 32768 +lCtlIndex * m_dRateC );//((lCtlIndex +32768) * m_dRateC);
			}
			dParam = wusbCtl;
		}
		break;
	case	2:
		dParam = lCtlIndex * dfToDisplay * m_dRateM;
		break;
	case	3:
		dParam = (lCtlIndex +1.0*m_lCtrlDF/m_nDFforFFBase) * dfToDisplay * m_dRateM;
		break;
	}
}
/////////////////////////////////////////////////////////////////////
