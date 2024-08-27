#include "stdafx.h"
#include "Communicator.h"
#include "DebugMessageDispatcher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCommunicator& CCommunicator::Instance()
{
	static CCommunicator Inst;
	return Inst;
}

CCommunicator::CCommunicator()
{
	
}