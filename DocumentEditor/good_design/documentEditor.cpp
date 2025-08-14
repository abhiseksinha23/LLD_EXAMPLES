#include "../../../builtin_files/bits-stdc++.h"
using namespace std;

class DocumentElement {
public:
    virtual string render() = 0;
};

class Text: public DocumentElement{
private:
    string text;
public:
    Text(string text){
        this->text = text;
    }
    string render(){
        return text + "\n";
    }
};

class Image: public DocumentElement{
private:
    string imagePath;
public:
    Image(string imagePath){
        this->imagePath = imagePath;
    }
    string render(){
        return "<img src=\"" + imagePath + "\" />\n";
    }
};

class Document{
private:
    vector<DocumentElement *> elements;
public:
    void addElement(DocumentElement *element) {
        elements.push_back(element);
    }

    string render() {
        string result = "";
        for(auto & element : elements) {
            result += element->render();
        }
        return result;
    }
};

class Persistence{
public:
    virtual void save(string &data) = 0;
};

class SaveToFile: public Persistence {
public:
    void save(string &data) override{
        cout << "Saving document to file...\n";
        ofstream file("document.txt");
        if(file.is_open()) {
            file << data;
            file.close();
            cout << "Document saved successfully.\n";
        } else {
            cout << "Error saving document.\n";
        }
    }
};

class SaveToDatabase: public Persistence {
public:
    void save(string &data) override {
        cout << "Saving document to database...\n";
        // Simulate database save
        cout << "Document saved successfully to database.\n";
    }
};

class DocumentEditor {
private:
    Document *document;
    Persistence *persistence;

public:
    DocumentEditor(Document *doc, Persistence *persist) {
        this->document = doc;
        this->persistence = persist;
    }

    void addText(string text) {
        document->addElement(new Text(text));
    }
    void addImage(string imagePath) {
        document->addElement(new Image(imagePath));
    }
    string render() {
        return document->render();
    }
    void save() {
        string data = document->render();
        persistence->save(data);
    }
};

int main() {
    Document *doc = new Document();
    Persistence *filePersistence = new SaveToFile();
    DocumentEditor editor(doc, filePersistence);

    editor.addText("Hello, this is a sample document.");
    editor.addImage("image1.jpg");
    editor.addText("This is another line of text.");
    editor.addImage("image2.png");

    cout << "Document Content:\n" << editor.render() << endl;

    editor.save();

    return 0;
}
