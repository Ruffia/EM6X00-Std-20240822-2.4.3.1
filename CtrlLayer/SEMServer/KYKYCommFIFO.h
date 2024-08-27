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
	//�ṩһ����̬ʵ��
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
	CFIFO<KYKYComm_Recv_Package, KYKYComm_Recv_Package&>	m_fifoRecvClientMsg;	// ��ÿһ�����ܰ�����������
	CFIFO<KYKYCommJSON_Recv, KYKYCommJSON_Recv&>	m_fifoSendToHost;		// С��ÿһ��������һ����
};