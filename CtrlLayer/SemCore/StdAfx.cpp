// stdafx.cpp : source file that includes just the standard includes
//	SemCore.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

//�Ը�����������������
float Round(const float fDecimal2Round,const int nPrecision)
{
#define buf_size 100
	//ͨ���������ɾ����ַ��� ��%0.*f��
	char Buf[buf_size] = {0};
	sprintf_s(Buf,buf_size,"%%0.%df",nPrecision);

	//ͨ���������ɶ�Ӧ���ȵ�float�δ�ӡ���
	char outputBuf[buf_size];
	sprintf_s(outputBuf,buf_size,Buf,fDecimal2Round);

	//��ת��float������
	float output = atof(outputBuf);
	return output;
}

