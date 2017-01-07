/*归并排序的源文件*/

//归并排序
//1  5  9  15 23 32
//56 50 34 19 13
//2,10,12,25,38,56,58,71,
//9,21,29,30,46,66,81,94
#include <stdio.h>
#include <stdlib.h>
extern void printdata(int* arr,int count);

/*
原来的想法是 
只要left小于中值 中值小于后值 即可循环比较
*/
void Merge(int arr[], int left, int mid, int right,int count)
{
	int* p_tmp = (int*)malloc(sizeof(int)*count+1);
	int i_sign=left,i_tmp_mid=mid+1,i_tmp_left=left,i_tmp_left_t=left;

#pragma region 原来自己的方法

/* 2016-12-16 丙申猴年 酉时 临大运通下班
原来的想法是 初始传入left值为数组的中间index right值为数组的长度
在循环比较中
只要left小于中值 中值小于后值 即可循环比较 写法如下：
while(i_tmp_left<mid && i_tmp_mid<right)
但是实际是要将left至mid中间所有值进行比较，如此写法会导致mid位置的值不会被比较，只比较至mid-1，所以应改成i_tmp_left<mid+1。同理，第一个循环处right亦该为right+1，此为改写法其一
且，比较相邻两个值时 中值mid比如为0，如若写成i_tmp_left<mid，第一轮循环进不去，也就是两个相邻的数据比较失败，算法错误。同理，right+1；此为改写法其二
*/
	while(i_tmp_left<mid+1 && i_tmp_mid<right+1)
	{
		if(arr[i_tmp_left]>arr[i_tmp_mid])
		{
			p_tmp[i_sign++]=arr[i_tmp_mid++];
		}
		else
		{
			p_tmp[i_sign++]=arr[i_tmp_left++];
		}
	}

	//也就是左面右没完事的 需要依次添加到数组中
	while(i_tmp_left<mid+1)
	{
		p_tmp[i_sign++]=arr[i_tmp_left++];
	}

	//也就是右面右没完事的 需要依次添加到数组中
	while(i_tmp_mid<right+1)
	{
		p_tmp[i_sign++]=arr[i_tmp_mid++];
	}
	/*
	原本写法为，使用i_sign记录p_tmp的添加数量，然后循环更新老数组，但是出错，遂仿写网上的写法。
	*/
	//循环添加至源数组
	for(int i=left;i<right+1;i++)
	{
		arr[i]=p_tmp[i];
	}

#pragma endregion


	free(p_tmp);
}


void MergeSort(int arr[], int left,int right,int count)
{
	if(left < right)
	{
		int mid =(right+left)/2;
		MergeSort(arr,left,mid,count);
		MergeSort(arr,mid+1,right,count);
		Merge(arr,left,mid,right,count);
	}

}







//从网上扒下来的好用的归并排序
void TMerge(int sourceArr[], int startIndex, int midIndex, int endIndex,int count)
{
	int len = sizeof(sourceArr);
	int* tempArr =(int*)malloc(count*sizeof(int));
	int i = startIndex, j=midIndex+1, k = startIndex;
	while(i!=midIndex+1 && j!=endIndex+1)
	{
		if(sourceArr[i] > sourceArr[j])
		{
			tempArr[k++] = sourceArr[j++];
		}
		else
		{
			tempArr[k++] = sourceArr[i++];
		}
	}
	while(i != midIndex+1)
	{
		tempArr[k++] = sourceArr[i++];
	}
	while(j != endIndex+1)
	{
		tempArr[k++] = sourceArr[j++];
	}

	for(i=startIndex; i<=endIndex; i++)
	{
		sourceArr[i] = tempArr[i];
	}
	free(tempArr);
}

//内部使用递归 从网上扒下来的好用的归并排序
void TMergeSort(int sourceArr[], int startIndex, int endIndex,int count)
{
	int midIndex;
	if(startIndex < endIndex)
	{
		midIndex = (startIndex + endIndex) / 2;
		TMergeSort(sourceArr, startIndex, midIndex,count);
		TMergeSort(sourceArr, midIndex+1, endIndex, count);
		TMerge(sourceArr,  startIndex, midIndex, endIndex,count);
	}
}


