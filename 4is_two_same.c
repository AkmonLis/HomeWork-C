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

int is_two_same(int size, int a[])
{
	for (int i = 0; i<(size-1); i++)
	{
		for (int j = i+1; j<size; j++)
		{
			if (a[i] == a[j])
			{
				return 1;
			}
		}
	}
	return 0;
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
	is_two_same(len, numbers) > 0 ? printf("YES") : printf("NO");
    return 0;
}
*/
