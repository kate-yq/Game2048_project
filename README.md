# This is a 2048 terminal game wrote in C language
    Slide 4-directionally to merge the cube with same number, ultimate goal is to reach 2048.

## How to run:
    $ gcc game2048.c
    $ ./a.out

## How to play:
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
    
    reminder:
        When the board is full, and there could be some operations, for example slide up, operations in other directions will not lead to losing the game, and nothing will change on the board. 
        Only when you slide up, will the board move, and then appear new cube. 

## Features realized:
    1. Generate new number at a random empty position, "2" of 75% probability, "4" of 25% probability.
    2. A time lag after slide operation, before new number shown. So the UI won't confuse the player. 
    3. Prevent illeagal input, a message will show to direct the player. 
    4. Clear the board before printing new information, so it appears to be an animated board in terminal.
    5. Tells the player and quit the game when he win or lose
    6. Save the current stage sutomatically when the player quit, and have the option to resume the game next time. 
