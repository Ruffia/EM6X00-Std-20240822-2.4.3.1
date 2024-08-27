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
    m_nFocusStart(0),
    m_nFocusStop(1),
    m_nFocusStep(1)
{
	
}