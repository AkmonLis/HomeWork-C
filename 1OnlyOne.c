#include <stdio.h>
#include <string.h>


#define SIZEANSWER 26
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

void sort_array (int size, int a[])
{
	int temp;
	for (int i = 0; i < (size-1); i++)
	{
		for (int j = 0; j < (size-1-i); j++)
		{
			if (a[j]>a[j+1])
			{
				temp = a[j+1];
				a[j+1] = a[j];
				a[j] = temp;
			}
		}
	}
}

void SearchForIdenticalLetters (char *lineIncome, int *lineOut)
{
	int size = strlen(lineIncome);
	int FirstWord[SIZEANSWER]={0}, SecondWord[SIZEANSWER]={0};
	int spase;
	for (int i = 0; i<size ; i++)
	{
		if (lineIncome[i] == ' ')
		{
			spase = i;
			break;
		}
	}
	for (int i = 0; i<spase; i++)
	{
		FirstWord[(int)lineIncome[i]-'a']++;
	}
	for (int i = spase+1; i<size; i++)
	{
		SecondWord[(int)lineIncome[i]-'a']++;
	}
	for (int i = 0, j = 0 ; i<SIZEANSWER; i++)
	{
		if ((FirstWord[i] == 1) && (SecondWord[i] == 1))
		{
		lineOut[j] = i+'a';
		j++;
		}
	}
}

int writingFile(int size, int *line)
{
	FILE *stream = fopen("answer1.txt", "w");
	if (stream != 0)
	{
		for (int i=0; i<size; i++)
		{
			if (line[i] != 0)
			{
				fprintf(stream, "%c ", line[i]);
			}
		}
		fclose(stream);
		return 0;
	}
	return 1;
}


int main() 
{
	char incoming[strLen];
	int output[SIZEANSWER]={0};
	
	if (readingFile(strLen, incoming)!= 0)
	{
		printf("Error occured while opening input file\n");
		return 1;
	}
	
	SearchForIdenticalLetters(incoming,output);
	sort_array(SIZEANSWER,output);
	
	
	if (writingFile(SIZEANSWER, output)!= 0)
	{
		printf("Error occured while opening output file\n");
		return 1;
	}
	
    return 0;
}


