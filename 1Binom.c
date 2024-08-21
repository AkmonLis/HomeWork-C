#include <stdio.h>




int main(int argc, char **argv)
{
	int a = 10, b = 5;
	scanf("%d %d", &a,&b);
	const int str = a+1;
	const int col = b+1;
	int arr[str][col];
	
	
	for (int i = 0; i<str;i++)
	{
		for (int j = 0; j<col;j++)
		{
			if( j == 0)
			{
				arr[i][j] = 1;
			}
			else
			{
				arr[i][j]=0;
			}
		}
	}
	
	for (int i = 1; i<str;i++)
	{
		for (int j = 1; j<col;j++)
		{
			if ((arr[i][j] == 0)&&(arr[i-1][j-1] == 0))
			{
				break;
			}
			else
			{
				arr[i][j]=arr[i-1][j-1]+arr[i-1][j];
			}
		}
	}
	
	printf("%d ", arr[a][b]);
	
	return 0;
}

