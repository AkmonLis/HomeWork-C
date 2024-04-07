#include <stdio.h>
#include <string.h>

#define SIZENAME 254

const int strLen = 1000;



int readingFile(int size, char *line)
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


void ChangeType (char *lineIncome)
{
	int size = strlen(lineIncome);
	int i = size;
	while (i > 0)
	{
		if (lineIncome[i] == '.')
		{
			lineIncome[i] = '\0';
			break;
		}
		if (lineIncome[i] == '/')
		{
			break;
		}
		i--;
	}
}


int writingFile(char *line, char *insert)
{
	FILE *stream = fopen("answer3.txt", "w");
	if (stream != 0)
	{
		fprintf(stream, "%s%s", line, insert);
		fclose(stream);
		return 0;
	}
	return 1;
}


int main() 
{
	char incoming[strLen];
	char insert[] = ".html";
	
	if (readingFile(strLen, incoming)!= 0)
	{
		printf("Error occured while opening input file\n");
		return 1;
	}
	
	ChangeType(incoming);
	
	if (writingFile(incoming,insert)!= 0)
	{
		printf("Error occured while opening output file\n");
		return 1;
	}
	
    return 0;
}


