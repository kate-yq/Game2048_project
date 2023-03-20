#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

// global variable
int board[4][4];
int emptyCell;
int highestNum;
int totalScore;

// Model part




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
    int ch;
    
    viewbar();
    printDisplay();
    //    
    // 3. show these 2 number in the form

    // while (isAlive()){
    //      4. wait for client's operation
    //      UP, DOWN, LEFT, RIGHT using keyboard
    //      5. Merge
    //      6. generate new cell
    // }
    while (ch = getchar() != '#'){
        system("clear");
        viewbar(totalScore, highestNum);

        printDisplay(board);

        if (ch==27) //ESC for exit the 'while'
            break;
        else if (ch=='w') {

            printf("UP");

        }
        else if (ch=='s'){
            printf("DOWN");

        }

        else if (ch=='a'){
            printf("LEFT");

        }

        else if (ch=='d'){
            printf("RIGHT");

        }



        return 0;
}


