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

void aPrint(int *arr, int len)
{
	for (int i = 0; i < len; i++)
	{
		printf("%d ",arr[i]);
	}
}

void Shift (int arr[], int len, int shift)
{
	int arrShift[Size];
	for (int i = 0, j = shift; i < Size; i++ , j++)
	{
		if (j >= Size)
		{
			j = 0;
		}
		arrShift[j] = arr[i];
	}
	for (int i = 0; i < Size; i++)
	{
		arr[i] = arrShift[i];
	}
}





int main() 
{
	int numbers[Size];
	input(numbers,Size);
	Shift(numbers,Size,ShiftRight);
	aPrint(numbers,Size);
	return 0;
}
