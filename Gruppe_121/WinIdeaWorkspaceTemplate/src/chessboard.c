#include "chessboard.h"
#include "gfx.h"
#include "glcdfont.h"
#include "stdio.h"
#include "string.h"
#include <stdint.h>
#include "s6e2ccxj.h"
#include "analog.h"
#include "delay.h"
#include "display.h"
#include "joystick.h"
#include "button.h"
#include <stdlib.h>

static int16_t cursorX, cursorY, pieceOriginalX, pieceOriginalY;
static uint16_t cursorColor;
static uint8_t textSize;
static int textBackground;

static int latticeSize = 40;
static int pieceRadius = 15;

const uint32_t sleepTime = 10000; // 0,01s

Dir_t joystickDir;

struct chesspiece{
    uint16_t  color;
    uint8_t   X;
    uint8_t   Y;
    bool      alive;
    bool      isQueen;
};


struct lattice{
    uint16_t bg;
    struct chesspiece* chessPiece;
    uint16_t x;
    uint16_t y;
    bool isChosen;
    bool haveCursor;
    bool canBePlaced;
};

void latticToBePlaced(struct chesspiece* cp, uint16_t color);


struct chesspiece *cpChosen = NULL;
struct chesspiece cpNull;


struct lattice latticeArray[8][8];
struct chesspiece chesspieceRed[12];
struct chesspiece chesspieceBlue[12];

void initLattice(){
    int i, j;                 
    for(i = 0; i < 8; i++)       // x
      for(j = 0; j < 8; j++){    // y
        latticeArray[i][j].x            = latticeSize*i;
        latticeArray[i][j].y            = latticeSize*j;     
        latticeArray[i][j].isChosen     = false;
        latticeArray[i][j].haveCursor   = false;
        latticeArray[i][j].canBePlaced  = false;
        if((i + j) % 2 == 0) {
            latticeArray[i][j].bg = BLACK;
        }else{
            latticeArray[i][j].bg = WHITE;
        }
      }    
}

/**
 * @brief to initialize the color of chess pieces on each lattice   
 */
void initChesspiece(){
   
   int i, j, k, m;
   k = 0;
   m = 0;
   
   cpNull.X       = 99;
   cpNull.Y       = 99;
   cpNull.color   = BLACK;
   cpNull.alive   = false;
   cpNull.isQueen = false;
   
   for(i = 0; i < 8; i++) // to the right
      for(j = 0; j < 8; j++){ // upwards
         if((i <= 2) && ((i + j) % 2 == 0)){
            latticeArray[i][j].chessPiece = &chesspieceRed[k];           
            chesspieceRed[k].color        = RED;
            chesspieceRed[k].X            = i;
            chesspieceRed[k].Y            = j;
            chesspieceRed[k].alive        = true;
            chesspieceRed[k].isQueen      = false;
            k++;
         }
         
         else if((i >= 5) && ((i + j) % 2 == 0)){
            latticeArray[i][j].chessPiece = &chesspieceBlue[m];
            chesspieceBlue[m].color       = BLUE;
            chesspieceBlue[m].X           = i;
            chesspieceBlue[m].Y           = j;
            chesspieceBlue[m].alive       = true;
            chesspieceBlue[m].isQueen     = false;
            m++;
         }else if((i + j) % 2 == 0){
            latticeArray[i][j].chessPiece = &cpNull;
         }
      } 
    cursorX        = 0;
    cursorY        = 0;
    pieceOriginalX = 0;
    pieceOriginalY = 0;
    cursorColor    = GREEN;
    //setCursor_Chess(cursorX, cursorY, cursorColor);      
}

/**
 * @brief to fill the Chessboard according to the settings of each Array
 * 
 */
void fillChessboard(){
    for(int i = 0; i < 8; i++) // to the right
        for(int j = 0; j < 8; j++){ // upwards
            cppp_fillRect(latticeArray[i][j].x, latticeArray[i][j].y, latticeSize, latticeSize ,latticeArray[i][j].bg);          
        }
}

void fillChesspiece(){
    for(int m = 0; m < 12; m++){
        if(chesspieceRed[m].alive){
            cppp_fillCircle(chesspieceRed[m].X*latticeSize + latticeSize/2, chesspieceRed[m].Y*latticeSize + latticeSize/2, pieceRadius, chesspieceRed[m].color);
        }
        if(chesspieceBlue[m].alive){
            cppp_fillCircle(chesspieceBlue[m].X*latticeSize + latticeSize/2, chesspieceBlue[m].Y*latticeSize + latticeSize/2, pieceRadius, chesspieceBlue[m].color);
        }
       
    }
}

/**
  * x and y is the index of lattice  
  * todo: The arrangement of odd and even columns is not the same and cannot be achieved simply by shifting left and right
  */
void setCursor_Chess(Dir_t joystickDir, uint16_t color){

    switch(joystickDir){
        case up:
            latticeArray[cursorX][cursorY].haveCursor = false;
            latticeArray[cursorX][cursorY].bg         = (cursorX + cursorY)%2 ? WHITE : BLACK;
            cppp_fillRect(latticeArray[cursorX][cursorY].x, latticeArray[cursorX][cursorY].y, latticeSize, latticeSize ,latticeArray[cursorX][cursorY].bg);
            cursorX = (cursorX + 8) % 8;
            cursorY = (cursorY + 1 + 8) % 8;
            latticeArray[cursorX][cursorY].haveCursor = true;
            latticeArray[cursorX][cursorY].bg         = color; 
            cppp_fillRect(latticeArray[cursorX][cursorY].x, latticeArray[cursorX][cursorY].y, latticeSize, latticeSize ,latticeArray[cursorX][cursorY].bg);
            cppp_microDelay(sleepTime*10);
            break;
        case down:
            latticeArray[cursorX][cursorY].haveCursor = false;
            latticeArray[cursorX][cursorY].bg = (cursorX + cursorY)%2 ? WHITE : BLACK;
            cppp_fillRect(latticeArray[cursorX][cursorY].x, latticeArray[cursorX][cursorY].y, latticeSize, latticeSize ,latticeArray[cursorX][cursorY].bg);
            cursorX = (cursorX + 8) % 8;
            cursorY = (cursorY - 1 + 8) % 8;
            latticeArray[cursorX][cursorY].haveCursor = true;
            latticeArray[cursorX][cursorY].bg = color;
            cppp_fillRect(latticeArray[cursorX][cursorY].x, latticeArray[cursorX][cursorY].y, latticeSize, latticeSize ,latticeArray[cursorX][cursorY].bg);
            cppp_microDelay(sleepTime*10);
            break;
        case left:
            latticeArray[cursorX][cursorY].haveCursor = false;
            latticeArray[cursorX][cursorY].bg = (cursorX + cursorY)%2 ? WHITE : BLACK;
            cppp_fillRect(latticeArray[cursorX][cursorY].x, latticeArray[cursorX][cursorY].y, latticeSize, latticeSize ,latticeArray[cursorX][cursorY].bg);
            cursorX = (cursorX - 1 + 8) % 8;
            cursorY = (cursorY + 8) % 8;        
            latticeArray[cursorX][cursorY].haveCursor = true;
            latticeArray[cursorX][cursorY].bg = color;
            cppp_fillRect(latticeArray[cursorX][cursorY].x, latticeArray[cursorX][cursorY].y, latticeSize, latticeSize ,latticeArray[cursorX][cursorY].bg);
            cppp_microDelay(sleepTime*10);
            break;
        case right:
            latticeArray[cursorX][cursorY].haveCursor = false;
            latticeArray[cursorX][cursorY].bg = (cursorX + cursorY)%2 ? WHITE : BLACK;
            cppp_fillRect(latticeArray[cursorX][cursorY].x, latticeArray[cursorX][cursorY].y, latticeSize, latticeSize ,latticeArray[cursorX][cursorY].bg);
            cursorX = (cursorX + 1 + 8) % 8;
            cursorY = (cursorY + 8) % 8;
            latticeArray[cursorX][cursorY].haveCursor = true;
            latticeArray[cursorX][cursorY].bg = color;
            cppp_fillRect(latticeArray[cursorX][cursorY].x, latticeArray[cursorX][cursorY].y, latticeSize, latticeSize ,latticeArray[cursorX][cursorY].bg);
            cppp_microDelay(sleepTime*10);
            break;
        /*case middle:
            latticeArray[cursorX][cursorY].haveCursor = true;
            latticeArray[cursorX][cursorY].bg = color; 
            break; */
        default:
            break;
    }

}
/**
 * @brief watch the movement of left Joystick
 * 
 * @param analog16 input of AN16
 * @param analog19 input of AN19
 * @return int 
 */
void moveCursorLeft(uint8_t analog16, uint8_t analog19){
    
    //uint16_t analogX = analog19;
    //cppp_write3Digits16Bit(&analogX);
    //writeTextln_s("");
    
    //cppp_microDelay(sleepTime);
    
    // joystick left
    if (analog16 >= 220 && analog19 < 220 && analog19 >= 150) {
        joystickDir = left;
    }
    // joystick right
    else if (analog16 <= 175 && analog19 < 220 && analog19 >= 150) {
        joystickDir = right;
    }
    // joystick up
    else if (analog19 <= 170 && analog16 < 220 && analog16 >= 150){
        joystickDir = up;
    }
    // joystick down
    else if (analog19 >= 220 && analog16 < 220 && analog16 >= 150) {
        joystickDir = down;
    }
    else
        joystickDir = middle;

    setCursor_Chess(joystickDir, cursorColor);
}

/**
 * @brief watch the movement of right Joystick
 * 
 * @param analog13 input of AN13(X)
 * @param analog23 input of AN23(Y)
 * @return int 
 */
void moveCursorRight(uint8_t analog13, uint8_t analog23){
    // joystick left

    if (analog13 >= 220 && analog23 < 220 && analog23 >= 150) {
        joystickDir = left;
    }
    // joystick middle
    else if (analog13 <= 175 && analog23 < 220 && analog23 >= 150) {
        // no reaction
        joystickDir = right;
    }
    // joystick right
    else if (analog23 <= 170 && analog13 < 220 && analog13 >= 150){
        joystickDir = up;
    }
    //setCursor_s(cursorX, cursorY);
    else if (analog23 >= 220 && analog13< 220 && analog13 >= 150){
        joystickDir = down;
    }else {
        joystickDir = middle;
    }
        
    setCursor_Chess(joystickDir, cursorColor);
}  

void resetLattice(uint16_t i, uint16_t j){      
    latticeArray[i][j].chessPiece = &cpNull;
    latticeArray[i][j].bg         = BLACK;
    cppp_fillRect(latticeArray[i][j].x, latticeArray[i][j].y, latticeSize, latticeSize ,latticeArray[i][j].bg);   
}

void setLattice(uint16_t i, uint16_t j, uint16_t color, bool placeable){
    latticeArray[i][j].bg           = color; 
    latticeArray[i][j].canBePlaced  = placeable;
    cppp_fillRect(latticeArray[i][j].x, latticeArray[i][j].y, latticeSize, latticeSize ,latticeArray[i][j].bg);
}

bool chessPieceChosen(uint16_t color) {
    if (latticeArray[cursorX][cursorY].chessPiece->color == color){
        drawChar_s(360, 180, 'c', RED, WHITE, 2);
        cursorColor = YELLOW;
        //cppp_microDelay(sleepTime*100);
        pieceOriginalX = cursorX;
        pieceOriginalY = cursorY;
        //todo: maybe cpChosen should be used as pointer
        cpChosen = latticeArray[cursorX][cursorY].chessPiece;
        latticToBePlaced(cpChosen, color);
        return true;
    }else {
        return false;
    }
}

void chessPieceEaten(uint16_t x, uint16_t y){
    latticeArray[x][y].chessPiece->alive = false;
    resetLattice(x, y);
}

/**
 * @breif can be put on the nearest position and judge if there is an enemy to be eaten
 *
 */
bool chessPieceDrop(uint16_t color){
    int i, j;
    if (latticeArray[cursorX][cursorY].canBePlaced == true){   
        cursorColor = GREEN;
        cpChosen->X = cursorX;
        cpChosen->Y = cursorY;
        resetLattice(pieceOriginalX, pieceOriginalY);
        for(i=0; i<8; i++)
            for(j=0; j<8; j++){
                if(latticeArray[i][j].canBePlaced == true){
                   setLattice(i, j, BLACK, false);
                }
            }
        latticeArray[cursorX][cursorY].chessPiece = cpChosen;
        if(abs(cursorX-pieceOriginalX)== 2 && abs(cursorY-pieceOriginalY)== 2){
            chessPieceEaten((cursorX + pieceOriginalX)/2, (cursorY + pieceOriginalY)/2);  
        }
        return true;
    }else {
        return false;
    }
}

void latticToBePlaced(struct chesspiece* cp, uint16_t color){
    uint8_t x = cp->X;
    uint8_t y = cp->Y;
   
    char xNei[4];
    char yNei[4];
    //find out all the nearest lattice
    for (int i=0; i<4; i++){
        xNei[0] = x - 1;
        yNei[0] = y + 1;
        xNei[1] = x + 1;
        yNei[1] = y + 1;
        xNei[2] = x - 1;
        yNei[2] = y - 1;
        xNei[3] = x + 1;
        yNei[3] = y - 1;        
    }
    
    char m = -1;
    char n = -1;
    for(int j=0; j<4; j++){
        if(0 <= xNei[j] && xNei[j] <= 7 && 0 <= yNei[j] && yNei[j] <= 7){
            if(color == RED){
                if(latticeArray[xNei[j]][yNei[j]].chessPiece->color == RED){
                    
                }else if(latticeArray[xNei[j]][yNei[j]].chessPiece->color == BLUE){         // gegner
                    m = 2*xNei[j]-x;
                    n = 2*yNei[j]-y;
                    if(0 <= m && m <= 7 && 0 <= n && n <= 7 && latticeArray[m][n].chessPiece->color == BLACK){
                        setLattice(m, n, YELLOW, true);      
                    }
                }else{                                             //if there are free places,
                    setLattice(xNei[j], yNei[j], YELLOW, true);
                
                }  
            }else {
                if(latticeArray[xNei[j]][yNei[j]].chessPiece->color == BLUE){
                    
                }else if(latticeArray[xNei[j]][yNei[j]].chessPiece->color == RED){         // gegner
                    m = 2*xNei[j]-x;                                                   // the lattice behind enemy
                    n = 2*yNei[j]-y;
                    if(0 <= m && m <= 7 && 0 <= n && n <= 7 && latticeArray[m][n].chessPiece->color == BLACK){
                        setLattice(m, n, YELLOW, true);      
                    }
                }else{                                             //if there are free places,
                    setLattice(xNei[j], yNei[j], YELLOW, true);
                }
            }
        }
    }                                           
}

bool redWin(){
    bool w = true;
    for(int j=0; j<12; j++){
        if(chesspieceBlue[j].alive)
             w = false;
    }
    return w;
}

bool blueWin(){
    bool w = true;
    for(int i=0; i<12; i++){
        if(chesspieceRed[i].alive)
             w = false;
    }
    return w;
}