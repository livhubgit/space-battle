#include "NewScopeCommand.h"
#include "InitCommand.h"
#include "Ioc/Ioc.h"
#include <mutex>

NewScopeCommand::NewScopeCommand(std::string id) : m_id(std::move(id)) {}

void NewScopeCommand::execute()
{
    std::lock_guard<std::mutex> lock(InitCommand::registryMutex);
    if (InitCommand::scopeRegistry.count(m_id))
        throw std::invalid_argument("Scope '" + m_id + "' already exists");

    auto scope = Ioc::Resolve<std::shared_ptr<Scope>>("IoC.Scope.Create", std::vector<std::any>{InitCommand::rootScope});
    InitCommand::scopeRegistry.emplace(m_id, scope);
}