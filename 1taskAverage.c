#include <stdio.h>

const static int Size = 5;

void input (int arr[], int len)
{
	for (int i = 0; i < len; i++)
	{
		scanf("%d", &arr[i]);
	}
}
float average (int arr[], int len)
{
	float sum;
	for (int i = 0; i < Size; ++i)
	{
		sum += arr[i];
	}
	return sum/Size;
}





int main() 
{
	int numbers[Size];
	input(numbers,Size);
	printf("average = %.3f", average(numbers,Size));
	return 0;
}
