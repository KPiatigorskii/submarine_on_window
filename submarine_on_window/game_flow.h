#pragma once
enum turn { USER =0, PC=1};
enum game_status { GAME_IS_ON=0, GENERATE_IS_ON=1, GAME_IS_OFF};

struct game_flow {
	enum turn current_turn;
	enum game_status game_status;
	int user_turn_counter;
	int pc_turn_counter;

};

int* get_next_hit_cell()
{
	int* arr = malloc(2);
	time_t t;
	srand((unsigned)time(&t));
	arr[0] = 1;
	arr[1] = 1;
	return arr;
}