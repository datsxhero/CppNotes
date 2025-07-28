# SQL 記事本（CppNotes）

📒 **CppNotes** 是一款功能強大的 C++ 記事本應用程式，使用 SQLite 資料庫進行資料管理。提供完整的記事管理功能，包括新增、編輯、刪除、搜尋、分類管理等，適合個人學習、待辦事項記錄或日記使用。

## ✨ 主要功能
- 📝 **新增記事**：支援標題、內容、分類的完整記事管理
- ✏️ **編輯記事**：可修改已儲存的記事標題、內容和分類
- ❌ **刪除記事**：可以移除不再需要的記事
- 🔍 **搜尋記事**：支援按標題、內容、分類進行模糊搜尋
- 📌 **標記重要記事**：可標記重要事件，並單獨查看
- 🏷️ **分類管理**：支援一般、工作、個人、學習等分類
- 📊 **SQL 資料庫**：使用 SQLite 提供可靠的資料儲存和查詢
- 🕒 **時間戳記**：自動記錄建立和更新時間
- 🛑 **防呆機制**：避免輸入錯誤或刪除重要記事時的誤操作

## 🖥️ 系統需求
- **作業系統**：Windows / macOS / Linux
- **C++ 編譯器**：GCC / Clang（支援 C++17 以上）
- **SQLite 開發庫**：libsqlite3-dev
- **編輯器**：Visual Studio Code（建議）或其他 C++ IDE

## 🔧 安裝與執行

### 1️⃣ **安裝環境**
請先確認已安裝：
- **VS Code**（[下載 VS Code](https://code.visualstudio.com/)）
- **C++ 編譯器**（`clang` 或 `g++`）
- **SQLite 開發庫**
- **Git（可選）**（[下載 Git](https://git-scm.com/)）

### 2️⃣ **安裝 SQLite 開發庫**

#### **macOS（使用 Homebrew）**
```sh
brew install sqlite3
```

#### **Ubuntu/Debian**
```sh
sudo apt-get update
sudo apt-get install libsqlite3-dev
```

#### **CentOS/RHEL/Fedora**
```sh
sudo yum install sqlite-devel
```

#### **Windows**
- 下載 SQLite 預編譯二進制檔案：[SQLite Downloads](https://www.sqlite.org/download.html)
- 或使用 vcpkg：`vcpkg install sqlite3`

### 3️⃣ **下載專案**
使用 Git Clone 下載專案：
```sh
git clone https://github.com/your_username/notebook_project.git
cd notebook_project
```

### 4️⃣ **編譯與執行**

#### **使用 Makefile（推薦）**
```sh
make all
./notebook
```

#### **使用 g++ 編譯**
```sh
g++ -std=c++17 -Wall -Wextra -g notebook.cpp -o notebook -lsqlite3
./notebook
```

#### **使用 Clang 編譯**
```sh
clang++ -std=c++17 -Wall -Wextra -g notebook.cpp -o notebook -lsqlite3
./notebook
```

## 📊 資料庫結構

應用程式使用 SQLite 資料庫（`notes.db`），包含以下表格結構：

```sql
CREATE TABLE notes (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT NOT NULL,
    content TEXT NOT NULL,
    category TEXT DEFAULT '一般',
    is_important INTEGER DEFAULT 0,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);
```

## ⚙️ VS Code 設定

### **1️⃣ 建立 `tasks.json`（自動編譯）**
1. **開啟 VS Code**
2. 點擊 **View → Command Palette**，輸入 `Tasks: Configure Task`
3. 選擇 **Create tasks.json file from template → C++ (Clang++)**
4. 在 `.vscode/tasks.json` 修改內容如下：
```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "編譯 SQL 記事本",
            "type": "shell",
            "command": "g++",
            "args": [
                "-std=c++17",
                "-Wall",
                "-Wextra",
                "-g",
                "notebook.cpp",
                "-o",
                "notebook",
                "-lsqlite3"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```

### **2️⃣ 執行程式**
1. 在 VS Code **終端機（Terminal）** 輸入：
   ```sh
   ./notebook
   ```
2. 若出現記事本選單，代表程式成功執行！

## 🗂️ 功能說明

### **新增記事**
- 輸入記事標題和內容
- 選擇分類（一般/工作/個人/學習）
- 自動記錄建立時間

### **查看記事**
- 顯示所有記事的表格化列表
- 包含 ID、標題、分類、重要標記、建立時間
- 按建立時間倒序排列

### **編輯記事**
- 選擇要編輯的記事 ID
- 可修改標題、內容、分類
- 自動更新修改時間

### **搜尋記事**
- 支援按標題、內容、分類搜尋
- 使用模糊匹配（LIKE 查詢）
- 顯示符合條件的記事列表

### **重要記事管理**
- 標記/取消標記重要記事
- 單獨查看所有重要記事
- 重要記事顯示 ★ 標記

### **分類管理**
- 預設分類：一般、工作、個人、學習
- 按分類查看記事
- 支援自訂分類名稱

## 📤 將專案上傳至 GitHub
### **1️⃣ 初始化 Git 專案**
```sh
git init
git add .
git commit -m "Initial commit with SQL functionality"
```

### **2️⃣ 連結遠端儲存庫**
```sh
git remote add origin https://github.com/your_username/notebook_project.git
git branch -M main
git push -u origin main
```

## 🛠 開發工具
- **C++17** - 核心開發語言
- **SQLite3** - 資料庫管理
- **VS Code** - 主要開發環境
- **Make** - 建置工具
- **GitHub** - 版本控制與協作


<<<<<<< HEAD
---
🎉 **恭喜！現在你的 SQL 記事本應用程式已經完成並可在 GitHub 上公開分享！**

## 🔧 故障排除

### **編譯錯誤：找不到 sqlite3.h**
```sh
# Ubuntu/Debian
sudo apt-get install libsqlite3-dev

# macOS
brew install sqlite3

# CentOS/RHEL
sudo yum install sqlite-devel
```

### **連結錯誤：找不到 sqlite3 庫**
確保編譯時包含 `-lsqlite3` 參數：
```sh
g++ notebook.cpp -o notebook -lsqlite3
```

### **資料庫權限錯誤**
確保程式有寫入當前目錄的權限：
```sh
chmod +w .
```

=======
>>>>>>> 2f554e572ddaa5a20695cfddf0fd6d0ff71f0f2e
