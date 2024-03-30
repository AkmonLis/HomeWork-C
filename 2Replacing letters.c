#include <stdio.h>
#include <string.h>

#define SIZENAME 254

const int strLen = 1000;



int readingFile(int size, char *line)
{
	char NameFile[SIZENAME];
	printf("Enter a file name, for example \"1.txt\"\n");
	scanf("%253s", NameFile);
	FILE *stream = fopen("1.txt", "r");
	if (stream != 0)
	{
		fscanf(stream,"%[^\n]",line);
		fclose(stream);
		return 0;
	}
	return 1;
}

void ReplacingLetters (char *lineIncome)
{
	int size = strlen(lineIncome);
	for (int i=0; i<size; i++)
	{
		if ((lineIncome[i] == 'a')||(lineIncome[i] == 'A'))
		{
			lineIncome[i]++;
		}
		else if ((lineIncome[i] == 'b')||(lineIncome[i] == 'B'))
		{
			lineIncome[i]--;
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
	
	ReplacingLetters(incoming);
	
	if (writingFile(incoming)!= 0)
	{
		printf("Error occured while opening output file\n");
		return 1;
	}
    return 0;
}


