#pragma once

#include <unordered_map>
#include <string>
#include <functional>
#include <any>
#include <vector>
#include <mutex>

#include "commands/Command.h"

using DependencyStratgey =
    std::function<std::any(std::vector<std::any>)>;

using Scope = std::unordered_map<
    std::string,
    DependencyStratgey>;

using Strategy = std::function<
    std::any(std::string, std::vector<std::any>)>;

using StrategyUpdater = std::function<
    Strategy(Strategy)>;



class InitCommand : public Command
{
public:
    inline static std::shared_ptr<Scope> rootScope = std::make_shared<Scope>();
    static thread_local std::shared_ptr<Scope> currentScope;

    inline static bool m_alreadyExecutesScuccessfully = false;
    inline static std::mutex rootScopeMutex;


    inline static std::unordered_map<std::string, std::shared_ptr<Scope>> scopeRegistry;
    inline static std::mutex registryMutex;

    void execute() override;
};