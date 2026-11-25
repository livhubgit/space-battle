#include <vector>
#include <memory>

#include "RotateWithVelocityCommand.h"
#include "Rotate.h"
#include "commands/ChangeVelocityCommand.h"
#include "CheckFuelCommand.h"


RotateWithVelocityCommand::RotateWithVelocityCommand(Rotable& rotable, VelocityChangeable& velocitychangeable)
    : m_macroCmd(std::vector<CommandPtr>{
          std::make_shared<Rotate>(rotable),
          std::make_shared<ChangeVelocityCommand>(rotable,velocitychangeable)})
{}

void RotateWithVelocityCommand::execute() {
    m_macroCmd.execute();
}