

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


unsigned int GetListCount(LinkList* plinklist)//��ȡLinkList�Ľڵ�����
{
	return plinklist->Count;
}

void InitLinkList(LinkList* plinklist)//��ʼ���������������
{
	ClearListNode(plinklist->HeadNode);
	plinklist->LastNode=NULL;
	plinklist->Count =0;
}

unsigned int ClearListNode(ListNode* HeadNode)//��ո����ڵ���ӽڵ�
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

int InsertListNodeOfPosi(LinkList* plinklist,const unsigned int posi,ListNode* p_node)//��ָ��λ�ô�����ListNode
{
		//��ʵ��
	return 1;
}

int GetListNodeOfPosi(LinkList* plinklist,const unsigned int posi)//��ȡָ��λ�õ�ListNode
{
		//��ʵ��
		return 1;
}

int PushBack(LinkList* plinklist,ListNode* p_node)//������������Node
{
	//��ʵ��
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
	ͨ�������ʵ��˼·����ʵ�о���������void*Ҳ����ʵ�֣���
	    �����е���������Ϊҵ��ͬ�����ǲ���ͨ�õġ�
		���ǿ������������������������ָ�룩 ����Ҫ������������������λ ���������ָ��ָ����һ������ 
		����ʱ������ĵ�ַ���������������׵�ַ ǿתһ��ָ������ Ҳ�Ϳ��Եõ�����������ȫ������
	*/
	struct Student stu ={NULL,23,"Tom"};//��һ��stu
	
	struct Student stu1 ={NULL,39,"Jerry"};//��2��stu

	struct Student* p_stu =NULL;

	LinkList* p_list = (LinkList*)calloc(sizeof(LinkList),1);

	ListNode* p_node=CreatNode();

	ListNode* p_node1=CreatNode();

	p_list->HeadNode = p_node;//����ͷ�ڵ�

	stu.p_Node =p_node;//����ҵ�����ݵ�����ڵ�

	stu.p_Node->NextNode =p_node1;//������һ��ҵ�����ݵ�����ڵ��NextNode

	stu1.p_Node=p_node1;//������һ��ҵ�����ݵ�����ڵ�

	p_stu = (struct Student*)&(stu1.p_Node);//�õ�������������ַ һǿת ����������׵�ַ  �����������������
	

	printf("age is %d\n",p_stu->Age);


}


