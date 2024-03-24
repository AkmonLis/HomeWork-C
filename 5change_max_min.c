#include <stdio.h>
/*
void input (int arr[], int n)
{
	for (int i = 0; i < n; i++)
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
*/

void change_max_min(int size, int a[])
{
	int max = a[0], min = a[0];
	int maxIndex = 0, minIndex = 0;
	int temp;
	for (int i = 0; i<size; i++)
	{
		if (a[i] > max)
		{
			max = a[i];
			maxIndex = i;
		}
		if (a[i] < min)
		{
			min =a[i];
			minIndex = i;
		}
	}
	temp = a[maxIndex];
	a[maxIndex] = a[minIndex];
	a[minIndex] = temp;
}

/*
int main(int argc, char **argv)
{
	int len;
	printf("Enter array length\n");
	scanf("%d", &len);
	printf("Enter array separated by space\n");
	int numbers[len];
	input(numbers,len);
	change_max_min(len, numbers);
	aPrint(numbers, len);
    return 0;
}
*/
