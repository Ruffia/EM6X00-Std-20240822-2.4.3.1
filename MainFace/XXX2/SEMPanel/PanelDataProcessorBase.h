#pragma once
#include <afxmt.h>

//字节序列
enum BYTE_SEQ
{
	Header = 0,          //命令头
	FunctionCode,        //功能码
	Data1,               //数据字节1
	Data2,               //数据字节2
	Verification,        //校验字节
	Tail,                //命令尾
	CmdLen,              //命令长度,6个字节
};

//收到硬件的数值有可能超过0xFFFF, 然后又从0X0000开始
//此时的数值变化量计算应该特殊处理
const long lRange = 60000;

//抽象类
//作用：根据从手控盒硬件收到的数据同步软件界面上的进度条
//同时向SEM控制硬件下发指令
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
	//手控盒硬件的数据收到后在此设置
	void SetCurrentData(BYTE dataCurrent[CmdLen]);
	//流程方法
	virtual void Notify();
	
protected:
	//计算前后改变值
	virtual long _CalcChangeData();
	//根据改变值同步位置
	virtual void _SyncPos(long lValue) = 0;

public:
	BYTE m_byteFunctionCode;        //功能码

protected:
	BYTE m_dataPrevious[CmdLen];    //上一次收到的数据
	BYTE m_dataCurrent[CmdLen];     //本次收到的数据
	HWND  m_hWnd;
	float   m_fDivisor;             //灵敏度控制因子
	int     m_nExec;                //逻辑执行次数，第一次特殊，作特殊处理
};