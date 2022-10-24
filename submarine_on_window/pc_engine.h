#include <stdlib.h>
#include <time.h>
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

void init_pc_logic()
{
}

void refresh_engine(int shot_result, int cell_x, int cell_y, int dead_ships_count)
{

}

int* get_next_cell() 
{
	int coords[2];

	coords[0] = rand() % 10 + 1;
	coords[1] = rand() % 10 + 1;
	return coords;
}

//enum shoot_directions { UP_SHOOT = 0, RIGHT_SHOOT = 1, DOWN_SHOOT = 2, LEFT_SHOOT = 3 };
//
//
//enum shoot_directions getRandomDirectionShoot()
//{
//    time_t t;
//    srand((unsigned)time(&t));
//    enum shoot_directions direction;
//    direction = (enum shoot_directions)(rand() % 3);
//    return direction;
//}
//
//int* getMultipliers(enum shoot_directions direction)
//{
//    int dir_arr[2];
//    switch (direction) {
//    case UP_SHOOT:
//        dir_arr[0] = 1;
//        dir_arr[1] = -1;
//        break;
//    case RIGHT_SHOOT:
//        dir_arr[0] = 1;
//        dir_arr[1] = 1;
//        break;
//    case DOWN_SHOOT:
//        dir_arr[0] = 1;
//        dir_arr[1] = 1;
//        break;
//    case LEFT_SHOOT:
//        dir_arr[0] = -1;
//        dir_arr[1] = 1;
//        break;
//
//    default:
//        dir_arr[0] = 1;
//        dir_arr[1] = -1;
//        break;
//    }
//    return dir_arr;
//}
//
//struct pc_engine {
//    int weight_area[12][12];
//    int prev_cell[2]; // y,x
//    int next_cell[2]; // y,x
//    int hit_count = 0;
//    int dead_ships;
//    enum shoot_directions shoot_direction;
//    int current_x_multiplier = 0;
//    int current_y_multiplier = 0;
//    int ship_cells[4][2];
//};
//
//
//struct pc_engine pc_engine_area;
//
//void set_diagonal_weight(int x, int y);
//void set_hor_ver_weight(int x, int y);
//int* get_biggest_weight();
//
//struct pc_engine init_pc_logic()
//{
//    pc_engine_area.prev_cell[0] = -1;
//    pc_engine_area.prev_cell[1] = -1;
//    for (int i = 0; i < 12; i++) // fill area zero
//    {
//        for (int j = 0; j < 12; j++)
//        {
//            pc_engine_area.weight_area[i][j] = 0;
//        }
//    }
//
//    return pc_engine_area;
//}
//
//void set_around_weight()
//{
//    for (int i = 0; i < 4; i++)
//    {
//        if (pc_engine_area.ship_cells[i][0] != -1 && pc_engine_area.ship_cells[i][1] != -1)
//        {
//            int y = pc_engine_area.ship_cells[i][0];
//            int x = pc_engine_area.ship_cells[i][1];
//            for (int i = y - 1; i < y + 2; i++)
//            {
//                for (int j = x - 1; j < x + 2; j++)
//                {
//                    pc_engine_area.weight_area[i][j] = 3;
//                }
//            }
//            
//        }
//    }
//}
//
//void refresh_engine(int hit_result, int x, int y, int deads)
//{
//    switch (hit_result)
//    {
//        case 2:
//            pc_engine_area.weight_area[y][x] = 3;
//            set_diagonal_weight(x, y);
//            set_hor_ver_weight(x, y);
//            if (deads > pc_engine_area.dead_ships)
//            {
//                pc_engine_area.dead_ships++;
//                set_around_weight();
//                break;
//            }
//            if (pc_engine_area.hit_count > 0) // not first hit
//            {
//                pc_engine_area.next_cell[y] = pc_engine_area.prev_cell[y] + pc_engine_area.current_y_multiplier;
//                pc_engine_area.next_cell[x] = pc_engine_area.prev_cell[x] + pc_engine_area.current_x_multiplier;
//            }
//            else
//            { // get random direction for shooting
//                pc_engine_area.shoot_direction = getRandomDirectionShoot();
//                int* multipliers = getMultipliers(pc_engine_area.shoot_direction);
//                pc_engine_area.current_y_multiplier = multipliers[0];
//                pc_engine_area.current_x_multiplier = multipliers[1];
//            }
//            pc_engine_area.hit_count++;
//        case 1:
//        case 0:
//            if (pc_engine_area.hit_count > 1) // not first 
//            { // turn shooting in a different way
//                pc_engine_area.next_cell[y] = pc_engine_area.ship_cells[0][0] + pc_engine_area.current_y_multiplier * -1;
//                pc_engine_area.next_cell[x] = pc_engine_area.ship_cells[0][1] + pc_engine_area.current_x_multiplier * -1;
//            }
//            else
//            {
//                int* position_ptr;
//                position_ptr = get_biggest_weight();
//                pc_engine_area.next_cell[0] = position_ptr[0];
//                pc_engine_area.next_cell[1] = position_ptr[1];
//            }
//            break;
//        default:
//            break;
//    }
//    if (hit_result > 1) // we hit totally
//    {
//
//
//
//    }
//    else // we miss
//    {
//
//    }
//}
//
//int* get_next_cell()
//{
//    return pc_engine_area.next_cell;
//}
//
//void set_diagonal_weight(int x, int y)
//{
//    pc_engine_area.weight_area[y - 1][x - 1] = -1;
//    pc_engine_area.weight_area[y + 1][x + 1] = -1;
//    pc_engine_area.weight_area[y + 1][x - 1] = -1;
//    pc_engine_area.weight_area[y - 1][x - 1] = -1;
//}
//
//void set_hor_ver_weight(int x, int y)
//{
//    if (pc_engine_area.weight_area[y - 1][x] != 3)
//        pc_engine_area.weight_area[y - 1][x] = 1;
//    if (pc_engine_area.weight_area[y][x - 1] != 3)
//        pc_engine_area.weight_area[y][x - 1] = 1;
//    if (pc_engine_area.weight_area[y + 1][x] != 3)
//        pc_engine_area.weight_area[y + 1][x] = 1;
//    if (pc_engine_area.weight_area[y][x + 1] != 3)
//        pc_engine_area.weight_area[y][x + 1] = 1;
//
//}
//
//int* get_biggest_weight()
//{
//    int bigger_weight = -1;
//    int bigger_cell[2];
//
//    for (int y_cell = 1; y_cell < 11; y_cell++)
//    {
//        for (int x_cell = 1; x_cell < 11; x_cell++)
//        {
//            if (pc_engine_area.weight_area[y_cell][x_cell] > bigger_weight && pc_engine_area.weight_area[y_cell][x_cell] < 3) { 
//                //* < 3 not shooted cells
//                bigger_weight = pc_engine_area.weight_area[y_cell][x_cell];
//                bigger_cell[0] = x_cell;
//                bigger_cell[1] = y_cell;
//            }
//        }
//    }
//    return bigger_cell;
//}
