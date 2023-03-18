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




// View part


void printDisplay(int arr[4][4]) {
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
            if (arr[i][j] == 0) {
                for (int k = 0; k < 4; k++) {
                    printf(" ");
                }
            } else {
                int numWidth = 0;
                int n = arr[i][j];
                while (n > 0) {
                    numWidth++;
                    n /= 10;
                }
                for (int k = 0; k < (4 - numWidth) / 2; k++) {
                    printf(" ");
                }
                printf("%d", arr[i][j]);
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

void viewbar(int total_score,int largest_num){

    printf("Total score:%d\n",total_score);
    printf("Highest number: %d\n",largest_num);

}

// Control part

int main(){
    //test example
     int total_score=3000;
     int largest_num=256;
     int arr[4][4] = {{2048, 256, 0, 0},
                     {0, 0, 0, 0},
                     {0, 1024, 0, 0},
                     {0, 0, 0, 8}};
    
     viewbar(total_score, largest_num);
     printDisplay(arr)
    //
    
    return 0;
}


