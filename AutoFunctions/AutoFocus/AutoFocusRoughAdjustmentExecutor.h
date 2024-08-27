#pragma once

//自动聚焦过程中用到的步骤
typedef struct Lens_Sharpness_Step{
	long	nFocus;
	double	dfSharpness;

	AF_Step()
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


public:
	AF_Step		g_AFLookTable[ c_nStepCount ];

	const long		c_nFocusMAX;
	const long		c_nFocusMIN;
	const long		c_nFocusStart;
	const long		c_nFocusStop;
	const long		c_nFocusStep;
};

