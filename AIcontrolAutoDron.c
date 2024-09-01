#include "HeaderFail.h"


void moveAuto(Autodron_t* autodron, int k)
{

	for (int i = autodron[k].tsize - 1; i > 0; i--){
		autodron[k].cart[i] = autodron[k].cart[i-1];
		}
	autodron[k].cart[0].x = autodron[k].x;
	autodron[k].cart[0].y = autodron[k].y;
		
		
	switch (autodron[k].Direction)
	{
		case LEFT:
			autodron[k].x--;
			if (autodron[k].x < 1)
			{
				autodron[k].x = MAX_X - 2;
			}
			break;
		case RIGHT:
			autodron[k].x++;
			if (autodron[k].x >= MAX_X - 2)
			{
				autodron[k].x = 1;
			}
			break;
		case DOWN:
			autodron[k].y++;
			if (autodron[k].y >= MAX_Y - 2)
			{
				autodron[k].y = 1;
			}
			break;
		case UP:
			autodron[k].y--;
			if (autodron[k].y < 1)
			{
				autodron[k].y = MAX_Y - 2;
			}
			break;
	}
	
}


int check_correct_direction (Autodron_t* AutoDron,
						int numberDron, int choiseDirection,char matrix[MAX_X][MAX_Y])
{
	if (choiseDirection == DOWN)															
	{
		if ((matrix[AutoDron[numberDron].x][AutoDron[numberDron].y+1] != DRON)&&			
			(matrix[AutoDron[numberDron].x][AutoDron[numberDron].y+1] != CART)&&			
			(matrix[AutoDron[numberDron].x][AutoDron[numberDron].y+1] != RIPENING))
		{
			AutoDron[numberDron].Direction = DOWN;
			return 0;
		}
	}
	else if (choiseDirection == UP)
	{
		if ((matrix[AutoDron[numberDron].x][AutoDron[numberDron].y-1] != DRON)&&
			(matrix[AutoDron[numberDron].x][AutoDron[numberDron].y-1] != CART)&&
			(matrix[AutoDron[numberDron].x][AutoDron[numberDron].y-1] != RIPENING))
		{
			AutoDron[numberDron].Direction = UP;
			return 0;
		}
	}
	else if (choiseDirection == LEFT)
	{
		if ((matrix[AutoDron[numberDron].x-1][AutoDron[numberDron].y] != DRON)&&
			(matrix[AutoDron[numberDron].x-1][AutoDron[numberDron].y] != CART)&&
			(matrix[AutoDron[numberDron].x-1][AutoDron[numberDron].y] != RIPENING))
		{
			AutoDron[numberDron].Direction = LEFT;
			return 0;
		}
	}
	else if (choiseDirection == RIGHT)
	{
		if ((matrix[AutoDron[numberDron].x+1][AutoDron[numberDron].y] != DRON)&&
			(matrix[AutoDron[numberDron].x+1][AutoDron[numberDron].y] != CART)&&
			(matrix[AutoDron[numberDron].x+1][AutoDron[numberDron].y] != RIPENING))
		{
			AutoDron[numberDron].Direction = RIGHT;
			return 0;
		}
	}
	return 1;
}


void AlchangeDirection(Autodron_t* AutoDron,food_t* pumpkin,warehouse_t house,
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
	
	for (int k = 0; k<HELPDRON;k++)															
	{

		if (AutoDron[k].status == OFF)														
		{
			break;
		}
																							
		int target = -1;																	
		if ((AutoDron[k].tsize < LENGTHCART-1)&&(count_pupkin_collected != harvest))		   
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
				else if (((checkMinus(DestinatoinX(AutoDron[k].x,pumpkin[target].x))) +		
						  (checkMinus(DestinatoinY(AutoDron[k].y,pumpkin[target].y)))) >	
						 ((checkMinus(DestinatoinX(AutoDron[k].x,pumpkin[i].x))) +
						  (checkMinus(DestinatoinY(AutoDron[k].y,pumpkin[i].y)))))
				{
					target = i;																
					pumpkin[i].employment = ON;
				} 
			}
			
																							
			if ((AutoDron[k].Direction == LEFT)||(AutoDron[k].Direction == RIGHT))
			{
				if (target == -1)
				{
					if ((matrix[AutoDron[k].x][AutoDron[k].y+1] = DRON)||					
						(matrix[AutoDron[k].x][AutoDron[k].y+1] = CART))
						{
							check_correct_direction(AutoDron,k,UP,matrix);
						}
					if ((matrix[AutoDron[k].x][AutoDron[k].y-1] = DRON)||
						(matrix[AutoDron[k].x][AutoDron[k].y-1] = CART))
						{
							check_correct_direction(AutoDron,k,DOWN,matrix);
						}
					if (check_correct_direction(AutoDron,k,AutoDron[k].Direction,matrix))
					{																	 
						if (check_correct_direction(AutoDron,k,DOWN,matrix))			 
						{
							check_correct_direction(AutoDron,k,UP,matrix);				 
						}
					}
				}
				else
				{
					if ((DestinatoinY(AutoDron[k].y,pumpkin[target].y)) > 0)
					{
						check_correct_direction(AutoDron,k,DOWN,matrix);
					} 
					else if ((DestinatoinY(AutoDron[k].y,pumpkin[target].y)) < 0)
					{
						check_correct_direction(AutoDron,k,UP,matrix);
					}
					else if (check_correct_direction(AutoDron,k,AutoDron[k].Direction,matrix))
					{																		  
						if (check_correct_direction(AutoDron,k,DOWN,matrix))			 	
						{
							check_correct_direction(AutoDron,k,UP,matrix);					 
						}
					}
				}
			}
			else
			{
				
				if (target == -1)
				{
					if ((matrix[AutoDron[k].x+1][AutoDron[k].y] = DRON)||					
						(matrix[AutoDron[k].x+1][AutoDron[k].y] = CART))
						{
							check_correct_direction(AutoDron,k,LEFT,matrix);
						}
					if ((matrix[AutoDron[k].x-1][AutoDron[k].y] = DRON)||
						(matrix[AutoDron[k].x-1][AutoDron[k].y] = CART))
						{
							check_correct_direction(AutoDron,k,RIGHT,matrix);
						}
					if (check_correct_direction(AutoDron,k,AutoDron[k].Direction,matrix))
					{																	 
						if (check_correct_direction(AutoDron,k,LEFT,matrix))				  
						{
							check_correct_direction(AutoDron,k,RIGHT,matrix);			
						}
					}
				}
				else
				{
					if ((DestinatoinX(AutoDron[k].x,pumpkin[target].x)) > 0)
					{
						check_correct_direction(AutoDron,k,RIGHT,matrix);
					} 
					else if ((DestinatoinX(AutoDron[k].x,pumpkin[target].x)) < 0)
					{
						check_correct_direction(AutoDron,k,LEFT,matrix);
					}
					else if (check_correct_direction(AutoDron,k,AutoDron[k].Direction,matrix)) 
					{																	 		
						if (check_correct_direction(AutoDron,k,LEFT,matrix))					
						{
							check_correct_direction(AutoDron,k,RIGHT,matrix);			
						}
					}
				}
			}
			
		}
		else 																				
		{
			if ((AutoDron[k].Direction == LEFT)||(AutoDron[k].Direction == RIGHT))
			{
				if ((DestinatoinY(AutoDron[k].y,house.y)) > 0)
				{
					check_correct_direction(AutoDron,k,DOWN,matrix);
				} 
				else if ((DestinatoinY(AutoDron[k].y,house.y)) < 0)
				{
					check_correct_direction(AutoDron,k,UP,matrix);
				}
				else if (check_correct_direction(AutoDron,k,AutoDron[k].Direction,matrix))
				{																	 		
					if (check_correct_direction(AutoDron,k,DOWN,matrix))			 		 
					{
						check_correct_direction(AutoDron,k,UP,matrix);				 		
					}
				}
			}
			else
			{
				if ((DestinatoinX(AutoDron[k].x,house.x)) > 0)
				{
					check_correct_direction(AutoDron,k,RIGHT,matrix);
				} 
				else if ((DestinatoinX(AutoDron[k].x,house.x)) < 0)
				{
					check_correct_direction(AutoDron,k,LEFT,matrix);
				}
				else if (check_correct_direction(AutoDron,k,AutoDron[k].Direction,matrix))
				{																		  
					if (check_correct_direction(AutoDron,k,LEFT,matrix))			 	 
					{
						check_correct_direction(AutoDron,k,RIGHT,matrix);			 	
					}
				}
			}
		}
		
		moveAuto(AutoDron,k);															
		matrix[AutoDron[k].x][AutoDron[k].y] = DRON;									
		for (int i = 0; i < AutoDron[k].tsize; ++i)
		{
			matrix[AutoDron[k].cart[i].x][AutoDron[k].cart[i].y] = CART;
		}
	}
}
