#pragma once
#include "gameArea.h"

int shipCellArray[10] = { 4,3,3,2,2,2,1,1,1,1 };

void incrementClickedCells(struct gameArea* gameArea, int x, int y)
{
    gameArea->clickedCells[gameArea->checkedCellsCount][0] = y;
    gameArea->clickedCells[gameArea->checkedCellsCount][1] = x;
    gameArea->checkedCellsCount++;
}

struct gameArea initGameArea(int startPosX, int startPosY, enum gameEntity gameEntity)
{
    gameArea.shipCounter = 0;
    gameArea.moveCounter = 0;
    gameArea.deadShipsCount = 0;
    gameArea.checkedCellsCount = 0;
    gameArea.startPosX = startPosX;
    gameArea.startPosY = startPosY;
    gameArea.gameEntity = gameEntity;

    for (int i = 0; i < AREA_SIZE_WITH_BORDERS; i++) // fill area zero
    {
        for (int j = 0; j < AREA_SIZE_WITH_BORDERS; j++)
        {
            gameArea.area[i][j] = 0;
        }
    }

    for (int i = 0; i < SHIPS_COUNT; i++)
    {
        gameArea.ships[i] = generateShip(gameArea.area, shipCellArray[i], i);
    }

    gameArea.shipCounter = 10;
    return gameArea;
}

int cellIsNotClicked(struct gameArea* gameArea, int x, int y)
{
    for (int i = 0; i < gameArea->checkedCellsCount; i++)
    {
        if (gameArea->clickedCells[i][0] == y && gameArea->clickedCells[i][1] == x)
            return 0;
    }
    return 1;
}

int getShipId(struct gameArea* gameArea, int x, int y)
{
    for (int i = 0; i < gameArea->shipCounter; i++)
    {
        for (int j = 0; j < gameArea->ships[i].deckCount; j++)
        {
            if (gameArea->ships[i].coords[j][0] == x && gameArea->ships[i].coords[j][1] == y)
            {
                return i;
            }
        }
    }

    return -1;
}

void decreaseShipHealth(struct gameArea* gameArea, int ship_id)
{
    gameArea->ships[ship_id].currentHealth = gameArea->ships[ship_id].currentHealth - 1;
}

int* checkCellPosition(struct gameArea* gameArea, int x, int y)
{
    int* arr = (int*)malloc(2);
    arr[0] = (int)(x - gameArea->startPosX) / CELL_SIZE;
    arr[1] = (int)(y - gameArea->startPosY) / CELL_SIZE;
    return arr;
}

int checkDeadShip(struct gameArea* gameArea, int  ship_id)
{
    if (gameArea->ships[ship_id].currentHealth == 0)
        return 1;
    else
        return 0;
}

