#include "temp_function.h"



int main(int argc, char *argv[])
{
	char NameFile[NAMEFILE] = "\0";
	int YearInput = 0;
	int MonthInput = 0;
	int enableShowErrorLine = 0;
	int rez = 0;
	opterr=0;
	while ((rez = getopt(argc, argv, "hf:y:m:e")) != -1)
	{
		switch(rez)
		{
		case 'h' : printHelp();return 0;
		case '?' : printf("unknown argument, please read help option -h");return 0;
		case 'f' : strcpy(NameFile, optarg);break;
		case 'e' : enableShowErrorLine = 1;break;
		case 'y' : ArgToInt(&YearInput, optarg, 4);break;
		case 'm' : 
			ArgToInt(&MonthInput, optarg, 2);
			if ((YearInput == 0)||(MonthInput < 1) || (MonthInput > 12))
			{
				printf("The month entered is incorrect\n");
				return 0;
			}
			break;
		}
	}
	if (strlen(NameFile) == 0)
	{
		printf("The program was launched without arguments -f. Read help -h\n");
		return 0;
	} 
	else
	{
		Program_body(NameFile, YearInput, MonthInput, enableShowErrorLine);
	}
	
	return 0;
}
