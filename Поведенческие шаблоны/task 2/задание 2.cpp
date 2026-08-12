#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <mutex>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};


class WarningObserver : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "[WARNING] " << message << std::endl;
    }
};

class ErrorObserver : public Observer {
private:
    std::string filePath;

public:
    explicit ErrorObserver(const std::string& path) : filePath(path) {}

    void onError(const std::string& message) override {
        std::ofstream file(filePath, std::ios::app);
        if (file.is_open()) {
            file << "[ERROR] " << message << std::endl;
            file.close();
        }
        else {
            std::cerr << "Error: failed to open the file " << filePath << std::endl;
        }
    }
};

class FatalErrorObserver : public Observer {
private:
    std::string filePath;

public:
    explicit FatalErrorObserver(const std::string& path) : filePath(path) {}

    void onFatalError(const std::string& message) override {
        std::cout << "[FATAL ERROR] " << message << std::endl;

        std::ofstream file(filePath, std::ios::app);
        if (file.is_open()) {
            file << "[FATAL ERROR] " << message << std::endl;
            file.close();
        }
        else {
            std::cerr << "Error: failed to open the file " << filePath << std::endl;
        }
    }
};

class LogSubject {
private:
    std::vector<Observer*> observers;
    mutable std::mutex mtx;

    template<typename Func>
    void notifyObservers(Func func) const {
        std::lock_guard<std::mutex> lock(mtx);

        auto observers_copy = observers;

        for (Observer* observer : observers_copy) {
            if (observer != nullptr) {
                func(observer);
            }
        }

        observers.erase(
            std::remove(observers.begin(), observers.end(), nullptr),
            observers.end()
        );
    }

public:
    void addObserver(Observer* observer) {
        if (observer == nullptr) {
            return;
        }

        std::lock_guard<std::mutex> lock(mtx);

        if (std::find(observers.begin(), observers.end(), observer) == observers.end()) {
            observers.push_back(observer);
        }
    }

    void removeObserver(Observer* observer) {
        std::lock_guard<std::mutex> lock(mtx);

        auto it = std::find(observers.begin(), observers.end(), observer);
        if (it != observers.end()) {
            *it = nullptr;
        }
    }

    void warning(const std::string& message) const {
        notifyObservers([&message](Observer* observer) {
            observer->onWarning(message);
            });
    }

    void error(const std::string& message) const {
        notifyObservers([&message](Observer* observer) {
            observer->onError(message);
            });
    }

    void fatalError(const std::string& message) const {
        notifyObservers([&message](Observer* observer) {
            observer->onFatalError(message);
            });
    }
};

int main() {
    LogSubject logger;

    WarningObserver warningObserver;
    ErrorObserver errorObserver("errors.log");
    FatalErrorObserver fatalObserver("fatal_errors.log");

    logger.addObserver(&warningObserver);
    logger.addObserver(&errorObserver);
    logger.addObserver(&fatalObserver);

    logger.warning("This is a warning.");
    logger.error("This is a mistake.");
    logger.fatalError("This is a fatal mistake.");

    {
        WarningObserver tempObserver;
        logger.addObserver(&tempObserver);
        logger.warning("Message from a temporary observer");
    }

    logger.warning("After the destruction of the temporary observer");

    logger.removeObserver(&warningObserver);
    logger.warning("This message will not be seen by WarningObserver.");

    return 0;
}