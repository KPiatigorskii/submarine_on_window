
#include "controls.h"

extern int pc_turn;
extern int user_turn;
extern int game_is_on;



HDC          hdc;
HBRUSH  NewBrush;
HINSTANCE g_hInst;

HWND hUserCell[12][12] = { NULL };
HWND hPCCell[12][12] = { NULL };

void clear_all_window(hWnd);

void open_area(struct game_area* game_area, HWND hWnd, HWND* hCell[12][12])
{
    HBITMAP hCellImageChange;
    LPCWSTR imageName;

    for (int i = 1; i < 11; i++)
    {
        for (int j = 1; j < 11; j++)
        {
            hCell[i][j] = CreateWindowW(
                L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP,
                game_area->start_pos_x + 35 * j,
                game_area->start_pos_y + 35 * i,
                NULL, NULL, hWnd, NULL, NULL, NULL);
            if (game_area->area[i][j] == 2)
                imageName = L"square32green.bmp";
            else
                imageName = L"square32white.bmp";
            hCellImageChange = (HBITMAP)LoadImageW(NULL, imageName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
            SendMessageW(hCell[i][j], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);
        }
    }
}

void loadDefaultImages(struct game_area* game_area, HWND hWnd, HWND* hCell[12][12])
{
    HBITMAP hCellImage = (HBITMAP)LoadImageW(NULL, L"square32red.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
    for (int i = 1; i < 11; i++)
    {
        for (int j = 1; j < 11; j++)
        {
            hCell[i][j] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, game_area->start_pos_x + 35 * j, game_area->start_pos_y + 35 * i, 120, 120, hWnd, NULL, NULL, NULL);
            SendMessageW(hCell[i][j], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImage);
        }
    }
}

void change_other_image(struct game_area* game_area, HWND hCell[12][12], HWND hWnd)
{
    HBITMAP hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32dot.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
    int start_pos_x;
    int start_pos_y;
    start_pos_x = game_area->start_pos_x;
    start_pos_y = game_area->start_pos_y;
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
                    SendMessageW(hCell[i][j], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);
                }
            }
        }
    }
}

void change_around_image(struct game_area* game_area, int ship_id, HWND hWnd, HWND hCell[12][12])
{
    HBITMAP hCellImage;

    for (int i = 0; i < game_area->ships[ship_id].deck_count * 2 + 6; i++)
    {
        int dead_x = game_area->ships[ship_id].around_coords[i][0];
        int dead_y = game_area->ships[ship_id].around_coords[i][1];
        hCellImage = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32dot.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);

        if (dead_x < 11 && dead_y < 11 && dead_x > 0 && dead_y > 0)
        {
            if (cell_is_not_clicked(game_area, dead_x, dead_y))
            {
                increment_clicked_cells(game_area, dead_x, dead_y);
                DestroyWindow(hCell[dead_y][dead_x]);
                hCell[dead_y][dead_x] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, game_area->start_pos_x + 35 * dead_x, game_area->start_pos_y + 35 * dead_y, NULL, NULL, hWnd, NULL, NULL, NULL);
                SendMessageW(hCell[dead_y][dead_x], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImage);
            }
        }
    }
}


int hitCell(struct game_area* game_area, HWND hCell[12][12], int x, int y, HWND hWnd)
{
    HBITMAP hCellImage;
    LPCWSTR imageName;

    DestroyWindow(hCell[y][x]); // delete clicked cell
    increment_clicked_cells(game_area, x, y);
    game_area->move_counter++;
    if (game_area->area[y][x] < 2)
        imageName = L"square32dot.bmp";
    else
        imageName = L"square32cross.bmp";
        
    hCellImage = (HBITMAP)LoadImageW(NULL, imageName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);

    hCell[y][x] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, game_area->start_pos_x + 35 * x, game_area->start_pos_y + 35 * y, NULL, NULL, hWnd, NULL, NULL, NULL);
    SendMessageW(hCell[y][x], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImage);

    if (game_area->area[y][x] == 2)
    {
        int ship_id = get_ship_id(game_area, x, y);
        decrease_ship_health(game_area, ship_id);

        if (check_dead_ship(game_area, ship_id))
        {
            game_area->dead_ships_count++;
            change_around_image(game_area, ship_id, hWnd, hCell, hCellImage);
        }
        if (game_area->dead_ships_count == 10)
        {
            if (game_area->game_entity == PC_ENTITY)
            {
                //change_other_image(game_area, hUserCell, hWnd);
                MessageBox(NULL, TEXT("YOU'RE WINNER!"),
                    TEXT("win window"), MB_ICONHAND);
                game_is_on = 0;
            }
            if (game_area->game_entity == USER_ENTITY)
            {
                //change_other_image(game_area, hPCCell, hWnd);
                MessageBox(NULL, TEXT("YOU'RE NOT WINNER! YOU SUCK!"),
                    TEXT("win window"), MB_ICONHAND);
                game_is_on = 0;
            }
            clear_all_window();
        }
    }
    else {
        if (game_area->game_entity == PC_ENTITY) {
            pc_turn = 1;
            user_turn = 0;
        }
        else {
            pc_turn = 0;
            user_turn = 1;
            SetWindowText(hStaticLabel, L"Your turn! ");
        }

    }
    return game_area->area[y][x];
}

void clear_all_window() {
    for (int i = 1; i < 11; i++)
    {
        for (int j = 1; j < 11; j++) {
            DestroyWindow(hPCCell[i][j]);
            DestroyWindow(hUserCell[i][j]);
        }
    }
    RemoveGameControls();

}

void clear_area(HWND hCell[12][12])
{
    for (int y = 1; y < 11; y++)
    {
        for (int x = 1; x < 11; x++) 
        {
            DestroyWindow(hCell[y][x]);
        }
    }
}
