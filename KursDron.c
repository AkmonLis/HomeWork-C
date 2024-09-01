#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <time.h>
#include <windows.h>

#define MAX_X 40																			//размер
#define MAX_Y 20																			//игрового поля
#define FOODQUANTITY 400																	//максимальное количество тыкв,в зависимости от размеров поля будут инициализировано определенное количество тыкв
#define START_LENGTH_DRONE 0																//длинна дрона в начале игры ставим 0, так как тележка будет появляться после сборы тыквы
#define LENGTHCART 5																		//максимальное количесво тележек у дрона
#define HELPDRON 4																			//количесвто дронов помошников
#define SPASE ' '																			//символ отрисовки игрового поля
#define BORDER '#'																			//символ границы и склада
#define DRON '+'																			//символ отображения дрона
#define CART '%'																			//символ отрисовки тележки
#define RIPENING 'O'																		//тыква созревающая
#define RIPEN '@'																			//тыква созревшая
#define COLLECTED '~'																		//тыква собранная или испорченая

#define CRASH (*h)++;return																	//макрос выхода пристолкновении


enum {INDOOR_LEFT = 4, INDOOR_RIGHT = 4, INDOOR_UP = 3, INDOOR_DOWN = 6,
	  INDOOR_BW_X = 2, INDOOR_BW_Y = 2};													//Отступы от краев экрана для расположения рядов с тыквами, а так же шаг посадки тыкв по оси Х и У
enum {PUMPKIN_STATUS_RIPENING, PUMPKIN_STATUS_RIPEN, 										//Статус тыквы: растет, выросла, собрана, а так же тыква не задействована в игре из-за размера поля
	  PUMPKIN_STATUS_COLLECTED,PUMPKIN_STATUS_NOTUSE };
enum {LEFT=1, RIGHT, UP, DOWN, CONTROLS=3};													//направление движения
enum {KEY_PAUSE ='P', KEY_SPACE = ' ', KEY_PLUS = '+',										//кнопки управления, и спец кнопки игры кнопка Р большая пауза
	  KEY_LEFT=1075,KEY_RIGHT=1077,KEY_UP=1072,KEY_DOWN=1080};								//кнопка + выпуск дрона помошника, кнопка пробел перевод дрона в автоматический режим
enum {MANUAL, AUTO};																		//режим управления дроном
enum {ON, OFF};																				//статус дронов помощников включен или выключен

typedef struct Control_Battons{
	int left;
	int right;
	int up;
	int down;
}Control_Battons;

struct Control_Battons def_control[CONTROLS] = {{KEY_LEFT, KEY_RIGHT, KEY_UP,KEY_DOWN},		//кнопки управления
												{'A','D','W','S'},
												{'a','d','w','s'}};

typedef struct food_t{																		//Структура тыквы
	int x;
	int y;
	int status;																				//статус созревания и присутсвие на поле
	int employment;
} food_t;


typedef struct cart_t{																		//Структура тележки дрона
	int x;
	int y;
	} cart_t;
	
	
typedef struct dron_t{																		//структура дрона игрока
	int x;
	int y;
	int Direction;
	struct cart_t * cart;
	size_t tsize;
	struct Control_Battons* control;
	int type_control;
	int point;
	}  dron_t;
	
	
typedef struct Autodron_t{																	//структура дрона помошника
	int x;
	int y;
	int Direction;
	struct cart_t * cart;
	size_t tsize;
	int status;
	}  Autodron_t;


typedef struct warehouse_t{																	//Структура тележки дрона
	int x;
	int y;
	} warehouse_t;


int init_Pumpkin (food_t* pumpkin)															//инициализация тыквы
{
	int harvest = 0;																			//количество тыкв которое будет посажено
	int count_PUMPKIN = 0;																	//переменная индекса массива тыкв
	for (int j = INDOOR_UP; j<(MAX_Y-INDOOR_DOWN); j += INDOOR_BW_Y)						//Проходим построчно с заданными отступами и расставляем тыквы по координатной сетке
	{
		for (int i = INDOOR_LEFT ; i<(MAX_X-INDOOR_RIGHT);i+=INDOOR_BW_X)
		{
			if (count_PUMPKIN >= FOODQUANTITY)												//Так как количество еды ограничено, проверяем чтобы количество еды не вышло за индекс масссива 
				{																			//
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
	for (int i = count_PUMPKIN; i<FOODQUANTITY; i++)										//проходим оставшиеся массив тыкв и инициализируеми координатами вне игрового поля и статусом что тыква не участвует в игре.
	{																						//возможно стоило бы использовать список чтобы не было "лишних" тыкв, 
		pumpkin[i].x = -100;																//однако список мне показался менее удобный в плане обращения к конретной тыкве внутри программы или их перебору.
		pumpkin[i].y = -100;																//
		pumpkin[i].status = PUMPKIN_STATUS_NOTUSE;
	}
	
	return harvest;
}


dron_t init_dron(warehouse_t house, int direct, int tupe_control)							//инициация дрона с ручным управлением
{
	dron_t dron;
	dron.x = house.x;
	dron.y = house.y;
	dron.tsize = START_LENGTH_DRONE;														//по умолчанию дрон будет вообще без тележек в начале игры
	dron.Direction = direct;
	dron.cart = (cart_t *) malloc (sizeof(cart_t) * LENGTHCART);
	dron.control = def_control;
	dron.type_control = tupe_control;														//тип управления дроном для возможности переключения ручной/автоматический
	dron.point = 0;
	return dron;
}


void init_Autodron(Autodron_t* autodron, warehouse_t house)									//инициация дронов помошников
{

	for (int i = 0; i < HELPDRON; i++)
	{
		autodron[i].Direction = RIGHT;														//инициализируем всех дронов в складе
		autodron[i].cart = (cart_t *) malloc (sizeof(cart_t) * LENGTHCART);
		autodron[i].tsize = 0;
		autodron[i].status = OFF;															// все дроны будут выключены, и вызываеться пользователем по очереди
		autodron[i].x = house.x;															// присвоим координаты склада
		autodron[i].y = house.y;
	}
}


void printField(food_t* pumpkin, warehouse_t house, dron_t* dron, Autodron_t* autodron,
						char matrix[MAX_X][MAX_Y], int harvest, int harvested, int health)	//Печать кадра
{
																							//заполняем игровое поле
	for (int i = 0; i < MAX_X; ++i)
	{
		for (int j = 0; j < MAX_Y; ++j)
		{
			if ( i == (MAX_X-1) || j ==(MAX_Y-1) || i == 0 || j == 0)						//обозначим визуально границы игрового поля
			{
				matrix[i][j] = BORDER;
			}
			else
			{
				matrix[i][j] = SPASE;
			}
		}
	}
	
	
	matrix[house.x][house.y] = BORDER;														//обозначим склад(точку появления дронов и отгрузки тыквы)
	
	
	for (int i = 0; i<FOODQUANTITY; i++)													//Проходим по массиву с тыквами и заполняем отображения согласно их статусу
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
	
	
	matrix[dron->x][dron->y] = DRON;														//заполнение отображения дрона игрока
	for (int i = 0; i < dron->tsize; ++i)
	{
		matrix[dron->cart[i].x][dron->cart[i].y] = CART;
	}
	
	for (int j = 0; j < HELPDRON; j++)														//заполнение отображения дронов помошников
	{
		if (autodron[j].status == ON)														//если дрон включен быдем его отрисовывать
		{
			matrix[autodron[j].x][autodron[j].y] = DRON;
			for (int i = 0; i < autodron[j].tsize; ++i)
			{
				matrix[autodron[j].cart[i].x][autodron[j].cart[i].y] = CART;				// не забыв и про тележки
			}
		}
	}
	
	
	for (int j = 0; j < MAX_Y; ++j)															//печать всей матрицы изображения
	{
		for (int i = 0; i < MAX_X; ++i)
		{
			if (matrix[i][j] == BORDER)														// расскрашиваем тыквы дроны и границы в разные цвета
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


int inputUser(int key)																		//Проверка на пользовательский ввод с клавиатуры
{
	if(kbhit())																				// проверяем есть ли в буфере введеные данные с клавиатуры
	{
		key = getch();																		// Считываем код нажатой кнопки
		if (key == 224)																		// по скольку клавиши стрелок кодируются 2мя байтами(например стрелка влево это 224 75)
		{																					// проверяем не является ли первый код управляющим и отбрасываем его считывая второй код из буфера
			key = getch()+1000;																// прибавляем 1000 чтобы ключ стрелок наверняка не совпал ни с одной кнопкой клавиатуры
		}
		return key;
    }
    return 0;
}


int checkDirection(dron_t* dron, int key)													//проверка соответствия направления движения, дрон не может моментально поменять направление движения в противоположную сторону даже без тележек
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


void changeDirection(dron_t* dron, int key)													//изменение направления движения ручного управления
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


int Execution_of_special_keys (int key, dron_t* dron, Autodron_t* Autodron)					//функция обработки символов спец ввода, пауза, перевод режима управления дроном и выпуск дронов помошников
{
	while(key == KEY_PAUSE)																	//включение отключения паузы, если была нажата кнопка паузы
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
			return 0;																		//возвращаем 0, так как пауза спец символ и не повлияет на управление
		}
	}
	
	if (key == KEY_SPACE)																	//если была нажата кнопка пробел переводим режим управления дроном
	{
		if (dron->type_control == MANUAL)													// в ручной режим или автоматический в зависимости от текущего статуса
		{
			dron->type_control = AUTO;
		}
		else
		{
			dron->type_control = MANUAL;
		}
		return 0;																			//так же был использован спец символ
	}
	
	if (key == KEY_PLUS)
	{
		for (int i = 0 ; i < HELPDRON; i++)													// Проходимся по массиву дронов
		{
			if (Autodron[i].status == OFF)													// доходим до первого выключеного дрона
			{
				Autodron[i].status = ON;													//включаем его и выходим из цикла
				break;
			}
		}
		return 0;																			//так же был использован спец символ
	}
	return 1;																				//спец сиволы не использованы возвращаем 1;
}


void moveUser(dron_t* dron)																	// фунция движения для дрона игрока
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


void moveAuto(Autodron_t* autodron, int k)													// фунция движения для автодронов
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

void checkEvent( dron_t* dron,Autodron_t* autodron, 
				food_t* pumpkin, warehouse_t house, int* score, int* h)						// проверка событий
{																							// блок сбора тыквы
	for (int i = 0; i < FOODQUANTITY; i++)
	{
		if (pumpkin[i].status == PUMPKIN_STATUS_COLLECTED)									//проверяем собиралась ли еда
		{
			continue;
		}
		if ((dron->x == pumpkin[i].x)&&(dron->y == pumpkin[i].y))
		{
			if ((pumpkin[i].status == PUMPKIN_STATUS_RIPENING)&&(dron->tsize != 0))			// если дрон пролетел над незрелой тыквой, то тележка позади может раздавить тыкву
			{
				pumpkin[i].status = PUMPKIN_STATUS_COLLECTED;								// и тыква защитана не будет, тележка не добавляется, тыква считается раздавленной
			}
			else if (pumpkin[i].status == PUMPKIN_STATUS_RIPEN)								// если тыква была созревшей, создаем тележку с ней
			{
				if (dron->tsize < LENGTHCART)												// если возможно собираем в тележку
				{
					dron->tsize++;
				}
				pumpkin[i].status = PUMPKIN_STATUS_COLLECTED;
			}
			continue;																		//если дрон игрока над тыквой остальные дроны проверять нет смысла, так как если они в одной и тойже точке то они столкнулись
		}
		for (int j = 0; j<HELPDRON; j++)													//теперь проверяем дронов помошников, принцип такой же
		{
			if (autodron[j].status == OFF)													//если дрон выключен следующий тоже,поэтому проверять их нет смысла
			{
				break;
			}
			if ((autodron[j].x == pumpkin[i].x)&&(autodron[j].y == pumpkin[i].y))			//
			{
				if ((pumpkin[i].status == PUMPKIN_STATUS_RIPENING)&&(autodron[j].tsize != 0))
				{
					pumpkin[i].status = PUMPKIN_STATUS_COLLECTED;
				}
				else if (pumpkin[i].status == PUMPKIN_STATUS_RIPEN)
				{
					if (autodron[j].tsize <LENGTHCART)										// если возможно собираем в тележку
					{
						autodron[j].tsize++;
					}
					pumpkin[i].status = PUMPKIN_STATUS_COLLECTED;
				}
				break;																		//если автодрон над тыквой, другой автодрон нет смысла проверять, если и другой дрон тоже над этой тыквой они столкнулись, об этом следующая проверка
			}
		}
	}
	
	if ((house.x == dron->x)&&(house.y == dron->y))											//отгружаем на склад продукцию
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
																							// !!! блок столковений, дроны в полете могут в последний миг сманеврировать и не врезаться
																							// однако тележки при врезании друг в друга опракидываюся ломаются и тыквы в них портятся
																							// поэтому тележки будут удаляться из игры а так как тыквы не были доставлены на склад они не будут засчитаны
																							// так же если дрон без тележек он может пролететь над чужими тележками без вреда
	
	for (int i = 0; i < dron->tsize; i++)													//проверяем чтобы дрон не столкнулся со своими тележками
	{
		if ((dron->x == dron->cart[i].x)&&(dron->y == dron->cart[i].y))
		{
			dron->tsize = 0;																// обнуляем тележки за столкновение
			CRASH;
		}
	}
	
	if (dron->tsize != 0)																	//если у дрона есть тележки
	{
		for (int i = 0; i < HELPDRON; i++)													//проверяем столкновение дрона с чужими тележками
		{
			if (autodron[i].status == OFF)													//если дрон выключен, то он в гараже, и нет смысла его рассматривать
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
	
	for (int i = 0; i < HELPDRON; i++)														// проверяем каждый дрон на момент столкновения
	{
		if (autodron[i].status == OFF)														// есди он выключен, проверять его и последующие нет смысла так как они вступают в игру последовательно
		{
			break;
		}
		if (autodron[i].tsize == 0)
		{
			continue;																		// если автодрон без тележек он может пролететь над тележками без вреда
		}
		for(int j = 0 ; j < dron->tsize; j++)												// проверка на столкновения с тележками дрона игрока
		{
			if ((autodron[i].x == dron->cart[j].x)&&
				(autodron[i].y == dron->cart[j].y))
			{
				dron->tsize = 0;						
				autodron[i].tsize = 0;
				CRASH;
			}
		}
		
		for (int k = 0; k < HELPDRON; k++)													// проверим дрон на столкновения с другими дронами и их тележками
		{
			if (autodron[k].status == OFF)
			{
				break;
			}
			if (autodron[k].tsize == 0)
			{
				continue;																	// если автодрон без тележек он может пролететь над тележками без вреда
			}
			for(int j = 0 ; j < autodron[k].tsize; j++)										// проверяем столкнулся ли дрон с какойнибудь тележкой из автодронов в том числе своими тележками
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


void updateFood (food_t* pumpkin)															// созревание тыквы
{

	for (int i = 0; i< FOODQUANTITY; i++)													// проходм по массиву  тыкв и с определенной вероятностью делаем их спелыми
	{
		pumpkin[i].employment = OFF;														// сбросим нацеливание на все тыквы для автодронов
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


int check_correct_direction (Autodron_t* AutoDron,
						int numberDron, int choiseDirection,char matrix[MAX_X][MAX_Y])		// проверка выбора движения на предмет столкновений
{
	if (choiseDirection == DOWN)															// если мы хотим повернуть вниз и там есть угроза столкновения, мы не меняем направление
	{
		if ((matrix[AutoDron[numberDron].x][AutoDron[numberDron].y+1] != DRON)&&			// если поле куда мы хотим переместиться разрешенное поле, например оно пусто или созревшая тыква или собранная тыква
			(matrix[AutoDron[numberDron].x][AutoDron[numberDron].y+1] != CART)&&			// иначе мы не поворачиваем
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


int check_directionAI (dron_t* Dron, int choiseDirection,char matrix[MAX_X][MAX_Y])			// проверка выбора движения на предмет столкновений
{
	if (choiseDirection == DOWN)															// если мы хотим повернуть вниз и там есть угроза столкновения, мы не меняем направление
	{
		if ((matrix[Dron->x][Dron->y+1] != DRON)&&											// если поле куда мы хотим переместиться разрешенное поле, например оно пусто или созревшая тыква или собранная тыква
			(matrix[Dron->x][Dron->y+1] != CART)&&											// иначе мы не поворачиваем
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

int checkMinus (int a)																		//функция возвращающая модуль числа
{
	return (a<0 ? (-1*a) : a);
}

int DestinatoinX (int a, int b)																// определяем кротчайший путь по координате где А координата своя, и вторая цели
{																							// возможно 2 случая,когда а-b короткий путь напрямую или через границу экрана.
	if (a>b)																				// если А правее то путь через правую краницу выглядит так (MAX_X-2) - (a - b) и напрямую как a-b
	{																						// необходимо сравнить какой путь короче и выдать правильный знак дистанции 
		return ((a-b)>(MAX_X-2-a+b)) ? (MAX_X-2-a+b) : (b-a);								// здесь минус будет значить что координату нужно уменьшить, а + увеличить
	}
	else
	{
		return ((b-a)>(MAX_X-2-b+a)) ? ((MAX_X-2-b+a)*(-1)) : (b-a);						//
	}
}

int DestinatoinY (int a, int b)																// Тоже самое для Y
{																							// 
	if (a>b)																				// 
	{																						// 
		return ((a-b)>(MAX_Y-2-a+b)) ? (MAX_Y-2-a+b) : (b-a);								// 
	}
	else
	{
		return ((b-a)>(MAX_Y-2-b+a)) ? ((MAX_Y-2-b+a)*(-1)) : (b-a);						//
	}
}

void Alcontrol(dron_t* dron,food_t* pumpkin,warehouse_t house, 
													char matrix[MAX_X][MAX_Y], int harvest)	// Автоматическое управление дроном
{
	int count_pupkin_collected = 0;															// проверим все ли тыквы созрели, или были перееханы))
	for (int i = 0; i< FOODQUANTITY; i++)
	{
		if (pumpkin[i].status == PUMPKIN_STATUS_COLLECTED)
		{
			count_pupkin_collected++;
		}
	}
																							// !!!!Выбираем цель ввиде тыквы
	int target = -1;																		// номер тыквы являющейся целью, зададим -1 так как может не быть созревших тыкв
	if ((dron->tsize < LENGTHCART-1)&&(count_pupkin_collected != harvest))					// если тележек не максимальное количество собираем тыкву и возможные к сбору тыквы не закончились  
	{
		for (int i = 0; i< FOODQUANTITY;i++)												// ищем ближайшую тыкву
		{
			
			if ((pumpkin[i].status != PUMPKIN_STATUS_RIPEN)||(pumpkin[i].employment == ON)) //
			{
				continue;																	// пропускаем все незрелые и исключаем нацеливание на одну и ту же тыкву
			}																				// на случай если в дальнейшем будет добавлен еще один пользовательский дрон с режимом автоматического управления
			if (target == -1)
			{	
				target = i; 																// первую же тыкву созревшую будем считать целью
				pumpkin[i].employment = ON;
			}
			else if (((checkMinus(DestinatoinX(dron->x,pumpkin[target].x))) +				// если  дельта x+y выбранной цели > чем х+у текущей тыквы  
					  (checkMinus(DestinatoinY(dron->y,pumpkin[target].y)))) >				//
					 ((checkMinus(DestinatoinX(dron->x,pumpkin[i].x))) +
					  (checkMinus(DestinatoinY(dron->y,pumpkin[i].y)))))
			{
				target = i;																	// выбираем новую цель
				pumpkin[i].employment = ON;
			} 
		}
		
																							// !!!! Меняем направление движения
		if ((dron->Direction == LEFT)||(dron->Direction == RIGHT))
		{
			if (target == -1)
			{
					if (check_directionAI(dron,dron->Direction,matrix))						// если нет цели, проверяем на столкновение текущее положение по оси X
					{																		// если направление не подтверждено пробуем поменять
						if (check_directionAI(dron,DOWN,matrix))			 				// если положение вниз приемлемо функция вернет 0
						{
							check_directionAI(dron,UP,matrix);				 				// и если вниз не сработает пробуем поменять наверх, последний вариант
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
				else if (check_directionAI(dron,dron->Direction,matrix))					// если мы находимся на линии еды, проверим нет ли препятсвий приближаясь к ней
				{																			 // 
					if (check_directionAI(dron,DOWN,matrix))			 					// 
					{
						check_directionAI(dron,UP,matrix);					 				// 
					}
				}
			}
		}
		else
		{
			
			if (target == -1)
			{
					if (check_directionAI(dron,dron->Direction,matrix))						// тоже самое только теперь по оси Y
					{																		 // 
						if (check_directionAI(dron,LEFT,matrix))				 			// 
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
				else if (check_directionAI(dron,dron->Direction,matrix))					// если мы находимся на линии еды, проверим нет ли препятсвий приближаясь к ней 
				{																			// 
					if (check_directionAI(dron,LEFT,matrix))								// 
					{
						check_directionAI(dron,RIGHT,matrix);			
					}
				}
			}
		}
		
	}
	else																					// иначе на склад
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
			else if (check_directionAI(dron,dron->Direction,matrix))						// если мы находимся на линии склада, проверим нет ли препятсвий приближаясь к ней
			{																	 			// 
				if (check_directionAI(dron,DOWN,matrix))			 						// 
				{
					check_directionAI(dron,UP,matrix);				 						// 
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
			else if (check_directionAI(dron,dron->Direction,matrix))						// если мы находимся на линии склада, проверим нет ли препятсвий приближаясь к ней
			{																				// 
				if (check_directionAI(dron,LEFT,matrix))			 						// 
				{
					check_directionAI(dron,RIGHT,matrix);			 						// 
				}
			}
		}
	}
}

void AlchangeDirection(Autodron_t* AutoDron,food_t* pumpkin,warehouse_t house,
													char matrix[MAX_X][MAX_Y], int harvest)	// выбор маршрута для автоматических дронов
{
	int count_pupkin_collected = 0;															// проверим все ли тыквы созрели, или были перееханы))
	for (int i = 0; i< FOODQUANTITY; i++)
	{
		if (pumpkin[i].status == PUMPKIN_STATUS_COLLECTED)
		{
			count_pupkin_collected++;
		}
	}
	
	for (int k = 0; k<HELPDRON;k++)															// !!!!!!Выбираем автодрон
	{

		if (AutoDron[k].status == OFF)														// если отвыключен пропускаем
		{
			break;
		}
																							// !!!!Выбираем цель ввиде тыквы
		int target = -1;																	// номер тыквы являющейся целью, зададим -1 так как может не быть созревших тыкв
		if ((AutoDron[k].tsize < LENGTHCART-1)&&(count_pupkin_collected != harvest))		// если тележек не максимальное количество собираем тыкву и возможные к сбору тыквы не закончились    
		{
			for (int i = 0; i< FOODQUANTITY;i++)											// ищем ближайшую тыкву
			{
				
				if ((pumpkin[i].status != PUMPKIN_STATUS_RIPEN)||(pumpkin[i].employment == ON)) //
				{
					continue;																// пропускаем все незрелые и исключаем нацеливание автодронов на одну и ту же тыкву
				}
				if (target == -1)
				{
					target = i; 															// первую же тыкву созревшую будем считать целью
					pumpkin[i].employment = ON;
				}
				else if (((checkMinus(DestinatoinX(AutoDron[k].x,pumpkin[target].x))) +		// если  дельта x+y выбранной цели > чем х+у текущей тыквы  
						  (checkMinus(DestinatoinY(AutoDron[k].y,pumpkin[target].y)))) >	//
						 ((checkMinus(DestinatoinX(AutoDron[k].x,pumpkin[i].x))) +
						  (checkMinus(DestinatoinY(AutoDron[k].y,pumpkin[i].y)))))
				{
					target = i;																// выбираем новую цель
					pumpkin[i].employment = ON;
				} 
			}
			
																							// !!!! Меняем направление движения
			if ((AutoDron[k].Direction == LEFT)||(AutoDron[k].Direction == RIGHT))
			{
				if (target == -1)
				{
					if ((matrix[AutoDron[k].x][AutoDron[k].y+1] = DRON)||					// чтобы дроны не ездили параллельно друг другу пробуем сменить направление движения
						(matrix[AutoDron[k].x][AutoDron[k].y+1] = CART))
						{
							check_correct_direction(AutoDron,k,UP,matrix);
						}
					if ((matrix[AutoDron[k].x][AutoDron[k].y-1] = DRON)||
						(matrix[AutoDron[k].x][AutoDron[k].y-1] = CART))
						{
							check_correct_direction(AutoDron,k,DOWN,matrix);
						}
					if (check_correct_direction(AutoDron,k,AutoDron[k].Direction,matrix))// если нет цели, проверяем на столкновение текущее положение по оси X
					{																	 // если направление не подтверждено пробуем поменять
						if (check_correct_direction(AutoDron,k,DOWN,matrix))			 // если положение вниз приемлемо функция вернет 0
						{
							check_correct_direction(AutoDron,k,UP,matrix);				 // и если вниз не сработает пробуем поменять наверх, последний вариант
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
					else if (check_correct_direction(AutoDron,k,AutoDron[k].Direction,matrix))// если мы находимся на линии еды, проверим нет ли препятсвий приближаясь к ней
					{																		 // 
						if (check_correct_direction(AutoDron,k,DOWN,matrix))			 	// 
						{
							check_correct_direction(AutoDron,k,UP,matrix);					 // 
						}
					}
				}
			}
			else
			{
				
				if (target == -1)
				{
					if ((matrix[AutoDron[k].x+1][AutoDron[k].y] = DRON)||					// чтобы дроны не ездили параллельно друг другу пробуем сменить направление движения
						(matrix[AutoDron[k].x+1][AutoDron[k].y] = CART))
						{
							check_correct_direction(AutoDron,k,LEFT,matrix);
						}
					if ((matrix[AutoDron[k].x-1][AutoDron[k].y] = DRON)||
						(matrix[AutoDron[k].x-1][AutoDron[k].y] = CART))
						{
							check_correct_direction(AutoDron,k,RIGHT,matrix);
						}
					if (check_correct_direction(AutoDron,k,AutoDron[k].Direction,matrix))// тоже самое только теперь по оси Y
					{																	 // 
						if (check_correct_direction(AutoDron,k,LEFT,matrix))				 // 
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
					else if (check_correct_direction(AutoDron,k,AutoDron[k].Direction,matrix))// если мы находимся на линии еды, проверим нет ли препятсвий приближаясь к ней 
					{																	 		// 
						if (check_correct_direction(AutoDron,k,LEFT,matrix))					// 
						{
							check_correct_direction(AutoDron,k,RIGHT,matrix);			
						}
					}
				}
			}
			
		}
		else 																				// иначе едем на склад
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
				else if (check_correct_direction(AutoDron,k,AutoDron[k].Direction,matrix))// если мы находимся на линии склада, проверим нет ли препятсвий приближаясь к ней
				{																	 		// 
					if (check_correct_direction(AutoDron,k,DOWN,matrix))			 		// 
					{
						check_correct_direction(AutoDron,k,UP,matrix);				 		// 
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
				else if (check_correct_direction(AutoDron,k,AutoDron[k].Direction,matrix))// если мы находимся на линии склада, проверим нет ли препятсвий приближаясь к ней
				{																		 // 
					if (check_correct_direction(AutoDron,k,LEFT,matrix))			 	// 
					{
						check_correct_direction(AutoDron,k,RIGHT,matrix);			 	// 
					}
				}
			}
		}
		
		moveAuto(AutoDron,k);															// двигаем сразу автодрона, чтобы его можно было сразу отрисовать
		matrix[AutoDron[k].x][AutoDron[k].y] = DRON;									// предварительное заполнение матрицы позволит исключить перемещение дронов в одну и ту же клетку
		for (int i = 0; i < AutoDron[k].tsize; ++i)
		{
			matrix[AutoDron[k].cart[i].x][AutoDron[k].cart[i].y] = CART;
		}
	}
}


int CheckEnd(int harvest,int harvested,food_t* pumpkin,dron_t* dron,Autodron_t* AutoDron)
{
	if (harvest == harvested)																//если собран весь возможный урожай заканчиваем игру
	{
		return 0;
	}
	int count_pupkin_collected = 0;															// проверим все ли тыквы созрели, или были перееханы))
	for (int i = 0; i< FOODQUANTITY; i++)
	{
		if (pumpkin[i].status == PUMPKIN_STATUS_COLLECTED)
		{
			count_pupkin_collected++;
		}
	}
	if ((count_pupkin_collected == harvest)&&(dron->tsize == 0))							// если зреющих тыкв нет и тележка дрона пуста
	{
		for (int i = 0; i < HELPDRON; i++)													// и тележки автодронов пусты
		{
			if (AutoDron[i].status == OFF)													// если дрон выключен выходим 
			{
				break;
			}
			if (AutoDron[i].tsize != 0)														// если хотябы один автодрон с тележкой ждем пока он ее отвезет
			{
				return 1;
			}
		}
		return 0;																			// если в цикле дронов не было автодронов  или все дроны без тележек конец игры
		
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
	
	srand(time(NULL));																		//функция рандома по текущему времени
	
	warehouse_t house = {MAX_X/2, MAX_Y - 3}; 												//инициализируем точку появления дронов(он же и есть склад)
	food_t pumpkin[FOODQUANTITY];
	
	int harvest = init_Pumpkin(pumpkin);													//Счетчик созданных тыкв которые можно будет собрать
	
	dron_t dron = init_dron(house,LEFT,MANUAL);
	
	Autodron_t AutoDron[HELPDRON];
	
	init_Autodron(AutoDron, house);
	
	int key = 0;																			//переменная нажатой клавиши клавиатуры
	int harvested = 0;																		//количество собранных тыкв очков
	int health = 0;																			//переменная столкновения
	
	
	char matrix[MAX_X][MAX_Y];																//инициализирум матрицу отображния, чтобы ее можно было передавать в в другие функции
	printField(pumpkin,house,&dron,AutoDron,matrix,harvest,harvested,health);				//это очень поможет при обработке корректности выбора автодронов на предмет столкновений чтобы не приходилось перебирать огромные массивы данных
	system("cls");																			//конечно способ передачи массива в функцию меня настараживает, но в процессе тестирования ошибок не было
	
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
