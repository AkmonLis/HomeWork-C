#include <stdio.h>
#include <stdint.h>


int inputValume (int n, int32_t m)
{
	int count = 0,temp;
	while (n){
		scanf("%d",&temp);
		if (temp == m)
		{
			count++;
		}
		else if (temp > m)
		{
			m = temp;
			count = 1;
		}
		n--;
	}
	return count;
}

int main()
{
	int N;
	scanf("%d", &N);
	int32_t max = ~0;
	max <<= (32-1);
	int count = inputValume(N,max);
	printf("%d", count);
	return 0;
}
