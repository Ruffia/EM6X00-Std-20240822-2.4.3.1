#pragma once

//自动聚焦过程中用到的步骤
typedef struct Lens_Sharpness_Step{
	long	nFocus;
	double	dfSharpness;

	Lens_Sharpness_Step()
	{
		nFocus = 0;
		dfSharpness = 0;
	}
}AF_Step;

#define	c_nStepCount  20//27					// 28


//自动聚焦阶段执行器
class AFX_EXT_CLASS CAutoFocusPhaseExecutor 
{
public:
	CAutoFocusPhaseExecutor();
	void Initialize();


public:
	AF_Step		g_AFLookTable[ c_nStepCount ];

	const long		c_nFocusMAX;
	const long		c_nFocusMIN;

protected:
	long		m_nFocusStart;
	long		m_nFocusStop;
	long		m_nFocusStep;
};

