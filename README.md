# This is a 2048 terminal game wrote in C language

## How to run:
    $ gcc game2048.c
    $ ./a.out

## How to operate:
    1. press # to start the game
    2. press ESC to exit the game at any time
    3. use wsad to control the board
    4. other inputs will do nothing to the program 

## How to win:
    when the highest number on board reach 2048

## How to calculate the total score:
    the totoal score is the overall nums you have merged

## Why lose:
    when the board is full, and no more operations can be done
    ### reminder:
        When the board is full, and there could be some operations, for example slide up, operations in other directions will not lead to losing the game, and nothing will change on the board. 
        Only when you slide up, will the board move, and then appear new cube. 
