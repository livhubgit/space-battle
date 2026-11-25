// CheckFuelCommand.cpp
#include "CheckFuelCommand.h"
#include "CommandException.h"

CheckFuelCommand::CheckFuelCommand(FuelConsumable &fuelconsumable) : m_fuelconsumable(fuelconsumable) {}

void CheckFuelCommand::execute()
{
    if (m_fuelconsumable.getFuel() < m_fuelconsumable.getFuelBurnRate())
    {
        throw CommandException("CheckFuelCommand execution failed - not enough fuel");
    }
}