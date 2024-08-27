#pragma once
#include <string>
using namespace std;
#include <cfifo.h>

typedef struct tagKYKYComm_Recv_Package
{
	char szRecv[768];
} KYKYComm_Recv_Package;

typedef struct tagKYKYCommJSON_Recv
{
	std::string	strRecv;
} KYKYCommJSON_Recv;


class  CommFIFO
{
public:
	//提供一个静态实例
	static CommFIFO& Instance();
	CFIFO<KYKYComm_Recv_Package, KYKYComm_Recv_Package&>& GetRecvClientMsgFIFO()
	{
		return m_fifoRecvClientMsg;
	}

	CFIFO<KYKYCommJSON_Recv, KYKYCommJSON_Recv&>& GetSendToHostFIFO()
	{
		return m_fifoSendToHost;
	}

	CommFIFO();
	virtual ~CommFIFO();

private:
	CFIFO<KYKYComm_Recv_Package, KYKYComm_Recv_Package&>	m_fifoRecvClientMsg;	// 大：每一个可能包含多条命令
	CFIFO<KYKYCommJSON_Recv, KYKYCommJSON_Recv&>	m_fifoSendToHost;		// 小：每一个包含单一命令
};