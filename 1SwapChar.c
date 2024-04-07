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


void swap (char *lineIncome)
{
	int size = strlen(lineIncome);
	int i = 0;
	int j = 0;
	char buffer;
	while (i < size)
	{
		while (lineIncome[i] == ' ')
		{
			i++;
		}
		j = i+1;
		while (lineIncome[j] == ' ')
		{
			j++;
		}
		if (j >=size)
		{
			break;
		}
		buffer = lineIncome[i];
		lineIncome[i] = lineIncome[j];
		lineIncome[j] = buffer;
		i = j+1;
	}
}


int writingFile(char *line)
{
	FILE *stream = fopen("answer1.txt", "w");
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
	
	if (readingFile(strLen, incoming)!= 0)
	{
		printf("Error occured while opening input file\n");
		return 1;
	}
	
	swap(incoming);
	
	if (writingFile(incoming)!= 0)
	{
		printf("Error occured while opening output file\n");
		return 1;
	}
    return 0;
}


