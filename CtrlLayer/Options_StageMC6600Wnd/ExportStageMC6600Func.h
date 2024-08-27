//---------------------------
// ExportStageMC6600Func.h
// 声明欲输出的函数
//---------------------------


#ifndef _EXPORT_StageMC6600_Func_H
#define _EXPORT_StageMC6600_Func_H

#ifdef _cplusplus
extern "C"{
#endif

void	StageMC6600_ShowWnd(HWND hMainWnd);
BOOL	StageMC6600_IsReady();
BOOL    StageMC6600_IsMoving();
void	StageMC6600_SetPixelsPerMM( double dX, double dY );
void	StageMC6600_SetMag( double dMag );
void	StageMC6600_MoveToCenter( int nCenterX, int nCenterY, int nX, int nY );
void	StageMC6600_SetLanguageID( WORD wID );
void	StageMC6600_GoOrigin();
void	StageMC6600_GoHome();
void	StageMC6600_OnVentExit();
void    StageMC6600_Move( short nAxis, float fStep );
void    StageMC6600_Move5Axis(float fX, float fY, float fZ, float fR, float fT);
void    StageMC6600_Move2Axis(float fX, float fY);
void	StageMC6600_GetParam( int nIndex, float& f1, float& f2, float& f3, float& f4, float& f5 );
void	StageMC6600_StopTrack();
void	StageMC6600_StopMove();
void	StageMC6600_SetTrackParam( int nSpeedX, int nSpeedY );
void    StageMC6600_SetResolution( CSize szReso );
void	StageMC6600_SetTracklocus1( int nSpeedX, int nSpeedY, int nSpeedR );
void	StageMC6600_SetTracklocus2( int nSpeedT, int nSpeedZ );
void	StageMC6600_StopTracklocus();
//void	Options_XYStage_FindIndex();


#ifdef _cplusplus
}
#endif

#endif