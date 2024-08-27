
#if !defined(AFX_SemDataType_H__INCLUDED_)
#define AFX_SemDataType_H__INCLUDED_

typedef struct tagSemCtrlFigures{
    VARIANT varMin;
    VARIANT varMax;
	VARIANT varPos;
} SemCtrlFigures;
typedef SemCtrlFigures FAR *LPSemCtrlFigures;

// Î´Íê´ýÐø
enum SemControlPanelClassID{
	SemControlPanelClass_NULL,
	SemControlPanelClass_HV,
	SemControlPanelClass_ScanShift,
	SemControlPanelClass_BeemAlignment,
	SemControlPanelClass_Brightness,
	SemControlPanelClass_Contrast,
	SemControlPanelClass_Stigmator,
	SemControlPanelClass_ObjectiveLens,
	SemControlPanelClass_CondenserLens,
	SemControlPanelClass_FilamentCurrent,
	SemControlPanelClass_GunCurrent,
	SemControlPanelClass_Magnifier,
	SemControlPanelClass_WorkDistance,
	SemControlPanelClass_ObjectiveLensReverse,
	SemControlPanelClass_CondenserLensReverse,
	SemControlPanelClass_AuxiliaryStigmation,
	SemControlPanelClass_DCondenserLens,
	SemControlPanelClass_ScanRotate,
	SemControlPanelClass_SACP,
	SemControlPanelClass_DFocus,
	SemControlPanelClass_DFocusSwitch,
	SemControlPanelClass_DObjectiveLens,
	SemControlPanelClass_DObjectiveLensSwitch,
	SemControlPanelClass_RobinsonDetectorSwitch,
	SemControlPanelClass_Signal2Brightness,
	SemControlPanelClass_Signal2Contrast,
	SemControlPanelClass_YAdjust,
	SemControlPanelClass_ImageReverse,
	SemControlPanelClass_GunCurrentAccelerate,
	SemControlPanelClass_HVAccelerate,
	SemControlPanelClass_Channel2AB,
	SemControlPanelClass_Channel2C,
	SemControlPanelClass_Channel2D,
	SemControlPanelClass_Channel2E,
	SemControlPanelClass_BseBySeDetectorSwitch,
	// CPS 3601
	SemControlPanelClass_CPS_HV,
	SemControlPanelClass_CPS_Bias,
	SemControlPanelClass_CPS_FilamentCurrent,
	// CPS 3601
	// Spellman EBM30N12/771
	SemControlPanelClass_SP_HV,
	SemControlPanelClass_SP_Bias,
	SemControlPanelClass_SP_Heater,
	SemControlPanelClass_SP_PMT,
	// Spellman EBM30N12/771
	SemControlPanelClass_ImageAlgo_AdjustmentParameter
};

const DWORD	FlagCtrlX = (DWORD)0x01;
const DWORD	FlagCtrlY = (DWORD)0x02;

#endif //AFX_SemDataType_H__INCLUDED_