#pragma once

#include "commands/Command.h"

class ClearCurrentScopeCommand   : public Command
{
public:
    void execute() override;
};