#pragma once
#include "ships.h"

#define AREA_SIZE_WITH_BORDERS 12
#define CELL_SIZE 35

enum gameEntity { USER_ENTITY = 0, PC_ENTITY = 1 };

struct gameArea
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
    struct ship ships[SHIPS_COUNT];
} gameArea;


void incrementClickedCells(struct gameArea* gameArea, int x, int y);

struct gameArea initGameArea(int startPosX, int startPosY, enum gameEntity gameEntity);

int cellIsNotClicked(struct gameArea* gameArea, int x, int y);

int getShipId(struct gameArea* gameArea, int x, int y);

void decreaseShipHealth(struct gameArea* gameArea, int ship_id);

int* checkCellPosition(struct gameArea* gameArea, int x, int y);

int checkDeadShip(struct gameArea* gameArea, int  ship_id);

