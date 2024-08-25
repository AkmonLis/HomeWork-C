#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <time.h>

#define MAX_X 30
#define MAX_Y 15
#define START_LENGTH_SNAKE 2
#define SPASE ' '
#define MENULETTERS 9
#define END (*h)--;printf ("\a\n");


enum {LEFT=1, RIGHT, UP, DOWN, CONTROLS=3};

enum {FOOD_STATUS_EMPTU, FOOD_STATUS_PRESENT};

enum {PAUSE ='P', KEY_LEFT=1075,KEY_RIGHT=1077,KEY_UP=1072,KEY_DOWN=1080};

enum {GAMEMOD_NO, GAMEMOD_SOLO, GAMEMOD_2, GAMEMOD_AI};

enum { PLAYER1, PLAYER2 };


typedef struct Control_Battons{
	int left;
	int right;
	int up;
	int down;
}Control_Battons;


struct Control_Battons def_control  [CONTROLS] = {{KEY_LEFT, KEY_RIGHT, KEY_UP,KEY_DOWN},
												{'A','D','W','S'},
												{'a','d','w','s'}};
												




typedef struct tail_t{
	int x;
	int y;
	} tail_t;
	
	
typedef struct snake_t{
	int x;
	int y;
	int Direction;
	struct tail_t * tail;
	size_t tsize;
	struct Control_Battons* control;
	int points;
	}  snake_t;
	
	
typedef struct food_t{
	int x;
	int y;
	int status;
	int lvl;
} food_t;



int speedometer(int lvl)
{
	float a = 1.134413;
	float z = a;
	for (int i=0; i<(100 - lvl); i++)
	{
		z = z*a;
	}
	return ((int)z);
}


snake_t initSnake(int x, int y, size_t tsize, int direct){
	struct snake_t snake;
	snake.x = x;
	snake.y = y;
	snake.tsize = tsize;
	snake.Direction = direct;
	snake.tail = (tail_t *) malloc (sizeof(tail_t) * 100);
	for (int i =0; i < tsize; ++i){
		snake.tail[i].x = x - i - 1;
		snake.tail[i].y = y;
		}
	snake.control = def_control;
	snake.points = 0;
	return snake;
}


int inputDirectoin(int key)
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


int checkDirection(struct snake_t* snake, int key)
{
	for ( int i = 0; i < CONTROLS; i++)
	{
		if ((key == snake->control[i].right && snake->Direction == LEFT) ||
			(key == snake->control[i].left && snake->Direction == RIGHT) ||
			(key == snake->control[i].down && snake->Direction == UP)    ||
			(key == snake->control[i].up && snake->Direction == DOWN))
		{
			return 0;
		}
	}
	return 1;
}


void changeDirection(snake_t* snake, int key, int mod, int player)
{
	int a = 0;
	int control = CONTROLS;
	
	if ( mod != GAMEMOD_SOLO)
	{
		if (player == PLAYER1)
		{
			a=1;
		}
		else
		{
			control -=2;
		}
	}
	
	for ( int i = a; i < control; i++)
	{
		
		if (key == snake->control[i].left)
		{
			snake->Direction = LEFT;
			break;
		}
		else if (key == snake->control[i].right)
		{
			snake->Direction = RIGHT;
			break;
		}
		else if (key == snake->control[i].down)
		{
			snake->Direction = DOWN;
			break;
		}
		else if (key == snake->control[i].up)
		{
			snake->Direction = UP;
			break;
		}
	}
}

void move(snake_t* snake){
	
	for (int i = snake->tsize - 1; i > 0; i--){
		snake->tail[i] = snake->tail[i-1];
		}
	snake->tail[0].x = snake->x;
	snake->tail[0].y = snake->y;
	
	
	switch (snake->Direction)
	{
		case LEFT:
			snake->x--;
			if (snake->x < 0)
			{
				snake->x = MAX_X - 1;
			}
			break;
		case RIGHT:
			snake->x++;
			if (snake->x >= MAX_X - 1)
			{
				snake->x = 0;
			}
			break;
		case DOWN:
			snake->y++;
			if (snake->y >= MAX_Y - 1)
			{
				snake->y = 0;
			}
			break;
		case UP:
			snake->y--;
			if (snake->y < 0)
			{
				snake->y = MAX_Y - 2;
			}
			break;
	}
}


void checkEvent( snake_t* snake, snake_t* snake2, food_t* f, int* h, int* speed)
{
	if (((snake->x == f->x) && (snake->y == f->y))||((snake2->x == f->x) && (snake2->y == f->y)))
	{
		printf ("\a\n");
		f->status = FOOD_STATUS_EMPTU;
		f->lvl++;
		(*speed) = speedometer(f->lvl);
		if (snake->x == f->x)
		{
			snake->tsize++;
			snake->tail[snake->tsize-1].x = snake->tail[snake->tsize-2].x;
			snake->tail[snake->tsize-1].y = snake->tail[snake->tsize-2].y;
			snake->points++;
		}
		else
		{
			snake2->tsize++;
			snake2->tail[snake2->tsize-1].x = snake2->tail[snake2->tsize-2].x;
			snake2->tail[snake2->tsize-1].y = snake2->tail[snake2->tsize-2].y;
			snake2->points++;
		}
	}
	
	for (int i = 0; i < snake->tsize; ++i)
	{
		if (((snake->x == snake->tail[i].x) && (snake->y == snake->tail[i].y))
		  ||((snake2->x == snake->tail[i].x) && (snake2->y == snake->tail[i].y)))
		{
			END
		}
	}
	for (int i = 0; i < snake2->tsize; ++i)
	{
		if (((snake2->x == snake2->tail[i].x) && (snake2->y == snake2->tail[i].y))
			||((snake->x == snake2->tail[i].x) && (snake->y == snake2->tail[i].y)))
		{
			END
		}
	}
	
	if (((snake2->x == snake->x)&&(snake2->y == snake->y))||(f->lvl == 100))
	{
		END
	}
}

void foodEvent (food_t* food)
{
	if (food->status == FOOD_STATUS_EMPTU)
	{
		if ((rand()%1000) > 900)
		{
			food->status = FOOD_STATUS_PRESENT;
			food->x = rand()%MAX_X;
			food->y = rand()%MAX_Y;
		}
	}
}


void printSnake(snake_t snake, snake_t snake2, food_t* f, int mode)
{
	char matrix[MAX_X][MAX_Y];
	for (int i = 0; i < MAX_X; ++i){
		for (int j = 0; j < MAX_Y; ++j)
		{
			if ( i == (MAX_X-1))
			{
				matrix[i][j] = '|';
			}
			else if ( j == (MAX_Y-1))
			{
				matrix[i][j] = '_';
			}
			else
			{
				matrix[i][j] = ' ';
			}
		}
	}

	matrix[snake.x][snake.y] = '%';
	for (int i = 0; i < snake.tsize; ++i)
	{
		matrix[snake.tail[i].x][snake.tail[i].y] = '#';
	}

	
	if ( mode != GAMEMOD_SOLO )
	{
		matrix[snake2.x][snake2.y] = 'O';
		for (int i = 0; i < snake2.tsize; ++i)
		{
			matrix[snake2.tail[i].x][snake2.tail[i].y] = '#';
		}
	}
	
	
	
	if (f->status == FOOD_STATUS_PRESENT)
	{
		if (matrix[f->x][f->y] == ' ')
		{
			matrix[f->x][f->y] = '@';
		}
		else 
		{
			f->status = FOOD_STATUS_EMPTU;
		}
	}
	
	for (int j = 0; j < MAX_Y; ++j){
		for (int i = 0; i < MAX_X; ++i)
		{
			printf("%c", matrix[i][j]);
			}
			printf("\n");
			}
	if (( mode == GAMEMOD_SOLO)||( mode == GAMEMOD_NO))
	{
		printf("LEVEL  =  %d", f->lvl);
	}
	else 
	{
		printf("PLAYER1=%d", snake.points);
		int spase = (MAX_X - 20);
		for (int i = 0; i<spase; i++)
		{
			printf(" ");
		}
		printf("PLAYER2=%d", snake2.points);
	}
}

void pause (void)
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
	do
	{
	}
	while ( getch() != PAUSE);
}


snake_t AI_Change_Direction(snake_t snake,food_t food)
{
	if (food.status == FOOD_STATUS_PRESENT)
	{
		if ((snake.Direction == LEFT)||(snake.Direction == RIGHT))
		{
			if (((snake.y - food.y) > 0 ) || (((snake.y - food.y)<0)&&((food.y - snake.y)>(snake.y + MAX_Y - food.y ))))
			{
				for (int i = 0; i<snake.tsize; i++)
				{
					if ((snake.x == snake.tail[i].x) && ((snake.y+1) == snake.tail[i].y))
					{
						break;
					}
					if ( i == (snake.tsize-1))
					{
						snake.Direction = UP;
					}
				}
			}
			else if (((snake.y - food.y) < 0) || (((snake.y - food.y)>0)&&((snake.y - food.y)>(food.y + MAX_Y - snake.y))))
			{
				for (int i = 0; i<snake.tsize; i++)
				{
					if ((snake.x == snake.tail[i].x) && ((snake.y-1) == snake.tail[i].y))
					{
						break;
					}
					if ( i == (snake.tsize-1))
					{
						snake.Direction = DOWN;
					}
				}
			}
		}
		else
		{
			if (((snake.x - food.x) > 0 ) || (((snake.x - food.x)<0)&&((food.x - snake.x)>(snake.x + MAX_X - food.x ))))
			{
				for (int i = 0; i<snake.tsize; i++)
				{
					if ((snake.y == snake.tail[i].y) && ((snake.x-1) == snake.tail[i].x))
					{
						break;
					}
					if ( i == (snake.tsize-1))
					{
						snake.Direction = LEFT;
					}
				}
			}
			else if (((snake.x - food.x) < 0) || (((snake.x - food.x)>0)&&((snake.x - food.x)>(food.x + MAX_X - snake.x))))
			{
				for ( int i = 0; i<snake.tsize; i++)
				{
					if ((snake.y == snake.tail[i].y) && ((snake.x+1) == snake.tail[i].x))
					{
						break;
					}
					if ( i == (snake.tsize-1))
					{
						snake.Direction = RIGHT;
					}
				}
			}
		}
	}
	else
	{
		int d = rand()%(8)+KEY_UP;
		if (checkDirection(&snake, d))
		{
			changeDirection(&snake, d, GAMEMOD_AI, PLAYER2);
		}
	}
	
	
	return snake;
}




void endGame (int lvl, int mode, snake_t snake, snake_t snake2)
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
	lvl == 100 ? printf("	END GAME\n\n") : printf("	GAME OVER\n\n");
	for (int i = 0; i< (MAX_X/2-2); i++)
	{
		printf (" ");
	}
	if ( mode == GAMEMOD_SOLO)
	{
		printf("Your score = %d\n", lvl);
	}
	else 
	{
		printf("Winner score = %d\n", snake.points >= snake2.points ? snake.points : snake2.points);
	}
	getchar();
}


void printMenu(snake_t snake,food_t* f)
{
	char matrix[MAX_X][MAX_Y];
	for (int i = 0; i < MAX_X; ++i)
	{
		for (int j = 0; j < MAX_Y; ++j)
		{
			if ( i == (MAX_X-1))
			{
				matrix[i][j] = '|';
			}
			else if ( j == (MAX_Y-1))
			{
				matrix[i][j] = '_';
			}
			else
			{
				matrix[i][j] = SPASE;
			}
		}
	}
	
	matrix[snake.x][snake.y] = '%';
	for (int i = 0; i < snake.tsize; ++i)
	{
		matrix[snake.tail[i].x][snake.tail[i].y] = '#';
	}
	
	for (int i = 0; i<MENULETTERS; i++)
	{
		if ((f[i].status == FOOD_STATUS_PRESENT)&&(matrix[f[i].x][f[i].y] == SPASE))
		{
				matrix[f[i].x][f[i].y] = '@';
		}
		else if ((f[i].status == FOOD_STATUS_EMPTU)&&(matrix[f[i].x][f[i].y] == SPASE))
		{
			if (i == MENULETTERS-9)
			{
				matrix[f[i].x][f[i].y] = 'T';
			}
			else if (i == MENULETTERS-8)
			{
				matrix[f[i].x][f[i].y] = 'H';
			}
			else if (i == MENULETTERS-7)
			{
				matrix[f[i].x][f[i].y] = 'E';
			}
			else if (i == MENULETTERS-6)
			{
				matrix[f[i].x][f[i].y] = 'S';
			}
			else if (i == MENULETTERS-5)
			{
				matrix[f[i].x][f[i].y] = 'N';
			}
			else if (i == MENULETTERS-4)
			{
				matrix[f[i].x][f[i].y] = 'A';
			}
			else if (i == MENULETTERS-3)
			{
				matrix[f[i].x][f[i].y] = 'K';
			}
			else if (i == (MENULETTERS-2))
			{
				matrix[f[i].x][f[i].y] = 'E';
			}
		}
	}
	
	
	
	for (int j = 0; j < MAX_Y; ++j)
	{
		for (int i = 0; i < MAX_X; ++i)
		{
			
			if ((i == MAX_X/2-6)&&(j == MAX_Y/2+1))
			{
				printf("   CHOISE MOD  ");
				
			}
			if ((i == MAX_X/2-6)&&(j == MAX_Y/2+2))
			{
				printf("1. PLAYER SOLO ");
			}
			if ((i == MAX_X/2-6)&&(j == MAX_Y/2+3))
			{
				printf("2. TWO PLAYER  ");
			}
			if ((i == MAX_X/2-6)&&(j == MAX_Y/2+4))
			{
				printf("3. PLAYER vs AI");
			}
			if (((i >= MAX_X/2-6)&&(j >= MAX_Y/2+1))&&((i <= MAX_X/2+8)&&(j <= MAX_Y/2+4)))
			{
				continue;
				
			}
			printf("%c", matrix[i][j]);
		}
		printf("\n");
	}
}

void eventMenu(snake_t* snake,food_t* f)
{
	for (int i = 0; i < MENULETTERS; i++)
	{
		if ((snake->x == f[i].x) && (snake->y == f[i].y))
		{
			printf ("\a\n");
			f[i].status = FOOD_STATUS_EMPTU;
			snake->tsize++;
			snake->tail[snake->tsize-1].x = snake->tail[snake->tsize-2].x;
			snake->tail[snake->tsize-1].y = snake->tail[snake->tsize-2].y;
			snake->points++;
		}
	}
}


int StartMenu(void)
{
	snake_t snake = initSnake(3,MAX_Y/2-2,START_LENGTH_SNAKE-1,RIGHT);
	food_t food[MENULETTERS] = {{MAX_X/2-4, MAX_Y/2-2, FOOD_STATUS_PRESENT,0},
								{MAX_X/2-3, MAX_Y/2-2, FOOD_STATUS_PRESENT,0},
								{MAX_X/2-2, MAX_Y/2-2, FOOD_STATUS_PRESENT,0},
								{MAX_X/2, MAX_Y/2-2, FOOD_STATUS_PRESENT,0},
								{MAX_X/2+1, MAX_Y/2-2, FOOD_STATUS_PRESENT,0},
								{MAX_X/2+2, MAX_Y/2-2, FOOD_STATUS_PRESENT,0},
								{MAX_X/2+3, MAX_Y/2-2, FOOD_STATUS_PRESENT,0},
								{MAX_X/2+4, MAX_Y/2-2, FOOD_STATUS_PRESENT,0},
								{MAX_X/2-6, MAX_Y/2-1,FOOD_STATUS_EMPTU,0}};
					
	while (snake.points != 9)
	{
		system("cls");
		printMenu(snake,food);
		move(&snake);
		eventMenu(&snake,food);
		if((snake.x == food[MENULETTERS-2].x)&&(snake.y == (food[MENULETTERS-2].y)))
		{
			snake.Direction = DOWN;
		}
		if ((snake.x == food[MENULETTERS-2].x)&&(snake.y == (food[MENULETTERS-2].y+1)))
		{
			snake.Direction = LEFT;
		}
		usleep(100000);
	}
	int choise = 0;
	int taimer = 0;
	while (taimer < 10)
	{
		if(kbhit())
		{
			choise = getch();
			if (choise == '1')
			{
				return GAMEMOD_SOLO;
			}
			else if (choise == '2')
			{
				return GAMEMOD_2;
			}
			else if (choise == '3')
			{
				return GAMEMOD_AI;
			}
		}
		sleep(1);
		taimer++;
	}
	system("cls");
	return GAMEMOD_NO;
}






int main()
{
	srand(time(NULL));
	
	
	int game_mod = StartMenu();

	
	
	
	int health = 1, key_press;
	
	int x_snake1 = -1, y_snake1 = -1;
	int x_snake2 = -1, y_snake2 = -1;
	
	if ( game_mod == GAMEMOD_SOLO)
	{
		x_snake1 = rand()%MAX_X;
		y_snake1 = rand()%MAX_Y;
	}
	else if (game_mod == GAMEMOD_NO)
	{
		x_snake2 = rand()%MAX_X;
		y_snake2 = rand()%MAX_Y;
	}
	else
	{
		x_snake1 = MAX_X/2;
		y_snake1 = MAX_Y/2;
		x_snake2 = MAX_X/3;
		y_snake2 = MAX_Y/3;
	}
	
	snake_t snake = initSnake( x_snake1, y_snake1, START_LENGTH_SNAKE,UP);
	
	snake_t snake2 = initSnake( x_snake2, y_snake2, START_LENGTH_SNAKE,DOWN);

	food_t food = {rand()%MAX_X,rand()%MAX_Y,FOOD_STATUS_EMPTU,0};
	
	int speedGame = speedometer(food.lvl);
	
	printSnake(snake,snake2,&food,game_mod);
	
	
	while(health)
	{
		if ((key_press = inputDirectoin(key_press)))
		{
			if (key_press == PAUSE)
			{
				pause();
			}
			else
			{
				if (checkDirection(&snake,key_press))
				{
					changeDirection(&snake,key_press,game_mod, PLAYER1);
				}
				if (game_mod == GAMEMOD_2)
				{
					if (checkDirection(&snake2,key_press))
					{
						changeDirection(&snake2,key_press,game_mod, PLAYER2);
					}
				}
			}
		}
		
		move(&snake);
		
		if (game_mod == GAMEMOD_2)
		{
			move(&snake2);
		}
		else if ((game_mod == GAMEMOD_AI)||(game_mod == GAMEMOD_NO))
		{
			snake2 = AI_Change_Direction(snake2,food);
			move(&snake2);
		}
		
		checkEvent(&snake,&snake2,&food, &health, &speedGame);
		
		usleep(speedGame);
		
		system("cls");
		
		printSnake(snake,snake2,&food,game_mod);
		
		foodEvent(&food);
	}
	
	endGame(food.lvl, game_mod,snake,snake2);
	
	
	return 0;
}
