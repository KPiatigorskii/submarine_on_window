#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define MAX_CELLS_IN_AREA 10
#define MIN_CELLS_IN_AREA 1
#define SHIPS_COUNT 10
#define MAX_DECK_IN_SHIP 4

enum ship_directions { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 };
struct ship
{
    int coords[4][2];
    int around_coords[18][2];
    int is_valid; // if not zero then true
    int id;
    int max_health;
    int current_health;
    int around_size;
    int deck_count;
};

struct ship ships[10];

void print_area_0(int area[12][12])
{
    for (int i = 1; i < 11; i++)
    {
        for (int j = 1; j < 11; j++)
        {
            printf("%d ", area[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_ship(struct ship* new_ship)
{
    printf("====================\n");
    printf("cells:\n");
    for (int count = 0; count < new_ship->deck_count; count++)
    {
        printf("{x=%d, y=%d} ", new_ship->coords[count][0], new_ship->coords[count][1]);
    }
    printf("\n");
    printf("around cells:\n");
    for (int i = 0; i < new_ship->deck_count * 2 + 6; i++)
    {
        printf("{x=%d, y=%d} ", new_ship->coords[i][0], new_ship->coords[i][1]);
    }
    printf("\n====================\n");
}

int check_cell_borders(int area[12][12], int x, int y)
{
    if (area[y][x] > 10 ||
        area[y][x] > 10 ||
        area[y][x] < 1 ||
        area[y][x] < 1)
        return 0;
    return 1;
}

int check_borders(struct ship* new_ship)
{
    for (int count = 0; count < new_ship->deck_count; count++)
    {
        if (new_ship->coords[count][0] > 10 || new_ship->coords[count][1] > 10 || new_ship->coords[count][1] < 1 || new_ship->coords[count][1] < 1)
            return 0;
    }
    return 1;
}

int cell_in_ship(struct ship* new_ship, int i, int j)
{
    for (int count = 0; count < new_ship->deck_count; count++)
    {
        if (new_ship->coords[count][0] == j && new_ship->coords[count][1] == i)
            return 1;
    }
    return 0;
}

void fill_area(struct ship* new_ship, int area[12][12])
{
    for (int i = 0; i < new_ship->deck_count; i++)
    {
        area[new_ship->coords[i][1]][new_ship->coords[i][0]] = 2;
    }
    for (int i = 0; i < new_ship->deck_count * 2 + 6; i++)
    {
        area[new_ship->around_coords[i][1]][new_ship->around_coords[i][0]] = 1;
    }

}

int set_around_coords_for_ship(struct ship* new_ship, int multipliers[2], int area[12][12])
{
    int start_x = new_ship->coords[0][0];
    int start_y = new_ship->coords[0][1];
    int stop_x = new_ship->coords[new_ship->deck_count - 1][0];
    int stop_y = new_ship->coords[new_ship->deck_count - 1][1];
    int add_x = multipliers[0];
    int add_y = multipliers[1];
    int around_coords_counter = 0;

    for (int i = start_y - add_y; i != stop_y + 2 * add_y; i = i + add_y) {
        for (int j = start_x - add_x; j != stop_x + 2 * add_x; j = j + add_x) {
            if (area[i][j] != 2)
            {
                if (!cell_in_ship(new_ship, i, j))
                {
                    new_ship->around_coords[around_coords_counter][0] = j;
                    new_ship->around_coords[around_coords_counter][1] = i;
                    //printf("add to around coords: %d,%d\n", i, j);
                    around_coords_counter = around_coords_counter + 1;
                }
            }
            else {
                new_ship->is_valid = 0;
                return 0;
            }
        }

    }
    return 1;
}

int set_all_coords_for_ship(int start_x, int start_y, int* multipliers, enum ship_directions direction, struct ship* new_ship, int area[12][12])
{
    for (int i = 0; i < new_ship->deck_count; i++)
    {
        switch (direction) {
        case UP:
            new_ship->coords[i][1] = start_y - i; // y
            new_ship->coords[i][0] = start_x; // x
            break;
        case RIGHT:
            new_ship->coords[i][1] = start_y;
            new_ship->coords[i][0] = start_x + i;
            break;
        case DOWN:
            new_ship->coords[i][1] = start_y + i;
            new_ship->coords[i][0] = start_x;
            break;
        case LEFT:
            new_ship->coords[i][1] = start_y;
            new_ship->coords[i][0] = start_x - i;
            break;
        default:
            break;
        }
        if (new_ship->coords[i][1] > 10 ||
            new_ship->coords[i][0] > 10 ||
            new_ship->coords[i][1] < 1 ||
            new_ship->coords[i][0] < 1 ||
            area[new_ship->coords[i][1]][new_ship->coords[i][0]] == 2)
        {
            new_ship->is_valid = 0;
            return 0;
        }

    }
    return 1;
}

int* get_multipliers(enum ship_directions direction)
{
    int* dir_arr = malloc(2);
    switch (direction) {
    case UP:
        dir_arr[0] = 1;
        dir_arr[1] = -1;
        break;
    case RIGHT:
        dir_arr[0] = 1;
        dir_arr[1] = 1;
        break;
    case DOWN:
        dir_arr[0] = 1;
        dir_arr[1] = 1;
        break;
    case LEFT:
        dir_arr[0] = -1;
        dir_arr[1] = 1;
        break;

    default:
        dir_arr[0] = 1;
        dir_arr[1] = -1;
        break;
    }
    return dir_arr;
}

enum ship_directions get_random_direction()
{
    enum ship_directions direction = (rand() % 3);
    return direction;
}

int is_neighbors_free(int area[12][12], int start_x, int start_y) {
    for (int i = start_y - 1; i < start_y + 2; i++)
    {
        for (int j = start_x - 1; j < start_x + 2; j++)
        {
            //printf("check cell x = %d, y = %d \n", j,i);
            if (area[i][j] == 2) {
                //printf("BUSTED\n");
                return 0;
            }
        }
    }
    return 1;
}

int* get_free_cell(int area[12][12])
{
    int* arr = malloc(2);
    int busy_cell_flag = 0;
    while (1) {
        arr[0] = rand() % 9 + 1;
        arr[1] = rand() % 9 + 1;
        //printf("Suggected cell: %d %d\n", arr[0], arr[1]);
        return arr;
    }
}

int check_all_cell_is_valid(struct ship* new_ship, int area[12][12])
{
    for (int i = 0; i < new_ship->deck_count; i++)
    {
        if (is_neighbors_free(area, new_ship->coords[i][0],
            new_ship->coords[i][1]) &&
            new_ship->coords[i][1] < 11 &&
            new_ship->coords[i][0] < 11 &&
            new_ship->coords[i][1] > 0 &&
            new_ship->coords[i][0] > 0)
        {
            return 1;
        }
    }
    return 0;
}

struct ship generate_ship(int area[12][12], int deck_count, int id)
{
    struct ship new_ship;
    int start_x, start_y;
    enum ship_directions direction;
    int* start_coordinates;
    int* multipliers;
    int* stop_coordinates;

    new_ship.deck_count = deck_count;

    time_t t;
    srand((unsigned)time(&t));

    while (1)
    {
        new_ship.is_valid = 1;
        new_ship.id = id;
        new_ship.current_health = deck_count;
        new_ship.max_health = deck_count;
        start_coordinates = get_free_cell(area); // find free cell
        direction = get_random_direction();
        multipliers = get_multipliers(direction); // first x, second y
        start_x = start_coordinates[0];
        start_y = start_coordinates[1];
        //if (!is_neighbors_free(area, start_x, start_y))
        //    continue;
        printf("direction is %d\n", direction);
        if (!set_all_coords_for_ship(start_x, start_y, multipliers, direction, &new_ship, area))
            continue;
        if (!check_borders(&new_ship))
            continue;
        if (!check_all_cell_is_valid(&new_ship, area))
            continue;
        if (!set_around_coords_for_ship(&new_ship, multipliers, area))
            continue;
        print_ship(&new_ship);
        if (new_ship.is_valid)
        {
            fill_area(&new_ship, area);
            //print_area_0(area);
            break;
        }

    }
    return new_ship;
}

