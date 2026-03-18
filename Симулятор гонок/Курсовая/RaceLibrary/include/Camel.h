#pragma once
#include "GroundVehicle.h"
#include "Export.h"

class RACE_API Camel : public GroundVehicle {
public:
    Camel();
protected:
    double calculateRestDuration(int restCount) const override;
};