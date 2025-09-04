// COMPOSITE DESING PATTERN
// EXAMPLE --> FILE SYSTEM (TREE LIKE STRUCTURES)

#include "../../../builtin_files/bits-stdc++.h"
using namespace std;

//////////////////////////////////////////////////////////////////
// 					IFILESYSYEMITEM 
//////////////////////////////////////////////////////////////////

class IFileSystemItem {
public:
	virtual void ls(int indent = 0) = 0;
	virtual void openAll(int indent = 0) = 0;
	virtual string getName() = 0;
	virtual int getSize() = 0;
	virtual IFileSystemItem* cd(const string& target) = 0;
	virtual bool isFolder() = 0;
	virtual ~IFileSystemItem() {}
};



//////////////////////////////////////////////////////////////////
// 					FILE 
//////////////////////////////////////////////////////////////////

class File : public IFileSystemItem {
private:
	string name;
	int size;

public:
	File(string name, int size){
		this->name = name;
		this->size = size;
	}

	void ls(int indent = 0) override {
		cout << string(indent, ' ') << name <<endl;
	}

	void openAll(int indent = 0) override {
		cout << string(indent, ' ') << name <<endl;
	}

	string getName() override {
		return name;
	}

	int getSize() override {
		return size;
	}

	IFileSystemItem* cd(const string& target) override {
		return nullptr;
	}

	bool isFolder() override {
		return false;
	}
};


//////////////////////////////////////////////////////////////////
// 					FOLDER 
//////////////////////////////////////////////////////////////////

class Folder : public IFileSystemItem {
private:
	string name;
	vector<IFileSystemItem*>children;
public:
	Folder(string name){
		this->name = name;
	}

	void addChild(IFileSystemItem *item){
		children.push_back(item);
	}

	void ls(int indent = 0) override {
		for(auto child: children){
			if(child->isFolder()){
				cout << string(indent, ' ') << "+ " << child->getName() <<endl;
			}else{
				cout << string(indent, ' ') << child->getName() <<endl;
			}
		}
	}

	void openAll(int indent = 0) override {
		cout << string(indent, ' ') << "+ " << name <<endl;
		for(auto child: children){
			child->openAll(indent + 4);
		}
	}

	string getName() override {
		return name;
	}

	int getSize() override {
		int total = 0;
		for(auto child: children){
			total += child->getSize();
		}
		return total;
	}

	IFileSystemItem* cd(const string& target) override {
		for(auto child: children){
			if(child->isFolder() && child->getName() == target){
				return child;
			}
		}
		return nullptr;
	}

	bool isFolder() override {
		return true;
	}

	~Folder(){
		for(auto child: children){
			delete child;
		}
	}
};


//////////////////////////////////////////////////////////////////
// 					CLIENT 
//////////////////////////////////////////////////////////////////

int main(){

	Folder *root = new Folder("root");

	root->addChild(new File("file1.txt", 2));
	root->addChild(new File("file2.txt", 2));

	Folder *docs = new Folder("docs");

	docs->addChild(new File("resume.pdf", 3));
	docs->addChild(new File("notes.txt", 1));

	root->addChild(docs);

	Folder* images = new Folder("images");
	images->addChild(new File("photo.jpg", 10));

	root->addChild(images);


	// root->ls();

	// docs->ls();

	root->openAll();

	// IFileSystemItem *cwd = root->cd("docs");
	// if(cwd != nullptr){
	// 	cwd->openAll();
	// }else{
	// 	cout<<"No Such Directory found !!"<<endl;
	// }

	// cwd = root->cd("images");
	// if(cwd != nullptr){
	// 	cwd->openAll();
	// }else{
	// 	cout<<"No Such Directory found !!"<<endl;
	// }

	// cwd = root->cd("error");
	// if(cwd != nullptr){
	// 	cwd->openAll();
	// }else{
	// 	cout<<"No Such Directory found !!"<<endl;
	// }

    // cwd = root->cd("file1.txt");
	// if(cwd != nullptr){
	// 	cwd->openAll();
	// }else{
	// 	cout<<"No Such Directory found !!"<<endl;
	// }

	return 0;
}





