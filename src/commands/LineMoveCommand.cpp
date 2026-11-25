#include <vector>
#include <memory>

#include "LineMoveCommand.h"
#include "CheckFuelCommand.h"
#include "Move.h"
#include "BurnFuelCommand.h"
#include "FuelConsumable.h"
#include "Movable.h"

LineMoveCommand::LineMoveCommand(FuelConsumable& fuelconsumable, Movable& movable)
    : m_macroCmd(std::vector<CommandPtr>{
          std::make_shared<CheckFuelCommand>(fuelconsumable),
          std::make_shared<Move>(movable),
          std::make_shared<BurnFuelCommand>(fuelconsumable)})
{}

void LineMoveCommand::execute() {
    m_macroCmd.execute();
}