#pragma once
#include <string>
class CMCICard;

namespace MCICardGetDataLogicExecutor
{
	class IMCICardGetDataLogicExecutor
	{
	public:
		IMCICardGetDataLogicExecutor(){};
		virtual void GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow) = 0;
	};


	class CLASS_IMCICardGetDataLogicExecutor_TRUE_TRUE: public IMCICardGetDataLogicExecutor
	{
	public:
		virtual void GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow);
	};

	class CLASS_IMCICardGetDataLogicExecutor_TRUE_FALSE: public IMCICardGetDataLogicExecutor
	{
	public:
		virtual void GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow);
	};


	class CLASS_IMCICardGetDataLogicExecutor_FALSE_TRUE: public IMCICardGetDataLogicExecutor
	{
	public:
		virtual void GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow);

	private:
		std::string _GetFunctionID(CMCICard* pMCICard, int nChnnIndex, const char* sProcessName = "" );
	};


	class CLASS_IMCICardGetDataLogicExecutor_FALSE_FALSE: public IMCICardGetDataLogicExecutor
	{
	public:
		virtual void GetData(CMCICard* pMCICard,LPVOID lpBuff,long lBuffSize,CRect& rcFrameDestWindow);
	};
}

