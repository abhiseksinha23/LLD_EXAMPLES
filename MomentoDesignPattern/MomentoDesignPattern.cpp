// MOMENTO DESIGN PATTERN
// EXAMPLE --> DB TRANSACTION MANAGEMENT

#include<bits/stdc++.h>
using namespace std;

class DatabaseMomento {
private:
	map<string, string>data;

public:
	DatabaseMomento(map<string, string> &dbData) {
		this->data = dbData;
	}

	map<string, string> getState() {
		return data;
	}
};


// ORIGINATOR - THE DATABASE WHOSE STATE WE WANT TO SAVE/RESTORE
class Database {
private:
	map<string, string> records;

public:

	// Insert a record
	void insert(const string &key, const string &value) {
		records[key] = value;
		cout << "Inserted: " << key << " = " << value << endl;
	}

	// Update a record
	void update(const string &key, const string &value) {
		if(records.find(key) != records.end()) {
			records[key] = value;
			cout << "Updated: " << key << " = " << value << endl;
		} else {
			cout << "Key not found for updation: " << key << endl;
		}
	}

	// Delete a record
	void remove(const string &key, const string &value) {
		if(records.find(key) != records.end()) {
			records.erase(key);
			cout << "Deleted: " << key  << endl;
		} else {
			cout << "Key not found for deletion: " << key << endl;
		}
	}


	// Create Momento - Save Current state
	shared_ptr<DatabaseMomento> createMomento() {
		cout << "Creating Database backup..." << endl;
		return make_shared<DatabaseMomento>(records);
	}

	// Restore from Momento - Rollback the saved state
	void restoreFromMemento(shared_ptr<DatabaseMomento> momento) {
		records = momento->getState();
		cout << "Database restored from backup!" << endl;
	}

	// Display current database state
    void displayRecords() {
        cout << "\n--- Current Database State ---" << endl;
        if (records.empty()) {
            cout << "Database is empty" << endl;
        } else {
            for (const auto& record : records) {
                cout << record.first << " = " << record.second << endl;
            }
        }
        cout << "-----------------------------\n" << endl;
    }
};

// Caretaker - Manages the memento (transaction manager)
class TransactionManager {
private:
	shared_ptr<DatabaseMomento> backup;

public:

	TransactionManager() : backup(nullptr) {}

	// Begin transaction - create backup	
	void beginTransaction(Database &db) {
		cout << "=== BEGIN TRANSACTION ===" << endl;
		backup = db.createMomento();
	}


	// Commit transaction - discard backup
	void commitTransaction() {
		cout << "=== COMMIT TRANSACTION ===" << endl;

        backup = nullptr;

        cout << "Transaction committed successfully!" << endl;
	}


	// Rollback transaction - restore from backup
    void rollbackTransaction(Database& db) {
        cout << "=== ROLLBACK TRANSACTION ===" << endl;
        if (backup) {
            db.restoreFromMemento(backup);
            backup = nullptr;
            cout << "Transaction rolled back!" << endl;
        } 
        else {
            cout << "No backup available for rollback!" << endl;
        }
    }

};


int main() {
	Database db;
    TransactionManager txManager;
   
    //success scenario
    txManager.beginTransaction(db);
    db.insert("user1", "Abhisek");
    db.insert("user2", "Gaurav");
    txManager.commitTransaction();

    db.displayRecords();

    // Failed scenario
    txManager.beginTransaction(db);
    db.insert("user3", "Saurav");
    db.insert("user4", "Manish");

    db.displayRecords();
    
    // Some error -> Rollback
    cout << "ERROR: Something went wrong during transaction!" << endl;
    txManager.rollbackTransaction(db);
    
    db.displayRecords();
    
    return 0;
}
