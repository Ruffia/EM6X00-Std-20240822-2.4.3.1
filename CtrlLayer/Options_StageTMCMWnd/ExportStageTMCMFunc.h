//---------------------------
// ExportStageTMCMFunc.h
// 声明欲输出的函数
//---------------------------


#ifndef _EXPORT_StageTMCM_Func_H
#define _EXPORT_StageTMCM_Func_H

#ifdef _cplusplus
extern "C"{
#endif

void	StageTMCM_ShowWnd(HWND hMainWnd);
BOOL	StageTMCM_IsReady();
BOOL    StageTMCM_IsMoving();
void	StageTMCM_SetPixelsPerMM( double dX, double dY );
void	StageTMCM_SetMag( double dMag );
void	StageTMCM_MoveToCenter( int nCenterX, int nCenterY, int nX, int nY );
void	StageTMCM_SetLanguageID( WORD wID );
void	StageTMCM_GoOrigin();
void	StageTMCM_GoHome();
void	StageTMCM_OnVentExit();
void    StageTMCM_Move( short nAxis, float fStep );
void    StageTMCM_Move5Axis(float fX, float fY, float fZ, float fR, float fT);
void    StageTMCM_Move2Axis(float fX, float fY);
void	StageTMCM_GetParam( int nIndex, float& f1, float& f2, float& f3, float& f4, float& f5 );
void	StageTMCM_StopTrack();
void	StageTMCM_StopMove();
void	StageTMCM_SetTrackParam( int nSpeedX, int nSpeedY );
void    StageTMCM_SetResolution( CSize szReso );
void	StageTMCM_SetTracklocus1( int nSpeedX, int nSpeedY, int nSpeedZ );
void	StageTMCM_SetTracklocus2( int nSpeedT, int nSpeedZ );
void	StageTMCM_StopTracklocus();


#ifdef _cplusplus
}
#endif

#endif