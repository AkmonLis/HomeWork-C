#include <stdio.h>



int main(int argc, char **argv)
{
	char input[2];
	scanf("%s", input);
	char letter = input[0]-63;
	int number = input[1]+1;
	(((letter%2 == 0)&&(number%2 == 0)) || ((letter%2 != 0)&&(number%2 != 0))) ? printf("BLACK") : printf("WHITE");
    return 0;
}

