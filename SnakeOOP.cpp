#include <iostream>
#include <vector>
#include <chrono>
#include <conio.h>
#include <cstdlib>
#include <ctime>
using namespace std;

/*
This code allows you to play snake in the terminal. 
This code is designed for young programmers to experiement with OOP.
Feel free to copy this and mess with it. 

To run: g++ SnakeOOP.cpp -o snakeOOP
Then call: .\snakeOOP.exe
*/


class Snake {
    /*
    Private is called an access modifier. The 'private' access modifier means
    that only member functions can access these functions/methods and variables directly.
    
    We put our variables here so that other people using the code cannot just call the
    variables directly and change the implementation, which could break the code. It also
    prevents people from calling specific functions/methods that we want covered up. This 
    concept is called abstraction, and it is when the client does not need to know the
    implementation of a specific function/method. Like how you do not need to know how an 
    engine works to drive a car, you do not need to understand the implementation to use a 
    function. 
    */
    private:
    int snakeHeadX, snakeHeadY;
    vector<pair<int, int>> snake;
    int fruitX, fruitY;
    int width = 20; 
    int height = 10;
    int score;
    bool gameOver;
    bool fruitEaten;
    char movement = 'd';

    void cycleCheck();
    void drawBoard();
    void spawnFruit();
    void getUserInput();
    void move();
    void wallCollisionCheck();
    void fruitCollisionCheck();
    void selfCollisionCheck();

    /*
    These are functions/methods that can be called anywhere by anyone. This includes the 
    constructor, destructor, and generateGame() function to start playing. These are the 
    only functions/methods that need to be accessed to actually play the game. 
    */
    public:
    Snake();
    ~Snake();

    void generateGame();

};

/*
All we do in the main() is create a Snake object (which calls the constructor) and generate the
game with the generateGame() function. All the implementation is done in the member functions. 
*/
int main() {
    Snake snake;
    snake.generateGame();
    return 0;
}

/*
This is the constructor. We can tell because it does not have a type (void, int, string, etc.) and
it just repeats the name of the class. Here, all I have done is initialize the variables to start the game.
*/
Snake::Snake() {
    pair<int, int> head = {width / 2, height / 2};
    snake.push_back(head);
    fruitX = width / 2 + 3;
    fruitY = height / 2;
    score = 1;

    cout << "Have fun!!!" << endl;
}

/*
This is the destruction. We can tell because of the ~ and it does not have a return type. I just have some
fun code in here for an example. C++ is pretty unique compared to other OOP languages, such as java, because 
of the destructors. Take advantage of them if you can!
*/
Snake::~Snake() {
    cout << endl << "Score: " << score << endl;
    cout << endl << "Come again" << endl;
}

/*
This member function, which is the only public one (besides constructor/destructor) actually runs the game.
*/
void Snake::generateGame() {
    auto start = chrono::system_clock::now();
    gameOver = false;
    
    drawBoard();

    cout << endl << "Press any button to start" << endl;
    _getch();

    while (!gameOver) {
        auto timer = chrono::system_clock::now();
        if (timer - start >= chrono::milliseconds(150)) {
            cycleCheck();
            start = chrono::system_clock::now();
        }
    }
}

/*
This function/method is called with every clock cycle, which is set in generateGame().
*/
void Snake::cycleCheck() {
    getUserInput();
    move();
    drawBoard();
    wallCollisionCheck();
    selfCollisionCheck();
    fruitCollisionCheck();
    if (fruitEaten) {
        spawnFruit();
    }
    fruitEaten = false;
}

/*
This draws the board and is called every clock cycle. The game is small enough to be able
to do this. 
*/
void Snake::drawBoard() {
    system("cls");
    for (int i = 0; i < width + 2; i++) {
        cout << "_";
    }
    cout << endl;

    for (int y = 0; y < height; y++) {
        cout << "|";
        for (int x = 0; x < width; x++) {
            bool printed = false;

            if (!printed && x == snake[0].first && y == snake[0].second) {
                cout << "O";
                printed = true;

            } 
            if (!printed && x == fruitX && y == fruitY) {
                cout << "F";
                printed = true;

            } 
            if (!printed) {
                for (int i = 1; i < snake.size(); i++) {
                    if (x == snake[i].first && y == snake[i].second) {
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

/*
This spawns the fruit and is called after the fruit is eaten, which is detected in another
function/method and ensures that the fruit does not spawn inside of the snake. 
*/
void Snake::spawnFruit() {
    bool inSnake = true;

    while (inSnake) {
        fruitX = rand() % (width - 2) + 1;
        fruitY = rand() % (height - 2) + 1;
        int newFruitX = fruitX;
        int newFruitY = fruitY;
        for (int i = 0; i < snake.size(); i++) {
            if (fruitX == snake[i].first && fruitY == snake[i].second) {
                newFruitX = rand() % (width - 2) + 1;
                newFruitY = rand() % (height - 2) + 1;
            }
        }

        if (newFruitX == fruitX && newFruitY == fruitY) {
            inSnake = false;
        }
    } 
}

/*
This detects user input and determines if it a valid input. If it is, it changes the private variable
to the new input. 
*/
void Snake::getUserInput() {
    //Ensure that inputs that aren't allowed don't happen...
    if (_kbhit()) {
        char input = _getch();
        if (movement == 'w' && input != 's') {
            movement = input;
        } else if (movement == 'a' && input != 'd') {
            movement = input;
        } else if (movement == 's' && input != 'w') {
            movement = input;
        } else if (movement == 'd' && input != 'a') {
            movement = input;
        } 
    }
}

/*
This actually moves the snake by moving the position of the head and the following vector of pair<int, int>
*/
void Snake::move() {
    for (int i = snake.size() - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    if (movement == 'w') {
        snake[0].second--;
    } else if (movement == 'a') {
        snake[0].first--;
    } else if (movement == 's') {
        snake[0].second++;
    } else if (movement == 'd') {
        snake[0].first++;
    }
}

/*
This checks if the snake hit the wall
*/
void Snake::wallCollisionCheck() {
    if (snake[0].first < 0 || snake[0].first >= width || snake[0].second < 0 || snake[0].second >= height) {
        gameOver = true;
        cout << endl << "Ran into the wall! Game over :(" << endl;
    }
}

/*
This checks if the snake ate a fruit
*/
void Snake::fruitCollisionCheck() {
    if (snake[0].first == fruitX && snake[0].second == fruitY) {
        pair<int, int> newBody = snake[snake.size() - 1];
        snake.push_back(newBody);
        score++;
        fruitEaten = true;
        cout << endl << "Fruit eaten! +1 score" << endl;
    }
}

/*
This detects if the snake hit itself
*/
void Snake::selfCollisionCheck() {
    for (int i = 1; i < snake.size(); i++) {
        if (snake[0].first == snake[i].first && snake[0].second == snake[i].second) {
            gameOver = true;
            cout << endl << "Ran into yourself! Game over :(" << endl;
        }
    }
}