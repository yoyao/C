
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

ListNode* GetListNodeOfPosi(LinkList* plinklist,const unsigned int posi)//��ȡָ��λ�õ�ListNode
{
	   ListNode* p_tmp =NULL;
	  unsigned int ui_sign=0;
	  //�˴�����дposi>=-1 ��Ϊposi���޷�������
	  if(posi>=0 && posi<plinklist->Count)
	  {
		p_tmp =  plinklist->HeadNode;
	   while (ui_sign < posi)
	   {
		   if(p_tmp->NextNode ==NULL)
		   {
			   return NULL;
		   }
		   p_tmp =p_tmp->NextNode;
		   ui_sign++;
	   }

	  }
	  
		return p_tmp;
}

int ListPushBack(LinkList* plinklist,ListNode* p_node)//������������Node
{
	if(plinklist->LastNode)//���һ���ڵ㲻����NULL
	{
	plinklist->LastNode->NextNode = p_node;
	}
	if(!plinklist->HeadNode)//�����ڿգ��ͰѴ˽ڵ�����Ϊͷ�ڵ�
	{
		plinklist->HeadNode=p_node;
	}
	plinklist->Count+=1;
	plinklist ->LastNode =p_node;
	//��ʵ��
	return plinklist->Count;
}

//���ݰ�������ڵ� �����ܰ�������ڵ�ָ��
struct Student{
	ListNode p_Node;
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

	/*
	ListNode* p_node=CreatNode();

	ListNode* p_node1=CreatNode();
	*/
	LinkList* p_list = (LinkList*)calloc(sizeof(LinkList),1);
	int count=0;
	struct Student stu;
	struct Student stu1;
	struct Student* p_stu =NULL;

	stu.Age =23;
	strcpy((char*)stu.Name,"Tom");


	//��2��stu
	stu1.Age = 54;
	strcpy((char*)stu1.Name,"Jerry");


	ListPushBack(p_list,(ListNode*)&stu);
    ListPushBack(p_list,(ListNode*)&stu1);

	for(count=0;count<2;count++)
	{
		p_stu=(struct Student* )GetListNodeOfPosi(p_list,count);
		printf("name is %s\n age is %d\n ",p_stu->Name,p_stu->Age);
	}

	/*
	//�˴�Ϊ�������������ڵ�ָ�� ���ǲ�����
	p_list->HeadNode = p_node;//����ͷ�ڵ�

	stu.p_Node =p_node;//����ҵ�����ݵ�����ڵ�

	stu.p_Node->NextNode =p_node1;//������һ��ҵ�����ݵ�����ڵ��NextNode

	stu1.p_Node=p_node1;//������һ��ҵ�����ݵ�����ڵ�
	p_node1->PrevNode =p_node;//��һ���ڵ�
	p_stu =&stu;
	p_stu = (struct Student*)(stu.p_Node->NextNode);//�õ�������������ַ һǿת ����������׵�ַ  �����������������

	*/
		


}


