#include "stdafx.h"
#include "LinkListManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
CLinkListManager& CLinkListManager::Instance()
{
	static CLinkListManager Inst;
	return Inst;
}

CLinkListManager::CLinkListManager()
{

}


//初始化链�?（只建表头）
int CLinkListManager::InitLinkList(LinkList &L)
{
	LinkList p = NULL;
	if (NULL == L)
	{
		p = (LinkList)malloc(sizeof(LNode));
		if (NULL == p)
		{
			free(p);
			return -1;
		}

		L = p;
		L->next = NULL;
	}
	return 0;
}

//插入元素
int CLinkListManager::InsertLinkList(LinkList &L, int nInsertPoint, ElemType nInsertValue)
{
	LinkList p = NULL, r = NULL;
	int i = 0;

	if (NULL == L)
	{
		//		printf("表头为空，请先初始化\n");
		return -1;
	}

	r = (LinkList)malloc(sizeof(LNode));
	if (NULL == r)
	{
		//		printf("内存分配失败\n");
		free(r);
		return -1;
	}
	r->data = nInsertValue;

	p = L;
	while ((NULL != p) && (i < nInsertPoint - 1))   //合法插入点，指针移动，定位到带插入点之前
	{
		p = p->next;
		i++;
	}

	while ((NULL == p) || (i > nInsertPoint - 1))   //非法插入点�?前者：插入点越界； 后者：插入�?=0�?
	{
		//		printf("非法插入点，请检查参数\n");
		return -1;
	}

	r->next = p->next;
	p->next = r;

	return 0;
}

//校验头指针和其next域是否为�?
int CLinkListManager::CheckLinkList(LinkList L)
{
	if (NULL == L)
	{
		//		printf("指针为空，请检查参数\n");
		return -1;
	}
	if (NULL == L->next)
	{
		//		printf("链表为空，无法销�?\n");
		return -1;
	}
	return 0;
}

//删除节点
int CLinkListManager::DeleteLinkList(LinkList &L, int nDeletePoint)
{
	LinkList p = NULL, r = NULL;
	int i = 0;

	CheckLinkList(L);

	p = L;
	while ((NULL != p) && (i < nDeletePoint - 1))        //指针移动到删除点之前
	{
		p = p->next;
		i++;
	}

	while ((NULL == p) || (i > nDeletePoint - 1))        //非法删除点。前者：是否越界�?后者：删除点是�?=0
	{
		//		printf("非法删除�?\n");
		return -1;
	}

	//删除节点
	r = p->next;
	p->next = r->next;

	free(r);
	return 0;
}

//销毁链�?
void CLinkListManager::DestroyLinkList(LinkList &L)
{
	LinkList p = NULL, r = NULL;

	CheckLinkList(L);

	p = L->next;        //注意：此处不是p = L;
	while (NULL != p)
	{
		r = p;
		p = p->next;
		free(r);
	}
	p = L;	// 销毁头
	free(p);

	//	printf("成功销毁链表\n");
}

//通过查找点，查找元素�?（返回p->data�?
ElemType CLinkListManager::FindElem(LinkList L, int nFindPoint)
{
	LinkList p = NULL;
	int i = 0;

	CheckLinkList(L);

	p = L;
	while ((NULL != p) && (i < nFindPoint))
	{
		p = p->next;
		i++;
	}
	while ((NULL == p) || (i > nFindPoint) || 0 == nFindPoint)            //非法查找点。前者：是否越界�?后者：删除点是�?=0
	{
		//		printf("非法查找�?\n");
		NULL;
	}

	return p->data;
}

//通过元素查找，若存在，返回其位序，否则返�?1
int CLinkListManager::LocateElem(LinkList L, ElemType elem)
{
	LinkList p = NULL;
	int nSeq = 1;

	CheckLinkList(L);

	p = L->next;
	while ((NULL != p) && (elem != p->data))
	{
		nSeq++;
		p = p->next;
	}

	if (NULL == p)
	{
		return -1;        //没找�?
	}
	else
	{
		return nSeq;    
	}
}

int CLinkListManager::LengthOfLinkList(LinkList L)
{
	LinkList p = NULL;
	int nLength = 0;

	CheckLinkList(L);

	p = L->next;
	while (NULL != p)
	{
		nLength++;
		p = p->next;
	}
	return nLength;
}
