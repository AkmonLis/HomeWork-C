#include <stdio.h>

unsigned long long recurs_power(int n, int p)
{
	static unsigned long long c = 1;
	if (p != 0)
	{
		c = c*n;
		recurs_power(n,--p);
	}
	return c;
}

int main(int argc, char **argv)
{
	int a, b;
	scanf("%d %d", &a, &b);
	printf("%llu", recurs_power(a,b));
	return 0;
}
