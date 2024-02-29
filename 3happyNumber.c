#include <stdio.h>

int is_happy_number (int a)
{
	int sum = 0, multi = 1;
	while (a>0){
			sum +=a%10;
			multi *=a%10;
			a /=10;
		}
	if (sum == multi){
		return 1;
	}
	return 0;
}
	




int main(int argc, char **argv)
{
	int a;
	scanf("%d", &a);
	is_happy_number (a)? printf("Yes") : printf("No");
	return 0;
}
