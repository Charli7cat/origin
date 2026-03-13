#include "Eagle.h"

Eagle::Eagle() {
    name = "Eagle";
    speed = 8;
}

double Eagle::getDistanceReduction(double distance) const {
    return 0.94;
}