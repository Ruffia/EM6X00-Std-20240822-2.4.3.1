#pragma once

//�޸�ʱ�䣺  2020-08-21
//�޸��ˣ�  �ܺ�
//�޸�ԭ�򣺽�ԭLinkList��ع��ܴ� App ���ж�������
//���ܣ� LinkList������


typedef double ElemType;

typedef struct node
{
	ElemType data;
	struct node* next;
}LNode, *LinkList;


typedef struct tagDL_Graph_Param_LinkTest
{
	BOOL	bGraph;
	int		nIndex;
	LinkList link;
	int		nZoomXIndex;
	int		nZoomYIndex;
	double	dXMin;
	double	dXMax;
	double	dYMin;
	double	dYMax;
}DL_Graph_Param_LinkTest;
///////////////////////////////////////

class CLinkListManager  
{

public:
	static CLinkListManager& Instance();

	int InitLinkList(LinkList &L);
	int InsertLinkList(LinkList &L, int nInsertPoint, ElemType nInsertValue);
	int CheckLinkList(LinkList L);
	int DeleteLinkList(LinkList &L, int nDeletePoint);
	void DestroyLinkList(LinkList &L);
	ElemType FindElem(LinkList L, int nFindPoint);
	int LocateElem(LinkList L, ElemType elem);
	int LengthOfLinkList(LinkList L);

protected:
	CLinkListManager();

};
