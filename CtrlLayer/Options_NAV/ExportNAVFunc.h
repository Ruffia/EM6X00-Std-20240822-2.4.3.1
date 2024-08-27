//---------------------------
// ExportNAVFunc.h
// 声明欲输出的函数
//---------------------------


#ifndef _EXPORT_NAV_Func_H
#define _EXPORT_NAV_Func_H

#ifdef _cplusplus
extern "C"{
#endif

//CBasicDemoDlg* InitBasicDemoDlg(CWnd* pParent);
//void SaveCameraDataToPath(CCameraSeting *set);

	void	NAV_ShowWnd(HWND hMainWnd, BOOL bShow=TRUE);
	void	NAV_UpdateXYPos( float fX, float fY );
	BOOL	NAV_IsReady();
	void	NAV_SetLanguageID( WORD wID );
	void	NAV_SetV1Status( BOOL bStatus);
	void	NAV_UpdateStatus( int nIndex, BOOL bStatus );

#ifdef _cplusplus
}
#endif

#endif
