#pragma once
#include <stdlib.h>
#include <time.h>

#include "gameArea.h"

// at start all cells has weight 0
// when we miss we set -1 weight
// when we hit 
// 1) ???
// 2) set weight 3 in current cell
// 3) set weight -1 for diagonal cells
// 4) set weight 2 for horizontal and vertical IF they not 3 
// =================================================
// init_pc_logic at start game
// get_next_cell when we hit
// hit_cell
// 
// 3 states of shooting
// 1) search (random shooting)
// 2) check direction shooting (when we hit first time we must check - ship position is horizontal or vertical) 
// 3) shoot other cells of ship


#define COMPLETE_SHIP_WEIGHT -3
#define MISS_WEIGHT -2
#define HIT_WEIGHT -1
#define EMPTY_WEIGHT 0
#define HIT_PREDICT_AROUND_WEIGHT 1
#define HIT_PREDICT_PLANE_WEIGHT 2

 typedef struct pcEngine {
    int priorityArea[12][12];
    int nextCell[2]; // y,x
    int hitCount;
    int shotCounter;
    int deadShips;
    int shipPlane;
    int shipCells[4][2];
} PC_ENGINE;

void test(int x);

PC_ENGINE initPCLogic();

enum shoot_directions getRandomDirectionShoot();

void setAroundPriority(PC_ENGINE* pcEngineArea);

void SetNextCell(PC_ENGINE* pcEngineArea, int hit_result, int x, int y, int deads);

void setPriorityWithPlane(PC_ENGINE* pcEngineArea);

void findPlane(PC_ENGINE* pcEngineArea);

int* getNextCell(PC_ENGINE* pcEngineArea);

void setDiagonalPriority(PC_ENGINE* pcEngineArea, int x, int y);

void setHorVerPriority(PC_ENGINE* pcEngineArea, int x, int y);

int* getBiggestPriority(PC_ENGINE* pcEngineArea);

void clearShipCells(PC_ENGINE* pcEngineArea);