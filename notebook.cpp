#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sqlite3.h>

using namespace std;

// 資料庫相關函式宣告
bool initDatabase();
void addNote();
void viewNotes();
void deleteNote();
void editNote();
void searchNotes();
void markImportant();
void viewImportantNotes();
void viewNotesByCategory();

// 全域資料庫連線
sqlite3* db = nullptr;

// 取得目前時間字串
string getCurrentTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}

// 初始化資料庫
bool initDatabase() {
    int rc = sqlite3_open("notes.db", &db);
    if (rc) {
        cout << "無法開啟資料庫: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    
    // 建立記事表
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS notes (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            content TEXT NOT NULL,
            category TEXT DEFAULT '一般',
            is_important INTEGER DEFAULT 0,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )";
    
    char* errMsg = 0;
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cout << "SQL 錯誤: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    return true;
}

// 新增記事
void addNote() {
    string title, content, category;
    
    cout << "請輸入記事標題: ";
    cin.ignore();
    getline(cin, title);
    
    cout << "請輸入記事內容: ";
    getline(cin, content);
    
    cout << "請選擇分類 (一般/工作/個人/學習) [預設: 一般]: ";
    getline(cin, category);
    if (category.empty()) category = "一般";
    
    const char* sql = "INSERT INTO notes (title, content, category, created_at, updated_at) VALUES (?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        cout << "準備 SQL 語句失敗: " << sqlite3_errmsg(db) << endl;
        return;
    }
    
    string currentTime = getCurrentTime();
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, content.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, category.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, currentTime.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, currentTime.c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        cout << "記事已新增！" << endl;
    } else {
        cout << "新增記事失敗: " << sqlite3_errmsg(db) << endl;
    }
    
    sqlite3_finalize(stmt);
}

// 查看所有記事
void viewNotes() {
    const char* sql = "SELECT id, title, content, category, is_important, created_at FROM notes ORDER BY created_at DESC";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        cout << "準備 SQL 語句失敗: " << sqlite3_errmsg(db) << endl;
        return;
    }
    
    cout << "\n========== 所有記事 ==========" << endl;
    cout << setw(5) << "ID" << setw(20) << "標題" << setw(15) << "分類" 
         << setw(10) << "重要" << setw(20) << "建立時間" << endl;
    cout << string(80, '-') << endl;
    
    bool hasNotes = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        hasNotes = true;
        int id = sqlite3_column_int(stmt, 0);
        const char* title = (const char*)sqlite3_column_text(stmt, 1);
        const char* content = (const char*)sqlite3_column_text(stmt, 2);
        const char* category = (const char*)sqlite3_column_text(stmt, 3);
        int isImportant = sqlite3_column_int(stmt, 4);
        const char* createdAt = (const char*)sqlite3_column_text(stmt, 5);
        
        cout << setw(5) << id << setw(20) << (title ? title : "") 
             << setw(15) << (category ? category : "") 
             << setw(10) << (isImportant ? "★" : "") 
             << setw(20) << (createdAt ? createdAt : "") << endl;
    }
    
    if (!hasNotes) {
        cout << "目前沒有任何記事。" << endl;
    }
    
    sqlite3_finalize(stmt);
}

// 刪除記事
void deleteNote() {
    viewNotes();
    
    int id;
    cout << "\n請輸入要刪除的記事 ID: ";
    cin >> id;
    
    const char* sql = "DELETE FROM notes WHERE id = ?";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        cout << "準備 SQL 語句失敗: " << sqlite3_errmsg(db) << endl;
        return;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        if (sqlite3_changes(db) > 0) {
            cout << "記事已刪除！" << endl;
        } else {
            cout << "未找到該記事。" << endl;
        }
    } else {
        cout << "刪除記事失敗: " << sqlite3_errmsg(db) << endl;
    }
    
    sqlite3_finalize(stmt);
}

// 編輯記事
void editNote() {
    viewNotes();
    
    int id;
    cout << "\n請輸入要編輯的記事 ID: ";
    cin >> id;
    
    // 先查詢現有記事
    const char* selectSql = "SELECT title, content, category FROM notes WHERE id = ?";
    sqlite3_stmt* selectStmt;
    
    int rc = sqlite3_prepare_v2(db, selectSql, -1, &selectStmt, 0);
    if (rc != SQLITE_OK) {
        cout << "準備 SQL 語句失敗: " << sqlite3_errmsg(db) << endl;
        return;
    }
    
    sqlite3_bind_int(selectStmt, 1, id);
    
    if (sqlite3_step(selectStmt) != SQLITE_ROW) {
        cout << "未找到該記事。" << endl;
        sqlite3_finalize(selectStmt);
        return;
    }
    
    string currentTitle = (const char*)sqlite3_column_text(selectStmt, 0);
    string currentContent = (const char*)sqlite3_column_text(selectStmt, 1);
    string currentCategory = (const char*)sqlite3_column_text(selectStmt, 2);
    
    sqlite3_finalize(selectStmt);
    
    string newTitle, newContent, newCategory;
    
    cout << "當前標題: " << currentTitle << endl;
    cout << "新標題 (按 Enter 保持不變): ";
    cin.ignore();
    getline(cin, newTitle);
    if (newTitle.empty()) newTitle = currentTitle;
    
    cout << "當前內容: " << currentContent << endl;
    cout << "新內容 (按 Enter 保持不變): ";
    getline(cin, newContent);
    if (newContent.empty()) newContent = currentContent;
    
    cout << "當前分類: " << currentCategory << endl;
    cout << "新分類 (按 Enter 保持不變): ";
    getline(cin, newCategory);
    if (newCategory.empty()) newCategory = currentCategory;
    
    // 更新記事
    const char* updateSql = "UPDATE notes SET title = ?, content = ?, category = ?, updated_at = ? WHERE id = ?";
    sqlite3_stmt* updateStmt;
    
    rc = sqlite3_prepare_v2(db, updateSql, -1, &updateStmt, 0);
    if (rc != SQLITE_OK) {
        cout << "準備 SQL 語句失敗: " << sqlite3_errmsg(db) << endl;
        return;
    }
    
    string currentTime = getCurrentTime();
    sqlite3_bind_text(updateStmt, 1, newTitle.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(updateStmt, 2, newContent.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(updateStmt, 3, newCategory.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(updateStmt, 4, currentTime.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(updateStmt, 5, id);
    
    rc = sqlite3_step(updateStmt);
    if (rc == SQLITE_DONE) {
        cout << "記事已更新！" << endl;
    } else {
        cout << "更新記事失敗: " << sqlite3_errmsg(db) << endl;
    }
    
    sqlite3_finalize(updateStmt);
}

// 搜尋記事
void searchNotes() {
    cout << "\n========== 搜尋記事 ==========" << endl;
    cout << "1. 按標題搜尋" << endl;
    cout << "2. 按內容搜尋" << endl;
    cout << "3. 按分類搜尋" << endl;
    cout << "請選擇搜尋方式: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    string searchTerm;
    cout << "請輸入搜尋關鍵字: ";
    getline(cin, searchTerm);
    
    string sql;
    switch (choice) {
        case 1:
            sql = "SELECT id, title, content, category, is_important, created_at FROM notes WHERE title LIKE ? ORDER BY created_at DESC";
            break;
        case 2:
            sql = "SELECT id, title, content, category, is_important, created_at FROM notes WHERE content LIKE ? ORDER BY created_at DESC";
            break;
        case 3:
            sql = "SELECT id, title, content, category, is_important, created_at FROM notes WHERE category LIKE ? ORDER BY created_at DESC";
            break;
        default:
            cout << "無效選項！" << endl;
            return;
    }
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        cout << "準備 SQL 語句失敗: " << sqlite3_errmsg(db) << endl;
        return;
    }
    
    string searchPattern = "%" + searchTerm + "%";
    sqlite3_bind_text(stmt, 1, searchPattern.c_str(), -1, SQLITE_STATIC);
    
    cout << "\n========== 搜尋結果 ==========" << endl;
    cout << setw(5) << "ID" << setw(20) << "標題" << setw(15) << "分類" 
         << setw(10) << "重要" << setw(20) << "建立時間" << endl;
    cout << string(80, '-') << endl;
    
    bool hasResults = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        hasResults = true;
        int id = sqlite3_column_int(stmt, 0);
        const char* title = (const char*)sqlite3_column_text(stmt, 1);
        const char* content = (const char*)sqlite3_column_text(stmt, 2);
        const char* category = (const char*)sqlite3_column_text(stmt, 3);
        int isImportant = sqlite3_column_int(stmt, 4);
        const char* createdAt = (const char*)sqlite3_column_text(stmt, 5);
        
        cout << setw(5) << id << setw(20) << (title ? title : "") 
             << setw(15) << (category ? category : "") 
             << setw(10) << (isImportant ? "★" : "") 
             << setw(20) << (createdAt ? createdAt : "") << endl;
    }
    
    if (!hasResults) {
        cout << "沒有找到符合的記事。" << endl;
    }
    
    sqlite3_finalize(stmt);
}

// 標記重要記事
void markImportant() {
    viewNotes();
    
    int id;
    cout << "\n請輸入要標記的記事 ID: ";
    cin >> id;
    
    const char* sql = "UPDATE notes SET is_important = CASE WHEN is_important = 0 THEN 1 ELSE 0 END WHERE id = ?";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        cout << "準備 SQL 語句失敗: " << sqlite3_errmsg(db) << endl;
        return;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        if (sqlite3_changes(db) > 0) {
            cout << "記事標記已更新！" << endl;
        } else {
            cout << "未找到該記事。" << endl;
        }
    } else {
        cout << "更新標記失敗: " << sqlite3_errmsg(db) << endl;
    }
    
    sqlite3_finalize(stmt);
}

// 查看重要記事
void viewImportantNotes() {
    const char* sql = "SELECT id, title, content, category, created_at FROM notes WHERE is_important = 1 ORDER BY created_at DESC";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        cout << "準備 SQL 語句失敗: " << sqlite3_errmsg(db) << endl;
        return;
    }
    
    cout << "\n========== 重要記事 ==========" << endl;
    cout << setw(5) << "ID" << setw(20) << "標題" << setw(15) << "分類" 
         << setw(20) << "建立時間" << endl;
    cout << string(70, '-') << endl;
    
    bool hasNotes = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        hasNotes = true;
        int id = sqlite3_column_int(stmt, 0);
        const char* title = (const char*)sqlite3_column_text(stmt, 1);
        const char* content = (const char*)sqlite3_column_text(stmt, 2);
        const char* category = (const char*)sqlite3_column_text(stmt, 3);
        const char* createdAt = (const char*)sqlite3_column_text(stmt, 4);
        
        cout << setw(5) << id << setw(20) << (title ? title : "") 
             << setw(15) << (category ? category : "") 
             << setw(20) << (createdAt ? createdAt : "") << endl;
    }
    
    if (!hasNotes) {
        cout << "目前沒有任何重要記事。" << endl;
    }
    
    sqlite3_finalize(stmt);
}

// 按分類查看記事
void viewNotesByCategory() {
    cout << "\n請選擇分類:" << endl;
    cout << "1. 一般" << endl;
    cout << "2. 工作" << endl;
    cout << "3. 個人" << endl;
    cout << "4. 學習" << endl;
    cout << "請輸入選項: ";
    
    int choice;
    cin >> choice;
    
    string category;
    switch (choice) {
        case 1: category = "一般"; break;
        case 2: category = "工作"; break;
        case 3: category = "個人"; break;
        case 4: category = "學習"; break;
        default:
            cout << "無效選項！" << endl;
            return;
    }
    
    const char* sql = "SELECT id, title, content, is_important, created_at FROM notes WHERE category = ? ORDER BY created_at DESC";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        cout << "準備 SQL 語句失敗: " << sqlite3_errmsg(db) << endl;
        return;
    }
    
    sqlite3_bind_text(stmt, 1, category.c_str(), -1, SQLITE_STATIC);
    
    cout << "\n========== " << category << " 記事 ==========" << endl;
    cout << setw(5) << "ID" << setw(20) << "標題" << setw(10) << "重要" 
         << setw(20) << "建立時間" << endl;
    cout << string(65, '-') << endl;
    
    bool hasNotes = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        hasNotes = true;
        int id = sqlite3_column_int(stmt, 0);
        const char* title = (const char*)sqlite3_column_text(stmt, 1);
        const char* content = (const char*)sqlite3_column_text(stmt, 2);
        int isImportant = sqlite3_column_int(stmt, 3);
        const char* createdAt = (const char*)sqlite3_column_text(stmt, 4);
        
        cout << setw(5) << id << setw(20) << (title ? title : "") 
             << setw(10) << (isImportant ? "★" : "") 
             << setw(20) << (createdAt ? createdAt : "") << endl;
    }
    
    if (!hasNotes) {
        cout << "該分類下沒有任何記事。" << endl;
    }
    
    sqlite3_finalize(stmt);
}

int main() {
    cout << "正在初始化資料庫..." << endl;
    if (!initDatabase()) {
        cout << "資料庫初始化失敗！" << endl;
        return 1;
    }
    cout << "資料庫初始化成功！" << endl;
    
    int choice;
    
    while (true) {
        cout << "\n========== SQL 記事本 ==========" << endl;
        cout << "1. 新增記事" << endl;
        cout << "2. 查看所有記事" << endl;
        cout << "3. 編輯記事" << endl;
        cout << "4. 刪除記事" << endl;
        cout << "5. 搜尋記事" << endl;
        cout << "6. 標記重要記事" << endl;
        cout << "7. 查看重要記事" << endl;
        cout << "8. 按分類查看記事" << endl;
        cout << "9. 退出程式" << endl;
        cout << "請選擇一個選項: ";
        cin >> choice;
        
        switch (choice) {
            case 1: addNote(); break;
            case 2: viewNotes(); break;
            case 3: editNote(); break;
            case 4: deleteNote(); break;
            case 5: searchNotes(); break;
            case 6: markImportant(); break;
            case 7: viewImportantNotes(); break;
            case 8: viewNotesByCategory(); break;
            case 9: 
                cout << "再見！" << endl; 
                if (db) sqlite3_close(db);
                return 0;
            default: cout << "無效選項，請重新輸入。" << endl;
        }
    }
    
    return 0;
}
