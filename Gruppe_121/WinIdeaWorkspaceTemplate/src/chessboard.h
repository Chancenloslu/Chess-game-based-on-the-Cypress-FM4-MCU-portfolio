#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <stdbool.h>
#include <stdint.h>
#include "joystick.h" 

void setCursor_Chess(Dir_t joystickDir, uint16_t color);

void fillChessboard();

void fillChesspiece();

void initLattice();

void resetLattice(uint16_t i, uint16_t j);

void setLattice(uint16_t i, uint16_t j, uint16_t color, bool placeable);

void initChesspiece();

void moveCursorLeft(uint8_t analog16, uint8_t analog19);

void moveCursorRight(uint8_t analog13, uint8_t analog23);

bool chessPieceChosen(uint16_t color);

bool chessPieceDrop(uint16_t color);

bool redWin();

bool blueWin();


#endif
