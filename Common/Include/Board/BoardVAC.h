
#ifndef __BoardVAC_H
#define __BoardVAC_H

#define WM_USER_VAC_UpdateCCGValue		(WM_USER	+61)
#define WM_USER_BoardVAC_UpdateStatus	(WM_USER	+69)

typedef struct tagVACcfg_Ports{
	int				nPort;			// COM端口号
	BOOL			bReady;			// 端口是否工作正常
} CfgVAC_Ports;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// 6000项目真空控制板的通讯(带低真空)

const	BYTE	byte_BrdHead	= 0xF2;
const	BYTE	byte_BrdTail	= 0x2F;
const	BYTE	byte_BrdLenSnd1	= 0x05;
const	BYTE	byte_BrdLenSnd2	= 0x06;		// 下送真空阈值是6字节
const	BYTE	byte_BrdLenRec	= 0x06;

// 1. PC下送真空控制板的通信帧格式
//	帧头Byte1	命令名称Byte2	操作数Byte3		校验(XOR)Byte4		帧尾Byte5
//	F2			opcXXX			XX				Byte2 XOR Byte3		2F

// PC下送真空控制板的命令
const	BYTE	byte_opcVentChamb	= 0x01;		// oprand = 0x01：样品室放气
const	BYTE	byte_opcVentGun		= 0x02;		// oprand = 0x02：电子枪放气

const	BYTE	byte_opcPumpChamb	= 0x03;		// oprand = 0x03：样品室抽真空
const	BYTE	byte_opcPumpGun		= 0x04;		// oprand = 0x04：电子枪抽真空

const	BYTE	byte_opcV1		= 0x05;		// oprand = 0x01：开启V1；oprand = 0x02：关闭V1
const	BYTE	byte_opcCCD		= 0x06;		// oprand = 01：CCD开；oprand = 0x02：CCD关

const	BYTE	byte_opcStatus1	= 0x11;		// oprand = 0x11：获取真空状态
const	BYTE	byte_opcStatus2	= 0x21;		// oprand = 0x21：获取阀门状态

const	BYTE	byte_opcCCG		= 0x31;		// oprand = 0x31：获取CCG值
const	BYTE	byte_opcTC3		= 0x41;		// oprand = 0x41：获取TC3值
const	BYTE	byte_opcTC4		= 0x51;		// oprand = 0x51：获取TC4值

// 2. PC下送真空控制板的各阈值的通信帧格式
//	帧头Byte1	命令名称Byte2	操作数Byte3		操作数Byte4		校验(XOR)Byte5				帧尾Byte6
//	F2			opcXXX			XX				XX				Byte2 XOR Byte3	XOR Byte4	2F

// PC下送真空控制板的各阈值命令
const	BYTE	byte_opcTsTC3	= 0x07;		// oprand = 阈值设置值XX XX(高8位在前)：设置TC3电压阈值
const	BYTE	byte_opcTsTC4	= 0x08;		// oprand = 阈值设置值XX XX(高8位在前)：设置TC4电压阈值
const	BYTE	byte_opcTsCCG1	= 0x09;		// oprand = 阈值设置值XX XX(高8位在前)：设置CCG电压上阈值(差于该值真空变坏)
const	BYTE	byte_opcTsCCG2	= 0x0A;		// oprand = 阈值设置值XX XX(高8位在前)：设置CCG电压下阈值(优于该值真空达标)
const	BYTE	byte_opcInterval= 0x0B;		// oprand = 放气阀间隔时间XX XX，单位秒，范围60-90秒
// 注：10V对应0xFFFF, 设置的阈值换算成相应的十六进制代码
// 例如：
// TC翻转4.0E-01相当于电压0.4V左右，即设置TC电压阈值为0x0A3D(65535*0.4/10)
// CCG翻转5.0E-05相当于电压4.3V左右，即设置CCG下阈值为0x6E14(65535*4.3/10)

// 3. 真空控制板上传的响应帧格式
//	帧头Byte1	命令名称Byte2	参数1 Byte3		参数2 Byte4		校验(XOR)Byte5				帧尾Byte6
//	F2			opcXXX			XX				XX				Byte2 XOR Byte3 XOR Byte4	2F

// 真空控制板上送的命令
//const	BYTE	byte_opcStatus1	= 0x11;		// 返回真空状态
// 参数1：0xXY
//	X：	0 初始化状态；1 样品室大气状态；2 电子枪大气状态
//		3 样品室抽真空状态；4 电子枪抽真空状态
//		5 样品室粗真空状态；6 电子枪粗真空状态；
//		7 TMP运行状态；8 TMP运行错误状态
//	Y：	0bMKQP
//		M：1 样品室真空错误状态；0 样品室真空正常状态
//		KQP 保留
//	参数2：0x00

//const	BYTE	byte_opcStatus2	= 0x21;		// 返回阀门状态
//	参数1：0b ABCD EFGH （1为开，0为关）
//		A:CCD；B:TMP；C:CCG；D:V5；E:V4；F:V3：G:V2；H:V1
//	参数2：0b ABCD EFGH （1为开，0为关）
//		H:V1回读
//		ABCDEFG保留

// const	BYTE	byte_opcCCG		= 0x31;		// 返回CCG值
// const	BYTE	byte_opcTC3		= 0x41;		// 返回TC3值
// const	BYTE	byte_opcTC4		= 0x51;		// 返回TC1值
// 参数1：0xXX；参数2：0xXX
// 参数1和2对应16位ADC数值

// 当真空控制模块收到PC发出的opcVentChamb、opcVentGun、opcPumpChamb、opcPumpGun、opcStatus1命令后，执行并应答opcStatus1
// 当真空控制模块收到PC发出的获取CCG或TC值命令时，应答返回相应值



// 4. 真空控制板上传的报警帧格式
//	帧头Byte1	命令名称Byte2	参数1 Byte3	参数2 Byte4		校验(XOR)Byte5				帧尾Byte6
//	F2			opcXXX			XX			XX				Byte2 XOR Byte3 XOR Byte4	2F

// 真空控制板上传的报警帧命令
const	BYTE	byte_opcWarning	= 0x92;		// 参数1：0x92;
											// 参数2：
											//		  0x00 TMP运行报警
											//		  0x01 样品室真空错误报警
// 下位机报警命令PC无响应值



/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//const	BYTE	byte_opcStatus1	= 0x11;		// 返回真空状态
// 参数1：0xXY
//	X：	0 初始化状态；1 样品室大气状态；2 电子枪大气状态
//		3 样品室抽真空状态；4 电子枪抽真空状态
//		5 样品室粗真空状态；6 电子枪粗真空状态；
//		7 TMP运行状态；8 TMP运行错误状态
//	Y：	0bMKQP
//		M：1 样品室真空错误状态；0 样品室真空正常状态
//		KQP 保留
//	参数2：0x00

typedef struct tagStatusVac6K{
	BYTE	Yrev	: 3;
	BYTE	YM		: 1;
	BYTE	X		: 4;
}StatusVac6K;

typedef union tagStatusParam16K{
	BYTE		bStatus;
	StatusVac6K	status;
}StatusParam16K;


//const	BYTE	byte_opcStatus2	= 0x21;		// 返回阀门状态
//	参数1：0b ABCD EFGH （1为开，0为关）
//		A:CCD；B:TMP；C:CCG；D:V5；E:V4；F:V3：G:V2；H:V1
//	参数2：0b ABCD EFGH （1为开，0为关）
//		H:V1回读
//		ABCDEFG保留

typedef struct tagStatusValve6K{
	WORD	V1		: 1;	// 00000000 0000000?
	WORD	V2		: 1;	// 00000000 000000?0
	WORD	V3		: 1;	// 00000000 00000?00
	WORD	V4		: 1;	// 00000000 0000?000
	WORD	V5		: 1;	// 00000000 000?0000
	WORD	CCG		: 1;	// 00000000 00?00000
	WORD	TMP		: 1;	// 00000000 0?000000
	WORD	CCD		: 1;	// 00000000 ?0000000
	WORD	V1Rec	: 1;	// 0000000? 00000000
	WORD	Rev		: 7;	// ???????0 00000000
}StatusValve6K;

typedef union tagStatusParam26K
{
	WORD			wStatus;
	StatusValve6K	status;	
}StatusParam26K;

typedef struct tagVACstatus6K_Board{
	BYTE	byteVent;	// 0x01：样品室放气；	0x02：电子枪放气
	BYTE	bytePump;	// 0x01：抽高真空；		0x02：抽低真空
	BYTE	byteV1;		// 0x01：打开；			0x02：关闭

	StatusParam16K	Vac;
	StatusParam26K	Valve;
}VACstatus6K_Board;

typedef struct tagVACstatus6K_CCG{
	double	dVac;
}VACvalue6K_CCG;

typedef struct tagVACstatus6K_TC{
	float	fTC3;	// 电压回读值
	float	fTC4;	// 电压回读值
}VACvalue6K_TC;

typedef struct tagVACstatus6K_Thresholds{
	float	fCCG1;	// 真空值Torr
	float	fCCG2;	// 真空值Torr
	float	fTC3;	// 电压值V
	float	fTC4;	// 电压值V
	float	fTC3Min;
	float	fTC3Max;
	float	fTC4Min;
	float	fTC4Max;
}VACvalue6K_Thresholds;

#endif
