#include "TransportManager.h"
#include "Transport.h"
#include <iostream>
#include <iomanip>

void TransportManager::printRegisteredTransports() const {
    std::cout << "\n=== Registered transports (" 
              << transports.size() << ") ===\n";
    
    if (transports.empty()) {
        std::cout << "The transports are not registered\n";
        return;
    }
    
    std::cout << std::left 
              << std::setw(5) << "№" 
              << std::setw(20) << "Title" 
              << "Скорость (км/ч)\n";
    std::cout << std::string(50, '-') << "\n";
    
    for (size_t i = 0; i < transports.size(); ++i) {
        std::cout << std::left 
                  << std::setw(5) << (i + 1)
                  << std::setw(20) << transports[i]->getName()
                  << transports[i]->getSpeed() << "\n";
    }
    std::cout << std::string(50, '=') << "\n";
}