#include <stdlib.h>
#include <time.h>
#include "game_area.h"
// at start all cells has weight 0
// when we miss we set -1 weight
// when we hit 
// 1) ???
// 2) set weight 3 in current cell
// 3) set weight -1 for diagonal cells
// 4) set weight 2 for horizontal and vertical IF they not 3 
// =================================================
// init_pc_logic at start game
// get_next_cell when we hit
// hit_cell
// 
// 3 states of shooting
// 1) search (random shooting)
// 2) check direction shooting (when we hit first time we must check - ship position is horizontal or vertical) 
// 3) shoot other cells of ship

struct pc_engine {
    int weight_area[12][12];
    int prev_cell[2]; // y,x
    int next_cell[2]; // y,x
    int next_cell_buffer[10][2];
    int hit_count;
    int shot_counter;
    int state; // 1 - search, 2 - check_dir, shoot other 
    int dead_ships;
    enum shoot_directions shoot_direction;
    int current_x_multiplier;
    int current_y_multiplier;
    int ship_cells[4][2];
};

void set_diagonal_weight(struct pc_engine* pc_engine_area,int x, int y);
void set_hor_ver_weight(struct pc_engine* pc_engine_area, int x, int y);
int* get_biggest_weight(struct pc_engine* pc_engine_area);
void clear_ship_cells(struct pc_engine* pc_engine_area);


struct pc_engine init_pc_logic()
{
    struct pc_engine pc_engine_area;
    pc_engine_area.prev_cell[0] = -1;
    pc_engine_area.prev_cell[1] = -1;
    pc_engine_area.next_cell[0] = -1;
    pc_engine_area.next_cell[1] = -1;
    pc_engine_area.shot_counter = 0;
    pc_engine_area.hit_count = 0;
    pc_engine_area.dead_ships = 0;

    
    for (int i = 0; i < 12; i++) // fill area zero
    {
        for (int j = 0; j < 12; j++)
        {
            pc_engine_area.weight_area[i][j] = 0;
        }
    }
    clear_ship_cells(&pc_engine_area);
    return pc_engine_area;
}

enum shoot_directions getRandomDirectionShoot()
{
    time_t t;
    srand((unsigned)time(&t));
    enum shoot_directions direction;
    direction = (enum shoot_directions)(rand() % 3);
    return direction;
}

void set_around_weight(struct pc_engine* pc_engine_area)
{
    for (int i = 0; i < 4; i++)
    {
        if (pc_engine_area->ship_cells[i][0] != -1 && pc_engine_area->ship_cells[i][1] != -1)
        {
            int y = pc_engine_area->ship_cells[i][0];
            int x = pc_engine_area->ship_cells[i][1];
            for (int i = y - 1; i < y + 2; i++)
            {
                for (int j = x - 1; j < x + 2; j++)
                {
                    pc_engine_area->weight_area[i][j] = 3;
                }
            }
            
        }
    }
}

void refresh_engine(struct pc_engine* pc_engine_area, int hit_result, int x, int y, int deads)
{
    pc_engine_area->shot_counter++;
    switch (hit_result)
    {
        case 2:
            pc_engine_area->weight_area[y][x] = 3;
            pc_engine_area->hit_count++;
            pc_engine_area->ship_cells[pc_engine_area->hit_count][0] = y;
            pc_engine_area->ship_cells[pc_engine_area->hit_count][1] = x;

            if (deads > pc_engine_area->dead_ships)
            {
                pc_engine_area->dead_ships++;
                set_around_weight(pc_engine_area);
                clear_ship_cells(pc_engine_area);
                pc_engine_area->hit_count = 0;
                int* position_ptr;
                position_ptr = get_biggest_weight(pc_engine_area);
                pc_engine_area->next_cell[0] = position_ptr[0];
                pc_engine_area->next_cell[1] = position_ptr[1];
                break;
            }

            set_diagonal_weight(pc_engine_area,x, y);
            set_hor_ver_weight(pc_engine_area, x, y);

            if (pc_engine_area->hit_count > 1) // not first hit
            {
                pc_engine_area->next_cell[y] = pc_engine_area->next_cell[y] + pc_engine_area->current_y_multiplier;
                pc_engine_area->next_cell[x] = pc_engine_area->next_cell[x] + pc_engine_area->current_x_multiplier;
            }
            else
            { // get random direction for shooting
                int next_x;
                int next_y;
                while (1) 
                {
                    pc_engine_area->shoot_direction = getRandomDirectionShoot();
                    int* multipliers = getMultipliers(pc_engine_area->shoot_direction);
                    pc_engine_area->current_y_multiplier = multipliers[0];
                    pc_engine_area->current_x_multiplier = multipliers[1];
                    next_y = pc_engine_area->next_cell[0] + pc_engine_area->current_y_multiplier;
                    next_x = pc_engine_area->next_cell[1] + pc_engine_area->current_x_multiplier;
                    if (next_x > 0 && next_x < 11 && next_y > 0 && next_y < 11)
                    {
                        pc_engine_area->next_cell[0] = next_y;
                        pc_engine_area->next_cell[1] = next_x;
                        break;
                    }

                }
            }
            break;
        case 1:
        case 0:
            pc_engine_area->weight_area[y][x] = -1;
            if (pc_engine_area->hit_count > 1) // not first 
            { // turn shooting in a different way
                pc_engine_area->next_cell[y] = pc_engine_area->ship_cells[0][0] + pc_engine_area->current_y_multiplier * -1;
                pc_engine_area->next_cell[x] = pc_engine_area->ship_cells[0][1] + pc_engine_area->current_x_multiplier * -1;
            }
            else
            {
                int* position_ptr;
                position_ptr = get_biggest_weight(pc_engine_area);
                pc_engine_area->next_cell[0] = position_ptr[0];
                pc_engine_area->next_cell[1] = position_ptr[1];
            }
            break;
        default:
            break;
    }
    printf("asd");
}

int* get_next_cell(struct pc_engine* pc_engine_area)
{
    if (pc_engine_area->shot_counter == 0) {
        int* coords = malloc(2);
	    coords[0] = rand() % 10 + 1;
	    coords[1] = rand() % 10 + 1;
	    return coords;
    }
    else
        return pc_engine_area->next_cell;
}

void set_diagonal_weight(struct pc_engine* pc_engine_area, int x, int y)
{
    if(pc_engine_area->weight_area[y - 1][x - 1] != 3)
        pc_engine_area->weight_area[y - 1][x - 1] = -1;
    if (pc_engine_area->weight_area[y + 1][x + 1] != 3)
        pc_engine_area->weight_area[y + 1][x + 1] = -1;
    if (pc_engine_area->weight_area[y + 1][x - 1] != 3)
        pc_engine_area->weight_area[y + 1][x - 1] = -1;
    if (pc_engine_area->weight_area[y - 1][x + 1] != 3)
        pc_engine_area->weight_area[y - 1][x + 1] = -1;
}

void set_hor_ver_weight(struct pc_engine* pc_engine_area, int x, int y)
{
    if (pc_engine_area->weight_area[y - 1][x] != 3)
        pc_engine_area->weight_area[y - 1][x] = 1;
    if (pc_engine_area->weight_area[y][x - 1] != 3)
        pc_engine_area->weight_area[y][x - 1] = 1;
    if (pc_engine_area->weight_area[y + 1][x] != 3)
        pc_engine_area->weight_area[y + 1][x] = 1;
    if (pc_engine_area->weight_area[y][x + 1] != 3)
        pc_engine_area->weight_area[y][x + 1] = 1;

}

int* get_biggest_weight(struct pc_engine* pc_engine_area)
{
    int bigger_weight = -1;
    int* bigger_cell = malloc(2);

    for (int y_cell = 1; y_cell < 11; y_cell++)
    {
        for (int x_cell = 1; x_cell < 11; x_cell++)
        {
            if (pc_engine_area->weight_area[y_cell][x_cell] > bigger_weight && pc_engine_area->weight_area[y_cell][x_cell] < 3) {
                //* < 3 not shooted cells
                bigger_weight = pc_engine_area->weight_area[y_cell][x_cell];
                bigger_cell[1] = x_cell;
                bigger_cell[0] = y_cell;
            }
        }
    }
    return bigger_cell;
}

void clear_ship_cells(struct pc_engine* pc_engine_area)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            pc_engine_area->ship_cells[i][j] = -1;
        }
    }
}
