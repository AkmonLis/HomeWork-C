#include <stdio.h>

void print_num(int num)
{
	if (num == 0)
	{
		return;
	}
	print_num(num/10);
	printf("%d ", num%10);
}


int main(int argc, char **argv)
{
	int a;
	scanf("%d", &a);	
	print_num(a);
	return 0;
}

