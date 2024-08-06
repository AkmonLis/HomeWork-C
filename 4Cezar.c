#include <stdio.h>

char cezar (int n, char s)
{
	if (s >= 'A' && s <= 'Z')
	{
		s = s + (char)n;
		if (s > 'Z')
		{
			s = 'A' + (s -'Z'-1);
		}
	}
	else if (s >= 'a' && s <= 'z')
	{
		s = s + (char)n;
		if (s > 'z')
		{
			s = 'a' + (s -'z'-1);
		}
	}
	return s;
}

int main()
{
	int N;
	scanf("%d", &N);
	if ( N > ('z' - 'a' +1 ))
	{
		N = N%('z'-'a' +1 );
	}
	char simvol;
	while ((simvol = getchar()) != '.'){
		printf ("%c", cezar(N,simvol));
	}
	printf(".");
	return 0;
}

