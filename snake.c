/*  
 *  SNAKE GAME 
 *  <This Game is a simple console application without graphics which will run within the terminal window.>
 *  Copyright (C) <2015>  <Amoli Prashant Kotalwar>
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "snake.h"


#define UP_ARROW (char)'A' 
#define LEFT_ARROW (char)'D'
#define RIGHT_ARROW (char)'C'
#define DOWN_ARROW (char)'B'

#define ENTER_KEY 10
	
const char SNAKE_HEAD = 'X';
const char SNAKE_BODY = '#';
const char WALL = '#';	
const char FOOD = '*';
const char BLANK = ' ';

#define EXIT_BUTTON 27 //ESC
#define PAUSE_BUTTON 112 //P
	


void loadEnviroment(int consoleWidth, int consoleHeight) {
	int i;
	int x = 1, y = 1;
	int rectangleHeight = consoleHeight - 4;
	clrscr(); /*clear the console*/
	
	gotoxy(x,y); /*Top left corner*/
	
	for (; y < rectangleHeight; y++)
	{
		gotoxy(x, y);  
		printf("%c",WALL);
		
		gotoxy(consoleWidth, y); 
		printf("%c",WALL);
	}
	
	y = 1;
	for (; x < consoleWidth + 1; x++)
	{
		gotoxy(x, y);  
		printf("%c",WALL);
		
		gotoxy(x, rectangleHeight); 
		printf("%c",WALL);
	}
	
	return;
}
void loadSnake(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength) {
	int i;
	for (i = 0; i < snakeLength; i++)
	{
		gotoxy(snakeXY[0][i], snakeXY[1][i]);
		printf("%c", SNAKE_BODY); 
	}
	
	return;
}
void prepairSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength) {
	int i, x;
	int snakeX = snakeXY[0][0];
	int snakeY = snakeXY[1][0];
	for(i = 1; i <= snakeLength; i++)
	{
		snakeXY[0][i] = snakeX + i;
		snakeXY[1][i] = snakeY;
	}
	
	return;
}
void loadGame(void) {
	int snakeXY[2][SNAKE_ARRAY_SIZE];   /*two dimensional array*/
	int snakeLength = 4;		    /*initial lenght*/
	int direction = LEFT_ARROW;	    /*initial direction*/
	int foodXY[] = {5,5};
	int score = 0;
	int consoleWidth = 80;
	int consoleHeight = 25;
	int speed = getGameSpeed();
	snakeXY[0][0] = 40;     /*starting position of the snake*/
	snakeXY[1][0] = 10;
	
	loadEnviroment(consoleWidth, consoleHeight); /*borders*/
	prepairSnakeArray(snakeXY, snakeLength);
	loadSnake(snakeXY, snakeLength);
	generateFood( foodXY, consoleWidth, consoleHeight, snakeXY, snakeLength);
	refreshInfoBar(score, speed); /*Bottom info bar. Score, Level etc*/
	startGame(snakeXY, foodXY, consoleWidth, consoleHeight, snakeLength, direction, score, speed);

	return;
}

char waitForAnyKey(void)
{
	int pressed;
	
	while(!kbhit());
	
	pressed = getch();
	
	return((char)pressed);
}


int getGameSpeed(void){
	int speed;
	clrscr();
	
	do
	{
		gotoxy(10,5);
		printf("Select The game speed between 1 and 9.");
		speed = waitForAnyKey()-48;
	} while(speed < 1 || speed > 9);
	return(speed);
}

void pauseMenu(void)
{
	int i;
	
	gotoxy(28,23);
	printf("**Paused**");
	
	waitForAnyKey();
	gotoxy(28,23);
	printf("            ");

	return;
}

int checkKeysPressed(int direction)
{
	int pressed;
	
	if(kbhit()) /*If a key has been pressed*/
	{
		pressed=getch();
		if (direction != pressed)
		{	
			
			if(pressed == DOWN_ARROW && direction != UP_ARROW)
				direction = pressed;
			else if (pressed == UP_ARROW && direction != DOWN_ARROW)
				direction = pressed;
			else if (pressed == LEFT_ARROW && direction != RIGHT_ARROW)
				direction = pressed;
			else if (pressed == RIGHT_ARROW && direction != LEFT_ARROW)
				direction = pressed;
			else if (pressed == EXIT_BUTTON || pressed == PAUSE_BUTTON)
				pauseMenu();
		}
	}
	return(direction);
}

int collisionSnake (int x, int y, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int detect) {
	int i;
	for (i = detect; i < snakeLength; i++) /*Checks if the snake collided with itself*/
	{
		if ( x == snakeXY[0][i] && y == snakeXY[1][i])
			return(1);
	}
	return(0);
}

void moveSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction) {
	int i;
	for( i = snakeLength-1; i >= 1; i-- )
	{
		snakeXY[0][i] = snakeXY[0][i-1];
		snakeXY[1][i] = snakeXY[1][i-1];
	}	
	
	/*
	because we dont actually know the new snakes head x y, 
	we have to check the direction and add or take from it depending on the direction.
	*/
	switch(direction)
	{
		case DOWN_ARROW:
			snakeXY[1][0]++;
			break;
		case RIGHT_ARROW:
			snakeXY[0][0]++;
			break;
		case UP_ARROW:
			snakeXY[1][0]--;
			break;
		case LEFT_ARROW:
			snakeXY[0][0]--;
			break;			
	}
	
	return;
}

void move(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction)
{
	int x;
	int y;

	/*Remove the tail */
	x = snakeXY[0][snakeLength-1];
	y = snakeXY[1][snakeLength-1];
	
	gotoxy(x,y);
	printf("%c",BLANK);	
	
	/*Changes the head of the snake to a body part*/
	gotoxy(snakeXY[0][0],snakeXY[1][0]);	
	printf("%c", SNAKE_BODY);
	
	moveSnakeArray(snakeXY, snakeLength, direction);
	
	gotoxy(snakeXY[0][0],snakeXY[1][0]);	
	printf("%c",SNAKE_HEAD);
	
	gotoxy(1,1); /*Gets rid of the darn flashing underscore.*/
	
	return;
}

int collisionDetection(int snakeXY[][SNAKE_ARRAY_SIZE], int consoleWidth, int consoleHeight, int snakeLength ) {
	int colision = 0;
	if ((snakeXY[0][0] == 1) || (snakeXY[1][0] == 1) || (snakeXY[0][0] == consoleWidth) || (snakeXY[1][0] == consoleHeight - 4)) 
	/*Checks if the snake collided wit the wall or it's self*/
		colision = 1;
	else
		if (collisionSnake(snakeXY[0][0], snakeXY[1][0], snakeXY, snakeLength, 1)) 
	/*If the snake collided with the wall, theres no point in checking if it collided with itself.*/
			colision = 1;
			
	return(colision);
}

int generateFood(int foodXY[], int width, int height, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
	int i;
	
	do
	{
		srand ( time(NULL) );
		foodXY[0] = rand() % (width - 2) + 2;
		srand ( time(NULL) );
		foodXY[1] = rand() % (height - 6) + 2;
	} while (collisionSnake(foodXY[0], foodXY[1], snakeXY, snakeLength, 0)); /*This should prevent the "Food" from being created on top of the snake. - However the food has a chance to be created ontop of the snake, in which case the snake should eat it...*/

	gotoxy(foodXY[0] ,foodXY[1]);
	printf("%c", FOOD);
	
	return(0);
}

int eatFood(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[]) /*This function checks if the snakes head his on top of the food, if it is then it'll generate some more food...*/
{
	if (snakeXY[0][0] == foodXY[0] && snakeXY[1][0] == foodXY[1])
	{
		foodXY[0] = 0;
		foodXY[1] = 0; 
		
		printf("\7"); /*Beep*/
		return(1);
	}		
	
	return(0);
}

void startGame( int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[], int consoleWidth, int consoleHeight, int snakeLength, int direction, int score, int speed)
{
	int gameOver = 0;
	clock_t endWait;
	
	
	int waitMili = CLOCKS_PER_SEC-(speed)*(CLOCKS_PER_SEC/10);	/*Sets the correct wait time according to the selected speed*/
	int tempScore = 10*speed;
	int oldDirection;
	int canChangeDirection = 1;
	
	endWait = clock() + waitMili;

	do
	{
		if(canChangeDirection)
		{
			oldDirection = direction;
			direction = checkKeysPressed(direction);
		}
		
		if(oldDirection != direction)
			canChangeDirection = 0;
			
		if(clock() >= endWait) /* it moves according to how fast the computer running it is...*/
		{
			
			move(snakeXY, snakeLength, direction);
			canChangeDirection = 1;

				
			if(eatFood(snakeXY, foodXY))
			{
				generateFood( foodXY, consoleWidth, consoleHeight, snakeXY, snakeLength); /*Generate More Food*/
				snakeLength++;
				score+=speed;
				
				if( score >= 10*speed+tempScore)
				
				{
					speed++;
					tempScore = score;

					if(speed <= 9)
						waitMili = waitMili - (CLOCKS_PER_SEC/10);
					else
					{
						if(waitMili >= 40) /*Maximum Speed (the game has to be beatable)*/
							waitMili = waitMili - (CLOCKS_PER_SEC/200);
						
					}
					
				}
				
				refreshInfoBar(score, speed);
			}
			
			endWait = clock() + waitMili; 
		}
		
		gameOver = collisionDetection(snakeXY, consoleWidth, consoleHeight, snakeLength);

		if(snakeLength >= SNAKE_ARRAY_SIZE-5) /*Just to make sure it doesn't get longer then the array size & crash*/
		{
			gameOver = 2;/*You Win!*/ 
			score+=1500; /*When you win you get an extra 1500 points!!!*/
		}
		
	} while (!gameOver);
	
	switch(gameOver)
	{
		case 1:
			printf("\7"); /*Beep*/
			printf("\7"); /*Beep*/

			gameOverScreen();

			break;
		case 2:
			youWinScreen();
			break;
	}
	
	if(score >= getLowestScore() && score != 0)
	{
		inputScore(score);
		displayHighScores();
	}
	
	return;
}

int menuSelector(int x, int y, int yStart)
{
	char key;
	int i = 0;
	x = x - 2;
	gotoxy(x,yStart);
	
	printf(">");
	
	gotoxy(1,1);

	
	do
	{
		key = waitForAnyKey();
		
		if ( key == (char)UP_ARROW )
		{
			gotoxy(x,yStart+i);
			printf(" ");
			
			if (yStart >= yStart+i ) 
				i = y - yStart - 2;
			else
				i--;
			gotoxy(x,yStart+i);
			printf(">");
		}
		else
			if ( key == (char)DOWN_ARROW )
			{
				gotoxy(x,yStart+i);
				printf(" ");
				
				if (i+2 >= y - yStart ) 
					i = 0;
				else
					i++;
				gotoxy(x,yStart+i);
				printf(">");				
			}	
			
	} while(key != (char)ENTER_KEY); 
	return(i);
}

void refreshInfoBar(int score, int speed) {
	gotoxy(5,23);
	printf("Score: %d", score);
	
	gotoxy(5,24);
	printf("Speed: %d", speed);

	gotoxy(52,23);
	printf("Coder: Amoli Kotalwar");

}

//**************HIGHSCORE STUFF**************//



void createHighScores(void)
{
	FILE *file; 
	int i;

	file = fopen("highscores.txt","w+");
	
	if(file == NULL)
	{
		printf("FAILED TO CREATE HIGHSCORES!!! EXITING!");
		exit(0);	
	}
	
	for(i = 0; i < 5; i++)
	{
		fprintf(file,"%d",i+1);
		fprintf(file,"%s","\t0\t\t\tEMPTY\n");
	}	
	
	fclose(file);
	return;
}

int getLowestScore()
{
	FILE *fp;
	char str[128];
	int lowestScore = 0;
	int i;
	int intLength;
	
	if((fp = fopen("highscores.txt", "r")) == NULL)
	{
		/*Create the file, then try open it again.. if it fails this time exit.*/
		createHighScores(); /*This should create a highscores file (If there isn't one)*/
		if((fp = fopen("highscores.txt", "r")) == NULL)
			exit(1);
	}

	while(!feof(fp))
	{
		fgets(str, 126, fp);  
	}
	fclose(fp);	
	
	i=0;
	
	/*Gets the Int length*/
	while(str[2+i] != '\t')
	{
		i++;
	}
	
	intLength = i;
	
	/*Gets converts the string to int*/
	for(i=0;i < intLength; i++)
	{
		lowestScore = lowestScore + ((int)str[2+i] - 48) * pow(10,intLength-i-1);
	}

	return(lowestScore);
}

void inputScore(int score) 
{
	FILE *fp;
	FILE *file; 
	char str[20];
	int fScore;
	int i, s, y;
	int intLength;
	int scores[5];
	int x;
	char highScoreName[20];
	char highScoreNames[5][20];
	
	char name[20];
	
	int entered = 0;
	
	clrscr(); /*clear the console*/
	
	if((fp = fopen("highscores.txt", "r")) == NULL)
	{
		/*Create the file, then try open it again.. if it fails this time exit.*/
		createHighScores(); /*This should create a highscores file (If there isn't one)*/
		if((fp = fopen("highscores.txt", "r")) == NULL)
			exit(1);
	}
	gotoxy(10,5);
	printf("Your Score made it into the top 5!!!");
	gotoxy(10,6);
	printf("Please enter your name: ");
	gets(name);
	
	x = 0;
	while(!feof(fp))
	{
		fgets(str, 126, fp);  /*Gets a line of text*/
		
		i=0;
		
		/*Gets the Int length*/
		while(str[2+i] != '\t')
		{
			i++;
		}
		
		s = i;
		intLength = i;
		i=0;
		while(str[5+s] != '\n')
		{
			
			highScoreName[i] = str[5+s];
			s++;
			i++;
		}
		
		
		fScore = 0;
		/*Gets converts the string to int*/
		for(i=0;i < intLength; i++)
		{
			
			fScore = fScore + ((int)str[2+i] - 48) * pow(10,intLength-i-1);
		}
		
		if(score >= fScore && entered != 1)
		{
			scores[x] = score;
			strcpy(highScoreNames[x], name);
			
			
			x++;
			entered = 1;
		}
		
		
		strcpy(highScoreNames[x], highScoreName);
		scores[x] = fScore;
		
		
		for(y=0;y<20;y++)
		{
			highScoreName[y] = NULL;
		}
		
		x++;
		if(x >= 5)
			break;
	}
	
	fclose(fp);
	
	file = fopen("highscores.txt","w+");
	
	for(i=0;i<5;i++)
	{
		
		fprintf(file, "%d\t%d\t\t\t%s\n", i+1, scores[i], highScoreNames[i]);	
	}

	fclose(file);
	
	return;
}

void displayHighScores(void) 
{
	FILE *fp;
	char str[128];
	int y = 5;
	
	clrscr(); /*clear the console*/
	
	if((fp = fopen("highscores.txt", "r")) == NULL) {
		/*Create the file, then try open it again.. if it fails this time exit.*/
		createHighScores(); /*This should create a highscores file (If there isn't one)*/
		if((fp = fopen("highscores.txt", "r")) == NULL)
			exit(1);
	}
	
	gotoxy(10,y++);
	printf("High Scores");	
	gotoxy(10,y++);
	printf("Rank\tScore\t\t\tName");
	while(!feof(fp)) {
		gotoxy(10,y++);
		if(fgets(str, 126, fp)) 
			printf("%s", str);
	}

	fclose(fp);	/*Close the file*/
	gotoxy(10,y++);
	
	printf("Press any key to continue...");
	waitForAnyKey();	
	return;
}

//**************END HIGHSCORE STUFF**************/

void youWinScreen(void) {
	int x = 6, y = 7;
	gotoxy(x,y++);
	printf("'##:::'##::'#######::'##::::'##::::'##:::::'##:'####:'##::: ##:'####:");
	gotoxy(x,y++);
	printf(". ##:'##::'##.... ##: ##:::: ##:::: ##:'##: ##:. ##:: ###:: ##: ####:");
	gotoxy(x,y++);
	printf(":. ####::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ####: ##: ####:");
	gotoxy(x,y++);
	printf("::. ##:::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ## ## ##:: ##::");
	gotoxy(x,y++);
	printf("::: ##:::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ##. ####::..:::");
	gotoxy(x,y++);
	printf("::: ##:::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ##:. ###:'####:");
	gotoxy(x,y++);
	printf("::: ##::::. #######::. #######:::::. ###. ###::'####: ##::. ##: ####:");
	gotoxy(x,y++);
	printf(":::..::::::.......::::.......:::::::...::...:::....::..::::..::....::");
	gotoxy(x,y++);	
	
	waitForAnyKey();
	clrscr(); /*clear the console*/
	return;
}
void gameOverScreen(void) {
	int x = 17, y = 3;
	
	
	
	gotoxy(x,y++);
	printf(":'######::::::'###::::'##::::'##:'########:\n");
	gotoxy(x,y++);
	printf("'##... ##::::'## ##::: ###::'###: ##.....::\n");
	gotoxy(x,y++);
	printf(" ##:::..::::'##:. ##:: ####'####: ##:::::::\n");
	gotoxy(x,y++);
	printf(" ##::'####:'##:::. ##: ## ### ##: ######:::\n");
	gotoxy(x,y++);
	printf(" ##::: ##:: #########: ##. #: ##: ##...::::\n");
	gotoxy(x,y++);
	printf(" ##::: ##:: ##.... ##: ##:.:: ##: ##:::::::\n");
	gotoxy(x,y++);
	printf(". ######::: ##:::: ##: ##:::: ##: ########:\n");
	gotoxy(x,y++);
	printf(":......::::..:::::..::..:::::..::........::\n");
	gotoxy(x,y++);
	printf(":'#######::'##::::'##:'########:'########::'####:\n");
	gotoxy(x,y++);
	printf("'##.... ##: ##:::: ##: ##.....:: ##.... ##: ####:\n");
	gotoxy(x,y++);
	printf(" ##:::: ##: ##:::: ##: ##::::::: ##:::: ##: ####:\n");
	gotoxy(x,y++);
	printf(" ##:::: ##: ##:::: ##: ######::: ########::: ##::\n");
	gotoxy(x,y++);
	printf(" ##:::: ##:. ##:: ##:: ##...:::: ##.. ##::::..:::\n");
	gotoxy(x,y++);
	printf(" ##:::: ##::. ## ##::: ##::::::: ##::. ##::'####:\n");
	gotoxy(x,y++);
	printf(". #######::::. ###:::: ########: ##:::. ##: ####:\n");
	gotoxy(x,y++);
	printf(":.......::::::...:::::........::..:::::..::....::\n");
		waitForAnyKey();
	clrscr(); /*clear the console*/
	return;
}

void welcomeArt(void) {
	clrscr(); 
	
	printf("\n");	
	printf("\t\t    _________         _________ 			\n");	
	printf("\t\t   /         \\       /         \\ 			\n");	
	printf("\t\t  /  /~~~~~\\  \\     /  /~~~~~\\  \\ 			\n");	
	printf("\t\t  |  |     |  |     |  |     |  | 			\n");		
	printf("\t\t  |  |     |  |     |  |     |  | 			\n");
	printf("\t\t  |  |     |  |     |  |     |  |         /	\n");
	printf("\t\t  |  |     |  |     |  |     |  |       //	\n");
	printf("\t\t (o  o)    \\  \\_____/  /     \\  \\_____/ / 	\n");
	printf("\t\t  \\__/      \\         /       \\        / 	\n");
	printf("\t\t    |        ~~~~~~~~~         ~~~~~~~~ 		\n");
	printf("\t\t    ^											\n");
	printf("\t		Welcome To The Snake Game!			\n");
	printf("\t			    Press Any Key To Continue...	\n");			
	printf("\n");
	
	waitForAnyKey();
	return;
}

void controls(void)
{
	int x = 10, y = 5;
	clrscr(); /*clear the console*/
	gotoxy(x,y++);
	printf("Controls\n");
	gotoxy(x++,y++);
	printf("Use the following arrow keys to direct the snake to the food: ");
	gotoxy(x,y++);
	printf("Right Arrow");
	gotoxy(x,y++);
	printf("Left Arrow");
	gotoxy(x,y++);
	printf("Top Arrow");
	gotoxy(x,y++);
	printf("Bottom Arrow");
	gotoxy(x,y++);
	gotoxy(x,y++);
	printf("P & Esc pauses the game.");
	gotoxy(x,y++);
	gotoxy(x,y++);
	printf("Press any key to continue...");
	waitForAnyKey();
	return;
}

int mainMenu(void)
{
	int x = 10, y = 5;
	int yStart = y;
	
	int selected;
	
	clrscr(); /*clear the console*/
	
	gotoxy(x,y++);
	printf("New Game\n");
	gotoxy(x,y++);
	printf("High Scores\n");
	gotoxy(x,y++);
	printf("Controls\n");
	gotoxy(x,y++);
	printf("Exit\n");
	gotoxy(x,y++);

	selected = menuSelector(x, y, yStart);

	return(selected);
}

void exitYN(void) {
	char pressed;
	gotoxy(9,8);
	printf("Are you sure you want to exit(Y/N)\n");
	do
	{
		pressed = waitForAnyKey();
		pressed = tolower(pressed);
	} while (!(pressed == 'y' || pressed == 'n'));
	if (pressed == 'y')
	{
		clrscr(); /*clear the console*/
		exit(1);
	}
	return;
}

void gotoxy(int x,int y) {
	printf("%c[%d;%df",0x1B,y,x);
}
int kbhit(void) {
	/* It checks whether the key is pressed or not*/
	  struct termios oldt, newt;
	  int ch;
	  int oldf;

	  tcgetattr(STDIN_FILENO, &oldt);
	  newt = oldt;
	  newt.c_lflag &= ~(ICANON | ECHO);
	  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	  ch = getchar();

	  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	  fcntl(STDIN_FILENO, F_SETFL, oldf);

	  if(ch != EOF)
	  {
		ungetc(ch, stdin);
		return 1;
	  }

	  return 0;
}
char getch() {
	char c;
	system("stty raw");
	c= getchar();
	system("stty sane");
	
	return(c);
}
void clrscr() {
	system("clear");
		return;
}








