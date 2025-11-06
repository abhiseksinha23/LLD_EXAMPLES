// SANKE AND LADDER EXAMPLE

/*
FUNCTION REQUIREMENTS

- SIZE OF THE BOARD SHOULD BE SCALABLE.
- THERE ARE STANDARD GAME RULES AND SHOULD BE FURTHER EXTENDIBLE.
- ALLOW ON APP NOTIFICATION FOR SNAKE AND LADDER, MOVES, WINS, AND DRAWS.
- THERE CAN BE GAME SETUP STRATEGIES --> RANDON SETUP, CUSTOM SETUP, STANDARD SETUP, ETC.

NON FUNCTION REQUIREMENTS
- SCALABLE
- SOLID PRINCIPLE
- LOOSLY COUPLED
*/

#include<bits/stdc++.h>
using namespace std;

// -------------------------------------------
// BOARD AND BOARD ENTITY
// -------------------------------------------

class BoardEntity {
private:
	string type;
	int start;
	int end;

public:

	BoardEntity(string type, int start, int end) {
		this->type = type;
		this->start = start;
		this->end = end;
	}

	string getType() {
		return type;
	}

	int getStart() {
		return start;
	}

	int getEnd() {
		return end;
	}
};


class Board {
private:
	int size;
	vector<shared_ptr<BoardEntity>> entities;
	unordered_map<int, shared_ptr<BoardEntity>> entityMap;

public:
	Board(int size) {
		this->size = size;
	}

	int getSize() {
		return size;
	}

	void addEntity(shared_ptr<BoardEntity> entity) {
		entities.push_back(entity);
		entityMap[entity->getStart()] = entity;
	}

	bool checkEntity(int place) {
		return (entityMap.find(place) != entityMap.end());
	}

	shared_ptr<BoardEntity> getEntity(int place) {
		return entityMap[place];
	}

	void display() {
		cout << "Board (size : " << size <<  ") entity placements : " << endl;
		for(auto entity : entities) {
			cout << entity->getType() << " : starts at " << entity->getStart() <<" , ends at " << entity->getEnd() << endl;
		}
	}
};

// -------------------------------------------
// PLAYER
// -------------------------------------------

class Player {
private:
	string name;
	int position;
	int winCount;

public:

	Player(string name) {
		this->name = name;
		position = 1;
		winCount = 0;
	}

	string getName() {
		return name;
	}

	int getPosition() {
		return position;
	}

	void updatePosition(int position) {
		this->position = position;
	}

	void increaseWinCount() {
		winCount++;
	}

	int getWinCount() {
		return winCount;
	}
};

// -------------------------------------------
//  NOTIFICATION SERVICE
// -------------------------------------------

class IObserver {
public:
	virtual void update(string msg) = 0;
};

class InAppObserver : public IObserver {
public:
	void update(string msg) override {
		cout << "[In-App Notification] " << msg << endl;
	}
};

class NotificationService {
private:
	static NotificationService *instance;
	static mutex mtx;

	unordered_set<shared_ptr<IObserver>> observers;

	NotificationService() {}


public:

	static NotificationService* getInstance() {
		if(instance == nullptr) {
			lock_guard<mutex> lock(mtx);
			if(instance == nullptr) {
				instance = new NotificationService();
			}
		}

		return instance;
	}

	void addObserver(shared_ptr<IObserver> obs) {
		observers.insert(obs);
	}

	void removeObserver(shared_ptr<IObserver> obs) {
		observers.erase(obs);
	}

	void notify(string msg) {
		for(auto obs : observers) {
			obs->update(msg);
		}
	}
};

NotificationService* NotificationService::instance = nullptr;
mutex NotificationService::mtx;

// -------------------------------------------
//  DICE
// -------------------------------------------

class Dice {
private:
	int size;
public:
	Dice(int size) {
		this->size = size;
	}

	int rollDice() {
		// LOGIC TO ROLL THE DICE.
		int randomNum = rand() % 6;
    	return (randomNum + 1);
	}
};

// -------------------------------------------
//  RULE
// -------------------------------------------

class Rule {
public:
	virtual bool checkWin(int initial, int roll, shared_ptr<Board>Board) = 0;
	virtual bool checkValidMove(int initial, int roll, shared_ptr<Board>Board) = 0;
	virtual int findFinalPosition(int initial, int roll, shared_ptr<Board>Board) = 0;
};


class StandardRule : public Rule {
public:
	bool checkWin(int initial, int roll, shared_ptr<Board>Board) override {
		int size = Board->getSize();
		int pos = findFinalPosition(initial, roll, Board);
		return (pos == size);
	}

	int findFinalPosition(int initial, int roll, shared_ptr<Board>Board) override {
		int finalPos = initial + roll;

		if(Board->checkEntity(finalPos) == false) {
			return finalPos;
		}

		auto entity = Board->getEntity(finalPos);
		return entity->getEnd();
	}

	bool checkValidMove(int initial, int roll, shared_ptr<Board>Board) override {
		int size = Board->getSize();
		if(initial + roll <= size) return true;
		return false;
	}
};

enum class RuleType {
	STANDARD
};

class RuleFactory {
public:
	static shared_ptr<StandardRule> createRule(RuleType rule) {
		if(rule == RuleType::STANDARD) {
			return make_shared<StandardRule>();
		}

		return nullptr;
	}
};

// -------------------------------------------
//  SETUP STRATEGY
// -------------------------------------------

enum class DifficultyType {
	EASY,
	MEDIUM,
	HARD
};

class SetupStrategy {
public:
	virtual void setupBoard(shared_ptr<Board> board) = 0;
};

class StandardSetup : public SetupStrategy {
public:
	void setupBoard(shared_ptr<Board> board) override {
		// Create entities at Standard Positions
		// Add entities to the board;
		
		// Only works for 10x10 board (100 cells)
        // if(board->getBoardSize() != 100) {
        //     cout << "Standard setup only works for 10x10 board!" << endl;
        //     return;
        // }
        
        // Standard snake positions (based on traditional board)
        // board->addBoardEntity(new Snake(99, 54));
        // board->addBoardEntity(new Snake(95, 75));
        // board->addBoardEntity(new Snake(92, 88));
        // board->addBoardEntity(new Snake(89, 68));
        // board->addBoardEntity(new Snake(74, 53));
        // board->addBoardEntity(new Snake(64, 60));
        // board->addBoardEntity(new Snake(62, 19));
        // board->addBoardEntity(new Snake(49, 11));
        // board->addBoardEntity(new Snake(46, 25));
        // board->addBoardEntity(new Snake(16, 6));
        
        // // Standard ladder positions
        // board->addBoardEntity(new Ladder(2, 38));
        // board->addBoardEntity(new Ladder(7, 14));
        // board->addBoardEntity(new Ladder(8, 31));
        // board->addBoardEntity(new Ladder(15, 26));
        // board->addBoardEntity(new Ladder(21, 42));
        // board->addBoardEntity(new Ladder(28, 84));
        // board->addBoardEntity(new Ladder(36, 44));
        // board->addBoardEntity(new Ladder(51, 67));
        // board->addBoardEntity(new Ladder(71, 91));
        // board->addBoardEntity(new Ladder(78, 98));
        // board->addBoardEntity(new Ladder(87, 94));
	}
};


class RandomSetup : public SetupStrategy {
private:
	DifficultyType difficulty;
public:
	void setDifficulty(DifficultyType type) {
		difficulty = type;
	}

	void setupBoard(shared_ptr<Board> board) override {
		// Create entities at Standard Positions
		// Add entities to the board;

		if(difficulty == DifficultyType::EASY) {

		} else if(difficulty == DifficultyType::MEDIUM) {

		} else if(difficulty == DifficultyType::HARD) {

		}
	}
};

class CustomSetup : public SetupStrategy {
private:
	vector<shared_ptr<BoardEntity>> entityList;

public:
	void addEntity(shared_ptr<BoardEntity> entity) {
		entityList.push_back(entity);
	} 

	void setupBoard(shared_ptr<Board> board) override {
		for(auto entity : entityList) {
			board->addEntity(entity);
		}
	}
};

enum class SetupType {
	RANDOM,
	STANDARD,
	CUSTOM
};

class SetupStrategyFactory {
public:
	static shared_ptr<SetupStrategy> createSetupStrategy(SetupType type) {
		if(type == SetupType::RANDOM) {
			return make_shared<RandomSetup>();
		} else if(type == SetupType::STANDARD) {
			return make_shared<StandardSetup>();
		} else if(type == SetupType::CUSTOM) {
			return make_shared<CustomSetup>();
		} else {
			return nullptr;
		}
	}
};

// -------------------------------------------
//  GAME FACADE
// -------------------------------------------

class GameFacade {
private:
	shared_ptr<Board>board;
	deque<shared_ptr<Player>>players;
	shared_ptr<Rule> rule;
	shared_ptr<SetupStrategy> setupStrategy;
	bool gameOver;
	shared_ptr<Dice> dice;

public:
	GameFacade(int size) {
		board = make_shared<Board>(size);
		gameOver = false;
		dice = make_shared<Dice>(6);

		shared_ptr<InAppObserver> inapp = make_shared<InAppObserver>();
		NotificationService::getInstance()->addObserver(inapp);
	}

	bool isGameOver() {
		return gameOver;
	}

	void setRule(shared_ptr<Rule> rule) {
		this->rule = rule;
	}

	void setupBoard(shared_ptr<SetupStrategy> setupStrategy) {
		this->setupStrategy = setupStrategy;
		this->setupStrategy->setupBoard(board);			
	}

	void addPlayer(shared_ptr<Player>player) {
		players.push_back(player);
	}

	void notify(string msg) {
		NotificationService::getInstance()->notify(msg);
	}

	void play() {
		// Logic to play the game.

		while(gameOver == false) {

			if(players.size() <= 1) {
				gameOver = true;
				continue;
			}

			auto player = players.front();

			string msg = player->getName() + " your chance, rolling the dice";
			notify(msg);

			int roll = dice->rollDice();

			msg = player->getName() + " your got : " + to_string(roll);
			notify(msg);

			if(rule->checkValidMove(player->getPosition(), roll, board) == false){
				msg = "Not a valid move, please retry";
				notify(msg);
				continue;
			}
			
			int pos = rule->findFinalPosition(player->getPosition(), roll, board);
		

			if(rule->checkWin(player->getPosition(), roll, board) == true) {
				msg = player->getName() + " your win, Congratulation !!!";
				notify(msg);
			    player->updatePosition(pos);
				players.pop_front();
				continue;
			}

			if(pos != (player->getPosition() + roll)) {
				int initial = player->getPosition() + roll;

				if(board->checkEntity(initial) == true) {
					auto entity = board->getEntity(initial);

					msg = "You encountered a " + entity->getType() + " at : " + to_string(entity->getStart()) + 
						  " which takes you to " + to_string(entity->getEnd());
					notify(msg);
				}
			}

			msg = player->getName() + " your new position is " + to_string(pos);
			notify(msg);
			
            player->updatePosition(pos);
			players.pop_front();
			players.push_back(player);
		}
	}
};

// -------------------------------------------
//  CLIENT
// -------------------------------------------

int main() {

	shared_ptr<GameFacade> game = make_shared<GameFacade>(100);

	shared_ptr<Rule> rule = RuleFactory::createRule(RuleType::STANDARD);
	game->setRule(rule);

// 	shared_ptr<CustomSetup> setupStrategy = SetupStrategyFactory::createSetupStrategy(SetupType::CUSTOM);

    shared_ptr<CustomSetup> setupStrategy = make_shared<CustomSetup>();

	shared_ptr<BoardEntity> snake1 = make_shared<BoardEntity>("Snake", 97, 23);
	setupStrategy->addEntity(snake1);

	shared_ptr<BoardEntity> snake2 = make_shared<BoardEntity>("Snake", 54, 38);
	setupStrategy->addEntity(snake2);

	shared_ptr<BoardEntity> snake3 = make_shared<BoardEntity>("Snake", 62, 12);
	setupStrategy->addEntity(snake3);

	shared_ptr<BoardEntity> snake4 = make_shared<BoardEntity>("Snake", 78, 45);
	setupStrategy->addEntity(snake4);



	shared_ptr<BoardEntity> ladder1 = make_shared<BoardEntity>("Ladder", 16, 55);
	setupStrategy->addEntity(ladder1);

	shared_ptr<BoardEntity> ladder2 = make_shared<BoardEntity>("Ladder", 22, 65);
	setupStrategy->addEntity(ladder2);

	shared_ptr<BoardEntity> ladder3 = make_shared<BoardEntity>("Ladder", 77, 96);
	setupStrategy->addEntity(ladder3);

	shared_ptr<BoardEntity> ladder4 = make_shared<BoardEntity>("Ladder", 29, 90);
	setupStrategy->addEntity(ladder4);

	shared_ptr<BoardEntity> ladder5 = make_shared<BoardEntity>("Ladder", 3, 27);
	setupStrategy->addEntity(ladder5);

	game->setupBoard(setupStrategy);


	//Players
	shared_ptr<Player> player1 = make_shared<Player>("Abhisek");
	game->addPlayer(player1);

	shared_ptr<Player> player2 = make_shared<Player>("Gaurav");
	game->addPlayer(player2);


	//play the game
	game->play();

	return 0;
}
