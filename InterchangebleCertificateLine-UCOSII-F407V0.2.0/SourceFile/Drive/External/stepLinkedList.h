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

typedef struct Node{ //ͨ�ý��
	float StopPostion;
	enum  _eAction Action;
	struct Node* next;
}Node,*PNode;

//����
typedef struct _tList
{
	PNode headNode; //ͷ�ڵ�
	PNode lastNode;//β�ڵ�
	uint32_t nodeCount; //������ 
}List;

extern void insertList(List *list, Node *pos,float stopPostion,enum _eAction action);

extern void listInit(List * list);

extern void pushBack(List *list, float stopPostion,enum _eAction action);

extern void pushFront(List *list, float stopPostion,enum _eAction action);

extern void showList(List *list);

extern void popBack(List *list);

extern void popFront(List *list);

extern void insertVal(List *list, float stopPostion,enum _eAction action);//��ֵ����

extern Node* find(List *list,enum _eAction action);

extern int readListLenth(List *list);//���س���

extern void deleteVal(List *list, enum _eAction action);//��ֵɾ��

extern void sortList(List *list);//����

extern void overFlowSetList(List *list,float temp); 

extern void reverseList(List *list);//���õ�����

extern void clearList(List *list);//���������

extern void destroyList(List *list);//�ݻٵ�����



#endif
