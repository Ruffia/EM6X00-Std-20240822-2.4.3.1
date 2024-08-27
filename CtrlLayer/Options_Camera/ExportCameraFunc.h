//---------------------------
// ExportCameraFunc.h
// 声明欲输出的函数
//---------------------------

#ifndef _EXPORT_Camera_Func_H
#define _EXPORT_Camera_Func_H

#ifdef _cplusplus
extern "C"{
#endif

void	ShowCameraDlg(HWND hMainWnd);
void	ShowCameraWnd(HWND hMainWnd, BOOL bShow=TRUE);
void	CCD_SetLanguageID( WORD wID );

#ifdef _cplusplus
}
#endif

#endif