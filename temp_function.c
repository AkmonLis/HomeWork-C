#include "temp_function.h"


_Bool empty_years(years *p)
{
	return p==NULL;
}


int pop(years **p)
{
	years *ptmp= *p;
	int c;
	if(empty_years(*p))
		exit (1);
	c=ptmp->f_quantity;
	*p=ptmp->next;
	free(ptmp);
	return c;
}


_Bool empty_queue(queueError *p)
{
	return p==NULL;
}


void Test_for_correct_ranges (int *arr)
{
	if (arr[ErrorInline] != 0)
	{
		return;
	}
	if ( arr[month] == 1 || arr[month] == 3 || arr[month] == 5 || arr[month] == 7 || arr[month] == 8 || arr[month] == 10 || arr[month] == 12)
	{
		if (arr[day] < 0 || arr[day] > 31)
		{
			MARKANDRET;
		}
	}
	else if ( arr[month] == 4 || arr[month] == 6 || arr[month] == 9 || arr[month] == 11)
	{
		if (arr[day] < 0 || arr[day] > 30)
		{
			MARKANDRET;
		}
	}
	else if (arr[month] == 2)
	{
		if ((arr[y]%4 == 0 && arr[y]%100 != 0) || (arr[y]%400 == 0))
		{
			if (arr[day] < 0 || arr[day] > 29)
			{
				MARKANDRET;
			}
		}
		else 
		{
			if (arr[day] < 0 || arr[day] > 28)
			{
				MARKANDRET;
			}
		}
	}
	else
	{
		MARKANDRET;
	}
	if ( arr[hour] < 0 || arr[hour] > 24)
	{
		MARKANDRET;
	}
	if ( arr[minute] < 0 || arr[minute] > 60)
	{
		MARKANDRET;
	}
	if ( arr[data] < -99 || arr[data] > 99)
	{
		MARKANDRET;
	}
}


void create_new_year(years **year, int *arr)
{
	years *creatYear;
	creatYear=malloc(sizeof(years));
	creatYear->a_Year = arr[y];
	for (int i = 0; i < 12; i++)
	{
		creatYear->g_month[i].a_month = i+1;
		creatYear->g_month[i].b_count = 0;
		creatYear->g_month[i].c_maxd = -100;
		creatYear->g_month[i].d_mind = 100;
		creatYear->g_month[i].error = 0;
		creatYear->g_month[i].f_quantity = 0;
	}
	if (arr[ErrorInline] != 0)
	{
		creatYear->error = 1;
		creatYear->b_count = 0;
		creatYear->c_maxd =  -100;
		creatYear->d_mind =  100;
		creatYear->f_quantity = 0;
		if (arr[month] > 0 && arr[month] < 13)
		{
			creatYear->g_month[arr[month]-1].error++;
		}
	}
	else
	{
	creatYear->b_count = 1;
	creatYear->c_maxd =  arr[data];
	creatYear->d_mind =  arr[data];
	creatYear->error = 0;
	creatYear->f_quantity = arr[data];
	
	creatYear->g_month[arr[month]-1].b_count++;
	creatYear->g_month[arr[month]-1].c_maxd = arr[data];
	creatYear->g_month[arr[month]-1].d_mind = arr[data];
	creatYear->g_month[arr[month]-1].f_quantity += arr[data];
	}
	creatYear->next = *year;
	*year=creatYear;
}


void filling (years *year, int *ar)
{
	if (ar[ErrorInline] != 0)
	{
		year->error++;
		if (ar[month] > 0 && ar[month] < 13)
		{
			year->g_month[ar[month]-1].error++;
		}
	}
	else
	{
		year->b_count++;
		if (year->c_maxd < ar[data])
		{
			year->c_maxd =  ar[data];
		}
		if (year->d_mind > ar[data])
		{
			year->d_mind =  ar[data];
		}
		year->f_quantity += ar[data];
		
		
		year->g_month[ar[month]-1].b_count++;
		if ((year->g_month[ar[month]-1].c_maxd) < ar[data])
		 {
			 year->g_month[ar[month]-1].c_maxd = ar[data];
		 }
		 if ((year->g_month[ar[month]-1].d_mind) > ar[data])
		 {
			 year->g_month[ar[month]-1].d_mind = ar[data];
		 }
		year->g_month[ar[month]-1].f_quantity += ar[data];
	}
}


char readline (FILE* in,int *arr)
{
	for (int k = 0; k<ARRAYSIZE; k++)
	{
		arr[k] = 0;
	}
	int counterOverflow = 0;
	int counter = 0;
	int singe = 1;
	char ch;
	while(((ch=fgetc(in))!= '\n')&&(ch != EOF))
	{
		if(ch==DELEMITER)
		{
			counter++;
			counterOverflow = 0;
			singe = 1;
		}
		else if (counter >= (ARRAYSIZE-1))
		{
			MARKANDCONT;
		}
		else if((ch>='0' && ch<='9') || (ch == '-'))
		{
			if (ch == '-')
			{
				singe = -1;
				continue;
			}
			if (counterOverflow > OVERFLOW)
			{
				continue;
			}
			arr[counter] = arr[counter]*10 + (ch - '0')*singe;
			counterOverflow++;
		}
		else
		{
			MARKANDCONT;
		}
	}
	return ch=fgetc(in);
}


int processing_by_year (int* arr, years **yearlist)
{
	if (arr[y]<1000 || arr[y] >9999)
	{
		return 1;
	}
	if (*yearlist == NULL)
	{
		create_new_year(yearlist,arr);
		return arr[ErrorInline]!=0 ? 1 : 0;
	}
	if ((*yearlist)->a_Year != arr[y])
	{
		years *ptr = *yearlist;
		while (ptr->next)
		{
			ptr = ptr->next;
			if ((ptr->a_Year) == arr[y])
			{
				filling(ptr,arr);
				return arr[ErrorInline]!=0 ? 1 : 0;
			}
		}
		create_new_year(yearlist,arr);
	}
	else
	{
		filling(*yearlist, arr);
	}
	return arr[ErrorInline]!=0 ? 1 : 0;
}


void print_result (int choiseYear, int choiseMonth, years *year, int *arrErr)
{
	int startMonth = 0;
	int lastMonth = 11;
	if (choiseMonth != 0)
	{
		startMonth = lastMonth = (choiseMonth-1);
	}
	while ((choiseYear != 0)&&(year->next != NULL)&&(year->a_Year != choiseYear))
	{
		year = year->next;
	}
	
	if (((choiseYear != 0)&&(year->a_Year != choiseYear))||(year == NULL))
	{
		printf("We are very sorry that such YEAR was not found in the incoming file..");
		return;
	}
	
	while (year)
	{
		printf("=======================================================================\n");
		printf(" #  year  month     nuValume     nuError    AvYear  minMonth  maxMonth \n");
		printf("-----------------------------------------------------------------------\n");
		for (int i = startMonth ; i < (lastMonth+1); i++)
		{
			printf("%2.1d  %4.1d  %3.1d   %11.1d  %10.1d     %5.1f      %3.1d      %3.1d\n", i,
			year->a_Year, year->g_month[i].a_month, year->g_month[i].b_count, year->g_month[i].error, 
			(year->g_month[i].b_count == 0 ? 0 : ((year->g_month[i].f_quantity*1.0)/year->g_month[i].b_count)),
			 (year->g_month[i].d_mind < 100 ? year->g_month[i].d_mind : 0), (year->g_month[i].c_maxd > -100 ? year->g_month[i].c_maxd : 0));
		}
		if (choiseMonth != 0)
		{
			return;
		}
		printf("=======================================================================\n");
		printf("Total valume = %d, correct DATA = %d, error line = %d,\n averangeYaer = %.1f, minYear = %d, maxYear = %d\n", 
		(year->b_count+year->error), year->b_count, year->error, (year->b_count ==0 ? 0 :((year->f_quantity*1.0)/year->b_count)), year->d_mind, year->c_maxd);
		printf("\n\n");
		if (choiseYear != 0)
		{
			return;
		}
		pop(&year);
	}
	printf("\n\n=======================================================================\n");
	printf("In FILE: total valume = %d, Error line = %d\n", (--arrErr[NumLine]), arrErr[NumError]);
	printf("-----------------------------------------------------------------------\n");
}


void сollecting_errors_in_strings (int *arr,int *line_err, queueError **p)
{
	if (arr[ErrorInline] == 0)
	{
		return;
	}
	if (line_err[PastError] == 0)
	{
		struct listError *ptmp = *p, *elem;
		elem = malloc(sizeof(struct listError));
		elem->one = line_err[NumLine];
		elem->two = line_err[NumLine];
		elem->next = NULL;
		if(*p==NULL)
		{
			*p = elem;
			return;
		}
		while(ptmp->next)
		{
			ptmp=ptmp->next;
		}
		ptmp->next = elem;
		
	}
	else
	{
		(*p)->two = line_err[NumLine];
	}
}


void printErrorsLine (queueError *er)
{
	printf("\n\n=======================================================================\n");
	if (er == NULL)
	{
		printf("No errors");
		return;
	}
	printf("Errors detected\n\tin the following lines\n\t\tof the source file -> ");
	er->one == er->two ? printf(" %d", er->one) : printf(" %d-%d", er->one, er->two);
	while (er->next)
	{
		queueError *ptmp = er;
		if(empty_queue(er)) 
		{
			exit(1);
		}
		er=ptmp->next;
		free(ptmp);
		er->one == er->two ? printf("; %d", er->one) : printf("; %d-%d", er->one, er->two);
		
	} 
}



void printHelp (void)
{
	printf("Temp statistics application. Please enter key:\n");
	printf("-h for help.\n");
	printf("-f file_name for load this file.\n");
	printf("-y xxxx statistic for xxxx year.\n");
	printf("-m xx statistic for xxxx year(doesn't work without -y).\n");
	printf("-e listing lines with errors.\n");

}


void ArgToInt (int *res, char* income, int sizeNumber)
{
	int i = 0;
	while ((i < sizeNumber)&&(income[i] != '\0'))
	{
		(*res) = (*res)*10 + income[i] - '0';
		i++;
	}
}

void Program_body (char* NameFile, int YearInput, int MonthInput, int enableShowErrorLine)
{
	FILE *income = fopen(NameFile, "r");
	if (income == 0)
	{
		printf("The file name was entered incorrectly..");
		return;
	}
	
	int array_Year_moth_day_hour_minyte_errorInline[ARRAYSIZE];
	
	years *year=NULL;
	queueError *err = NULL;
	
	char forEnd;
	int arr_Numline_NumError_PastError[] = {1,0,0};
	do
	{
		forEnd = readline(income,array_Year_moth_day_hour_minyte_errorInline);
		
		fseek(income,-1,SEEK_CUR);
		
		Test_for_correct_ranges(array_Year_moth_day_hour_minyte_errorInline);
		
		сollecting_errors_in_strings(array_Year_moth_day_hour_minyte_errorInline, arr_Numline_NumError_PastError, &err);
		
		arr_Numline_NumError_PastError[NumError] += processing_by_year(array_Year_moth_day_hour_minyte_errorInline,&year);
		
		array_Year_moth_day_hour_minyte_errorInline[ErrorInline] != 0 ? (arr_Numline_NumError_PastError[PastError] = 1) : (arr_Numline_NumError_PastError[PastError] = 0);
		
		arr_Numline_NumError_PastError[NumLine]++;
	}while (forEnd != EOF);
	
	fclose(income);
	
	
	print_result(YearInput, MonthInput, year, arr_Numline_NumError_PastError); 
	
	
	if (enableShowErrorLine == 1)
	{
		printErrorsLine(err);
	}
}
