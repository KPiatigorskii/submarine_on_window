#pragma once
#include "ships.h"

#define AREA_SIZE_WITH_BORDERS 12
#define CELL_SIZE 35

enum gameEntity { USER_ENTITY = 0, PC_ENTITY = 1, PC2_ENTITY = 2 };

typedef struct gameArea
{
    int shipCounter;
    int moveCounter;
    int checkedCellsCount;
    int deadShipsCount;
    int startPosX;
    int startPosY;
    int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS];
    int clickedCells[100][2];
    enum gameEntity gameEntity;
    SHIP ships[SHIPS_COUNT];
} GAME_AREA;


void incrementClickedCells(GAME_AREA* gameArea, int x, int y);

GAME_AREA initGameArea(int startPosX, int startPosY, enum gameEntity gameEntity);

int cellIsNotClicked(GAME_AREA* gameArea, int x, int y);

int getShipId(GAME_AREA* gameArea, int x, int y);

void decreaseShipHealth(GAME_AREA* gameArea, int ship_id);

int* checkCellPosition(GAME_AREA* gameArea, int x, int y);

int checkDeadShip(GAME_AREA* gameArea, int  ship_id);

