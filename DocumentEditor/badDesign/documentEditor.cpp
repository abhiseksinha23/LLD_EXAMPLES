#include <bits/stdc++.h>
// #include "../../../builtin_files/bits-stdc++.h"
using namespace std;

class DocumentEditor {
private:
    vector<string>elements;
public:
    void addText(string text) {
        elements.push_back(text);
    }

    void addImage(string image) {
        elements.push_back(image);
    }

    string render(){
        string result = "";
        for(auto & element : elements) {
            if(element.length() > 4 && (element.substr(element.length() - 4) == ".jpg" || 
                element.substr(element.length() - 4) == ".png")) {
                result += "<img src=\"" + element + "\" />\n";
            } else{
                result += element + "\n";
            }
        }
        return result;
    }

    void saveToFile(){
        // Simulate saving to database
        cout << "Saving document to database...\n";
        ofstream file("document.txt");
        if(file.is_open()) {
            file << render();
            file.close();
            cout << "Document saved successfully.\n";
        } else {
            cout << "Error saving document.\n";
        }
    }
};

int main(){
    DocumentEditor editor;
    editor.addText("Hello, this is a sample document.");
    editor.addImage("image1.jpg");
    editor.addText("This is another line of text.");
    editor.addImage("image2.png");
    
    cout << "Document Content:\n" << editor.render() << endl;

    editor.saveToFile();

    return 0;
}
