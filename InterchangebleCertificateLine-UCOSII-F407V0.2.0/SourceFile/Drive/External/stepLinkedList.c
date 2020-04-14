#include "Resources.h"
#include "stepLinkedList.h"

/*-------------------------------<< 模块 >>-----------------------------------*/
/*
 * - :CRJ-
 * - :201700721.001.00.1
 * - System->APP->Drive->CPU，注意包含顺序
 * - 4类型函数，初始化配置，控制，状态，执行
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

/*-----------------------创建一个结点-----------------------------------*/
//优化
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

/*-----------------------返回第一个结点-----------------------------------*/
static Node* begin(List *list) 
{
  return list->headNode->next;
}

/*-----------------------返回最后一个结点的下一个结点-----------------------------------*/
static Node* end(List *list) 
{
  return list->lastNode->next;
}

/*-----------------------将元素插入链表中中，要求按顺序排列----------------------------------*/
void insertList(List *list, Node *pos,float stopPostion,enum _eAction action)
{
  //step 1:创建一个新的结点
  Node *s = CreateNode(stopPostion,action);
  //step 2:确定待插入的位置
  Node *p = list->headNode;
  while (p->next != pos)
    p = p->next;
  //step 3:插入结点
  s->next = p->next;
  p->next = s;
  //step 4:判断结点是否插入刀链表的表尾,若是更新
  if (pos == NULL)
    list->lastNode = s;
  //step 5:更新链表长度
  list->nodeCount++;
}

/*-----------------------初始化单链表----------------------------------*/
void listInit(List * list)
{
	list->headNode =list->lastNode = (Node*)malloc(sizeof(Node));
	if(list->headNode == NULL){
	 printf("list->headNode is wrong\r\n");
	}
	list->headNode->next = NULL;
	list->nodeCount = 0;
}
/*----------------------插入最后一个结点----------------------------------*/
void pushBack(List *list, float stopPostion,enum _eAction action) 
{
		insertList(list, end(list),stopPostion,action);
}

/*-----------------------插入第一个结点----------------------------------*/
void pushFront(List *list, float stopPostion,enum _eAction action) 
{
//		//step 1:创建一个新节点
//	Node *s = (Node*)malloc(sizeof(Node));
//	if(s == NULL){
//	 printf(" push_front s is wrong\r\n");
//	}
//  s->Action = action;
//	s->StopPostion = stopPostion;
//	s->next = NULL;
//		//step 2:插入头部
//	s->next = list->headNode->next;
//	list->headNode->next = s;
//	//step 3: 判断插入的节点时候是单链表的第一节点，若是更新尾巴
//	if(list->nodeCount == 0)
//		list->lastNode = s;
//	//step 4:更新单链表长度
//	list->nodeCount ++;
	insertList(list, begin(list),stopPostion,action);
	
}
/*-----------------------打印链表----------------------------------*/
void showList(List *list) 
{
  //step 1:指针p只想单链表的第一个节点
  Node *p = list->headNode->next;
  //step 2:打印节点
  while (p != NULL) {
    printf("%d->", (int)p->StopPostion);
    p = p->next;
  }
  printf("Nul.\r\n");
}

/*-----------------------删除最后一个元素----------------------------------*/
void popBack(List *list)
{
  //step 1:判断单链表是否空
  if (list->nodeCount == 0) return;
  //step 2:
  Node *p = list->headNode;//从头节点开始
  while (p->next != list->lastNode)
    p = p->next;
  //step 3:删除
  free(list->lastNode);
  list->lastNode = p;
  list->lastNode->next = NULL;
  //step 4:
  list->nodeCount--;
}

/*-----------------------//删除第一个节点---------------------------------*/
void popFront(List *list) 
{
  //step 1:
  if (list->nodeCount == 0) return;
  //step 2:定义指针p只想目标结点的前一个结点
  Node *p = list->headNode->next;
  //step 3:删除目标结点
  list->headNode->next = p->next;
  free(p);
  //step 4:
  if (list->nodeCount == 1)
    list->lastNode = list->headNode;
  //step 4: 更新长度
  list->nodeCount--;
}

/*-----------------------将数据元素插入到单链表中---------------------------------*/
void insertVal(List *list, float stopPostion,enum _eAction action)
{
	Node *s = CreateNode(stopPostion,action);
	//定义指针p指向待插入位置的前一个结点
	Node *p = list->headNode;
	while(p->next != NULL && p->next->StopPostion < s->StopPostion)
	p = p->next;

	if(p->next ==NULL)
	list->lastNode = s;
	
	s->next = p->next;
	p->next = s;
	
	list->nodeCount ++;
}	

/*-----------------------根据元素寻找结点--------------------------------*/
Node* find(List *list,enum _eAction action)
{
	  Node *p = list->headNode->next;
  while (p != NULL && p->Action != action)
    p = p->next;
  return p;
}

/*-----------------------返回链表长度--------------------------------*/
int readListLenth(List *list)
{
	return list->nodeCount;
}

/*-----------------------根据元素删除结点--------------------------------*/
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

/*-----------------------排序--------------------------------*/
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
/*----------------------数据溢出设置链表-------------------------------*/
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

/*-----------------------逆置单链表-------------------------------*/
void reverseList(List *list)//
{
	
}

/*----------------------清除链表-------------------------------*/
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

/*-----------------------摧毁------------------------------*/
void destroyList(List *list) 
{
  //step 1:
  clearList(list);
  //step 2:
  free(list->headNode);
  //step 3:
  list->headNode = list->lastNode = NULL;
}
