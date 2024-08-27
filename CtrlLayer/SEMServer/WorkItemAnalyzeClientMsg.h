#pragma once
#include "WorkQueue.h"
#include "KYKYCommFIFO.h"

class WorkItemAnalyzeClientMsg : public WorkItemBase
{
public:
	WorkItemAnalyzeClientMsg(KYKYComm_Recv_Package& oMsg);

public:
	virtual void DoWork(void* pThreadContext);
	virtual void Abort(){ delete this; }

private:
	KYKYComm_Recv_Package& msg; 
};
