#pragma once
#include "controls.h"
#include "constants.h"

extern int pcTurn;
extern int userTurn;
extern int gameIsOn;
int msgBox;

#define START_X_USER_POSITION 150
#define START_Y_USER_POSITION 30
#define START_X_PC_POSITION 620
#define START_Y_PC_POSITION 30
#define CELL_SIZE 35
HDC hdc;
HBRUSH  NewBrush;
HINSTANCE gHInst;

HWND hUserCell[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS] = { NULL };
HWND hPCCell[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS] = { NULL };

void clearAllWindow(hWnd);

void openArea(struct gameArea* game_area, HWND hWnd, HWND hCell[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS])
{
    HBITMAP hCellImageChange;
    LPCWSTR imageName;

    for (int i = 1; i < AREA_SIZE_WITH_BORDERS - 1; i++)
    {
        for (int j = 1; j < AREA_SIZE_WITH_BORDERS - 1; j++)
        {
            hCell[i][j] = CreateWindowW(
                L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP,
                game_area->startPosX + CELL_SIZE * j,
                game_area->startPosY + CELL_SIZE * i,
                NULL, NULL, hWnd, NULL, NULL, NULL);
            if (game_area->area[i][j] == 2)
                imageName = OPEN_SHIP_CELL;
            else
                imageName = OPEN_EMPTY_CELL;
            hCellImageChange = (HBITMAP)LoadImageW(NULL, imageName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
            SendMessageW(hCell[i][j], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);
        }
    }
}

void loadDefaultImages(struct gameArea* game_area, HWND hWnd, HWND hCell[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS])
{
    HBITMAP hCellImage = (HBITMAP)LoadImageW(NULL, CLOSE_CELL, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
    for (int i = 1; i < AREA_SIZE_WITH_BORDERS - 1; i++)
    {
        for (int j = 1; j < AREA_SIZE_WITH_BORDERS - 1; j++)
        {
            hCell[i][j] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, game_area->startPosX + CELL_SIZE * j, game_area->startPosY + CELL_SIZE * i, 120, 120, hWnd, NULL, NULL, NULL);
            SendMessageW(hCell[i][j], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImage);
        }
    }
}

void changeOtherImage(struct gameArea* gameArea, HWND hCell[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS], HWND hWnd)
{
    HBITMAP hCellImageChange = (HBITMAP)LoadImageW(NULL, MISS_CELL, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
    int start_pos_x;
    int start_pos_y;
    start_pos_x = gameArea->startPosX;
    start_pos_y = gameArea->startPosY;
    for (int i = 1; i < AREA_SIZE_WITH_BORDERS; i++)
    {
        for (int j = 1; j < AREA_SIZE_WITH_BORDERS; j++)
        {
            if (gameArea->area[i][j] == 0)
            {
                if (i < AREA_SIZE_WITH_BORDERS - 1 && j < AREA_SIZE_WITH_BORDERS - 1 && i > 0 && j > 0) {
                    DestroyWindow(hCell[i][j]);
                    hCell[i][j] = CreateWindowW(
                        L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP,
                        start_pos_x + CELL_SIZE * i,
                        start_pos_y + CELL_SIZE * j,
                        NULL, NULL, hWnd, NULL, NULL, NULL);
                    SendMessageW(hCell[i][j], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);
                }
            }
        }
    }
}

void changeAroundImage(struct gameArea* gameArea, int shipId, HWND hWnd, HWND hCell[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS])
{
    HBITMAP hCellImage;

    for (int i = 0; i < gameArea->ships[shipId].deckCount * 2 + 6; i++)
    {
        int dead_x = gameArea->ships[shipId].aroundCoords[i][0];
        int dead_y = gameArea->ships[shipId].aroundCoords[i][1];
        hCellImage = (HBITMAP)LoadImageW(NULL, MISS_CELL, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);

        if (dead_x < AREA_SIZE_WITH_BORDERS - 1 && dead_y < AREA_SIZE_WITH_BORDERS - 1 && dead_x > 0 && dead_y > 0)
        {
            if (cellIsNotClicked(gameArea, dead_x, dead_y))
            {
                incrementClickedCells(gameArea, dead_x, dead_y);
                DestroyWindow(hCell[dead_y][dead_x]);
                hCell[dead_y][dead_x] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, gameArea->startPosX + CELL_SIZE * dead_x, gameArea->startPosY + CELL_SIZE * dead_y, NULL, NULL, hWnd, NULL, NULL, NULL);
                SendMessageW(hCell[dead_y][dead_x], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImage);
            }
        }
    }
}


int shootCell(struct gameArea* gameArea, HWND hCell[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS], int x, int y, HWND hWnd)
{
    HBITMAP hCellImage;
    LPCWSTR imageName;

    DestroyWindow(hCell[y][x]); // delete clicked cell
    incrementClickedCells(gameArea, x, y);
    gameArea->moveCounter++;
    if (gameArea->area[y][x] < 2)
        imageName = MISS_CELL;
    else
        imageName = HIT_CELL;
        
    hCellImage = (HBITMAP)LoadImageW(NULL, imageName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);

    hCell[y][x] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, gameArea->startPosX + CELL_SIZE * x, gameArea->startPosY + CELL_SIZE * y, NULL, NULL, hWnd, NULL, NULL, NULL);
    SendMessageW(hCell[y][x], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImage);

    if (gameArea->area[y][x] == 2)
    {
        int ship_id = getShipId(gameArea, x, y);
        decreaseShipHealth(gameArea, ship_id);

        if (checkDeadShip(gameArea, ship_id))
        {
            gameArea->deadShipsCount++;
            changeAroundImage(gameArea, ship_id, hWnd, hCell);
        }
        if (gameArea->deadShipsCount == 10)
        {
            gameIsOn = 2;
            if (gameArea->gameEntity == PC_ENTITY)
            {
                //change_other_image(game_area, hUserCell, hWnd);
                PlaySound(win_game_sound, NULL, SND_ASYNC | SND_FILENAME);
                msgBox = MessageBox(hWnd, TEXT("YOU'RE WINNER!  Would you retry?"),
                    TEXT("win window"), MB_OKCANCEL);

                gameIsOn = 0;
            }
            if (gameArea->gameEntity == USER_ENTITY)
            {
                //change_other_image(game_area, hPCCell, hWnd);
                PlaySound(gameOverSound, NULL, SND_ASYNC | SND_FILENAME);
                msgBox = MessageBox(NULL, TEXT("YOU'RE NOT WINNER! Would you retry?"),
                    TEXT("lose window"), MB_OKCANCEL);
                gameIsOn = 0;
            }


            switch (msgBox)
            {
            case IDOK:
                SendMessageW(hWnd, WM_COMMAND, NEW_GAME_WITH_PC, 0);
                break;
            case IDCANCEL:
                SendMessageW(hWnd, WM_DESTROY, 0, 0);
                break;
            }
        }
    }
    else {
        if (gameArea->gameEntity == PC_ENTITY) {
            pcTurn = 1;
            userTurn = 0;
        }
        else {
            pcTurn = 0;
            userTurn = 1;
            SetWindowText(hStaticLabel, L"Your turn! ");
        }

    }
    return gameArea->area[y][x];
}

void clearAllWindow() {
    for (int i = 1; i < AREA_SIZE_WITH_BORDERS - 1; i++)
    {
        for (int j = 1; j < AREA_SIZE_WITH_BORDERS - 1; j++) {
            DestroyWindow(hPCCell[i][j]);
            DestroyWindow(hUserCell[i][j]);
        }
    }
    RemoveGameControls();

}

void clearArea(HWND hCell[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS])
{
    for (int y = 1; y < AREA_SIZE_WITH_BORDERS - 1; y++)
    {
        for (int x = 1; x < AREA_SIZE_WITH_BORDERS - 1; x++)
        {
            DestroyWindow(hCell[y][x]);
        }
    }
}

