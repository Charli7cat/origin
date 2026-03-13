#include "Camel.h"

Camel::Camel() {
    name = "Camel";
    speed = 10;
    restInterval = 30;
}

double Camel::calculateRestDuration(int restCount) const {
    return (restCount == 1) ? 5 : 8;
}