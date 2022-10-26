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


#define COMPLETE_SHIP_WEIGHT -3
#define MISS_WEIGHT -2
#define HIT_WEIGHT -1
#define EMPTY_WEIGHT 0
#define HIT_PREDICT_AROUND_WEIGHT 1
#define HIT_PREDICT_PLANE_WEIGHT 2
struct pc_engine {
    int weight_area[12][12];
    int next_cell[2]; // y,x
    int hit_count;
    int shot_counter;
    int dead_ships;
    int ship_plane;
    int ship_cells[4][2];
};

void set_diagonal_weight(struct pc_engine* pc_engine_area,int x, int y);
void set_hor_ver_weight(struct pc_engine* pc_engine_area, int x, int y);
int* get_biggest_weight(struct pc_engine* pc_engine_area);
void clear_ship_cells(struct pc_engine* pc_engine_area);
void set_weights_with_plane(struct pc_engine* pc_engine_area);
void find_plane(struct pc_engine* pc_engine_area);

struct pc_engine init_pc_logic()
{
    struct pc_engine pc_engine_area;
    pc_engine_area.next_cell[0] = -1;
    pc_engine_area.next_cell[1] = -1;
    pc_engine_area.shot_counter = 0;
    pc_engine_area.hit_count = 0;
    pc_engine_area.dead_ships = 0;
    pc_engine_area.ship_plane = 0;

    
    for (int i = 0; i < 12; i++) // fill negative nums
    {
        for (int j = 0; j < 12; j++)
        {
            pc_engine_area.weight_area[i][j] = EMPTY_WEIGHT;
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
    for (int i = 0; i < pc_engine_area->hit_count; i++)
    {
        int y = pc_engine_area->ship_cells[i][0];
        int x = pc_engine_area->ship_cells[i][1];
        for (int i = y - 1; i < y + 2; i++)
        {
            for (int j = x - 1; j < x + 2; j++)
            {
                pc_engine_area->weight_area[i][j] = COMPLETE_SHIP_WEIGHT;
            }
        }
    }
}

void set_next_cell(struct pc_engine* pc_engine_area, int hit_result, int x, int y, int deads)
{
    int* position_ptr;
    pc_engine_area->shot_counter++;
    switch (hit_result)
    {
        case 0:
        case 1:
             if (pc_engine_area->weight_area[y][x] == 5)
                 pc_engine_area->weight_area[y][x] = MISS_WEIGHT;
             if (pc_engine_area->weight_area[y][x] == -9)
                 pc_engine_area->weight_area[y][x] = MISS_WEIGHT;
            pc_engine_area->weight_area[y][x] = MISS_WEIGHT;
            break;
        case 2: // we hit
            pc_engine_area->weight_area[y][x] = HIT_WEIGHT;
            pc_engine_area->ship_cells[pc_engine_area->hit_count][0] = y;
            pc_engine_area->ship_cells[pc_engine_area->hit_count][1] = x;
            pc_engine_area->hit_count++;

            if (deads > pc_engine_area->dead_ships)
            {
                pc_engine_area->dead_ships++;
                set_around_weight(pc_engine_area);
                clear_ship_cells(pc_engine_area);
                pc_engine_area->hit_count = 0;
                break;
            }

            switch (pc_engine_area->hit_count)
            {
                case 1: // first hit
                    set_diagonal_weight(pc_engine_area, x, y);
                    set_hor_ver_weight(pc_engine_area, x, y);
                    break;
                case 2: // 2nd hit
                    find_plane(pc_engine_area);
                case 3: // 3d hit
                    set_weights_with_plane(pc_engine_area);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    position_ptr = get_biggest_weight(pc_engine_area);
    pc_engine_area->next_cell[0] = position_ptr[0];
    pc_engine_area->next_cell[1] = position_ptr[1];
}

void set_weights_with_plane(struct pc_engine* pc_engine_area)
{
    for (int i = 0; i < pc_engine_area->hit_count; i++)
    {
        int x = pc_engine_area->ship_cells[i][1];
        int y = pc_engine_area->ship_cells[i][0];
        if (pc_engine_area->ship_plane == 0) // hor
        {
            pc_engine_area->weight_area[y - 1][x] = MISS_WEIGHT;
            pc_engine_area->weight_area[y + 1][x] = MISS_WEIGHT;
            if (pc_engine_area->weight_area[y][x + 1] > HIT_WEIGHT)
                pc_engine_area->weight_area[y][x + 1] = HIT_PREDICT_PLANE_WEIGHT;
            if (pc_engine_area->weight_area[y][x - 1] > HIT_WEIGHT)
                pc_engine_area->weight_area[y][x - 1] = HIT_PREDICT_PLANE_WEIGHT;
        }
        else 
        {
            pc_engine_area->weight_area[y][x - 1] = MISS_WEIGHT;
            pc_engine_area->weight_area[y][x + 1] = MISS_WEIGHT;
            if (pc_engine_area->weight_area[y + 1][x] > HIT_WEIGHT)
                pc_engine_area->weight_area[y + 1][x] = HIT_PREDICT_PLANE_WEIGHT;
            if (pc_engine_area->weight_area[y - 1][x] > HIT_WEIGHT)
                pc_engine_area->weight_area[y - 1][x] = HIT_PREDICT_PLANE_WEIGHT;
        }
    }
}

void find_plane(struct pc_engine* pc_engine_area)
{
    int x_dif = pc_engine_area->ship_cells[0][1] - pc_engine_area->ship_cells[1][1];
    int y_dif = pc_engine_area->ship_cells[0][0] - pc_engine_area->ship_cells[1][0];
    if (x_dif == 0) 
        pc_engine_area->ship_plane = 1; // vertical
    else
        pc_engine_area->ship_plane = 0; // horisontal
}

int* get_next_cell(struct pc_engine* pc_engine_area)
{
    if (pc_engine_area->shot_counter == 0 || pc_engine_area->hit_count == 0) {
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
    if (pc_engine_area->weight_area[y - 1][x - 1] >= EMPTY_WEIGHT)
        pc_engine_area->weight_area[y - 1][x - 1] = COMPLETE_SHIP_WEIGHT;
    if (pc_engine_area->weight_area[y + 1][x + 1] >= EMPTY_WEIGHT)
        pc_engine_area->weight_area[y + 1][x + 1] = COMPLETE_SHIP_WEIGHT;
    if (pc_engine_area->weight_area[y + 1][x - 1] >= EMPTY_WEIGHT)
        pc_engine_area->weight_area[y + 1][x - 1] = COMPLETE_SHIP_WEIGHT;
    if (pc_engine_area->weight_area[y - 1][x + 1] >= EMPTY_WEIGHT)
        pc_engine_area->weight_area[y - 1][x + 1] = COMPLETE_SHIP_WEIGHT;
}

void set_hor_ver_weight(struct pc_engine* pc_engine_area, int x, int y)
{
    if (pc_engine_area->weight_area[y - 1][x] >= EMPTY_WEIGHT)
        pc_engine_area->weight_area[y - 1][x] = HIT_PREDICT_AROUND_WEIGHT;
    if (pc_engine_area->weight_area[y][x - 1] >= EMPTY_WEIGHT)
        pc_engine_area->weight_area[y][x - 1] = HIT_PREDICT_AROUND_WEIGHT;
    if (pc_engine_area->weight_area[y + 1][x] >= EMPTY_WEIGHT)
        pc_engine_area->weight_area[y + 1][x] = HIT_PREDICT_AROUND_WEIGHT;
    if (pc_engine_area->weight_area[y][x + 1] >= EMPTY_WEIGHT)
        pc_engine_area->weight_area[y][x + 1] = HIT_PREDICT_AROUND_WEIGHT;

}

int* get_biggest_weight(struct pc_engine* pc_engine_area)
{
    int bigger_weight = EMPTY_WEIGHT;
    int* bigger_cell = malloc(2);

    for (int y_cell = 1; y_cell < 11; y_cell++)
    {
        for (int x_cell = 1; x_cell < 11; x_cell++)
        {
            int cell = pc_engine_area->weight_area[y_cell][x_cell];
            if (cell > bigger_weight) {
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
