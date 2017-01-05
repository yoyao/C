

#include <stdio.h>
#include <stdlib.h>
#include "LinkList.h"


#define LISTCOUNTLIMT 9999

ListNode* CreatNode()
{
	ListNode* p_node =(ListNode*)malloc(sizeof(ListNode));
	p_node->NextNode =NULL;
	p_node->PrevNode =NULL;
	return p_node;
}


unsigned int GetListCount(LinkList* plinklist)//获取LinkList的节点数量
{
	return plinklist->Count;
}

void InitLinkList(LinkList* plinklist)//初始化链表，将链表清空
{
	ClearListNode(plinklist->HeadNode);
	plinklist->LastNode=NULL;
	plinklist->Count =0;
}

unsigned int ClearListNode(ListNode* HeadNode)//清空给定节点的子节点
{
	ListNode* tmp =HeadNode->NextNode;
	ListNode* temp=NULL;
	unsigned int ui_sign=0;
	while (tmp)
	{
		temp =tmp;
		tmp =tmp->NextNode;
		free(temp);
		ui_sign++;
	}
	return ui_sign;
}

int InsertListNodeOfPosi(LinkList* plinklist,const unsigned int posi,ListNode* p_node)//在指定位置处插入ListNode
{
		//待实现
	return 1;
}

int GetListNodeOfPosi(LinkList* plinklist,const unsigned int posi)//获取指定位置的ListNode
{
		//待实现
		return 1;
}

int PushBack(LinkList* plinklist,ListNode* p_node)//在链表最后添加Node
{
	//待实现
	return plinklist->Count;
}


struct Student{
	ListNode* p_Node;
	unsigned int Age;
	unsigned char Name[32];
};


int main(int argc,char* argv[])
{
	/*
	通用链表的实现思路（其实感觉数据项用void*也可以实现）：
	    链表中的数据项因为业务不同所以是不能通用的。
		但是可以用数据项包含链表（或链表指针） 但是要把链表放在数据项的首位 这样链表的指针指向下一个链表 
		而这时候链表的地址就是这个数据项的首地址 强转一下指针类型 也就可以得到这个数据项的全部内容
	*/
	struct Student stu ={NULL,23,"Tom"};//第一个stu
	
	struct Student stu1 ={NULL,39,"Jerry"};//第2个stu

	struct Student* p_stu =NULL;

	LinkList* p_list = (LinkList*)calloc(sizeof(LinkList),1);

	ListNode* p_node=CreatNode();

	ListNode* p_node1=CreatNode();

	p_list->HeadNode = p_node;//设置头节点

	stu.p_Node =p_node;//设置业务数据的链表节点

	stu.p_Node->NextNode =p_node1;//设置上一个业务数据的链表节点的NextNode

	stu1.p_Node=p_node1;//设置下一个业务数据的链表节点

	p_stu = (struct Student*)&(stu1.p_Node);//得到数据项的链表地址 一强转 即数据项的首地址  即可随意操作数据项
	

	printf("age is %d\n",p_stu->Age);


}


