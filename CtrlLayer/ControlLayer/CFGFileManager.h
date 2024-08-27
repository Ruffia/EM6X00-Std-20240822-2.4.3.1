#pragma once
#include "USB_Functions.h"
#include "SemCtrl.h"

//修改时间：  2020-08-20
//修改人：  周宏
//修改原因：重构，将INI文件读取功能从CXXX2App 中独立出来，
//          减少CXXX2App 的复杂度
//功能： INI文件管理器

class AFX_EXT_CLASS CCFGFileManager  
{

public:
	static CCFGFileManager& Instance();

	//void	CFGfile_InitParams();
	void	CFGfile_InitChangingParams( char *path,DL_Sum_Param& DL);
	void	CFGfile_InitLoggingParams( char *path, DL_Sum_Param& DL,double* &testData);
	void	CFGfile_InitSelectedADC(char* path,DL_Sum_Param& DL);
	//void	CFGfile_InitHVParams(char *path);
	//void	CFGfile_InitSCNParams(char *path, CSemCtrl& SemCtrl);
	void    CFGfile_InitSEMCtrlParams( int nPosX, int nPosY ); 
	void	CFGfile_InitLNSParams(char *path ,int&  nDFforFFBase);
	void	CFGfile_InitMCIParams(char* path );
	void	CFGfile_InitAutoTestParams(char *path, AutoTest_Param& AT);
	//void	CFGfile_InitScanParams(char *path);
	void	CFGfile_InitFastParams(char *path, FastChanges& Fast,const DWORD dwHVtype);
	void    CFGFile_InitSemCoreParams( char * path ,const CString& strLogo ); 
	void    CFGFile_InitNetParams( char *path, Net_Param& Net); 

	BOOL	WriteINI( int nType, int nIndex, double* pdValue1, int nIndexStage, int nIndexXY );	// 向配置文件写参数

	void    InitializeColor(char * path);
	void    InitializeLineWidth(char * path);
	void    InitializeFontName(char * path); 
	void    InitializeFontSize(char * path); 
	void    InitializeShowParameter(char * path); 
	void    InitializeFooter(char * path); 
	
protected:
	CCFGFileManager();

};
