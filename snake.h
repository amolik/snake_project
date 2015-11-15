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




#define SNAKE_ARRAY_SIZE 310



void gotoxy(int x,int y);
int kbhit(void);
char getch();
void clrscr();
char waitForAnyKey(void);
int getGameSpeed(void);
void pauseMenu(void);
int checkKeysPressed(int direction);
int collisionSnake (int x, int y, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int detect);
int generateFood(int foodXY[], int width, int height, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength);
void moveSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction);
void move(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction);
int eatFood(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[]);
int collisionDetection(int snakeXY[][SNAKE_ARRAY_SIZE], int consoleWidth, int consoleHeight, int snakeLength );
void refreshInfoBar(int score, int speed);
void createHighScores(void);
int getLowestScore();
void inputScore(int score);
void displayHighScores(void);
void youWinScreen(void);
void gameOverScreen(void);
void startGame( int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[], int consoleWidth, int consoleHeight, int snakeLength, int direction, int score, int speed);
void loadEnviroment(int consoleWidth, int consoleHeight);
void loadSnake(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength);
void prepairSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength);
void loadGame(void);
int menuSelector(int x, int y, int yStart);
void welcomeArt(void);
void controls(void);
void exitYN(void);
int mainMenu(void);


