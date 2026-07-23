CC = gcc 
CFLAGS = -Wall -Wextra -O2 
 
CLANG = clang 
CLANGFLAGS = -Wall -Wextra -O2 
 
TARGET = app 
SOURCES = main.c 
 
all: gcc-build clang-build 
 
gcc-build: $(SOURCES) 
[TAB]$(CC) $(CFLAGS) -o $(TARGET)_gcc $(SOURCES) 
[TAB]@echo "=== GCC build complete ===" 
 
clang-build: $(SOURCES) 
[TAB]$(CLANG) $(CLANGFLAGS) -o $(TARGET)_clang $(SOURCES) 
[TAB]@echo "=== CLANG build complete ===" 
 
clean: 
[TAB]rm -f $(TARGET)_gcc $(TARGET)_clang 
 
run-gcc: gcc-build 
[TAB]./$(TARGET)_gcc 
 
run-clang: clang-build 
[TAB]./$(TARGET)_clang 
 
run-all: all 
[TAB]@echo "\n=== RUNNING GCC VERSION ===" 
[TAB]./$(TARGET)_gcc 
[TAB]@echo "\n=== RUNNING CLANG VERSION ===" 
[TAB]./$(TARGET)_clang 
 
.PHONY: all gcc-build clang-build clean run-gcc run-clang run-all 
