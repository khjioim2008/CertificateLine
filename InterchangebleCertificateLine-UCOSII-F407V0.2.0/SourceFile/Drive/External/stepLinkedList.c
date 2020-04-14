#include "Resources.h"
#include "stepLinkedList.h"

/*-------------------------------<< ģ�� >>-----------------------------------*/
/*
 * - :CRJ-
 * - :201700721.001.00.1
 * - System->APP->Drive->CPU��ע�����˳��
 * - 4���ͺ�������ʼ�����ã����ƣ�״̬��ִ��
 * - 
 * - :
 **/
/*___________________________________END______________________________________*/

Node *stepNodeCreate(float stopPostion,enum  _eAction action)
{
	Node *newNode =NULL;
	newNode = malloc(sizeof(Node));
	if(newNode == NULL){
		printf("newNode malloc is wrong\r\n");
		return NULL;
	}
	memset(newNode,0,sizeof(Node));
	newNode->StopPostion = stopPostion;
	newNode->Action     = action;
	newNode->next = NULL;
	return newNode;
}

/*-----------------------����һ�����-----------------------------------*/
//�Ż�
Node* CreateNode(float stopPostion,enum _eAction action)
{
  Node *s = (Node*)malloc(sizeof(Node));
  if (s == NULL){
		printf("CreateNode is wrong");
	}
  s->Action = action;
	s->StopPostion = stopPostion;
  s->next = NULL;
  return s;
}

/*-----------------------���ص�һ�����-----------------------------------*/
static Node* begin(List *list) 
{
  return list->headNode->next;
}

/*-----------------------�������һ��������һ�����-----------------------------------*/
static Node* end(List *list) 
{
  return list->lastNode->next;
}

/*-----------------------��Ԫ�ز����������У�Ҫ��˳������----------------------------------*/
void insertList(List *list, Node *pos,float stopPostion,enum _eAction action)
{
  //step 1:����һ���µĽ��
  Node *s = CreateNode(stopPostion,action);
  //step 2:ȷ���������λ��
  Node *p = list->headNode;
  while (p->next != pos)
    p = p->next;
  //step 3:������
  s->next = p->next;
  p->next = s;
  //step 4:�жϽ���Ƿ���뵶����ı�β,���Ǹ���
  if (pos == NULL)
    list->lastNode = s;
  //step 5:����������
  list->nodeCount++;
}

/*-----------------------��ʼ��������----------------------------------*/
void listInit(List * list)
{
	list->headNode =list->lastNode = (Node*)malloc(sizeof(Node));
	if(list->headNode == NULL){
	 printf("list->headNode is wrong\r\n");
	}
	list->headNode->next = NULL;
	list->nodeCount = 0;
}
/*----------------------�������һ�����----------------------------------*/
void pushBack(List *list, float stopPostion,enum _eAction action) 
{
		insertList(list, end(list),stopPostion,action);
}

/*-----------------------�����һ�����----------------------------------*/
void pushFront(List *list, float stopPostion,enum _eAction action) 
{
//		//step 1:����һ���½ڵ�
//	Node *s = (Node*)malloc(sizeof(Node));
//	if(s == NULL){
//	 printf(" push_front s is wrong\r\n");
//	}
//  s->Action = action;
//	s->StopPostion = stopPostion;
//	s->next = NULL;
//		//step 2:����ͷ��
//	s->next = list->headNode->next;
//	list->headNode->next = s;
//	//step 3: �жϲ���Ľڵ�ʱ���ǵ�����ĵ�һ�ڵ㣬���Ǹ���β��
//	if(list->nodeCount == 0)
//		list->lastNode = s;
//	//step 4:���µ�������
//	list->nodeCount ++;
	insertList(list, begin(list),stopPostion,action);
	
}
/*-----------------------��ӡ����----------------------------------*/
void showList(List *list) 
{
  //step 1:ָ��pֻ�뵥����ĵ�һ���ڵ�
  Node *p = list->headNode->next;
  //step 2:��ӡ�ڵ�
  while (p != NULL) {
    printf("%d->", (int)p->StopPostion);
    p = p->next;
  }
  printf("Nul.\r\n");
}

/*-----------------------ɾ�����һ��Ԫ��----------------------------------*/
void popBack(List *list)
{
  //step 1:�жϵ������Ƿ��
  if (list->nodeCount == 0) return;
  //step 2:
  Node *p = list->headNode;//��ͷ�ڵ㿪ʼ
  while (p->next != list->lastNode)
    p = p->next;
  //step 3:ɾ��
  free(list->lastNode);
  list->lastNode = p;
  list->lastNode->next = NULL;
  //step 4:
  list->nodeCount--;
}

/*-----------------------//ɾ����һ���ڵ�---------------------------------*/
void popFront(List *list) 
{
  //step 1:
  if (list->nodeCount == 0) return;
  //step 2:����ָ��pֻ��Ŀ�����ǰһ�����
  Node *p = list->headNode->next;
  //step 3:ɾ��Ŀ����
  list->headNode->next = p->next;
  free(p);
  //step 4:
  if (list->nodeCount == 1)
    list->lastNode = list->headNode;
  //step 4: ���³���
  list->nodeCount--;
}

/*-----------------------������Ԫ�ز��뵽��������---------------------------------*/
void insertVal(List *list, float stopPostion,enum _eAction action)
{
	Node *s = CreateNode(stopPostion,action);
	//����ָ��pָ�������λ�õ�ǰһ�����
	Node *p = list->headNode;
	while(p->next != NULL && p->next->StopPostion < s->StopPostion)
	p = p->next;

	if(p->next ==NULL)
	list->lastNode = s;
	
	s->next = p->next;
	p->next = s;
	
	list->nodeCount ++;
}	

/*-----------------------����Ԫ��Ѱ�ҽ��--------------------------------*/
Node* find(List *list,enum _eAction action)
{
	  Node *p = list->headNode->next;
  while (p != NULL && p->Action != action)
    p = p->next;
  return p;
}

/*-----------------------����������--------------------------------*/
int readListLenth(List *list)
{
	return list->nodeCount;
}

/*-----------------------����Ԫ��ɾ�����--------------------------------*/
void deleteVal(List *list, enum _eAction action)
{

	if(list->nodeCount == 0)return;
	Node *p = find(list,action);
	
	if(p ==NULL)
	{
		printf("This node is not exist");
		return;
	}
	if(p == list->lastNode){
		popBack(list);
	}else{
		Node *q        = p->next;
		p->Action     = q->Action;
		p->StopPostion = q->StopPostion;
		p->next        = q->next;
		free(q);
		list->nodeCount --;
	}
}

/*-----------------------����--------------------------------*/
void sortList(List *list) 
{
	if(list->nodeCount == 0 || list->nodeCount ==1)
		return;
	Node *s = list->headNode->next;
	Node *p = s->next;
	list->lastNode = s;
	list->lastNode->next = NULL;
	while(p!=NULL)
	{
		s = p;
		p = p->next;
		Node *q = list->headNode;
		while(q->next != NULL && q->next->StopPostion < s->StopPostion)
			q = q->next;
		
		if(q->next == NULL)
			list->lastNode = s;
		  
		 s->next = q->next;
		  q->next = s ;
	}
}
/*----------------------���������������-------------------------------*/
void overFlowSetList(List *list,float temp) 
{
	if (list->nodeCount == 0) return;
	Node *p = list->headNode->next;
	while(p != NULL)
	{
		p->StopPostion = p->StopPostion - temp;
		p = p->next;
	}	
}

/*-----------------------���õ�����-------------------------------*/
void reverseList(List *list)//
{
	
}

/*----------------------�������-------------------------------*/
void clearList(List *list) 
{
	if (list->nodeCount == 0) return;
	Node *p = list->headNode->next;
	while(p != NULL)
	{
		list->headNode->next = p->next;
		free(p);
		p = list->headNode->next;
	}
	list->lastNode = list->headNode;
	list->nodeCount = 0;
	
}

/*-----------------------�ݻ�------------------------------*/
void destroyList(List *list) 
{
  //step 1:
  clearList(list);
  //step 2:
  free(list->headNode);
  //step 3:
  list->headNode = list->lastNode = NULL;
}
