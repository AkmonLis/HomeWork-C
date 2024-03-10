#include <stdio.h>

void print_num(int a, int b)
{
	printf("%d ", a);
	if (a == b)
	{
		return;
	}
	(a>b) ? print_num(--a,b): print_num(++a,b);
}


int main(int argc, char **argv)
{
	int a,b;
	scanf("%d %d", &a,&b);
	print_num(a,b);
	return 0;
}

