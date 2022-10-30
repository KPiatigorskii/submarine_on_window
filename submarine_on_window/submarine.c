#pragma comment (lib , "winmm.lib")
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

struct pcEngine pcEngineStruct;
struct gameArea gameUserArea;
struct gameArea gamePCArea;

int shipsIsGenerated = 0;
int gameIsOn = 0;
int pcTurn = 0;
int userTurn = 0;

LRESULT CALLBACK WindProc(HWND, UINT, WPARAM, LPARAM);

HWND hLogo;

HWND hUserArea, hPCArea, hWndExample, hWndTurn;

HBITMAP hLogoImage, hCellUserImage[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS];
HBITMAP hCellPCImage[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS], hGenerateImage;
HBITMAP hBitmap;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    int mouseX, mouseY;
    int cellX, cellY;

    switch (msg)
    {
    case WM_TIMER:
        if (pcTurn && gameIsOn == 1)
        {
            int shotResult;
            SetWindowText(hStaticLabel, TEXT("PC turn! "));
            Sleep(500);
            while (1) {

                int *ptr = getNextCell(&pcEngineStruct);
                cellY = ptr[0];
                cellX = ptr[1];

                if (cellIsNotClicked(&gameUserArea, cellX, cellY)) {
                    break;
                }
            }
            shotResult = shootCell(&gameUserArea, hUserCell, cellX, cellY, hWnd);
            SetNextCell(&pcEngineStruct, shotResult, cellX, cellY, gameUserArea.deadShipsCount);
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
                    shootCell(&gamePCArea, hPCCell, cellX, cellY, hWnd);
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
            userTurn = 0;
            pcTurn = 0;
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
        case GENERATE_SHIPS:
            if (!gameIsOn)
            {
                clearArea(hUserCell);
                gameUserArea = initGameArea(START_X_USER_POSITION, START_Y_USER_POSITION, USER_ENTITY);
                openArea(&gameUserArea, hWnd, hUserCell);
                //clearArea(hPCCell); // uncomment for debug and see all PC ships
                //openArea(&game_pc_area, hWnd, &hPCCell); // uncomment for debug and see all PC ships
                shipsIsGenerated = 1;
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




