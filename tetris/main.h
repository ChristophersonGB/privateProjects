#ifndef MAIN_H
#define MAIN_H

#define DEFAULT_WIDTH           10      // default amount of spaces that tetrimos can be stacked in, horizontally
#define DEFAULT_HEIGHT          20      // default amount of spaces that tetrimos can be stacked in, vertically
#define COMBINED_WALL_WIDTH     2       // character width of one wall * 2
#define PADDING_CHAR            "#"     // character to be printed outside of the 

// tetrimos defined in main.h

const int ROW_WIDTH = DEFAULT_WIDTH + COMBINED_WALL_WIDTH;

// const char TETRIMOS[7][] = 
// {
//     "l/rr", // I
//     "l/uu", // J
//     "r/uu", // L
//     "rdl", // O 
//     "r/dl", // S
//     "l/d/r", // T
//     "l/dr"  // Z
// };


// I was so proud of myself for using just one byte for the block placements... then I had to do the offsets...
// decided against memory optimization there because it is mental health month
typedef struct piece {
    unsigned char blockPlacement;   // 8 bits that define the placement of a piece's blocks, 
                                    // 1 for block, 0 for no block, 4 bits per row/column
    char xOffset[4];                // this is the x offset added to the index to center the center piece
    char yOffset[4];                // this is the y offset, one piece has 4 possible rotations thus 4 possible offsets
} TETRIMO;

const TETRIMO TETRIMOS[7] = 
{
    {0b00001111, {0, 1, 0, 0}, {0, -2, -1, -2}}, // I
    {0b00010111, {1, 1, -2, 0}, {0, -3, -1, 0}}, // J
    {0b10001110, {-2, 1, 1, 0}, {0, 0, -1, -3}}, // L
    {0b11001100, {-1, 1, 1, 1}, {0, 0, 0, -2}}, // O 
    {0b01101100, {-1, 0, 0, 0}, {0, -1, 0, -2}}, // S
    {0b01001110, {-1, 1, 0, 0}, {0, -1, -1, -2}}, // T
    {0b11000110, {-1, 0, 0, 0}, {0, -1, 0, -2}}  // Z
};

struct winsize window;

#endif