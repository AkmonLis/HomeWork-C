#include <stdio.h>


void input (int n, int *arr)
{
	for (int i = 0; i < n*n; i++)
	{
		scanf("%d", &arr[i]);
	}
	printf("\n");
}


void aPrint(int n, int *arr)
{
	for (int i = 0, j=1; i < n*n; i++)
	{
		if (i == (n*j))
		{
			printf("\n");
			j++;
		}
		printf("%d ",arr[i]);
	}
	printf("\n");
}


float MainDiagonal(int size, int a[size][size])
{
	float sum = 0;
	int i;
	for ( i=0 ; i < size; i++)
	{
		sum = sum + a[i][i];
	}
	return sum/i;
}

int AboveAverage (int n, int arr[n][n])
{
	int count = 0;
	float Average = MainDiagonal(n, arr);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (arr[i][j] > Average)
			{
				count++;
			}
		}
	}
	return count;
}

int main(int argc, char **argv)
{
	int len;
	printf("enter the side of the square array\n");
	scanf("%d", &len);
	printf("enter all the digits of the matrix sequentially separated by a space\n");
	int numbers[len][len];
	input(len,&numbers[0][0]);
	aPrint(len,&numbers[0][0]);
	printf("\n AboveAverage %d", AboveAverage(len,numbers));
    return 0;
}

