#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>

#define MAX_X 30
#define MAX_Y 15
#define START_LENGTH_SNAKE 2


enum {LEFT=1, RIGHT, UP, DOWN, CONTROLS=3};

enum {FOOD_STATUS_EMPTU, FOOD_STATUS_PRESENT, FOOD_STATUS_EATEN};

enum {PAUSE ='P', KEY_LEFT=1075,KEY_RIGHT=1077,KEY_UP=1072,KEY_DOWN=1080};

typedef struct Control_Battons{
	int left;
	int right;
	int up;
	int down;
}Control_Battons;


struct Control_Battons def_control[CONTROLS] = {{KEY_LEFT, KEY_RIGHT, KEY_UP,KEY_DOWN},
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


struct snake_t initSnake(int x, int y, size_t tsize, int direct){
	struct snake_t snake;
	snake.x = x;
	snake.y = y;
	snake.tsize = tsize;
	snake.Direction = direct;
	snake.tail = (tail_t *) malloc (sizeof(tail_t) * 100);
	for (int i =0; i < tsize; ++i){
		snake.tail[i].x = x + i + 1;
		snake.tail[i].y = y;
		}
	snake.control = def_control;
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
	for (int i = 0; i < CONTROLS; i++)
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


void changeDirection(snake_t* snake, int key)
{
	for (int i = 0; i < CONTROLS; i++)
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





void checkEvent( snake_t* snake, food_t* f, int* h, int* speed)
{
	if ((snake->x == f->x) && (snake->y == f->y))
	{
		printf ("\a\n");
		f->status = FOOD_STATUS_EATEN;
		f->lvl++;
		(*speed) = speedometer(f->lvl);
	}
	
	if ((f->status == FOOD_STATUS_EATEN)&&(f->x == snake->tail[snake->tsize-1].x) && (f->y == snake->tail[snake->tsize-1].y))
	{
		snake->tsize++;
		snake->tail[snake->tsize-1].x = f->x;
		snake->tail[snake->tsize-1].y = f->y;
		f->status = FOOD_STATUS_EMPTU;
	}
	
	for (int i =0; i < snake->tsize; ++i)
	{
		if (((snake->x == snake->tail[i].x) && (snake->y == snake->tail[i].y)) || (f->lvl == 100))
		{
			(*h)--;
			printf ("\a\n");
		}
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


void printSnake(struct snake_t snake, food_t* f)
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
	for (int i = 0; i < snake.tsize; ++i){
		matrix[snake.tail[i].x][snake.tail[i].y] = '#';
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
	
	printf("LEVEL  =  %d", f->lvl);
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

void endGame (int lvl)
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
	printf("your level = %d", lvl);
}

int main()
{
	srand(5);
	int health = 1, key_press;
	struct snake_t snake = initSnake( rand()%MAX_X, rand()%MAX_Y, START_LENGTH_SNAKE,LEFT);
	food_t food = {rand()%MAX_X,rand()%MAX_Y,FOOD_STATUS_EMPTU,0};
	int speedGame = speedometer(food.lvl);
	printSnake(snake,&food);
	
	
	while(health)
	{
		if ((key_press = inputDirectoin(key_press)))
		{
			if (key_press == PAUSE)
			{
				pause();
			}
			else if (checkDirection(&snake,key_press))
			{
				changeDirection(&snake,key_press);
			}
		}
		
		move(&snake);
		
		checkEvent(&snake,&food, &health, &speedGame);
		
		usleep(speedGame);
		
		system("cls");
		
		printSnake(snake,&food);
		
		foodEvent(&food);
	}
	
	endGame(food.lvl);
	
	return 0;
}
