#pragma once

#include <queue>
#include <exception>

#include "commands/Command.h"

//заново выполняет команду, которую ей дали, возвращает тип
class RepeatTwiceCommand : public Command
{
public:
    RepeatTwiceCommand(CommandPtr cmd);
    void execute() override;

private:
    CommandPtr m_cmd;
};