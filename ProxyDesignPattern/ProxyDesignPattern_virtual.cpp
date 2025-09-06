// PROXY DESIGN PATTERN --> VIRTUAL PROXY
// EXAMPLE --> IMAGE DISPLAY

#include<bits/stdc++.h>
using namespace std;

//////////////////////////////////////////////////////////////////
// 					IDISPLAY 
//////////////////////////////////////////////////////////////////

class IDisplay {
public:
	virtual void display() = 0;
	virtual ~IDisplay() {}
};


//////////////////////////////////////////////////////////////////
// 					IMAGEDISPLAY 
//////////////////////////////////////////////////////////////////

class ImageDisplay : public IDisplay {
private:
	string path;
public:
	ImageDisplay(const string& path){
		this->path = path;

		// HEAVY OPERATIONS
		cout <<"[IMAGEDISPLAY] Doing Some Heavy operations ... "<<endl;
		cout <<"[IMAGEDISPLAY] Loading the image from "<< path <<endl;
	}

	void display() override {
		cout<<"[IMAGEDISPLAY] Displaying the image : "<< path <<endl;
	}
};

//////////////////////////////////////////////////////////////////
// 					IMAGEPROXY 
//////////////////////////////////////////////////////////////////

class ImageProxy : public IDisplay {
private:
	string path;
	shared_ptr<ImageDisplay> iDis;
public:

	ImageProxy(const string& path){
		this->path = path;
		this->iDis = nullptr;
	}

	void display() override {
		if(iDis == nullptr){
			iDis = make_shared<ImageDisplay>(path);
		}

		iDis->display();
	}
};


//////////////////////////////////////////////////////////////////
// 					CLIENT 
//////////////////////////////////////////////////////////////////

int main() {
	unique_ptr<IDisplay> image = make_unique<ImageProxy>("/cd/photos/image.jpg");
	image->display();

	return 0;
}
