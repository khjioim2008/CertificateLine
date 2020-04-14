#ifndef __STEPLINKEDLIST_H
#define	__STEPLINKEDLIST_H

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include  <stdlib.h>
#include  <string.h>
#include <stdio.h>

enum _eAction
{
	CutChamfer = 1,
	CutFlotasm = 2,
	StampAC    = 3
};

typedef struct Node{ //通用结点
	float StopPostion;
	enum  _eAction Action;
	struct Node* next;
}Node,*PNode;

//链表
typedef struct _tList
{
	PNode headNode; //头节点
	PNode lastNode;//尾节点
	uint32_t nodeCount; //计数器 
}List;

extern void insertList(List *list, Node *pos,float stopPostion,enum _eAction action);

extern void listInit(List * list);

extern void pushBack(List *list, float stopPostion,enum _eAction action);

extern void pushFront(List *list, float stopPostion,enum _eAction action);

extern void showList(List *list);

extern void popBack(List *list);

extern void popFront(List *list);

extern void insertVal(List *list, float stopPostion,enum _eAction action);//按值插入

extern Node* find(List *list,enum _eAction action);

extern int readListLenth(List *list);//返回长度

extern void deleteVal(List *list, enum _eAction action);//按值删除

extern void sortList(List *list);//排序

extern void overFlowSetList(List *list,float temp); 

extern void reverseList(List *list);//逆置单链表

extern void clearList(List *list);//清除单链表

extern void destroyList(List *list);//摧毁单链表



#endif
