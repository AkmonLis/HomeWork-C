#include <stdio.h>
#include <stdlib.h>

#define SIZE_IN_OUT 255
#define STACK_SIZE 255
#define SPASE ' '

_Bool Digit(char c)
{
	return ((c >= '0')&&(c <= '9'));
}

_Bool Operator(char c)
{
	return ((c == '&')||(c == '|')||(c == '^'));
}

_Bool Empty(char c[])
{
	return c[0] == '\0';
}

int priority(char c) 
{
	if ( c == '|')
	{
		return 1;
	}
	if (c == '^')
	{
		return 2;
	}
	if (c == '&')
	{
		return 3;
	}
	return 0;
}

void pushOut (char c, int* index, char out[])
{
	out[*index] = c ;
	out[*index+1] = '\0';
	(*index)++;
}

void pushStec (char c, int index, char stec[])
{
	stec[index] = c;
	stec[index+1] = '\0';
}

void delHeadStec (int* index, char stec[])
{
	stec[*index] = '\0';
	if (*index > 0)
	{
		(*index)--;
	}
}


int main(void)
{
	char c;
	char input[SIZE_IN_OUT]= "\0";
	char out[SIZE_IN_OUT] = "\0";
	char stec[STACK_SIZE] = "\0";
	
	int index_in = 0, index_out = 0, index_stec = 0;
	
	while ((c = getchar()) != '\n')
	{
		if (c == ' ')
		{
			continue;
		}
		input[index_in] = c;
		index_in++;
	}
	input[index_in+1] = '\0';
	
	index_in = 0;
	
	while ( input[index_in] != '\0' )
	{
		if ( Digit(input[index_in]) )
		{
				pushOut(input[index_in], &index_out, out);
				if ( !Digit(input[index_in+1]))
				{
					pushOut(SPASE, &index_out, out);
				}
		}
		else if (Operator(input[index_in]))
		{
			while (!Empty(stec))
			{
				if ( priority(stec[index_stec]) >= priority(input[index_in]) )
				{
					pushOut(stec[index_stec], &index_out, out);
					pushOut(SPASE, &index_out, out);
					delHeadStec(&index_stec,stec);
				}
				else
				{
					index_stec++;
					pushStec(input[index_in], index_stec, stec);
					break;
				}
			}
			if (Empty(stec))
			{
				pushStec(input[index_in],index_stec, stec);
			}
		}
		else if ( input[index_in] == '(' )
		{
			if (Empty(stec))
			{
				pushStec(input[index_in],index_stec, stec);
			}
			else
			{
				index_stec++;
				pushStec(input[index_in], index_stec, stec);
			}
		}
		else if ( input[index_in] == ')')
		{
			while ( stec[index_stec] != '(' )
			{
				if (Empty(stec))
				{
					printf("EXPRESSION ENTRY ERROR, BRACKETS DO NOT MATCH");
					exit(1);
				}
				else
				{
					if ( stec[index_stec] == '(' )
					{
						delHeadStec(&index_stec,stec);
					}
					else
					{
						pushOut(stec[index_stec], &index_out, out);
						pushOut(SPASE, &index_out, out);
						delHeadStec(&index_stec,stec);
					}
				}
			}
		}
		index_in++;
	}
	
	while (!Empty(stec))
	{
		if ( stec[index_stec] == '(' )
		{
			delHeadStec(&index_stec,stec);
		}
		else
		{
			pushOut(stec[index_stec], &index_out, out);
			pushOut(SPASE, &index_out, out);
			delHeadStec(&index_stec,stec);
		}
	}
	
	out[index_out]= '\0';
	
	printf("out = %s\n", out);
	
	return 0;
}
