#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

// global variable
int N = 4; 
int board[4][4];
int emptyCell;
int highestNum;
int totalScore;

// Model part

void printDisplay();
void test(); 


// check if there can be another operation
// when the board is full
bool canOperate(){
    // check if there is a cell with same num on the right/down side
    // of each cell
    int i, j;
    for (i=0; i<4; i++) {
        for (j=0; j<4; j++) {
            if (j<3 && board[i][j] == board[i][j+1]){
                return true;
            }
            if (i<3 && board[i][j] == board[i+1][j]){
                return true;
            }
        }
    }
    return false;
}

// check if the game is over
bool isAlive(){
    if (emptyCell > 0){
        return true;
    }
    return canOperate();
}

// check if the highest number reach 2048
bool win(){
    return highestNum == 2048;
}

// generate a new number between 2 & 4, with equal probability
int newNum(){
    return (rand() % 2 + 1) * 2;
}

// among all the empty cell
// draw a random cell and fill the cell with a number
void generateNewCell(){
    // if there is no more empty cell, stop generating
    if (emptyCell == 0){
        return;
    }
    // generate a random number between 0..emptyCell
    // loop through all empty cell
    // fill the random number th empty cell
    int randNum = rand() % emptyCell;
    int i, j;
    for (i=0; i<4; i++){
        for (j=0; j<4; j++){
            if (board[i][j] == 0){
                if (randNum == 0){
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

// move tile [rowb][col] to [rowa][col] 
void merge(int col, int rowa, int rowb) {
    board[rowa][col] += board[rowb][col]; 
    totalScore += board[rowa][col]; 
    board[rowb][col] = 0; 
}

void moveToLast(int col, int rowa, int rowb) {
    board[rowa][col] += board[rowb][col]; 
    board[rowb][col] = 0; 
}

void moveAndMerge() {
    for (int col = 0; col < N; col++) {
        int last = 0; 
        int index = 0;  // the index for next tile to be moved 
        for (int row = 0; row < N; row++) {
            if (board[row][col] == 0) continue; 
            if (board[row][col] == last) {
                merge(col, index++, row); 
                last = 0; 
                // printDisplay();
            } else {
                last = board[row][col]; 
                if (index != row) {
                    if (board[index][col] != 0) {
                        moveToLast(col, ++index, row); 
                    } else {
                        moveToLast(col, index, row); 
                    }
                    // printDisplay();
                } 
                // printDisplay();
            }
            // if it is zero, continue;  
            // if it is not zero
                // check if it can be merged to last
                    // if yes merge, set last to 0, index = 
                    // if no, make it last and move it to index++
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
            } else {
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

void viewbar(){

    printf("Total score:%d\n", totalScore);
    printf("Highest number: %d\n",highestNum);

}

// Control part

int main(){

    // 1. draw overall map/form
    // 2. generate 2 random cell at beginning
    //test example
    totalScore = 3000;
    highestNum = 256;

    // test(); 

    // 3. show these 2 number in the form

    // while (isAlive()){
    //      4. wait for client's operation
    //      UP, DOWN, LEFT, RIGHT using keyboard
    //      5. Merge
    //      6. generate new cell
    // }

    return 0;
}


void test() {
        int i = 0;
    int j=0;
    for (i=0; i<4; i++){
        // if (i == 1) continue; 
        for (j=0; j<2; j++){
            board[i][j] = 2;
        }
        for (j = 2; j < 4; j++) {
            board[i][j] = 4; 
        }
    }
    viewbar();
    printDisplay();

    moveUp(); 
    printDisplay();

    moveLeft(); 
    printDisplay(); 

    moveRight(); 
    printDisplay(); 

    moveDown(); 
    printDisplay(); 
}


