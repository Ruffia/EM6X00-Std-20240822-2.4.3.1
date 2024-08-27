
#ifndef __Z80Message_H
#define __Z80Message_H

//====== PC -> Z80A MessageID ==============================

//#define Z80A_										(BYTE)0
#define Z80A_ScanFunction_SpotScan					(BYTE)1
#define Z80A_ScanFunction_LineScan					(BYTE)2
#define Z80A_ScanFunction_AreaScan					(BYTE)3
#define Z80A_ScanFunction_DualMagnification			(BYTE)4
#define Z80A_ScanFunction_SplitScan					(BYTE)5
#define Z80A_ScanFunction_RasterScan512				(BYTE)6
#define Z80A_ScanFunction_RasterScan1024			(BYTE)7
//#define Z80A_										(BYTE)8
//#define Z80A_										(BYTE)9
//#define Z80A_										(BYTE)10
#define Z80A_Inquirer								(BYTE)11
#define Z80A_DigitalPhoto							(BYTE)12
#define Z80A_AnalogPhoto							(BYTE)13
#define Z80A_HV										(BYTE)14
#define Z80A_DFocus									(BYTE)15
#define Z80A_DFinalLens								(BYTE)16
#define Z80A_ERotate								(BYTE)17
#define Z80A_YAdjust								(BYTE)18
#define Z80A_DCondenserLens							(BYTE)19
#define Z80A_RotationScan							(BYTE)20
#define Z80A_COMMTEST								(BYTE)21

//#define Z80A_										(BYTE)

//==========================================================


//====== PC -> Z80B MessageID ==============================

#define Z80B_ObjectiveLens							(BYTE)0
#define Z80B_CondenserLens							(BYTE)1
#define Z80B_StigmatorX								(BYTE)2
#define Z80B_StigmatorY								(BYTE)3
#define Z80B_Contrast								(BYTE)4
#define Z80B_BeamAlignmentY							(BYTE)5
#define Z80B_BeamAlignmentX							(BYTE)6
#define Z80B_Brightness								(BYTE)7
#define Z80B_Amplifier								(BYTE)8
#define Z80B_Signal2Contrast						(BYTE)9
#define Z80B_Signal2Brightness						(BYTE)10
#define Z80B_EMoveX									(BYTE)11
#define Z80B_EMoveY									(BYTE)12
#define Z80B_ScanShiftX								Z80B_EMoveX
#define Z80B_ScanShiftY								Z80B_EMoveY
#define Z80B_FilamentI								(BYTE)13
#define Z80B_FilamentV								(BYTE)14	// 3200 废止
#define Z80B_AutoFinalLens							(BYTE)15	// 3200 废止
#define Z80B_Inquirer								(BYTE)16
#define Z80B_AutoStigmator							(BYTE)17	// 3200 废止

// 1999-3-8 Lian Ning
//#define	Z80B_SignalConverter						(BYTE)18 // SE3 -> Pc10B
//#define	Z80B_SignalProcesser						(BYTE)19 // SE4
//#define	Z80B_AutoFilament							(BYTE)21

#define	Z80B_SignalProcesser						(BYTE)18	// 3200 废止 // SE4 -> Pc10B
#define	Z80B_AutoFilament							(BYTE)19	// 3200 废止

#define Z80B_FinalLensReverse						(BYTE)20
#define Z80B_CondenserLensReverse					(BYTE)21
#define Z80B_SACP									(BYTE)22
#define Z80B_AuxiliaryStigmation					(BYTE)23
#define Z80B_RobinsonDetectorSwitch					(BYTE)24

#define Z80B_Channel_A								(BYTE)25	// 3200 废止
#define Z80B_Channel_B								(BYTE)26	// 3200 废止
#define Z80B_Channel_C								(BYTE)27	// 3200 废止
#define Z80B_Channel_D								(BYTE)28	// 3200 废止
#define Z80B_Channel_E								(BYTE)29	// 3200 废止
#define Z80B_HeatingFilamentElectricity				(BYTE)30	// 3200 废止

// 2004-1-15 Leo 3200
#define Z80B_ImageInvert							(BYTE)25	// 3200 图像信号翻转
#define Z80B_PC10_SE6								(BYTE)14	//31	// 3200 图像处理与通道选择1
#define Z80B_PC10_SE1								(BYTE)15	//32	// 3200 通道选择2

// 2007.10.17
#define	Z80B_BseBySeDetectorSwitch					(BYTE)32	//使用二次电子探测器看背散射像开关
//===========================================================
// OSE: Optial Shaft Encoder
#define Z80B_OSE_SetupParam							(BYTE)31 

typedef struct tagOSEncoderParam{
	BYTE	bFunction;
	BYTE	bStep;
	BYTE	bNotifyTimer;
	BYTE	bPulseDivier;
	WORD	nCtrlMin;
	WORD	nCtrlMax;
}OSEncoderParam;
#define OSE_Fn_NULL						(BYTE)255
#define OSE_Fn_ObjectiveLens			Z80B_ObjectiveLens
#define OSE_Fn_CondenserLens			Z80B_CondenserLens
#define OSE_Fn_StigmatorX				Z80B_StigmatorX
#define OSE_Fn_StigmatorY				Z80B_StigmatorY
#define OSE_Fn_Contrast					Z80B_Contrast
#define OSE_Fn_BeamAlignmentY			Z80B_BeamAlignmentY
#define OSE_Fn_BeamAlignmentX			Z80B_BeamAlignmentX
#define OSE_Fn_Brightness				Z80B_Brightness
#define OSE_Fn_ScanShiftX					Z80B_ScanShiftX
#define OSE_Fn_ScanShiftY					Z80B_ScanShiftY
//===========================================================

//#define Z80B_										(BYTE)

//==========================================================

//====== Z80B -> PC MessageID ==============================

#define	H_Z80B_Flag									(BYTE)16

#define		H_Z80B_Flag_HV				(BYTE)0x01
#define		H_Z80B_Flag_Filament		(BYTE)0x02
#define		H_Z80B_Enable_HV			(BYTE)0x04
//#define	H_Z80B_Flag_Vacuum			(BYTE)0x04
#define		H_Z80B_Flag_BUSY			(BYTE)0x08		//2003-11-7 Leo

//#define		H_Z80B_Flag_HOLD			(BYTE)0x10	//2003-11-7 Leo
//#define		H_Z80B_Flag_CST				(BYTE)0x20	//2003-11-7 Leo
#define		H_Z80B_Flag_CST				(BYTE)0x10		//2003-11-7 Leo
#define		H_Z80B_Flag_HOLD			(BYTE)0x20		//2003-11-7 Leo
#define		H_Z80B_Flag_Bit6			(BYTE)0x40		//2003-11-7 Leo
#define		H_Z80B_Flag_Vacuum			(BYTE)0x80
 

#define	H_Z80B_GunI									(BYTE)8
#define	H_Z80B_GunISaturated						(BYTE)13
#define	H_Z80B_AutoFocus							(BYTE)15
#define	H_Z80B_AutoStigmator						(BYTE)17

// 1999-3-8 Lian Ning
//#define	H_Z80B_SignalProcesser						(BYTE)19
//#define	H_Z80B_Reset								(BYTE)20
//#define	H_Z80B_AutoFilament							(BYTE)21

#define	H_Z80B_SignalProcesser						(BYTE)18	// SE4 -> Pc10B
#define	H_Z80B_AutoFilament							(BYTE)19

//===========================================================
// OSE: Optial Shaft Encoder
#define H_Z80B_OSE_Ctrl								(BYTE)31 // OSE: Optial Shaft Encoder
typedef struct tagOSEncoderCtrl{
	BYTE	bCtrlFunction;
	union{
		BYTE	bData;
		WORD	nData;
	}Ctrl;
}OSEncoderCtrl;
//===========================================================

#define H_Z80B_FilamentHeatingCurrent				(BYTE)32

#define	H_Z80B_Reset								(BYTE)40

/*
#define	Z80B_SignalProcesser_SE0					(BYTE)18
#define	Z80B_SignalProcesser_SE1					(BYTE)19
#define	Z80B_SignalProcesser_SE2					(BYTE)20
#define	Z80B_SignalProcesser_SE3					(BYTE)21
#define	Z80B_SignalProcesser_SE4					(BYTE)22
#define	Z80B_SignalProcesser_SE5					(BYTE)23
#define	Z80B_SignalProcesser_SE6					(BYTE)24
#define	Z80B_SignalProcesser_SE7					(BYTE)25
*/


//====== Z80A -> PC MessageID ==============================

#define	H_Z80A_Reset								(BYTE)40

//==========================================================

//====== Z80A/Z80B -> PC MessageID =========================
// 2004-5-19_处理Z80接收到未识别的消息
#define	H_Z80_UnrecognizedMsg						(BYTE)41

// 2004-5-20_处理Z80接收到校验错误的SEM消息
#define	H_Z80_CheckedErrMsg							(BYTE)42


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Cypress Semiconductor microcontroller on MCB board
// USB message ID
//===========================================================
// OSE: Optial Shaft Encoder
#define USB_OSE_SetupParam							(BYTE)31 
/*
typedef struct tagOSEncoderParam{
	BYTE	bFunction;
	BYTE	bStep;
	BYTE	bNotifyTimer;
	BYTE	bPulseDivier;
	WORD	nCtrlMin;
	WORD	nCtrlMax;
}OSEncoderParam;
#define OSE_Fn_NULL						(BYTE)255
#define OSE_Fn_ObjectiveLens			Z80B_ObjectiveLens
#define OSE_Fn_CondenserLens			Z80B_CondenserLens
#define OSE_Fn_StigmatorX				Z80B_StigmatorX
#define OSE_Fn_StigmatorY				Z80B_StigmatorY
#define OSE_Fn_Contrast					Z80B_Contrast
#define OSE_Fn_BeamAlignmentY			Z80B_BeamAlignmentY
#define OSE_Fn_BeamAlignmentX			Z80B_BeamAlignmentX
#define OSE_Fn_Brightness				Z80B_Brightness
#define OSE_Fn_ScanShiftX					Z80B_ScanShiftX
#define OSE_Fn_ScanShiftY					Z80B_ScanShiftY
//===========================================================
*/
//===========================================================
// OSE: Optial Shaft Encoder
#define H_USB_OSE_Ctrl								(BYTE)31 // OSE: Optial Shaft Encoder
/*
typedef struct tagOSEncoderCtrl{
	BYTE	bCtrlFunction;
	union{
		BYTE	bData;
		WORD	nData;
	}Ctrl;
}OSEncoderCtrl;*/
//===========================================================
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif
