#include "FastCamel.h"

FastCamel::FastCamel() {
    name = "FastCamel";
    speed = 40;
    restInterval = 10;
}

double FastCamel::calculateRestDuration(int restCount) const {
    if (restCount == 1) return 5;
    if (restCount == 2) return 6.5;
    return 8;
}