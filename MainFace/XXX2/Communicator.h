#pragma once
#include "USB_Functions.h"

#include "TCPServer.h"
#include "Message.h"

//Զ��ͨ��ģ��
class AFX_EXT_CLASS CCommunicator
{
public:
	static CCommunicator& Instance();
	


protected:
	CCommunicator();

public:
	// Remote
	CWinThread*		m_commCmdThd;		// �ͻ��˽��պͷ�����Ϣ���߳�
	CWinThread*		m_commSendImageThd;	// ����ͼ�����ݵ��߳�
	CTCPServer*		m_pcommServer;		// ���Ϳ�������
	CTCPServer*		m_pcommSendImgData;	// ����ͼ������
	KYKYCommMsg_ImgData m_commmsgSendData;// ���͵�ͼ�����ݵĽṹ��
	volatile int	m_nCommRectLen;		// �������͵�����
	BOOL	m_bCommListenStarted;
	BOOL	m_bCommSendImgData;
	double	m_dCommMag, m_dCommWD, m_dCommBrt, m_dCommBrt2, m_dCommCst, m_dCommHV, m_dCommFila, m_dCommBias, m_dCommBeamCurrent;
	int		m_nCommScanRotAngInit, m_nCommScanRotAng;
	int		m_nCommSEMType, m_nCommStatusVac, m_nCommStatusGun, m_nCommHVType;
	CSize	m_szCommStigmator;
	BYTE	m_btCommStageStatus;
	CSize	m_szCommStageGrating, m_szCommStageRange, m_szCommStagePosition;
	KYKYCommMsg_Data2f	m_2fStageRange, m_2fStagePos;
	KYKYCommMsg_Data5f	m_5fStageRange5, m_5fStagePos5;
};
