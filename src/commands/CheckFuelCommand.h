// CheckFuelCommand.h
#pragma once
#include "FuelConsumable.h"
#include "Command.h"

class CheckFuelCommand : public Command
{
public:
    explicit CheckFuelCommand(FuelConsumable &fuelconsumable);
    void execute() override;

private:
    FuelConsumable &m_fuelconsumable;
};