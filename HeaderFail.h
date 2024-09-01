#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <time.h>
#include <windows.h>

#define MAX_X 40
#define MAX_Y 20
#define FOODQUANTITY 400
#define START_LENGTH_DRONE 0
#define LENGTHCART 5
#define HELPDRON 4
#define SPASE ' '																			
#define BORDER '#'																			
#define DRON '+'																			
#define CART '%'																	
#define RIPENING 'O'																		
#define RIPEN '@'																			
#define COLLECTED '~'																		

#define CRASH (*h)++;return																	


enum {INDOOR_LEFT = 4, INDOOR_RIGHT = 4, INDOOR_UP = 3, INDOOR_DOWN = 6, INDOOR_BW_X = 2, INDOOR_BW_Y = 2};													
enum {PUMPKIN_STATUS_RIPENING, PUMPKIN_STATUS_RIPEN, PUMPKIN_STATUS_COLLECTED,PUMPKIN_STATUS_NOTUSE };
enum {LEFT=1, RIGHT, UP, DOWN, CONTROLS=3};													
enum {KEY_PAUSE ='P', KEY_SPACE = ' ', KEY_PLUS = '+',KEY_LEFT=1075,KEY_RIGHT=1077,KEY_UP=1072,KEY_DOWN=1080};								
enum {MANUAL, AUTO};																		
enum {ON, OFF};																				

typedef struct Control_Battons{
	int left;
	int right;
	int up;
	int down;
}Control_Battons;


typedef struct food_t{																		
	int x;
	int y;
	int status;																				
	int employment;
} food_t;


typedef struct cart_t{																		
	int x;
	int y;
	} cart_t;
	
	
typedef struct dron_t{																		
	int x;
	int y;
	int Direction;
	struct cart_t * cart;
	size_t tsize;
	struct Control_Battons* control;
	int type_control;
	int point;
	}  dron_t;
	
	
typedef struct Autodron_t{																	
	int x;
	int y;
	int Direction;
	struct cart_t * cart;
	size_t tsize;
	int status;
	}  Autodron_t;


typedef struct warehouse_t{																	
	int x;
	int y;
	} warehouse_t;


int init_Pumpkin (food_t* pumpkin);															

dron_t init_dron(warehouse_t house, int direct, int tupe_control);

void init_Autodron(Autodron_t* autodron, warehouse_t house);

void printField(food_t* pumpkin, warehouse_t house, dron_t* dron, Autodron_t* autodron, char matrix[MAX_X][MAX_Y], int harvest, int harvested, int health);

int inputUser(int key);

int checkDirection(dron_t* dron, int key);

void changeDirection(dron_t* dron, int key);

int Execution_of_special_keys (int key, dron_t* dron, Autodron_t* Autodron);

void moveUser(dron_t* dron);

void moveAuto(Autodron_t* autodron, int k);

void checkEvent( dron_t* dron,Autodron_t* autodron, food_t* pumpkin, warehouse_t house, int* score, int* h);

void updateFood (food_t* pumpkin);

int check_correct_direction (Autodron_t* AutoDron, int numberDron, int choiseDirection,char matrix[MAX_X][MAX_Y]);

int check_directionAI (dron_t* Dron, int choiseDirection,char matrix[MAX_X][MAX_Y]);

int checkMinus (int a);

int DestinatoinX (int a, int b);

int DestinatoinY (int a, int b);

void Alcontrol(dron_t* dron,food_t* pumpkin,warehouse_t house, char matrix[MAX_X][MAX_Y], int harvest);

void AlchangeDirection(Autodron_t* AutoDron,food_t* pumpkin,warehouse_t house, char matrix[MAX_X][MAX_Y], int harvest);

int CheckEnd(int harvest,int harvested,food_t* pumpkin,dron_t* dron,Autodron_t* AutoDron);

void endMenu(int harvest,int harvested,int health, dron_t dron);

void StartMenu(void);
