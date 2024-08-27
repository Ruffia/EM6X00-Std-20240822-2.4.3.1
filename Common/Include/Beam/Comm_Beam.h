#ifndef __CommBeam_H
#define __CommBeam_H

#define WM_USER_BeamDetection_Update		(WM_USER +601)


typedef struct _Beam6485_Params_Pos{
	float	fBeamDetectionPos[2];

} Beam6485_Params_Pos;

typedef struct _Beam6485_Params_Beam{
	float	fCurBeam;

} Beam6485_Params_Beam;

#endif
