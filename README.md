# This is a 2048 terminal game wrote in C language
    Slide 4-directionally to merge the cube with same number, ultimate goal is to reach 2048.

## How to run:
    Pull the whole repo to your local directory.
    Make sure there is a .txt file named **_game_state.txt_**
    $ gcc game2048.c
    $ ./a.out

## How to play:
    1. press # to start the game
    2. press r to resume the latest saved game status
    3. press ESC to exit the game at any time
    4. use wsad to control the board
    5. other inputs will do nothing to the program 

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
    2. Prevent illeagal input, a message will show to direct the player. 
    3. Tells the player and quit the game when he win or lose
    4. When the player quit the game, ask him whether to save the current status. If yes, it could be resumed in later stage. 
    5. Reasonable time lags, make the UI more user friendly.