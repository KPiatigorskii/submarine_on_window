#pragma once
#include <windows.h>
#include <windowsx.h>
#include "gameArea.h"
#include "controls.h"
#include "constants.h"

extern int pcTurn;
extern int userTurn;
extern int gameIsOn;
extern int gameType;
extern HWND hStaticLabel;
int msgBox;

#define START_X_USER_POSITION 150
#define START_Y_USER_POSITION 30
#define START_X_PC_POSITION 620
#define START_Y_PC_POSITION 30
#define START_X_PC2_POSITION 150
#define START_Y_PC2_POSITION 30
#define CELL_SIZE 35
HDC hdc;
HBRUSH  NewBrush;
HINSTANCE gHInst;

HWND hUserCell[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS];
HWND hPCCell[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS];
HWND hPC2Cell[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS];

void clearAllWindow(hWnd);

void openArea(GAME_AREA* game_area, HWND hWnd, HWND hCell[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS]);

void loadDefaultImages(GAME_AREA* game_area, HWND hWnd, HWND hCell[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS]);

void changeOtherImage(GAME_AREA* gameArea, HWND hCell[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS], HWND hWnd);

void changeAroundImage(GAME_AREA* gameArea, int shipId, HWND hWnd, HWND hCell[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS]);

int shootCell(GAME_AREA* gameArea, HWND hCell[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS], int x, int y, HWND hWnd, HWND hStaticLabel);

void clearAllWindow();

void clearArea(HWND hCell[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS]);
