#include <stdio.h>
#include <string.h>
#include <stdlib.h>



#define STR_SIZE 2000


typedef struct list
{
 char* word;
 struct list *next;
}list;


list* insert2(char* value,list *head)
{
	list *temp = head;
	list *res = (list*)calloc(1,sizeof(list));
	int len = strlen(value);
	res->word = malloc(len+1);
	strcpy(res->word, value);
	if (head == NULL)
	{
		return res;
	}
	while (temp->next)
	{
		temp = temp->next;
	}
	temp->next = res;
	return head;
}



list* insert (list* ptr_list,char* line)
{
	int i =0,j;
	char word[STR_SIZE] = "\0";
	while (line[i] != '\0')
	{
		if (line[i] == ' ')
		{
			i++;
			continue;
		}
		
		j = 0;
		while ( (line[i] != ' ') && (line[i] != '\0') )
		{
			word[j] = line[i];
			i++;
			j++;
		}
		
		word[j] = '\0';
		
		ptr_list = insert2(word,ptr_list);

		i++;
	}
	return ptr_list;
}

list* del(list* ptr_list)
{
	list *ptmp = ptr_list;
	if (ptr_list == NULL)
	{
		exit(1);
	}
	ptr_list = ptmp->next;
	free(ptmp);
	return ptr_list;
}


void print (list* ptr_list)
{

	while (ptr_list)
	{
		printf("%s ", ptr_list->word);
		if(ptr_list->next !=0)
		{
			ptr_list = ptr_list->next;
			ptr_list = del(ptr_list);
		}
		else
		{
			break;
		}
	}
}


int main(int argc, char **argv)
{
	char inputLine[STR_SIZE] = "\0";
	scanf("%[^\n]", inputLine);
	list* Head_ptr_list = NULL;
	Head_ptr_list = insert(Head_ptr_list,inputLine);
	print(Head_ptr_list);
	return 0;
}

