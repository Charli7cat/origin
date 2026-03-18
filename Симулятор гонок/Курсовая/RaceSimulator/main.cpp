#include <iostream>
#include <locale>
#include <windows.h>
#include <memory>
#include <vector>
#include <iomanip>
#include "Race.h"
#include "Camel.h"
#include "FastCamel.h"
#include "Centaur.h"
#include "AllTerrainBoots.h"
#include "MagicCarpet.h"
#include "Eagle.h"
#include "Broom.h"

int main() {
    // Set console encoding for Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
    
    std::cout << "====================================\n";
    std::cout << "   WELCOME TO THE RACING SIMULATOR\n";
    std::cout << "====================================\n";
    
    while (true) {
        try {
            std::cout << "\n=== RACE TYPE SELECTION ===\n";
            std::cout << "1. Ground vehicles only\n";
            std::cout << "2. Air vehicles only\n";
            std::cout << "3. Ground and air vehicles\n";
            std::cout << "Choose race type (1-3): ";
            
            int typeChoice;
            std::cin >> typeChoice;
            
            RaceType raceType;
            if (typeChoice == 1) raceType = RaceType::GROUND_ONLY;
            else if (typeChoice == 2) raceType = RaceType::AIR_ONLY;
            else if (typeChoice == 3) raceType = RaceType::ANY;
            else {
                std::cout << "Invalid choice!\n";
                continue;
            }
            
            std::cout << "\n=== DISTANCE INPUT ===\n";
            std::cout << "Enter distance (km): ";
            
            double distance;
            std::cin >> distance;
            
            if (distance <= 0) {
                std::cout << "Distance must be positive!\n";
                continue;
            }
            
            Race race(distance, raceType);
            
            std::cout << "\nRace created: " << distance << " km, type: " 
                      << raceTypeToString(raceType) << "\n";
            
            std::cout << "\n=== PARTICIPANT REGISTRATION ===\n";
            
            while (true) {
                // Show current registered vehicles
                std::cout << "\n----------------------------------------\n";
                std::cout << "REGISTERED VEHICLES (" << race.getParticipantCount() << "):\n";
                
                auto registeredNames = race.getParticipantNames();
                if (registeredNames.empty()) {
                    std::cout << "  No vehicles registered yet\n";
                } else {
                    for (size_t i = 0; i < registeredNames.size(); ++i) {
                        std::cout << "  " << (i+1) << ". " << registeredNames[i] << "\n";
                    }
                }
                std::cout << "----------------------------------------\n";
                
                std::cout << "\nAvailable vehicles to register:\n";
                std::cout << "1. Camel (ground)\n";
                std::cout << "2. Fast Camel (ground)\n";
                std::cout << "3. Centaur (ground)\n";
                std::cout << "4. All-terrain boots (ground)\n";
                std::cout << "5. Magic carpet (air)\n";
                std::cout << "6. Eagle (air)\n";
                std::cout << "7. Broom (air)\n";
                std::cout << "0. Start the race\n";
                std::cout << "Your choice: ";
                
                int vehicleChoice;
                std::cin >> vehicleChoice;
                
                if (vehicleChoice == 0) {
                    if (race.canStart()) {
                        std::cout << "Registration complete. Starting the race!\n";
                        break;
                    } else {
                        std::cout << "Error! Need at least 2 vehicles. Current: " 
                                  << race.getParticipantCount() << "\n";
                        continue;
                    }
                }
                
                std::unique_ptr<Vehicle> vehicle;
                std::string vehicleName;
                
                switch(vehicleChoice) {
                    case 1: 
                        vehicle = std::make_unique<Camel>();
                        vehicleName = "Camel";
                        break;
                    case 2: 
                        vehicle = std::make_unique<FastCamel>();
                        vehicleName = "Fast Camel";
                        break;
                    case 3: 
                        vehicle = std::make_unique<Centaur>();
                        vehicleName = "Centaur";
                        break;
                    case 4: 
                        vehicle = std::make_unique<AllTerrainBoots>();
                        vehicleName = "All-terrain boots";
                        break;
                    case 5: 
                        vehicle = std::make_unique<MagicCarpet>();
                        vehicleName = "Magic carpet";
                        break;
                    case 6: 
                        vehicle = std::make_unique<Eagle>();
                        vehicleName = "Eagle";
                        break;
                    case 7: 
                        vehicle = std::make_unique<Broom>();
                        vehicleName = "Broom";
                        break;
                    default:
                        std::cout << "Invalid choice!\n";
                        continue;
                }
                
                if (race.registerVehicle(std::move(vehicle))) {
                    std::cout << vehicleName << " successfully registered!\n";
                } else {
                    std::cout << "Registration error! " << vehicleName 
                              << " may already be registered or not suitable for this race.\n";
                }
            }
            
            auto results = race.start();
            
            std::cout << "\n=== RACE RESULTS ===\n";
            std::cout << std::left << std::setw(6) << "Place" 
                      << std::setw(22) << "Vehicle" 
                      << "Time (hours)\n";
            std::cout << "----------------------------------------\n";
            
            int place = 1;
            for (const auto& result : results) {
                std::cout << std::left << std::setw(6) << place++
                          << std::setw(22) << result.name
                          << std::fixed << std::setprecision(2) << result.time << "\n";
            }
            
            std::cout << "\n=== CONTINUE? ===\n";
            std::cout << "1. Run another race\n";
            std::cout << "2. Exit program\n";
            std::cout << "Choose action (1-2): ";
            
            int again;
            std::cin >> again;
            
            if (again != 1) {
                break;
            }
            
        } catch (const std::exception& e) {
            std::cerr << "Error occurred: " << e.what() << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Press Enter to continue...";
            std::cin.get();
        }
    }
    
    std::cout << "\nThank you for using the racing simulator! Goodbye!\n";
    return 0;
}