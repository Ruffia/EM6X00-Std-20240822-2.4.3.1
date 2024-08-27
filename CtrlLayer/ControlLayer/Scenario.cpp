#include "StdAfx.h"
#include "Scenario.h"

CScenario::CScenario()
{
	Reset();
}

void CScenario::Reset() 
{
	for (int i = 0; i < 256; i++)
	{
		szSampleName[i] = 0;
	}

	for (int i = 0; i < 64; i++)
	{
		szCreateTime[i] = 0;
	}

	for (int i = 0; i < 64; i++)
	{
		szAuthorName[i] = 0;
	}

	for (int i = 0; i < 256; i++)
	{
		szDescription[i] = 0;
	}

	HVType = 0;                 //高压类型CPS或者Spellman
	HV = 0.0;                   //高压值
	Bias = 0.0;                 //偏压或者束流
	FilamentCurrent = 0.0;      //灯丝电流
	Contrast = 0.0;             //对比度
	Brightness = 0.0;           //亮度
	CondenserLens = 0.0;        //聚光镜
	ObjectiveLens = 0.0;        //物镜
	Magnifier = 0.0;            //放大倍数	
	BeemAlignmentX = 0.0;       //电对中X
	BeemAlignmentY = 0.0;       //电对中Y
	StigmatorX = 0.0;           //消像散X
	StigmatorY = 0.0;           //消像散Y
}