#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "game_area.h"
#include "game_flow.h"

#define FILE_MENU_NEW 1
#define TEST 2
#define FILE_MENU_EXIT 3
#define NEW_GAME 4
#define NEW_GAME_WITH_PC 5
#define GENERATE_SHIPS 6
#define START_GAME 7

static  HWND hStaticLabel;

HDC          hdc;
HBRUSH  NewBrush;
HINSTANCE g_hInst;

struct game_area game_user_area;
struct game_area game_pc_area;
int* checkCellPosition(struct game_area* ,int, int);
int hitCell(struct game_area*,HWND, int, int, HWND);

void changeImage(int, int, int ,HWND );
void open_user_area( HWND);
void print_empty_area(int, int, HWND);
void change_other_image(struct game_area*, HWND, HWND);
void change_around_image(struct game_area*, int, HWND, HWND);
void decrease_ship_health(struct game_area*, int);
int ships_is_generated = 0;
int game_is_on = 0;
int try_counter = 0;
int dead_ships_count = 0;
int pc_turn = 0;
int user_turn = 0;

void AddMenus(HWND);
void AddControls(HWND);
void AddGameControls(HWND);
void loadDefaultImages(HWND);
void PCHitCell(HWND);


LRESULT CALLBACK WindProc(HWND, UINT, WPARAM, LPARAM);

HWND hLogo, hUserCell[12][12], hPCCell[12][12], hUserArea, hPCArea, hWndExample, hWndTurn, hGenerateButton;

HMENU hMenu;
HBITMAP hLogoImage, hCellUserImage[12][12], hCellPCImage[12][12], hGenerateImage, hStartGameButton;
HBITMAP hBitmap;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    int mouse_x;
    int mouse_y;


    switch (msg)
    {
    case WM_LBUTTONUP:
        if (game_is_on == 1) {
            game_pc_area.move_counter++;
            mouse_x = GET_X_LPARAM(lParam);
            mouse_y = GET_Y_LPARAM(lParam);
            int cell_x, cell_y;
            int* position_ptr;
            position_ptr = checkCellPosition(&game_pc_area, mouse_x, mouse_y);
            int arr[2];
            cell_x = *(position_ptr);
            cell_y = *(position_ptr + 1);
            if (cell_x < 11 && cell_x >= 1 && cell_y >= 1 && cell_y < 11)
                DestroyWindow(hPCCell[cell_y][cell_x]);
            if (hitCell(&game_pc_area, hCellPCImage, cell_x, cell_y, hWnd) == 2)
                {
                    pc_turn = 0;
                    user_turn = 1;
                }
            else
                {
                    pc_turn = 1;
                    user_turn = 0;
                    PCHitCell(hWnd);
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
            AddGameControls(hWnd);
            //AddControls(hWnd);
            game_pc_area = init_game_area(500, 30, 1);
            loadDefaultImages(hWnd);
            break;
        case GENERATE_SHIPS:
            game_user_area = init_game_area(30, 30, 0);
            open_user_area(hWnd);
            ships_is_generated = 1;
            break;
        case START_GAME:
            if (ships_is_generated)
            {
                game_is_on = 1;
                hStaticLabel = CreateWindow(L"Static", L"Your turn", WS_CHILD | WS_VISIBLE, 35, 15, 175, 25, hWnd, 0, g_hInst, 0);
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
    AppendMenu(hFileMenu, MF_STRING, NEW_GAME_WITH_PC, L"New game with PC");
    AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, L"Exit");


    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hFileMenu, L"File");
    AppendMenu(hMenu, MF_STRING, 2, L"Help");
    SetMenu(hWnd, hMenu);
}

void AddGameControls(HWND hWnd) 
{
    hGenerateButton = CreateWindowW(L"Button", L"Generate ships", WS_VISIBLE | WS_CHILD, 200, 480, 120, 20, hWnd, (HMENU) GENERATE_SHIPS, NULL, NULL);
    hStartGameButton = CreateWindowW(L"Button", L"Start game", WS_VISIBLE | WS_CHILD, 400, 480, 120, 20, hWnd, (HMENU)START_GAME, NULL, NULL);

}

void AddControls(HWND hWnd)
{
    int start_pos_x = 500;
    int start_pos_y = 30;
    for (int i = 1; i < 11; i++)
    {
        for (int j = 1; j < 11; j++)
        {
            hPCCell[i][j] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, start_pos_x+35*j, start_pos_y+35*i, 120, 120, hWnd, NULL, NULL, NULL);
            SendMessageW(hPCCell[i][j], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellPCImage[i][j]);
        }
    }
}

void loadDefaultImages(HWND hWnd)
{
    int start_pos_x = 500;
    int start_pos_y = 30;

    for (int i = 1; i < 11; i++)
    {
        for (int j = 1; j < 11; j++)
        {
        hCellPCImage[i][j] = (HBITMAP)LoadImageW(NULL, L"square32red.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
        hPCCell[i][j] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, start_pos_x + 35 * j, start_pos_y + 35 * i, 120, 120, hWnd, NULL, NULL, NULL);
        SendMessageW(hPCCell[i][j], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellPCImage[i][j]);
        }
    }
}

int* checkCellPosition(struct game_area *game_area, int x, int y)
{
    int arr[2];
    arr[0] = (int)(x - game_area->start_pos_x) / 35;
    arr[1] = (int)(y - game_area->start_pos_y) / 35;
    return arr;
}

int hitCell(struct game_area *game_area,HWND hCell[12][12], int x, int y, HWND hWnd)
{
    HBITMAP hCellImageChange;

    if (game_area->area[y][x] < 2)
        hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32dot.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
    else
        hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32cross.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
    if (game_area->game_entity == USER_ENTITY) 
    {
        hUserCell[y][x] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, game_area->start_pos_x + 35 * x, game_area->start_pos_y + 35 * y, NULL, NULL, hWnd, NULL, NULL, NULL);
        SendMessageW(hUserCell[y][x], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);
    }
    else
    {
        hPCCell[y][x] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, game_area->start_pos_x + 35 * x, game_area->start_pos_y + 35 * y, NULL, NULL, hWnd, NULL, NULL, NULL);
        SendMessageW(hPCCell[y][x], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);
    }

    if (game_area->area[y][x] == 2) 
    {

        int ship_id = -1;
        ship_id = get_ship_id(game_area, x, y);
        decrease_ship_health(game_area, ship_id);

        if (check_dead_ship(game_area, ship_id))
        {
            game_area->dead_ships_count++;
            change_around_image(game_area, ship_id, hCell, hWnd);
        }
        if (game_area->dead_ships_count == 10)
        {
            change_other_image(game_area, hCell, hWnd);
            MessageBox(NULL, TEXT("YOU'RE WINNER!"),
                TEXT("win window"), MB_ICONHAND);
            game_is_on = 0;
        }
    }
    return game_area->area[y][x];
}

void changeImage( int cell_value, int x, int y,HWND hCell[12][12], HWND hWnd)
{
    HWND hCellImageChange;
    int start_pos_x = 500;
    int start_pos_y = 30;
    hCell[y][x] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, start_pos_x + 35 * x, start_pos_y + 35 * y, NULL, NULL, hWnd, NULL, NULL, NULL);

    if (cell_value < 2)
        hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32dot.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
    else
        hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32cross.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
    SendMessageW(hCell[y][x], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);
}

void change_around_image(struct game_area* game_area, int ship_id, HWND hCell[12][12], HWND hWnd)
{
    HWND hCellImageChange;

    for (int i = 0; i < game_area->ships[ship_id].deck_count * 2 + 6; i++)
    {
        int dead_x = game_area->ships[ship_id].around_coords[i][0];
        int dead_y = game_area->ships[ship_id].around_coords[i][1];
        if (dead_x < 11 && dead_y < 11 && dead_x > 0 && dead_y > 0)
        {
            if (game_area->game_entity == USER)
            {
                DestroyWindow(hUserCell[dead_y][dead_x]);
                hUserCell[dead_y][dead_x] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, game_area->start_pos_x + 35 * dead_x, game_area->start_pos_y + 35 * dead_y, NULL, NULL, hWnd, NULL, NULL, NULL);
                hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32dot.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
                SendMessageW(hUserCell[dead_y][dead_x], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);
            }
            else
            {
                DestroyWindow(hPCCell[dead_y][dead_x]);
                hPCCell[dead_y][dead_x] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, game_area->start_pos_x + 35 * dead_x, game_area->start_pos_y + 35 * dead_y, NULL, NULL, hWnd, NULL, NULL, NULL);
                hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32dot.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
                SendMessageW(hPCCell[dead_y][dead_x], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);
            }
        }
    }
}

int check_dead_ship(struct game_area* game_area, int  ship_id)
{
    if (game_area->ships[ship_id].current_health == 0)
        return 1;
    else
        return 0;
}

int get_ship_id(struct game_area *game_area, int x, int y)
{
    for (int i = 0; i < game_area->ship_counter; i++)
    {
        for (int j = 0; j < game_area->ships[i].deck_count; j++)
        {
            if (game_area->ships[i].coords[j][0] == x && game_area->ships[i].coords[j][1] == y)
            {
                return i;
            }
        }
    } // move to function

    return -1;
}

void decrease_ship_health(struct game_area* game_area, int ship_id)
{
    game_area->ships[ship_id].current_health = game_area->ships[ship_id].current_health - 1;
}

void change_other_image(struct game_area* game_area,HWND hCell[12][12], HWND hWnd)
{
    HWND hCellImageChange;
    int start_pos_x = 500;
    int start_pos_y = 30;

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

void open_user_area(HWND hWnd)
{
    HBITMAP hCellImageChange;

    for (int i = 1; i < 11; i++)
    {
        for (int j = 1; j < 11; j++)
        {
            hUserCell[i][j] = CreateWindowW(
                L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP,
                game_user_area.start_pos_x + 35 * j,
                game_user_area.start_pos_y + 35 * i,
                NULL, NULL, hWnd, NULL, NULL, NULL);
            if (game_user_area.area[i][j] == 2)
                hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32green.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
            else
                hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32white.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);

            SendMessageW(hUserCell[i][j], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);
        }
    }
}

void PCHitCell(HWND hWnd)
{
    if ((game_is_on == 1) && (pc_turn == 1))
    {
        SetWindowText(hStaticLabel, L"PC turn! ");

        int cell_x = rand() % 9 + 1;
        int cell_y = rand() % 9 + 1;
        //arr = get_next_hit_cell();

        Sleep(1000);
        if (cell_x < 11 && cell_x >= 1 && cell_y >= 1 && cell_y < 11)
            DestroyWindow(hUserCell[cell_y][cell_x]);
        if (hitCell(&game_user_area, hCellUserImage, cell_x, cell_y, hWnd) == 2)
        {
            pc_turn = 1;
            user_turn = 0;
        }
        else
        {
            pc_turn = 0;
            user_turn = 1;
            SetWindowText(hStaticLabel, L"Your turn! ");
        }
    }
}