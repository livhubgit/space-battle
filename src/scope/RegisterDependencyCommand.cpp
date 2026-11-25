#include "RegisterDependencyCommand.h"
#include "Ioc/Ioc.h"

RegisterDependencyCommand::RegisterDependencyCommand(std::string dependency, DependencyStratgey depednecyResolverStrategy)
    : m_dependency(dependency), m_dependencyResolverStrategy(depednecyResolverStrategy) {}

void RegisterDependencyCommand::execute()
{
    auto currentScope = Ioc::Resolve<std::shared_ptr<Scope>>("IoC.Scope.Current", std::vector<std::any>{});
    currentScope->emplace(m_dependency, m_dependencyResolverStrategy);
}