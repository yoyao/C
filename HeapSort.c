
#include <stdio.h>
#include <stdlib.h>

extern void print_data(int* arr,int count)
{
	int i,size;
	size=0;
	for(i=0;i<count;i++)
	{
		printf("%d ",arr[i]);
		size++;
		if(i==(count-1))
		{
			printf("\n");
		}
	}
	printf("count is %d \n",size);
}
/*
堆排序：
堆排序首先要是完全二叉树（和满二叉树是两个东西），节点都偏左面，堆由上至下是上大于下 
满二叉树：
叶子出现在最底层，并且叶子在树的的左侧

堆的最上面的是最大的值

假设当前节点的编号是 j,则它左孩子节点的编号是2j（如果是从0开始算就是2j+1）,右孩子节点的编号是2j+1（如果是从0开始算就是2j+2）

堆排序的实现思路是 从下至上依次以每个节点作为根节点(构建堆)，与其两个孩子节点相比，最大的放最上面的即整个二叉树的根节点。
*/
void CreatHeap(int arr[],const unsigned int par_i_size,unsigned int par_i_current)
{

	unsigned int left = par_i_current*2+1;// 左侧的孩子节点
	unsigned int right = par_i_current*2+2;//右侧的孩子节点
	
	/*切记有符号（signed）和无符号(unsigned )中判断-1这类错误  */
	if(par_i_current <par_i_size && par_i_current >=0)
	{
		unsigned int i_curval =arr[par_i_current];//当前节点
		unsigned int i_big =0;
		unsigned int i_leftval = arr[left];//左侧孩子节点
		unsigned int i_rightval = arr[right];//右侧孩子节点
		int i_cur_val ;
		if(left<par_i_size)//不能越界
		{
			i_cur_val=arr[par_i_current];//当前位置的值
			if(i_cur_val<i_leftval)//如果小于左节点的值 就换位置
			{
				i_big=arr[par_i_current];
				arr[par_i_current]=i_leftval;
				arr[left]=i_big;
			}
		}

		if(right<par_i_size)//不能越界
		{
			i_cur_val=arr[par_i_current];//当前位置的值
			if(i_cur_val<i_rightval)//如果小于右节点的值 就换位置
			{
				i_big=arr[par_i_current];
				arr[par_i_current]=i_rightval;
				arr[right]=i_big;
			}
		}
		CreatHeap(arr,par_i_size,--par_i_current);
	}

}
//挑选出数组中最大的数字
void HeapSort(int arr[],unsigned int size)
{
	int max,i;
	//从数组的最后位置进行建堆操作
	for(i=(size-1);i>=0;i--)
	{
		CreatHeap(arr,(i+1),i);//每进行一次建堆操作 就是把数组从0开始至current位置筛选最大的数字
		max=arr[0];//得出的最大的数字 
		arr[0]=arr[i];
		arr[i]=max;//将最前面的（即最大的） 换到最后面
	}
}


int main(int argc,char* argv[])
{


	int arr_t[15]={49,38,65,97,26,13,27,29,55,37,15,12,1,985,30};

	int arr[] ={39,30,10};

	HeapSort(arr_t,15);
	
	print_data(arr_t,15);

	return 1;
}






















































































































































