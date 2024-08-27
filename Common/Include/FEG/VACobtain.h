
#ifndef __VACObtain_H
#define __VACObtain_H

#define WM_USER_VAC_UpdateStatus	(WM_USER	+57)	// Status byte from the controller is passed to user via the wParam argument
#define WM_USER_VAC_UpdateValue		(WM_USER	+58)
#define WM_USER_VAC_UpdateIPGValue	(WM_USER	+59)
#define WM_USER_VAC_UpdateIPCValue	(WM_USER	+60)
#define WM_USER_VAC_UpdateCCGValue	(WM_USER	+61)

#define EPSILON_E11 (float)(1E-11)
#define EPSILON_E9 (float)(1E-9)
#define EPSILON_E7 (float)(1E-7)
#define EPSILON_E5 (float)(1E-5)

typedef struct tagVACcfg_CCGIPG{
	int				nPortIPG;		// IPG COM端口号
	int				nPortIPC;		// IPC COM端口号
	int				nPortCCG;		// CCG COM端口号
	BOOL			bIPGready;
	BOOL			bIPCready;
	BOOL			bCCGready;
	double			dVacIPG;		// IPG真空值
	double			dVacIPC;
	double			dVacCCG;
} CfgVAC_CCGIPG;

typedef struct tagVACcfg_IP_CCG{
	double			dVac;			// IonPump 真空值
	double			dVacBak;
	double			dThreshold1;	// 阈值1
	double			dThreshold2;	// 阈值2
	int				nPort;			// COM端口号
	int				nType;			// 是IPG(1)、IPC(2)，还是CCG(3)
	BOOL			bReady;			// 端口是否工作正常
	BOOL			bInInterlock;	// 是否参与真空高压联锁
} CfgVAC_IP_CCG;



/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// 场枪项目与真空控制板的通讯

const	BYTE	byte_FEGBrdHead			= 0xF2;
const	BYTE	byte_FEGBrdTail			= 0x2F;
const	BYTE	byte_FEGBrdLenSnd		= 0x05;
const	BYTE	byte_FEGBrdLenRec		= 0x06;

// 1. PC下送真空控制板的通信帧格式
//	帧头Byte1	命令名称Byte2	操作数Byte3		校验(XOR)Byte4		帧尾Byte5
//	F2			opcXXX			XX				Byte2 XOR Byte3		2F

// PC下送真空控制板的命令
const	BYTE	byte_FEGBrdopcVent		= 0x01;		// oprand = 0x01：放气
const	BYTE	byte_FEGBrdopcPump		= 0x03;		// oprand = 0x03：抽真空

const	BYTE	byte_FEGBrdopcV1		= 0x02;		// oprand = 0x01：开启V1；oprand = 0x02：关闭V1

const	BYTE	byte_FEGBrdopcStatus	= 0x11;		// oprand = 0x11：获取真空状态
const	BYTE	byte_FEGBrdopcIPG		= 0x21;		// oprand = 0x21：获取IPG值
const	BYTE	byte_FEGBrdopcClr		= 0x91;		// oprand = 0x91：清除枪真空错误



// 2. 真空控制板上传的响应帧格式
//	帧头Byte1	命令名称Byte2	参数1 Byte3		参数2 Byte4		校验(XOR)Byte5				帧尾Byte6
//	F2			opcXXX			XX				XX				Byte2 XOR Byte3 XOR Byte4	2F

// 真空控制板上送的命令
//const	BYTE	byte_opcStatus	= 0x11;		// 返回真空状态和各个开关量的状态
											// 参数1：0xXY
											//	X：	0 初始化状态；1 样品室大气状态；2 抽真空状态
											//		3 粗真空状态；4 TMP运行状态；5 TMP运行错误状态
											//	Y：	0bMKQP
											//		M：1 枪真空错误状态；0 枪真空正常状态
											//		KQP 保留
											//	参数2：0b ABCD EFGH （1为开，0为关）
											//		A、B：保留；C：CCG电源；D：TMP；E：MP；F：Vvent2；G：Vvent1；H：V1

//const	BYTE	byte_opcIPG		= 0x21;		// 参数1：0xXX；参数2：0xXX；分别对应16位ADC数值
//const	BYTE	byte_opcClr		= 0x91;		// 参数1：0x00；参数2：0x00；已清除枪真空错误



// 当真空控制模块收到PC发出的opcVent、opcV1、opcPump、opcStatus命令后，执行并应答opcStatus
// 当真空控制模块收到PC发出的opcIPG命令时，应答opcIPG
// 当真空控制模块收到PC发出的opcClr命令时，清除枪真空错误并应答opcClr



// 3. 真空控制板上传的报警帧格式
//	帧头Byte1	命令名称Byte2	参数1 Byte3	参数2 Byte4		校验(XOR)Byte5				帧尾Byte6
//	F2			opcXXX			XX			XX				Byte2 XOR Byte3 XOR Byte4	2F

// 真空控制板上传的报警帧命令
const	BYTE	byte_FEGBrdopcWarning	= 0x92;		// 参数1：0x92;
													// 参数2：0x01 枪真空错误报警; 0x00 TMP运行报警
// 下位机报警命令PC无响应值



typedef struct tagStatusFEGBrdVAC_Board{
	double	dPipg;

	BYTE	byteVent;		// 0x01：放气；0x03：抽气
	BYTE	byteV1;			// 0x01：开启；0x02：关闭

	WORD	wStatus;
	BOOL	bV1;
	BOOL	bVvent1;
	BOOL	bVvent2;
	BOOL	bMP;
	BOOL	bTMP;
	BOOL	bCCG;
	BYTE	bX;
	BYTE	bY;
} StatusFEGBrdVAC_board;

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// 与真空规WPC400的通讯
const	BYTE	byte_Sn			= 0x01;
const	BYTE	byte_FuncCodeR	= 0x03;	// 读指令命令代码
const	BYTE	byte_FuncCodeW	= 0x10;	// 写指令命令代码
const	BYTE	byte_AddrH		= 0x00;	// 寄存器高地址
const	BYTE	byte_AddrReq1	= 0x01;	// 寄存器低地址：规管1的科学计数法格式的真空度数据
const	BYTE	byte_ReadLenH	= 0x00;	// 读指令数据字长度高8位
const	BYTE	byte_ReadLenL	= 0x02;	// 读指令数据字长度低8位
const	BYTE	byte_ReadLen	= 0x04;	// 读指令数据字节长度低8位
const	BYTE	byte_AddrSet1	= 0x21;	// 控制开关1的设置真空度浮点数据
const	BYTE	byte_AddrSet2	= 0x23;	// 控制开关2的设置真空度浮点数据
const	BYTE	byte_AddrSet3	= 0x31;	// 控制开关1对应的规管。1为规管1；2为规管2
const	BYTE	byte_AddrSet4	= 0x32;	// 控制开关2对应的规管。1为规管1；2为规管2
const	BYTE	byte_LenReqSend	= 0x08;	// 读指令下发字节数
const	BYTE	byte_LenReqRecv	= 0x09;	// 读指令上传字节数
const	BYTE	byte_LenSetSend	= 0x0D;	// 写指令下发字节数
const	BYTE	byte_LenSetRecv	= 0x08;	// 写指令上传字节数

// 1. PC读参数的通信帧格式
//	仪表编号	功能代码		寄存器高地址H	寄存器低地址L	数据字长度H		数据字长度L		CRC L	CRC H
//	byteSn		byte_FuncCodeR	byte_AddrH		byte_AddrReq1L	byte_ReadLenH	byte_ReadLenL
//												byte_AddrReq2L

// 2. WPC返回的通讯帧格式
//	仪表编号	功能代码		读取字节数		数据1 - 4		CRC L	CRC H
//	byteSn		byte_FuncCodeR	byte_ReadLen	

// 3. PC写参数的通信帧格式
//	仪表编号	功能代码		寄存器高地址H	寄存器低地址L	数据字长度H		数据字长度L		数据字节数	数据高8位	数据中8位	数据中8位2	数据低8位	CRC L	CRC H
//	byteSn		byte_FuncCodeW	byte_AddrSetH	byte_AddrSet1L
//												byte_AddrSet2L
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

#endif
