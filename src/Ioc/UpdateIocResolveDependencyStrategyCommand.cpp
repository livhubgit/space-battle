
#include "Ioc.h"
#include "UpdateIocResolveDependencyStrategyCommand.h"

UpdateIocResolveDependencyStrategyCommand::UpdateIocResolveDependencyStrategyCommand(
    std::function<
        std::function<std::any(std::string, std::vector<std::any>)>(
            std::function<std::any(std::string, std::vector<std::any>)>)>
        updater)
    : m_updateIoCStrategy(std::move(updater))
{
}

void UpdateIocResolveDependencyStrategyCommand::execute()
{
    Ioc::_strategy = m_updateIoCStrategy(Ioc::_strategy);
};
