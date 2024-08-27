#pragma once
#include "USB_Functions.h"

#include "TCPServer.h"
#include "Message.h"

//远程通信模块
class AFX_EXT_CLASS CCommunicator
{
public:
	static CCommunicator& Instance();
	


protected:
	CCommunicator();

public:
	// Remote
	CWinThread*		m_commCmdThd;		// 客户端接收和发送消息的线程
	CWinThread*		m_commSendImageThd;	// 发送图像数据的线程
	CTCPServer*		m_pcommServer;		// 发送控制命令
	CTCPServer*		m_pcommSendImgData;	// 发送图像数据
	KYKYCommMsg_ImgData m_commmsgSendData;// 发送的图像数据的结构体
	volatile int	m_nCommRectLen;		// 批量发送的行数
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
