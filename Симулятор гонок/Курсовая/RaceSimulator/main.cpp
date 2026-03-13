#include <iostream>
#include <locale>
#include <windows.h>
#include <memory>
#include <vector>
#include "Race.h"
#include "Camel.h"
#include "FastCamel.h"
#include "Centaur.h"
#include "AllTerrainBoots.h"
#include "MagicCarpet.h"
#include "Eagle.h"
#include "Broom.h"

int main() {
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
                std::cout << "\nRegistered vehicles: " << race.getParticipantCount() << "\n";
                std::cout << "Available vehicles:\n";
                std::cout << "1. Camel\n";
                std::cout << "2. Fast Camel\n";
                std::cout << "3. Centaur\n";
                std::cout << "4. All-terrain boots\n";
                std::cout << "5. Magic carpet\n";
                std::cout << "6. Eagle\n";
                std::cout << "7. Broom\n";
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
                switch(vehicleChoice) {
                    case 1: vehicle = std::make_unique<Camel>(); break;
                    case 2: vehicle = std::make_unique<FastCamel>(); break;
                    case 3: vehicle = std::make_unique<Centaur>(); break;
                    case 4: vehicle = std::make_unique<AllTerrainBoots>(); break;
                    case 5: vehicle = std::make_unique<MagicCarpet>(); break;
                    case 6: vehicle = std::make_unique<Eagle>(); break;
                    case 7: vehicle = std::make_unique<Broom>(); break;
                    default:
                        std::cout << "Invalid choice!\n";
                        continue;
                }
                
                if (race.registerVehicle(std::move(vehicle))) {
                    std::cout << "Vehicle successfully registered!\n";
                } else {
                    std::cout << "Registration error! Vehicle may already be registered or not suitable for this race.\n";
                }
            }
            
            auto results = race.start();
            
            std::cout << "\n=== RACE RESULTS ===\n";
            std::cout << "Place | Vehicle              | Time (hours)\n";
            std::cout << "----------------------------------------\n";
            
            int place = 1;
            for (const auto& result : results) {
                printf("%2d    | %-20s | %.2f\n", 
                       place++, result.name.c_str(), result.time);
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