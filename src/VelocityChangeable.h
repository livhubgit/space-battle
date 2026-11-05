#pragma once

#include "Vector.h"

class VelocityChangeable {
public:
    virtual ~VelocityChangeable() = default;
    virtual bool hasVelocity() const = 0;   //есть ли скорость
    virtual Vector getVelocity() const = 0;    //получить скорость
    virtual void setVelocity(Vector newVelocity) = 0;//установить скорость
};