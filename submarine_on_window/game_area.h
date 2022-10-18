#include "ships.h"

#define AREA_SIZE_WITH_BORDERS 12
#define CELL_SIZE 35

enum game_entity { USER_ENTITY = 0, PC_ENTITY = 1 };

struct game_area
{
    int ship_counter;
    int move_counter;
    int checked_cells_count;
    int dead_ships_count;
    int start_pos_x;
    int start_pos_y;
    int area[AREA_SIZE_WITH_BORDERS][AREA_SIZE_WITH_BORDERS];
    int clicked_cells[100][2];
    enum game_entity game_entity;
    struct ship ships[SHIPS_COUNT];
} game_area;

int ship_cell_array[10] = { 4,3,3,2,2,2,1,1,1,1 };

void incrementClickedCells(struct game_area* game_area, int x, int y)
{
    game_area->clicked_cells[game_area->checked_cells_count][0] = y;
    game_area->clicked_cells[game_area->checked_cells_count][1] = x;
    game_area->checked_cells_count++;
}

struct game_area initGameArea(int start_pos_x,int start_pos_y, enum game_entity game_entity)
{
    game_area.ship_counter = 0;
    game_area.move_counter = 0;
    game_area.dead_ships_count = 0;
    game_area.checked_cells_count = 0;
    game_area.start_pos_x = start_pos_x;
    game_area.start_pos_y = start_pos_y;
    game_area.game_entity = game_entity;

    for (int i = 0; i < AREA_SIZE_WITH_BORDERS; i++) // fill area zero
    {
        for (int j = 0; j < AREA_SIZE_WITH_BORDERS; j++)
        {
            game_area.area[i][j] = 0;
        }
    }
    for (int i = 0; i < SHIPS_COUNT; i++)
    {
        game_area.ships[i] = generateShip(game_area.area, ship_cell_array[i], i);
    }

    game_area.ship_counter = 10;
    return game_area;
}

int cellIsNotClicked(struct game_area* game_area, int x, int y)
{
    for (int i = 0; i < game_area->checked_cells_count; i++)
    {
        if (game_area->clicked_cells[i][0] == y && game_area->clicked_cells[i][1] == x)
            return 0;
    }
    return 1;
}

int getShipId(struct game_area* game_area, int x, int y)
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
    }

    return -1;
}

void decreaseShipHealth(struct game_area* game_area, int ship_id)
{
    game_area->ships[ship_id].current_health = game_area->ships[ship_id].current_health - 1;
}

int* checkCellPosition(struct game_area* game_area, int x, int y)
{
    int arr[2];
    arr[0] = (int)(x - game_area->start_pos_x) / CELL_SIZE;
    arr[1] = (int)(y - game_area->start_pos_y) / CELL_SIZE;
    return arr;
}

int checkDeadShip(struct game_area* game_area, int  ship_id)
{
    if (game_area->ships[ship_id].current_health == 0)
        return 1;
    else
        return 0;
}