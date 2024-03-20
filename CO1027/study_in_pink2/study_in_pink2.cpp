#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

const Position Position::npos = Position(-1, -1);

Position::Position(const string & str_pos) {
	for (int i = 1; i < str_pos.length() - 1; i++) {
		if (str_pos[i] == ',') {
			this->r = stoi(str_pos.substr(1, i - 1));
			this->c = stoi(str_pos.substr(i + 1, str_pos.length() - 1 - (i + 1)));
		}
	}
}

Map::Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls) : num_rows(num_rows), num_cols(num_cols) {
	this->num_rows = num_rows;
	this->num_cols = num_cols;
	map = new MapElement**[num_rows];
	for (int i = 0; i < num_rows; i++) {
		map[i] = new MapElement*[num_cols];
		for (int j = 0; j < num_cols; j++) {
			// check if (i,j) is in array_walls
			for (int k = 0; k < num_walls; k++) {
				if (array_walls[k].getRow() == i && array_walls[k].getCol() == j) {
					map[i][j] = new Wall(WALL);
					break;
				}
				if (array_fake_walls[k].getRow() == i && array_fake_walls[k].getCol() == j) {
					map[i][j] = new FakeWall((i * 257 + j * 139 + 89) % 900 + 1, FAKE_WALL);
					break;
				}
			}
			if (map[i][j] == nullptr)
				map[i][j] = new Path(PATH);
		}
	}
}

Map::~Map() {
	/*
	for (int i = 0; i < num_rows; i++)
		delete[] map[i];
	delete map;*/
}

bool Map::isValid(const Position &pos, MovingObject *mv_obj) const {
	// kiem tra cham bien tren/trai
	if (pos.getCol() < 0 || pos.getRow() < 0) return false;
	string name = mv_obj->getName();

	if (pos.getRow() >= num_rows || pos.getCol() >= num_cols) return false;

	switch (map[pos.getRow()][pos.getCol()]->getType()) {
		case WALL: return false;
		case FAKE_WALL:
				if (name == "Sherlock" || name == "Criminal") return true;
				if (name == "Watson") {
					Watson *miniWatson = (Watson*)(mv_obj);
					FakeWall *miniFakeWall = (FakeWall*)(map[pos.getRow()][pos.getCol()]);
					if (miniWatson->getEXP() >= miniFakeWall->getReqExp()) {
						free(miniWatson);
						free(miniFakeWall);
						return true;
					}
				}
		case PATH: return true;
		default: return false;
	}
}

void Character::_move() {
	Position nextPosition = getNextPosition();
	if (name == "Sherlock" || name == "Watson") {
		if (hp > 0 && exp > 0 &&
				!nextPosition.isEqual(Position::npos.getRow(), Position::npos.getCol()))
			pos = nextPosition;
	} else {
		if (!nextPosition.isEqual(Position::npos.getRow(), Position::npos.getCol()))
			pos = nextPosition;
	}
}

string Character::_str() const {
	string result = name + "[index=" + to_string(index) + ";pos=" + pos.str();
	if (name == "Sherlock" || name == "Watson")
		result += ";moving_rule=" + moving_rule;
	result += ']';
	return result;
}

Position getNextPos_sherlock_watson(int indexMovingRule, string moving_rule, Position pos, Map* map, MovingObject* mv_obj) {
	int currentIndex = (indexMovingRule + 1) % moving_rule.length();
	Position nextPosition;
	pair<char, pair<int, int>> coordinates[] = { {'U', {-1, 0}}, {'D', {1, 0}}, {'L', {0, -1}}, {'R', {0, 1}} };
	for (int i = 0; i < 4; i++) {
		if (moving_rule[currentIndex] == coordinates[i].first) {
			nextPosition = Position(pos.getRow() + coordinates[i].second.first, pos.getCol() + coordinates[i].second.second);
			if (map->isValid(nextPosition, mv_obj))
				return nextPosition;
		}
	}
	return Position::npos;
}

Position Sherlock::getNextPosition() {
	return getNextPos_sherlock_watson(indexMovingRule, moving_rule, pos, map, this);
}

Position Watson::getNextPosition() {
	return getNextPos_sherlock_watson(indexMovingRule, moving_rule, pos, map, this);
}

Position getNextPos_criminal_robot(Position pos, Map* map, MovingObject* mv_obj, Position sherlockPos = Position::npos, Position watsonPos = Position::npos) {
#define dist(x,y) abs(x.getRow()-y.getRow())+abs(x.getCol()-y.getCol())
#define null(x) (x.getRow()==Position::npos.getRow()&&x.getCol()==Position::npos.getCol())
#define maximizeDist if(max_minDist<dist[i]){max_minDist=dist[i];returnValue=nextPosition;}
#define minimizeDist if(max_minDist>dist[i]){max_minDist=dist[i];returnValue=nextPosition;}
	pair<int, int> coordinates[] = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };
	char a = ((mv_obj->getName() == "Criminal") ? -1 : 1);
	int dist[4] = { a * INT_MAX, a * INT_MAX, a * INT_MAX, a * INT_MAX }, max_minDist = a * INT_MAX;
	Position returnValue = Position::npos;
	
	for (int i = 0; i < 4; i++) {
		Position nextPosition = Position(pos.getRow() + coordinates[i].first, pos.getCol() + coordinates[i].second);
		if (map->isValid(nextPosition, mv_obj)) {
			if ((mv_obj->getName() == "Criminal" || mv_obj->getName() == "RobotSW") &&
					!null(sherlockPos) && !null(watsonPos)) {
				dist[i] = dist(nextPosition, sherlockPos) + dist(nextPosition, watsonPos);
				if (mv_obj->getName() == "Criminal") { maximizeDist }
				else { minimizeDist }
			}
			if ((mv_obj->getName() == "RobotS" || mv_obj->getName() == "RobotW") && !null(sherlockPos)) {
				dist[i] = dist(nextPosition, sherlockPos);
				minimizeDist
			}
		} 
	}
#undef minimizeDist
#undef maximizeDist
#undef dist
	return returnValue;
}

Position Criminal::getNextPosition() {
	return getNextPos_criminal_robot(pos, map, this, sherlock->getCurrentPosition(), watson->getCurrentPosition());
}

void processPositionArrays(Position* &arr, string value) {
	int begin = -1, wallCount = 0;
	for (int i = 1; i < value.length() - 1; i++) {
		switch (value[i]) {
			case '(': begin = i + 1; break;
			case ')':
				Position newWall = Position(value.substr(begin, i - begin));
				arr[wallCount++] = newWall;
				break;
		}
	}
}

bool ArrayMovingObject::add(MovingObject* mv_obj) {
	if (count < capacity) {
		arr_mv_objs[count++] = mv_obj;
		return true;
	}
	return false;
}

string ArrayMovingObject::str() const {
	string result = "ArrayMovingObject[count=" + to_string(count) + ";capacity=" + to_string(capacity) + ";";
	for (int i = 0; i < count; i++) {
		result += arr_mv_objs[i]->str();
		if (i < count - 1) result += ';';
	}
	result += ']';
	return result;
}

Configuration::Configuration(const string & filepath) {
	// init variables
	arr_walls = new Position[num_walls];
	arr_fake_walls = new Position[num_fake_walls];
	// reading input
	ifstream input(filepath);
	if (input.is_open()) {
		string line;
		while (cin >> line) {
			int equalPosition = line.find('=');
			string key = line.substr(0, equalPosition);
			string value = line.substr(equalPosition + 1);
			if (key == "MAP_NUM_ROWS") map_num_rows = stoi(value);
			else if (key == "MAP_NUM_COLS") map_num_cols = stoi(value);
			else if (key == "MAX_NUM_MOVING_OBJECTS") max_num_moving_objects = stoi(value);
			else if (key == "ARRAY_WALLS") processPositionArrays(arr_walls, value);
			else if (key == "ARRAY_FAKE_WALLS") processPositionArrays(arr_fake_walls, value);
			else if (key == "SHERLOCK_MOVING_RULE") sherlock_moving_rule = value;
			else if (key == "SHERLOCK_INIT_POS") sherlock_init_pos = Position(value);
			else if (key == "WATSON_MOVING_RULE") watson_moving_rule = value;
			else if (key == "WATSON_INIT_POS") watson_init_pos = Position(value);
			else if (key == "CRIMINAL_INIT_POS") criminal_init_pos = Position(value);
			else if (key == "NUM_STEPS") num_steps = stoi(value);
			else if (key == "SHERLOCK_INIT_HP") sherlock_init_hp = stoi(value);
			else if (key == "SHERLOCK_INIT_EXP") sherlock_init_exp = stoi(value);
			else if (key == "WATSON_INIT_HP") watson_init_hp = stoi(value);
			else if (key == "WATSON_INIT_EXP") watson_init_exp = stoi(value);
		}
		input.close();
	}
}

string Configuration::str() const {
	string result = "Configuration[\nMAP_NUM_ROWS=" + to_string(map_num_rows) +
		"\nMAP_NUM_COLS=" + to_string(map_num_cols) +
		"\nMAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects) +
		"\nNUM_WALLS=" + to_string(num_walls) +
		"\nARRAY_WALLS=[";
	for (int i = 0; i < num_walls; i++) {
		result += arr_walls[i].str();
		if (i < num_walls - 1) result += ';';
	}
	result += "]\nNUM_FAKE_WALLS=" + to_string(num_fake_walls) +
		"\nARRAY_FAKE_WALLS=[";
	for (int i = 0; i < num_fake_walls; i++) {
		result += arr_fake_walls[i].str();
		if (i < num_fake_walls - 1) result += ';';
	}
	result += "]\nSHERLOCK_MOVING_RULE=" + sherlock_moving_rule +
		"SHERLOCK_INIT_POS=" + sherlock_init_pos.str() +
		"\nSHERLOCK_INIT_HP=" + to_string(sherlock_init_hp) +
		"\nSHERLOCK_INIT_EXP=" + to_string(sherlock_init_exp) +
		"\nWATSON_MOVING_RULE=" + watson_moving_rule +
		"\nWATSON_INIT_POS=(" + watson_init_pos.str() +
		"\nWATSON_INIT_HP=" + to_string(watson_init_hp) +
		"\nWATSON_INIT_EXP=" + to_string(watson_init_exp) +
		"\nCRIMINAL_INIT_POS=" + criminal_init_pos.str() +
		"\nNUM_STEPS=" + to_string(num_steps);
	return result;
}

Robot::Robot(int index, const Position & init_pos, Map * map, RobotType robot_type, string name, Criminal* criminal,
				Sherlock* sherlock, Watson* watson) : MovingObject(index, init_pos, map, name) {
	this->robot_type = robot_type;
	this->criminal = criminal;
	this->sherlock = sherlock;
	this->watson = watson;
	// init item
	int p = init_pos.getRow() * init_pos.getCol(), s = 0;
	while (floor(log10(p) + 1) > 1) {
		while (p > 0) {
			s += p % 10;
			p /= 10;
		}
		p = s;
	}
	switch (s) {
		case 0: case 1: item = new MagicBook; break;
		case 2: case 3: item = new EnergyDrink; break;
		case 4: case 5: item = new FirstAid; break;
		case 6: case 7: item = new ExcemptionCard; break;
		case 8: case 9:
			int t = (init_pos.getRow() * 11 + init_pos.getCol()) % 4;
			switch (t) {
				case 0: item = new PassingCard("RobotS"); break;
				case 1: item = new PassingCard("RobotC"); break;
				case 2: item = new PassingCard("RobotSW"); break;
				case 3: item = new PassingCard("all"); break;
			}
			break;
	}
}

void Robot::_move() {
	char count = ((robot_type == RobotType::SW) ? 1 : 2);
	for (int i = 0; i < count; i++) {
		Position nextPosition = getNextPosition();
		if (!nextPosition.isEqual(Position::npos.getRow(), Position::npos.getCol()))
			pos = nextPosition;
	}
}

string Robot::_str(int distance) const {
	string result = "Robot[pos=" + pos.str() + ";type=";
	switch (robot_type) {
		case C: result += 'C'; break;
		case S: result += 'S'; break;
		case W: result += 'W'; break;
		case SW: result += "SW"; break;
	}
	result += ";dist=" + ((distance > 0) ? to_string(distance) : "") + ']';
	return result;
}

Position RobotS::getNextPosition() {
	return getNextPos_criminal_robot(pos, map, this, sherlock->getCurrentPosition());
}

Position RobotW::getNextPosition() {
	return getNextPos_criminal_robot(pos, map, this, watson->getCurrentPosition());
}

Position RobotSW::getNextPosition() {
	return getNextPos_criminal_robot(pos, map, this, sherlock->getCurrentPosition(), watson->getCurrentPosition());
}

bool SherlockBag::insert(BaseItem* item) {
	if (count < 13) {
		Node node = { nullptr, item };
		if (head == nullptr) head = &node;
		else {
			node.ptr = head;
			head = &node;
		}
		return true;
	}
	return false;
}

bool WatsonBag::insert(BaseItem* item) {
	if (count < 15) {
		Node node = { nullptr, item };
		if (head == nullptr) head = &node;
		else {
			node.ptr = head;
			head = &node;
		}
		return true;
	}
	return false;
}

BaseItem* BaseBag::get() {
	Node* previousNode = head;
	for (Node* item = head; item != nullptr; item = item->ptr) {
		if (item->data->canUse(obj, robot)) {
			BaseItem* returnItem = item->data;
			// bring first element to here
			previousNode->ptr = head->ptr;
			head->ptr = item->ptr;
			delete item;
			return returnItem;
		}
		previousNode = item;
	}
	return nullptr;
}
BaseItem* BaseBag::get(ItemType itemType) {
	Node* previousNode = head;
	for (Node* item = head; item != nullptr; item = item->ptr) {
		if (item->data->getType() == itemType) {
			BaseItem* returnItem = item->data;
			// bring first element to here
			previousNode->ptr = head->ptr;
			head->ptr = item->ptr;
			delete item;
			return returnItem;
		}
		previousNode = item;
	}
	return nullptr;
}
string BaseBag::str() const {
	string result = "Bag[count=" + to_string(count) + ';';
	for (Node* item = head; item != nullptr; item = item->ptr) {
		switch (item->data->getType()) {
			case ItemType::MAGIC_BOOK: result += "MAGIC_BOOK";
			case ItemType::ENERGY_DRINK: result += "ENERGY_DRINK";
			case ItemType::FIRST_AID: result += "FIRST_AID";
			case ItemType::EXCEMPTION_CARD: result += "EXCEMPTION_CARD";
			case ItemType::PASSING_CARD: result += "PASSING_CARD";
		}
		if (item->ptr != nullptr) result += ',';
	}
	return result;
}

void Sherlock::meetRobot(Robot* robot) {
	// get item from bag
	BaseItem* item = bag->get(ItemType::EXCEMPTION_CARD);
	if (item != nullptr) {
		if (!item->canUse(this, robot)) {
			switch (robot->getType()) {
				case S:
					if (exp > 400) {
						bag->insert(robot->getItem());
						delete robot;
					}
					else
						exp = exp * 9 / 10;
					break;
				case W:
					bag->insert(robot->getItem());
					delete robot;
					break;
				case C:
					if (exp > 500) {
						// TODO: Criminal captured
					} else {
						bag->insert(robot->getItem());
						delete robot;
					}
					break;
				case SW:
					if (exp > 300 && hp > 335) {
						bag->insert(robot->getItem());
						delete robot;
					} else {
						hp = hp * 85 / 100;
						exp = exp * 85 / 100;
						nm();
						// TODO: if HP/EXP = 0?
					}
					break;
			}
		}
	}
	// get item from bag
	item = bag->get();
	if (item->canUse(this, nullptr)) item->use(this, nullptr);
}

void Watson::meetRobot(Robot* robot) {
	// get item from bag
	BaseItem* item = bag->get(ItemType::EXCEMPTION_CARD);
	if (item != nullptr) {
		if (!item->canUse(this, robot)) {
			switch (robot->getType()) {
				case W:
					if (exp > 350) {
						bag->insert(robot->getItem());
						delete robot;
					} else {
						hp = hp * 95 / 100;
						nm();
					}
					break;
				case C:
					bag->insert(robot->getItem());
					delete robot;
					// But cannot capture the criminal
					break;
				case SW:
					if (exp > 600 && hp > 165) {
						bag->insert(robot->getItem());
						delete robot;
					} else {
						hp = hp * 85 / 100;
						exp = exp * 85 / 100;
						nm();
					}
					break;
				default:
					break;
			}
		}
	}
	// get item from bag
	item = bag->get();
	if (item->canUse(this, nullptr)) item->use(this, nullptr);
}

void StudyPinkProgram::sendGift() {
	Position watsonPos = watson->getCurrentPosition();
	if (sherlock->getCurrentPosition().isEqual(watsonPos.getRow(), watsonPos.getCol())) {
		// Sherlock tang qua Watson
		SherlockBag* sherlockBag = sherlock->getBag();
		WatsonBag* watsonBag = watson->getBag();
		BaseItem* taken_out = watsonBag->get(ItemType::EXCEMPTION_CARD);
		if (taken_out != nullptr) {
			watsonBag->insert(taken_out);
			BaseItem* card = sherlockBag->get(ItemType::EXCEMPTION_CARD);
			while (card != nullptr) {
				watsonBag->insert(card);
				card = sherlockBag->get(ItemType::EXCEMPTION_CARD);
			}
		}
		// Watson tang qua Sherlock
	  taken_out = sherlockBag->get(ItemType::PASSING_CARD);
		if (taken_out != nullptr) {
			sherlockBag->insert(taken_out);
			BaseItem* card = watsonBag->get(ItemType::PASSING_CARD);
			while (card != nullptr) {
				sherlockBag->insert(card);
				card = watsonBag->get(ItemType::PASSING_CARD);
			}
		}
	}
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
