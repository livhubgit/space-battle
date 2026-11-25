#pragma once
#include <string>
#include "commands/Command.h"


class SetCurrentScopeByIdCommand : public Command {
    std::string m_id;
public:
    explicit SetCurrentScopeByIdCommand(std::string id);
    void execute() override;
};