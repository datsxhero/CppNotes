# Makefile for SQL Notebook Application

CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lsqlite3

TARGET = notebook
SRC = notebook.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

# Install SQLite development library (Ubuntu/Debian)
install-sqlite-ubuntu:
	sudo apt-get update
	sudo apt-get install libsqlite3-dev

# Install SQLite development library (macOS)
install-sqlite-macos:
	brew install sqlite3

# Install SQLite development library (CentOS/RHEL/Fedora)
install-sqlite-centos:
	sudo yum install sqlite-devel

# Run the application
run: $(TARGET)
	./$(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all              - Build the application"
	@echo "  clean            - Remove build files and database"
	@echo "  install-sqlite-ubuntu  - Install SQLite dev library (Ubuntu/Debian)"
	@echo "  install-sqlite-macos    - Install SQLite dev library (macOS)"
	@echo "  install-sqlite-centos   - Install SQLite dev library (CentOS/RHEL)"
	@echo "  run              - Build and run the application"
	@echo "  help             - Show this help message"

.PHONY: all clean install-sqlite-ubuntu install-sqlite-macos install-sqlite-centos run help 