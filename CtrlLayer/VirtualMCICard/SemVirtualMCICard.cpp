#include "stdafx.h"
#include "MCICard.h"
#include "SemVirtualMCICard.h"
#include "MCICardOption.h"
#include "MCICardCommonSetting.h"
#include "GlobalParamSetting.h"
#include "CommonFunctionManager.h"
#include "ScaleManager.h"
#include "StageXYManager.h"
#include "AppOption.h"
#include "ExportStageMC600Func.h"
#include "ExportStageMC6600Func.h"
#include "ExportStageTMCMFunc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CSemVirtualMCICard& CSemVirtualMCICard::Instance()
{
	static CSemVirtualMCICard Inst;
	return Inst;
}

CSemVirtualMCICard::CSemVirtualMCICard()
{
	m_pMCICard		= NULL;
	m_dblZoom		= 1.0;
	m_sizeReso		= CSize(512,512);
	m_nDetectorMode	= 0;
	m_nDet2nd		= 0;
	m_bDebugZ		= FALSE;
	m_bDebugOsc		= FALSE;
	m_bDebugOsc90	= FALSE;
	m_nusbFunction	= USB_ST_RasterScan;
	m_bSplit		= FALSE;
	m_bLive			= FALSE;
	m_nChnIndex[0]	= 0;
	m_nChnIndex[1]	= 1;
	m_sizeMonitor.cx= 480;
	m_sizeMonitor.cy= 270;

	m_nMaxPix = 2559;
	m_nDiv = 10;
	m_nSEratio = 50;

	m_csPhotoSN		= _T("SN:0000");			// 照相编号显示
	for( int i=0; i<3; i++ )
	{
		m_csDetNames[i*2]	= _T("SE");
		m_csDetNames[i*2 +1]= _T("SE/BSE");
	}
}

CSemVirtualMCICard::~CSemVirtualMCICard()
{
	//Close();
}

HDIB CSemVirtualMCICard::GetDib()
{ 
	if ( !isReady() )
		return NULL;

	if ( m_pMCICard == NULL )
		return NULL;

	return m_pMCICard->CopyImageDib();
}

BOOL CSemVirtualMCICard::GetGraphBmp( CBitmap& bmpGraphLay )
{
	if ( !isReady() )
		return FALSE;

	if ( m_pMCICard == NULL )
		return FALSE;

	return m_pMCICard->CopyGraphBmp(bmpGraphLay);
}

BOOL CSemVirtualMCICard::CopyScreenBmp( CBitmap& bmpScreen )
{
	if ( !isReady() )
		return FALSE;

	if ( m_pMCICard == NULL )
		return FALSE;

	return m_pMCICard->CopyScreenBmp(bmpScreen);
}

// 2021.06 原始数据存储
void CSemVirtualMCICard::GetImageDataSrc( int nIndex, WORD* pData )
{
	if ( m_pMCICard != NULL )
		m_pMCICard->GetImageDataSrc( nIndex, pData );
}

BOOL CSemVirtualMCICard::Close( )
{
	if ( m_pMCICard != NULL )
	{
		m_pMCICard->Close();
		m_pMCICard->KillThread();
		m_pMCICard = NULL;
	}

	m_bReady = FALSE;

	return TRUE;
}

BOOL CSemVirtualMCICard::Reset( CWnd* pWnd )
{
	Close( );

	m_pMCICard = new CMCICard();
	ASSERT_VALID(m_pMCICard);
	if(!m_pMCICard) return m_bReady;

	int i=0;
	for( i=0; i<13; i++ )
		m_pMCICard->SetColor( i, m_sysOpt.color[i] );
	for( i=0; i<3; i++ )
		m_pMCICard->SetFontName( i, m_sysOpt.fName[i] );
	for( i=0; i<3; i++ )
		m_pMCICard->SetFontSize( i, m_sysOpt.fSize[i] );
	m_pMCICard->SetTransparent( m_sysOpt.nShowParams[SysOpt_showLabelTrans] );

	m_pMCICard->SetZoom( m_dblZoom );
	m_pMCICard->SetParams( SET_PARAMS_DetectorMode, m_nDetectorMode );
	m_pMCICard->SetParams( 4, m_nDet2nd );
	m_pMCICard->SetParams( 5, m_bDebugZ );
	m_pMCICard->SetParams( 6, m_bDebugOsc );
	m_pMCICard->SetParams( 8, m_bDebugOsc90 );
	m_pMCICard->SetParams( 10, m_sysOpt.nLineWidth );
	m_pMCICard->SetParams( 13, m_sizeMonitor.cx );
	m_pMCICard->SetParams( 14, m_sizeMonitor.cy );
	m_pMCICard->SetParams( 15, m_nMaxPix );
	m_pMCICard->SetParams( 16, m_nDiv );
	m_pMCICard->SetParams( 17, m_nSEratio );

	m_pMCICard->SetFunction( m_nusbFunction );
	m_pMCICard->SetSplit( m_bSplit );
	m_pMCICard->SetLiveMode( m_bLive );
	m_pMCICard->SetResolution( m_sizeReso );
	m_pMCICard->GetLabelControler()->UpdatePhotoSN( m_csPhotoSN );
	for( i=0; i<11; i++ )
	{
		m_pMCICard->UpdateFooter( i, m_footerOpt.opt[i].bVisible, m_footerOpt.opt[i].ptPos );
		m_pMCICard->UpdateFooterText( i, m_footerOpt.opt[i].strDisp );
	}
	for( i=0; i<6; i++ )
		m_pMCICard->UpdateChnNames( i, m_csDetNames[i] );
	m_pMCICard->SetParams( 11, m_nChnIndex[0] );
	m_pMCICard->SetParams( 12, m_nChnIndex[1] );

	if ( m_pMCICard->Reset( pWnd ) )
	{
		m_pMCICard->m_pThreadParams = NULL;

		// Create Thread in a suspended state so we can set the Priority
		// before it starts getting away from us
		if (!m_pMCICard->CreateThread(CREATE_SUSPENDED))
		{
			delete m_pMCICard;
			m_pMCICard = NULL;
		}
		else
		{
			//VERIFY(m_pMCICard->SetThreadPriority(THREAD_PRIORITY_IDLE));
//				VERIFY(m_pMCICard->SetThreadPriority(THREAD_PRIORITY_NORMAL));
			//VERIFY(m_pMCICard->SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL));
			VERIFY(m_pMCICard->SetThreadPriority(THREAD_PRIORITY_HIGHEST));

			// Now the thread can run wild
			m_pMCICard->ResumeThread();  // remark by LMH 2001-8-7 Test for 慢扫死机 T9

			m_bReady = TRUE;
		}
	}
	else
	{
		Close();
	}

	return m_bReady;
}

BOOL CSemVirtualMCICard::IsShouldReset()
{
	if ( m_pMCICard != NULL )
		return m_pMCICard->IsShouldReset();

	return FALSE;
}

BOOL CSemVirtualMCICard::InitData( DWORD dwExpectantDMABlockSize, int nXOffset, int nYOffset )
{
	if ( m_pMCICard != NULL )
		return m_pMCICard->InitData( dwExpectantDMABlockSize, nXOffset, nYOffset );

	return FALSE;
}

BOOL CSemVirtualMCICard::ClearData()
{
	if ( m_pMCICard != NULL )
		return m_pMCICard->ClearData();

	return FALSE;
}

BOOL CSemVirtualMCICard::Write( LPVOID lpBuff, long lBuffSize, CRect& rcFrameWindow, int nDataStoreMode )
{
	if ( m_pMCICard == NULL )
		return FALSE;

	if ( !(m_pMCICard->IsReady()) )
		return FALSE;

	if( nDataStoreMode > 0 )
		m_pMCICard->ImageDibWriteNewXYZ( lpBuff, lBuffSize, rcFrameWindow );// 11.11.28
	else
//		m_pMCICard->ImageDibWriteNewZZZ( lpBuff, lBuffSize, rcFrameWindow );// 11.12.06
		m_pMCICard->ImageDibWrite( lpBuff, lBuffSize, rcFrameWindow );// 11.12.06

	return TRUE;
}

BOOL CSemVirtualMCICard::PostData( LPVOID lpBuff, long lBuffSize, CRect& rcFrameDestWindow )
{
	if ( !m_pMCICard ) return FALSE;
	if ( !m_pMCICard->IsReady() ) return FALSE;

	m_pMCICard->PostData( lpBuff, lBuffSize, rcFrameDestWindow );

	return TRUE;
}

BOOL CSemVirtualMCICard::Read( LPVOID lpBuff, long lBuffSize, CRect& rcFrameWindow )
{
	if ( !m_pMCICard ) return FALSE;
	if ( !m_pMCICard->IsReady() ) return FALSE;
		
	m_pMCICard->ImageDibRead( lpBuff, lBuffSize, rcFrameWindow );

	return TRUE;
}

void CSemVirtualMCICard::SetColor( int nType, COLORREF color )
{
	m_sysOpt.color[nType]	= color;

	if ( m_pMCICard != NULL )
		m_pMCICard->SetColor( nType, color );
}

void CSemVirtualMCICard::SetFontName( int nType, CString strName )
{
	m_sysOpt.fName[nType]	= strName;

	if ( m_pMCICard != NULL )
		m_pMCICard->SetFontName( nType, strName );
}

void CSemVirtualMCICard::SetFontSize( int nType, int nSize )
{
	m_sysOpt.fSize[nType]	= nSize;

	if ( m_pMCICard != NULL )
		m_pMCICard->SetFontSize( nType, nSize );
}

void CSemVirtualMCICard::SetFontWeight( int nType, int nWeight )
{
	if ( m_pMCICard != NULL )
		m_pMCICard->SetFontWeight( nType, nWeight );
}

void CSemVirtualMCICard::SetTransparent( int nTransparent )
{
	m_sysOpt.nShowParams[SysOpt_showLabelTrans] = nTransparent;

	if ( m_pMCICard != NULL )
		m_pMCICard->SetTransparent( nTransparent );
}

void CSemVirtualMCICard::SetParams( int nType, int nParam )
{
	switch( nType )
	{
	case	SET_PARAMS_DetectorMode:
		m_nDetectorMode = nParam;
		break;
	case	4:	// 显示奇数/偶数通道图像数据
		m_nDet2nd = nParam;	// 0: 偶数通道，默认值；1：奇数通道
		break;
	case	5:	// 调试模式吗？
		m_bDebugZ = (BOOL)nParam;
		break;
	case	6:
		m_bDebugOsc = (BOOL)nParam;
		break;
	case	8:
		m_bDebugOsc90 = (BOOL)nParam;
		break;
	case	10:
		m_sysOpt.nLineWidth = nParam;
		break;
	case	11:
		m_nChnIndex[0] = nParam;
		break;
	case	12:
		m_nChnIndex[1] = nParam;
		break;
	case	13:
		m_sizeMonitor.cx = nParam;
		break;
	case	14:
		m_sizeMonitor.cy = nParam;
		break;
	case	15:
		m_nMaxPix = nParam;
		break;
	case	16:
		m_nDiv = nParam;
		break;
	case	17:
		m_nSEratio = nParam;
		break;
	default:
		break;
	}

	if ( m_pMCICard == NULL )
		return;

	if ( !(m_pMCICard->IsReady()) )
		return;

	m_pMCICard->SetParams( nType, nParam );
}

void CSemVirtualMCICard::SetSharedImageBuffIndex( int nIndex )
{
	ASSERT(m_pMCICard);
	m_pMCICard->SetSharedImageBuffIndex( nIndex );
}

void CSemVirtualMCICard::GetCurrentY( int &nY1, int &nY2 )
{
	if( m_pMCICard != NULL )
	{
		nY1	= m_pMCICard->m_nCurrentY1;
		nY2	= m_pMCICard->m_nCurrentY2;
	}
	else
	{
		nY1 = 0;
		nY2	= 0;
	}
}

CSize CSemVirtualMCICard::GetResolution()
{ 
	if (m_pMCICard != NULL)
		return m_pMCICard->GetResolution();

	return m_sizeReso;
}

void CSemVirtualMCICard::SetResolution(const CSize& szReso)
{
	m_sizeReso = szReso;

	if (m_pMCICard != NULL)
		m_pMCICard->SetResolution( m_sizeReso );
}

void CSemVirtualMCICard::SetFunction( USB_ScanType nFunction )
{
	m_nusbFunction = nFunction;

	if (m_pMCICard != NULL)
		m_pMCICard->SetFunction( nFunction );
}

void CSemVirtualMCICard::SetSplit( BOOL bSplit )
{
	m_bSplit = bSplit;

	if (m_pMCICard != NULL)
		m_pMCICard->SetSplit( bSplit );
}

void CSemVirtualMCICard::SetLiveMode( BOOL bLive )
{
	m_bLive = bLive;
	
	if (m_pMCICard != NULL)
		m_pMCICard->SetLiveMode( bLive );
}

double CSemVirtualMCICard::GetZoom()
{
	if (m_pMCICard != NULL)
		return m_pMCICard->GetZoom();

	return m_dblZoom;
}

void CSemVirtualMCICard::SetZoom(double dblZoom)
{
	m_dblZoom = dblZoom;

	if (m_pMCICard != NULL)
		m_pMCICard->SetZoom( m_dblZoom );
}

void CSemVirtualMCICard::SetDestFrame(CRect rcDestFrame)
{
	if (!m_pMCICard) return;
	m_pMCICard->SetDestFrame( rcDestFrame );
}

CSize CSemVirtualMCICard::GetSizes( int nIndex )
{
	if( m_pMCICard != NULL )
		return m_pMCICard->GetSizes( nIndex );

	return CSize(0,0);
}

void CSemVirtualMCICard::PhotoIndication( const CRect& rcDest, const COLORREF color, const BOOL boolFill )//09.06.01
{
	if (!m_pMCICard) return;
	m_pMCICard->PhotoIndication( rcDest, color, boolFill );
}

void CSemVirtualMCICard::SetPhotoPreview( BOOL boolPreview )//05.02.06
{
	if( m_pMCICard != NULL )
		m_pMCICard->SetPhotoPreview( boolPreview );
}

BOOL CSemVirtualMCICard::GetPhotoPreview()
{
	if (!m_pMCICard) return FALSE;
	return m_pMCICard->GetPhotoPreview();
}


BOOL CSemVirtualMCICard::IsStopScan()
{
	if (!m_pMCICard) return FALSE;
	return m_pMCICard->IsStopScan();
}

void  CSemVirtualMCICard::SetStopScan(BOOL bStop)
{
	if(!m_pMCICard) return;
	m_pMCICard->SetStopScan(bStop);
}

void CSemVirtualMCICard::CalculateClientWindowRect()
{
	if (!m_pMCICard) return;
	m_pMCICard->CalculateClientWindowRect();
}

//06.11.22 3200与3900采集屏显示兼容
void CSemVirtualMCICard::SetScanScreenMode( BOOL boolNewScanScreenMode )
{
	if( !m_pMCICard ) return;
	m_pMCICard->SetScanScreenMode( boolNewScanScreenMode );
}

void CSemVirtualMCICard::SetProductLogo( CString csLogo )
{
	if( !m_pMCICard ) return;
	m_pMCICard->GetLabelControler()->SetProductLogo( csLogo );
}

BOOL CSemVirtualMCICard::ShowLabel( BOOL boolShow )
{
	if ( !m_pMCICard ) return FALSE;
	return m_pMCICard->GetLabelControler()->ShowLabel( boolShow );	
}

BOOL CSemVirtualMCICard::ShowTime( BOOL boolShow )
{
	if ( !m_pMCICard ) return FALSE;
	return m_pMCICard->ShowTime( boolShow );
}

BOOL CSemVirtualMCICard::ShowNote( BOOL boolShow )
{
	if ( !m_pMCICard ) return FALSE;
	return m_pMCICard->ShowNote( boolShow );
}

BOOL CSemVirtualMCICard::ShowMobileUScale( BOOL boolShow )
{
	if ( !m_pMCICard ) return FALSE;
	return m_pMCICard->GetMobileUScaleControler()->ShowMobileUScale(m_pMCICard->GetLabelControler(), boolShow);
}

BOOL CSemVirtualMCICard::ShowFrameRate( BOOL boolShow )
{
	if ( !m_pMCICard ) return FALSE;
	return m_pMCICard->GetFrameRateControler()->ShowFrameRate( boolShow );
}

BOOL CSemVirtualMCICard::ShowHeader( BOOL boolShow )
{
	if ( !m_pMCICard ) return FALSE;
	return m_pMCICard->GetHeaderControler()->ShowHeader( boolShow );
}

BOOL CSemVirtualMCICard::ShowPixelSize( BOOL boolShow )
{
	if ( m_pMCICard != NULL )
		return m_pMCICard->ShowPixelSize( boolShow );

	return FALSE;
}

void CSemVirtualMCICard::UpdateHV( CString csHV )
{
	if ( !m_pMCICard ) return;
	m_pMCICard->GetLabelControler()->UpdateHV( csHV );
}

void CSemVirtualMCICard::UpdateWD( CString csWD )
{
	if ( !m_pMCICard ) return;
	m_pMCICard->GetLabelControler()->UpdateWD( csWD );
}

void CSemVirtualMCICard::UpdatePhotoSN( CString csPhotoSN )
{
	m_csPhotoSN = csPhotoSN;
	if ( !m_pMCICard ) return;
	m_pMCICard->GetLabelControler()->UpdatePhotoSN( csPhotoSN );
}

void CSemVirtualMCICard::UpdateTime( CString csTime )
{
	if ( !m_pMCICard ) return;
	m_pMCICard->GetLabelControler()->UpdateTime( csTime );
}

void CSemVirtualMCICard::UpdateNote( CString csNote )
{
	if ( !m_pMCICard ) return;
	m_pMCICard->GetLabelControler()->UpdateNote( csNote );
}

void CSemVirtualMCICard::UpdatePixelSize( CString csPixelSize )
{
	if (m_pMCICard != NULL ) return;
	m_pMCICard->GetLabelControler()->UpdatePixelSize( csPixelSize );
}

void CSemVirtualMCICard::UpdateVac( CString csVac )
{
	if (!m_pMCICard)  return;
	m_pMCICard->GetLabelControler()->UpdateVac( csVac );
}

void CSemVirtualMCICard::UpdateMAG(
	CString		csMag,					// 放大倍数显示字符
	int			nUScalePixelNum,		// 固定标尺显示长度
	CString		csUScal,				// 固定标尺显示字符
	CString		csMobileUScaleText )	// 移动标尺显示字符
{	
	if ( !m_pMCICard )  return;
	m_pMCICard->UpdateMAG(csMag, nUScalePixelNum, csUScal, csMobileUScaleText);
}

void CSemVirtualMCICard::UpdateUScale(
	int			nMobileUScalePixelNum,	// 移动标尺显示长度
	CString		csMobileUScaleText )	// 移动标尺显示字符
{
	if (!m_pMCICard ) return;
	m_pMCICard->GetMobileUScaleControler()->UpdateUScale(nMobileUScalePixelNum, csMobileUScaleText);
}

void CSemVirtualMCICard::GetMobileUScaleParam( int& nMobileUScalePixelNum, CPoint& ptMobileUScale )
{
	if (!m_pMCICard) return;
	m_pMCICard->GetMobileUScaleControler()->GetMobileUScaleParam(nMobileUScalePixelNum, ptMobileUScale);
}

//======2007.08.20_任意方向移动标尺========================
// 修改标示：2007.08.20_任意方向移动标尺
// 修改日期：2007.08.20星期一
// 修改人  ：J.Y.Ma
// 修改原因：移动标尺由单一水平方向变为任意方向

void CSemVirtualMCICard::GetMobileUScaleParamEx( int& nMobileUScalePixelNum, CPoint& ptMobileUScale, CPoint& ptMobileUScale2 )
{
	if (!m_pMCICard) return;
	m_pMCICard->GetMobileUScaleControler()->GetMobileUScaleParamEx(nMobileUScalePixelNum, ptMobileUScale, ptMobileUScale2);
}

void CSemVirtualMCICard::SetMobileUScalePosEx(
		const CPoint ptMobileUScale, const CPoint ptMobileUScale2, const COLORREF color )
{
	if (!m_pMCICard) return;
	m_pMCICard->GetMobileUScaleControler()->SetMobileUScalePosEx(ptMobileUScale, ptMobileUScale2, color);
	m_pMCICard->GetMobileUScaleControler()->UpdateUScaleRect(m_pMCICard->GetLabelControler());
}

// 16.06.12 多移动标尺实验
void CSemVirtualMCICard::GetMobileUScaleParamArray( int nIndex, int&	nMobileUScalePixelNum, CPoint& ptMobileUScale, CPoint& ptMobileUScale2 )
{
	if ( !m_pMCICard ) return;
	m_pMCICard->GetMobileUScaleControler()->GetMobileUScaleParamArray(nIndex, nMobileUScalePixelNum, ptMobileUScale, ptMobileUScale2);
}

void CSemVirtualMCICard::SetMobileUScalePosArray( int nIndex, CPoint ptMobileUScale, CPoint ptMobileUScale2, COLORREF color )
{
	if (!m_pMCICard) return;
	m_pMCICard->GetMobileUScaleControler()->SetMobileUScalePosArray(nIndex, ptMobileUScale, ptMobileUScale2, color);
	m_pMCICard->GetMobileUScaleControler()->UpdateUScaleRect(m_pMCICard->GetLabelControler());
}

void CSemVirtualMCICard::UpdateUScaleArray(	int			nIndex,
						  int			nMobileUScalePixelNum,	// 移动标尺显示长度
						  CString		csMobileUScaleText )	// 移动标尺显示字符
{
	if (!m_pMCICard )  return;
	m_pMCICard->GetMobileUScaleControler()->UpdateUScaleArray( nIndex, nMobileUScalePixelNum, csMobileUScaleText );
}

void CSemVirtualMCICard::UpdateMAGArray(	int nIndex,
					   CString	csMobileUScaleText )	// 移动标尺显示字符
{
	if (!m_pMCICard)  return;
	m_pMCICard->UpdateMAGArray( nIndex, csMobileUScaleText );
}
// 16.06.12 多移动标尺实验

// 2023.07 角度测量
BOOL CSemVirtualMCICard::ShowAngle( BOOL boolShow )
{
	if ( !m_pMCICard ) return FALSE;
	return m_pMCICard->GetAngleControler()->ShowAngle( boolShow );
}

void CSemVirtualMCICard::ChangeAngle( int nIndex, BOOL bAdd )
{
	if ( m_pMCICard != NULL )
		m_pMCICard->GetAngleControler()->ChangeAngle( nIndex, bAdd );
}

void CSemVirtualMCICard::SetAngleParam( int nIndex, AngleMeasure* pAM)
{
	if ( m_pMCICard != NULL )
		m_pMCICard->GetAngleControler()->SetAngleParam( nIndex, pAM );
}

void CSemVirtualMCICard::ReArrangeAngles()
{
	if ( m_pMCICard != NULL )
		m_pMCICard->GetAngleControler()->ReArrangeAngles();
}

void CSemVirtualMCICard::ResetAngles()
{
	if ( m_pMCICard != NULL )
		m_pMCICard->GetAngleControler()->ResetAngles();
}
// 2023.07 角度测量

void CSemVirtualMCICard::UpdateFooter( int nIndex, BOOL bVisible, CPoint ptPos )
{
	m_footerOpt.opt[nIndex].bVisible = bVisible;
	m_footerOpt.opt[nIndex].ptPos = ptPos;
	if (!m_pMCICard)  return;
	m_pMCICard->UpdateFooter( nIndex, bVisible, ptPos );
}

void CSemVirtualMCICard::UpdateFooterText( int nIndex, CString csText )
{
	m_footerOpt.opt[nIndex].strDisp = csText;
	if (!m_pMCICard)  return;
	m_pMCICard->UpdateFooterText( nIndex, csText );
}

void CSemVirtualMCICard::UpdateChnNames( int nIndex, CString csText )
{
	m_csDetNames[nIndex] = csText;
	if ( m_pMCICard != NULL )
		m_pMCICard->UpdateChnNames( nIndex, csText );
}

BOOL CSemVirtualMCICard::GraphDrawLine(
		const CPoint ptPos0, const CPoint ptPos1, const COLORREF color )
{
	if (!m_pMCICard)  return FALSE;
	return m_pMCICard->GraphDrawLine(	ptPos0, ptPos1, color );
}

BOOL CSemVirtualMCICard::GraphDrawLineEx( const int nLength )	// 单位：mm
{
	if (!m_pMCICard)  return FALSE;
	return m_pMCICard->GraphDrawLineEx(	nLength );
}

BOOL CSemVirtualMCICard::GraphDrawRect( const CRect rcDest, const COLORREF color )
{
	if (!m_pMCICard)  return FALSE;
	return m_pMCICard->GraphDrawRect( rcDest, color );
}

BOOL CSemVirtualMCICard::GraphDrawRectEx( const BOOL bDrawCircle, const CRect rcDest, const COLORREF color )
{
	if (!m_pMCICard)  return FALSE;
	return m_pMCICard->GraphDrawRectEx( bDrawCircle, rcDest, color );
}

BOOL CSemVirtualMCICard::GraphDrawCircle( const CPoint ptCenter, const CPoint ptEnd, BOOL bDrawArrow, const int nRadius, const COLORREF color )
{
	if (!m_pMCICard)  return FALSE;
	return m_pMCICard->GraphDrawCircle( ptCenter, ptEnd, bDrawArrow, nRadius, color );
}

BOOL CSemVirtualMCICard::GraphClear( const CRect rcDest, const COLORREF color )
{
	if (!m_pMCICard)  return FALSE;
	return m_pMCICard->GraphClear( rcDest, color );
}

CSize CSemVirtualMCICard::GraphTextOut( const CPoint ptPos, const CString csText )
{
	if (!m_pMCICard)  return CSize(0,0);
	return m_pMCICard->GraphTextOut( ptPos, csText );
}

CSize CSemVirtualMCICard::GetOutputTextExtent( const CString csText )
{
	if (!m_pMCICard)  return CSize(0,0);
	return m_pMCICard->GetOutputTextExtent( csText );
}

BOOL CSemVirtualMCICard::ShowOscillograph( BOOL boolShow )
{
	if (!m_pMCICard) return FALSE;
	if (m_pMCICard->GetOscillographControler()->GetShowOscillograph() == boolShow)
	{
		return boolShow;
	}

	m_pMCICard->GetOscillographControler()->UpdateOscillographRect();
	return m_pMCICard->GetOscillographControler()->ShowOscillograph( boolShow );
}

void CSemVirtualMCICard::GetOscillographParam( CPoint& ptPos )
{
	if (!m_pMCICard) return;
	m_pMCICard->GetOscillographControler()->GetOscillographParam(ptPos);
}

void CSemVirtualMCICard::SetOscillographPos( CPoint ptPos )
{
	if (!m_pMCICard) return;
	m_pMCICard->GetOscillographControler()->UpdateOscillographRect();
	m_pMCICard->GetOscillographControler()->SetOscillographPos(ptPos);
}

void CSemVirtualMCICard::UpdateOscillograph( LPBYTE pBuff, long nScanLine, long nStart, long nStop )
{
	if (!m_pMCICard) return;
	m_pMCICard->GetOscillographControler()->UpdateOscillograph( pBuff, nScanLine, nStart, nStop,m_pMCICard->m_nDetectorMode,CMCICardCommonSetting::Instance().m_szImage );
}

void CSemVirtualMCICard::SetOscillographAmplifer(double dblAmplifer)
{
	if (!m_pMCICard)  return;
	m_pMCICard->GetOscillographControler()->SetOscillographAmplifer( dblAmplifer );
}


BOOL CSemVirtualMCICard::IsReadyFramGrayHistogram()
{
	if (!m_pMCICard)  return FALSE;
	return m_pMCICard->GetOscillographControler()->IsReadyFramGrayHistogram();
}

void CSemVirtualMCICard::FramGrayHistogramStart()
{
	if (!m_pMCICard)  return;
	m_pMCICard->GetOscillographControler()->FramGrayHistogramStart();
}

void CSemVirtualMCICard::FramGrayHistogramStop()
{
	if (!m_pMCICard)  return;
	m_pMCICard->GetOscillographControler()->FramGrayHistogramStop();
}

void CSemVirtualMCICard::FramGrayHistogramReset()
{
	if (!m_pMCICard)  return;
	m_pMCICard->GetOscillographControler()->FramGrayHistogramReset();
}

DWORD CSemVirtualMCICard::CopyFramGrayHistogramBuff( DWORD* pFramGrayHistogramBuff )
{
	if (!m_pMCICard)  return 0;
	return m_pMCICard->GetOscillographControler()->CopyFramGrayHistogramBuff( pFramGrayHistogramBuff );
}

BOOL CSemVirtualMCICard::MonitorError( int nModID, int nIndex )
{
	USB_SEM_ControlMessage	mciMSG;
	mciMSG.bType			= 0x1;
	mciMSG.bCtlMSGLength	= 3;
	mciMSG.bMessageID		= USB_TTTT_Mon_Err;
	mciMSG.bData[1]			= nModID;
	mciMSG.bData[2]			= nIndex;
	CMP::Instance().USB_PostMSGtoHost( mciMSG );

	return TRUE;
}

BOOL CSemVirtualMCICard::IsSyncFinish()
{
	if (!m_pMCICard)  return FALSE;
	return m_pMCICard->m_bSyncFinished;
}


CString  CSemVirtualMCICard::GetMobileUScaleText()
{
	if (!m_pMCICard) return "";
	return m_pMCICard->GetMobileUScaleText();
}

int CSemVirtualMCICard::CalculateMobileUScale( double dScale )
{
	// 由欲标注的尺寸得到标尺像素长度
	if ( CGlobalParamSetting::Instance().g_dfMobileUScaleParameter == -1 )
		return 0;

	return (int)(dScale *CGlobalParamSetting::Instance().g_dfMobileUScaleParameter +0.5);
}


CString CSemVirtualMCICard::CalculateMobileUScale( int nScalePixels )
{
	if ( CGlobalParamSetting::Instance().g_dfMobileUScaleParameter == -1 )
		return CGlobalParamSetting::Instance().g_csUScaleUnit;

	double dblScale = nScalePixels / CGlobalParamSetting::Instance().g_dfMobileUScaleParameter;

	CString csUScale;
	if (dblScale > 99.5)
	{
		csUScale.Format( "%.1f ", dblScale + 0.5 );//"%d", (int)(dblScale + 0.5) );
		csUScale += CGlobalParamSetting::Instance().g_csUScaleUnit;
	}
	else if (dblScale > 9.5)
	{
		csUScale.Format( "%.3f ", (dblScale*10.0 + 0.5) / 10.0 );
		csUScale += CGlobalParamSetting::Instance().g_csUScaleUnit;
	}
	else if (dblScale < 1.0)
	{
		if( CGlobalParamSetting::Instance().g_csUScaleUnit == "cm" )
			csUScale.Format( "%d mm", (int)(dblScale *1000 + 0.5) );
		else if( CGlobalParamSetting::Instance().g_csUScaleUnit == "mm" )
			csUScale.Format( "%d μm", (int)(dblScale *1000 + 0.5) );
		else if( CGlobalParamSetting::Instance().g_csUScaleUnit == "μm" )
			csUScale.Format( "%d nm", (int)(dblScale *1000 + 0.5) );
		else
			csUScale.Format( "%.3f nm", (dblScale*100.0 + 0.5) / 100.0 );
	}
	else
	{
		csUScale.Format( "%.3f ", (dblScale*100.0 + 0.5) / 100.0 );
		csUScale += CGlobalParamSetting::Instance().g_csUScaleUnit;
	}

	if( CGlobalParamSetting::Instance().m_wLanguageID == MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT))
	{
		// 在英文操作系统中，μm显示为乱码，所以改为um
		csUScale.Replace("μm", "um");
	}
	return csUScale;
}


void CSemVirtualMCICard::CFGfile_InitImageCardParams( char * path) 
{
	char szBuf[40] = {0};
	char szBuf1[20] = {0};
	CCommonFunctionManager::Instance().GetMonitorSizeFromEDID( CMCICardCommonSetting::Instance().m_dblPixelsPerMM_HORZ, CMCICardCommonSetting::Instance().m_dblPixelsPerMM_VERT );	// 不保险
	if(GetPrivateProfileString("Hardware", "Monitor", "", szBuf, 15, path) != 0)
	{
		strcpy_s(szBuf1, szBuf);
		if(strchr(szBuf1, ',') != NULL)
		{
			strcpy_s(szBuf1, strrchr(szBuf, ',')+1);	// szBuf1中是高度值
			*strchr(szBuf, ',') = '\0';				// szBuf中是宽度值
			CMCICardCommonSetting::Instance().m_dblPixelsPerMM_HORZ = 1.0 * GetSystemMetrics(SM_CXSCREEN) / atoi( szBuf );
			CMCICardCommonSetting::Instance().m_dblPixelsPerMM_VERT = 1.0 * GetSystemMetrics(SM_CYSCREEN) / atoi( szBuf1 );
			SetParams(13, atoi(szBuf));
			SetParams(14, atoi(szBuf1));
		}
	}
	if( CMCICardCommonSetting::Instance().m_dblPixelsPerMM_HORZ < 0 || CMCICardCommonSetting::Instance().m_dblPixelsPerMM_VERT < 0 )
	{
		CMCICardCommonSetting::Instance().m_dblPixelsPerMM_HORZ = 1.0 * GetSystemMetrics(SM_CXSCREEN) / 480;
		CMCICardCommonSetting::Instance().m_dblPixelsPerMM_VERT = 1.0 * GetSystemMetrics(SM_CYSCREEN) / 270;
		SetParams(13, 480);
		SetParams(14, 270);
	}
}

void CSemVirtualMCICard::InitializeImageCard(const CSize& sizeReso, CWnd* pWnd, BOOL bShowLabel, const CString& strLogoDisp) 
{
	SetZoom( 1.0 );
	//	SetResolution(RESO_1024_768);
	SetResolution(sizeReso);
	Reset( pWnd );
	ShowLabel( bShowLabel );
	SetParams( 4, 0 );	// 默认显示偶数通道图像数据

	SetScanScreenMode( TRUE );
	SetProductLogo( strLogoDisp );
}

CString CSemVirtualMCICard::CalculateUScale( int nScreenWidth, double dfMag, int& nScalePixels )
{
	CString strRet;
	int		u_scale;

	// Mag = 1, 
	// PhotoWidth = 120 mm		<==>	ScreenWidth = 1024 Pixel
	// u_scale = 10 mm			<==>	ScaleWidth	= ? Pixel
	////////////////////////////////////////////////////////////
	// ScaleWidth		= u_scale * ScreenWidth / PhotoWidth
	// SreenScaleParam	= ScreenWidth / PhotoWidth = 1024 / 120
	const double SreenScaleParam = 1024.0 / 127.0 / (1024.0 /nScreenWidth);
	//	const double SreenScaleParam = nScreenWidth / 120.0;	//1024.0 / 120.0;	// 11.05
	char szBuffer[64];
	CString str;

	if(dfMag<0.1 && dfMag>=0.01)
	{
		u_scale=1;
		strRet.Format("%3d cm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam*1000 + 0.5);

		CGlobalParamSetting::Instance().g_csUScaleUnit = _T("cm");
		CGlobalParamSetting::Instance().g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000;
	}
	else if(dfMag<1.0 && dfMag>=0.1)
	{
		u_scale=100;
		strRet.Format("%3d mm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam + 0.5);

		CGlobalParamSetting::Instance().g_csUScaleUnit = _T("mm");
		CGlobalParamSetting::Instance().g_dfMobileUScaleParameter = dfMag*SreenScaleParam;
	}
	else if(dfMag<10.&& dfMag>=1.0 )
	{
		u_scale=10;
		strRet.Format("%3d mm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam + 0.5);

		CGlobalParamSetting::Instance().g_csUScaleUnit = _T("mm");
		CGlobalParamSetting::Instance().g_dfMobileUScaleParameter = dfMag*SreenScaleParam;

		CStageXYManager::Instance().m_Stage.xy.nStageSPX = 10000;
		CStageXYManager::Instance().m_Stage.xy.nStageSPY = 10000;
		str.Format( "SP %ld,%ld", CStageXYManager::Instance().m_Stage.xy.nStageSPX, CStageXYManager::Instance().m_Stage.xy.nStageSPY );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	}
	else if(dfMag<100.&& dfMag>=10.)
	{
		u_scale=1;
		strRet.Format("%3d mm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam + 0.5);

		CGlobalParamSetting::Instance().g_csUScaleUnit = _T("mm");
		CGlobalParamSetting::Instance().g_dfMobileUScaleParameter = dfMag*SreenScaleParam;

		CStageXYManager::Instance().m_Stage.xy.nStageSPX = 8000;
		CStageXYManager::Instance().m_Stage.xy.nStageSPY = 8000;
		str.Format( "SP %ld,%ld", CStageXYManager::Instance().m_Stage.xy.nStageSPX, CStageXYManager::Instance().m_Stage.xy.nStageSPY );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	}
	else if(dfMag<1000. && dfMag>=100.)
	{
		u_scale=100;
		strRet.Format("%3d μm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000 + 0.5);

		CGlobalParamSetting::Instance().g_csUScaleUnit = _T("μm");
		CGlobalParamSetting::Instance().g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000;

		CStageXYManager::Instance().m_Stage.xy.nStageSPX = 800;
		CStageXYManager::Instance().m_Stage.xy.nStageSPY = 800;
		str.Format( "SP %ld,%ld", CStageXYManager::Instance().m_Stage.xy.nStageSPX, CStageXYManager::Instance().m_Stage.xy.nStageSPY );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	}
	else if(dfMag<10000.&& dfMag>=1000.)
	{
		u_scale=10;
		strRet.Format("%3d μm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000 + 0.5);

		CGlobalParamSetting::Instance().g_csUScaleUnit = _T("μm");
		CGlobalParamSetting::Instance().g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000;

		CStageXYManager::Instance().m_Stage.xy.nStageSPX = 500;
		CStageXYManager::Instance().m_Stage.xy.nStageSPY = 500;
		str.Format( "SP %ld,%ld", CStageXYManager::Instance().m_Stage.xy.nStageSPX, CStageXYManager::Instance().m_Stage.xy.nStageSPY );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	}
	else if( dfMag<100000. && dfMag>=10000.)
	{
		u_scale=1;
		strRet.Format("%3d μm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000 + 0.5);

		CGlobalParamSetting::Instance().g_csUScaleUnit = _T("μm");
		CGlobalParamSetting::Instance().g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000;

		CStageXYManager::Instance().m_Stage.xy.nStageSPX = 250;
		CStageXYManager::Instance().m_Stage.xy.nStageSPY = 250;
		str.Format( "SP %ld,%ld", CStageXYManager::Instance().m_Stage.xy.nStageSPX, CStageXYManager::Instance().m_Stage.xy.nStageSPY );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	}
	else if(dfMag>=100000.)
	{
		u_scale=100;
		strRet.Format("%3d nm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000000 + 0.5);

		CGlobalParamSetting::Instance().g_csUScaleUnit = _T("nm");
		CGlobalParamSetting::Instance().g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000000;

		CStageXYManager::Instance().m_Stage.xy.nStageSPX = 100;
		CStageXYManager::Instance().m_Stage.xy.nStageSPY = 100;
		str.Format( "SP %ld,%ld", CStageXYManager::Instance().m_Stage.xy.nStageSPX, CStageXYManager::Instance().m_Stage.xy.nStageSPY );
		CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	}
	else
	{
		strRet = _T("-----");
		nScalePixels = 20;

		CGlobalParamSetting::Instance().g_csUScaleUnit = _T("-----");
		CGlobalParamSetting::Instance().g_dfMobileUScaleParameter = -1;
	}

	return strRet;
}



CString CSemVirtualMCICard::CalculateUScale2( int nScreenWidth, double dfMag, int& nScalePixels )
{
	CString strRet;
	int		u_scale;

	// Mag = 1, 
	// PhotoWidth = 120 mm		<==>	ScreenWidth = 1024 Pixel
	// u_scale = 10 mm			<==>	ScaleWidth	= ? Pixel
	////////////////////////////////////////////////////////////
	// ScaleWidth		= u_scale * ScreenWidth / PhotoWidth
	// SreenScaleParam	= ScreenWidth / PhotoWidth = 1024 / 120
	const double SreenScaleParam = 1024.0 / 127.0 / (1024.0 /nScreenWidth);
	//	const double SreenScaleParam = nScreenWidth / 120.0;	//1024.0 / 120.0;	// 11.05
	CString str;
	char szBuffer[64];
	// 将固定标尺线长固定在图像分辨率的1/4以内，且在底标的靠右侧标注
	// 即标注尺寸将不一定是1、10、100这样的整数值
	double dScale = (nScreenWidth /5) / (dfMag *SreenScaleParam );
	if( dScale > 0.999 )
	{
		u_scale = (int)dScale;
		strRet.Format("%dmm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam + 0.5);
		CGlobalParamSetting::Instance().g_csUScaleUnit = _T("mm");
		CGlobalParamSetting::Instance().g_dfMobileUScaleParameter = dfMag*SreenScaleParam;

		CStageXYManager::Instance().m_Stage.xy.nStageSPX = 10000;
		CStageXYManager::Instance().m_Stage.xy.nStageSPY = 10000;
	}
	else if( dScale > 0.0999 )
	{
		u_scale = (int)(dScale *1000) /100 *100;
		strRet.Format("%dμm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000 + 0.5);
		CGlobalParamSetting::Instance().g_csUScaleUnit = _T("μm");
		CGlobalParamSetting::Instance().g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000;

		CStageXYManager::Instance().m_Stage.xy.nStageSPX = 8000;
		CStageXYManager::Instance().m_Stage.xy.nStageSPY = 8000;
	}
	else if( dScale > 0.00999 )
	{
		u_scale = (int)(dScale *1000 +0.05) /10 *10;
		strRet.Format("%dμm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000 + 0.5);
		CGlobalParamSetting::Instance().g_csUScaleUnit = _T("μm");
		CGlobalParamSetting::Instance().g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000;

		CStageXYManager::Instance().m_Stage.xy.nStageSPX = 4000;
		CStageXYManager::Instance().m_Stage.xy.nStageSPY = 4000;
	}
	else if( dScale > 0.00099 )
	{
		u_scale = (int)(dScale *1000 +0.05);
		strRet.Format("%dμm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000 + 0.5);
		CGlobalParamSetting::Instance().g_csUScaleUnit = _T("μm");
		CGlobalParamSetting::Instance().g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000;

		CStageXYManager::Instance().m_Stage.xy.nStageSPX = 1000;
		CStageXYManager::Instance().m_Stage.xy.nStageSPY = 1000;
	}
	else if( dScale > 0.000099 )
	{
		u_scale = (int)(dScale *1000000 +0.05) /100 *100;
		strRet.Format("%dnm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000000 + 0.5);
		CGlobalParamSetting::Instance().g_csUScaleUnit = _T("nm");
		CGlobalParamSetting::Instance().g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000000;

		CStageXYManager::Instance().m_Stage.xy.nStageSPX = 800;
		CStageXYManager::Instance().m_Stage.xy.nStageSPY = 800;
	}
	else if( dScale > 0.0000099 )
	{
		u_scale = (int)(dScale *1000000 +0.05) /10 *10;
		strRet.Format("%dnm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000000 + 0.5);
		CGlobalParamSetting::Instance().g_csUScaleUnit = _T("nm");
		CGlobalParamSetting::Instance().g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000000;

		CStageXYManager::Instance().m_Stage.xy.nStageSPX = 100;
		CStageXYManager::Instance().m_Stage.xy.nStageSPY = 100;
	}
	else if( dScale > 0.00000099 )
	{
		u_scale = (int)(dScale *1000000 +0.05);
		strRet.Format("%dnm", u_scale);
		nScalePixels = (int)(dfMag*u_scale*SreenScaleParam/1000000 + 0.5);
		CGlobalParamSetting::Instance().g_csUScaleUnit = _T("nm");
		CGlobalParamSetting::Instance().g_dfMobileUScaleParameter = dfMag*SreenScaleParam/1000000;

		CStageXYManager::Instance().m_Stage.xy.nStageSPX = 50;
		CStageXYManager::Instance().m_Stage.xy.nStageSPY = 50;
	}
	str.Format( "SP %ld,%ld", CStageXYManager::Instance().m_Stage.xy.nStageSPX, CStageXYManager::Instance().m_Stage.xy.nStageSPY );
	CStageXYManager::Instance().m_DMCWin.Command( (char*)((const char*)str), szBuffer, sizeof(szBuffer) );
	return strRet;
}



void CSemVirtualMCICard::SemLabel_UpdateMag( int nScreenWidth, const double dblMag, const CString csMag )
{
	CString		strTemp;
	int			nScalPixelNum;
	CString		csUScale;
	CString		csMobileUScaleText;
	int			nMobileUScalePixelNum;
	CPoint		ptMobileUScale;

	csUScale = CalculateUScale2( nScreenWidth, dblMag, nScalPixelNum );
	CGlobalParamSetting::Instance().g_dfMag = dblMag;

	GetMobileUScaleParam( nMobileUScalePixelNum, ptMobileUScale );
	csMobileUScaleText = CalculateMobileUScale( nMobileUScalePixelNum );

	BOOL bIsChn = (CGlobalParamSetting::Instance().m_wLanguageID == MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED));
	if( !bIsChn )
	{
		// 在英文操作系统中，μm显示为乱码，所以改为um
		csUScale.Replace("μm", "um");
		csMobileUScaleText.Replace("μm", "um");
	}

	// 2005-1-17_0.0KV问题
// 	if ( !IsStopScan() )
		UpdateMAG( csMag,nScalPixelNum,csUScale,csMobileUScaleText );

	// 多移动标尺的值也应随放大倍数改变而更新
	CPoint pt1, pt2;
	for( int i=0; i<10; i++ )
	{
		GetMobileUScaleParamArray( i, nMobileUScalePixelNum, pt1, pt2 );
		csMobileUScaleText = CalculateMobileUScale( nMobileUScalePixelNum );
		if( !bIsChn )
		{
			// 在英文操作系统中，μm显示为乱码，所以改为um
			csUScale.Replace("μm", "um");
			csMobileUScaleText.Replace("μm", "um");
		}
		UpdateMAGArray( i, csMobileUScaleText );
	}
}


void CSemVirtualMCICard::UpdateScreenMag( double dMag, BOOL bSync )
{
	const double dScale = CScaleManager::Instance().GetCurScale();
	if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 )
		StageTMCM_SetMag( dMag );
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 )
		StageMC6600_SetMag( dMag );
	else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 )
		StageMC600_SetMag( dMag );

	double dMagScreen = dMag;
	if( CAppOption::Instance().m_sysOpt.nShowParams[SysOpt_showScrMag] > 0 )	// 是否显示屏幕放大倍数
	{
		// 屏幕放大倍数与设备放大倍数一致的概念是指12.7cm*8.9cm大小的图像
		if( GetPhotoPreview() )
			dMagScreen	= dMag;
		else
		{
			if( !bSync )
			{
				dMagScreen = CMCICardCommonSetting::Instance().GetMagScreen(dMag);
				dMagScreen *= dScale;
				if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 4 )
					StageTMCM_SetMag( dMagScreen );
				else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 3 )
					StageMC6600_SetMag( dMagScreen );
				else if( CStageXYManager::Instance().m_Stage.nHasAutoStage > 1 )
					StageMC600_SetMag( dMagScreen );
			}
		}
	}

	CString str;
	// Format Magnification Display string
	if( dMagScreen < 10. )
		str.Format( "%1.2f X", dMagScreen );
	else if( dMagScreen < 100. )
		str.Format( "%2.1f X", dMagScreen );
	else if( dMagScreen < 1000. )
		str.Format( "%3.0f X", dMagScreen );
	else if( dMagScreen < 10000. )
		str.Format( "%1.2f KX", dMagScreen / 1000 );
	else if( dMagScreen < 100000. )
		str.Format( "%2.1f KX", dMagScreen / 1000 );
	else //if( dMagScreen < 1000000. )
		str.Format( "%3.0f KX", dMagScreen / 1000 );
	SemLabel_UpdateMag(CMCICardCommonSetting::Instance().m_sizeReso.cx, dMag, str);

	/////////////////////////////////////////////////////////////////
	// 更新FOV及Pixel Size
	if (!m_pMCICard)  return;
	CString strH, strV, strUnit;
	// FOV
	strH = CalculateMobileUScale( m_sizeReso.cx );
	strV = CalculateMobileUScale( m_sizeReso.cy );
	str.Format( "%s | %s", strH, strV );
	m_pMCICard->UpdateFOV( str );
	// Pixel Size
	int nPos = strH.Find( ' ' );	// 以空格为界，前边数值后面单位
	strUnit = strH.Right( strH.GetLength() -nPos -1 );
	str = strH.Left( nPos );
	double dValue = atof( str ) / m_sizeReso.cx;
	if( (dValue < 1.0) && (strUnit != "nm") )
	{
		// 值太小，需要乘1000并换算单位
		dValue *= 1000;
		if( strUnit == "cm" )
			strUnit = "mm";
		else if( strUnit == "mm" )
			strUnit = "μm";
		else
			strUnit = "nm";
	}
	else
	{
		if( (strUnit != "cm") && (strUnit != "mm") && (strUnit != "nm") )
			strUnit = "μm";
	}
	if( CGlobalParamSetting::Instance().m_wLanguageID == MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT))
	{
		// 在英文操作系统中，μm显示为乱码，所以改为um
		strUnit.Replace("μm", "um");
	}
	str.Format( "Pixel Size: %.3f %s", dValue, strUnit );
	m_pMCICard->GetLabelControler()->UpdatePixelSize( str );
}
