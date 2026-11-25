#pragma once

#include "commands/Command.h"

#include "InitCommand.h"

class RegisterDependencyCommand : public Command
{
public:
    RegisterDependencyCommand(std::string dependency, DependencyStratgey depednecyResolverStrategy);
    void execute() override;

private:
    std::string m_dependency;
    DependencyStratgey m_dependencyResolverStrategy;
};