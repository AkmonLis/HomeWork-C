#include <stdio.h>
#include <string.h>


#define SIZENAME 254

const int strLen = 1000;



int readingFile(char *line)
{
	char NameFile[SIZENAME];
	printf("Enter a file name, for example \"1.txt\"\n");
	scanf("%253s", NameFile);
	FILE *stream = fopen(NameFile, "r");
	if (stream != 0)
	{
		fscanf(stream,"%[^\n]",line);
		fclose(stream);
		return 0;
	}
	return 1;
}


void lastLett(int sizeStr, char *instr, char lastLett, int *countLetter)
{
	char buffer[sizeStr];
	for (int i=0, j=0; i<sizeStr; i++)
	{
		if (instr[i] != ' ')
		{
			buffer[j] = instr[i];
			buffer[j+1] = '\0';
			j++;
		}
		else 
		{
			j = 0;
		}
		if ((instr[i] == ' ') || (i == (sizeStr-1)))
		{
			if (buffer[strlen(buffer)-1] == lastLett)
			{
				(*countLetter)++;
			}
		}
	}
}


int writingFile(int a)
{
	FILE *stream = fopen("answer6.txt", "w");
	if (stream != 0)
	{
		fprintf(stream, "%d", a);
		fclose(stream);
		return 0;
	}
	return 1;
}


int main() 
{
	char incoming[strLen];
	int Count = 0;
	char Lastletters = 'a';
	
	if (readingFile(incoming)!= 0)
	{
		printf("Error occured while opening input file\n");
		return 1;
	}
	
	int lenStr = strlen(incoming);
	
	lastLett(lenStr, incoming, Lastletters, &Count);
	
	if (writingFile(Count)!= 0)
	{
		printf("Error occured while opening output file\n");
		return 1;
	}
    return 0;
}

