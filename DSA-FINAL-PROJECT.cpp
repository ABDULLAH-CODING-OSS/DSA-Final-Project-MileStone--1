#include <iostream>
#include <string>
#include <stack>
using namespace std;

class Date {
public:
    int day;
    int month;
    int year;

    Date(int d = 0, int m = 0, int y = 0) : day(d), month(m), year(y) {}

    void display() {
        cout << day << "/" << month << "/" << year << endl;
    }
};
class File {
public:
    string name;
    string extension;
    Date lastModificationDate;
    int size; 
    File* next;
    File* prev;

    File(string n, string ext, int s, Date d)
        : name(n), extension(ext), size(s), lastModificationDate(d), next(nullptr), prev(nullptr) {
    }
    void display() {
        cout << name << "." << extension << " (" << size << " bytes, " << size / 1000.0 << " KB), Last Modified: ";
        lastModificationDate.display();
        cout << endl;
    }
    void editFile(string content, Date d) {
        size = content.length();
        lastModificationDate = d; 
    }
};

class Directory {
public:
    string name;
    Date lastModified;
    Directory* parent;
    Directory* subDirectoriesHead;
    Directory* subDirectoriesTail;
    Directory* next;
    Directory* prev; 
    File* filesHead;
    File* filesTail;

    Directory(string n, Date d, Directory* p = nullptr)
        : name(n), lastModified(d), parent(p), subDirectoriesHead(nullptr), subDirectoriesTail(nullptr), next(nullptr), prev(nullptr), filesHead(nullptr), filesTail(nullptr) {
    }

    void addFile(File* file) {
        if (!filesHead) {
            filesHead = filesTail = file;
        }
        else {
            filesTail->next = file;
            file->prev = filesTail;
            filesTail = file;
        }
    }

    void addSubDirectory(Directory* dir) {
        if (!subDirectoriesHead) {
            subDirectoriesHead = subDirectoriesTail = dir;
        }
        else {
            subDirectoriesTail->next = dir;
            dir->prev = subDirectoriesTail;
            subDirectoriesTail = dir;
        }
    }
    void display() {
        cout << "Directory: " << name << " Last Modified: ";
        lastModified.display();
        cout << endl;
    }
    File* searchFile(string name) {
        File* current = filesHead;
        while (current) {
            if (current->name == name) return current;
            current = current->next;
        }
        return nullptr;
    }

    Directory* searchSubDirectory(string name) {
        Directory* current = subDirectoriesHead;
        while (current) {
            if (current->name == name) return current;
            current = current->next;
        }
        return nullptr;
    }

    void deleteFile(string name) {
        File* current = filesHead;
        while (current) {
            if (current->name == name) {
                if (current->prev) {
                    current->prev->next = current->next;
                }
                else {
                    filesHead = current->next;
                }
                if (current->next) {
                    current->next->prev = current->prev;
                }
                else {
                    filesTail = current->prev;
                }
                delete current;
                return;
            }
            current = current->next;
        }
    }

    void deleteSubDirectory(string name) {
        Directory* current = subDirectoriesHead;
        while (current) {
            if (current->name == name) {
                if (current->prev) {
                    current->prev->next = current->next;
                }
                else {
                    subDirectoriesHead = current->next;
                }
                if (current->next) {
                    current->next->prev = current->prev;
                }
                else {
                    subDirectoriesTail = current->prev;
                }
                delete current;
                return;
            }
            current = current->next;
        }
    }
    void displayContents() {
        cout << "Contents of Directory '" << name << "':\n";
        Directory* currentDir = subDirectoriesHead;
        while (currentDir) {
            cout << "[Subdirectory] ";
            currentDir->display();
            currentDir = currentDir->next;
        }

        File* currentFile = filesHead;
        while (currentFile) {
            cout << "[File] ";
            currentFile->display();
            currentFile = currentFile->next;
        }
    }
};
class RecentFiles {
public:
    stack<string> recentFiles;

    void addFile(string name) {
        recentFiles.push(name);
    }

    void showRecentFiles() {
        cout << "Recently opened files/directories:\n";
        stack<string> temp = recentFiles;
        while (!temp.empty()) {
            cout << temp.top() << endl;
            temp.pop();
        }
    }
};
void createDirectory(Directory* parent) {
    string name;
    int day, month, year;
    cout << "Enter directory name: ";
    cin >> name;
    cout << "Enter Date In Format day month year: ";
    cin >> day >> month >> year;
    Date date(day, month, year);

    // Check for duplicate directory names
    int counter = 1;
    string originalName = name;
    while (parent->searchSubDirectory(name)) {
        name = originalName + "(" + to_string(counter) + ")";
        counter++;
    }

    Directory* newDir = new Directory(name, date, parent);
    if (parent) {
        parent->addSubDirectory(newDir);
    }
    cout << "Directory '" << name << "' created successfully.\n";
}
void createFile(Directory* dir) {
    string name, ext;
    int day, month, year;
    cout << "Enter file name: ";
    cin >> name;
    cout << "Enter file extension: ";
    cin >> ext;
    cout << "Enter Date In Format day month year: ";
    cin >> day >> month >> year;
    Date date(day, month, year);

    File* newFile = new File(name, ext, 0, date);
    dir->addFile(newFile);
    cout << "File '" << name << "." << ext << "' created successfully.\n";
}
void editFile(File* file) {
    string content;
    int day, month, year;
    cout << "Enter new content: ";
    cin.ignore();
    getline(cin, content);
    cout << "Enter Date In Format day month year: ";
    cin >> day >> month >> year;
    Date date(day, month, year);

    file->editFile(content, date);
    cout << "File edited successfully.\n";
}
void openDirectory(Directory* dir, RecentFiles& recentFiles) {
    cout << "Opening directory:\n";
    dir->displayContents();
    recentFiles.addFile(dir->name);
}

void openFile(File* file, RecentFiles& recentFiles) {
    cout << "Opening file:\n";
    file->display();
    recentFiles.addFile(file->name);
}
void showAllDirectories(Directory* dir, int level = 0) {
    if (!dir) return;

    for (int i = 0; i < level; i++) cout << "  ";
    cout << "[Directory] ";
    dir->display();

    File* currentFile = dir->filesHead;
    while (currentFile) {
        for (int i = 0; i < level + 1; i++) cout << "  ";
        cout << "[File] ";
        currentFile->display();
        currentFile = currentFile->next;
    }
    Directory* currentDir = dir->subDirectoriesHead;
    while (currentDir) {
        showAllDirectories(currentDir, level + 1);
        currentDir = currentDir->next;
    }
}
void searchAll(Directory* dir, string name) {
    File* file = dir->searchFile(name);
    if (file) {
        cout << "Found file: ";
        file->display();
        return;
    }
    Directory* subDir = dir->searchSubDirectory(name);
    if (subDir) {
        cout << "Found directory: ";
        subDir->display();
        return;
    }
    Directory* currentDir = dir->subDirectoriesHead;
    while (currentDir) {
        searchAll(currentDir, name);
        currentDir = currentDir->next;
    }
    cout << "Not found.\n";
}
void menu() {
    Directory* root = new Directory("root", Date());
    RecentFiles recentFiles;
    string choice;
    do {
        cout << "\n";
        cout << "**************************************\n";
        cout << "*    DIRECTORY MANAGEMENT SYSTEM    *\n";
        cout << "**************************************\n";
        cout << "* To Create Directory   ->   Type 'cr'  *\n";
        cout << "* To Create Subdirectory->   Type 'crS' *\n";
        cout << "* To Create File        ->   Type 'crF' *\n";
        cout << "* To Edit File          ->   Type 'edit' *\n";
        cout << "* To Search Directory   ->   Type 'cd'  *\n";
        cout << "* To Open Directory     ->   Type 'open' *\n";
        cout << "* To Open File          ->   Type 'openF' *\n";
        cout << "* To Delete Directory   ->   Type 'free' *\n";
        cout << "* To Delete File        ->   Type 'delF' *\n";
        cout << "* To Show All Contents  ->   Type 'dir'  *\n";
        cout << "* To Show Recent Files  ->   Type 'recent'*\n";
        cout << "* To Exit Program       ->   Type 'exit' *\n";
        cout << "****************************************\n";
        cout << "Enter your choice: ";
        
        cin >> choice;
        if (choice == "cr") {
            createDirectory(root);
        }
        else if (choice == "crS") {
            string dirName;
            cout << "Enter parent directory name: ";
            cin >> dirName;
            Directory* parent = root->searchSubDirectory(dirName);
            if (parent) {
                createDirectory(parent);
            }
            else {
                cout << "Parent directory not found.\n";
            }
        }
        else if (choice == "crF") {
            string dirName;
            cout << "Enter directory name to add file: ";
            cin >> dirName;
            Directory* dir = root->searchSubDirectory(dirName);
            if (dir) {
                createFile(dir);
            }
            else {
                cout << "Directory not found.\n";
            }
        }
        else if (choice == "edit") {
            string dirName, fileName;
            cout << "Enter directory name: ";
            cin >> dirName;
            Directory* dir = root->searchSubDirectory(dirName);
            if (dir) {
                cout << "Enter file name: ";
                cin >> fileName;
                File* file = dir->searchFile(fileName);
                if (file) {
                    editFile(file);
                }
                else {
                    cout << "File not found.\n";
                }
            }
            else {
                cout << "Directory not found.\n";
            }
        }
        else if (choice == "cd") {
            string name;
            cout << "Enter name to search: ";
            cin >> name;
            searchAll(root, name);
        }
        else if (choice == "open") {
            string dirName;
            cout << "Enter directory name to open: ";
            cin >> dirName;
            Directory* dir = root->searchSubDirectory(dirName);
            if (dir) {
                openDirectory(dir, recentFiles);
            }
            else {
                cout << "Directory not found.\n";
            }
        }
        else if (choice == "openF") {
            string dirName, fileName;
            cout << "Enter directory name: ";
            cin >> dirName;
            Directory* dir = root->searchSubDirectory(dirName);
            if (dir) {
                cout << "Enter file name: ";
                cin >> fileName;
                File* file = dir->searchFile(fileName);
                if (file) {
                    openFile(file, recentFiles);
                }
                else {
                    cout << "File not found.\n";
                }
            }
            else {
                cout << "Directory not found.\n";
            }
        }
        else if (choice == "free") {
            string dirName;
            cout << "Enter directory name to delete: ";
            cin >> dirName;
            root->deleteSubDirectory(dirName);
            cout << "Directory deleted successfully.\n";
        }
        else if (choice == "delF") {
            string dirName, fileName;
            cout << "Enter directory name: ";
            cin >> dirName;
            Directory* dir = root->searchSubDirectory(dirName);
            if (dir) {
                cout << "Enter file name: ";
                cin >> fileName;
                dir->deleteFile(fileName);
                cout << "File deleted successfully.\n";
            }
            else {
                cout << "Directory not found.\n";
            }
        }
        else if (choice == "dir") {
            showAllDirectories(root);
        }
        else if (choice == "recent") {
            recentFiles.showRecentFiles();
        }
        else if (choice == "exit") {
            cout << "Exiting the program.\n";
            break;
        }
        else {
            cout << "Invalid command. Try again.\n";
        }
    } while (true);
}

int main() {
    menu();
    return 0;
}