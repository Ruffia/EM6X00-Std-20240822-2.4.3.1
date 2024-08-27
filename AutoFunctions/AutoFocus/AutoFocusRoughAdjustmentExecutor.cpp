#include "stdafx.h"
#include "AutoFocusPhaseExecutor.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAutoFocusPhaseExecutor::CAutoFocusPhaseExecutor():
	c_nFocusMAX(32767),
    c_nFocusMIN(0),
    c_nFocusStart(320*32),
    c_nFocusStop(750*32),
    c_nFocusStep((c_nFocusStop - c_nFocusStart) / c_nStepCount)	//1142;
{
	
}
