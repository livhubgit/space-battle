#include "ClearCurrentScopeCommand.h"
#include "InitCommand.h"

void ClearCurrentScopeCommand ::execute()
{
    InitCommand::currentScope = nullptr;
}