/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
// class Configuration;
class Map;

// class Criminal;
// class RobotS;
// class RobotW;
// class RobotSW;
// class RobotC;
class Robot;

// class ArrayMovingObject;
// class StudyPinkProgram;

class BaseItem;
//class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;

enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C=0, S, W, SW };

class MapElement {
	friend class TestStudyInPink;
protected:
    ElementType type;
public:
    MapElement(ElementType in_type) { this->type = in_type; };
    virtual ~MapElement() {};
    virtual ElementType getType() const { return type; };
};

class Path : public MapElement {
	friend class TestStudyInPink;
	public:
		Path(ElementType type) : MapElement(type) {}
};

class Wall : public MapElement {
	friend class TestStudyInPink;
	public:
		Wall(ElementType type) : MapElement(type) {} 
};

class FakeWall : public MapElement {
	friend class TestStudyInPink;
	private:
		int req_exp;
	public:
		FakeWall(int in_req_exp, ElementType type) : MapElement(type) { req_exp = in_req_exp; }
		int getReqExp() const { return req_exp; }
};

class Position {
	friend class TestStudyInPink;
private:
    int r, c;
public:
    static const Position npos;

		Position(int r=0, int c=0) { this->r = r; this->c = c; }
		Position(const string & str_pos); 

    int getRow() const { return r; }
    int getCol() const { return c; }
    void setRow(int r) { this->r = r; }
    void setCol(int c) { this->c = c; }

		string str() const { return ('(' + to_string(r) + ',' + to_string(c) + ')'); }
		bool isEqual(int in_r, int in_c) const { return (in_r == r && in_c == c); }
};

// abstract class
class MovingObject {
	friend class TestStudyInPink;
protected:
    int index;
    Position pos;
    Map * map;
    string name;

public:
		MovingObject(int index, const Position pos, Map * map, const string & name="") {
			this->index = index;
			this->pos = pos;
			this->map = map;
			this->name = name;
		}
		virtual ~MovingObject() { free(map); }
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const { return pos; }
    virtual void move() = 0;
    virtual string str() const = 0;
		string getName() { return name; }
};

class Map {
	friend class TestStudyInPink;
private:
    int num_rows, num_cols;
		MapElement*** map;
public:
		Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls);
		~Map();
		bool isValid ( const Position & pos , MovingObject * mv_obj ) const;
};

class Character : public MovingObject {
	friend class TestStudyInPink;
	protected:
    int hp, exp, indexMovingRule;
		string moving_rule;
	public:
		Character(int index, const Position & init_pos, Map * map, string name, int init_hp = 0, int init_exp = 0,
				const string & moving_rule = "") : MovingObject(index, init_pos, map, name) { 
			this->hp = max(min(init_hp, 500), 0);
			this->exp = max(min(init_exp, 900), 0);
			indexMovingRule = 0;
			this->moving_rule = moving_rule;
		}
		void nm() {
			hp = max(min(hp, 500), 0);
			exp = max(min(exp, 900), 0);
			// if (hp == 0 || exp == 0) TODO: Cannot move
		}
		void _move(); 
		string _str() const;
		int getEXP() const { return exp; }
		void setEXP(int x) { exp += x; }
		int getHP() const { return hp; }
		void setHP(int x) { hp += x; }
};

class Sherlock : public Character {
	friend class TestStudyInPink;
	private:
		SherlockBag* bag;
	public:
		Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp) :
			Character(index, init_pos, map, "Sherlock", init_hp, init_exp, moving_rule) {}	
		Position getNextPosition();
		void move() {_move(); }
		string str() const { return _str(); }
		void meetRobot(Robot* robot);
		SherlockBag* getBag() { return bag; }
};

class Watson : public Character {
	friend class TestStudyInPink;
	private:
		WatsonBag* bag;
	public:
		Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp) :
			Character(index, init_pos, map, "Watson", init_hp, init_exp, moving_rule) {}
		Position getNextPosition();
		void move() { _move(); }
		string str() const { return _str(); }
		void meetRobot(Robot* robot);
		WatsonBag* getBag() { return bag; }
};

class Criminal : public Character {
	friend class TestStudyInPink;
	private:
		Sherlock* sherlock;
		Watson* watson;

	public:
  	Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson) : 
			Character(index, init_pos, map, "Criminal") {
			this->sherlock = sherlock;
			this->watson = watson;
  	}
		Position getNextPosition();
		void move() { _move(); }
		string str() const { return _str(); }
};

class ArrayMovingObject {
	friend class TestStudyInPink;
	private:
		MovingObject** arr_mv_objs;
		int count, capacity;

	public:
		ArrayMovingObject(int capacity) {
			this->capacity = capacity;
			arr_mv_objs = new MovingObject*[capacity];
		}
		~ArrayMovingObject() { delete[] arr_mv_objs; }
		bool isFull() const { return (count == capacity); }
		bool add(MovingObject * mv_obj);
		MovingObject * get(int index) const { return arr_mv_objs[index]; }
		int size() const { return count; } // return current number of elements in the array
		string str() const;
};

class Configuration {
	friend class TestStudyInPink;
	friend class StudyPinkProgram;

	private:
    int map_num_rows, map_num_cols;
		int max_num_moving_objects;
		int num_walls, num_fake_walls;
		Position* arr_walls;
		Position* arr_fake_walls;
		string sherlock_moving_rule, watson_moving_rule;
		Position sherlock_init_pos, watson_init_pos, criminal_init_pos;
		int sherlock_init_hp, sherlock_init_exp, watson_init_hp, watson_init_exp;
		int num_steps;

	public:
		Configuration(const string & filepath);
		~Configuration() {
			delete[] arr_walls;
			delete[] arr_fake_walls;
		}
		string str() const; 
};

class Robot : public MovingObject {
	friend class TestStudyInPink;
	protected:
		RobotType robot_type;
		BaseItem* item;
		Criminal* criminal;
		Sherlock* sherlock;
		Watson* watson;

		void _move(); 
		int _getDistance(MovingObject* mv_obj) const {
			Position point = mv_obj->getCurrentPosition();
			return abs(point.getRow() - pos.getRow()) + abs(point.getCol() - pos.getCol());
		}
		string _str(int distance) const; 
	public:
		Robot(int index, const Position & init_pos, Map * map, RobotType robot_type, string name, Criminal* criminal = nullptr,
				Sherlock* sherlock = nullptr, Watson* watson = nullptr);
		RobotType getType() { return robot_type; }
		BaseItem* getItem() { return item; }
};

class RobotC : public Robot {
	friend class TestStudyInPink;
	public:
		RobotC(int index, const Position & init_pos, Map * map, Criminal* criminal) :
			Robot(index, init_pos, map, RobotType::C, "RobotC") {
			this->criminal = criminal;
			this->robot_type = robot_type;
		}
		Position getNextPosition() { return criminal->getCurrentPosition(); }
		void move() { _move(); }
		int getDistance(Sherlock* mv_obj) const { return _getDistance(mv_obj); }
		int getDistance(Watson* mv_obj) const { return _getDistance(mv_obj); }
		string str() const { return _str(-1); }
};

class RobotS : public Robot {
	friend class TestStudyInPink;
	public:
		RobotS(int index, const Position & init_pos, Map * map,
				RobotType robot_type, Criminal* criminal, Sherlock* Sherlock) : Robot(index, init_pos, map, RobotType::S, "RobotS") {
			this->criminal = criminal;
			this->robot_type = robot_type;
			this->sherlock = Sherlock;
		}
		Position getNextPosition(); 
		void move() { _move(); }
		int getDistance() const { return _getDistance(sherlock); }
		string str() const { return _str(getDistance()); }
};

class RobotW : public Robot {
	friend class TestStudyInPink;
	public:
		RobotW(int index, const Position & init_pos, Map * map,
				RobotType robot_type, Criminal* criminal, Watson* watson) : Robot(index, init_pos, map, RobotType::W, "RobotW") {
			this->criminal = criminal;
			this->robot_type = robot_type;
			this->watson = watson;
		}
		Position getNextPosition(); 
		void move() { _move(); }
		int getDistance() const { return _getDistance(watson); }
		string str() const { return _str(getDistance()); }
};

class RobotSW : public Robot {
	friend class TestStudyInPink;
	public:
		RobotSW(int index, const Position & init_pos, Map * map,
				RobotType robot_type, Criminal* criminal, Sherlock* sherlock, Watson* watson) :
			Robot(index, init_pos, map, RobotType::SW, "RobotSW") {
			this->criminal = criminal;
			this->robot_type = robot_type;
			this->sherlock = sherlock;
			this->watson = watson;
		}
		Position getNextPosition(); 
		void move() { _move(); }
		int getDistance() const {
			int distanceToSherlock = _getDistance(sherlock);
			int distanceToWatson = _getDistance(watson);
			return distanceToSherlock + distanceToWatson;
		}
		string str() const { return _str(getDistance()); }
};

// abstract class
class BaseItem {
	friend class TestStudyInPink;
	private:
		ItemType itemType;
	public:
		BaseItem(ItemType itemType) { this->itemType = itemType; }
		virtual bool canUse(Character* obj, Robot * robot) = 0;
		virtual void use(Character* obj, Robot * robot) = 0;
		ItemType getType() { return itemType; }
};

class MagicBook : public BaseItem {
	friend class TestStudyInPink;
	public:
		MagicBook() : BaseItem(ItemType::MAGIC_BOOK) {};
		bool canUse(Character* obj, Robot * robot) { return (obj->getEXP() <= 350); }
		void use(Character* obj, Robot * robot) { obj->setEXP(obj->getEXP() * 5 / 4); obj->nm(); }
};

class EnergyDrink: public BaseItem {
	friend class TestStudyInPink;
	public:
		EnergyDrink() : BaseItem(ItemType::ENERGY_DRINK) {};
		bool canUse(Character* obj, Robot * robot) { return (obj->getHP() <= 100); }
		void use(Character* obj, Robot * robot) { obj->setHP(obj->getHP() * 6 / 5); obj->nm(); }
};

class FirstAid: public BaseItem {
	friend class TestStudyInPink;
	public:
		FirstAid() : BaseItem(ItemType::FIRST_AID) {};
		bool canUse(Character* obj, Robot * robot) { return (obj->getHP() <= 100 || obj->getEXP() <= 350); }
		void use(Character* obj, Robot * robot) { obj->setHP(obj->getHP() * 3 / 2); obj->nm(); }
};

class ExcemptionCard: public BaseItem {
	friend class TestStudyInPink;
	public:
		ExcemptionCard() : BaseItem(ItemType::EXCEMPTION_CARD) {};
		bool canUse(Character* obj, Robot * robot) { return (obj->getName() == "Sherlock" && obj->getHP() % 2 == 1); }
		void use(Character* obj, Robot * robot) { if (robot != nullptr) delete robot; }
};

class PassingCard: public BaseItem {
	friend class TestStudyInPink;
	private:
		string challenge;
	public:
		PassingCard(string challenge) : BaseItem(ItemType::PASSING_CARD) { this->challenge = challenge; }
		bool canUse(Character* obj, Robot * robot) { return (obj->getName() == "Watson" && obj->getHP() % 2 == 0); }
		void use(Character* obj, Robot * robot) {
			if (robot != nullptr) {
				if (challenge != robot->getName() && challenge != "all") {
					obj->setEXP(obj->getEXP() - 50);
					obj->nm();
				}
				delete robot;
			}
		}
};

struct Node {
	Node* ptr;
	BaseItem* data;
};

class BaseBag {
	friend class TestStudyInPink;
	protected:
		Character* obj;
		int count;
		Robot* robot;
		Node* head;
	public:
		BaseBag(Character* obj) { this->obj = obj; head = nullptr; robot = nullptr; count = 0; }
		virtual void meetRobot(Robot* robot) { this->robot = robot; }
		virtual bool insert(BaseItem* item) { return false; }
		virtual BaseItem* get();
		virtual BaseItem* get(ItemType itemType); 
		virtual string str() const;
};

class SherlockBag : public BaseBag {
	friend class TestStudyInPink;
	public:
		SherlockBag(Sherlock* sherlock) : BaseBag(sherlock) {}
		virtual bool insert(BaseItem* item); 
};

class WatsonBag : public BaseBag {
	friend class TestStudyInPink;
	public:
		WatsonBag(Watson* watson) : BaseBag(watson) {}
		virtual bool insert(BaseItem* item); 
};

class StudyPinkProgram {
	friend class TestStudyInPink;
private:
    // Sample attributes
    Configuration * config;
    
    Sherlock * sherlock;
    Watson * watson;
    Criminal * criminal;
    
    Map * map;
    ArrayMovingObject * arr_mv_objs;

		int validCriminalSteps, robotCount;


public:
		StudyPinkProgram(const string & config_file_path, Sherlock* sherlock, Watson* watson, Criminal* criminal, Map* map, ArrayMovingObject* arr_mv_objs) {
			config = new Configuration(config_file_path);
			this->sherlock = sherlock;
			this->watson = watson;
			this->criminal = criminal;
			this->map = map;
			this->arr_mv_objs = arr_mv_objs;
			validCriminalSteps = robotCount = 0;
		}

    bool isStop() const;

    void printResult() const {
			Position criminalPos = criminal->getCurrentPosition();
        if (sherlock->getCurrentPosition().isEqual(criminalPos.getRow(), criminalPos.getCol()))
            cout << "Sherlock caught the criminal" << endl;
        else if (watson->getCurrentPosition().isEqual(criminalPos.getRow(), criminalPos.getCol()))
            cout << "Watson caught the criminal" << endl;
        else
            cout << "The criminal escaped" << endl;
    }

    void printStep(int si) const {
        cout << "Step: " << setw(4) << setfill('0') << si
            << "--"
            << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }

		void sendGift();

    void run(bool verbose) {
        // Note: This is a sample code. You can change the implementation as you like.
        // TODO
        for (int istep = 0; istep < config->num_steps; ++istep) {
            for (int i = 0; i < arr_mv_objs->size(); ++i) {
							if (!arr_mv_objs->isFull()) {
								if (arr_mv_objs->get(i)->getName() == "Criminal") {
									Position initialPos = criminal->getCurrentPosition();
									arr_mv_objs->get(i)->move();
									if (!criminal->getCurrentPosition().isEqual(initialPos.getRow(), initialPos.getCol()))
										validCriminalSteps++;
									if (validCriminalSteps == 3) {
										if (robotCount == 0) {
											arr_mv_objs->add(new RobotC(arr_mv_objs->size(), initialPos, map, criminal));
										} else {
#define dist(x,y) abs(x.getRow()-y.getRow())+abs(x.getCol()-y.getCol())
											int sherlockDist = dist(sherlock->getCurrentPosition(), initialPos);
											int watsonDist = dist(watson->getCurrentPosition(), initialPos);
											if (sherlockDist < watsonDist)
												arr_mv_objs->add(new RobotS(arr_mv_objs->size(), initialPos, map, RobotType::S, criminal, sherlock));
											else if (sherlockDist > watsonDist)
												arr_mv_objs->add(new RobotW(arr_mv_objs->size(), initialPos, map, RobotType::W, criminal, watson));
											else
												arr_mv_objs->add(new RobotSW(arr_mv_objs->size(), initialPos, map, RobotType::SW, criminal, sherlock, watson));
										}
									}
								}
							}
                if (isStop()) {
                    printStep(istep);
                    break;
                }
                if (verbose)
                    printStep(istep);
            }
        }
        printResult();
    }

    ~StudyPinkProgram();
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
