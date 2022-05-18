#include<iostream>
#include<conio.h>
//#include <conio.h> for 'int getch ();' that takes a character input, and output returns a number
#include<dos.h> 
#include <windows.h>
#include <time.h>
#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70 

#define KEY_LEFT 75 //has to be 75
#define KEY_RIGHT 77 //has to be 77
#define KEY_UP 72 //has to be 72
#define KEY_DOWN 80 //has to be 80
#define KEY_ESCAPE 0x1B //from the link bellow

/* some other keys you can define and work with:
https://graphics.stanford.edu/~mdfisher/Code/Engine/KeyDefs.h.html */

//this is considered bad practice for some reason
using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD coords;
void clrscr() //clears the screen while looking better than system("clr");
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD bufferSize, charsWritten;

	COORD topleft = { 0, 0 };

	GetConsoleScreenBufferInfo(console, &csbi);
	bufferSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(console, TEXT(' '), bufferSize, topleft, &charsWritten);
	GetConsoleScreenBufferInfo(console, &csbi);
	FillConsoleOutputAttribute(console, csbi.wAttributes, bufferSize, topleft, &charsWritten);
	SetConsoleCursorPosition(console, topleft);
}

int enemyY[3];
int enemyX[3];
int enemyFlag[3];
char car[4][4] = {  ' ','#','#',' ',
					'#','#','#','#',
					' ','#','#',' ',
					'#','#','#','#' };

int carPos = WIN_WIDTH / 2;
int score = 0;

void setcolor(int color) //change console text with this methode thinge
{
	SetConsoleTextAttribute(console, color);
}

void gotoxy(int x, int y) { //change the cordinates the text outputs to
	coords.X = x;
	coords.Y = y;
	SetConsoleCursorPosition(console, coords);
}
void setcursor(bool visible, DWORD size) { //if we set it to (0,0) in main. the cursor won't be there anymore. the size becomes 0, and it will no longer be visible?
	CONSOLE_CURSOR_INFO lpCursor{};
	if (size == 0) size = 20; //makes the cursor bigger so it won't show?
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}

void drawBorder() {
	for (int i = 0; i < SCREEN_HEIGHT; i++) { //INT I, for how many verticle outputs there will be.
		for (int j = 0; j < 17; j++) {
			gotoxy(0 + j, i); 
			cout << "±";
			gotoxy(WIN_WIDTH - j, i); 
			cout << "±";
		}
	}
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		gotoxy(SCREEN_WIDTH, i); cout << "±";
	}
}
void genEnemy(int ind) {//ind means indeterminate.
	enemyX[ind] = 17 + rand() % (33);
}               //17(where our border ends) + another number between 1 and 33.
void drawEnemy(int ind) {
	if (enemyFlag[ind] != false) { //change to '= true' for two cars to fall at once
		// there is a glitch whereby one car flickers more than the other.. do a Sleep(50); to see it
		//Sleep(50);
		if (ind == 1) setcolor(1); //blue
		gotoxy(enemyX[ind], enemyY[ind]    ); cout << "****";
		gotoxy(enemyX[ind], enemyY[ind] + 1); cout << " ** ";
		gotoxy(enemyX[ind], enemyY[ind] + 2); cout << "****";
		gotoxy(enemyX[ind], enemyY[ind] + 3); cout << " ** ";
		if (ind == 1) setcolor(7); //default color
	}
}
void eraseEnemy(int ind) { //change to '= true' for two cars to fall at once
	if (enemyFlag[ind] != false) {
		gotoxy(enemyX[ind], enemyY[ind]    ); cout << "    ";
		gotoxy(enemyX[ind], enemyY[ind] + 1); cout << "    ";
		gotoxy(enemyX[ind], enemyY[ind] + 2); cout << "    ";
		gotoxy(enemyX[ind], enemyY[ind] + 3); cout << "    ";
	}
}
void resetEnemy(int ind) {
	eraseEnemy(ind);
	enemyY[ind] = 0; //bool= false
	genEnemy(ind);
}

void drawCar() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			gotoxy(j + carPos, i + 22); 
			cout << car[i][j];
		}
	}
}
void eraseCar() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			gotoxy(j + carPos, i + 22); cout << " ";
		}
	}
}
int collision() {
	if (enemyY[0] + 4 >= 23) {
		if (enemyX[0] + 4 - carPos >= 0 && enemyX[0] + 4 - carPos < 9) {
			return 1;
		}
	}
	if (enemyY[1] + 4 >= 23) {
		if (enemyX[1] + 4 - carPos >= 0 && enemyX[1] + 4 - carPos < 9) {
			return 1;
		}
	}
	return 0;
}
void gameover() {
	clrscr();
	cout << endl;
	cout << "\t\t--------------------------" << endl; //'\t' simply adds 10 spaces
	cout << "\t\t-------- Game Over -------" << endl;
	cout << "\t\t--------------------------" << endl << endl;
	cout << "\t\tPress any key to go back to menu.";
	_getch(); //does not let the next methode *clrscr();* run till ya click something
	clrscr();
}
void updateScore() {
	setcolor(5);
	gotoxy(WIN_WIDTH + 7, 5);cout << "Score: " << score << endl;
	setcolor(7);
}

void instructions() {

	clrscr(); 
	setcolor(4);
	gotoxy(19, 6); cout << "Instructions";
	gotoxy(17, 7); cout << "----------------";
	gotoxy(10, 8); cout << "Avoid Cars by moving left or right. ";
	gotoxy(10, 9); cout << "Press the "; setcolor(6); cout << "'left arrow'"; setcolor(4); cout << " to move left";
	gotoxy(10, 10); cout << "Press the "; setcolor(6); cout << "'right arrow'"; setcolor(4); cout << " to move right";
	gotoxy(10, 11); cout << "Press "; setcolor(6); cout << "'3'"; setcolor(4); cout << " to exit";
	gotoxy(10, 12); cout << "Press any key to go back to menu";
	setcolor(7);
}

void play() {
	carPos = -1 + WIN_WIDTH / 2;
	score = 0;
	enemyFlag[0] = 1;
	enemyFlag[1] = 0;
	enemyY[0] = 1;
	enemyY[1] = 1;
	clrscr();
	drawBorder();
	updateScore();
	genEnemy(0);
	genEnemy(1);
	gotoxy(WIN_WIDTH + 7, 2);cout << "Car Game";
	gotoxy(WIN_WIDTH + 6, 4);cout << "----------";
	gotoxy(WIN_WIDTH + 6, 6);cout << "----------";
	setcolor(4); gotoxy(WIN_WIDTH + 7, 12);cout << "Controls"; setcolor(7);
	gotoxy(WIN_WIDTH + 6, 13);cout << "----------";
	gotoxy(WIN_WIDTH + 4, 14);cout << "<--Left arrow";
	gotoxy(WIN_WIDTH + 4, 15);cout << "Right arrow-->";
	gotoxy(18, 5);cout << "Press any key to start";
	_getch(); //doesn't let the while(1) thing and gotoxy below load till ya press a key.
	gotoxy(18, 5);cout << "                      "; 
	while (1) {
		if (_kbhit()) {
			char c = _getch();
			switch (c)
			{
			case KEY_LEFT: {if (carPos > 18) carPos -= 4;} break;
			case KEY_RIGHT: {if (carPos < 50) carPos += 4;} break;
			case KEY_ESCAPE: { clrscr(); cout << "\t\tyou ended the game"; exit(0);} break;
			case '3': { clrscr(); cout << "\t\tyou ended the game"; exit(0);} break;
			}
		}
		setcolor(2);
		drawCar();
		setcolor(4);
		drawEnemy(0);
		drawEnemy(1);
		setcolor(7);
		if (collision() == 1) {
			gameover();
			return;
		}
		Sleep(50);
		eraseCar();
		eraseEnemy(0);
		eraseEnemy(1);
		if (enemyY[0] == 10)
			if (enemyFlag[1] == 0)
				enemyFlag[1] = 1;

		if (enemyFlag[0] == 1)
			enemyY[0] += 1;

		if (enemyFlag[1] == 1)
			enemyY[1] += 1;

		if (enemyY[0] > SCREEN_HEIGHT - 4) {
			resetEnemy(0);
			score++;
			updateScore();
		}
		if (enemyY[1] > SCREEN_HEIGHT - 4) {
			resetEnemy(1);
			score++;
			updateScore();
		}
	}
}

int main()
{
	setcursor(0, 0);//makes that cursor on the console that awaits your text input go invisible
	srand((unsigned)time(NULL));
		
		int Set[] = {7,7}; 
		int counter = 2; //list size
		char key;

	do
	{
		gotoxy(30, 5); cout << " -------------------------- ";
		gotoxy(30, 6); cout << " |        Car Game        | ";
		gotoxy(30, 7); cout << " --------------------------";
		gotoxy(10, 13); cout << "press the Up and Down arrows to move, and select an option with the 'enter' key.";
		gotoxy(10, 17); cout << "press '3' to quit";
		gotoxy(10, 9);

		setcolor(Set[0]);
		cout << "- Start Game";

		gotoxy(10, 10);
		setcolor(Set[1]);
		cout << "- Instructions";
		key = _getch();

		if (key == KEY_UP && (counter >= 1 && counter <= 2))
		{
			counter--;
			if (counter == 0) counter = 2;
		}
		if (key == KEY_DOWN && (counter >= 1 && counter <= 2))
		{
			counter++;
			if (counter == 3) counter = 1;
		}
		setcolor(7); //default color
		Set[0] = 7;
		Set[1] = 7;

		if (counter == 1)
		{
			Set[0] = 2; //new color.. it can go from 1 to 7. if you want more go make a game with graphics. consoles peak here.
		}
		if (counter == 2)
		{
			Set[1] = 3;
		}


		setcolor(7);
		if (key != '\r')
			if (key == '3' || key == KEY_ESCAPE) {
				clrscr();
				cout << "\t\t you ended the game" <<'\n';
				exit(0);
			}

		if (key == '\r') //it's called a carriage return, i use it for the 'enter' key
		{
			if (counter == 1)
			{
				play();
				//after clicking enter, the code sorta starts here.
			}
			if (counter == 2)
			{
				instructions();

				_getch(); //does not let the screen clear till ya press something.
				clrscr(); //clears the screen
			}
		}
	} while (1);
	return 0;
}