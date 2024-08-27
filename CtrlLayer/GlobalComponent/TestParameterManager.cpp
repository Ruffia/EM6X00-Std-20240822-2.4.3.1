#include "stdafx.h"
#include "TestParameterManager.h"

CTestParameterManager& CTestParameterManager::Instance()
{
	static CTestParameterManager Inst;
	return Inst;
}


CTestParameterManager::CTestParameterManager()
{	
	m_bEnableAlgoAdjust = false;
	m_fAlgoAdjust = 1.f;
	m_dwSleep_AutoFocus = 1000;  //Ä¬ÈÏ1000ºÁÃë
}