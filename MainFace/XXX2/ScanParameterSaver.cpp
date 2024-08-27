#include "stdafx.h"
#include "ScanParameterSaver.h"

CScanParameterSaver& CScanParameterSaver::Instance()
{
	static CScanParameterSaver Inst;
	return Inst;
}

CScanParameterSaver::CScanParameterSaver()
{	
	m_nDispPixel = 1024;
	m_fSampleHeight = 0.0;
}