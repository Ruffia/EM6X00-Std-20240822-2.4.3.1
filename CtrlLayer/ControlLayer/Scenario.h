#pragma once

class AFX_EXT_CLASS CScenario
{
public:
	CScenario();
    void Reset(); 

public:
	char szSampleName[256];      //样品名称
	char szCreateTime[64];       //场景创建时间
	char szAuthorName[64];       //作者名称
	char szDescription[256];     //场景描述
	int  HVType;                 //高压类型CPS或者Spellman
	double HV;                   //高压值
	double Bias;                 //偏压或者束流
	double FilamentCurrent;      //灯丝电流
	double Contrast;             //对比度
	double Brightness;           //亮度
	double CondenserLens;        //聚光镜
	double ObjectiveLens;        //物镜
	double Magnifier;            //放大倍数	
	double BeemAlignmentX;       //电对中X
	double BeemAlignmentY;       //电对中Y
	double StigmatorX;           //消像散X
	double StigmatorY;           //消像散Y
};