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



void delSpace (char *lineIncome)
{
	int size = strlen(lineIncome);
	int i = 0;
	while (i < size)
	{
		if (((lineIncome[i] == ' ') && (lineIncome[i+1]) == ' ') || ((lineIncome[0] == ' ')))
		{
			for (int j=i; j<size; j++)
			{
				lineIncome[j] = lineIncome[j+1];
			}
			size--;
		}
		else
		{
			i++;
		}
	}
}


int writingFile(char *line)
{
	FILE *stream = fopen("answer2.txt", "w");
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

	delSpace(incoming);

	
	if (writingFile(incoming)!= 0)
	{
		printf("Error occured while opening output file\n");
		return 1;
	}
	
    return 0;
}


