#include <stdio.h>

int brackets (char a)
{
	int count = 0;
	if ( a == '(' )
		count = 1;
	else if (a == ')' )
		count = -1;
	return count;
}
	

int main(int argc, char **argv)
{
	char input;
	int check = 0;
	while (1){
		scanf("%c", &input);
		if (input == '.')
			break;
		if (brackets(input))
		check = check + brackets(input);
	}
	check ? printf("No") : printf("Yes");
	return 0;
}
