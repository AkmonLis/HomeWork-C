#include <stdio.h>
/*

void input (int arr[], int n)
{
	for (int i = 0; i < n; i++)
	{
		scanf("%d", &arr[i]);
	}
}
*/

void sort_array (int size, int a[])
{
	int temp;
	for (int i = 0; i < (size-1); i++)
	{
		for (int j = 0; j < (size-1-i); j++)
		{
			if (a[j]>a[j+1])
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
	int len;
	printf("Enter array length\n");
	scanf("%d", &len);
	printf("Enter array separated by space\n");
	int numbers[len];
	input(numbers,len);
	sort_array(len,numbers);
	aPrint(numbers,len);
    return 0;
}
*/
