#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    int choice;
    string filename, line, data;
    
    cout << "File Operations Program\n";
    cout << "-----------------------\n";
    
    do {
        cout << "\n1. Write to file\n";
        cout << "2. Read from file\n";
        cout << "3. Append to file\n";
        cout << "4. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;
        
        if(choice == 1) {
            cout << "Enter filename: ";
            cin >> filename;
            
            ofstream file;
            file.open(filename);
            
            if(file.is_open()) {
                cin.ignore();
                cout << "Enter text (type 'done' to stop):\n";
                while(true) {
                    getline(cin, data);
                    if(data == "done") {
                        break;
                    }
                    file << data << "\n";
                }
                file.close();
                cout << "File saved!\n";
            } else {
                cout << "Couldn't open file\n";
            }
        }
        
        else if(choice == 2) {
            cout << "Enter filename: ";
            cin >> filename;
            
            ifstream file(filename);
            
            if(file.is_open()) {
                cout << "\nFile contents:\n";
                cout << "--------------\n";
                while(getline(file, line)) {
                    cout << line << endl;
                }
                file.close();
            } else {
                cout << "File not found\n";
            }
        }
        
        else if(choice == 3) {
            cout << "Enter filename: ";
            cin >> filename;
            
            ofstream file;
            file.open(filename, ios::app);
            
            if(file.is_open()) {
                cin.ignore();
                cout << "Enter text to append (type 'done' to stop):\n";
                while(true) {
                    getline(cin, data);
                    if(data == "done") {
                        break;
                    }
                    file << data << "\n";
                }
                file.close();
                cout << "Text appended!\n";
            } else {
                cout << "Couldn't open file\n";
            }
        }
        
        else if(choice == 4) {
            cout << "Goodbye!\n";
        }
        
        else {
            cout << "Invalid option\n";
        }
        
    } while(choice != 4);
    
    return 0;
}