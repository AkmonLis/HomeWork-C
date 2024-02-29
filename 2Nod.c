#include <stdio.h>

int nod (int a, int b)
{
	while (a != 0&&b != 0){
    if (a > b)
        a = a % b;
    else
        b = b % a;
	}
	return (a+b);
}




int main(int argc, char **argv)
{
	int a,b;
	scanf("%d %d", &a, &b);
	printf("%d", nod (a,b));
}
