#pragma once
#include "CustomButton.h"

class CKYKYFeSEMComm;
class CFillButton : public CCustomButton
{
	DECLARE_DYNAMIC(CFillButton)

public:
	CFillButton();
	virtual ~CFillButton();

	void SetStageClient(CKYKYFeSEMComm*	pClient)
	{
		m_pClient = pClient;
	}

	void SetStagePosition(float fX,float fY)
	{
		m_fXStagePos = fX;
		m_fYStagePos = fY;
	}

	void SetPosition(int nRow,int nColumn)
	{
		m_nRow = nRow;
		m_nCol = nColumn;
	}

	//根据Fill框体在对话框中的相对位置，计算其在样品台中对应的位置
	void CalculateStagePosition(const CPoint& ptWaferCenter);

	void ShowPostion();

	//移动自动台
	void MoveStage();

public:
	CKYKYFeSEMComm*	m_pClient;

	float m_fXStagePos;   //Fill对应的样品台X坐标
	float m_fYStagePos;   //Fill对应的样品台Y坐标

	int m_nRow;           //所在晶圆的行
	int m_nCol;           //所在晶圆的列

	int m_nShowID;        //展示在界面上的ID
};
