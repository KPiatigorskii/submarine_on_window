#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "game_area.h"

#define FILE_MENU_NEW 1
#define TEST 2
#define FILE_MENU_EXIT 3
#define NEW_GAME 4
#define NEW_GAME_WITH_PC 5

struct game_area game_area;
struct game_area game_pc_area;
int* checkCellPosition(int, int);
void hitCell(struct game_area*,int, int, HWND);
void changeImage(struct game_area*, int, int, int, HWND);
void open_area(struct game_area, int, int, HWND);
void print_empty_area(int, int, HWND);
int game_is_on = 0;
int try_counter = 0;
int dead_ships_count = 0;

void AddMenus(HWND);
void AddControls(HWND);
void loadDefaultImages();



LRESULT CALLBACK WindProc(HWND, UINT, WPARAM, LPARAM);

HWND hLogo, hCell[12][12];

HMENU hMenu;
HBITMAP hLogoImage, hCellImage[12][12], hGenerateImage;
HBITMAP hBitmap;

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    int mouse_x;
    int mouse_y;
    //struct game_area game_area;
    //game_area = init_game_area();
    switch (msg)
    {
    case WM_LBUTTONUP:
        if (game_is_on == 1) {
            game_area.move_counter++;
            mouse_x = GET_X_LPARAM(lParam);
            mouse_y = GET_Y_LPARAM(lParam);
            int cell_x, cell_y;
            int* position_ptr;
            position_ptr = checkCellPosition(mouse_x, mouse_y);
            int arr[2];
            cell_x = *(position_ptr);
            cell_y = *(position_ptr + 1);
            if (cell_x < 11 && cell_x >= 1 && cell_y >= 1 && cell_y < 11)
                DestroyWindow(hCell[cell_y][cell_x]);
            hitCell(&game_area, cell_x, cell_y, hWnd);
            }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
        switch (wParam)
        {
        case TEST:
            //clicker_test();
            break;
        case NEW_GAME:
            
            game_area = init_game_area();
            loadDefaultImages();
            game_is_on = 1;
            break;
        case NEW_GAME_WITH_PC:
            game_is_on = 1;
            game_area = init_game_area();
            loadDefaultImages();
            game_pc_area = init_game_area();
            print_empty_area(30,30, hWnd);
            //open_area(game_area, 30, 30, hWnd);
            break;
        case FILE_MENU_EXIT:
            DestroyWindow(hWnd);
            break; 
        default:
            break;
        }
    case WM_CREATE:
        AddMenus(hWnd);
        AddControls(hWnd);
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

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
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

void AddMenus(HWND hWnd)
{
    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();

    HMENU hSubMenu = CreateMenu();

    AppendMenu(hSubMenu, MF_STRING, NULL, L"SubMenu");

    AppendMenu(hFileMenu, MF_STRING, NEW_GAME, L"New game");
    AppendMenu(hFileMenu, MF_STRING, NEW_GAME_WITH_PC, L"New game with PC");
    AppendMenu(hFileMenu, MF_STRING, TEST, L"Test game");
    AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR)hSubMenu, L"Open Sub");
    AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, L"Exit");


    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hFileMenu, L"File");
    AppendMenu(hMenu, MF_STRING, 2, L"Help");
    SetMenu(hWnd, hMenu);
}

void AddControls(HWND hWnd)
{
    //CreateWindowW(L"static", L"Enter text here :", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 100, 120, 20, hWnd, NULL, NULL, NULL);
    //HWND hBut = CreateWindowW(L"Button", L"Generate", WS_VISIBLE | WS_CHILD, 200, 100, 120, 20, hWnd, NULL, NULL, NULL);
    
    //HWND hBut2 = CreateWindowW(L"Button", L"Generate", WS_VISIBLE | WS_CHILD, 200, 150, 80, 80, hWnd, (HMENU) 4, NULL, NULL);
    //CreateWindowW(L"edit", L"...", WS_VISIBLE | WS_CHILD, 200, 122, 120, 50, hWnd, NULL, NULL, NULL);
    int start_pos_x = 500;
    int start_pos_y = 30;
    for (int i = 1; i < 11; i++)
    {
        for (int j = 1; j < 11; j++)
        {
            hCell[i][j] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, start_pos_x+35*j, start_pos_y+35*i, 120, 120, hWnd, NULL, NULL, NULL);
            SendMessageW(hCell[i][j], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImage[i][j]);
        }
    }
}

void loadDefaultImages()
{

    for (int i = 1; i < 11; i++)
    {
        for (int j = 1; j < 11; j++)
        {
        hCellImage[i][j] = (HBITMAP)LoadImageW(NULL, L"square32red.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
        }
    }
}

int* checkCellPosition(int x, int y)
{
    int arr[2];
    int start_pos_x = 500;
    int start_pos_y = 30;
    arr[0] = (int)(x - start_pos_x) / 35;
    arr[1] = (int)(y - start_pos_y) / 35;
    return arr;
}

void hitCell(struct game_area *game_area, int x, int y, HWND hWnd)
{
    int start_pos_x = 500;
    int start_pos_y = 30;
    changeImage(game_area, x, y, game_area->area[y][x], hWnd);

}

void changeImage(struct game_area *game_area, int x, int y, int cell, HWND hWnd)
{
    HWND hCellChange;
    HBITMAP hCellImageChange;
    int start_pos_x = 500;
    int start_pos_y = 30;

    switch (cell)
    {
    case 0:
        hCell[y][x] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, start_pos_x + 35 * x, start_pos_y + 35 * y, NULL, NULL, hWnd, NULL, NULL, NULL);
        hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32dot.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
        SendMessageW(hCell[y][x], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);
        break;

    case 1:
        hCell[y][x] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, start_pos_x + 35 * x, start_pos_y + 35 * y, NULL, NULL, hWnd, NULL, NULL, NULL);
        hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32dot.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
        SendMessageW(hCell[y][x], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);
        break;

    case 2:
        hCell[y][x] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, start_pos_x + 35 * x, start_pos_y + 35 * y, NULL, NULL, hWnd, NULL, NULL, NULL);
        hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32cross.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
        SendMessageW(hCell[y][x], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);
        int ship_id = -1;
        for (int i = 0; i < game_area->ship_counter; i++)
        {
            for (int j = 0; j < game_area->ships[i].deck_count; j++)
            {
                if (game_area->ships[i].coords[j][0] == x && game_area->ships[i].coords[j][1] == y)
                {
                    ship_id = i;
                }
            }
        } // move to function
        //ship_id = get_ship_id_by_cell(&game_area, x, y);// (game_area_ptr, x, y);
        game_area->ships[ship_id].current_health = game_area->ships[ship_id].current_health - 1;
        if (game_area->ships[ship_id].current_health == 0)
        {
            game_area->dead_ships_count++;
            for (int i = 0; i < game_area->ships[ship_id].deck_count*2+6; i++)
            {
                int dead_x = game_area->ships[ship_id].around_coords[i][0];
                int dead_y = game_area->ships[ship_id].around_coords[i][1];
                if (dead_x < 11 && dead_y < 11 && dead_x > 0 && dead_y > 0)
                {
                    DestroyWindow(hCell[dead_y][dead_x]); // TODO FIX THIS SHIT

                    hCell[dead_y][dead_x] = CreateWindowW(
                        L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP,
                        start_pos_x + 35 * dead_x,
                        start_pos_y + 35 * dead_y,
                        NULL, NULL, hWnd, NULL, NULL, NULL);
                    hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32dot.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
                    SendMessageW(hCell[dead_y][dead_x], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);

                }
             }
            if (game_area->dead_ships_count == 10) {
                for (int i = 1; i < 12; i++)
                {
                    for (int j = 1; j < 12; j++)
                    {
                        if (game_area->area[i][j] == 0)
                        {
                            if (i < 11 && j < 11 && i > 0 && j > 0) {
                                DestroyWindow(hCell[i][j]);
                                hCell[i][j] = CreateWindowW(
                                    L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP,
                                    start_pos_x + 35 * i,
                                    start_pos_y + 35 * j,
                                    NULL, NULL, hWnd, NULL, NULL, NULL);
                                hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32dot.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
                                SendMessageW(hCell[i][j], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);
                            }
                        }
                    }
                }
                MessageBox(NULL, TEXT("YOU'RE WINNER!"),
                    TEXT("win window"), MB_ICONHAND);
                game_is_on = 0; //TODO change non clicked cells
            }
        }
        break;
    default:
        hCellChange = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, start_pos_x + 35 * x, start_pos_y + 35 * y, NULL, NULL, hWnd, NULL, NULL, NULL);
        hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32dot.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
        SendMessageW(hCellChange, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);
        break;
    }



}

void print_empty_area(int shift_x, int shift_y, HWND hWnd) 
{
    HBITMAP hCellImageChange;
    HWND hCell[12][12];
    for (int i = 1; i < 11; i++)
    {
        for (int j = 1; j < 11; j++)
        {
            hCell[i][j] = CreateWindowW(
                L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP,
                shift_x + 35 * i,
                shift_y + 35 * j,
                NULL, NULL, hWnd, NULL, NULL, NULL);
                hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32white.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);

            SendMessageW(hCell[i][j], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);
        }
    }
}

void open_area(struct game_area area,int shift_x,int shift_y,HWND hWnd )
{
    HBITMAP hCellImageChange;
    HWND hCell[12][12];


    for (int i = 1; i < 11; i++)
    {
        for (int j = 1; j < 11; j++)
        {
            hCell[i][j] = CreateWindowW(
                L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP,
                shift_x + 35 * i,
                shift_y + 35 * j,
                NULL, NULL, hWnd, NULL, NULL, NULL);
            if (area.area[i][j] == 2)
                hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32cross.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
            else
                hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32dot.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);

            SendMessageW(hCell[i][j], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);
        }
    }
}