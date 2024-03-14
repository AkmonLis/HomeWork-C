#include <stdio.h>

const static int Size = 5;

void input (int arr[], int len)
{
	for (int i = 0; i < len; i++)
	{
		scanf("%d", &arr[i]);
	}
}

int min (int arr[], int len)
{
	int min = arr[0];
	for (int i = 1; i < Size; ++i)
	{
		if (min > arr[i])
		min = arr[i];
	}
	return min;
}


int main() 
{
	int numbers[Size];
	input(numbers,Size);
	printf("average = %d", min(numbers,Size));
    return 0;
}
