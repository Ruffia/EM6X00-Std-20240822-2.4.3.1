#pragma once
class CMCICard;

namespace MCICardGetDataDet2ndLogicExecutor
{
	class IMCICardGetDataDet2ndLogicExecutor
	{
	public:
		IMCICardGetDataDet2ndLogicExecutor(){};

		virtual void GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow) = 0;
	};


	class CLASS_IMCICardGetDataDet2ndLogicExecutor_TRUE_SEGMENT1: public IMCICardGetDataDet2ndLogicExecutor
	{
	public:
		virtual void GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow);
	};

	class CLASS_IMCICardGetDataDet2ndLogicExecutor_TRUE_SEGMENT2: public IMCICardGetDataDet2ndLogicExecutor
	{
	public:
		virtual void GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow);
	};

	class CLASS_IMCICardGetDataDet2ndLogicExecutor_TRUE_SEGMENT3: public IMCICardGetDataDet2ndLogicExecutor
	{
	public:
		virtual void GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow);
	};


	class CLASS_IMCICardGetDataDet2ndLogicExecutor_FALSE_SEGMENT1: public IMCICardGetDataDet2ndLogicExecutor
	{
	public:
		virtual void GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow);
	};


	class CLASS_IMCICardGetDataDet2ndLogicExecutor_FALSE_SEGMENT2: public IMCICardGetDataDet2ndLogicExecutor
	{
	public:
		virtual void GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow);
	};


	class CLASS_IMCICardGetDataDet2ndLogicExecutor_FALSE_SEGMENT3: public IMCICardGetDataDet2ndLogicExecutor
	{
	public:
		virtual void GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow);
	};
}

