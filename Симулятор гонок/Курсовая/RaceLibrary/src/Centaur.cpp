#include "Centaur.h"

Centaur::Centaur() {
    name = "Centaur";
    speed = 15;
    restInterval = 8;
}

double Centaur::calculateRestDuration(int restCount) const {
    return 2;
}