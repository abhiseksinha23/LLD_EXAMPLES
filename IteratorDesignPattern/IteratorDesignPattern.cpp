// ITERATOR DESIGN PATTERN
// EXAMPLE --> LINKED LIST, BINARY TREE & PLAYLIST

#include <bits/stdc++.h>
using namespace std;

template<typename T>
class Iterator {
public:
	virtual bool hasNext() = 0;
	virtual bool hasPrev() = 0;
	virtual T next() = 0;
	virtual T prev() = 0;
};

template<typename T>
class Iterable {
public:
	virtual Iterator<T>* getIterator() = 0;
};

class LinkedList : public Iterable<int> {
public:
	int data;
	LinkedList* next;

	LinkedList(int value) {
		data = value;
		next = nullptr;
	} 

	Iterator<int>* getIterator() override;	
};

class BinaryTree : public Iterable<int> {
public:
	int data;
	BinaryTree *left, *right;

	BinaryTree(int value) {
		data = value;
		right = nullptr;
		left = nullptr;
	} 

	Iterator<int>* getIterator() override;	
};


class Song {
public:
	string title;
	string artist;

	Song(const string& t, const string& a) {
        title  = t;
        artist = a;
    }
};

class Playlist : public Iterable<Song> {
public:
    vector<Song> songs;

    void addSong(const Song& s) {
        songs.push_back(s);
    }

    Iterator<Song>* getIterator() override;
};


class LinkedListIterator : public Iterator<int> {
private:
	LinkedList* current;

public:
    LinkedListIterator(LinkedList* head) {
        current = head;
    }

    bool hasNext() override {
        return current != nullptr;
    }

    int next() override {
        int val = current->data; // 1-> 2-> 3  
        current = current->next;
        return val;
    }

    bool hasPrev() override {
    	// prev
        return current != nullptr;
    }
    int prev() override {
    	// prev logic
        int val = current->data; // 1-> 2-> 3  
        current = current->next;
        return val;
    }
};

class BinaryTreeInorderIterator : public Iterator<int> {
private:
    stack<BinaryTree*> stk;
    BinaryTree* curr;

    void pushLefts(BinaryTree* node) {
        while (node) {
            stk.push(node);
            node = node->left;
        }
    }
public:
    BinaryTreeInorderIterator(BinaryTree* root) {
        curr = root;
        pushLefts(curr);
    }

    bool hasNext() override {
        return !stk.empty();
    }

    int next() override {
        BinaryTree* node = stk.top();
        stk.pop();
        int val = node->data;
        if (node->right) {
            pushLefts(node->right);
        }
        return val;
    }

    bool hasPrev() override {
    	// prev
        return true;
    }

    int prev() override {
    	// prev logic
        int val = 0;
        return val;
    }
};


class PlaylistIterator : public Iterator<Song> {
private:
    vector<Song> vec;
    size_t index;
public:
    PlaylistIterator(vector<Song> v) {
        vec = v;
        index = 0;
    }

    bool hasNext() override {
        return index < vec.size();
    }

    Song next() override {
        return vec[index++];
    }

    bool hasPrev() override {
    	// prev
        return true;
    }

    Song prev() override {
    	// prev logic
        return vec[0];
    }
};


// Attaching Iterators
Iterator<int>* LinkedList::getIterator() {
    return new LinkedListIterator(this);
}

Iterator<int>* BinaryTree::getIterator() {
    return new BinaryTreeInorderIterator(this);
}

Iterator<Song>* Playlist::getIterator() {
    return new PlaylistIterator(songs);
}


int main() {

    //------------------------------------------------
    // LinkedList: 1 → 2 → 3
    LinkedList* list = new LinkedList(1);
    list->next = new LinkedList(2);
    list->next->next = new LinkedList(3);

    Iterator<int>* iterator1 = list->getIterator();

    cout << "LinkedList contents: ";

    while (iterator1->hasNext()) {
        cout << iterator1->next() << " ";
    }

    cout << "\n";

    //------------------------------------------------

    // BinaryTree:
    //    2
    //   / \
    //  1   3
    BinaryTree* root = new BinaryTree(2);
    root->left  = new BinaryTree(1);
    root->right = new BinaryTree(3);

    Iterator<int>* iterator2 = root->getIterator();

    cout << "BinaryTree inorder: ";

    while (iterator2->hasNext()) {
        cout << iterator2->next() << " ";
    }

    cout << "\n";

    //------------------------------------------------

    // Playlist
    Playlist playlist;
    playlist.addSong(Song("Admirin You", "Karan Aujla"));
    playlist.addSong(Song("Husn", "Anuv Jain"));

    Iterator<Song>* iterator3 = playlist.getIterator();

    cout << "Playlist songs:\n";

    while (iterator3->hasNext()) {
        Song s = iterator3->next();
        cout << "  " << s.title << " by " << s.artist << "\n";
    }

    //------------------------------------------------

    // Cleanup
    delete list->next->next;
    delete list->next;
    delete list;
    delete root->left;
    delete root->right;
    delete root;

    return 0;
}
