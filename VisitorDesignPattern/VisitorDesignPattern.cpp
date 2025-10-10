// VISITOR DESIGN PATTERN
// EXAMPLE --> DOCUMENT ELEMENT

#include <bits/stdc++.h>
using namespace std;

// FORWARD DECLARATIONS OF ALL FILE TYPES
class TextFile;
class ImageFile;
class VideoFile;


// VISITOR INTERFACE
class FileSystemVisitor {
public:
	virtual void visit(TextFile *file) = 0;
	virtual void visit(ImageFile *file) = 0;
	virtual void visit(VideoFile *file) = 0;
};

class FileSystemItem {
protected:
	string name;

public:
	FileSystemItem(const string &name) {
		this->name = name;
	}

	virtual string getName() {
		return name;
	}

	virtual void accept(FileSystemVisitor *visitor) = 0;
};

class TextFile : public FileSystemItem {
private:
	string content;
public:
	TextFile(const string &name, const string &content) : FileSystemItem(name) {
		this->content = content;
	}

	string getContent(){
		return content;
	}

	void accept(FileSystemVisitor *visitor) override {
		visitor->visit(this);
	}
};

class ImageFile : public FileSystemItem {
public:
	ImageFile(const string &name) : FileSystemItem(name) {}

	void accept(FileSystemVisitor *visitor) override {
		visitor->visit(this);
	}
};

class VideoFile : public FileSystemItem {
public:
	VideoFile(const string &name) : FileSystemItem(name) {}

	void accept(FileSystemVisitor *visitor) override {
		visitor->visit(this);
	}
};


// SIZE CALCULATION VISITOR
class SizeCalculationVisitor : public FileSystemVisitor {
public:
	void visit(TextFile *file) override {
		cout <<"Calculating size of the Text file " << file->getName() << endl;
	}

	void visit(ImageFile *file) override {
		cout <<"Calculating size of the Image file " << file->getName() << endl;
	}

	void visit(VideoFile *file) override {
		cout <<"Calculating size of the Video file " << file->getName() << endl;
	}
};


// COMPRESSION VISITOR
class CompressionVisitor : public FileSystemVisitor {
public:
	void visit(TextFile *file) override {
		cout <<"Compressing Text file " << file->getName() << endl;
	}

	void visit(ImageFile *file) override {
		cout <<"Compressing Image file " << file->getName() << endl;
	}

	void visit(VideoFile *file) override {
		cout <<"Compressing Video file " << file->getName() << endl;
	}
};



// VIRUS SCANNING VISITOR
class VirusScanningVisitor : public FileSystemVisitor {
public:
	void visit(TextFile *file) override {
		cout <<"Scanning Text file " << file->getName() << endl;
	}

	void visit(ImageFile *file) override {
		cout <<"Scanning Image file " << file->getName() << endl;
	}

	void visit(VideoFile *file) override {
		cout <<"Scanning Video file " << file->getName() << endl;
	}
};

int main() {

	FileSystemItem *img1 = new ImageFile("sample.jpg");

	img1->accept(new SizeCalculationVisitor());
	img1->accept(new CompressionVisitor());
	img1->accept(new VirusScanningVisitor());


	FileSystemItem *vid1 = new VideoFile("test.mp4");

	vid1->accept(new CompressionVisitor());

	return 0;

}
