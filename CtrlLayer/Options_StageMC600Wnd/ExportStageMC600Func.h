//---------------------------
// ExportStageMC600Func.h
// 声明欲输出的函数
//---------------------------

#ifndef _EXPORT_StageMC600_Func_H
#define _EXPORT_StageMC600_Func_H

#ifdef _cplusplus
extern "C"{
#endif

void    StageMC600_Move_ToXY(float x, float y);  //这个函数的功能是单纯的给控制台x,y值然后让它移动
void    StageMC600_upDate(float fX, float fY, float fZ, float fR, float fT);  //这个函数的功能是单纯的更新控制台输入框的值
void    StageMC600_ReadStage(float &fX, float &fY, float &fZ, float &fR, float &fT);//用于获取控制台输入框的数据

void	StageMC600_ShowWnd(HWND hMainWnd);
//void	Options_XYStage_FindIndex();
BOOL	StageMC600_IsReady();
void	StageMC600_SetLanguageID( WORD wID );
void	StageMC600_SetPixelsPerMM( double dX, double dY );
void	StageMC600_SetResolution( CSize szReso );
void	StageMC600_SetMag( double dMag );
BOOL	StageMC600_IsMoving();
void	StageMC600_StopMove();
void	StageMC600_MoveToCenter( int nCenterX, int nCenterY, int nX, int nY );
void	StageMC600_Move( short nAxis, float fPos );
void	StageMC600_Move4Axis( float fX, float fY, float fZ, float fR );
void	StageMC600_Move5Axis( float fX, float fY, float fZ, float fR, float fT );
void	StageMC600_GoOrigin();
void	StageMC600_GoHome();
void	StageMC600_OnVentExit();
void	StageMC600_SetTrackParam( int nSpeedX, int nSpeedY );
void	StageMC600_StopTrack();
void	StageMC600_SetParam( int nIndex, double dParam );
void	StageMC600_GetParam( int nIndex, float& f1, float& f2, float& f3, float& f4, float& f5 );

#ifdef _cplusplus
}
#endif

#endif