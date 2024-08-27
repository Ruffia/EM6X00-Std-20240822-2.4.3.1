#pragma once
#include "../GlobalComponent/StatusDetector.h"
#include "../GlobalComponent/Serial.h"
#include "../../Common/Include/FEG/PLCvacBrd.h"

class AFX_EXT_CLASS CPLCvacBrdStatusDetector : public CStatusDetector
{
protected:
	CPLCvacBrdStatusDetector(void);
	~CPLCvacBrdStatusDetector(void){};

public:
	static CPLCvacBrdStatusDetector& Instance();

	BOOL	Init( HWND hWnd, int nGaugeType = 0 );
	BOOL	SetCmd( BYTE bFunc, BYTE bOpr );
	BOOL	SetThresholds( double dVacPre, double dVacOK, double dVacNotOK );

	virtual	BOOL Start();

	virtual void Release();
	virtual DWORD DoJob();

protected:
	void	RecordInit();
	void	Record( LPCTSTR lpszRcd );

	unsigned short	CRCcheck(BYTE* DataCode, int nLen);
	int		ReadFrom( char FAR * buf, int len );
	int		WriteTo( const char FAR * buf, int len );
	void	Close();
	int		SendCmd( BYTE byteOpc, BYTE byteOpr );
	int		SendThresholds( BYTE byteOpc, WORD wOpr );

	int		Recv_VACstatus( BYTE* data );
	int		Recv_VACvalve( BYTE* data );
	int		Recv_VACthreshold( BYTE* data );
	int		Recv_Status( BYTE* data );
	int		Recv_SwitchMode( BYTE* data );
	int		Recv_PumpVent_SEM( BYTE* data );
	int		Recv_PumpVent_FEM( BYTE* data );
	int		Recv_V1( BYTE* data );
	int		Recv_Manual_V1( BYTE* data );
	int		Recv_Manual_V2( BYTE* data );
	int		Recv_Manual_V3( BYTE* data );
	int		Recv_Manual_V4( BYTE* data );
	int		Recv_Manual_TMP( BYTE* data );
	int		Recv_CCD( BYTE* data );
	int		Recv_WarnReset( BYTE* data );
	int		Recv_NAV( BYTE* data );
	int		Recv_BeamDetect( BYTE* data );
	int		Recv_BSE( BYTE* data );

	int		GetFrameLength( BYTE bFunc );
	int		ConvertReceiveData( int nLen, char* data );
	int		CommTest( int nSleep );
	DWORD	Connect_Init();

protected:
	HWND	m_hWnd;
	int		m_nGaugeType;

	SOCKET	m_sockPLCvacBrd;
	struct	sockaddr_in local_addr,remote_addr;

	HANDLE	m_hFilePLCvacBrd;
	int		m_nLoopsPLCvacBrd;

	Cfg_PLCvacBrd		m_cfgPLCvacBrd;
	Status_PLCvacBrd	m_StatusPLCvacBrd;
};
