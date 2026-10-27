#pragma once
#include "Vector.h"

class Movable {
public:
    virtual ~Movable() = default;
    virtual Vector getPosition() const = 0;
    virtual Vector getVelocity() const = 0;
    virtual void setPosition(Vector newPosition) = 0;
};