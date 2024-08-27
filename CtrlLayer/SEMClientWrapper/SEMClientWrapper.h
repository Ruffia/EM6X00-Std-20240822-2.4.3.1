// SEMClientWrapper.h : main header file for the SEMCLIENTWRAPPER DLL
//

#if !defined(AFX_SEMCLIENTWRAPPER_H__C59CF2C8_92C3_4254_AF5F_87BED89DF401__INCLUDED_)
#define AFX_SEMCLIENTWRAPPER_H__C59CF2C8_92C3_4254_AF5F_87BED89DF401__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "KYKYSEMComm.h"

/////////////////////////////////////////////////////////////////////////////
// CSEMClientWrapperApp
// See SEMClientWrapper.cpp for the implementation of this class
//
CKYKYSEMComm CommKYKY;

class CSEMClientWrapperApp : public CWinApp
{
public:
	CSEMClientWrapperApp();

	void	KYKYCtrlInit( HWND hWnd );
	bool	KYKYCtrlConnect(int IP1, int IP2, int IP3, int IP4);
	// 断开连接，根据bActive的状态有主动断开和被动断开
	void	KYKYCtrlDisconnect(bool bActive);
	bool	KYKYCtrlGetServerStatus(int* status);	// check if com server is connected
													// 1: the connection is successful; 0: failure
	/////////////////////////////////////////////

	bool	KYKYCtrlGetMagn(float* mgn);			// Get Magnification
	bool	KYKYCtrlSetMagn(float mgn);				// Set Magnification: range (15X ~ 150KX)
	bool	KYKYCtrlGetWD(float* wd);				// working distance in mm
	bool	KYKYCtrlGetBrightness(float* brt);		// Get Brightness
	bool	KYKYCtrlSetBrightness(float brt);		// Set Brightness: range (-100 ~ 100)
	bool	KYKYCtrlGetContrast(float* cst);		// Get Contrast
	bool	KYKYCtrlSetContrast(float cst);			// Set Contrast: range (0.0 ~ 100.0)
	bool	KYKYCtrlGetStig(float* stx, float* sty);// stigmator parameters
	bool	KYKYCtrlSetStig(float stx, float sty);	// stigmator parameters: range X/Y (-2048 ~ 2047)
	bool	KYKYCtrlGetScnRotAngleInit(float* angle);// scan rotate init angle in degree
	bool	KYKYCtrlGetScnRotAngle(float* angle);	// scan rotate angle
	bool	KYKYCtrlSetScnRotAngle(float angle);	// scan rotate angle: range(0 ~ 359)

	bool	KYKYCtrlGetVacuumStatus(int* status);	// check if vacuum ready: 1 is ready; 0 is not ready
	bool	KYKYCtrlGetGunStatus(int* status);		// check current gun status
													// 2- HV is on; 1- HV is off, interlock is on; 0- HV is off, interlock is off
	bool	KYKYCtrlGetAccVoltage(float* volt);		// get acceleration voltage in [kV]
	bool	KYKYCtrlSetAccVoltage(float volt);		// acceleration voltage in [kV]: range (0 ~ 30KV)
	bool	KYKYCtrlGetHVPowerType(int* type);		// 0- HVPower is CPS3603 (can NOT set Beam Current); 1- HVPower is Spellman771 (can set Beam Current)
	bool	KYKYCtrlGetFilament(float* fila);		// reads filament heater current in Amps
	bool	KYKYCtrlSetFilament(float fila);		// set filament heater current in Amps: range (0.0 ~ 2.7A)
	bool	KYKYCtrlGetBeamCurrent(float* curr);	// reads beam current in [uA]
	bool	KYKYCtrlSetBeamCurrent(float curr);	// set beam current in [uA]: range (0 ~ 300uA)

	bool	KYKYCtrlGetStageGratings(int* xSt, int* ySt);// X,Y grating ruler[nm]
	bool	KYKYCtrlGetStageRanges(int* xSt, int* ySt);	// X,Y range [mm]: (50,50) or (80,60) or (150,150)
	bool	KYKYCtrlGetStageStatus(int* status);	// status:	bit0: 1- has auto stage; 0- not has auto stage
													//			bit1: 1- connected; 0- not connected
													//			bit2: 1- is moving; 0- not moving
	bool	KYKYCtrlMoveStageXY(float xSt, float ySt);	// X,Y absolute position [mm]
													// if range is (50,50), X/Y range is (-25 ~ 25)
													// if range is (80,60), X range is (-40 ~ 40), Y range is (-30 ~ 30)
													// if range is (150,150), X/Y range is (-75 ~ 75)

	bool	KYKYCtrlGetStageXY(float* xSt, float* ySt);	// X,Y position [mm]

	bool	KYKYCtrlGetStageRanges5(float* xSt, float* ySt, float* zSt, float* rSt, float* tSt);	// X,Y range [mm]: (50,50) or (80,60) or (150,150)
	bool	KYKYCtrlMoveStage5(float xSt, float ySt, float zSt, float rSt, float tSt);	// X,Y absolute position [mm]
	bool	KYKYCtrlGetStage5(float* xSt, float* ySt, float* zSt, float* rSt, float* tSt);	// X,Y position [mm]

	bool	KYKYCtrlStopStage();					// stop the stage

	bool	KYKYCtrlAutoCstBrt();					// to call Auto BC function in SEM software

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSEMClientWrapperApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSEMClientWrapperApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEMCLIENTWRAPPER_H__C59CF2C8_92C3_4254_AF5F_87BED89DF401__INCLUDED_)
