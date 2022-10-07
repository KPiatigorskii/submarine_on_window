#include <stdlib.h>

struct pc_engine {
	int weight_area[12][12];
    int prev_cell[2];
    int next_cell[2];

} pc_engine;

struct pc_engine init_pc_logic()
{
    pc_engine.prev_cell[0] = -1;
    pc_engine.prev_cell[1] = -1;
    for (int i = 0; i < 12; i++) // fill area zero
    {
        for (int j = 0; j < 12; j++)
        {
            pc_engine.weight_area[i][j] = 0;
        }
    }
	return pc_engine;
}

int* get_next_cell()
{
    if (pc_engine.prev_cell[0] == -1)
    {
        pc_engine.next_cell[0] = rand() % 9 + 1;
        pc_engine.next_cell[1] = rand() % 9 + 1;
    }
    else
    {
        int* position_ptr;
        position_ptr = get_biggest_weight();
        pc_engine.next_cell[0] = *(position_ptr);
        pc_engine.next_cell[1] = *(position_ptr + 1);

    }
    return pc_engine.next_cell;
}

int* set_weight(int x, int y, int weight)
{
    pc_engine.weight_area[x][y] = weight;
    if (weight == 3)
    {
        set_diagonal_weight(x, y);
        set_hor_ver_weight(x, y);
    }
}

void set_diagonal_weight(int x, int y)
{
    pc_engine.weight_area[x - 1][y - 1] = -1;
    pc_engine.weight_area[x + 1][y + 1] = -1;
    pc_engine.weight_area[x - 1][y + 1] = -1;
    pc_engine.weight_area[x - 1][y - 1] = -1;
}

void set_hor_ver_weight(int x, int y)
{
    if (pc_engine.weight_area[x][y-1] != 3)
        pc_engine.weight_area[x][y-1] = 1;
    if (pc_engine.weight_area[x-1][y] != 3)
        pc_engine.weight_area[x-1][y] = 1;
    if (pc_engine.weight_area[x][y+1] != 3)
        pc_engine.weight_area[x][y+1] = 1;
    if (pc_engine.weight_area[x+1][y] != 3)
        pc_engine.weight_area[x+1][y] = 1;
    
}

int* get_biggest_weight()
{
    int bigger_weight = -1;
    int bigger_cell[2];

    for (int y_cell = 1; y_cell < 11; y_cell++)
    {
        for (int x_cell = 1; x_cell < 11; x_cell++)
        {
            if (pc_engine.weight_area[y_cell][x_cell] > bigger_weight) {
                bigger_weight = pc_engine.weight_area[y_cell][x_cell];
                bigger_cell[0] = x_cell;
                bigger_cell[1] = y_cell;
            }
        }
    }
    return bigger_cell;
}