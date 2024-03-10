#include <stdio.h>

int dec_to_bin(int n)
{
	static int sumOne = 0;
	if (n >= 2)
	{
		dec_to_bin(n / 2);
	}
	if((n%2) == 1)
	{
		sumOne++;
	}
	return sumOne;
}
int main()
{
	int n;
	scanf("%d",&n);
	int sum = dec_to_bin(n);
	printf("sum = %d", sum);
	return 0;
}
