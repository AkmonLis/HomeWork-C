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

void Shift (int arr[], int len, int shift)
{
	for (int i = 0; i < shift; i++)
	{
		int temp = 0;
		for (int i = (len-1); i > 0; --i)
		{
			temp = arr[i-1];
			arr[i-1] = arr[i];
			arr[i] = temp;
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
	Shift(numbers,Size,ShiftRight);
	aPrint(numbers,Size);
    return 0;
}
