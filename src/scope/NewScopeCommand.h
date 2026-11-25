#pragma once
#include <string>
#include "commands/Command.h"


class NewScopeCommand : public Command {
    std::string m_id;
public:
    explicit NewScopeCommand(std::string id);
    void execute() override;
};