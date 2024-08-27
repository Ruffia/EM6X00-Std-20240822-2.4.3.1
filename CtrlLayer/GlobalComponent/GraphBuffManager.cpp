#include "stdafx.h"
#include "GraphBuffManager.h"

CGraphBuffMgr& CGraphBuffMgr::Instance()
{
	static CGraphBuffMgr Inst;
	return Inst;
}


void CGraphBuffMgr::Release()
{
	for(int i=0; i<SharedImageBuffCounts; i++ )
	{
		if ( m_pSharedImageBuff[i] != NULL )
		{
			delete [] m_pSharedImageBuff[i];
			m_pSharedImageBuff[i]	= NULL;
		}
	}

	for(int i=0; i<GraphDataBuffGroups; i++ )
	{
		if ( m_pGraphDataBuff[i] != NULL )
		{
			delete [] m_pGraphDataBuff[i];
			m_pGraphDataBuff[i]	= NULL;
		}
	}

	for(int i=0; i<GraphDataBuffGroups; i++ )
	{
		if ( m_pDiffGraphDataBuff[i] != NULL )
		{
			delete [] m_pDiffGraphDataBuff[i];
			m_pDiffGraphDataBuff[i]	= NULL;
		}
	}
}

CGraphBuffMgr::CGraphBuffMgr()
{
	// Shared Image Buff
	for(int i=0; i<SharedImageBuffCounts; i++ )
		m_pSharedImageBuff[i] = NULL;
	// Diag/Logging Graph
	for(int i=0; i<GraphDataBuffGroups; i++ )
		m_pGraphDataBuff[i] = NULL;
	for(int i=0; i<GraphDataBuffGroups; i++ )
		m_pDiffGraphDataBuff[i] = NULL;
}


void CGraphBuffMgr::AllocateBuff() 
{
	///////////////////////////////////////////////////////
	// 分配多个内存段，并通知SemScan和VirtualMCICard
	for(int i=0; i<SharedImageBuffCounts; i++ )
	{
		m_pSharedImageBuff[i] = new unsigned short[LOOP_COUNTS * 40960];
	}
	// 按最快100ms一个数据计算，1秒10个，1分钟600个，1小时36000个，10小时360000个
	for(int i=0; i<GraphDataBuffGroups; i++ )
	{
		m_pGraphDataBuff[i] = new double[GraphDataBuffCounts];
		ZeroMemory( m_pGraphDataBuff[i], sizeof(double)*GraphDataBuffCounts );
	}
	for(int i=0; i<GraphDataBuffGroups; i++ )
	{
		m_pDiffGraphDataBuff[i] = new double[GraphDataBuffCounts];
		ZeroMemory( m_pDiffGraphDataBuff[i], sizeof(double)*GraphDataBuffCounts );
	}
	///////////////////////////////////////////////////////
}