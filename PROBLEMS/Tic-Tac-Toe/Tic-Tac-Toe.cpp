// TIC-TAC-TOE EXAMPLE

/*

FUNCTION REQUIREMENTS

- SIZE OF THE BOARD SHOULD BE SCALABLE.
- THERE ARE STANDARD GAME RULES AND SHOULD BE FURTHER EXTENDIBLE.
- ALLOW ON APP NOTIFICATION FOR TIC-TAC-TOE MOVES, WINS, AND DRAWS.

NON FUNCTION REQUIREMENTS
- SCALABLE
- SOLID PRINCIPLE
- LOOSLY COUPLED
*/

#include<bits/stdc++.h>
using namespace std;
// -------------------------------------------
// PLAYER
// -------------------------------------------

class Player {
private:
	string name;
	char symbol;
	int winCount;

public:

	Player(string name, char symbol) {
		this->name = name;
		this->symbol = symbol;
		winCount = 0;
	}

	void increaseWinCount() {
		winCount++;
	}

	string getName() {
		return name;
	}

	char getSymbol() {
		return symbol;
	}

	int getWinCount() {
		return winCount;
	}
};


// -------------------------------------------
// BOARD
// -------------------------------------------

class Board {
private:
	vector<vector<char>> grid;
	int size;
	char emptySymbol;

public:

	Board(int size, char symbol) {
		this->size = size;
		emptySymbol = symbol;
		grid.resize(size);
		for(int i=0; i<size; i++){
			grid[i].resize(size, emptySymbol);
		}
	}

	int getSize() {
		return size;
	}

	void display() {
		cout <<" ============== GRID ================" << endl;

		cout <<"  ";
		for(int i=0; i<size; i++) {
			cout<< i <<" ";
		}cout<< endl;

		for(int i=0; i<size; i++){
			cout << i << " ";
			for(int j=0; j<size; j++) {
				cout << grid[i][j] << " ";
			}cout << endl;
		}
	}

	bool isValidCell(int r, int c) {
		if(r < 0 || r >= size || c < 0 || c >= size) return false;
		
		return true;
	}

	bool isCellEmpty(int r, int c) {
		if(isValidCell(r,c) == false)return false;
		
		if(grid[r][c] == emptySymbol)return true;

		return false;	
	}

	bool updateCell(int r, int c, char symbol){
		if(isValidCell(r,c) == false)return false;

		grid[r][c] = symbol;
		return true;
	}

	char getCell(int r, int c) {
		if(isValidCell(r,c) == false)return emptySymbol;
		
		return grid[r][c];
	}

};

// -------------------------------------------
// NOTIFICATION SYSTEM
// -------------------------------------------

class IObserver {
public:
	virtual void update(const string& msg) = 0;
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

	void registerObserver(shared_ptr<IObserver> obs){
		observers.insert(obs);
	}

	void removeObserver(shared_ptr<IObserver> obs){
		if(observers.find(obs) == observers.end()) return;
		
		observers.erase(obs);
	}

	void notify(const string& msg){
		for(auto obs: observers) {
			obs->update(msg);
		}
	}
};

NotificationService* NotificationService::instance = nullptr;
mutex NotificationService::mtx;

class InAppNotification : public IObserver {
public:
	void update(const string& msg) override {
		cout << "[Notification in APP] : " << msg << endl;
	}
};


// -------------------------------------------
// RULES
// -------------------------------------------

class Rule {
public:
	virtual bool checkWin(int row, int col, char symbol, shared_ptr<Board> board) = 0;
	virtual bool checkDraw(shared_ptr<Board> board) = 0;
	virtual bool isValidMove(int row, int col, char symbol, shared_ptr<Board> board) = 0;
};


class StandardRule : public Rule {
private:
	bool checkRow(int row, char symbol, shared_ptr<Board> board) {
		int n = board->getSize();

		for(int i=0; i<n; i++){
			if(board->getCell(row, i) != symbol) return false;
		}

		return true;
	}

	bool checkCol(int col, char symbol, shared_ptr<Board> board) {
		int n = board->getSize();

		for(int i=0; i<n; i++){
			if(board->getCell(i, col) != symbol) return false;
		}

		return true;

	}

	bool checkLeftDiagonals(int row, int col, char symbol, shared_ptr<Board>board) {
		int n = board->getSize();

		int i = 0, j = 0;

		while(i <n && j <n ) {
			if(board->getCell(i,j) != symbol) return false;
			i++;
			j++;
		}

		return true;
	}

	bool checkRightDiagonals(int row, int col, char symbol, shared_ptr<Board>board) {
		int n = board->getSize();

		int i = 0, j = n-1;

		while(i < n && j >=0 ) {
			if(board->getCell(i,j) != symbol) return false;
			i++;
			j--;
		}

		return true;
	}


public:
	bool checkWin(int row, int col, char symbol, shared_ptr<Board> board) override {
		if(checkRow(row, symbol, board) == true) return true;
		if(checkCol(col, symbol, board) == true) return true;
		if(checkLeftDiagonals(row, col, symbol, board) == true) return true;
		if(checkRightDiagonals(row, col, symbol, board) == true) return true;

		return false;
	}

	bool checkDraw(shared_ptr<Board> board) override {
		int n = board->getSize();

		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				if(board->isCellEmpty(i,j) == true)return false;
			}
		}

		return true;
	}

	bool isValidMove(int row, int col, char symbol, shared_ptr<Board> board) override {
		return board->isCellEmpty(row, col);
	}
};

// -------------------------------------------
// RULES FACTORY
// -------------------------------------------

enum class RuleType {
	STANDARD
};

class RuleFactory {
private:
	static RuleFactory *instance;
	static mutex mtx;
	RuleFactory() {}

public:

	static RuleFactory* getInstance() {
		if(instance == nullptr) {
			lock_guard<mutex> lock(mtx);

			if(instance == nullptr) {
				instance = new RuleFactory();
			}
		}

		return instance;
	}

	shared_ptr<Rule> getRule(RuleType type) {
		if(type == RuleType::STANDARD){
			return make_shared<StandardRule>();
		}

		return nullptr;
	}
};

RuleFactory* RuleFactory::instance = nullptr;
mutex RuleFactory::mtx;


// -------------------------------------------
// GAMEFACADE
// -------------------------------------------

class GameFacade {
private:
	shared_ptr<Board> board;
	shared_ptr<Rule> rule;
	deque<Player> players;
	bool isGameOver;

public:
	GameFacade(int size, char symbol) {
		this->board = make_shared<Board>(size, symbol);
		isGameOver = false;

		shared_ptr<IObserver> inApp = make_shared<InAppNotification>();
		NotificationService::getInstance()->registerObserver(inApp);
	}


	void setRule(shared_ptr<Rule>rule) {
		this->rule = rule;
	}

	void addPlayer(Player p){
		players.push_back(p);
	}

	void removePlayer(Player p) {
		// Implementation
	}

	bool gameOver() {
		return isGameOver;
	}

	void play() {

		while(isGameOver == false) {
			auto player = players.front();
			int row, col;

		    string askInput = player.getName() + ", it's your move, please enter the coordinates";
		    NotificationService::getInstance()->notify(askInput);
		    
			cin >> row >> col;

			string message = "Entered coordinates by " + player.getName() + " : " + to_string(row) + " " + to_string(col);
			NotificationService::getInstance()->notify(message);

			if(rule->isValidMove(row, col, player.getSymbol(), board) == false) {
				cout << "Not a valid move, please retry !" << endl;
				continue;
			}

			board->updateCell(row, col, player.getSymbol());
			board->display();

			if(rule->checkWin(row, col, player.getSymbol(), board) == true) {
				string msg = "Player " + player.getName() + " won the Game, Congrats !!";
				NotificationService::getInstance()->notify(msg);
				isGameOver = true;
				continue;
			}

			if(rule->checkDraw(board) == true) {
				string msg = "Game DRAW !!";
				NotificationService::getInstance()->notify(msg);
				isGameOver = true;
				continue;
			}

			players.pop_front();
			players.push_back(player);
		}
	}

	void display() {
		board->display();
	}
};


int main() {

	char symbol = '-';
	GameFacade game(3, symbol);

	shared_ptr<Rule> rule = RuleFactory::getInstance()->getRule(RuleType::STANDARD);

	game.setRule(rule);	

	char player1 = 'X';
	Player abhisek("Abhisek", player1); 

	char player2 = 'O';
	Player gaurav("Gaurav", player2); 

	game.addPlayer(abhisek);
	game.addPlayer(gaurav);

	game.display();

	game.play();

	return 0;
}
