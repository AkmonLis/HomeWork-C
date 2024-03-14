#include <stdio.h>

const static int Size = 10;


void input (int arr[], int len)
{
	for (int i = 0; i < len; i++)
	{
		scanf("%d", &arr[i]);
	}
}


void aPrint(int *arr, int len)
{
	for (int i = 0; i < len; i++)
	{
		printf("%d ",arr[i]);
	}
}


int Zero (int *arr, int *arr2, int len)
{
	int j = 0;
	for (int i = 0; i < Size; i++)
	{
		if (((arr[i]/10)%10) == 0)
		{
			arr2[j] = arr[i];
			j++;
		}
	}
	return j;
}



int main() 
{
	int numbers[Size], numbersZero[Size];
	input(numbers,Size);
	int newSize = Zero(numbers, numbersZero, Size);
	aPrint(numbersZero,newSize);
	return 0;
}
