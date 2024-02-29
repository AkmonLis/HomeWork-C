#include <stdio.h>

int cell (int a)
{
	int res = 1;
	int count = 1;
	while (count < a ){
		
	 res = res * 2;
	 count++;
    }
	return res;
}




int main(int argc, char **argv)
{
	int a;
	scanf("%d", &a);
	printf("%d", cell(a));
}
