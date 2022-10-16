#include "init.h"
#include "blink.h"
#include "acceleration_app.h"
#include "blinkrainbow.h"
#include "display.h"
#include "delay.h"
#include "joystick.h"
#include "button.h"
#include "chessboard.h"
#include "gfx.h"
#include "analog.h"

uint8_t analog11;
uint8_t analog12;
uint8_t analog13;
uint8_t analog16;
uint8_t analog19;
uint8_t analog23;
uint8_t analog17; 

enum STATE {
    RED_CHOOSE,
    RED_MOVE,
    JUDGE_RED_WIN,
    BLUE_CHOOSE,
    BLUE_MOVE,
    JUDGE_BLUE_WIN,
    END
} chess_state;


int main() {
    uint32_t sleepTime_main = 10000; // 0,01s
    

    initBoard();
    
    cppp_fillScreen(WHITE);
    initLattice();
    initChesspiece();  
    //controlLedsInit_s();
    fillChessboard();
    initLED();
    
    chess_state = RED_CHOOSE;
    
    while(1){
        fillChesspiece();
        //controlLeds();
        //ButtonToggleBlueLED();
        cppp_getAnalogValues(&analog11, &analog12, &analog13, &analog16, &analog17, &analog19, &analog23); //left joystick: AN16(X), AN19(Y), right joystick: AN13(X), AN23(Y)
        
        //different LEDs for different state 
        
        if(chess_state == RED_CHOOSE || chess_state == RED_MOVE) {
            RedLedOn();
        }else if(chess_state == BLUE_CHOOSE || chess_state == BLUE_MOVE){
            BlueLedOn();
        }else {
            GreenLedOn();
        }
        
       
       // drawChar_s(360, 80, 'H', RED, WHITE, 2);
        
        switch (chess_state)
        {
            case RED_CHOOSE:
                moveCursorLeft(analog16, analog19);
                if (isLeftButtonPressed()){
                    cppp_microDelay(sleepTime_main);
                    if (isLeftButtonPressed()) {
                        
                        //ButtonToggleBlueLED();
                        if(chessPieceChosen(RED)){
                            chess_state = RED_MOVE;
                        }else{
                            chess_state = RED_CHOOSE;
                        }
                        while (isLeftButtonPressed());
                        cppp_microDelay(sleepTime_main);
                        
                    }else {
                        chess_state = RED_CHOOSE;
                    } 
                }
                break;
            
            case RED_MOVE:
                moveCursorLeft(analog16, analog19);
                if (isLeftButtonPressed()) {
                    cppp_microDelay(sleepTime_main);
                    if(isLeftButtonPressed){
                        if(chessPieceDrop(RED)){
                            chess_state = JUDGE_RED_WIN;
                        }else{
                            chess_state = RED_MOVE;
                        }   
                        while (isLeftButtonPressed());
                        cppp_microDelay(sleepTime_main);
                    }else{
                        chess_state = RED_MOVE;    
                    }  
                }
                break;
            case JUDGE_RED_WIN:
                if(redWin()){
                    writeText_s("Red Wins !!");
                    chess_state = END;
                }else{
                    chess_state = BLUE_CHOOSE;
                }
                break;
            case BLUE_CHOOSE:
                moveCursorRight(analog13, analog23);
                if (isRightButtonPressed()) {
                    cppp_microDelay(sleepTime_main);
                    if(isRightButtonPressed()){
                        if(chessPieceChosen(BLUE)){
                            chess_state = BLUE_MOVE;
                        }else{
                            chess_state = BLUE_CHOOSE;
                        }                      
                        while (isRightButtonPressed());
                        cppp_microDelay(sleepTime_main);
                    }else{
                        chess_state = BLUE_CHOOSE;
                    }                 
                }          
                break;
            case BLUE_MOVE:
                moveCursorRight(analog13, analog23);
                if (isRightButtonPressed()) {
                    cppp_microDelay(sleepTime_main);
                    if(isRightButtonPressed()){   
                        if(chessPieceDrop(BLUE)){
                            chess_state = JUDGE_BLUE_WIN;
                        }else{
                            chess_state = BLUE_MOVE;
                        }                     
                        while (isRightButtonPressed());
                        cppp_microDelay(sleepTime_main);
                    }else{
                        chess_state = BLUE_MOVE;
                    }                   
                }
                break;
            case JUDGE_BLUE_WIN:
                if(blueWin()){
                    writeText_s("Blue Wins !!");
                    chess_state = END;
                }else{
                    chess_state = RED_CHOOSE;
                }
                break;
            case END:
                break;
            default:
                break;
        }  
       
    }     
                 
             
    
    return 0;
}
