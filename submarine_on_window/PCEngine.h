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

struct pcEngine {
    int priorityArea[12][12];
    int nextCell[2]; // y,x
    int hitCount;
    int shotCounter;
    int deadShips;
    int shipPlane;
    int shipCells[4][2];
};

void test(int x);

struct pcEngine initPCLogic();

enum shoot_directions getRandomDirectionShoot();

void setAroundPriority(struct pcEngine* pcEngineArea);

void SetNextCell(struct pcEngine* pcEngineArea, int hit_result, int x, int y, int deads);

void setPriorityWithPlane(struct pcEngine* pcEngineArea);

void findPlane(struct pcEngine* pcEngineArea);

int* getNextCell(struct pcEngine* pcEngineArea);

void setDiagonalPriority(struct pcEngine* pcEngineArea, int x, int y);

void setHorVerPriority(struct pcEngine* pcEngineArea, int x, int y);

int* getBiggestPriority(struct pcEngine* pcEngineArea);

void clearShipCells(struct pcEngine* pcEngineArea);