#pragma once

#include <queue>
#include <exception>
#include "commands/Command.h"


class CommandProcessor {
public:
    ~CommandProcessor() = default;
    void work();
    std::queue<CommandPtr>& queue() { return m_commandsQueue; }

private:
    std::queue<CommandPtr> m_commandsQueue;
};