#pragma once
#include "USB_Functions.h"
#include "SemCtrl.h"

//�޸�ʱ�䣺  2020-08-20
//�޸��ˣ�  �ܺ�
//�޸�ԭ���ع�����INI�ļ���ȡ���ܴ�CXXX2App �ж���������
//          ����CXXX2App �ĸ��Ӷ�
//���ܣ� INI�ļ�������

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

	BOOL	WriteINI( int nType, int nIndex, double* pdValue1, int nIndexStage, int nIndexXY );	// �������ļ�д����

	void    InitializeColor(char * path);
	void    InitializeLineWidth(char * path);
	void    InitializeFontName(char * path); 
	void    InitializeFontSize(char * path); 
	void    InitializeShowParameter(char * path); 
	void    InitializeFooter(char * path); 
	
protected:
	CCFGFileManager();

};
