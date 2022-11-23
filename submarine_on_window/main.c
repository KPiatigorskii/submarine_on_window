﻿#pragma comment (lib , "winmm.lib")
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mmsystem.h>
#include "gameArea.h"
#include "draw.h"
#include "PCEngine.h"

#define ID_TIMER 1
#define IDB_BITMAP1 101

PC_ENGINE pcEngineStruct;
PC_ENGINE pc2EngineStruct;
GAME_AREA gameUserArea;
GAME_AREA gamePCArea;
GAME_AREA gamePC2Area;

int shipsIsGenerated = 0;
int gameIsOn = 0; // 0 - no game, 1 - user vs pc, 2 - pc vs pc
int pcTurn = 0; // 1 for pc in pc_vs_user and 1st pc for pc_vs_pc, 2 - 2nd pc in pc_vs_pc
int userTurn = 0;
int gameType = 0; // 0 - user vs pc, 1 - pc vs pc

LRESULT CALLBACK WindProc(HWND, UINT, WPARAM, LPARAM);

HWND hLogo;

HWND hUserArea, hPCArea, hPC2Area, hWndExample, hWndTurn;

HBITMAP hCellUserImage[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS];
HBITMAP hCellPCImage[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS];
HBITMAP hCellPC2Image[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS];
HBITMAP hLogoImage, hGenerateImage;
HBITMAP hBitmap;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    int mouseX, mouseY;
    int cellX, cellY;
    int* nextCellPtr;
    int shotResult;

    switch (msg)
    {
    case WM_TIMER:
        if (gameType == 0 && pcTurn && gameIsOn == 1)
        {
            //SetWindowText(hStaticLabel, TEXT("PC turn! "));
            Sleep(500);
            while (1) {

                nextCellPtr = getNextCell(&pcEngineStruct);
                cellY = nextCellPtr[0];
                cellX = nextCellPtr[1];

                if (cellIsNotClicked(&gameUserArea, cellX, cellY)) {
                    break;
                }
            }
            shotResult = shootCell(&gameUserArea, hUserCell, cellX, cellY, hWnd, hStaticLabel);
            SetNextCell(&pcEngineStruct, shotResult, cellX, cellY, gameUserArea.deadShipsCount);
        }
        if (gameType == 1 && gameIsOn == 1) {

            if (pcTurn == 1) { // pc #1 turn
                //SetWindowText(hStaticLabel, TEXT("PC #1 turn! "));
                Sleep(500);
                while (1) {

                    nextCellPtr = getNextCell(&pcEngineStruct);
                    cellY = nextCellPtr[0];
                    cellX = nextCellPtr[1];

                    if (cellIsNotClicked(&gamePC2Area, cellX, cellY)) {
                        break;
                    }
                }
                shotResult = shootCell(&gamePC2Area, hPC2Cell, cellX, cellY, hWnd, hStaticLabel);
                SetNextCell(&pcEngineStruct, shotResult, cellX, cellY, gamePC2Area.deadShipsCount);
            }
            else {  // pc #2 turn
                //SetWindowText(hStaticLabel, TEXT("PC #2 turn! "));
                Sleep(500);
                while (1) {

                    nextCellPtr = getNextCell(&pc2EngineStruct);
                    cellY = nextCellPtr[0];
                    cellX = nextCellPtr[1];

                    if (cellIsNotClicked(&gamePCArea, cellX, cellY)) {
                        break;
                    }
                }
                shotResult = shootCell(&gamePCArea, hPCCell, cellX, cellY, hWnd, hStaticLabel);
                SetNextCell(&pc2EngineStruct, shotResult, cellX, cellY, gamePCArea.deadShipsCount);
            }
        }


    case WM_LBUTTONUP:
        if (gameIsOn == 1) {
            mouseX = GET_X_LPARAM(lParam);
            mouseY = GET_Y_LPARAM(lParam);

            int* positionPtr;

            positionPtr = checkCellPosition(&gamePCArea, mouseX, mouseY);
            cellX = positionPtr[0];
            cellY = positionPtr[1];

            if (cellX < AREA_SIZE_WITH_BORDERS - 1 && cellX >= 1 && cellY >= 1 && cellY < AREA_SIZE_WITH_BORDERS - 1) {
                if (cellIsNotClicked(&gamePCArea, cellX, cellY))
                {
                    shootCell(&gamePCArea, hPCCell, cellX, cellY, hWnd, hStaticLabel);
                }
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
        switch (wParam)
        {
            case NEW_GAME_WITH_PC:
                PlaySound(NULL, NULL, 0);
                shipsIsGenerated = 0;
                gameIsOn = 0;
                userTurn = 1;
                pcTurn = 0;
                gameType = 0;
                AddGameControls(hWnd);
                pcEngineStruct = initPCLogic();
                gamePCArea = initGameArea(START_X_PC_POSITION, START_Y_PC_POSITION, PC_ENTITY);
                gameUserArea = initGameArea(START_X_USER_POSITION, START_Y_USER_POSITION, USER_ENTITY);
                DestroyWindow(hStaticLabel);
                clearArea(hPCCell);
                loadDefaultImages(&gamePCArea, hWnd, hPCCell);

                clearArea(hUserCell);
                loadDefaultImages(&gameUserArea, hWnd, hUserCell);
                break;
            case NEW_GAME_PC_VS_PC:
                PlaySound(NULL, NULL, 0);
                shipsIsGenerated = 0;
                gameIsOn = 0;
                userTurn = 0;
                pcTurn = 0;
                gameType = 1;
                AddGameControls(hWnd);

                pcEngineStruct = initPCLogic();
                pc2EngineStruct = initPCLogic();
                gamePCArea = initGameArea(START_X_PC_POSITION, START_Y_PC_POSITION, PC_ENTITY);
                gamePC2Area = initGameArea(START_X_PC2_POSITION, START_Y_PC2_POSITION, PC2_ENTITY);
                DestroyWindow(hStaticLabel);
                clearArea(hPCCell);
                loadDefaultImages(&gamePCArea, hWnd, hPCCell);

                clearArea(hPC2Cell);
                loadDefaultImages(&gamePC2Area, hWnd, hPC2Cell);
                break;
            case GENERATE_SHIPS:

                if (!gameIsOn)
                {
                    switch (gameType)
                    {
                    case 0:
                        clearArea(hUserCell);
                        gameUserArea = initGameArea(START_X_USER_POSITION, START_Y_USER_POSITION, USER_ENTITY);
                        openArea(&gameUserArea, hWnd, hUserCell);
                        //clearArea(hPCCell); // uncomment for debug and see all PC ships
                        //openArea(&gamePCArea, hWnd, &hPCCell); // uncomment for debug and see all PC ships
                        shipsIsGenerated = 1;
                        break;
                    case 1:
                        clearArea(hPCCell);
                        clearArea(hPC2Cell);

                        gamePCArea = initGameArea(START_X_PC_POSITION, START_Y_PC_POSITION, PC_ENTITY);
                        openArea(&gamePCArea, hWnd, hPCCell);
                        Sleep(1100);
                        gamePC2Area = initGameArea(START_X_PC2_POSITION, START_Y_PC2_POSITION, PC2_ENTITY);
                        openArea(&gamePC2Area, hWnd, hPC2Cell);
                        //clearArea(hPCCell); // uncomment for debug and see all PC ships
                        //openArea(&gamePCArea, hWnd, &hPCCell); // uncomment for debug and see all PC ships
                        shipsIsGenerated = 1;
                        break;
                    default:
                        break;
                    }
                }
                break;
            case START_GAME:
                if (!gameIsOn && shipsIsGenerated)
                {
                    PlaySound(startGameSound, NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
                    SetTimer(hWnd, ID_TIMER, 250, NULL);
                    gameIsOn = 1;
                    hStaticLabel = CreateWindow(TEXT("Static"), TEXT("Your turn"), WS_CHILD | WS_VISIBLE | SS_CENTER, 535, 15, 120, 25, hWnd, 0, gHInst, 0);
                    RemoveGameControlsBeforeStart();
                    pcTurn = 0;
                    userTurn = 1;
                }
                break;
            case FILE_MENU_EXIT:
                DestroyWindow(hWnd);
                break; 
            default:
                break;
        }
    case WM_CREATE:
        AddMenus(hWnd);
        break;
    case 4:
        break;
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSW wc = { 0 };

    wc.hbrBackground = CreatePatternBrush((HBITMAP)LoadImageW(NULL, BACKGROUND_PATH, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("myWindowClass");
    wc.lpfnWndProc = WndProc;
    wc.style = CS_DBLCLKS;
    
    if (!RegisterClassW(&wc))
        return -1;

    MSG msg = { 0 };

    CreateWindowW( TEXT("myWindowClass"), TEXT("Submarine"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 1200, 600, NULL, NULL, NULL, NULL);

    while (GetMessage((&msg), (UINT_PTR)NULL, (UINT_PTR)NULL, (UINT_PTR)NULL))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}




