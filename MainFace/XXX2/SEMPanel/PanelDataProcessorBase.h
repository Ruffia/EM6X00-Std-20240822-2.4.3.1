#pragma once
#include <afxmt.h>

//�ֽ�����
enum BYTE_SEQ
{
	Header = 0,          //����ͷ
	FunctionCode,        //������
	Data1,               //�����ֽ�1
	Data2,               //�����ֽ�2
	Verification,        //У���ֽ�
	Tail,                //����β
	CmdLen,              //�����,6���ֽ�
};

//�յ�Ӳ������ֵ�п��ܳ���0xFFFF, Ȼ���ִ�0X0000��ʼ
//��ʱ����ֵ�仯������Ӧ�����⴦��
const long lRange = 60000;

//������
//���ã����ݴ��ֿغ�Ӳ���յ�������ͬ����������ϵĽ�����
//ͬʱ��SEM����Ӳ���·�ָ��
class IPanelDataProcessor
{
public:
	IPanelDataProcessor();

	void Attach(HWND hWnd)
	{
		m_hWnd = hWnd;
	}

	void SetDivisor(float fValue)
	{
		m_fDivisor = fValue;
	}
	//�ֿغ�Ӳ���������յ����ڴ�����
	void SetCurrentData(BYTE dataCurrent[CmdLen]);
	//���̷���
	virtual void Notify();
	
protected:
	//����ǰ��ı�ֵ
	virtual long _CalcChangeData();
	//���ݸı�ֵͬ��λ��
	virtual void _SyncPos(long lValue) = 0;

public:
	BYTE m_byteFunctionCode;        //������

protected:
	BYTE m_dataPrevious[CmdLen];    //��һ���յ�������
	BYTE m_dataCurrent[CmdLen];     //�����յ�������
	HWND  m_hWnd;
	float   m_fDivisor;             //�����ȿ�������
	int     m_nExec;                //�߼�ִ�д�������һ�����⣬�����⴦��
};