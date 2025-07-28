#!/bin/bash

# SQL Notebook Build Script
# 自動檢測系統並編譯 SQL 記事本應用程式

echo "🔧 SQL 記事本編譯腳本"
echo "========================"

# 檢測作業系統
if [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macOS"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    if command -v apt-get &> /dev/null; then
        OS="Ubuntu/Debian"
    elif command -v yum &> /dev/null; then
        OS="CentOS/RHEL"
    else
        OS="Linux"
    fi
else
    OS="Unknown"
fi

echo "檢測到作業系統: $OS"

# 檢查 C++ 編譯器
if command -v g++ &> /dev/null; then
    COMPILER="g++"
    echo "✅ 找到 g++ 編譯器"
elif command -v clang++ &> /dev/null; then
    COMPILER="clang++"
    echo "✅ 找到 clang++ 編譯器"
else
    echo "❌ 未找到 C++ 編譯器，請安裝 g++ 或 clang++"
    exit 1
fi

# 檢查 SQLite 開發庫
check_sqlite() {
    if [[ "$OS" == "macOS" ]]; then
        if brew list sqlite3 &> /dev/null; then
            return 0
        fi
    else
        if pkg-config --exists sqlite3; then
            return 0
        fi
    fi
    return 1
}

if check_sqlite; then
    echo "✅ SQLite 開發庫已安裝"
else
    echo "⚠️  SQLite 開發庫未安裝，正在嘗試安裝..."
    
    case $OS in
        "macOS")
            if command -v brew &> /dev/null; then
                echo "使用 Homebrew 安裝 SQLite..."
                brew install sqlite3
            else
                echo "❌ 請先安裝 Homebrew: https://brew.sh/"
                exit 1
            fi
            ;;
        "Ubuntu/Debian")
            echo "使用 apt-get 安裝 SQLite 開發庫..."
            sudo apt-get update
            sudo apt-get install -y libsqlite3-dev
            ;;
        "CentOS/RHEL")
            echo "使用 yum 安裝 SQLite 開發庫..."
            sudo yum install -y sqlite-devel
            ;;
        *)
            echo "❌ 無法自動安裝 SQLite 開發庫"
            echo "請手動安裝 SQLite 開發庫後再執行此腳本"
            exit 1
            ;;
    esac
fi

# 編譯應用程式
echo ""
echo "🔨 開始編譯..."
echo "使用編譯器: $COMPILER"

if [[ "$OS" == "macOS" ]]; then
    # macOS 需要指定 SQLite 路徑
    $COMPILER -std=c++17 -Wall -Wextra -g notebook.cpp -o notebook -lsqlite3
else
    # Linux 使用 pkg-config
    $COMPILER -std=c++17 -Wall -Wextra -g notebook.cpp -o notebook $(pkg-config --cflags --libs sqlite3)
fi

if [ $? -eq 0 ]; then
    echo "✅ 編譯成功！"
    echo "執行檔: ./notebook"
    echo ""
    echo "要執行程式，請輸入:"
    echo "  ./notebook"
else
    echo "❌ 編譯失敗！"
    echo "請檢查錯誤訊息並修正問題"
    exit 1
fi

# 檢查是否要立即執行
echo ""
read -p "是否要立即執行程式？(y/n): " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "🚀 啟動 SQL 記事本..."
    ./notebook
fi 