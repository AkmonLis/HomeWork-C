#include "HeaderFail.h"

struct Control_Battons def_control[CONTROLS] = {{KEY_LEFT, KEY_RIGHT, KEY_UP,KEY_DOWN},		
												{'A','D','W','S'},
												{'a','d','w','s'}};

int init_Pumpkin (food_t* pumpkin)															
{
	int harvest = 0;																		
	int count_PUMPKIN = 0;																	
	for (int j = INDOOR_UP; j<(MAX_Y-INDOOR_DOWN); j += INDOOR_BW_Y)						 
	{
		for (int i = INDOOR_LEFT ; i<(MAX_X-INDOOR_RIGHT);i+=INDOOR_BW_X)
		{
			if (count_PUMPKIN >= FOODQUANTITY)												
				{																			
					return harvest;
				}
			pumpkin[count_PUMPKIN].x = i;
			pumpkin[count_PUMPKIN].y = j;
			pumpkin[count_PUMPKIN].status = PUMPKIN_STATUS_RIPENING;
			pumpkin[count_PUMPKIN].employment = OFF;
			harvest++;
			count_PUMPKIN++;
		}
	}
	for (int i = count_PUMPKIN; i<FOODQUANTITY; i++)										
	{																						 
		pumpkin[i].x = -100;																
		pumpkin[i].y = -100;																
		pumpkin[i].status = PUMPKIN_STATUS_NOTUSE;
	}
	
	return harvest;
}


dron_t init_dron(warehouse_t house, int direct, int tupe_control)							
{
	dron_t dron;
	dron.x = house.x;
	dron.y = house.y;
	dron.tsize = START_LENGTH_DRONE;														
	dron.Direction = direct;
	dron.cart = (cart_t *) malloc (sizeof(cart_t) * LENGTHCART);
	dron.control = def_control;
	dron.type_control = tupe_control;														
	dron.point = 0;
	return dron;
}


void init_Autodron(Autodron_t* autodron, warehouse_t house)									
{

	for (int i = 0; i < HELPDRON; i++)
	{
		autodron[i].Direction = RIGHT;														
		autodron[i].cart = (cart_t *) malloc (sizeof(cart_t) * LENGTHCART);
		autodron[i].tsize = 0;
		autodron[i].status = OFF;															
		autodron[i].x = house.x;															
		autodron[i].y = house.y;
	}
}


void printField(food_t* pumpkin, warehouse_t house, dron_t* dron, Autodron_t* autodron,
						char matrix[MAX_X][MAX_Y], int harvest, int harvested, int health)	
{
																							
	for (int i = 0; i < MAX_X; ++i)
	{
		for (int j = 0; j < MAX_Y; ++j)
		{
			if ( i == (MAX_X-1) || j ==(MAX_Y-1) || i == 0 || j == 0)						
			{
				matrix[i][j] = BORDER;
			}
			else
			{
				matrix[i][j] = SPASE;
			}
		}
	}
	
	
	matrix[house.x][house.y] = BORDER;														
	
	
	for (int i = 0; i<FOODQUANTITY; i++)													
	{
		if (pumpkin[i].status == PUMPKIN_STATUS_RIPENING)
		{
			matrix[pumpkin[i].x][pumpkin[i].y] = RIPENING;
		}
		else if (pumpkin[i].status == PUMPKIN_STATUS_RIPEN)
		{
			matrix[pumpkin[i].x][pumpkin[i].y] = RIPEN;
		}
		else if (pumpkin[i].status == PUMPKIN_STATUS_COLLECTED)
		{
			matrix[pumpkin[i].x][pumpkin[i].y] = COLLECTED;
		}
	}
	
	
	matrix[dron->x][dron->y] = DRON;														
	for (int i = 0; i < dron->tsize; ++i)
	{
		matrix[dron->cart[i].x][dron->cart[i].y] = CART;
	}
	
	for (int j = 0; j < HELPDRON; j++)														
	{
		if (autodron[j].status == ON)														
		{
			matrix[autodron[j].x][autodron[j].y] = DRON;
			for (int i = 0; i < autodron[j].tsize; ++i)
			{
				matrix[autodron[j].cart[i].x][autodron[j].cart[i].y] = CART;				
			}
		}
	}
	
	
	for (int j = 0; j < MAX_Y; ++j)															
	{
		for (int i = 0; i < MAX_X; ++i)
		{
			if (matrix[i][j] == BORDER)														
			{
				if ((house.x == i)&&(house.y == j))
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 51);
				}
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 136);
				}
			}
			else if (matrix[i][j] == DRON)
			{
				for (int k = 0; k < HELPDRON;k++)
				{
					if (autodron[k].status == OFF)
					{
						break;
					}
					if ((k == 0)&&(autodron[k].x == i)&&(autodron[k].y == j))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),2);
						break;
					}
					else if ((k == 1)&&(autodron[k].x == i)&&(autodron[k].y == j))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),3);
						break;
					}
					else if ((k == 2)&&(autodron[k].x == i)&&(autodron[k].y == j))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),4);
						break;
					}
					else if ((k == 3)&&(autodron[k].x == i)&&(autodron[k].y == j))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),5);
					}
				}
				if((dron->x == i)&&(dron->y == j))
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),1);
				}
			}
			else if (matrix[i][j] == RIPENING)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
			}
			else if (matrix[i][j] == RIPEN)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
			}
			else if (matrix[i][j] == CART)
			{
				for (int c =0; c<dron->tsize; c++)
				{
					if ((dron->cart[c].x == i)&&(dron->cart[c].y == j))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),1);
						break;
					}
				}
				for (int k = 0; k < HELPDRON;k++)
				{
					if (autodron[k].status == OFF)
					{
						break;
					}
					for (int c = 0; c < autodron[k].tsize; c++)
					{
						if ((k == 0)&&(autodron[k].cart[c].x == i)&&(autodron[k].cart[c].y == j))
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),2);
							break;
						}
						else if ((k == 1)&&(autodron[k].cart[c].x == i)&&(autodron[k].cart[c].y == j))
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),3);
							break;
						}
						else if ((k == 2)&&(autodron[k].cart[c].x == i)&&(autodron[k].cart[c].y == j))
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),4);
							break;
						}
						else if ((k == 3)&&(autodron[k].cart[c].x == i)&&(autodron[k].cart[c].y == j))
						{
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),5);
						}
					}
				}
			}
			printf("%c", matrix[i][j]);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		}
			printf("\n");
	}
	printf("HARVEST = %d\t", harvest);
	printf("HARVESTED = %d\t", harvested);
	printf("\nAccidents = %d\t", health);
	printf("USER HARVESTED = %d  ", dron->point);
}


void checkEvent( dron_t* dron,Autodron_t* autodron, 
				food_t* pumpkin, warehouse_t house, int* score, int* h)						
{																							
	for (int i = 0; i < FOODQUANTITY; i++)
	{
		if (pumpkin[i].status == PUMPKIN_STATUS_COLLECTED)									
		{
			continue;
		}
		if ((dron->x == pumpkin[i].x)&&(dron->y == pumpkin[i].y))
		{
			if ((pumpkin[i].status == PUMPKIN_STATUS_RIPENING)&&(dron->tsize != 0))			
			{
				pumpkin[i].status = PUMPKIN_STATUS_COLLECTED;								
			}
			else if (pumpkin[i].status == PUMPKIN_STATUS_RIPEN)								
			{
				if (dron->tsize < LENGTHCART)												
				{
					dron->tsize++;
				}
				pumpkin[i].status = PUMPKIN_STATUS_COLLECTED;
			}
			continue;																		
		}
		for (int j = 0; j<HELPDRON; j++)													
		{
			if (autodron[j].status == OFF)													
			{
				break;
			}
			if ((autodron[j].x == pumpkin[i].x)&&(autodron[j].y == pumpkin[i].y))			
			{
				if ((pumpkin[i].status == PUMPKIN_STATUS_RIPENING)&&(autodron[j].tsize != 0))
				{
					pumpkin[i].status = PUMPKIN_STATUS_COLLECTED;
				}
				else if (pumpkin[i].status == PUMPKIN_STATUS_RIPEN)
				{
					if (autodron[j].tsize <LENGTHCART)										
					{
						autodron[j].tsize++;
					}
					pumpkin[i].status = PUMPKIN_STATUS_COLLECTED;
				}
				break;																		
			}
		}
	}
	
	if ((house.x == dron->x)&&(house.y == dron->y))											
	{
		(*score) += dron->tsize;
		dron->point += dron->tsize;
		dron->tsize = 0;
	}
	for (int i= 0; i<HELPDRON; i++)
	{
		if ((house.x == autodron[i].x)&&(house.y == autodron[i].y))
		{
			(*score) += autodron[i].tsize;
			autodron[i].tsize = 0;
		}
	}
																							
																						
																						
																						
	
	for (int i = 0; i < dron->tsize; i++)												
	{
		if ((dron->x == dron->cart[i].x)&&(dron->y == dron->cart[i].y))
		{
			dron->tsize = 0;															
			CRASH;
		}
	}
	
	if (dron->tsize != 0)																	
	{
		for (int i = 0; i < HELPDRON; i++)													
		{
			if (autodron[i].status == OFF)													
			{
				break;
			}
			for (int j = 0; j < autodron[i].tsize; j++)
			{
				if ((dron->x == autodron[i].cart[j].x)&&(dron->y == autodron[i].cart[j].y))
				{
					dron->tsize = 0;						
					autodron[i].tsize = 0;				
					CRASH;
				}
			}
		}
	}
	
	for (int i = 0; i < HELPDRON; i++)														
	{
		if (autodron[i].status == OFF)													
		{
			break;
		}
		if (autodron[i].tsize == 0)
		{
			continue;																		
		}
		for(int j = 0 ; j < dron->tsize; j++)											
		{
			if ((autodron[i].x == dron->cart[j].x)&&
				(autodron[i].y == dron->cart[j].y))
			{
				dron->tsize = 0;						
				autodron[i].tsize = 0;
				CRASH;
			}
		}
		
		for (int k = 0; k < HELPDRON; k++)												
		{
			if (autodron[k].status == OFF)
			{
				break;
			}
			if (autodron[k].tsize == 0)
			{
				continue;																	
			}
			for(int j = 0 ; j < autodron[k].tsize; j++)										
			{
				if ((autodron[i].x == autodron[k].cart[j].x)&&
					(autodron[i].y == autodron[k].cart[j].y))
				{
					autodron[i].tsize = 0;
					autodron[k].tsize = 0;
					CRASH;
				}
			}
		}
	}
}


void updateFood (food_t* pumpkin)															
{

	for (int i = 0; i< FOODQUANTITY; i++)													
	{
		pumpkin[i].employment = OFF;														
		if (pumpkin[i].status == PUMPKIN_STATUS_RIPENING)
		{
			if ((rand()%10000) > 9960)
			{
				pumpkin[i].status = PUMPKIN_STATUS_RIPEN;
				break;
			}
		}
	}

}


int CheckEnd(int harvest,int harvested,food_t* pumpkin,dron_t* dron,Autodron_t* AutoDron)
{
	if (harvest == harvested)																
	{
		return 0;
	}
	int count_pupkin_collected = 0;														
	for (int i = 0; i< FOODQUANTITY; i++)
	{
		if (pumpkin[i].status == PUMPKIN_STATUS_COLLECTED)
		{
			count_pupkin_collected++;
		}
	}
	if ((count_pupkin_collected == harvest)&&(dron->tsize == 0))						
	{
		for (int i = 0; i < HELPDRON; i++)													
		{
			if (AutoDron[i].status == OFF)												 
			{
				break;
			}
			if (AutoDron[i].tsize != 0)														
			{
				return 1;
			}
		}
		return 0;																			
		
	}
	
	return 1;
}


void endMenu(int harvest,int harvested,int health, dron_t dron)
{
	system("cls");
	for (int i = 0; i< (MAX_Y/2); i++)
	{
		printf ("\n");
	}
	for (int i = 0; i< (MAX_X/2 - 4); i++)
	{
		printf (" ");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
	printf("total harvest = %d\n", harvest);
	for (int i = 0; i< (MAX_X/2 - 4); i++)
	{
		printf (" ");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
	printf("total harvested = %d\n", harvested);
	for (int i = 0; i< (MAX_X/2 - 4); i++)
	{
		printf (" ");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),1);
	printf("USER  harvested = %d\n", dron.point);
	for (int i = 0; i< (MAX_X/2 - 4); i++)
	{
		printf (" ");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
	printf("allowed to clash = %d\n", health);
	
	getchar();
}

void StartMenu(void)
{	
	printf("\n\n");
	
	for (int i = 0; i< (MAX_X/2 - 7); i++)
	{
		printf (" ");
	}
	printf("COLLECTED all harvest ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
	printf("@\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
	
	for (int i = 0; i< (MAX_X/2 - 7); i++)
	{
		printf (" ");
	}
	printf("use the keys W A S D to control the drone ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),1);
	printf("+  +%%\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
	for (int i = 0; i< (MAX_X/2 - 7); i++)
	{
		printf (" ");
	}
	printf("keys + releases assistant drone,\n");
	for (int i = 0; i< (MAX_X/2 - 7); i++)
	{
		printf (" ");
	}
	printf("keys space change manual/auto, and P pause\n");
	for (int i = 0; i< (MAX_X/2 - 7); i++)
	{
		printf (" ");
	}
	printf("collect up to 4 carts and take them to the warehouse ->");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),51);
	printf("#\n\n\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
	for (int i = 0; i< (MAX_X/2 - 7); i++)
	{
		printf (" ");
	}
	printf("GOOD LUCK");
	getch();
}


int main()
{
	
	StartMenu();
	
	srand(time(NULL));																		
	
	warehouse_t house = {MAX_X/2, MAX_Y - 3}; 												
	food_t pumpkin[FOODQUANTITY];
	
	int harvest = init_Pumpkin(pumpkin);													
	
	dron_t dron = init_dron(house,LEFT,MANUAL);
	
	Autodron_t AutoDron[HELPDRON];
	
	init_Autodron(AutoDron, house);
	
	int key = 0;																			
	int harvested = 0;																		
	int health = 0;																			
	
	
	char matrix[MAX_X][MAX_Y];																
	printField(pumpkin,house,&dron,AutoDron,matrix,harvest,harvested,health);				
	system("cls");																			
	
	while (CheckEnd(harvest,harvested,pumpkin,&dron,AutoDron))
	{
		if ((key = inputUser(key)))
		{
			if (Execution_of_special_keys(key,&dron,AutoDron))
			{
				if ((dron.type_control == MANUAL)&&(checkDirection(&dron,key)))
				{
					changeDirection(&dron,key);
				}
			} 
		}
		if (dron.type_control == AUTO)
		{
			Alcontrol(&dron,pumpkin,house,matrix,harvest);
		}
		moveUser(&dron);
		
		
		AlchangeDirection(AutoDron,pumpkin,house,matrix,harvest);
		
		printField(pumpkin,house,&dron,AutoDron,matrix,harvest,harvested,health);
		checkEvent(&dron,AutoDron,pumpkin,house,&harvested,&health);
		usleep(300000);
		system("cls");
		updateFood(pumpkin);
	}
	endMenu(harvest,harvested,health,dron);
	
	return 0;
}
