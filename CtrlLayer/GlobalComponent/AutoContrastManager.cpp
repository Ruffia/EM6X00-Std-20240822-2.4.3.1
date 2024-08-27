#include "stdafx.h"
#include "AutoContrastManager.h"

CAutoContrastManager& CAutoContrastManager::Instance()
{
	static CAutoContrastManager Inst;
	return Inst;
}


CAutoContrastManager::CAutoContrastManager()
{	
	fAutoBC = FALSE;
	fMatrix = FALSE;
	fPosGamma = 100.0;
	fim = FALSE;
	nCounth = 1;
	nPosBright= FALSE;
	nPosContrast= FALSE;
	bOptionState = FALSE;
	bStateBC = FALSE;
	bState = FALSE;
	bGrade = FALSE;
	m_nmin = 0;
	m_nmax = 65535;
	colorTable = FALSE;
	
}