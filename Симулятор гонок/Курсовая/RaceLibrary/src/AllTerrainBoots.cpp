#include "AllTerrainBoots.h"

AllTerrainBoots::AllTerrainBoots() {
    name = "AllTerrainBoots";
    speed = 6;
    restInterval = 60;
}

double AllTerrainBoots::calculateRestDuration(int restCount) const {
    return (restCount == 1) ? 10 : 5;
}