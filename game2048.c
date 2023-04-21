#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

// global variable
int N = 4;
int board[4][4];
int emptyCell = 16;
int highestNum = 0;
int totalScore = 0;
int sleepTime = 400000;
bool started = false;

//struct to for game state to save and resume
typedef struct game_state{
    int board[4][4];
    int highestNum;
    int totalScore;
    int emptyCell;
}Game_states;

// function name 
// helper functions
bool canOperate();
bool canOperateUp();
bool canOperateDown();
bool canOperateLeft();
bool canOperateRight();
bool isAlive();
bool win();
int newNum();
void generateNewCell();
// move functions
void rotate(int times);
void moveToLast(int col, int rowa, int rowb);
void moveAndMerge();
void moveUp();
void moveDown();
void moveLeft();
void moveRight();
// resume functions
void save_game_in_file(const Game_states* game);
bool load_game(Game_states* game);
// print functions
void printDisplay();
void viewbar();

// comprehensive control functions
void showInfo();

void startGame();
void slideUP();
void slideDOWN();
void slideLEFT();
void slideRIGHT();

void save_game();
void resume_game();


// Model part

// check if the game is over
bool isAlive() {
    return emptyCell>0 || canOperate();
}

// check if there can be another operation
// when the board is full
bool canOperate() {
    return canOperateUp() || canOperateDown() 
        || canOperateLeft() || canOperateRight();
}

// check if the board will be different if move up
bool canOperateUp() {
    int i, j;
    for (i=1; i<N; i++) {
        for (j=0; j<N; j++) {
            if (board[i][j] > 0 && 
            (board[i-1][j] == board[i][j] || board[i-1][j] == 0)){
                return true;
            }
        }
    }
    return false;
}

// check if the board will be different if move down
bool canOperateDown() {
    int i, j;
    for (i=0; i<N-1; i++) {
        for (j=0; j<N; j++) {
            if (board[i][j] > 0 && 
            (board[i+1][j] == board[i][j] || board[i+1][j] == 0)){
                return true;
            }
        }
    }
    return false;
}

// check if the board will be different if move left
bool canOperateLeft() {
    int i, j;
    for (i=0; i<N; i++) {
        for (j=1; j<N; j++) {
            if (board[i][j] > 0 && 
            (board[i][j-1] == board[i][j] || board[i][j-1] == 0)){
                return true;
            }
        }
    }
    return false;
}

// check if the board will be different if move right
bool canOperateRight() {
    int i, j;
    for (i=0; i<N; i++) {
        for (j=0; j<N-1; j++) {
            if (board[i][j] > 0 && 
            (board[i][j+1] == board[i][j] || board[i][j+1] == 0)){
                return true;
            }
        }
    }
    return false;
}

// check if the highest number reach 2048
bool win() {
    return highestNum == 2048;
}

// generate a new number between 2 & 4
// probability: 2 - 75%, 4 - 25%
int newNum() {
    int p = rand() % 4;
    if (p<3){
        return 2;
    } else {
        return 4;
    }
}

// among all the empty cell
// draw a random cell and fill the cell with a number
void generateNewCell() {
    // if there is no more empty cell, stop generating
    // but not fail the game because need to check if it can be slide
    // form other directions
    if (emptyCell == 0) {
        return;
    }
    // generate a random number between 0..emptyCell
    // loop through all empty cell
    // fill the random number th empty cell
    int randNum = rand() % emptyCell;
    int i, j;
    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            if (board[i][j] == 0){
                if (randNum == 0){
                    board[i][j] = newNum();
                    emptyCell--;
                    highestNum = board[i][j] > highestNum? board[i][j] : highestNum;
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
    if (board[rowa][col] != 0){
        // means merge a new one into this one
        emptyCell++;
    }
    board[rowa][col] += board[rowb][col];
    highestNum = board[rowa][col] > highestNum ? board[rowa][col] : highestNum;
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
            } else {
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


// resume implementation

// save the game states to a file
//write file
void save_game_in_file(const Game_states* game){
    FILE * file=fopen("game_state.bin","wb");
    if(file==NULL){
        printf("Error:Unable to save the game state\n");
        return;
    }
    size_t items_written= fwrite(game,sizeof(Game_states),1,file);
    if (items_written != 1) {
        perror("Error: fwrite failed");
    }
    fclose(file);
}

//load game
//if can find and read the file return true, otherwise return false
bool load_game(Game_states* game){
    FILE * file=fopen("game_state.bin","rb");
    if(file==NULL){
        printf("Error: No game state to load.\n");
        return false;
    }
    size_t items_read=fread(game,sizeof(Game_states),1,file);
    fclose(file);

    if(items_read!=1){
        printf("Error: Incorrect number of items read from the game.\n");
        return false;
    }

    return true;
}


// view part

// detect system and call corresponding terminal command
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

// print the board
void printDisplay() {
    //top of the board
    printf("+");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("-");
        }
        printf("+");
    }
    printf("\n");

    for (int i = 0; i < N; i++) {
        printf("|");
        for (int j = 0; j < N; j++) {
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
                for (int k = 0; k < (N - numWidth) / 2; k++) {
                    printf(" ");
                }
                printf("%d", board[i][j]);
                for (int k = 0; k < (N - numWidth + 1) / 2; k++) {
                    printf(" ");
                }
            }
            printf("|");
        }
        //bottom of the board
        printf("\n+");
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                printf("-");
            }
            printf("+");
        }
        printf("\n");
    }
}

// print the score information
void viewbar() {
    printf("Total score:%d\n", totalScore);
    printf("Highest number: %d\n", highestNum);
}



// Control part

// clear the page and then print the viewbar and board
void showInfo(){
    clearScreen();
    viewbar();
    printf("\n");
    printDisplay();
}

// start game
void startGame(){
    generateNewCell();
    generateNewCell();
    showInfo();
    started = true;
}

// four major function and outprint for 4 direction slide
// show the merge result first
// wait for a while then generate the new cube
void slideUP(){
    if (!canOperateUp()){
        printf("can't move up!\n"); 
        return;
    }
    moveUp();
    showInfo();
    printf("UP\n");
    usleep(sleepTime);
    generateNewCell();
    showInfo();
    printf("UP\n");
}


void slideDOWN(){
    if (!canOperateDown()){
        printf("can't move down!\n"); 
        return;
    }
    moveDown();
    showInfo();
    printf("DOWN\n");
    usleep(sleepTime);
    generateNewCell();
    showInfo();
    printf("DOWN\n");
}


void slideLEFT(){
    if (!canOperateLeft()){
        printf("can't move to left!\n"); 
        return;
    }
    moveLeft();
    showInfo();
    printf("LEFT\n");
    usleep(sleepTime);
    generateNewCell();
    showInfo();
    printf("LEFT\n");
}


void slideRIGHT(){
    if (!canOperateRight()){
        printf("can't move to right!\n"); 
        return;
    }
    moveRight();
    showInfo();
    printf("RIGHT\n");
    usleep(sleepTime);
    generateNewCell();
    showInfo();
    printf("RIGHT\n");
}

// same game
void save_game(){
    printf("Saving game...\n");
    Game_states* game=malloc(sizeof(Game_states));
    memcpy(game->board,board,sizeof(board));
    game->highestNum=highestNum;
    game->totalScore=totalScore;
    game->emptyCell=emptyCell;
    save_game_in_file(game);
    usleep(sleepTime);
    printf("Game saved!\n");
    free(game);
}

//resume game
void resume_game(){
    Game_states* game=malloc(sizeof(Game_states));
    printf("Loading game..\n");
    if(load_game(game)){
        memcpy(board,game->board,sizeof(board));
        highestNum=game->highestNum;
        totalScore=game->totalScore;
        emptyCell=game->emptyCell;
        showInfo();
        printf("Game loaded!\n");
        started = true;
    }

    free(game);
}



int main() {

    //1. print starting message
    printf("Welcome to the 2048 game!\n\n");
    printf("Here are the instructions:\n\n");
    printf("Press # to start the game or ESC to quit\n");
    printf("Press w to slide UP\n");
    printf("Press s to slide DOWN\n");
    printf("Press a to slide LEFT\n");
    printf("Press d to slide RIGHT\n");
    printf("Press r to Resume game\n");

    while (isAlive() && !win()) {
        //receiving [wasd]
        int ch1, ch2;
        ch1 = getchar();
        if (ch1 == 27) {
            break;
        } else {
            ch2 = getchar();  //receiving "\n"
            // switch case
            switch (ch1) {
                case '#':  
                    if (!started){
                        startGame();
                    } else {
                        printf("The game already started.\n");
                    }
                    break;
                case  'w':
                    slideUP();
                    break;
                case 's':
                    slideDOWN();
                    break;
                case 'a':
                    slideLEFT();
                    break;
                case 'd':
                    slideRIGHT();
                    break;
                case 'r':
                    if (!started){
                        resume_game();
                    } else {
                        printf("The game already started, cannot resume.\n");
                    }
                    break;
                default:
                    printf("Illeagal input! Please use wsad.\n");
            }
        }
    }

    if (win()){
        printf("YOU WIN!\n");
    } else if (!isAlive()){
        printf("YOU LOSE!\n");
    } else {
        printf("Do you want to save the current status? (Y/N)\n");
        char ans;
        scanf("%s",&ans);

        if(ans=='Y' || ans=='y'){
            save_game();
        }
    }

    return 0;
}
