#pragma once
#include "PanelDataProcessorBase.h"
#include "PanelDataProcessorSwitchCode.h"

//��λ��X
class PanelDataProcessor_ScanShift_X : public PanelDataProcessor_SwitchCode
{
public:
	PanelDataProcessor_ScanShift_X();
	virtual void _SyncPos(long lValue);
};

//��λ��Y
class PanelDataProcessor_ScanShift_Y : public PanelDataProcessor_SwitchCode
{
public:
	PanelDataProcessor_ScanShift_Y();
	virtual void _SyncPos(long lValue);
};


//F1��ݼ�
class PanelDataProcessor_F1 : public IPanelDataProcessor
{
public:
	PanelDataProcessor_F1();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//F2��ݼ�
class PanelDataProcessor_F2 : public IPanelDataProcessor
{
public:
	PanelDataProcessor_F2();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//F3��ݼ�
class PanelDataProcessor_F3 : public IPanelDataProcessor
{
public:
	PanelDataProcessor_F3();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//S1��ݼ�
class PanelDataProcessor_S1 : public IPanelDataProcessor
{
public:
	PanelDataProcessor_S1();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//S2��ݼ�
class PanelDataProcessor_S2 : public IPanelDataProcessor
{
public:
	PanelDataProcessor_S2();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//S3��ݼ�
class PanelDataProcessor_S3 : public IPanelDataProcessor
{
public:
	PanelDataProcessor_S3();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//����ɢX
class PanelDataProcessor_Stigmator_X : public PanelDataProcessor_SwitchCode
{
public:
	PanelDataProcessor_Stigmator_X();
	virtual void _SyncPos(long lValue);
};

//����ɢY
class PanelDataProcessor_Stigmator_Y : public PanelDataProcessor_SwitchCode
{
public:
	PanelDataProcessor_Stigmator_Y();
	virtual void _SyncPos(long lValue);
};


//�����X
class PanelDataProcessor_BeemAlignment_X : public PanelDataProcessor_SwitchCode
{
public:
	PanelDataProcessor_BeemAlignment_X();
	virtual void _SyncPos(long lValue);
};

//�����Y
class PanelDataProcessor_BeemAlignment_Y : public PanelDataProcessor_SwitchCode
{
public:
	PanelDataProcessor_BeemAlignment_Y();
	virtual void _SyncPos(long lValue);
};


//��������бX
class PanelDataProcessor_ElectronBeemTilt_X : public PanelDataProcessor_SwitchCode
{
public:
	PanelDataProcessor_ElectronBeemTilt_X();
	virtual void _SyncPos(long lValue);
};

//��������бY
class PanelDataProcessor_ElectronBeemTilt_Y : public PanelDataProcessor_SwitchCode
{
public:
	PanelDataProcessor_ElectronBeemTilt_Y();
	virtual void _SyncPos(long lValue);
};


//�Ŵ����ֵ�ϸ��
class PanelDataProcessor_MagnificationMode : public IPanelDataProcessor
{
public:
	PanelDataProcessor_MagnificationMode();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};

//�Ŵ�������
class PanelDataProcessor_Magnifier : public IPanelDataProcessor
{
public:
	PanelDataProcessor_Magnifier();
	virtual void _SyncPos(long lValue);
};


//�Զ����ȶԱȶ�
class PanelDataProcessor_AutoBrightContrast : public IPanelDataProcessor
{
public:
	PanelDataProcessor_AutoBrightContrast();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//���ȵ���
class PanelDataProcessor_Brightness : public IPanelDataProcessor
{
public:
	PanelDataProcessor_Brightness();
	virtual void _SyncPos(long lValue);
};


//�Աȶȵ���
class PanelDataProcessor_Contrast : public IPanelDataProcessor
{
public:
	PanelDataProcessor_Contrast();
	virtual void _SyncPos(long lValue);
};


//�۽����ֵܴ�ϸ��
class PanelDataProcessor_FocusMode : public IPanelDataProcessor
{
public:
	PanelDataProcessor_FocusMode();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//�Զ��۽�
class PanelDataProcessor_AutoFocus : public IPanelDataProcessor
{
public:
	PanelDataProcessor_AutoFocus();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//�۽�����
class PanelDataProcessor_Focus : public IPanelDataProcessor
{
public:
	PanelDataProcessor_Focus();
	virtual void _SyncPos(long lValue);
};


//ɨ���ٶ�-
class PanelDataProcessor_ScanSpeedMinus : public IPanelDataProcessor
{
public:
	PanelDataProcessor_ScanSpeedMinus();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//ɨ���ٶ�+
class PanelDataProcessor_ScanSpeedAdd : public IPanelDataProcessor
{
public:
	PanelDataProcessor_ScanSpeedAdd();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};


//����ɢ������й����л�
class PanelDataProcessor_StigApertureModeSwitch : public IPanelDataProcessor
{
public:
	PanelDataProcessor_StigApertureModeSwitch();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};



//��λ�ƣ��۹⾵�����л�
class PanelDataProcessor_ShiftCondeserModeSwitch : public IPanelDataProcessor
{
public:
	PanelDataProcessor_ShiftCondeserModeSwitch();
	virtual void Notify();
	virtual void _SyncPos(long lValue);
};