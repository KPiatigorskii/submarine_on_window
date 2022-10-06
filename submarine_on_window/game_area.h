#include "ships.h"


enum game_entity {USER_ENTITY=0, PC_ENTITY=1};

struct game_area
{
    int ship_counter;
    int move_counter;
    int checked_cells_count;
    int dead_ships_count;
    int start_pos_x;
    int start_pos_y;
    int area[12][12];
    int clicked_cells[100][2];
    enum game_entity game_entity;
    struct ship ships[SHIPS_COUNT];
} game_area;



struct game_area init_game_area(int start_pos_x,int start_pos_y, enum game_entity game_entity)
{
    game_area.ship_counter = 0;
    game_area.move_counter = 0;
    game_area.dead_ships_count = 0;
    game_area.checked_cells_count = 0;
    game_area.start_pos_x = start_pos_x;
    game_area.start_pos_y = start_pos_y;
    game_area.game_entity = game_entity;

    for (int i = 0; i < 12; i++) // fill area zero
    {
        for (int j = 0; j < 12; j++)
        {
            game_area.area[i][j] = 0;
        }
    }

    game_area.ships[0] = generate_ship(game_area.area, 4, 0);
    game_area.ships[1] = generate_ship(game_area.area, 3, 1);
    game_area.ships[2] = generate_ship(game_area.area, 3, 2);
    game_area.ships[3] = generate_ship(game_area.area, 2, 3);
    game_area.ships[4] = generate_ship(game_area.area, 2, 4);
    game_area.ships[5] = generate_ship(game_area.area, 2, 5);
    game_area.ships[6] = generate_ship(game_area.area, 1, 6);
    game_area.ships[7] = generate_ship(game_area.area, 1, 7);
    game_area.ships[8] = generate_ship(game_area.area, 1, 8);
    game_area.ships[9] = generate_ship(game_area.area, 1, 9);
    game_area.ship_counter = 10;
    return game_area;
}

//int* checkCellPosition(struct game_area* game_area, int x, int y)
//{
//    int arr[2];
//    arr[0] = (int)(x - game_area->start_pos_x) / 35;
//    arr[1] = (int)(y - game_area->start_pos_y) / 35;
//    return arr;
//}