// BurnFuelCommand.cpp
#include "BurnFuelCommand.h"

BurnFuelCommand::BurnFuelCommand(FuelConsumable &fuelconsumable) : m_fuelconsumable(fuelconsumable) {}

void BurnFuelCommand::execute()
{
    m_fuelconsumable.setFuel(m_fuelconsumable.getFuel()-m_fuelconsumable.getFuelBurnRate());
}