#pragma once


#include "Command.h"
#include "MacroCommand.h"

class VelocityChangeable;
class Rotable;

class RotateWithVelocityCommand : public Command {
public:
    RotateWithVelocityCommand(Rotable& rotable, VelocityChangeable& velocitychangeable);
    void execute() override;
private:
    MacroCommand m_macroCmd;
};