#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define DELEMITER ';'
#define NAMEFILE 255
#define ARRAYSIZE 7
#define OVERFLOW 8


enum { NumLine = 0, NumError = 1, PastError = 2};
enum { y = 0, month = 1, day = 2, hour = 3, minute = 4 , data = 5, ErrorInline = 6};


#define MARKANDCONT arr[ErrorInline]++;continue
#define MARKANDRET  arr[ErrorInline]++;return


typedef struct m
{
	int a_month;
	int b_count;
	int c_maxd;
	int d_mind;
	int error;
	int f_quantity;
}listmonth;


typedef struct listYears 
{
	int a_Year;
	int b_count;
	int c_maxd;
	int d_mind;
	int error;
	int f_quantity;
	listmonth g_month[12];
	struct listYears* next;
}years;


typedef struct listError
{
	int one;
	int two;
	struct listError* next;
}queueError;

_Bool empty_queue(queueError *p);

_Bool empty_years(years *p);

int pop(years **p);

void Test_for_correct_ranges (int *arr);

void create_new_year(years **year, int *arr);

void filling (years *year, int *ar);

char readline (FILE* in,int *arr);

int processing_by_year (int* arr, years **yearlist);

void print_result (int choiseYear, int choiseMonth, years *year, int *arrErr);

void сollecting_errors_in_strings (int *arr,int *line_err, queueError **p);

void printErrorsLine (queueError *er);

void printHelp (void);

void ArgToInt (int *res, char* income, int sizeNumber);

void Program_body (char* NameFile, int YearInput, int MonthInput, int enableShowErrorLine);
