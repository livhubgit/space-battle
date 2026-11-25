#pragma once
#include <memory>

#include "Command.h"
#include "MacroCommand.h"

class FuelConsumable;
class Movable;

class LineMoveCommand : public Command {
public:
    LineMoveCommand(FuelConsumable& fuelconsumable, Movable& movable);
    void execute() override;
private:
    MacroCommand m_macroCmd;
};