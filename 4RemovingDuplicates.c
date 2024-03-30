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


void RemowingSpase(int sizeStr, char *instr)
{
	for (int i=0; i<sizeStr; i++)
	{
		while (instr[i] == ' ')
		{
			for (int j=i; j<sizeStr; j++)
			{
				instr[j] = instr[j+1];
			}
			sizeStr--;
		}
	
	}
}

void RemowingDuplicates (int size, char *lineIncome)
{
	for (int i=0; i<size-1; i++)
	{
		for (int j=i+1; j<size; j++)
		{
			if (lineIncome[i] == lineIncome[j])
			{
				lineIncome[j] = ' ';
			}
		}
	}
}

int writingFile(char *line)
{
	FILE *stream = fopen("answer4.txt", "w");
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
	
	if (readingFile(incoming)!= 0)
	{
		printf("Error occured while opening input file\n");
		return 1;
	}
	
	int lenStr = strlen(incoming);
	
	RemowingDuplicates(lenStr, incoming);
	RemowingSpase(lenStr, incoming);
	
	if (writingFile(incoming)!= 0)
	{
		printf("Error occured while opening output file\n");
		return 1;
	}
    return 0;
}

