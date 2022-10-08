#pragma comment (lib , "winmm.lib")

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "game_area.h"
#include "game_flow.h"
#include "draw.h"
#include "controls.h"
#include <mmsystem.h>

#define ID_TIMER 1
#define IDB_BITMAP1 101

static  HWND hStaticLabel;

HDC          hdc;
HBRUSH  NewBrush;
HINSTANCE g_hInst;

struct game_area game_user_area;
struct game_area game_pc_area;

int ships_is_generated = 0;
int game_is_on = 0;
int try_counter = 0;
int dead_ships_count = 0;
int pc_turn = 0;
int user_turn = 0;



LRESULT CALLBACK WindProc(HWND, UINT, WPARAM, LPARAM);

HWND hLogo;
HWND hUserCell[12][12] = { NULL };
HWND hPCCell[12][12] = {NULL};
HWND hUserArea, hPCArea, hWndExample, hWndTurn;


HBITMAP hLogoImage, hCellUserImage[12][12], hCellPCImage[12][12], hGenerateImage;
HBITMAP hBitmap;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    int mouse_x;
    int mouse_y;
    static TCHAR* intro_sound = TEXT("intro_sound.wav");
    static TCHAR* start_game_sound = TEXT("start_game.wav");
    static TCHAR* win_game_sound = TEXT("win_game.wav");
    static TCHAR* game_over_sound = TEXT("game_over.wav");


    switch (msg)
    {
    case WM_TIMER:
        if (pc_turn) 
        {
            SetWindowText(hStaticLabel, L"PC turn! ");
            Sleep(1000);
            PCHitCell(&game_user_area, hWnd, hStaticLabel, hUserCell);

        }
        break;
    case WM_LBUTTONUP:
        if (game_is_on == 1) {

            mouse_x = GET_X_LPARAM(lParam);
            mouse_y = GET_Y_LPARAM(lParam);

            int cell_x, cell_y;
            int* position_ptr;
            position_ptr = checkCellPosition(&game_pc_area, mouse_x, mouse_y);
            int arr[2];
            cell_x = *(position_ptr);
            cell_y = *(position_ptr + 1);
            if (cell_x < 11 && cell_x >= 1 && cell_y >= 1 && cell_y < 11) {
                if (cell_is_not_clicked(&game_pc_area, cell_x, cell_y))
                {
                    DestroyWindow(hPCCell[cell_y][cell_x]);
                    game_pc_area.clicked_cells[game_pc_area.move_counter][0] = cell_y;
                    game_pc_area.clicked_cells[game_pc_area.move_counter][1] = cell_x;
                    game_pc_area.move_counter++;
                    game_pc_area.checked_cells_count++;
                    if (hitCell(&game_pc_area, hPCCell, cell_x, cell_y, hWnd) == 2)
                    {
                        pc_turn = 0;
                        user_turn = 1;
                    }
                    else
                    {
                        pc_turn = 1;
                        user_turn = 0;
                    }
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
            ships_is_generated = 0;
            game_is_on = 0;
            user_turn = 0;
            pc_turn = 0;
            AddGameControls(hWnd);
            clear_area(hPCCell);
            clear_area(hUserCell);
            game_pc_area = init_game_area(500, 30, 1);
            loadDefaultImages(hWnd, &hPCCell);
            break;
        case GENERATE_SHIPS:
            if (!game_is_on)
            {
                clear_area(hUserCell);
                game_user_area = init_game_area(30, 30, 0);
                open_area(&game_user_area, hWnd, &hUserCell);
                ships_is_generated = 1;
            }
            break;
        case START_GAME:
            if (!game_is_on && ships_is_generated)
            {
                PlaySound(start_game_sound, NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
                SetTimer(hWnd, ID_TIMER, 200, NULL);
                game_is_on = 1;
                hStaticLabel = CreateWindow(L"Static", L"Your turn", WS_CHILD | WS_VISIBLE, 420, 15, 175, 25, hWnd, 0, g_hInst, 0);
                ShowWindow(GetDlgItem(hGenerateButton, GENERATE_SHIPS), SW_HIDE);
            }
            break;
        case FILE_MENU_EXIT:
            DestroyWindow(hWnd);
            break; 
        default:
            break;
        }
    case WM_CREATE:
        //PlaySound(intro_sound, NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
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

    wc.hbrBackground = CreatePatternBrush((HBITMAP)LoadImageW(NULL, (LPCWSTR)L"bg.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInstance;
    wc.lpszClassName = L"myWindowClass";
    wc.lpfnWndProc = WndProc;
    wc.style = CS_DBLCLKS;
    
    if (!RegisterClassW(&wc))
        return -1;

    MSG msg = { 0 };

    CreateWindowW(L"myWindowClass", L"My window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 1200, 600, NULL, NULL, NULL, NULL);

    while (GetMessage((&msg), NULL, NULL, NULL))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}




