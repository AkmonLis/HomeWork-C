#include <stdio.h>

const static int Size = 12;
const static int ShiftRight = 4;


void input (int arr[], int len)
{
	for (int i = 0; i < len; i++)
	{
		scanf("%d", &arr[i]);
	}
}


void aPrint(int *arr, int len, int a)
{
	for (int i = a; i < len; i++)
	{
		printf("%d ",arr[i]);
	}
}

int main() 
{
	int numbers[Size];
	input(numbers,Size);
	aPrint(numbers,Size,(Size-ShiftRight));
	aPrint(numbers,(Size-ShiftRight),(Size-Size));
    return 0;
}
