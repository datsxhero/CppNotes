#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void addNote();
void viewNotes();
void deleteNote();

int main() {
    int choice;

    while (true) {
        cout << "========== 記事本 ==========" << endl;
        cout << "1. 新增記事" << endl;
        cout << "2. 查看記事" << endl;
        cout << "3. 刪除記事" << endl;
        cout << "4. 退出程式" << endl;
        cout << "請選擇一個選項: ";
        cin >> choice;

        switch (choice) {
            case 1: addNote(); break;
            case 2: viewNotes(); break;
            case 3: deleteNote(); break;
            case 4: cout << "再見！" << endl; return 0;
            default: cout << "無效選項，請重新輸入。" << endl;
        }
    }
}

void addNote() {
    ofstream file("notes.txt", ios::app);
    if (!file) {
        cout << "無法開啟檔案！" << endl;
        return;
    }
    string note;
    cout << "請輸入您的記事: ";
    cin.ignore();
    getline(cin, note);
    file << note << endl;
    cout << "記事已新增！" << endl;
    file.close();
}

void viewNotes() {
    ifstream file("notes.txt");
    if (!file) {
        cout << "無法開啟檔案！" << endl;
        return;
    }
    string line;
    cout << "========== 所有記事 ==========" << endl;
    int lineNumber = 1;
    while (getline(file, line)) {
        cout << lineNumber++ << ". " << line << endl;
    }
    file.close();
}

void deleteNote() {
    ifstream file("notes.txt");
    if (!file) {
        cout << "無法開啟檔案！" << endl;
        return;
    }

    ofstream tempFile("temp.txt");
    if (!tempFile) {
        cout << "無法建立臨時檔案！" << endl;
        return;
    }

    int deleteLine;
    cout << "請輸入要刪除的記事行號: ";
    cin >> deleteLine;

    string line;
    int lineNumber = 1;
    bool deleted = false;

    while (getline(file, line)) {
        if (lineNumber == deleteLine) {
            deleted = true;
        } else {
            tempFile << line << endl;
        }
        lineNumber++;
    }

    file.close();
    tempFile.close();

    remove("notes.txt");
    rename("temp.txt", "notes.txt");

    if (deleted)
        cout << "記事已刪除！" << endl;
    else
        cout << "未找到該行號，刪除失敗。" << endl;
}
