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

int find_max_array (int size, int a[])
{
	int max = a[0];
	for (int i = 1; i < size; i++)
	{
		if (a[i] > max)
		{
			max = a[i];
		}
	}
	return max;
}


/*

int main() 
{
	int len;
	printf("Enter array length\n");
	scanf("%d", &len);
	printf("Enter array separated by space\n");
	int numbers[len];
	input(numbers,len);
	printf("%d", find_max_array(len,numbers));
    return 0;
}
*/


