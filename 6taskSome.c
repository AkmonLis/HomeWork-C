#include <stdio.h>


const static int Size = 10;


void input (int arr[], int len)
{
	for (int i = 0; i < len; i++)
	{
		scanf("%d", &arr[i]);
	}
}


int max (int *array, int len)
{
	int indexMax;
	int max = array[0];
	for (int i = 1; i < Size; i++)
	{
		if (max <= array[i])
		{
		max = array[i];
		indexMax = i;
		}
	}
	return indexMax;
}


int moreOften (int *arr, int size)
{
	int arrCount[Size];
	for (int i = 0 ; i < Size; i++)
	{
		arrCount[i] = 0;
		for (int j = 0 ; j < Size; j++)
		if (arr[i] == arr[j])
		{
			arrCount[i]++;
		}
	}
	return max(arrCount, Size);
}


int main() 
{
	int numbers[Size];
	input(numbers,Size);
	printf("%d", numbers[moreOften(numbers,Size)]);
	return 0;
}
