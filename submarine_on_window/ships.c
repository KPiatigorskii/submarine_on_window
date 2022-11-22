#pragma once

#include "ships.h"

void printArea0(int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS])
{
    for (int i = 1; i < AREA_SIZE_WITH_BORDERS - 1; i++)
    {
        for (int j = 1; j < AREA_SIZE_WITH_BORDERS - 1; j++)
        {
            printf("%d ", area[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printShip(struct ship* newShip)
{
    printf("====================\n");
    printf("cells:\n");
    for (int count = 0; count < newShip->deckCount; count++)
    {
        printf("{x=%d, y=%d} ", newShip->coords[count][0], newShip->coords[count][1]);
    }
    printf("\n");
    printf("around cells:\n");
    for (int i = 0; i < newShip->deckCount * 2 + 6; i++)
    {
        printf("{x=%d, y=%d} ", newShip->coords[i][0], newShip->coords[i][1]);
    }
    printf("\n====================\n");
}

int checkCellBorders(int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS], int x, int y)
{
    if (area[y][x] > 10 ||
        area[y][x] > 10 ||
        area[y][x] < 1 ||
        area[y][x] < 1)
        return 0;
    return 1;
}

int checkBorders(struct ship* newShip)
{
    for (int count = 0; count < newShip->deckCount; count++)
    {
        if (newShip->coords[count][0] > 10 || newShip->coords[count][1] > 10 || newShip->coords[count][1] < 1 || newShip->coords[count][1] < 1)
            return 0;
    }
    return 1;
}

int cellInShip(struct ship* newShip, int i, int j)
{
    for (int count = 0; count < newShip->deckCount; count++)
    {
        if (newShip->coords[count][0] == j && newShip->coords[count][1] == i)
            return 1;
    }
    return 0;
}

void fillArea(struct ship* newShip, int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS])
{
    for (int i = 0; i < newShip->deckCount; i++)
    {
        area[newShip->coords[i][1]][newShip->coords[i][0]] = 2;
    }
    for (int i = 0; i < newShip->deckCount * 2 + 6; i++)
    {
        area[newShip->aroundCoords[i][1]][newShip->aroundCoords[i][0]] = 1;
    }

}

int setAroundCoordsForShip(struct ship* newShip, int multipliers[2], int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS])
{
    int startX = newShip->coords[0][0];
    int startY = newShip->coords[0][1];
    int stopX = newShip->coords[newShip->deckCount - 1][0];
    int stopY = newShip->coords[newShip->deckCount - 1][1];
    int addX = multipliers[0];
    int addY = multipliers[1];
    int aroundCoordsCounter = 0;

    for (int i = startY - addY; i != stopY + 2 * addY; i = i + addY) {
        for (int j = startX - addX; j != stopX + 2 * addX; j = j + addX) {
            if (area[i][j] != 2)
            {
                if (!cellInShip(newShip, i, j))
                {
                    newShip->aroundCoords[aroundCoordsCounter][0] = j;
                    newShip->aroundCoords[aroundCoordsCounter][1] = i;
                    //printf("add to around coords: %d,%d\n", i, j);
                    aroundCoordsCounter = aroundCoordsCounter + 1;
                }
            }
            else {
                newShip->isValid = 0;
                return 0;
            }
        }

    }
    return 1;
}

int setAllCoordsForShip(int startX, int startY, int* multipliers, enum ship_directions direction, struct ship* newShip, int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS])
{
    for (int i = 0; i < newShip->deckCount; i++)
    {
        switch (direction) {
        case UP:
            newShip->coords[i][1] = startY - i; // y
            newShip->coords[i][0] = startX; // x
            break;
        case RIGHT:
            newShip->coords[i][1] = startY;
            newShip->coords[i][0] = startX + i;
            break;
        case DOWN:
            newShip->coords[i][1] = startY + i;
            newShip->coords[i][0] = startX;
            break;
        case LEFT:
            newShip->coords[i][1] = startY;
            newShip->coords[i][0] = startX - i;
            break;
        default:
            break;
        }
        if (newShip->coords[i][1] > 10 ||
            newShip->coords[i][0] > 10 ||
            newShip->coords[i][1] < 1 ||
            newShip->coords[i][0] < 1 ||
            area[newShip->coords[i][1]][newShip->coords[i][0]] == 2)
        {
            newShip->isValid = 0;
            return 0;
        }

    }
    return 1;
}

int* getMultipliers(enum ship_directions direction)
{
    int* dirArr = (int*)malloc(2);
    switch (direction) {
    case UP:
        dirArr[0] = 1;
        dirArr[1] = -1;
        break;
    case RIGHT:
        dirArr[0] = 1;
        dirArr[1] = 1;
        break;
    case DOWN:
        dirArr[0] = 1;
        dirArr[1] = 1;
        break;
    case LEFT:
        dirArr[0] = -1;
        dirArr[1] = 1;
        break;

    default:
        dirArr[0] = 1;
        dirArr[1] = -1;
        break;
    }
    return dirArr;
}

enum shipDirections getRandomDirection()
{
    enum shipDirections direction = (rand() % 3);
    return direction;
}

int isNeighborsFree(int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS], int startX, int startY) {
    for (int i = startY - 1; i < startY + 2; i++)
    {
        for (int j = startX - 1; j < startX + 2; j++)
        {
            //printf("check cell x = %d, y = %d \n", j,i);
            if (area[i][j] == 2) {
                //printf("BUSTED\n");
                return 0;
            }
        }
    }
    return 1;
}

int* getFreeCell(int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS])
{
    int* arr = (int*)malloc(2);
    int busyCellFlag = 0;
    while (1) {
        arr[0] = rand() % 9 + 1;
        arr[1] = rand() % 9 + 1;
        //printf("Suggected cell: %d %d\n", arr[0], arr[1]);
        return arr;
    }
}

int checkAllCellIsValid(struct ship* newShip, int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS])
{
    for (int i = 0; i < newShip->deckCount; i++)
    {
        if (isNeighborsFree(area, newShip->coords[i][0],
            newShip->coords[i][1]) &&
            newShip->coords[i][1] < AREA_SIZE_WITH_BORDERS - 1 &&
            newShip->coords[i][0] < AREA_SIZE_WITH_BORDERS - 1 &&
            newShip->coords[i][1] > 0 &&
            newShip->coords[i][0] > 0)
        {
            return 1;
        }
    }
    return 0;
}

struct ship generateShip(int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS], int deckCount, int id)
{
    struct ship newShip;
    int startX, startY;
    enum ship_directions direction;
    int* startCoordinates;
    int* multipliers;

    newShip.deckCount = deckCount;

    time_t t;
    srand((unsigned)time(&t));

    while (1)
    {
        newShip.isValid = 1;
        newShip.id = id;
        newShip.currentHealth = deckCount;
        newShip.maxHealth = deckCount;
        startCoordinates = getFreeCell(area); // find free cell
        direction = getRandomDirection();
        multipliers = getMultipliers(direction); // first x, second y
        startX = startCoordinates[0];
        startY = startCoordinates[1];
        //if (!is_neighbors_free(area, start_x, start_y))
        //    continue;
        printf("direction is %d\n", direction);
        if (!setAllCoordsForShip(startX, startY, multipliers, direction, &newShip, area))
            continue;
        if (!checkBorders(&newShip))
            continue;
        if (!checkAllCellIsValid(&newShip, area))
            continue;
        if (!setAroundCoordsForShip(&newShip, multipliers, area))
            continue;
        printShip(&newShip);
        if (newShip.isValid)
        {
            fillArea(&newShip, area);
            //print_area_0(area);
            break;
        }

    }
    return newShip;
}
