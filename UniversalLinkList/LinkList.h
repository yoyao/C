

#ifndef _LINKLIST_H_

#define _LINKLIST_H_
#include <stdio.h>
#include <stdlib.h>



typedef struct listnode ListNode;

struct listnode{
	ListNode* PrevNode;//上一个节点
	ListNode* NextNode;//下一个节点
};

typedef struct link_list LinkList;

struct link_list{
	unsigned int Count;//子节点数量
	ListNode* HeadNode;//头节点
	ListNode* LastNode;//尾节点
};
ListNode* CreatNode();//创建节点，返回指针

void InitLinkList(LinkList* plinklist);//初始化链表，将链表清空

unsigned int ClearListNode(ListNode* HeadNode);//清空给定节点的子节点

unsigned int GetListCount();//获取LinkList的节点数量

int InsertListNodeOfPosi(LinkList* plinklist,const unsigned int posi,ListNode* p_node);//在指定位置处插入ListNode

ListNode* GetListNodeOfPosi(LinkList* plinklist,const unsigned int posi);//获取指定位置的ListNode

int ListPushBack(LinkList* plinklist,ListNode* p_node);//在链表最后添加Node

#endif
