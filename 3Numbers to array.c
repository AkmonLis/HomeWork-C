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

void NumbersToArray (char *lineIncome, int *lineOut, int *sizeOutArr)
{
	int size = strlen(lineIncome);
	for (int i = 0, j = 0; i < size; i++)
	{
		if ((lineIncome[i] >= '0')&&(lineIncome[i] <= '9'))
		{
			if ((i != 0)&&(lineIncome[i-1] >= '0')&&(lineIncome[i-1] <= '9'))
			{
				lineOut[j] = lineOut[j]*10+(lineIncome[i]-'0');
			}
			else
			{
				lineOut[j] = lineIncome[i]-'0';
			}
			if ((lineIncome[i+1] < '0') || (lineIncome[i+1] > '9'))
			{
				j++;
				(*sizeOutArr)++;
			}
		}
	}
}

int writingFile(int size, int *line)
{
	FILE *stream = fopen("answer3.txt", "w");
	if (stream != 0)
	{
		for (int i=0; i<size; i++)
		{
			fprintf(stream, "%d ", line[i]);
		}
		fclose(stream);
		return 0;
	}
	return 1;
}


int main() 
{
	char incoming[strLen];
	int output[strLen];
	int OutputSizeArray = 0;
	
	if (readingFile(strLen, incoming)!= 0)
	{
		printf("Error occured while opening input file\n");
		return 1;
	}
	
	NumbersToArray(incoming,output,&OutputSizeArray);
	
	sort_array(OutputSizeArray,output);
	
	
	if (writingFile(OutputSizeArray, output)!= 0)
	{
		printf("Error occured while opening output file\n");
		return 1;
	}
	
    return 0;
}

