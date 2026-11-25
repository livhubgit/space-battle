#include "SetCurrentScopeByIdCommand.h"
#include "InitCommand.h"
#include "Ioc/Ioc.h"
#include <mutex>

SetCurrentScopeByIdCommand::SetCurrentScopeByIdCommand(std::string id) : m_id(std::move(id)) {}

void SetCurrentScopeByIdCommand::execute()
{
    std::lock_guard<std::mutex> lock(InitCommand::registryMutex);

    auto it = InitCommand::scopeRegistry.find(m_id);
    if (it == InitCommand::scopeRegistry.end())
    {
        throw std::invalid_argument("Scope '" + m_id + "' not found");
    }
    InitCommand::currentScope = it->second;
}