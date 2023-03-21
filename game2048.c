#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
//#include <ncurses.h>

// global variable
int N = 4;
int board[4][4];
int emptyCell = 16;
int highestNum;
int totalScore;

// Model part

void printDisplay();
void test();
void clearScreen() {
#ifdef _WIN32
    // Windows platform
    system("cls");
#elif __APPLE__
    // macOS platform
        system("clear");
#else
    // Other platforms (e.g., Linux)
    system("clear");
#endif
}

// check if there can be another operation
// when the board is full
bool canOperate() {
    // check if there is a cell with same num on the right/down side
    // of each cell
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (j < 3 && board[i][j] == board[i][j + 1]) {
                return true;
            }
            if (i < 3 && board[i][j] == board[i + 1][j]) {
                return true;
            }
        }
    }
    return false;
}

// check if the game is over
bool isAlive() {
    if (emptyCell > 0) {
        return true;
    }
    return canOperate();
}

// check if the highest number reach 2048
bool win() {
    return highestNum == 2048;
}

// generate a new number between 2 & 4, with equal probability
int newNum() {
    return (rand() % 2 + 1) * 2;
}

// among all the empty cell
// draw a random cell and fill the cell with a number
void generateNewCell() {
    // if there is no more empty cell, stop generating
    if (emptyCell == 0) {
        return;
    }
    // generate a random number between 0..emptyCell
    // loop through all empty cell
    // fill the random number th empty cell
    int randNum = rand() % emptyCell;
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (board[i][j] == 0) {
                if (randNum == 0) {
                    board[i][j] = newNum();
                    emptyCell--;
                    return;
                }
                randNum--;
            }
        }
    }
}


// MOVE-FOUR-DIRECTIONS

// this function rotates the board clockwise for 90 degrees
// the parameter times indicates how many times we do this 90-degree rotation
void rotate(int times) {
    while (times-- > 0) {
        for (int i = 0; i < N / 2; i++) {
            for (int j = i; j < N - i - 1; j++) {
                int temp = board[i][j];
                board[i][j] = board[N - j - 1][i];
                board[N - j - 1][i] = board[N - i - 1][N - j - 1];
                board[N - i - 1][N - j - 1] = board[j][N - i - 1];
                board[j][N - i - 1] = temp;
            }
        }
    }
}

// move tile board[rowb][col] to board[rowa][col], merge when possible
void moveToLast(int col, int rowa, int rowb) {
    if (rowa == rowb) return;
    board[rowa][col] += board[rowb][col];
    board[rowb][col] = 0;
}

void moveAndMerge() {
    for (int col = 0; col < N; col++) {
        int last = 0;   // last is the row index for last tile
        bool foundFirst = false;
        bool canMerge = true;
        for (int row = 0; row < N; row++) {
            // skip if current block is not occupied
            if (board[row][col] == 0) continue;
            // base case: if we found the first tile,
            // we move the tile to row 0
            if (!foundFirst) {
                foundFirst = true;
                moveToLast(col, last, row);
                continue;
            }
            // general case: compare curr tile to last tile
            // if they can be merged, merge them
            // otherwise move curr tile to last tile's next tile
            if (canMerge && board[last][col] == board[row][col]) {
                moveToLast(col, last, row);
                canMerge = false;   // we can't merge to a tile that's already been merged
                totalScore += board[last][col];
                highestNum = (highestNum > board[last][col]) ? highestNum : board[last][col];

            }
            else {
                moveToLast(col, ++last, row);
                canMerge = true;
            }
        }
    }
}


void moveUp() {
    moveAndMerge();
}

void moveDown() {
    rotate(2);
    moveAndMerge();
    rotate(2);
}

void moveLeft() {
    rotate(1);
    moveAndMerge();
    rotate(3);

}

void moveRight() {
    rotate(3);
    moveAndMerge();
    rotate(1);
}


void highestNumber() {

}



// View part


void printDisplay() {
    //top of the board
    printf("+");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("-");
        }
        printf("+");
    }
    printf("\n");

    for (int i = 0; i < 4; i++) {
        printf("|");
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == 0) {
                for (int k = 0; k < 4; k++) {
                    printf(" ");
                }
            }
            else {
                int numWidth = 0;
                int n = board[i][j];
                while (n > 0) {
                    numWidth++;
                    n /= 10;
                }
                for (int k = 0; k < (4 - numWidth) / 2; k++) {
                    printf(" ");
                }
                printf("%d", board[i][j]);
                for (int k = 0; k < (4 - numWidth + 1) / 2; k++) {
                    printf(" ");
                }
            }
            printf("|");
        }
        //bottom of the board
        printf("\n+");
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                printf("-");
            }
            printf("+");
        }
        printf("\n");
    }
}

void viewbar() {

    printf("Total score:%d\n", totalScore);
    printf("Highest number: %d\n", highestNum);

}


int main() {


    //0. Prepare the variables

    //1. display the board
    viewbar();
    printDisplay();
    printf("Push '#' to start the game or 'ESC' to quit\n");
    while (isAlive()) {

        //receiving [wasd]
        int ch1, ch2;
        ch1 = getchar();
        if (ch1 == 27)
            break;
        else {
            ch2 = getchar();  //receiving "\n"
            // switch case
            switch (ch1) {
                case '#':  //start the game
                    clearScreen();
                    viewbar();
                    generateNewCell();
                    printDisplay();
                    break;
                case  'w':
                    moveUp();
                    clearScreen();
                    viewbar();
                    generateNewCell();
                    printDisplay();
                    printf("UP");
                    break;
                case 's':
                    moveDown();
                    clearScreen();
                    viewbar();
                    generateNewCell();
                    printDisplay();
                    printf("DOWN");
                    break;
                case 'a':
                    moveLeft();
                    clearScreen();
                    viewbar();
                    generateNewCell();
                    printDisplay();
                    printf("LEFT");
                    break;
                case 'd':
                    moveRight();
                    clearScreen();
                    viewbar();
                    generateNewCell();
                    printDisplay();
                    printf("RIGHT");
                    break;
                default:
                    break;
            }
            if (win()) {
                printf("Congratulations! you win.\n");
            }
        }
    }


    return 0;
}
