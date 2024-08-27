// SEMClientWrapper.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "SEMClientWrapper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CSEMClientWrapperApp

BEGIN_MESSAGE_MAP(CSEMClientWrapperApp, CWinApp)
	//{{AFX_MSG_MAP(CSEMClientWrapperApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSEMClientWrapperApp construction

CSEMClientWrapperApp::CSEMClientWrapperApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSEMClientWrapperApp object

CSEMClientWrapperApp theApp;

void CSEMClientWrapperApp::KYKYCtrlInit(HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CommKYKY.KYKYSEM_Init(hWnd);
}

bool CSEMClientWrapperApp::KYKYCtrlConnect(int IP1, int IP2, int IP3, int IP4)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_Connect(IP1, IP2, IP3, IP4);
}

void CSEMClientWrapperApp::KYKYCtrlDisconnect(bool bActive)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CommKYKY.KYKYSEM_Disconnect(bActive);
}

bool CSEMClientWrapperApp::KYKYCtrlGetServerStatus(int* status)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetServerStatus(status);
}

bool CSEMClientWrapperApp::KYKYCtrlGetMagn(float* mgn)	
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetMagn(mgn);
}

bool CSEMClientWrapperApp::KYKYCtrlSetMagn(float mgn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_SetMagn(mgn);
}

bool CSEMClientWrapperApp::KYKYCtrlGetWD(float* wd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetWD(wd);
}

bool CSEMClientWrapperApp::KYKYCtrlGetBrightness(float* brt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetBrightness(brt);
}

bool CSEMClientWrapperApp::KYKYCtrlSetBrightness(float brt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_SetBrightness(brt);
}

bool CSEMClientWrapperApp::KYKYCtrlGetContrast(float* cst)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetContrast(cst);
}

bool CSEMClientWrapperApp::KYKYCtrlSetContrast(float cst)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_SetContrast(cst);
}

bool CSEMClientWrapperApp::KYKYCtrlGetStig(float* stx, float* sty)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetStig(stx, sty);
}

bool CSEMClientWrapperApp::KYKYCtrlSetStig(float stx, float sty)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_SetStig(stx, sty);
}

bool CSEMClientWrapperApp::KYKYCtrlGetScnRotAngleInit(float* angle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetScnRotAngleInit(angle);
}

bool CSEMClientWrapperApp::KYKYCtrlGetScnRotAngle(float* angle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetScnRotAngle(angle);
}

bool CSEMClientWrapperApp::KYKYCtrlSetScnRotAngle(float angle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_SetScnRotAngle(angle);
}

bool CSEMClientWrapperApp::KYKYCtrlGetVacuumStatus(int* status)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetVacuumStatus(status);
}

bool CSEMClientWrapperApp::KYKYCtrlGetGunStatus(int* status)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetGunStatus(status);
}

bool CSEMClientWrapperApp::KYKYCtrlGetHVPowerType(int* type)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetHVPowerType(type);
}

bool CSEMClientWrapperApp::KYKYCtrlGetAccVoltage(float* volt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetAccVoltage(volt);
}

bool CSEMClientWrapperApp::KYKYCtrlSetAccVoltage(float volt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_SetAccVoltage(volt);
}

bool CSEMClientWrapperApp::KYKYCtrlGetFilament(float* fila)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetFilament(fila);
}

bool CSEMClientWrapperApp::KYKYCtrlSetFilament(float fila)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_SetFilament(fila);
}

bool CSEMClientWrapperApp::KYKYCtrlGetBeamCurrent(float* curr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetBeamCurrent(curr);
}

bool CSEMClientWrapperApp::KYKYCtrlSetBeamCurrent(float curr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_SetBeamCurrent(curr);
}

bool CSEMClientWrapperApp::KYKYCtrlGetStageGratings(int* xSt, int* ySt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetStageGratings(xSt, ySt);
}

bool CSEMClientWrapperApp::KYKYCtrlGetStageRanges(int* xSt, int* ySt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetStageRanges(xSt, ySt);
}

bool CSEMClientWrapperApp::KYKYCtrlGetStageStatus(int* status)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetStageStatus(status);
}

bool CSEMClientWrapperApp::KYKYCtrlMoveStageXY(float xSt, float ySt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_MoveStageXY(xSt, ySt);
}

bool CSEMClientWrapperApp::KYKYCtrlGetStageXY(float* xSt, float* ySt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetStageXY(xSt, ySt);
}

bool CSEMClientWrapperApp::KYKYCtrlGetStageRanges5(float* xSt, float* ySt, float* zSt, float* rSt, float* tSt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetStageRanges5(xSt, ySt, zSt, rSt, tSt);
}

bool CSEMClientWrapperApp::KYKYCtrlMoveStage5(float xSt, float ySt, float zSt, float rSt, float tSt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_MoveStage5(xSt, ySt, zSt, rSt, tSt);
}

bool CSEMClientWrapperApp::KYKYCtrlGetStage5(float* xSt, float* ySt, float* zSt, float* rSt, float* tSt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_GetStage5(xSt, ySt, zSt, rSt, tSt);
}

bool CSEMClientWrapperApp::KYKYCtrlStopStage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CommKYKY.KYKYSEM_StopStage();
}
