#pragma once

class FuelConsumable {
public:
    virtual ~FuelConsumable() = default;
    virtual int getFuel() const = 0;            //текущий запас 
    virtual int getFuelBurnRate() const = 0;    //расход за шаг
    virtual void setFuel(int newFuel) = 0;
};