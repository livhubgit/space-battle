#pragma once
#include "Angle.h"

class Rotable {
public:
    virtual ~Rotable() = default;
    virtual Angle getAngle() const = 0;
    virtual Angle getAngularVelocity() const = 0;
    virtual void setAngle(Angle newAngle) = 0;
};