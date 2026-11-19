#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
using namespace std;

struct Book {
    int id;
    char title[50];
    char author[50];
    int issuedTo;
};

struct Member {
    int id;
    char name[50];
};

void addBook() {
    Book b{};

    cout << "Enter Book ID: ";
    cin >> b.id;

    ifstream check("books.dat", ios::binary);
    Book temp;

    while (check.read((char*)&temp, sizeof(temp))) {
        if (temp.id == b.id) {
            cout << "Book ID already exists!\n";
            return;
        }
    }
    check.close();

    cin.ignore();
    cout << "Enter Book Title: ";
    cin.getline(b.title, 50);

    cout << "Enter Author Name: ";
    cin.getline(b.author, 50);

    b.issuedTo = 0;

    ofstream file("books.dat", ios::binary | ios::app);
    file.write((char*)&b, sizeof(b));

    cout << "Book Added Successfully!\n";
}

void addMember() {
    Member m{};

    cout << "Enter Member ID: ";
    cin >> m.id;

    ifstream check("members.dat", ios::binary);
    Member temp;

    while (check.read((char*)&temp, sizeof(temp))) {
        if (temp.id == m.id) {
            cout << "Member ID already exists!\n";
            return;
        }
    }
    check.close();

    cin.ignore();
    cout << "Enter Member Name: ";
    cin.getline(m.name, 50);

    ofstream file("members.dat", ios::binary | ios::app);
    file.write((char*)&m, sizeof(m));

    cout << "Member Added Successfully!\n";
}

bool memberExists(int id) {
    ifstream file("members.dat", ios::binary);
    Member m;

    while (file.read((char*)&m, sizeof(m))) {
        if (m.id == id)
            return true;
    }
    return false;
}

void displayBooks() {
    ifstream file("books.dat", ios::binary);
    Book b;

    cout << "\nID\tTitle\t\tAuthor\t\tStatus\n";
    cout << "------------------------------------------------\n";

    bool found = false;

    while (file.read((char*)&b, sizeof(b))) {
        found = true;
        cout << b.id << "\t"
             << b.title << "\t\t"
             << b.author << "\t\t"
             << (b.issuedTo == 0 ? "Available" : "Issued")
             << endl;
    }

    if (!found)
        cout << "No Books Found!\n";
}

void displayMembers() {
    ifstream file("members.dat", ios::binary);
    Member m;

    cout << "\nMember ID\tMember Name\n";
    cout << "------------------------------\n";

    bool found = false;

    while (file.read((char*)&m, sizeof(m))) {
        found = true;
        cout << m.id << "\t\t" << m.name << endl;
    }

    if (!found)
        cout << "No Members Found!\n";
}

void searchBook() {
    int choice;
    char key[50];

    cout << "\n1. Search by Title";
    cout << "\n2. Search by Author";
    cout << "\nEnter Choice: ";
    cin >> choice;

    if (choice != 1 && choice != 2) {
        cout << "Invalid Choice!\n";
        return;
    }

    cin.ignore();
    cout << "Enter Search Text: ";
    cin.getline(key, 50);

    ifstream file("books.dat", ios::binary);
    Book b;
    bool found = false;

    while (file.read((char*)&b, sizeof(b))) {
        char* field = (choice == 1) ? b.title : b.author;

        if (strstr(field, key) != nullptr) {
            cout << "\nBook ID: " << b.id;
            cout << "\nTitle: " << b.title;
            cout << "\nAuthor: " << b.author;
            cout << "\nStatus: "
                 << (b.issuedTo == 0 ? "Available" : "Issued")
                 << "\n";
            found = true;
        }
    }

    if (!found)
        cout << "Book Not Found!\n";
}

void issueBook() {
    int id, memberId;

    cout << "Enter Book ID: ";
    cin >> id;

    cout << "Enter Member ID: ";
    cin >> memberId;

    if (!memberExists(memberId)) {
        cout << "Member Not Found!\n";
        return;
    }

    fstream file("books.dat", ios::binary | ios::in | ios::out);
    Book b;

    while (file.read((char*)&b, sizeof(b))) {
        if (b.id == id) {
            if (b.issuedTo != 0) {
                cout << "Book Already Issued!\n";
                return;
            }

            b.issuedTo = memberId;

            file.seekp(-static_cast<streamoff>(sizeof(b)), ios::cur);
            file.write((char*)&b, sizeof(b));

            cout << "Book Issued Successfully!\n";
            return;
        }
    }

    cout << "Book Not Found!\n";
}

void returnBook() {
    int id, memberId;

    cout << "Enter Book ID: ";
    cin >> id;

    cout << "Enter Member ID: ";
    cin >> memberId;

    fstream file("books.dat", ios::binary | ios::in | ios::out);
    Book b;

    while (file.read((char*)&b, sizeof(b))) {
        if (b.id == id) {
            if (b.issuedTo == 0) {
                cout << "Book Is Already Available!\n";
                return;
            }

            if (b.issuedTo != memberId) {
                cout << "Incorrect Member ID!\n";
                return;
            }

            b.issuedTo = 0;

            file.seekp(-static_cast<streamoff>(sizeof(b)), ios::cur);
            file.write((char*)&b, sizeof(b));

            cout << "Book Returned Successfully!\n";
            return;
        }
    }

    cout << "Book Not Found!\n";
}

int main() {
    int choice;

    do {
        cout << "\n====================================";
        cout << "\n      LIBRARY MANAGEMENT SYSTEM";
        cout << "\n====================================";
        cout << "\n1. Add Book";
        cout << "\n2. Add Member";
        cout << "\n3. Display Books";
        cout << "\n4. Display Members";
        cout << "\n5. Search Book";
        cout << "\n6. Issue Book";
        cout << "\n7. Return Book";
        cout << "\n0. Exit";
        cout << "\n====================================";
        cout << "\nEnter Your Choice: ";

        cin >> choice;

        switch (choice) {
            case 1: addBook(); break;
            case 2: addMember(); break;
            case 3: displayBooks(); break;
            case 4: displayMembers(); break;
            case 5: searchBook(); break;
            case 6: issueBook(); break;
            case 7: returnBook(); break;
            case 0:
                cout << "Thank You!\n";
                break;
            default:
                cout << "Invalid Choice!\n";
        }

    } while (choice != 0);

    return 0;
}