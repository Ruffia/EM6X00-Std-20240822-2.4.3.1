#include "StdAfx.h"
#include "MCICardGetDataLogicExecutor.h"
using namespace MCICardGetDataLogicExecutor;
#include "MCICard.h"
#include "MCICardGetDataDet2ndLogicExecutor.h"
using namespace MCICardGetDataDet2ndLogicExecutor;
#include "Factory.h"

// 分割扫描
IMPLEMENT_FACTORY(IMCICardGetDataLogicExecutor,CLASS_IMCICardGetDataLogicExecutor_TRUE_TRUE,std::string,"m_bSplit_True_m_nDataStoreMode_True_GetData")
void CLASS_IMCICardGetDataLogicExecutor_TRUE_TRUE::GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow)
{
	if(!pMCICard) return;
	switch(pMCICard->GetFunction())
	{
	case 3:
	case 4:
	case 5:
		pMCICard->ADxyzLine2( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	case 6:
		pMCICard->ADxyzSync2( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	default:
		pMCICard->ADxyz2( lpBuff, lBuffSize, rcFrameDestWindow );
		break;
	}

}


IMPLEMENT_FACTORY(IMCICardGetDataLogicExecutor,CLASS_IMCICardGetDataLogicExecutor_TRUE_FALSE,std::string,"m_bSplit_True_m_nDataStoreMode_False_GetData")
void CLASS_IMCICardGetDataLogicExecutor_TRUE_FALSE::GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow)
{
	if(!pMCICard) return;
	pMCICard->ImageDibWriteNewZZZ2( lpBuff, lBuffSize, rcFrameDestWindow );
}


IMPLEMENT_FACTORY(IMCICardGetDataLogicExecutor,CLASS_IMCICardGetDataLogicExecutor_FALSE_TRUE,std::string,"m_bSplit_False_m_nDataStoreMode_True_GetData")
void CLASS_IMCICardGetDataLogicExecutor_FALSE_TRUE::GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow)
{
	if(!pMCICard) return;

	if(pMCICard->Det2nd())	// 奇数通道
	{
		std::string sFunctionID = _GetFunctionID(pMCICard,1,"Det2nd");
		IMCICardGetDataDet2ndLogicExecutor* pExecuter = Factory<IMCICardGetDataDet2ndLogicExecutor,std::string>::Instance().BuildProduct(sFunctionID);
		if (pExecuter)
		{
			pExecuter->GetData(pMCICard,lpBuff,lBuffSize,rcFrameDestWindow);
			delete pExecuter;pExecuter = NULL; 
		}
	}
	else			// 偶数通道
	{
		std::string sFunctionID = _GetFunctionID(pMCICard,0,"Det2nd");
		IMCICardGetDataDet2ndLogicExecutor* pExecuter = Factory<IMCICardGetDataDet2ndLogicExecutor,std::string>::Instance().BuildProduct(sFunctionID);
		if (pExecuter)
		{
			pExecuter->GetData(pMCICard,lpBuff,lBuffSize,rcFrameDestWindow);
			delete pExecuter;pExecuter = NULL; 
		}
	}
}



std::string CLASS_IMCICardGetDataLogicExecutor_FALSE_TRUE::_GetFunctionID(CMCICard* pMCICard, int nIndex, const char* sProcessName)
{
	int nSegment = 0;
	if( pMCICard->m_nChnIndex[nIndex] > 2 )
	{
		nSegment = 2;
	}
	else if( pMCICard->m_nChnIndex[nIndex] > 1 )
	{
		nSegment = 1;
	}
	else if( pMCICard->m_nChnIndex[nIndex] > 0 )
	{
		nSegment = 2;
	}
	else 
	{
		nSegment = 3;
	}

	char sz[128] = {0};
	sprintf_s(sz,128,"%s%d_%s%d_%s","Index",nIndex,"Segment",nSegment,sProcessName);
	std::string sID = sz;
	return sID;
}


IMPLEMENT_FACTORY(IMCICardGetDataLogicExecutor,CLASS_IMCICardGetDataLogicExecutor_FALSE_FALSE,std::string,"m_bSplit_False_m_nDataStoreMode_False_GetData")
void CLASS_IMCICardGetDataLogicExecutor_FALSE_FALSE::GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow)
{
	if(!pMCICard) return;
	pMCICard->ImageDibWriteNewZZZ( lpBuff, lBuffSize, rcFrameDestWindow );
}
