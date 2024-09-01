#include "HeaderFail.h"

int inputUser(int key)																		
{
	if(kbhit())																				
	{
		key = getch();																		
		if (key == 224)																		
		{																					
			key = getch()+1000;																
		}
		return key;
    }
    return 0;
}


int checkDirection(dron_t* dron, int key)													
{
	for ( int i = 0; i < CONTROLS; i++)
	{
		if ((key == dron->control[i].right && dron->Direction == LEFT) ||
			(key == dron->control[i].left && dron->Direction == RIGHT) ||
			(key == dron->control[i].down && dron->Direction == UP)    ||
			(key == dron->control[i].up && dron->Direction == DOWN))
		{
			return 0;
		}
	}
	return 1;
}


void changeDirection(dron_t* dron, int key)													
{

	for ( int i = 0; i < CONTROLS; i++)
	{
		
		if (key == dron->control[i].left)
		{
			dron->Direction = LEFT;
			break;
		}
		else if (key == dron->control[i].right)
		{
			dron->Direction = RIGHT;
			break;
		}
		else if (key == dron->control[i].down)
		{
			dron->Direction = DOWN;
			break;
		}
		else if (key == dron->control[i].up)
		{
			dron->Direction = UP;
			break;
		}
	}
}


int Execution_of_special_keys (int key, dron_t* dron, Autodron_t* Autodron)					
{
	while(key == KEY_PAUSE)																	
	{
		system("cls");
		for (int i = 0; i< (MAX_Y/2); i++)
		{
			printf ("\n");
		}
		for (int i = 0; i< (MAX_X/2 - 2); i++)
		{
			printf (" ");
		}
		printf ("PAUSE\n\n");
		for (int i = 0; i< (MAX_X/2 - 5); i++)
		{
			printf (" ");
		}
		printf ("PRESS KEY P");
		if (getch() == KEY_PAUSE)
		{
			return 0;																		
		}
	}
	
	if (key == KEY_SPACE)																	
	{
		if (dron->type_control == MANUAL)													
		{
			dron->type_control = AUTO;
		}
		else
		{
			dron->type_control = MANUAL;
		}
		return 0;																			
	}
	
	if (key == KEY_PLUS)
	{
		for (int i = 0 ; i < HELPDRON; i++)													
		{
			if (Autodron[i].status == OFF)													
			{
				Autodron[i].status = ON;													
				break;
			}
		}
		return 0;																			
	}
	return 1;																				
}


void moveUser(dron_t* dron)																	
{
	
	for (int i = dron->tsize - 1; i > 0; i--){
		dron->cart[i] = dron->cart[i-1];
		}
	dron->cart[0].x = dron->x;
	dron->cart[0].y = dron->y;
	
	
	switch (dron->Direction)
	{
		case LEFT:
			dron->x--;
			if (dron->x < 1)
			{
				dron->x = MAX_X - 2;
			}
			break;
		case RIGHT:
			dron->x++;
			if (dron->x >= MAX_X - 2)
			{
				dron->x = 1;
			}
			break;
		case DOWN:
			dron->y++;
			if (dron->y >= MAX_Y - 2)
			{
				dron->y = 1;
			}
			break;
		case UP:
			dron->y--;
			if (dron->y < 1)
			{
				dron->y = MAX_Y - 2;
			}
			break;
	}
}


int check_directionAI (dron_t* Dron, int choiseDirection,char matrix[MAX_X][MAX_Y])			
{
	if (choiseDirection == DOWN)															
	{
		if ((matrix[Dron->x][Dron->y+1] != DRON)&&											
			(matrix[Dron->x][Dron->y+1] != CART)&&											
			(matrix[Dron->x][Dron->y+1] != RIPENING))
		{
			Dron->Direction = DOWN;
			return 0;
		}
	}
	else if (choiseDirection == UP)
	{
		if ((matrix[Dron->x][Dron->y-1] != DRON)&&
			(matrix[Dron->x][Dron->y-1] != CART)&&
			(matrix[Dron->x][Dron->y-1] != RIPENING))
		{
			Dron->Direction = UP;
			return 0;
		}
	}
	else if (choiseDirection == LEFT)
	{
		if ((matrix[Dron->x-1][Dron->y] != DRON)&&
			(matrix[Dron->x-1][Dron->y] != CART)&&
			(matrix[Dron->x-1][Dron->y] != RIPENING))
		{
			Dron->Direction = LEFT;
			return 0;
		}
	}
	else if (choiseDirection == RIGHT)
	{
		if ((matrix[Dron->x+1][Dron->y] != DRON)&&
			(matrix[Dron->x+1][Dron->y] != CART)&&
			(matrix[Dron->x+1][Dron->y] != RIPENING))
		{
			Dron->Direction = RIGHT;
			return 0;
		}
	}
	return 1;
}


void Alcontrol(dron_t* dron,food_t* pumpkin,warehouse_t house, 
													char matrix[MAX_X][MAX_Y], int harvest)	
{
	int count_pupkin_collected = 0;															
	for (int i = 0; i< FOODQUANTITY; i++)
	{
		if (pumpkin[i].status == PUMPKIN_STATUS_COLLECTED)
		{
			count_pupkin_collected++;
		}
	}
																							
	int target = -1;																		
	if ((dron->tsize < LENGTHCART-1)&&(count_pupkin_collected != harvest))					  
	{
		for (int i = 0; i< FOODQUANTITY;i++)												
		{
			
			if ((pumpkin[i].status != PUMPKIN_STATUS_RIPEN)||(pumpkin[i].employment == ON)) 
			{
				continue;																	
			}																				
			if (target == -1)
			{	
				target = i; 																
				pumpkin[i].employment = ON;
			}
			else if (((checkMinus(DestinatoinX(dron->x,pumpkin[target].x))) +				
					  (checkMinus(DestinatoinY(dron->y,pumpkin[target].y)))) >				
					 ((checkMinus(DestinatoinX(dron->x,pumpkin[i].x))) +
					  (checkMinus(DestinatoinY(dron->y,pumpkin[i].y)))))
			{
				target = i;																	
				pumpkin[i].employment = ON;
			} 
		}
		
																							
		if ((dron->Direction == LEFT)||(dron->Direction == RIGHT))
		{
			if (target == -1)
			{
					if (check_directionAI(dron,dron->Direction,matrix))						
					{																		
						if (check_directionAI(dron,DOWN,matrix))			 				
						{
							check_directionAI(dron,UP,matrix);				 				
						}
					}
			}
			else
			{
				if ((DestinatoinY(dron->y,pumpkin[target].y)) > 0)
				{
					check_directionAI(dron,DOWN,matrix);
				} 
				else if ((DestinatoinY(dron->y,pumpkin[target].y)) < 0)
				{
					check_directionAI(dron,UP,matrix);
				}
				else if (check_directionAI(dron,dron->Direction,matrix))					
				{																			
					if (check_directionAI(dron,DOWN,matrix))			 					
					{
						check_directionAI(dron,UP,matrix);					 				
					}
				}
			}
		}
		else
		{
			
			if (target == -1)
			{
					if (check_directionAI(dron,dron->Direction,matrix))						
					{																		 
						if (check_directionAI(dron,LEFT,matrix))				 			 
						{
							check_directionAI(dron,RIGHT,matrix);			
						}
					}
			}
			else
			{
				if ((DestinatoinX(dron->x,pumpkin[target].x)) > 0)
				{
					check_directionAI(dron,RIGHT,matrix);
				} 
				else if ((DestinatoinX(dron->x,pumpkin[target].x)) < 0)
				{
					check_directionAI(dron,LEFT,matrix);
				}
				else if (check_directionAI(dron,dron->Direction,matrix))					 
				{																			
					if (check_directionAI(dron,LEFT,matrix))								
					{
						check_directionAI(dron,RIGHT,matrix);			
					}
				}
			}
		}
		
	}
	else																					
	{
		if ((dron->Direction == LEFT)||(dron->Direction == RIGHT))
		{
			if ((DestinatoinY(dron->y,house.y)) > 0)
			{
				check_directionAI(dron,DOWN,matrix);
			} 
			else if ((DestinatoinY(dron->y,house.y)) < 0)
			{
				check_directionAI(dron,UP,matrix);
			}
			else if (check_directionAI(dron,dron->Direction,matrix))						
			{																	 			
				if (check_directionAI(dron,DOWN,matrix))			 						
				{
					check_directionAI(dron,UP,matrix);				 						
				}
			}
		}
		else
		{
			if ((DestinatoinX(dron->x,house.x)) > 0)
			{
				check_directionAI(dron,RIGHT,matrix);
			} 
			else if ((DestinatoinX(dron->x,house.x)) < 0)
			{
				check_directionAI(dron,LEFT,matrix);
			}
			else if (check_directionAI(dron,dron->Direction,matrix))						
			{																				
				if (check_directionAI(dron,LEFT,matrix))			 						
				{
					check_directionAI(dron,RIGHT,matrix);			 						
				}
			}
		}
	}
}

