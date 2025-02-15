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


//åå§åé¾è¡?ï¼åªå»ºè¡¨å¤´ï¼
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

//æå¥åç´ 
int CLinkListManager::InsertLinkList(LinkList &L, int nInsertPoint, ElemType nInsertValue)
{
	LinkList p = NULL, r = NULL;
	int i = 0;

	if (NULL == L)
	{
		//		printf("è¡¨å¤´ä¸ºç©ºï¼è¯·ååå§å\n");
		return -1;
	}

	r = (LinkList)malloc(sizeof(LNode));
	if (NULL == r)
	{
		//		printf("åå­åéå¤±è´¥\n");
		free(r);
		return -1;
	}
	r->data = nInsertValue;

	p = L;
	while ((NULL != p) && (i < nInsertPoint - 1))   //åæ³æå¥ç¹ï¼æéç§»å¨ï¼å®ä½å°å¸¦æå¥ç¹ä¹å
	{
		p = p->next;
		i++;
	}

	while ((NULL == p) || (i > nInsertPoint - 1))   //éæ³æå¥ç¹ã?åèï¼æå¥ç¹è¶çï¼ åèï¼æå¥ç?=0ï¼?
	{
		//		printf("éæ³æå¥ç¹ï¼è¯·æ£æ¥åæ°\n");
		return -1;
	}

	r->next = p->next;
	p->next = r;

	return 0;
}

//æ ¡éªå¤´æéåå¶nextåæ¯å¦ä¸ºç©?
int CLinkListManager::CheckLinkList(LinkList L)
{
	if (NULL == L)
	{
		//		printf("æéä¸ºç©ºï¼è¯·æ£æ¥åæ°\n");
		return -1;
	}
	if (NULL == L->next)
	{
		//		printf("é¾è¡¨ä¸ºç©ºï¼æ æ³éæ¯?\n");
		return -1;
	}
	return 0;
}

//å é¤èç¹
int CLinkListManager::DeleteLinkList(LinkList &L, int nDeletePoint)
{
	LinkList p = NULL, r = NULL;
	int i = 0;

	CheckLinkList(L);

	p = L;
	while ((NULL != p) && (i < nDeletePoint - 1))        //æéç§»å¨å°å é¤ç¹ä¹å
	{
		p = p->next;
		i++;
	}

	while ((NULL == p) || (i > nDeletePoint - 1))        //éæ³å é¤ç¹ãåèï¼æ¯å¦è¶çï¼?åèï¼å é¤ç¹æ¯å?=0
	{
		//		printf("éæ³å é¤ç?\n");
		return -1;
	}

	//å é¤èç¹
	r = p->next;
	p->next = r->next;

	free(r);
	return 0;
}

//éæ¯é¾è¡?
void CLinkListManager::DestroyLinkList(LinkList &L)
{
	LinkList p = NULL, r = NULL;

	CheckLinkList(L);

	p = L->next;        //æ³¨æï¼æ­¤å¤ä¸æ¯p = L;
	while (NULL != p)
	{
		r = p;
		p = p->next;
		free(r);
	}
	p = L;	// éæ¯å¤´
	free(p);

	//	printf("æåéæ¯é¾è¡¨\n");
}

//éè¿æ¥æ¾ç¹ï¼æ¥æ¾åç´ å?ï¼è¿åp->dataï¼?
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
	while ((NULL == p) || (i > nFindPoint) || 0 == nFindPoint)            //éæ³æ¥æ¾ç¹ãåèï¼æ¯å¦è¶çï¼?åèï¼å é¤ç¹æ¯å?=0
	{
		//		printf("éæ³æ¥æ¾ç?\n");
		NULL;
	}

	return p->data;
}

//éè¿åç´ æ¥æ¾ï¼è¥å­å¨ï¼è¿åå¶ä½åºï¼å¦åè¿å?1
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
		return -1;        //æ²¡æ¾å?
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
