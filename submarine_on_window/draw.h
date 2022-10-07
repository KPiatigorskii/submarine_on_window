extern int pc_turn;
extern int user_turn;
extern int game_is_on;

void open_area(struct game_area* game_area, HWND hWnd, HWND* hCell[12][12])
{
    HBITMAP hCellImageChange;

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
                hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32green.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
            else
                hCellImageChange = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32white.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);

            SendMessageW(hCell[i][j], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImageChange);
        }
    }
}

void loadDefaultImages(HWND hWnd, HWND* hCell[12][12] )
{
    int start_pos_x = 500;
    int start_pos_y = 30;
    HBITMAP hCellImage[12][12];

    for (int i = 1; i < 11; i++)
    {
        for (int j = 1; j < 11; j++)
        {
            hCellImage[i][j] = (HBITMAP)LoadImageW(NULL, L"square32red.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
            hCell[i][j] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, start_pos_x + 35 * j, start_pos_y + 35 * i, 120, 120, hWnd, NULL, NULL, NULL);
            SendMessageW(hCell[i][j], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImage[i][j]);
        }
    }
}

void change_other_image(struct game_area* game_area, HWND hCell[12][12], HWND hWnd)
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



void PCHitCell(struct game_area* game_area, HWND hWnd, HWND hStaticLabel, HWND hCell[12][12], HWND hCellImage)
{
    SetWindowText(hStaticLabel, L"PC turn! ");
    while (1)
    {
        int cell_x = rand() % 10 + 1;
        int cell_y = rand() % 10 + 1;

        Sleep(10);

        if (cell_x < 11 && cell_x >= 1 && cell_y >= 1 && cell_y < 11)
        {
            if (cell_is_not_clicked(game_area, cell_x, cell_y))
            {
                DestroyWindow(hCell[cell_y][cell_x]);
                game_area->clicked_cells[game_area->move_counter][0] = cell_y;
                game_area->clicked_cells[game_area->move_counter][1] = cell_x;
                game_area->move_counter++;
                game_area->checked_cells_count++;
                if (hitCell(game_area, hCellImage, cell_x, cell_y, hWnd, hCellImage) == 2)
                {
                    pc_turn = 1;
                    user_turn = 0;
                }
                else
                {
                    pc_turn = 0;
                    user_turn = 1;
                    SetWindowText(hStaticLabel, L"Your turn! ");
                    break;
                }
            }
        }
    }
}


void change_around_image(struct game_area* game_area, int ship_id, HWND hWnd, HWND hCell[12][12], HBITMAP hCellImage)
{

    for (int i = 0; i < game_area->ships[ship_id].deck_count * 2 + 6; i++)
    {
        int dead_x = game_area->ships[ship_id].around_coords[i][0];
        int dead_y = game_area->ships[ship_id].around_coords[i][1];

        if (dead_x < 11 && dead_y < 11 && dead_x > 0 && dead_y > 0)
        {
            if (cell_is_not_clicked(game_area, dead_x, dead_y))
            {

                game_area->clicked_cells[game_area->checked_cells_count][0] = dead_y;
                game_area->clicked_cells[game_area->checked_cells_count][1] = dead_x;
                game_area->checked_cells_count++;
                DestroyWindow(hCell[dead_y][dead_x]);
                hCell[dead_y][dead_x] = CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, game_area->start_pos_x + 35 * dead_x, game_area->start_pos_y + 35 * dead_y, NULL, NULL, hWnd, NULL, NULL, NULL);
                hCellImage = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32dot.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
                SendMessageW(hCell[dead_y][dead_x], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hCellImage);
            }
        }
    }
}


int hitCell(struct game_area* game_area, HWND hCell[12][12], int x, int y, HWND hWnd)
{
    HBITMAP hCellImage;

    if (game_area->area[y][x] < 2)
        hCellImage = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32dot.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
    else
        hCellImage = (HBITMAP)LoadImageW(NULL, (LPCWSTR)L"square32cross.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);

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
            if (game_area->game_entity == PC)
            {
                //change_other_image(game_area, hUserCell, hWnd);
                MessageBox(NULL, TEXT("YOU'RE WINNER!"),
                    TEXT("win window"), MB_ICONHAND);
                game_is_on = 0;
            }
            if (game_area->game_entity == USER)
            {
                //change_other_image(game_area, hPCCell, hWnd);
                MessageBox(NULL, TEXT("YOU'RE NOT WINNER! YOU SUCK!"),
                    TEXT("win window"), MB_ICONHAND);
                game_is_on = 0;
            }
        }
    }
    return game_area->area[y][x];
}

