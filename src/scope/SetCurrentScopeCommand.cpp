#include "SetCurrentScopeCommand.h"


SetCurrentScopeCommand::SetCurrentScopeCommand(std::shared_ptr<Scope> scope) : m_scope(scope) {}


void SetCurrentScopeCommand::execute()
{
    InitCommand::currentScope = m_scope;
}