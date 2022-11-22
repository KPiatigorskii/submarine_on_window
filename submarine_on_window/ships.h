#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define MAX_CELLS_IN_AREA 10
#define MIN_CELLS_IN_AREA 1
#define SHIPS_COUNT 10
#define MAX_DECK_IN_SHIP 4

#define AREA_SIZE_WITH_BORDERS 12

enum shipDirections { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 };

struct ship
{
    int coords[4][2];
    int aroundCoords[18][2];
    int isValid; // if not zero then true
    int id;
    int maxHealth;
    int currentHealth;
    int aroundSize;
    int deckCount;
};

void printArea0(int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS]);

void printShip(struct ship* newShip);

int checkCellBorders(int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS], int x, int y);

int checkBorders(struct ship* newShip);

void fillArea(struct ship* newShip, int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS]);

int setAroundCoordsForShip(struct ship* newShip, int multipliers[2], int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS]);

int setAllCoordsForShip(int startX, int startY, int* multipliers, enum ship_directions direction, struct ship* newShip, int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS]);

int* getMultipliers(enum ship_directions direction);

enum shipDirections getRandomDirection();

int isNeighborsFree(int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS], int startX, int startY);

int* getFreeCell(int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS]);

int checkAllCellIsValid(struct ship* newShip, int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS]);

struct ship generateShip(int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS], int deckCount, int id);

