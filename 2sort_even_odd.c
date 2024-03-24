#include <stdio.h>

/*
void input (int arr[], int len)
{
	for (int i = 0; i < len; i++)
	{
		scanf("%d", &arr[i]);
	}
}
*/

void sort_even_odd (int n, int a[])
{
	for (int i = 0; i < (n-1); i++)
	{
		for (int j = 0; j < (n-1-i); j++)
		{
			int temp;
			if ((a[j]%2)>(a[j+1]%2))
			{
				temp = a[j+1];
				a[j+1] = a[j];
				a[j] = temp;
			}
		}
	}
}

/*

void aPrint(int *arr, int len)
{
	for (int i = 0; i < len; i++)
	{
	printf("%d ",arr[i]);
	}
}

int main() 
{
	int Size;
	printf("Enter array length\n");
	scanf("%d", &Size);
	printf("Enter array separated by space\n");
	int numbers[Size];
	input(numbers,Size);
	sort_even_odd(Size,numbers);
	aPrint(numbers,Size);
    return 0;
}

*/

