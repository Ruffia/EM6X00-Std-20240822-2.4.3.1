#pragma once
#include "USB_Functions.h"

//Í¼Ïñ»º´æ¹ÜÀíÆ÷
class AFX_EXT_CLASS CGraphBuffMgr
{
public:
	static CGraphBuffMgr& Instance();

	void Release();

	void AllocateBuff(); 

protected:
	CGraphBuffMgr();


public:
	unsigned short*	m_pSharedImageBuff[SharedImageBuffCounts];
	double*	m_pGraphDataBuff[GraphDataBuffGroups];
	double* m_pDiffGraphDataBuff[GraphDataBuffGroups];
};