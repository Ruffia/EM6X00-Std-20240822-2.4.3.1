#pragma once

//修改时间：  2020-08-21
//修改人：  周宏
//修改原因：将原LinkList相关功能从 App 类中独立出来
//功能： LinkList管理器


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
