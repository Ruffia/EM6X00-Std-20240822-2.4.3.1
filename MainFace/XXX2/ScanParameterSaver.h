#pragma once
#include "USB_Functions.h"


class CScanParameterSaver 
{
public:
	static CScanParameterSaver& Instance();

protected:
	CScanParameterSaver();

public:
	int			m_nAutoFuction;

	WORD		m_nMagnifierSaved;
	double		m_dblMagnifierSaved;
	long		m_lObjectiveLensSaved;
	long		m_lLastlObjectiveLens;
	//ZYX add 090407
	double		m_dbLastlMagnifierSaved;

	long		m_lStigmatorXSaved;
	long		m_lStigmatorYSaved;

	long		m_lBrightnessSaved;
	long		m_lContrastSaved;

	long		m_lFilaSaved;
	BOOL		m_bAutoFilaPause;

	// 16.04.26 为诊断功能添加参数保存
	long		m_lCondensorLensSaved;
	long		m_lAlignXSaved;
	long		m_lAlignYSaved;

	// 19.06.19 扫描区域显示像素
	int			m_nDispPixel;
	//2022.11.2 自动聚焦参数
	float		m_fSampleHeight;
};
