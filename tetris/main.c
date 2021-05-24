#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
// #include <curses.h> //figure out curses sometime
#include <sys/ioctl.h> // used to find window size - unix-based specific
#include <signal.h> // used to catch 

#include "main.h"


// // Tetrimos, when rotated, are off from their center. This array stores the offset for each Tetrimo to return to its center
// const char TETRIMO_OFFSET[7][3] = 
// {
//     {1 - (2 * ROW_WIDTH), -1 - ROW_WIDTH, -ROW_WIDTH}, // I
//     {-1 - (2 * ROW_WIDTH), -2 + ROW_WIDTH, ROW_WIDTH}, // J
//     {1, 2, 3}, // L
//     {-2 * ROW_WIDTH, -2, 3}, // O
//     {1, 2, 3}, // S
//     {1, 1, -ROW_WIDTH}, // T
//     {1, 2, 3}  // Z
// };



/*
*   Tetris Program
*   May 2021 - @ChristophersonGB
*   
*   
*   - Currently Linux (potentially Unix) specific, not very portable
*       EX: Terminal Size, exit code, etc
*
*/

void onScreenResize(int sig){
    ioctl(fileno(stdout), TIOCGWINSZ, &window);
}

// Returns 0 if there are no collisions, returns 1 if there are
int checkCollision(char frameData[], int frameDataIndex){
    // check if blocks being drawn to are anything other than ' ' or '_'
    
}

void drawBlock(char frameData[], int blockIndex){
    // TODO: Handle collision 
    //       Every block must be checked for collision before anything is drawn, call checkCollision with tetrimo first 
    frameData[blockIndex] = 'X';
}

// TODO: Handle collision
// void drawTetrimo(char frameData[], int tetrimoFrameIndex, int tetrimoTypeIndex, int rotation){
//     int bottomCenterPieceIndex;
//     // int topLeftCenterPieceIndex; 
//     TETRIMO tetrimo = TETRMIOS[tetrimoTypeIndex];
//     for (int h = 0; h < tetrimo.centerHeight; h++){
//         for (int w = 0; w < tetrimo.centerWidth; w++){
//             frameData[tetrimoFrameIndex];
//         }
//     }
// }

void drawTetrimo(char frameData[], int tetrimoFrameIndex, int tetrimoTypeIndex, int rotation){
    int rowSize = DEFAULT_WIDTH + COMBINED_WALL_WIDTH;
    TETRIMO tetrimo = TETRIMOS[tetrimoTypeIndex];
    // Tetrimos are stored as a string of length 8. They are drawn in as a 4 x 2 or 2 x 4 grid. We get
    // four total rotations from them being drawn as a combination of the two dimensions, plus the option of
    // reading in the terimos memory from right-to-left or left-to-right.
    // 
    // When it is rotated, It could potentially be drawn from top to bottom, left to right,
    // right to left, or bottom up. These affect the for loop variables i, columnsOrRows, and 
    // increment.

    tetrimoFrameIndex += tetrimo.xOffset[rotation];
    tetrimoFrameIndex += (tetrimo.yOffset[rotation] * rowSize);
    switch (rotation){
        case 0 :
            // // Row 1 of 2 x 4
            // for (int i = 0; i < 4; i++){
            //     frameData[currentTetrimoFrameIndex + i] = TETRIMOS[tetrimoTypeIndex][i];
            // }
            // // Row 2 of 2 x 4
            // for (int i = 0; i < 4; i++){
            //     frameData[currentTetrimoFrameIndex + rowSize + i] = TETRIMOS[tetrimoTypeIndex][i + 4];
            // }
            for (int r = 0; r < 2; r++){
                for (int c = 0; c < 4; c++){
                    if (tetrimo.blockPlacement >> (c + (r * 4)) & 1){
                        drawBlock(frameData, (tetrimoFrameIndex + (rowSize * r) + c));
                    }
                }
            }
            break;
        case 1 :
            // for (int i = 0; i < 4; i++){
            //     frameData[currentTetrimoFrameIndex + (rowSize * i)] = TETRIMOS[tetrimoTypeIndex][i];
            // }
            // for (int i = 4; i < 8; i++){
            //     frameData[currentTetrimoFrameIndex + (rowSize * (i - 4) + 1)] = TETRIMOS[tetrimoTypeIndex][i];
            // }
            for (int r = 0; r < 4; r++){
                for (int c = 0; c < 2; c++){
                    if (tetrimo.blockPlacement >> ((c * 4) + 3 - r) & 1){
                        drawBlock(frameData, (tetrimoFrameIndex + (rowSize * r) + c));
                    }
                }
            }
            break;
        case 2 :
            // for (int i = 7; i >= 4; i--){
            //     frameData[currentTetrimoFrameIndex + (i - 4)] = TETRIMOS[tetrimoTypeIndex][i];
            // }
            // for (int i = 4; i >= 0; i--){
            //     frameData[currentTetrimoFrameIndex + rowSize + i] = TETRIMOS[tetrimoTypeIndex][i];
            // }
            //tetrimoFrameIndex = tetrimoFrameIndex + 1; // test offset
            for (int r = 0; r < 2; r++){
                for (int c = 0; c < 4; c++){
                    if (tetrimo.blockPlacement >> (7 - (c + (r * 4))) & 1){
                        drawBlock(frameData, (tetrimoFrameIndex + (rowSize * r) + c));
                    }
                }
            }
            break;
        case 3 :
            // for (int i = 7; i >= 4; i--){
            //     frameData[currentTetrimoFrameIndex + (rowSize * (i - 4))] = TETRIMOS[tetrimoTypeIndex][i];
            // }
            // for (int i = 4; i >= 0; i--){
            //     frameData[currentTetrimoFrameIndex + (rowSize * i + 1)] = TETRIMOS[tetrimoTypeIndex][i];
            // }
            // tetrimoFrameIndex = tetrimoFrameIndex - rowSize;

            for (int r = 0; r < 4; r++){
                for (int c = 0; c < 2; c++){
                    if (tetrimo.blockPlacement >> (7 - ((c * 4) + 3 - r)) & 1){
                        drawBlock(frameData, (tetrimoFrameIndex + (rowSize * r) + c));
                    }
                }
            }
            break;
        default :
            printf("Error has occured, rotation is >4 or negative\n");
            exit(EXIT_FAILURE);
            break;
    }
}

// Called whenever a Tetrimo is spawned. For a spawned Tetrimo, updateTetrimo() is called
void spawnTetrimo(char frameData[], int tetrimoIndex){
    // FIXME: Cleaner way to find spawn index? Define frame top buffer instead of using explicit value?
    int spawnIndex = (DEFAULT_WIDTH / 2) + (4 * (DEFAULT_WIDTH + COMBINED_WALL_WIDTH));
    if (DEFAULT_WIDTH > spawnIndex + 4){
        printf("ERROR: Default width too small! Currently %d", DEFAULT_WIDTH);
        exit(EXIT_FAILURE); 
    }
    
    drawTetrimo(frameData, spawnIndex, tetrimoIndex, 0);
}

// returns 1 when tetrimo is 'set', otherwise returns 0
int updateTetrimo(char frameData[], int currentTetrimoFrameIndex, int tetrimoTypeIndex, int rotation){
    return -1;
}

void printPadding(int padding){
    for (int p = 0; p < padding; p++){
        printf(PADDING_CHAR);
    }
}

void drawFrame(char frameData[], int frameDataSize){
    int terminalColSize = window.ws_col;
    int frameWidth = DEFAULT_WIDTH + COMBINED_WALL_WIDTH;
    // int frameHeight = (frameDataSize / frameWidth) - 4;

    // The Tetris grid is DEFAULT_WIDTH wide, DEFAULT_HEIGHT tall. To draw it in the center,
    // we have to calulate the center of the terminal columns, then subtract DEFAULT_WIDTH / 2 from that
    // to get the correct padding on the left side of the Tetris grid, and since its symmetrical, the right aswell.

    int paddingW = (terminalColSize / 2) - ((frameWidth) / 2);

    // The grid is drawn starting at frameWidth * 4 because the frame extends 4 
    // places above the screen to allow for pieces to spawn above the board and be offset correctly.

    for (int h = frameWidth * 4; h < frameDataSize; h += frameWidth) {

        // Prints out the padding on the left side
        printPadding(paddingW);

        // Prints out the information in frameData in the current row
        for (int w = h; w < h + frameWidth; w++){
            printf("%c", frameData[w]);
        }

        // Prints out the padding on the right side
        printPadding(paddingW);
        
        // If the terminal has an odd number of columns, we print 1 extra character. If its even, we have nothing else to print.
        printPadding(terminalColSize % 2);

        //printPadding(2);

        printf("\n");
    }
}

// Used by buildCleanFrame() to create spaces in char array between walls
void fillRowWithSecondArg(char frameData[], char c, int rowWidth, int currentHeight){
    frameData[currentHeight] = '|';
    for (int i = 1; i <= rowWidth; i++){
        frameData[currentHeight + i] =  c;
    }
    frameData[currentHeight + rowWidth + 1] = '|';
}

// Called at the beginning of a game to get a char array with walls characters filled and empty spaces 
void buildCleanFrame(char frameData[],  int frameDataSize){
    int frameWidth = DEFAULT_WIDTH + COMBINED_WALL_WIDTH;

    for (int h = 0; h < frameDataSize - frameWidth; h += frameWidth){
        fillRowWithSecondArg(frameData, ' ', DEFAULT_WIDTH, h);
    }

   fillRowWithSecondArg(frameData, '_', DEFAULT_WIDTH, frameDataSize - frameWidth);
}

// runs a demo cycle through tetrimo pieces and their rotations, must be called on by Update to run
void demo(char frameData[], int frameDataSize, int frameNumber){
    int rotation = (frameNumber - 1) % 4;
    int tetrimoIndex = ((frameNumber - 1) / 4) % 7;
    buildCleanFrame(frameData, frameDataSize);
    drawTetrimo(frameData, 53, tetrimoIndex, rotation);
}

// All game logic goes here, called once per frame before drawing
int update(char frameData[], int frameDataSize, int framesSinceLastSetPiece){
    // TODO: Implement input that does not interrupt system
    // char input;
    int difficultyLevel = 1; // gets higher as time goes on

    // TODO: create an int rotation amount; rotation is from 0 - 3, 0 being default
    //       if rotation key is pressed, increment rotation and then mod 4

    // // HANDLING INPUT HERE
    // input = getchar();
    // switch (input){
    //     case 'w' : // rotates piece
    //         break;
    //     case 's' : // moves piece to the right
    //         break;
    //     case 'a' : // moves piece to the left
    //         break;
    //     case 'd' : // increases the speed of the piece falling while it is held down
    //         break;
    //     case 'k' :
    //         return -1; // k kills 
    //     default :
    //         break;
    // }

    // Store frame number that last piece was set on, 

    demo(frameData, frameDataSize, frameNumber);

    // int rotation = (frameNumber - 1) % 4;
    // buildCleanFrame(frameData, frameDataSize, frameWidth);
    // drawTetrimo(frameData, 5, 5, rotation);
    
    return 1;
}

// Calls update() for game logic, then draws the current frame
void run(int framesPerSecond){
    int frameWidth = DEFAULT_WIDTH + COMBINED_WALL_WIDTH;
    int frameHeight = DEFAULT_HEIGHT;

    int frameDataSize = frameWidth * (frameHeight + 4); // we add four to the height to have extra space for spawned tetrimos
    char frameData[frameDataSize];
    char setPieces[frameDataSize]; // setPieces stores the frame of pieces that are 

    int running = 1;
    int framesSinceLastSetPiece = 1;

    long framesPerNanosecond = (long) 1000000000 / framesPerSecond;

    struct timespec *timer = malloc(sizeof(struct timespec));
    timer->tv_sec = (int) framesPerNanosecond / 1000000000;
    timer->tv_nsec = (long) framesPerNanosecond % 1000000000; // one second / framesPerSecond % one second
    
    

    buildCleanFrame(frameData, frameDataSize);

    while (running){
        nanosleep(timer, NULL);
        running = update(frameData, frameDataSize, framesSinceLastSetPiece);
        printf("----Frame Number %d----\n", framesSinceLastSetPiece);
        drawFrame(frameData, frameDataSize);
        framesSinceLastSetPiece++;
    }

    free(timer);
}

int main(int argc, char** argv){
    // check arguments, if there are 2 arguments, or 1 argument, then process input. if only 1 arg, set the default height to twenty

    // define char array of size DEFAULT_WIDTH + 2 for drawing the walls, otherwise 
    

    // terminal = initscr(); // CURSES.h

    signal(SIGWINCH, onScreenResize); // FIXME: Error checking?

    ioctl(fileno(stdout), TIOCGWINSZ, &window); // column width and height of terminal set

    printf("Welcome to Tetris!\n");

    if (argc < 2){
        run(60);
    }
    else {
        run(atoi(argv[1]));
    }    
}