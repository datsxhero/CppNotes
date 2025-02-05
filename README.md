# 記事本（CppNotes）

📒 **CppNotes** 是一款簡單的 C++ 記事本應用程式，讓使用者可以新增、編輯、刪除記事，並提供重要事件提醒與字體顏色更換功能。適合個人學習、待辦事項記錄或日記使用。

## ✨ 主要功能
- 📝 **新增記事**：使用者可以輸入並儲存新記事。
- ✏️ **編輯記事**：可修改已儲存的記事內容。
- ❌ **刪除記事**：可以移除不再需要的記事。
- 📌 **標記重要記事**：可標記重要事件，以便快速查看。
- 🎨 **更換字體顏色**：讓使用者自訂記事顏色，方便分類管理。
- 🛑 **防呆機制**：避免輸入錯誤或刪除重要記事時的誤操作。

## 🖥️ 系統需求
- **作業系統**：Windows / macOS / Linux
- **C++ 編譯器**：GCC / Clang（支援 C++17 以上）
- **編輯器**：Visual Studio Code（建議）或其他 C++ IDE

## 🔧 安裝與執行

### 1️⃣ **安裝環境**
請先確認已安裝：
- **VS Code**（[下載 VS Code](https://code.visualstudio.com/)）
- **C++ 編譯器**（`clang` 或 `g++`）
- **Git（可選）**（[下載 Git](https://git-scm.com/)）

### 2️⃣ **下載專案**
使用 Git Clone 下載專案：
```sh
git clone https://github.com/your_username/notebook_project.git
cd notebook_project
```

### 3️⃣ **編譯與執行**
#### **使用 g++ 編譯（推薦）**
```sh
g++ -o notebook notebook.cpp
./notebook
```

#### **使用 Clang 編譯**
```sh
clang++ -o notebook notebook.cpp
./notebook
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
            "label": "編譯 C++ 檔案",
            "type": "shell",
            "command": "g++",
            "args": [
                "-g",
                "notebook.cpp",
                "-o",
                "notebook"
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

## 📤 將專案上傳至 GitHub
### **1️⃣ 初始化 Git 專案**
```sh
git init
git add .
git commit -m "Initial commit"
```

### **2️⃣ 連結遠端儲存庫**
```sh
git remote add origin https://github.com/your_username/notebook_project.git
git branch -M main
git push -u origin main
```

## 🛠 開發工具
- **C++** - 核心開發語言
- **VS Code** - 主要開發環境
- **GitHub** - 版本控制與協作

## 📜 授權
本專案採用 **MIT License**，詳見 [LICENSE](LICENSE)。

---
🎉 **恭喜！現在你的記事本應用程式已經完成並可在 GitHub 上公開分享！**

