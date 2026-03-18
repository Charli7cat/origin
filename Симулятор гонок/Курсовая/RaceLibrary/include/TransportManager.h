#pragma once
#include "Export.h"
#include <vector>
#include <string>
#include <memory>

class Transport;

class RACE_API TransportManager {
public:
    static TransportManager& getInstance();
    
    void registerTransport(std::unique_ptr<Transport> transport);
    size_t getRegisteredCount() const { return transports.size(); }
    void printRegisteredTransports() const;
    
    
private:
    TransportManager() = default;
    std::vector<std::unique_ptr<Transport>> transports;
};