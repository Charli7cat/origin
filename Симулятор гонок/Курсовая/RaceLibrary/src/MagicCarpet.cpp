#include "MagicCarpet.h"

MagicCarpet::MagicCarpet() {
    name = "MagicCarpet";
    speed = 10;
}

double MagicCarpet::getDistanceReduction(double distance) const {
    if (distance < 1000) return 1.0;
    if (distance < 5000) return 0.97;
    if (distance < 10000) return 0.90;
    return 0.95;
}