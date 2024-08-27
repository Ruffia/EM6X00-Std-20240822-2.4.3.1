//---------------------------
// ExportBSDFunc.h
// 声明欲输出的函数
//---------------------------

#ifndef _EXPORT_BSD_Func_H
#define _EXPORT_BSD_Func_H

#ifdef _cplusplus
extern "C"{
#endif
	
	void	BSD_ShowWnd(HWND hMainWnd, BOOL bShow);
	BOOL	BSD_IsReady();
	void	BSD_SetLanguageID( WORD wID );
	
#ifdef _cplusplus
}
#endif

#endif