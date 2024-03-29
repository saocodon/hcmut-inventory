#include <iostream>
#include <fstream>
using namespace std;

int map_num_rows;

int main() {
	// reading input
	ifstream input("sa_tc_02_config");
	if (input.is_open()) {
		string line;
		while (input.good()) {
			input >> line;
			cout << line << '\n';
			int equalPosition = line.find('=');
			string key = line.substr(0, equalPosition);
			string value = line.substr(equalPosition + 1);
			if (key == "MAP_NUM_ROWS") map_num_rows = stoi(value);
			//else if (key == "MAP_NUM_COLS") map_num_cols = stoi(value);
			//else if (key == "MAX_NUM_MOVING_OBJECTS") max_num_moving_objects = stoi(value);
			//else if (key == "ARRAY_WALLS") processPositionArrays(arr_walls, value);
			//else if (key == "ARRAY_FAKE_WALLS") processPositionArrays(arr_fake_walls, value);
			//else if (key == "SHERLOCK_MOVING_RULE") sherlock_moving_rule = value;
			//else if (key == "SHERLOCK_INIT_POS") sherlock_init_pos = Position(value);
			//else if (key == "WATSON_MOVING_RULE") watson_moving_rule = value;
			//else if (key == "WATSON_INIT_POS") watson_init_pos = Position(value);
			//else if (key == "CRIMINAL_INIT_POS") criminal_init_pos = Position(value);
			//else if (key == "NUM_STEPS") num_steps = stoi(value);
			//else if (key == "SHERLOCK_INIT_HP") sherlock_init_hp = stoi(value);
			//else if (key == "SHERLOCK_INIT_EXP") sherlock_init_exp = stoi(value);
			//else if (key == "WATSON_INIT_HP") watson_init_hp = stoi(value);
			//else if (key == "WATSON_INIT_EXP") watson_init_exp = stoi(value);
		}
		input.close();
	}
}

