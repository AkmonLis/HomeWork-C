#include "HeaderFail.h"


int checkMinus (int a)																		
{
	return (a<0 ? (-1*a) : a);
}

int DestinatoinX (int a, int b)																
{																							
	if (a>b)																				
	{																						 
		return ((a-b)>(MAX_X-2-a+b)) ? (MAX_X-2-a+b) : (b-a);								
	}
	else
	{
		return ((b-a)>(MAX_X-2-b+a)) ? ((MAX_X-2-b+a)*(-1)) : (b-a);						
	}
}

int DestinatoinY (int a, int b)																
{																							
	if (a>b)																				
	{																						 
		return ((a-b)>(MAX_Y-2-a+b)) ? (MAX_Y-2-a+b) : (b-a);								 
	}
	else
	{
		return ((b-a)>(MAX_Y-2-b+a)) ? ((MAX_Y-2-b+a)*(-1)) : (b-a);						
	}
}
