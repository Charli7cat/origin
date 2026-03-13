#include "Broom.h"

Broom::Broom() {
    name = "Broom";
    speed = 20;
}

double Broom::getDistanceReduction(double distance) const {
    int thousands = static_cast<int>(distance / 1000);
    return 1.0 - (thousands * 0.01);
}