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


void LongestWord(int sizeStr, char *instr, char *outstr)
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
			if (strlen(buffer) > strlen(outstr))
				{
					strcpy(outstr,buffer);
				}
		}
	}
}


int writingFile(char *line)
{
	FILE *stream = fopen("answer5.txt", "w");
	if (stream != 0)
	{
		fprintf(stream, "%s", line);
		fclose(stream);
		return 0;
	}
	return 1;
}


int main() 
{
	char incoming[strLen];
	char outWord[strLen];
	outWord[0] = '\0';
	
	if (readingFile(incoming)!= 0)
	{
		printf("Error occured while opening input file\n");
		return 1;
	}
	
	int lenStr = strlen(incoming);
	
	LongestWord(lenStr, incoming, outWord);
	
	if (writingFile(outWord)!= 0)
	{
		printf("Error occured while opening output file\n");
		return 1;
	}
    return 0;
}

