#include<iostream>
#include<string>
using namespace std;

class Date {
public:
    int day;
    int month;
    int year;

    Date(int d, int m, int y) {
        day = d;
        month = m;
        year = y;
    }
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

    File(string n, string ext, int s, Date d)
        : name(n), extension(ext), size(s), lastModificationDate(d) {
    }
    void display() {
        cout << name << "." << extension << " (" << size << "KB), Last Modified: ";
        lastModificationDate.display();
        cout << endl;
    }
};

class Directory {
public:
    string name;
    int size;
    Date lastModified;
public:
    Directory(string n, Date d) : name(n), lastModified(d) {}
    int addFile(File& f) {
        return size += f.size;
    }
    void display() {
        cout << "Directory: " << name << " Last Modified: ";
        lastModified.display();
        cout << endl;
    }
    bool searchByName(string n) {
        return name == n;
    }
};

class Node {
public:
    Directory dir;
    Node* left;
    Node* right;
    Node(Directory d) : dir(d), left(nullptr), right(nullptr) {}
};

class BST {
public:
    Node* root;
    BST() {
        root = nullptr;
    }
    Node* insert(Node* node, Directory dir) {
        if (!node) return new Node(dir);

        if (dir.name < node->dir.name) {
            node->left = insert(node->left, dir);
        }
        else if (dir.name > node->dir.name) {
            node->right = insert(node->right, dir);
        }

        return node;
    }
    void insert(Directory dir) {
        root = insert(root, dir);
    }
    Node* search(Node* node, string name) {
        if (!node || node->dir.searchByName(name)) {
            return node;
        }

        if (name < node->dir.name) {
            return search(node->left, name);
        }

        return search(node->right, name);
    }
    Node* searchByName(string name) {
        return search(root, name);
    }
    void inorder(Node* node) {
        if (!node) return;
        inorder(node->left);
        node->dir.display();
        inorder(node->right);
    }
    void showAllDirectories() {
        inorder(root);
    }
    Node* deleteNode(Node* root, string name) {
        if (!root) return root;

        if (name < root->dir.name) {
            root->left = deleteNode(root->left, name);
        }
        else if (name > root->dir.name) {
            root->right = deleteNode(root->right, name);
        }
        else {
            if (!root->left) {
                Node* temp = root->right;
                delete root;
                return temp;
            }
            else if (!root->right) {
                Node* temp = root->left;
                delete root;
                return temp;
            }

            Node* temp = minValueNode(root->right);
            root->dir = temp->dir;
            root->right = deleteNode(root->right, temp->dir.name);
        }
        return root;
    }
    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left) {
            current = current->left;
        }
        return current;
    }
    void deleteNodeByName(string name) {
        root = deleteNode(root, name);
    }
};

void createDirectory(BST& bst) {
    string name;
    int day, month, year;
    cout << "Enter directory name: ";
    cin >> name;
    cout << "Enter Date In Format day/month/year: ";
    cin >> day;
    cin >> month;
    cin >> year;
    Date date(day, month, year);

    string originalName = name;
    int counter = 0;

    while (bst.searchByName(name)) {
        counter++;
        name = originalName + "(" + to_string(counter) + ")";
    }

    Directory dir(name, date);
    bst.insert(dir);
    cout << "Directory '" << name << "' created successfully.\n";
}

void searchDirectory(BST& bst) {
    string name;
    cout << "Enter the name of the directory to search: ";
    cin >> name;
    Node* result = bst.searchByName(name);
    if (result) {
        cout << "Directory found:\n";
        result->dir.display();
    }
    else {
        cout << "Directory not found.\n";
    }
}

void openDirectory(BST& bst) {
    string name;
    cout << "Enter the name of the directory to open: ";
    cin >> name;
    Node* result = bst.searchByName(name);
    if (result) {
        cout << "Opening directory:\n";
        result->dir.display();
    }
    else {
        cout << "Directory not found.\n";
    }
}

void menu() {
    BST bst;
    string choice;

    do {
        cout << "\n";
        cout << "**************************************\n";
        cout << "*    DIRECTORY MANAGEMENT SYSTEM    *\n";
        cout << "**************************************\n";
        cout << "* Consider The Small Cases For Commands *\n";
        cout << "****************************************\n";
        cout << "* To Create Directory   ->   Type 'cr'  *\n";
        cout << "* To Search Directory   ->   Type 'cd'  *\n";
        cout << "* To Open Directory     ->   Type 'open' *\n";
        cout << "* To Delete Directory   ->   Type 'free' *\n";
        cout << "* To Show Directories   ->   Type 'dir'  *\n";
        cout << "* To Exit Program       ->   Type 'exit' *\n";
        cout << "****************************************\n";
        cout << "Enter your choice: ";

        getline(cin, choice);

        if (choice == "cr") {
            createDirectory(bst);
        }
        else if (choice == "cd") {
            searchDirectory(bst);
        }
        else if (choice == "open") {
            openDirectory(bst);
        }
        else if (choice == "free") {
            string name;
            cout << "Enter the name of the directory to delete: ";
            getline(cin, name);
            bst.deleteNodeByName(name);
            cout << "Directory deleted successfully.\n";
        }
        else if (choice == "dir") {
            bst.showAllDirectories();
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