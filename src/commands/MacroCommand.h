// MacroCommand.h
#pragma once
#include <vector>

#include "Command.h"

class MacroCommand : public Command
{
public:
    explicit MacroCommand(std::vector<CommandPtr> comArray);
    void execute() override;

private:
    std::vector<CommandPtr> m_comArray;
};