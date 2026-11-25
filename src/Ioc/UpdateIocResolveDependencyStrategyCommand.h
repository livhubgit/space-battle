
#pragma once

#include <functional>
#include <vector>
#include <any>
#include <string>

#include "commands/Command.h"

class UpdateIocResolveDependencyStrategyCommand : public Command
{
private:
    std::function<
        std::function<std::any(std::string, std::vector<std::any>)>(
            std::function<std::any(std::string, std::vector<std::any>)>)>
        m_updateIoCStrategy;

public:
    UpdateIocResolveDependencyStrategyCommand(std::function<
                                              std::function<std::any(std::string, std::vector<std::any>)>(
                                                  std::function<std::any(std::string, std::vector<std::any>)>)>
                                                  updater);

    void execute() override;
};