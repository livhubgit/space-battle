#include "Ioc.h"

#include <stdexcept>
#include <memory>

#include "UpdateIocResolveDependencyStrategyCommand.h"

std::function<std::any(std::string, std::vector<std::any>)> Ioc::_strategy =
    [](std::string dependency, std::vector<std::any> args) -> std::any
{
    if ("Update Ioc Resolve Dependency Strategy" == dependency)
    {
        if (args.empty())
        {
            throw std::invalid_argument("Updater argument is required.");
        }
        CommandPtr cmd = std::make_shared<UpdateIocResolveDependencyStrategyCommand>(
            std::any_cast<
                std::function<
                    std::function<std::any(std::string, std::vector<std::any>)>(
                        std::function<std::any(std::string, std::vector<std::any>)>)>>(args[0]));
        return cmd;
    }

    throw std::invalid_argument("Dependency " + dependency + " is not found.");
};