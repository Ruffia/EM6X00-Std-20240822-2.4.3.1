#ifndef __CommVince_H
#define __CommVince_H

#include <afxtempl.h>

#define WM_USER_Vince_UpdateValue	(WM_USER+99)

/////////////////////////////////////////////////////////////////////
// ���ת������0.3ת~1ת����Ӧ32΢��ϸ�֣���Լ��1920 ~ 6400 (200��ÿ1ת)
// ��������Ϊ1.8�ȣ�32ϸ���൱����32������1.8��
// ��ϸ���£���� 1 STEP = 1.8 / 32
// T��ĳ��ִ������� 36:1
// 1 STEP ��Ӧ����б�ն�ת��Ӧ��Ϊ 1.8 / 32 / 36 = 1 / 640
// ��Ӧ��ͬϸ��״̬�£�1 STEP������Ӧ���ն�ת��Ϊ��
//		= 1.8 / ϸ���� / ���ٴ�����
/////////////////////////////////////////////////////////////////////

const int	nStep		= 200;	// 200��ÿת
const int	nDriveRat	= 36;	// T����ִ�����

typedef struct tag_Vince_Params_Sys{
	short	mcs;	// ΢��ϸ��
	long	acc;	// ���ٶ�
	long	dec;	// ���ٶ�
	int		zsd;	// �����ٶ�
	long	zsp;	// �����ȫλ������
} Vince_Params_Sys;

typedef struct tag_Vince_Params{
	DWORD	speed;	// �ٶ�
	int		pos;	// λ��
} Vince_Params;

// ÿһ������������Ӧ�ĽǶ�Ϊ��StepAngle / (ϸ���� *Step) /DriveRat
// ����360 / (32 *200) / 36 = 0.0015625
// T����������-11953 ~ 46267�������Ϊ��-18.8756 ~ 72.2921875 ��
// �����ȫλ��zsp�̶�����Ϊ4800����֤��������λ���غ����˳�������֤����Ƭ����ѹ

typedef struct tag_Vince_Status{
	DWORD	S1		: 1;	// 00000000 00000000 00000000 0000000?	// ������S1״̬λ��0Ϊ�͵�ƽ��1Ϊ�ߵ�ƽ
	DWORD	S2		: 1;	// 00000000 00000000 00000000 000000?0
	DWORD	S3		: 1;	// 00000000 00000000 00000000 00000?00
	DWORD	S4		: 1;	// 00000000 00000000 00000000 0000?000
	DWORD	pos		: 1;	// 00000000 00000000 00000000 000?0000	// ��ǰλ����Ŀ��λ���Ƿ����
	DWORD	spd		: 1;	// 00000000 00000000 00000000 00?00000	// ��ǰ�ٶ���Ŀ���ٶ��Ƿ����
	DWORD	err_hw	: 1;	// 00000000 00000000 00000000 0?000000	// Ӳ������״̬λ
	DWORD	org		: 1;	// 00000000 00000000 00000000 ?0000000	// ԭ���־λ
	DWORD	stp		: 1;	// 00000000 00000000 0000000? 00000000	// ֹͣ��־λ
	DWORD	err_cmd	: 1;	// 00000000 00000000 000000?0 00000000	// ָ������־λ
	DWORD	err_wr	: 1;	// 00000000 00000000 00000?00 00000000	// �洢����д�����־λ
	DWORD	off		: 1;	// 00000000 00000000 0000?000 00000000	// �������б�־λ
	DWORD	shake	: 1;	// 00000000 00000000 000?0000 00000000	// �����źű�־λ
	DWORD	en		: 1;	// 00000000 00000000 00?00000 00000000	// �羵ʹ�ܱ�־λ
	DWORD	Zerofin	: 1;	// 00000000 00000000 0?000000 00000000	// ���������־λ
	DWORD	Rev15	: 1;	// 00000000 00000000 ?0000000 00000000
	DWORD	S5		: 1;	// 00000000 0000000? 00000000 00000000	// ������S5״̬λ��0Ϊ�͵�ƽ��1Ϊ�ߵ�ƽ
	DWORD	S6		: 1;	// 00000000 000000?0 00000000 00000000	// ������S5״̬λ��0Ϊ�͵�ƽ��1Ϊ�ߵ�ƽ
	DWORD	Rev1819	: 2;	// 00000000 0000??00 00000000 00000000
	DWORD	ovrh	: 1;	// 00000000 000?0000 00000000 00000000	// ���ȱ�����־λ��0Ϊ����
	DWORD	ovrc	: 1;	// 00000000 00?00000 00000000 00000000	// ����������־λ��0Ϊ����
	DWORD	ovrv	: 1;	// 00000000 0?000000 00000000 00000000	// ��ѹ������־λ��0Ϊ����
	DWORD	Rev		: 9;	// ???????? ?0000000 00000000 00000000
} Vince_Status;

typedef union tag_Vince_Param_Status
{
	DWORD			dwStatus;
	Vince_Status	status;	
} Vince_Param_Status;

typedef enum _Vince_Param_Cfg{
	Cfg_brd,	// ������					// 9600 ~ 921600
	Cfg_cid,	// ͨѶID					// 1 ~ 32
	Cfg_mcs,	// ΢��ϸ��					// 0 ~ 8 (5, 32΢��)
	Cfg_spd,	// �ٶ�						// -192000 ~ 192000
	Cfg_acc,	// ���ٶ�					// 0 ~ 19200 0000
	Cfg_dec,	// ���ٶ�					// 0 ~ 19200 0000
	Cfg_cra,	// ���ٵ���					// 0 ~ 2.5
	Cfg_crn,	// �������е���				// 0 ~ 2.5
	Cfg_crh,	// ���ֵ���					// 0 ~ 2.5
	Cfg_s1f,	// ������1�½��ش����¼�	// 0 ~ 9
	Cfg_s1r,	// ������1�����ش����¼�	// 0 ~ 9
	Cfg_s2f,	// ������2�½��ش����¼�	// 0 ~ 9
	Cfg_s2r,	// ������2�����ش����¼�	// 0 ~ 9
	Cfg_s3f,	// ������3�½��ش����¼�	// 0 ~ 9
	Cfg_s3r,	// ������3�����ش����¼�	// 0 ~ 9
	Cfg_s4f,	// ������4�½��ش����¼�	// 0 ~ 9
	Cfg_s4r,	// ������4�����ش����¼�	// 0 ~ 9
	Cfg_s5f,	// ������5�½��ش����¼�	// 0 ~ 9
	Cfg_s5r,	// ������5�����ش����¼�	// 0 ~ 9
	Cfg_s6f,	// ������6�½��ش����¼�	// 0 ~ 9
	Cfg_s6r,	// ������6�����ش����¼�	// 0 ~ 9
	Cfg_s3,		// ������3ģʽ����			// 0Ϊ����(sf/sr��Ч)��1Ϊ���(sf/sr��Ч)
	Cfg_s4,		// ������4ģʽ����			// 0Ϊ����(sf/sr��Ч)��1Ϊ���(sf/sr��Ч)
	Cfg_s5,		// ������5ģʽ����			// 0Ϊ����(sf/sr��Ч)��1Ϊ���(sf/sr��Ч)
	Cfg_s6,		// ������6ģʽ����			// 0Ϊ����(sf/sr��Ч)��1Ϊ���(sf/sr��Ч)
	Cfg_zmd,	// ���㹦��					// 0 ~ 5
	Cfg_snr,	// �����ô�����				// 0 ~ 5
	Cfg_osv,	// �����ô�����������������	// 0:����״̬ʱΪ�͵�ƽ��1:�ߵ�ƽ
	Cfg_zsd,	// �����ٶ�					// -192000 ~ 192000
	Cfg_zsp,	// �����ȫλ��			// -2147483647 ~ 2147483647
	Cfg_sds,	// �޸й���������			// 0 ~ 100
	Cfg_zcr,	// �޸й������				// 0.2 ~ 2.5
	Cfg_dmd,	// ��������ģʽ				// 0:��ͨģʽ��1:����ģʽ�����������п�ʼǰ���й���
	Cfg_dar,	// ��������ģʽ�Զ�����ʱ��	// 0 ~ 60��0Ϊ�������Զ�ִ����������ģʽ��1 ~ 60����λ��
	Cfg_msr,	// �����޴�����				// 0 ~ 6
	Cfg_msv,	// �����ߴ�����������ƽ		// 0Ϊ�͵�ƽ������1Ϊ�ߵ�ƽ����
	Cfg_psr,	// �����޴�����				// 0 ~ 6
	Cfg_psv,	// �����޴�����������ƽ		// 0Ϊ�͵�ƽ������1Ϊ�ߵ�ƽ����
	Cfg_pae,	// �ϵ�ʹ��					// 0Ϊ�ϵ粻ʹ�ܣ�1Ϊ�ϵ�ʹ��
	Cfg_zar,	// �ϵ����					// 0Ϊ�ϵ粻�Զ����㣻1Ϊ�ϵ��Զ�����
} Vince_Param_Cfg;

/*
typedef enum _Vince_Param_Speed{
	Speed,			// �������ٶ�
	SpeedInit,		// ��������ٶ�
	SpeedAcc,		// ��������ٶ�
	SpeedHome		// �������ԭ���ٶ�
} Vince_Param_Speed;

typedef enum _Vince_Param_Home{
	Origin,			// �˶������ģʽ
	HomeMode,		// �˶�����㷽ʽ
	UserOrigin		// �˶��Ṥ��ԭ��
} Vince_Param_Home;

typedef enum _Vince_Param_Limit{
	LimitAble,		// �˶�������λ״̬
	LimitPositive,	// �˶��������������
	LimitNegative	// �˶��Ḻ���������
} Vince_Param_Limit;

typedef enum _Vince_Param_Sys{
	Unit,			// �˶���ʹ�õ�λ
	Style,			// ̨������
	WorkState,		// ̨�ӹ���״̬
	Pitch,			// ˿�ܵ���
	StepsRev,		// ÿת������������
	DriveRat,		// ������
	Distance		// ƽ��̨���г�
// 	Radius,			// ��դ�뾶
// 	EncoderStyle,	// ��դ������
// 	EncoderReso		// ��դ�߷ֱ���
} Vince_Param_Sys;

// PortOutput,	// ������IO��

typedef struct _Vince_Params_Sys{
	float	fPitch[5];		// ˿�ܵ���
	float	fStepsRev[5];	// ÿת������������
	float	fRat[5];		// ������
	float	fDistance[5];	// ƽ��̨���г�
} Vince_Params_Sys;

typedef struct _Vince_Params_Speed{
	float	fSpeed[5];		// �������ٶ�
	float	fSpeedInit[5];	// ��������ٶ�
	float	fSpeedAcc[5];	// ��������ٶ�
	float	fSpeedHome[5];	// �������ԭ���ٶ�
} Vince_Params_Speed;

typedef struct _Vince_Params_Pos{
	float	fSteps[5];		// ��ǰλ��
	float	fOrigin[5];		// �û�ԭ��λ��
} Vince_Params_Pos;

typedef struct _Vince_Params_Bl{	// ��϶����
	short	nDir[5];		// -1��ʾ������1��ʾ������
	int		nCount[5];		// ��϶������λpp
} Vince_Params_Bl;

struct structVincePos{
	CString	strName;		// ����
	float	fPos[5];		// λ��
};
typedef CTypedPtrList< CPtrList, structVincePos* > ListVincePos;
*/
#endif
