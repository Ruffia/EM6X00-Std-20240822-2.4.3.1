#pragma once

//�Ŵ�������ģʽ
enum MagnificationControlMode
{
	MagnificationMode_Large = 0,   //�ֵ�
	MagnificationMode_Small = 1,   //ϸ��
};


//�۽�ģʽ
enum FocusControlMode
{
	FocusControlMode_Large = 0,   //�ֵ�-----����Ӧ�ڽ����ϵ�ϸ��
	FocusControlMode_Small = 1,   //ϸ��-----����Ӧ�ڽ����ϵ�΢��
	FocusControlMode_ScreenFocus = 2,   //��Ļ�۽�����
};


//�����л�ģʽ
enum StigApertureMode
{
	StigmatorMode = 0x01,       //����ɢģʽ
	ApertureMode,               //����ģʽ(�ֿغ�����ϵı��)
	FilamentMode,               //��˿����ģʽ    
};

//״̬
enum ShiftCondeserSwitchStatus
{
	ShiftCondeserSwitchNormal = 0,
	ShiftCondeserSwitching,
};

//״̬
enum StigmatorApertureSwitchStatus
{
	StigmatorApertureSwitchNormal = 0,
	StigmatorApertureSwitching,
};

//�ֿغй���ģʽ������
class CPanelModeMgr
{
protected:
	CPanelModeMgr();
	~CPanelModeMgr(){};

public:
	static CPanelModeMgr& Instance();
	void RestoreFocusMode()
	{
		FocusMode = FocusModeBK;
	}

public:
	MagnificationControlMode MagMode;
	FocusControlMode         FocusMode;                                 //��ǰ�۽�ģʽ
	FocusControlMode         FocusModeBK;                               //֮ǰ�ľ۽�ģʽ(��ԭ��Large��Smalll����ģʽ�Ļ����ϣ�������ScrrenMode),
	                                                                    //����ScreenMode ģʽ���˳�ʱ��ϣ����ԭ��ԭ��ģʽ        
	int  m_nShiftCondeserSwitchMode;                                    //��λ�ƣ��۹⾵�л�ģʽ(ȡֵ1��2)
	int  m_nStigmatorApertureSwitchMode;                                //����ɢ��������л�ģʽ(ȡֵ1��2��3)
	int  m_nScanSpeedMode;                                              //ɨ���ٶ�ģʽ(ȡֵ1��2��3��4)
	ShiftCondeserSwitchStatus      m_eShiftCondeserSwitchSatusX;   
	ShiftCondeserSwitchStatus      m_eShiftCondeserSwitchSatusY;  
	StigmatorApertureSwitchStatus  m_eStigmatorSwitchSatusX;             //����ɢX
	StigmatorApertureSwitchStatus  m_eStigmatorSwitchSatusY;             //����ɢY
	StigmatorApertureSwitchStatus  m_eApertureSwitchSatusX;              //�����X
	StigmatorApertureSwitchStatus  m_eApertureSwitchSatusY;              //�����Y
	StigmatorApertureSwitchStatus  m_eElectronBeemTiltSwitchSatusX;      //��������бX
	StigmatorApertureSwitchStatus  m_eElectronBeemTiltSwitchSatusY;      //��������бY
};