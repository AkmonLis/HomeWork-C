#include <stdio.h>

void print_odd (void)
{
	int a;
	scanf ("%d", &a);
	if ((a%2)!= 0)
	{
		printf ("%d ", a);
	}
	if (a != 0)
	{
		print_odd ();
	}
	else
	{
		return;
	}
}
int main()
{
 print_odd();
 return 0;
}
