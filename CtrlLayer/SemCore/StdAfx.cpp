// stdafx.cpp : source file that includes just the standard includes
//	SemCore.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

//对浮点数进行四舍五入
float Round(const float fDecimal2Round,const int nPrecision)
{
#define buf_size 100
	//通过输入生成精度字符串 （%0.*f）
	char Buf[buf_size] = {0};
	sprintf_s(Buf,buf_size,"%%0.%df",nPrecision);

	//通过输入生成对应精度的float形打印输出
	char outputBuf[buf_size];
	sprintf_s(outputBuf,buf_size,Buf,fDecimal2Round);

	//回转回float行数据
	float output = atof(outputBuf);
	return output;
}

