#include <iostream>
#include <vector>
#include <chrono>
#include <conio.h>
#include <cstdlib>
#include <ctime>
using namespace std;

//Initialize global variables
//Initialize methods to control game
//Call constructor in main()

//Step 1
int snakeHeadX, snakeHeadY;
int fruitX, fruitY;
int width = 20;
int height = 10;
vector<pair<int, int>> snakeBody;
bool gameOver;
bool fruitEaten = false;
int score = 1;
auto tTime = chrono::system_clock::now();
char movement = 'd';

//Step 2
void startGame();
void cycleCheck();
void drawBoard();
void spawnFruit();
char getUserInput();
void move(char c);
void wallCollisionCheck();
void fruitCollisionCheck();
void selfCollisionCheck();

//Step 3
int main() {
    startGame();
    return 0;
}

void startGame() {
    srand(time(0));
    gameOver = false;
    snakeHeadX = width / 2;
    snakeHeadY = height / 2;
    pair<int, int> head = {snakeHeadX, snakeHeadY};
    snakeBody.push_back(head);
    fruitX = width / 2 + 3;
    fruitY = height / 2;

    drawBoard();

    cout << "Press any key to begin" << endl;
    _getch();

    while (!gameOver) {
        auto current = chrono::system_clock::now();
        auto elapsedSeconds = current - tTime;
        if (elapsedSeconds >= chrono::milliseconds(500)) {
            cycleCheck();
            fruitEaten = false;
            tTime = chrono::system_clock::now();
        }
    }


}

void cycleCheck() {
    move(getUserInput());
    drawBoard();
    wallCollisionCheck();
    selfCollisionCheck();
    fruitCollisionCheck();
    if (fruitEaten) {
        spawnFruit();
    }
}

void drawBoard() {
    system("cls");

    for (int i = 0; i < width + 2; i++) {
        cout << "_";
    }
    cout << endl;

    for (int y = 0; y < height; y++) {
        cout << "|";
        for (int x = 0; x < width; x++) {
            bool printed = false;

            if (!printed && x == snakeBody[0].first && y == snakeBody[0].second) {
                cout << "O";
                printed = true;

            } 
            if (!printed && x == fruitX && y == fruitY) {
                cout << "F";
                printed = true;

            } 
            if (!printed) {
                for (int i = 1; i < snakeBody.size(); i++) {
                    if (x == snakeBody[i].first && y == snakeBody[i].second) {
                        cout << "o";
                        printed = true;
                    }
                }
            }

            if (!printed) {
                cout << " ";
            }
        }
        cout << "|" << endl;
    }
    
    for (int i = 0; i < width + 2; i++) {
        cout << "_";
    }
}

void spawnFruit() {
    fruitX = rand() % (width - 2)+ 1;
    fruitY = rand() % (height - 2) + 1;
}

char getUserInput() {
    if (_kbhit()) {
        char newKey = _getch();
        if (movement == 'a' && newKey != 'd') {
            movement = newKey;
        } else if (movement == 'd' && newKey != 'a') {
            movement = newKey;
        } else if (movement == 'w' && newKey != 's') {
            movement = newKey;
        } else if (movement == 's' && newKey != 'w') {
            movement = newKey;
        } 
    } 
    return movement;
}

void move(char c) {
    for (int i = snakeBody.size() - 1; i > 0; i--) {
        snakeBody[i] = snakeBody[i - 1];
    }

    if (c == 'w') {
        snakeBody[0].second--;
    } else if (c == 'a') {
        snakeBody[0].first--;
    } else if (c == 's') {
        snakeBody[0].second++;
    } else if (c == 'd') {
        snakeBody[0].first++;
    }
}

void wallCollisionCheck() {
    if (snakeBody[0].first >= width || snakeBody[0].second >= height || snakeBody[0].first < 0 || snakeBody[0].second < 0) {
        gameOver = true;
        cout << "Hit wall. Game over :(" << endl;
    }
}

void fruitCollisionCheck() {
    if (snakeBody[0].first == fruitX && snakeBody[0].second == fruitY) {
        pair<int, int> newSegment = snakeBody[snakeBody.size() - 1];
        snakeBody.push_back(newSegment);
        score++;
        cout << "Yay fruit!! +1" << endl;
        fruitEaten = true;
    }
}

void selfCollisionCheck() {
    for (int i = 1; i < snakeBody.size(); i++) {
        if (snakeBody[0].first == snakeBody[i].first && snakeBody[0].second == snakeBody[i].second) {
            gameOver = true;
            cout << "Self collision! Game over :(" << endl;
        }
    }
}