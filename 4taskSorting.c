#include <stdio.h>

const static int Size = 10;

void input (int arr[], int len)
{
	for (int i = 0; i < len; i++)
	{
		scanf("%d", &arr[i]);
	}
}

void Sort (int arr[], int len)
{
	int temp;
	for (int i = 0 ; i < (Size-1); i++)
	{
		for (int i = 0 ; i < (Size-1); i++)
		{
			if ((arr[i]%10)>(arr[i+1]%10))
			{
				temp = arr[i];
				arr[i] = arr[i+1];
				arr[i+1] = temp;
			}
		}
	}
}

void aPrint(int *arr, int len)
{
	for (int i = 0; i < len; i++)
	{
		printf("%d ",arr[i]);
	}
}

int main() 
{
	int numbers[Size];
	input(numbers,Size);
	Sort(numbers,Size);
	aPrint(numbers,Size);
	return 0;
}
