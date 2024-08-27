#include "StdAfx.h"
#include "MCICardGetDataDet2ndLogicExecutor.h"
using namespace MCICardGetDataDet2ndLogicExecutor;
#include "MCICard.h"
#include "Factory.h"
#include <string>

// ����ͨ��
// ����ͨ����ʾ���ͼ��
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

// ����ͨ��
// ����ͨ����ʾZb
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


// ����ͨ��
// ����ͨ����ʾZa
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

// ż��ͨ��
// ż��ͨ����ʾ���ͼ��
IMPLEMENT_FACTORY(IMCICardGetDataDet2ndLogicExecutor,CLASS_IMCICardGetDataDet2ndLogicExecutor_FALSE_SEGMENT1,std::string,"Index0_Segment1_Det2nd")
void CLASS_IMCICardGetDataDet2ndLogicExecutor_FALSE_SEGMENT1::GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow)
{
	const int nFunctionID = pMCICard->GetFunction();
	switch( nFunctionID )
	{
	case 3:
	case 4:
	case 5:		// ��ɨ�衢��ɨ������ˢ��ͼ������
		pMCICard->ADxyzLine2ndMix( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	case 6:		// ͬ��ɨ��
		pMCICard->ADxyzSync2ndMix( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	default:
		pMCICard->ADxyz2ndMix( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	}
}


// ż��ͨ��
// ż��ͨ����ʾZb
IMPLEMENT_FACTORY(IMCICardGetDataDet2ndLogicExecutor,CLASS_IMCICardGetDataDet2ndLogicExecutor_FALSE_SEGMENT2,std::string,"Index0_Segment2_Det2nd")
void CLASS_IMCICardGetDataDet2ndLogicExecutor_FALSE_SEGMENT2::GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow)
{
	const int nFunctionID = pMCICard->GetFunction();
	switch( nFunctionID )
	{
	case 3:
	case 4:
	case 5:		// ��ɨ�衢��ɨ������ˢ��ͼ������
		pMCICard->ADxyzLine2nd( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	case 6:		// ͬ��ɨ��
		pMCICard->ADxyzSync2nd( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	default:
		pMCICard->ADxyz2nd( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	}
}


// ż��ͨ��
// ż��ͨ����ʾZa
IMPLEMENT_FACTORY(IMCICardGetDataDet2ndLogicExecutor,CLASS_IMCICardGetDataDet2ndLogicExecutor_FALSE_SEGMENT3,std::string,"Index0_Segment3_Det2nd")
void CLASS_IMCICardGetDataDet2ndLogicExecutor_FALSE_SEGMENT3::GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow)
{
	const int nFunctionID = pMCICard->GetFunction();
	switch( nFunctionID )
	{
	case 3:
	case 4:
	case 5:		// ��ɨ�衢��ɨ������ˢ��ͼ������
		pMCICard->ADxyzLine( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	case 6:		// ͬ��ɨ��
		pMCICard->ADxyzSync( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	default:
		pMCICard->ADxyz( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	}
}
