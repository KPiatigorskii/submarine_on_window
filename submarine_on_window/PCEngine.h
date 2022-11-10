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

void setDiagonalPriority(struct pcEngine* pcEngineArea,int x, int y);
void setHorVerPriority(struct pcEngine* pcEngineArea, int x, int y);
int* getBiggestPriority(struct pcEngine* pcEngineArea);
void clearShipCells(struct pcEngine* pcEngineArea);
void setPriorityWithPlane(struct pcEngine* pcEngineArea);
void findPlane(struct pcEngine* pcEngineArea);

struct pcEngine initPCLogic()
{
    struct pcEngine pcEngineArea;
    pcEngineArea.nextCell[0] = -1;
    pcEngineArea.nextCell[1] = -1;
    pcEngineArea.shotCounter = 0;
    pcEngineArea.hitCount = 0;
    pcEngineArea.deadShips = 0;
    pcEngineArea.shipPlane = 0;

    
    for (int i = 0; i < 12; i++) // fill negative nums
    {
        for (int j = 0; j < 12; j++)
        {
            pcEngineArea.priorityArea[i][j] = EMPTY_WEIGHT;
        }
    }
    clearShipCells(&pcEngineArea);
    return pcEngineArea;
}

enum shoot_directions getRandomDirectionShoot()
{
    time_t t;
    srand((unsigned)time(&t));
    enum shoot_directions direction;
    direction = (enum shoot_directions)(rand() % 3);
    return direction;
}

void setAroundPriority(struct pcEngine* pcEngineArea)
{
    for (int i = 0; i < pcEngineArea->hitCount; i++)
    {
        int y = pcEngineArea->shipCells[i][0];
        int x = pcEngineArea->shipCells[i][1];
        for (int i = y - 1; i < y + 2; i++)
        {
            for (int j = x - 1; j < x + 2; j++)
            {
                pcEngineArea->priorityArea[i][j] = COMPLETE_SHIP_WEIGHT;
            }
        }
    }
}

void SetNextCell(struct pcEngine* pcEngineArea, int hit_result, int x, int y, int deads)
{
    int* position_ptr;
    pcEngineArea->shotCounter++;
    switch (hit_result)
    {
        case 0:
        case 1:
             if (pcEngineArea->priorityArea[y][x] == 5)
                 pcEngineArea->priorityArea[y][x] = MISS_WEIGHT;
             if (pcEngineArea->priorityArea[y][x] == -9)
                 pcEngineArea->priorityArea[y][x] = MISS_WEIGHT;
             pcEngineArea->priorityArea[y][x] = MISS_WEIGHT;
            break;
        case 2: // we hit
            pcEngineArea->priorityArea[y][x] = HIT_WEIGHT;
            pcEngineArea->shipCells[pcEngineArea->hitCount][0] = y;
            pcEngineArea->shipCells[pcEngineArea->hitCount][1] = x;
            pcEngineArea->hitCount++;

            if (deads > pcEngineArea->deadShips)
            {
                pcEngineArea->deadShips++;
                setAroundPriority(pcEngineArea);
                clearShipCells(pcEngineArea);
                pcEngineArea->hitCount = 0;
                break;
            }

            switch (pcEngineArea->hitCount)
            {
                case 1: // first hit
                    setDiagonalPriority(pcEngineArea, x, y);
                    setHorVerPriority(pcEngineArea, x, y);
                    break;
                case 2: // 2nd hit
                    findPlane(pcEngineArea);
                case 3: // 3d hit
                    setPriorityWithPlane(pcEngineArea);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    position_ptr = getBiggestPriority(pcEngineArea);
    pcEngineArea->nextCell[0] = position_ptr[0];
    pcEngineArea->nextCell[1] = position_ptr[1];
}

void setPriorityWithPlane(struct pcEngine* pcEngineArea)
{
    for (int i = 0; i < pcEngineArea->hitCount; i++)
    {
        int x = pcEngineArea->shipCells[i][1];
        int y = pcEngineArea->shipCells[i][0];
        if (pcEngineArea->shipPlane == 0) // hor
        {
            pcEngineArea->priorityArea[y - 1][x] = MISS_WEIGHT;
            pcEngineArea->priorityArea[y + 1][x] = MISS_WEIGHT;
            if (pcEngineArea->priorityArea[y][x + 1] > HIT_WEIGHT)
                pcEngineArea->priorityArea[y][x + 1] = HIT_PREDICT_PLANE_WEIGHT;
            if (pcEngineArea->priorityArea[y][x - 1] > HIT_WEIGHT)
                pcEngineArea->priorityArea[y][x - 1] = HIT_PREDICT_PLANE_WEIGHT;
        }
        else 
        {
            pcEngineArea->priorityArea[y][x - 1] = MISS_WEIGHT;
            pcEngineArea->priorityArea[y][x + 1] = MISS_WEIGHT;
            if (pcEngineArea->priorityArea[y + 1][x] > HIT_WEIGHT)
                pcEngineArea->priorityArea[y + 1][x] = HIT_PREDICT_PLANE_WEIGHT;
            if (pcEngineArea->priorityArea[y - 1][x] > HIT_WEIGHT)
                pcEngineArea->priorityArea[y - 1][x] = HIT_PREDICT_PLANE_WEIGHT;
        }
    }
}

void findPlane(struct pcEngine* pcEngineArea)
{
    int x_dif = pcEngineArea->shipCells[0][1] - pcEngineArea->shipCells[1][1];
    int y_dif = pcEngineArea->shipCells[0][0] - pcEngineArea->shipCells[1][0];
    if (x_dif == 0) 
        pcEngineArea->shipPlane = 1; // vertical
    else
        pcEngineArea->shipPlane = 0; // horisontal
}

int* getNextCell(struct pcEngine* pcEngineArea)
{
    if (pcEngineArea->shotCounter == 0 || pcEngineArea->hitCount == 0) {
        int* coords = malloc(2);
	    coords[0] = rand() % 10 + 1;
	    coords[1] = rand() % 10 + 1;
	    return coords;
    }
    else
        return pcEngineArea->nextCell;
}

void setDiagonalPriority(struct pcEngine* pcEngineArea, int x, int y)
{
    if (pcEngineArea->priorityArea[y - 1][x - 1] >= EMPTY_WEIGHT)
        pcEngineArea->priorityArea[y - 1][x - 1] = COMPLETE_SHIP_WEIGHT;
    if (pcEngineArea->priorityArea[y + 1][x + 1] >= EMPTY_WEIGHT)
        pcEngineArea->priorityArea[y + 1][x + 1] = COMPLETE_SHIP_WEIGHT;
    if (pcEngineArea->priorityArea[y + 1][x - 1] >= EMPTY_WEIGHT)
        pcEngineArea->priorityArea[y + 1][x - 1] = COMPLETE_SHIP_WEIGHT;
    if (pcEngineArea->priorityArea[y - 1][x + 1] >= EMPTY_WEIGHT)
        pcEngineArea->priorityArea[y - 1][x + 1] = COMPLETE_SHIP_WEIGHT;
}

void setHorVerPriority(struct pcEngine* pcEngineArea, int x, int y)
{
    if (pcEngineArea->priorityArea[y - 1][x] >= EMPTY_WEIGHT)
        pcEngineArea->priorityArea[y - 1][x] = HIT_PREDICT_AROUND_WEIGHT;
    if (pcEngineArea->priorityArea[y][x - 1] >= EMPTY_WEIGHT)
        pcEngineArea->priorityArea[y][x - 1] = HIT_PREDICT_AROUND_WEIGHT;
    if (pcEngineArea->priorityArea[y + 1][x] >= EMPTY_WEIGHT)
        pcEngineArea->priorityArea[y + 1][x] = HIT_PREDICT_AROUND_WEIGHT;
    if (pcEngineArea->priorityArea[y][x + 1] >= EMPTY_WEIGHT)
        pcEngineArea->priorityArea[y][x + 1] = HIT_PREDICT_AROUND_WEIGHT;

}

int* getBiggestPriority(struct pcEngine* pcEngineArea)
{
    int biggerPriority = EMPTY_WEIGHT;
    int* biggerCell = malloc(2);

    for (int y_cell = 1; y_cell < 11; y_cell++)
    {
        for (int x_cell = 1; x_cell < 11; x_cell++)
        {
            int cell = pcEngineArea->priorityArea[y_cell][x_cell];
            if (cell > biggerPriority) {
                //* < 3 not shooted cells
                biggerPriority = pcEngineArea->priorityArea[y_cell][x_cell];
                biggerCell[1] = x_cell;
                biggerCell[0] = y_cell;
            }
        }
    }
    return biggerCell;
}

void clearShipCells(struct pcEngine* pcEngineArea)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            pcEngineArea->shipCells[i][j] = -1;
        }
    }
}
