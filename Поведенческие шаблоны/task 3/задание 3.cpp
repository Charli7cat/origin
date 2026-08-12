#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <memory>

enum class MessageType {
    Warning,
    Error,
    FatalError,
    Unknown
};

class LogMessage {
private:
    MessageType type_;
    std::string message_;

public:
    LogMessage(MessageType type, const std::string& message)
        : type_(type), message_(message) {}

    MessageType type() const {
        return type_;
    }

    const std::string& message() const {
        return message_;
    }
};

class Handler {
protected:
    std::unique_ptr<Handler> next_;

public:
    virtual ~Handler() = default;

    void setNext(std::unique_ptr<Handler> next) {
        next_ = std::move(next);
    }

    virtual void handle(const LogMessage& message) {
        if (next_) {
            next_->handle(message);
        }
    }
};


class FatalErrorHandler : public Handler {
public:
    void handle(const LogMessage& message) override {
        if (message.type() == MessageType::FatalError) {
            throw std::runtime_error("FATAL ERROR: " + message.message());
        }
        Handler::handle(message);
    }
};

class ErrorHandler : public Handler {
private:
    std::string filePath_;

public:
    explicit ErrorHandler(const std::string& path) : filePath_(path) {}

    void handle(const LogMessage& message) override {
        if (message.type() == MessageType::Error) {
            std::ofstream file(filePath_, std::ios::app);
            if (file.is_open()) {
                file << "[ERROR] " << message.message() << std::endl;
                file.close();
            }
            else {
                std::cerr << "The file could not be opened to record the error.: "
                    << filePath_ << std::endl;
            }
            return;
        }
        Handler::handle(message);
    }
};

class WarningHandler : public Handler {
public:
    void handle(const LogMessage& message) override {
        if (message.type() == MessageType::Warning) {
            std::cout << "[WARNING] " << message.message() << std::endl;
            return;
        }
        Handler::handle(message);
    }
};

class UnknownMessageHandler : public Handler {
public:
    void handle(const LogMessage& message) override {
        if (message.type() == MessageType::Unknown) {
            throw std::runtime_error("Unknown message type: " + message.message());
        }
        Handler::handle(message);
    }
};

class HandlerChain {
public:
    static std::unique_ptr<Handler> createDefaultChain(const std::string& errorLogPath = "errors.log") {
        auto fatalHandler = std::make_unique<FatalErrorHandler>();
        auto errorHandler = std::make_unique<ErrorHandler>(errorLogPath);
        auto warningHandler = std::make_unique<WarningHandler>();
        auto unknownHandler = std::make_unique<UnknownMessageHandler>();

        fatalHandler->setNext(std::move(errorHandler));
        fatalHandler->next_->setNext(std::move(warningHandler));
        fatalHandler->next_->next_->setNext(std::move(unknownHandler));

        return fatalHandler;
    }
};

void logMessage(Handler& handler, const LogMessage& message) {
    handler.handle(message);
}

int main() {
    try {
        auto chain = HandlerChain::createDefaultChain("application_errors.log");

        std::cout << "=== Test 1: Warning ===" << std::endl;
        LogMessage warningMsg(MessageType::Warning, "This is a test warning.");
        logMessage(*chain, warningMsg);

        std::cout << "\n=== Test 2: Error (writing to a file) ===" << std::endl;
        LogMessage errorMsg(MessageType::Error, "This is a test error.");
        logMessage(*chain, errorMsg);

        std::cout << "\n=== Test 3: Unknown message ===" << std::endl;
        LogMessage unknownMsg(MessageType::Unknown, "This is an unknown message.");
        logMessage(*chain, unknownMsg);

        std::cout << "\n=== Test 4: Fatal Error ===" << std::endl;
        LogMessage fatalMsg(MessageType::FatalError, "This is a fatal mistake.");
        logMessage(*chain, fatalMsg);

        std::cout << "The program continues to run...." << std::endl;

    }
    catch (const std::runtime_error& e) {
        std::cerr << "\n=== The exception has been caught. ===" << std::endl;
        std::cerr << "Message: " << e.what() << std::endl;
    }

    return 0;
}