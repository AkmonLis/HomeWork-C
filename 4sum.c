#include <stdio.h>

int sumN (int a)
{
	int sum = 0;
	int i = 1;
	while (i<=a){
		sum = sum + i;
		i++;
	}
	return sum;
}
	




int main(int argc, char **argv)
{
	int a;
	scanf("%d", &a);
	printf("%d", sumN(a));
	return 0;
}
