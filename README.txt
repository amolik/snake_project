SNAKE GAME
*************

KOTALWAR AMOLI PRASHANT
111408030

DESCRIPTION :

This Snake Game is a simple console application without graphics.
The game will run within the terminal window. 
In this, you can play the popular 'Snake Game' just like you played it elsewhere. 
You have to use the up, down, right or left arrows to move the snake.

Foods are provided at the several co-ordinates of the screen for the snake to eat. 
Every time the snake eats the food, its length will by increased by one unit along with the score.

Here, the snake is an two dimensional array which can move vertically or horizontally i.e. only one co-ordinate changes at a time. The wall is generated surrounding the specified console using the 'gotoxy' function. Also, the food is generated randomly using 'gotoxy' function and  'srand' function. 
You will lose the game if the snake touches  itself or the wall.
That would be detected using collision detection function.
This function is also used in preparing food i.e. it checks whether the food generated is not on the snake.

It also records the player's name and the corresponding score obtained. This is achieved using file handling. It creates a text file called 'highscores.txt' wherein teh top 5 players and their corresponding scores are being stored. If the player of the new game manages to the top 5, then his name would be stored in the file according to the corresponding rank and at the same time, the lowest scorer entry would be deleted. 
You can also view the high scores of the game.

