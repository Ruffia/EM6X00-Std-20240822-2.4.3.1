#include "StdAfx.h"
#include "MCICardGetDataDet2ndLogicExecutor.h"
using namespace MCICardGetDataDet2ndLogicExecutor;
#include "MCICard.h"
#include "Factory.h"
#include <string>

// 奇数通道
// 奇数通道显示混合图像
IMPLEMENT_FACTORY(IMCICardGetDataDet2ndLogicExecutor,CLASS_IMCICardGetDataDet2ndLogicExecutor_TRUE_SEGMENT1,std::string,"Index1_Segment1_Det2nd")
void CLASS_IMCICardGetDataDet2ndLogicExecutor_TRUE_SEGMENT1::GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow)
{
	const int nFunctionID = pMCICard->GetFunction();
	switch( nFunctionID )
	{
	case 3:
	case 4:
	case 5:
		pMCICard->ADxyzLine2ndMix( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	case 6:
		pMCICard->ADxyzSync2ndMix( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	default:
		pMCICard->ADxyz2ndMix( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	}
}

// 奇数通道
// 奇数通道显示Zb
IMPLEMENT_FACTORY(IMCICardGetDataDet2ndLogicExecutor,CLASS_IMCICardGetDataDet2ndLogicExecutor_TRUE_SEGMENT2,std::string,"Index1_Segment2_Det2nd")
void CLASS_IMCICardGetDataDet2ndLogicExecutor_TRUE_SEGMENT2::GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow)
{
	const int nFunctionID = pMCICard->GetFunction();
	switch( nFunctionID )
	{
	case 3:
	case 4:
	case 5:
		pMCICard->ADxyzLine2nd( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	case 6:
		pMCICard->ADxyzSync2nd( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	default:
		pMCICard->ADxyz2nd( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	}
}


// 奇数通道
// 奇数通道显示Za
IMPLEMENT_FACTORY(IMCICardGetDataDet2ndLogicExecutor,CLASS_IMCICardGetDataDet2ndLogicExecutor_TRUE_SEGMENT3,std::string,"Index1_Segment3_Det2nd")
void CLASS_IMCICardGetDataDet2ndLogicExecutor_TRUE_SEGMENT3::GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow)
{
	const int nFunctionID = pMCICard->GetFunction();
	switch( nFunctionID )
	{
	case 3:
	case 4:
	case 5:
		pMCICard->ADxyzLine( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	case 6:
		pMCICard->ADxyzSync( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	default:
		pMCICard->ADxyz( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	}
}

// 偶数通道
// 偶数通道显示混合图像
IMPLEMENT_FACTORY(IMCICardGetDataDet2ndLogicExecutor,CLASS_IMCICardGetDataDet2ndLogicExecutor_FALSE_SEGMENT1,std::string,"Index0_Segment1_Det2nd")
void CLASS_IMCICardGetDataDet2ndLogicExecutor_FALSE_SEGMENT1::GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow)
{
	const int nFunctionID = pMCICard->GetFunction();
	switch( nFunctionID )
	{
	case 3:
	case 4:
	case 5:		// 线扫描、点扫描无需刷新图像数据
		pMCICard->ADxyzLine2ndMix( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	case 6:		// 同步扫描
		pMCICard->ADxyzSync2ndMix( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	default:
		pMCICard->ADxyz2ndMix( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	}
}


// 偶数通道
// 偶数通道显示Zb
IMPLEMENT_FACTORY(IMCICardGetDataDet2ndLogicExecutor,CLASS_IMCICardGetDataDet2ndLogicExecutor_FALSE_SEGMENT2,std::string,"Index0_Segment2_Det2nd")
void CLASS_IMCICardGetDataDet2ndLogicExecutor_FALSE_SEGMENT2::GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow)
{
	const int nFunctionID = pMCICard->GetFunction();
	switch( nFunctionID )
	{
	case 3:
	case 4:
	case 5:		// 线扫描、点扫描无需刷新图像数据
		pMCICard->ADxyzLine2nd( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	case 6:		// 同步扫描
		pMCICard->ADxyzSync2nd( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	default:
		pMCICard->ADxyz2nd( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	}
}


// 偶数通道
// 偶数通道显示Za
IMPLEMENT_FACTORY(IMCICardGetDataDet2ndLogicExecutor,CLASS_IMCICardGetDataDet2ndLogicExecutor_FALSE_SEGMENT3,std::string,"Index0_Segment3_Det2nd")
void CLASS_IMCICardGetDataDet2ndLogicExecutor_FALSE_SEGMENT3::GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow)
{
	const int nFunctionID = pMCICard->GetFunction();
	switch( nFunctionID )
	{
	case 3:
	case 4:
	case 5:		// 线扫描、点扫描无需刷新图像数据
		pMCICard->ADxyzLine( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	case 6:		// 同步扫描
		pMCICard->ADxyzSync( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	default:
		pMCICard->ADxyz( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	}
}
