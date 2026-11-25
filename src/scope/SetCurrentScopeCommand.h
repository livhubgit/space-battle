#pragma once



#include "commands/Command.h"

#include "InitCommand.h"


class SetCurrentScopeCommand  : public Command
{
public:
    SetCurrentScopeCommand (std::shared_ptr<Scope> scope);
    void execute() override;

private:
    std::shared_ptr<Scope> m_scope;
};