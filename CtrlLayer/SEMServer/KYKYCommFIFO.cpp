#include "StdAfx.h"
#include "KYKYCommFIFO.h"

CommFIFO& CommFIFO::Instance()
{
	static CommFIFO inst;
	return inst;
}

CommFIFO::CommFIFO():m_fifoRecvClientMsg(200),m_fifoSendToHost(500)
{
	m_fifoRecvClientMsg.ClearFIFO();
}

CommFIFO::~CommFIFO()
{
	m_fifoRecvClientMsg.ClearFIFO();
}