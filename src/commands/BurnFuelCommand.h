// BurnFuelCommand.h
#pragma once
#include "FuelConsumable.h"
#include "Command.h"

class BurnFuelCommand : public Command
{
public:
    explicit BurnFuelCommand(FuelConsumable &fuelconsumable);
    void execute() override;

private:
    FuelConsumable &m_fuelconsumable;
};