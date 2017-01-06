

#ifndef _LINKLIST_H_

#define _LINKLIST_H_
#include <stdio.h>
#include <stdlib.h>



typedef struct listnode ListNode;

struct listnode{
	ListNode* PrevNode;//��һ���ڵ�
	ListNode* NextNode;//��һ���ڵ�
};

typedef struct link_list LinkList;

struct link_list{
	unsigned int Count;//�ӽڵ�����
	ListNode* HeadNode;//ͷ�ڵ�
	ListNode* LastNode;//β�ڵ�
};
ListNode* CreatNode();//�����ڵ㣬����ָ��

void InitLinkList(LinkList* plinklist);//��ʼ���������������

unsigned int ClearListNode(ListNode* HeadNode);//��ո����ڵ���ӽڵ�

unsigned int GetListCount();//��ȡLinkList�Ľڵ�����

int InsertListNodeOfPosi(LinkList* plinklist,const unsigned int posi,ListNode* p_node);//��ָ��λ�ô�����ListNode

ListNode* GetListNodeOfPosi(LinkList* plinklist,const unsigned int posi);//��ȡָ��λ�õ�ListNode

int ListPushBack(LinkList* plinklist,ListNode* p_node);//������������Node

#endif
