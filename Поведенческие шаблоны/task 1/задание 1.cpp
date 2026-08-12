#include <iostream>
#include <fstream>
#include <string>

class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

class ConsoleLogCommand : public LogCommand {
public:
    void print(const std::string& message) override {
        std::cout << message << std::endl;
    }
};

class FileLogCommand : public LogCommand {
private:
    std::string filePath;

public:
    explicit FileLogCommand(const std::string& path) : filePath(path) {}

    void print(const std::string& message) override {
        std::ofstream file(filePath, std::ios::app);
        if (file.is_open()) {
            file << message << std::endl;
            file.close();
        }
        else {
            std::cerr << "Error: failed to open the file " << filePath << std::endl;
        }
    }
};

void print(LogCommand& command) {
    command.print("Hello, World!");
}

int main() {
    ConsoleLogCommand consoleLogger;
    FileLogCommand fileLogger("log.txt");

    print(consoleLogger);
    print(fileLogger);

    return 0;
}