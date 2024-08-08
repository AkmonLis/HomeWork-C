#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>

#define MAX_X 30
#define MAX_Y 15
#define StartSpeed 300000
#define ChangeSpeed 20000
#define ChangeSpeed2 1000
#define FoodStatusEmpty 0
#define FoodStatusPresent 1
#define FoodStatusEaten 2

enum {left = -1, right = 1, up = -1, down = 1, zero = 0};
enum {arrx = 0, arry = 1};

typedef struct tail_t{
	int x;
	int y;
	} tail_t;
	
typedef struct snake_t{
	int x;
	int y;
	struct tail_t * tail;
	size_t tsize;
	}  snake_t;
	
typedef struct food_t{
	int x;
	int y;
	int status;
} food_t;


struct snake_t initSnake(int x, int y, size_t tsize){
	struct snake_t snake;
	snake.x = x;
	snake.y = y;
	snake.tsize = tsize;
	snake.tail = (tail_t *) malloc (sizeof(tail_t) * 100);
	for (int i =0; i < tsize; ++i){
		snake.tail[i].x = x + i + 1;
		snake.tail[i].y = y;
		}
	return snake;
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
		
		if (f->status == FoodStatusPresent)
		{
			if (matrix[f->x][f->y] == ' ')
			{
				matrix[f->x][f->y] = '@';
			}
			else 
			{
				f->status = FoodStatusEmpty;
			}
		}
		
		for (int j = 0; j < MAX_Y; ++j){
			for (int i = 0; i < MAX_X; ++i)
			{
				printf("%c", matrix[i][j]);
				}
				printf("\n");
				}
}


snake_t move(snake_t snake, int k[]){
	for (int i = snake.tsize - 1; i > 0; i--){
		snake.tail[i] = snake.tail[i-1];
		}
	snake.tail[0].x = snake.x;
	snake.tail[0].y = snake.y;
	
	snake.x = snake.x + k[arrx];
	snake.y = snake.y + k[arry];
	if (snake.x < 0){
		snake.x = MAX_X - 2;
		}
	else if (snake.x >= MAX_X-1){
		snake.x = 0;
		}
	else if (snake.y >= MAX_Y-1){
		snake.y = 0;
		}
	else if (snake.y < 0){
		snake.y = MAX_Y - 2;
		}
	return snake;
	}
	
	
void choiceDirectoin(int XY[])
{
	if(kbhit())
	{
		int ch = getch();
		if (ch == 224)
		{
			ch = getch()+1000;
		}
		switch(ch)
		{
			case 65:
			case 97:
			case 228:
			case 148:
			case 1075:
				if( XY[arrx] == left || XY[arrx] == right )
				{
					return;
				}
				XY[arrx] = left;
				XY[arry] = zero;
				break;
			case 100:
			case 68:
			case 162:
			case 130:
			case 1077:
				if(XY[arrx] == right || XY[arrx] == left )
				{
					return;
				}
				XY[arrx] = right;
				XY[arry] = zero;
				break;
			case 119:
			case 87:
			case 150:
			case 230:
			case 1072:
				if(XY[arry] == up || XY[arry] == down)
				{
					return;
				}
				XY[arrx] = zero;
				XY[arry] = up;
				break;
			case 115:
			case 83:
			case 235:
			case 155:
			case 1080:
				if(XY[arry] == down || XY[arry] == up)
				{
					return;
				}
				XY[arrx] = zero;
				XY[arry] = down;
				break;
			
        }
    }
}

snake_t checkEvent( snake_t snake, food_t* f, int* h, int* speed)
{
	if ((snake.x == f->x) && (snake.y == f->y))
	{
		printf ("\a\n");
		f->status = FoodStatusEaten;
	}
	
	if ((f->status == FoodStatusEaten)&&(f->x == snake.tail[snake.tsize-1].x) && (f->y == snake.tail[snake.tsize-1].y))
	{
		snake.tsize++;
		snake.tail[snake.tsize-1].x = f->x;
		snake.tail[snake.tsize-1].y = f->y;
		if (*speed < ChangeSpeed)
		{
			*speed -= ChangeSpeed2;
		}
		else
		{
			*speed -= ChangeSpeed;
		}
		f->status = FoodStatusEmpty;
	}
	
	for (int i =0; i < snake.tsize; ++i)
	{
		if ((snake.x == snake.tail[i].x) && (snake.y == snake.tail[i].y))
		{
			(*h)--;
			printf ("\a\n");
		}
	}
	return snake;
}

food_t foodEvent (food_t food)
{
	if (food.status == FoodStatusEmpty)
	{
		if ((rand()%1000) > 850)
		{
			food.status++;
			food.x = rand()%MAX_X;
			food.y = rand()%MAX_Y;
		}
	}
	return food;
}
	
int main(){
	srand(5);
	int health = 1;
	int speedGame = StartSpeed;
	struct snake_t snake = initSnake( rand()%MAX_X, rand()%MAX_Y, 2);
	food_t food = {rand()%MAX_X,rand()%MAX_Y,FoodStatusEmpty};
	printSnake(snake,&food);
	int KeyDirectionXY[] = {left,zero};
	
	
	while(health)
	{
		choiceDirectoin(KeyDirectionXY);
		snake = move(snake,KeyDirectionXY);
		snake = checkEvent(snake,&food, &health, &speedGame);
		usleep(speedGame);
		system("cls");
		printSnake(snake,&food);
		food = foodEvent(food);
	}
	return 0;
}
