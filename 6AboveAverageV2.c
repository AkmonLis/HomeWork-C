#include <stdio.h>


float Average_trace_matrix(int m, int n, int a[m][n])
{
	int sum = 0;
	float count = 0;
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			if (i == j)
			{
				sum += a[i][j];
				count++;
			}
		}
	}
	return sum/count;
}


int AboveAverage(int m, int n, int arr[m][n])
{
	int count = 0;
	float Average = Average_trace_matrix(m,n,arr);
	for (int i = 0; i < m; i++)
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
	int matrix[5][5] = 
	{	
		{1,1,1,1,1},
		{2,2,2,2,2},
		{3,3,3,3,3},
		{4,4,4,4,4},
		{5,5,5,5,5}
	};
	printf("%d", AboveAverage(5,5,matrix));
    return 0;
}

