#pragma once

#include <queue>
#include <exception>

#include "commands/Command.h"

//заново выполняет команду, которую ей дали
class RepeatCommand : public Command
{
public:
    RepeatCommand(CommandPtr cmd);
    void execute() override;

private:
    CommandPtr m_cmd;
};