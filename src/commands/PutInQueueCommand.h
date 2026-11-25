#pragma once

#include <queue>
#include <exception>

#include "commands/Command.h"

//кладёт указанную команду в очередь
class PutInQueueCommand : public Command
{
public:
    PutInQueueCommand(std::queue<CommandPtr>& q, CommandPtr cmd);
    void execute() override;

private:
    std::queue<CommandPtr>& m_queue;
    CommandPtr m_cmd;

};