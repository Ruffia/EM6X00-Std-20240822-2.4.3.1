
#include "StdAfx.h"
#include "insample0.h"

#include "Calc.h"

#include <iostream>
#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

//////////////////////////////以下部分为辅助函数 功能与计算器的计算关系不算太大/////////////////////
double tenci(int a)
{
	double x=1;
	while(a>0)
	{
		x*=10;a--;
	}
	while(a<0)
	{
		x/=10;a++;
	}
	return x;
}

double StoD(char *f,char *e=0)//将小数 分数 整数的字符串转化为具体的double值
{
	long int z=0,x=0,i=0,c=0,m=0,fs=0;char *hh=f;
	double result=0.0;
	if(*f=='+')
		f++;
	if(*f=='-')
	{
		fs=1;
		f++;
	}
	if(e==0)
	{
		int a=0;
		while(*(f+a)!='\0')
			a++;
		e=f+a-1;
	}
    while(f+m<=e)
	{
		if(*(f+m)=='/'||*(f+m)=='|')
			c=1;
		m++;
	}
	m=0;
	if(c==0)
	{ 
		while(f+m<=e)
		{
			if(i==0 && 47<*(f+m) && *(f+m)<58)
			{
				z++;
			}
			if(*(f+m)=='.')
			{
				i=1;
			}
			if(i==1 && 47<*(f+m) && *(f+m)<58)
			{
				x++;
			}
			m++;
		}
		while(z>0)
		{
			result+=((*f-48)*tenci(z-1));
			z--;
			f++;
		}
		f+=1;m=1;                                      
		while(x>=m)
		{
			result+=((*f-48)*tenci(-m));
			f++;
			m++;
		}
	}
	else
	{
		double fz=0,fm=0;m=0;x=0;
		while(f+m<=e)
		{
			if(i==0 && 47<*(f+m) && *(f+m)<58)
			{
				z++;
			}
			if(*(f+m)=='/'||*(f+m)=='|')
			{
				i=1;
			}
			if(i==1 && 47<*(f+m) && *(f+m)<58)
			{
				x++;
			}
			m++;
		}
		while(z>0)
		{
			fz+=((*f-48)*tenci(z-1));
			z--;
			f++;
		}                       
		f+=1;
		m=0;
		while(x>0)
		{
			fm+=((*f-48)*tenci(x-1));
			f++;
			x--;
		}
		result=fz/fm;
	} 
	if(fs==1)
		result*=-1;
	return result;
}

double COUNTER_FSXS(double x,int m=1,bool js=1,bool FFM=0)//
{
	double a=1.1,b=1,k=0;
	if(m==0 && x==0)
	{
		if(FFM==1)
			return 1;
		else
			return 0;
	}
	while((int)a!=a)
	{
		if(js && a>=0 && (int)(a+0.000001)-a<0.000001 && (int)(a+0.000001)-a>-0.000001)
		{
			break;
		}
		if(js && a<0 && (int)(a-0.000001)-a<0.000001 && (int)(a-0.000001)-a>-0.000001)
		{
			break;
		}
		a=x*b;
		b++;
		if(b>10000)
		{
			k=1;
			break;
		}
	}
	b--;
	if(FFM==1)
	{
		return b;
	}
	if(k==1)
	{
		if(m!=2)
			cout<<x;
		return 2;
	}
	else
	{
		if(b!=1)
		{
			if(m!=2)
				cout<<a<<"|"<<b;
			return 1;
		}
		else
		{
			if(m!=2)
				cout<<a;
		}
	}
	return 0;
}

int PFZYS(int x)
{
	int y,z,m;
	if(x==0)
	{
		return 0;
	}
	for(y=1;y<=sqrt(x);y++)
	{
		z=y*y;
		z=x%z;
		if(z==0)
		{
			m=y;
		}
	}
	return m;
}

int COUNTER_GSXS(double x,int m=1,bool js=1)
{
	double a=1.1,b=1,k=0;
	int temp;
	if(m==0 && x==0)
	{
		return 1;
	}
	if(x==0)
	{
		if(m!=2)
			cout<<"0";
		return 1;
	}
	if(x-1 < 0.00000001 && x-1 > -0.00000001)
	{
		if(m!=2)
			cout<<"1";
		return 1;
	}
	if(x<0)
		return 0;
	while( (int)a != a )
	{
		if(js && a>=0 && (int)(a+0.000001)-a<0.000001 && (int)(a+0.000001)-a>-0.000001)
		{
			a=(double)(int)(a+0.000001);
			break;
		}
		if(js && a<0 && (int)(a-0.000001)-a<0.000001 && (int)(a-0.000001)-a>-0.000001)
		{
			a=(double)(int)(a+0.000001);
			break;
		}
		a=x*b;
		b++;
		if(b>10000)
		{
			k=1;
			break;
		}
	}
	b--;
	if(k==1)
	{
		if(m!=2)
			cout<<"√"<<x;
		return 3;
	}
	temp=PFZYS((int)(a*b));
	a=a*b/temp/temp;
	if(temp/b != 1)
	{
		if(m!=2)
		{
			if(a!=1)
				cout<<"(";
			COUNTER_FSXS(temp/b);
			if(a!=1)
				cout<<")";
		}
	}
	if(a!=1)
	{
		if(m!=2)
			cout<<"√"<<a;
		return 2;
	}
	else
		return 1;
}



////////////////////////////一下为计算器主要计算函数///////////////////////////////////////////////////////////////
int counterJSsimp(COUNTERpar *parpF,COUNTERpar *parpE,COUNTERpar *parF=0,COUNTERpar *parp=0)
{
	COUNTERpar *partemp,*partemp2,*partemp3,*partemp4;
	double temp1=0,temp2=0,temp3=0;
//	double PI=3.1415926535;
	bool i=0,i2=0;//特殊情况标识
//cout<<"|||||||||"<<endl;
//CHECK
	partemp=parpF->next;
/*	while(partemp->kind!=')')
	{
		if(partemp->oper=='!')//处理阶乘
		{
			partemp2=partemp->last;
			if(partemp2->kind!='N')
			{
				cout<<"\t\tINPUT ERROR"<<endl;
				return 0;
			}//!前一定要为'数'
			temp1=temp2=1;//阶乘运算
			while(temp1<=partemp2->num)
			{
				temp2*=temp1;
				temp1++;
			}
			partemp2->num=temp2;//将结果存入被阶乘元素栏
			partemp2->next=partemp->next;
			partemp4=partemp->next;
			partemp4->last=partemp2;
			free(partemp);//删除阶乘符号元素
			partemp=partemp2;//连接乘符号元素前后两元素为相邻元素
		}
		partemp=partemp->next;
	}
*/
M3:	partemp=parpE;//倒搜索
	while(partemp->kind!='(')
	{
		if(partemp->oper=='-')//处理负号 
		{
			partemp2=partemp->last;
			partemp3=partemp->next;
			if(partemp3->kind=='F')//处理-sin30情况
			{
				partemp=partemp3;
				i2=1;
				break;
//				goto M2;	// 应该不会用到？？？？？
			}

			if(partemp3->kind!='N')
			{
				cout<<"\t\tINPUT ERROR"<<endl;
				return 0;
			}
			if(partemp2->kind=='N')//4 - 5的情况转为4 + -5
			{
				partemp3->num*=-1;
				partemp->oper='+';
			}                                                             //2 1 3
			if(partemp2->kind == '(' || partemp2->kind=='F' || partemp2->kind=='O')//( - 5 )或sin - 0.5 或- - 5情况转换为( -5 ) sin -0.5 - -5 
			{
				partemp3->num*=-1;
				partemp2->next=partemp3;
				partemp3->last=partemp2;
				free(partemp);
				partemp=partemp3;
			}
		}
		partemp=partemp->last;
	}

M1:	partemp=parpF;
/*	while(partemp->kind!=')')
	{
		if(partemp->oper=='^')//处理次方运算
		{
			partemp2=partemp->last;//partemp2指向x^y中的x
			partemp3=partemp->next;//partemp3指向x^y中的y
			if(partemp3->kind=='F')
			{
				partemp=partemp3;
				i=1;
				goto M2;        
			}//处理5^sin30这种情况
			if(partemp2->kind!='N' || partemp3->kind!='N')
			{
				cout<<"\t\tINPUT ERROR"<<endl;
				return 0;
			}//^前后都要为'数'
			temp1=partemp2->num;
			temp2=partemp3->num;
			if(-1<temp2 && temp2<1 && temp2!=0 && temp1<0)
			{
				cout<<"\t\tMATH ERROR"<<endl;
				return 0;
			}
			temp3=pow(temp1,temp2);
			partemp2->num=temp3;//计算结果保存入x^y中x项
			//z x ^ y m
			// 2 1 3 4
			partemp4=partemp3->next;
			partemp2->next=partemp4;
			partemp4->last=partemp2;
			free(partemp);free(partemp3);//删除^和y两项
			partemp=partemp2;
		} 
		partemp=partemp->next;
	}
*/
	partemp=parpF;
/*	while(partemp->kind!=')')
	{
		if(partemp->kind=='F')//处理函数运算
		{
M2:			partemp2=partemp->next;//partemp2指向'函数'的下一项
			if(partemp2->kind!='N')
			{
				cout<<"\t\tINPUT ERROR"<<endl;
				return 0;
			}//'函数'后要为数 
			temp1=partemp2->num;
			switch(*partemp->func)
			{
			case 'a':
				switch(*(partemp->func+1))
				{
				case 's':
					if(temp1<-1 || temp1>1)
					{
						cout<<"\t\tMATH ERROR"<<endl;
						return 0;
					}
					temp2=asin(temp1)*180/PI;
					break;//asin
				case 'c':
					temp2=acos(temp1)*180/PI;
					break;//acos
				case 't':
					temp2=atan(temp1)*180/PI;
					break;//atan
				}
				break;

			case 'c':
				temp2=cos(temp1*PI/180);
				break;//cos
			case 'l':
				switch(*(partemp->func+1))
				{
				case 'o':
					if(temp1<0)
					{
						cout<<"\t\tMATH ERROR"<<endl;
						return 0;
					}
					temp2=log(temp1);
					break;//log
				case 'n':
					if(temp1<0)
					{
						cout<<"\t\tMATH ERROR"<<endl;
						return 0;
					}
				temp2=log10(temp1);
				break;//ln
				}
				break;

			case 's':
				switch(*(partemp->func+1))
				{
				case 'i':
					temp2=sin(temp1*PI/180);
					break;//sin
				case 'q':
					if(temp1<0)
					{
						cout<<"\t\tMATH ERROR"<<endl;
						return 0;
					}
					temp2=sqrt(temp1);
					break;//sqrt
				}
				break;

			case 't':
				temp2=tan(temp1*PI/180);
				break;//tan
			}
			//x sin 5 y
			//4 1 2
			partemp2->num=temp2;//将结果放在函数的参数项中
			partemp4=partemp->last;
			partemp4->next=partemp2;
			partemp2->last=partemp4;
			free(partemp);//删'函数'元素项
			partemp=partemp4;
			if(i==1)
			{
				i=0;
				goto M1;
			}//对应上"处理5 ^ sin 30这种情况"让其变为5 ^ 0.5
			if(i2==1)
			{
				i2=0;
				goto M3;
			}//对应上 处理- sin 30这种情况 让其变为- 0.5
		}
		partemp=partemp->next;
	}
*/

	partemp=parpF;
	while(partemp->kind!=')')//x 4 * -5 y
	{                        // 2 1 3 4
		if(partemp->oper=='*')//处理乘法运算
		{   
			partemp2=partemp->last;
			partemp3=partemp->next;
			if(partemp2->kind!='N' || partemp3->kind!='N')
			{
				cout<<"\t\tINPUT ERROR"<<endl;
				return 0;
			}//*前后都要为'数'

			temp1=partemp2->num*partemp3->num;
			partemp2->num=temp1;
			partemp4=partemp3->next;
			partemp2->next=partemp4;
			partemp4->last=partemp2;
			free(partemp);free(partemp3);
			partemp=partemp2;
		} 

		if(partemp->oper=='/')//处理除法运算
		{
			partemp2=partemp->last;
			partemp3=partemp->next;
			if(partemp2->kind!='N' || partemp3->kind!='N')
			{
				cout<<"\t\tINPUT ERROR"<<endl;
				return 0;
			}//*前后都要为'数'
			if(partemp3->num==0)
			{
				cout<<"\t\tMATH ERROR"<<endl;
				return 0;
			}//除数不为0
			temp1=partemp2->num/partemp3->num;
			partemp2->num=temp1;
			partemp4=partemp3->next;
			partemp2->next=partemp4;
			partemp4->last=partemp2;
			free(partemp);free(partemp3);
			partemp=partemp2;
		} 
		partemp=partemp->next;
	}

	partemp=parpF;
	while(partemp->kind!=')')
	{
		if(partemp->oper=='+')//处理加法运算
		{
			partemp2=partemp->last;
			partemp3=partemp->next;
			if(partemp2->kind!='N' || partemp3->kind!='N')
			{
				cout<<"\t\tINPUT ERROR"<<endl;
				return 0;
			}//*前后都要为'数'
			temp1=partemp2->num+partemp3->num;
			partemp2->num=temp1;
			partemp4=partemp3->next;
			partemp2->next=partemp4;
			partemp4->last=partemp2;
			free(partemp);
			free(partemp3);
			partemp=partemp2;
		} 
		partemp=partemp->next;
	}

	//CHECK
	partemp=parpF->next;
	partemp2=partemp->next;
	if(parpF->num==125 && parpE->num==-125)
	{
		return 2;
	}//最外括号了
	else
	{
		//x ( N ) y
		//3 F 1 E 2
		//( x ( N )
		// 2 F 1 E
		if(parpE->num == -125)//反括号为最外括号 但正括号不为
		{
			partemp2=parpF->last;
			partemp2->next=partemp;
			partemp->last=partemp2;
			free(parpF);
			return 1;
		}
		partemp2=parpE->next;
		partemp2->last=partemp;
		partemp->next=partemp2;
		free(parpE);
		partemp3=parpF->last;
		partemp3->next=partemp;
		partemp->last=partemp3;
		free(parpF);
		return 1;
	} 
}

//接 c++ 科学计算器 支持阶乘 次方号 结果记忆 数学函数 等多种功能 （上）

double counterJS(char* input, int k,COUNTER_RESULT *pF)
{
	char *readF,*readE;
	int i=0;
	COUNTERpar *parF,*parE,*parp;
	COUNTER_RESULT *MP;
//****************************************//分析结构/////////////////////////////////////////////////////
	parF=parE=parp=new COUNTERpar;
	parF->kind='(';//首项设为'('
	parE->num=125;
	parE->oper='\0';
	parE->func="\0\0\0\0\0";
	parE->next=new COUNTERpar;
	parp=parE;
	parE=parE->next;
	parE->last=parp;
	parE->num=0;
	parE->oper='\0';
	parE->func="\0\0\0\0\0";
   
   
{
	int pare=0;
	readF=input;
	while(*readF!='\0')
	{ 
ME1:/*	if(*readF=='A'&&*(readF+1)=='n'&&*(readF+2)=='s')//为记忆数
		{
			//x Ans15
			//     FE
			int reorder,i=1;
			readF+=3;
			readE=readF;
			while('0'<=*readE && *readE<='9')
				readE++;
			readE--;
			parE->kind='N';
			reorder=(int)StoD(readF,readE);
			if(reorder >= (k-1))
			{
				cout<<"\t\tCANNOT FOUND THE MEMORY NUMBER"<<endl;
				return 0;
			}
			MP=pF;
			while(i<reorder)
			{
				MP=MP->next;
				i++;
			}
			parE->num=MP->result;//提取记忆数
			parE->next=new COUNTERpar;
			parp=parE;
			parE=parE->next;
			parE->last=parp;
			parE->num=0;
			parE->oper='\0';
			parE->func="\0\0\0\0\0";
			readF=readE;
			readF++;
        }
*/
		if(('0'<=*readF && *readF<='9'))//记录数
		{
			bool i=0;//是否有此数标识
			readE=readF;
			while(1)//确定此数尾数
			{
				if(('0'>*readE || *readE>'9') && *readE!='|' && *readE!='.')
					break;//非数的时候结束自加
				readE++;
				if(readE=='\0'){readE--;break;}//超出了算式字符串时结束
			}
			readE--;
			parE->num=StoD(readF,readE);//存入数值
			parE->kind='N';
			parE->next=new COUNTERpar;
			parp=parE;
			parE=parE->next;
			parE->last=parp;
			parE->num=0;
			parE->oper='\0';
			parE->func="\0\0\0\0\0";
			while(('0'<=*readF&&*readF<='9') || *readF=='|' || *readF=='.')
				readF++;

			if((64<*readF && *readF<91) || (96<*readF && *readF<123) || *readF=='(')//处理5(5+6)与5sin30情况 在中间加*
			{
				parE->oper='*';
				parE->kind='O';

				parE->next=new COUNTERpar;
				parp=parE;
				parE=parE->next;
				parE->last=parp;
				parE->num=0;
				parE->oper='\0';
				parE->func="\0\0\0\0\0";
			}
		}

		if(*readF=='+'
			|| *readF=='-'
			|| *readF=='*'
			|| *readF=='/'
			|| *readF=='^'
			|| *readF=='!')//记录运算
		{
			parE->oper=*readF;//存入运算

			parE->kind='O';
			parE->next=new COUNTERpar;
			parp=parE;
			parE=parE->next;
			parE->last=parp;
			parE->num=0;
			parE->oper='\0';
			parE->func="\0\0\0\0\0";
			readF++;
		}

/*		if((64<*readF && *readF<91) || (96<*readF && *readF<123))//记录函数
		{
			if(*readF=='A' && *(readF+1)=='n' && *(readF+2)=='s')
				goto ME1;
			switch(*readF)//寻找对应函数名
            {
			case 'a':
				switch(*(readF+1))
				{
				case 's':parE->func="asin";break;//asin
				case 'c':parE->func="acos";break;//acos
				case 't':parE->func="atan";break;//atan
				default:cout<<"\t\tINPUT ERROR"<<endl;return 0;
				}
				break;
			case 'c':parE->func="cos";break;//cos
			case 'l':
				switch(*(readF+1))
				{
				case 'o':parE->func="log";break;//log
				case 'n':parE->func="ln";break;//ln
				default:cout<<"\t\tINPUT ERROR"<<endl;return 0;
				}
				break;
			case 's':
				switch(*(readF+1))
				{
				case 'i':parE->func="sin";break;//sin
				case 'q':parE->func="sqrt";break;//sqrt
				default:cout<<"\t\tINPUT ERROR"<<endl;return 0;
				}
				break;
			case 't':parE->func="tan";break;//tan
			default:cout<<"\t\tINPUT ERROR"<<endl;return 0;
			}
			parE->kind='F';
			parE->next=new COUNTERpar;
			parp=parE;
			parE=parE->next;
			parE->last=parp;
			parE->num=0;
			parE->oper='\0';
			parE->func="\0\0\0\0\0";
			while((64<*readF&&*readF<91) || (96<*readF&&*readF<123))
				readF++;
		}
*/
		if(*readF=='(')//记录括号
		{
			parE->kind='(';
			pare++;
			parE->next=new COUNTERpar;
			parp=parE;
			parE=parE->next;
			parE->last=parp;
			parE->num=0;
			parE->oper='\0';
			parE->func="\0\0\0\0\0";
			readF++;
		}
		if(*readF==')')
		{
			if(pare==0)
			{
				cout<<"\t\tINPUT ERROR"<<endl;
				return 0;
			}//在没出现多余正括号时 出现反括号
			parE->kind=')';
			pare--;
			parE->next=new COUNTERpar;
			parp=parE;
			parE=parE->next;
			parE->last=parp;
			parE->num=0;
			parE->oper='\0';
			parE->func="\0\0\0\0\0";
			readF++;
		}
	} 
	if(pare<0)
	{
		cout<<"\t\tINPUT ERROR"<<endl;
		return 0;
	}//反括号数量多与正括号
	parE->kind=')';//解析元素链以)项结束(便于后续程序)
	parE->num=-125;
	parE->next=NULL;
}    
//分析结构//****************************************/////////////////////////////////////////////////////
    
  

//****************************************//化简计算/////////////////////////////////////////////////////
{
	COUNTERpar *parp2,*parp3;
	int Y;
	AS: parp=parF;
	parp3=parE;
	while(parp->next!=NULL)//用parp2,parp3分别锁定式子最中间的两个对应正反括号
	{
		if(parp->kind=='(')
		{
			parp2=parp;
		}
		if(parp->kind==')')
		{
			parp3=parp;
			break;
		}
		parp=parp->next;
	}
	//cout<<parp2->kind<<parp3->kind<<" "<<parp2->num<<" "<<parp3->num<<endl;
	Y=counterJSsimp(parp2,parp3,parF);
	if(Y==0)
	{
		return 0;
	}//输入有错误
	////输出/////////////////////////////////////////////////////////
	parp=parF->next;
	cout<<"Ans";
	cout<<k-1;
	cout<<"=";
	while(parp->kind!=')' || parp->num!=-125)
	{
		if(parp->kind=='(')
			cout<<'(';
		if(parp->kind==')')
			cout<<')';
		if(parp->kind=='N')
		{
			if(parp->num<=0)
			{
//				COUNTER_FSXS(parp->num);
				parp=parp->next;
				continue;
			}
/*			if(COUNTER_GSXS(parp->num*parp->num,2) == 3)
			{
				cout.flush();
				printf("%f",parp->num);
				cout.flush();
			}
			else
			{
				COUNTER_GSXS(parp->num*parp->num);
			}
*/		}
		if(parp->kind=='O')
			cout<<parp->oper;
		if(parp->kind=='F')
			cout<<parp->func;
		parp=parp->next;
	}
	cout<<endl;
	////输出/////////////////////////////////////////////////////////
	if(Y==1)
		goto AS;//还有括号
	parp=parF->next;
}
	//化简计算//****************************************///////////////////////////////////////////////////// 
	double dValue = parp->num;
	delete parp;
	delete parE;
	delete parF;
	return dValue;
}

double counter( char* cFunc )
{
	int k=1;
	COUNTER_RESULT *pF;
	pF=new COUNTER_RESULT;
	k++;

	double dValue = counterJS(cFunc, k, pF);
	delete pF;
	return dValue;
}