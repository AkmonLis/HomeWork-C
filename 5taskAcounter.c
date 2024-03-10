#include <stdio.h>

int acounter(void)
{
	static int count = 0;
	char ch;
	scanf ("%c", &ch);
	if (ch == 'a')
	{
	count++;
	}
	if (ch != '.')
	{
		acounter();
	}
	return count;
}

int main(int argc, char **argv)
{
	printf("%d", acounter());
	return 0;
}
