#include <stdio.h>
#include <unistd.h>
//#include <curses.h> //figure out curses sometime

#define DEFAULT_WIDTH 10 // default amount of spaces that tetrimos can be stacked in, horizontally
#define DEFAULT_HEIGHT 20 // default amount of spaces that tetrimos can be stacked in, vertically
#define COMBINED_WALL_WIDTH 2 // character width of one wall * 2

const char TETRIMOS[7][8] = 
{
    "xxxx    ", // I
    "x   xxx ", // J
    "  x xxx ", // L
    "xx  xx  ", // O 
    " xx xx  ", // S
    " x  xxx ", // T
    "xx   xx "  // Z
}


void drawFrame(char frameData[], int frameDataSize, int frameWidth){
    for (int h = 0; h < frameDataSize; h += frameWidth) {
        for (int w = h; w < h + frameWidth; w++){
            printf("%c", frameData[w]);
        }
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
void buildCleanFrame(char frameData[],  int frameDataSize, int frameWidth){
    int rowWidth = frameWidth - COMBINED_WALL_WIDTH;

    for (int h = 0; h < frameDataSize - frameWidth; h += frameWidth){
        fillRowWithSecondArg(frameData, ' ', rowWidth, h);
    }
   fillRowWithSecondArg(frameData, '_', rowWidth, frameDataSize - frameWidth);
}

// Called once per 'frame' while running. Returns 0 on success, 1 on error, and a negative value on exit
int update(char frameData[], int frameDataSize, int frameWidth, int frameNumber){
    char input;


    // HANDLING INPUT HERE
    input = getchar();
    switch (input){
        case 'w' : // rotates piece
            break;
        case 's' : // moves piece to the right
            break;
        case 'a' : // moves piece to the left
            break;
        case 'd' : // increases the speed of the piece falling while it is held down
            break;
        case 'k' :
            return -1; // k kills 
        default :
            break;
    }
    printf("----Frame Number %d----\n", frameNumber);
    drawFrame(frameData, frameDataSize, frameWidth);
    return 0;
}

int main(int argc, char** argv){
    // check arguments, if there are 2 arguments, or 1 argument, then process input. if only 1 arg, set the default height to twenty

    // define char array of size DEFAULT_WIDTH + 2 for drawing the walls, otherwise 

    int frameWidth = DEFAULT_WIDTH + COMBINED_WALL_WIDTH;
    int frameHeight = DEFAULT_HEIGHT + 1;

    int frameDataSize = frameWidth * frameHeight;
    char frameData[frameDataSize];

    printf("Welcome to Tetris!\n");
    buildCleanFrame(frameData, frameDataSize, frameWidth);
    // for (int i = 0; i < frameDataSize; i++){
    //     frameData[i] = 'x';
    // }
    drawFrame(frameData, frameDataSize, frameWidth);

    int frameNumber = 1;
    int running = 1;

    while (running) {
        running = update(frameData, frameDataSize, frameWidth, frameNumber);
        frameNumber++;
        sleep(1);
    }
}