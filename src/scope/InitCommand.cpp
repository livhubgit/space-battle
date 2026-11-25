#include <memory>
#include <stdexcept>

#include "InitCommand.h"
#include "commands/Command.h"
#include "SetCurrentScopeCommand.h"
#include "ClearCurrentScopeCommand.h"
#include "RegisterDependencyCommand.h"
#include "NewScopeCommand.h"
#include "SetCurrentScopeByIdCommand.h"
#include "DependencyResolver.h"
#include "Ioc/Ioc.h"

thread_local std::shared_ptr<Scope> InitCommand::currentScope = nullptr;

void InitCommand::execute()
{

    std::lock_guard<std::mutex> lock(rootScopeMutex);

    if (m_alreadyExecutesScuccessfully)
        return;

    rootScope->try_emplace(
        "IoC.Scope.Current.Set",
        [](std::vector<std::any> args) -> std::any
        {
            std::shared_ptr<Scope> scope = std::any_cast<std::shared_ptr<Scope>>(args[0]);

            CommandPtr command = std::make_shared<SetCurrentScopeCommand>(scope);
            return command;
        });

    rootScope->try_emplace(
        "IoC.Scope.Current.Clear",
        [](std::vector<std::any> args) -> std::any
        {
            CommandPtr command = std::make_shared<ClearCurrentScopeCommand>();
            return command;
        });

    rootScope->try_emplace(
        "IoC.Scope.Current",
        [](std::vector<std::any> args) -> std::any
        {
            return currentScope != nullptr ? currentScope : rootScope;
        });

    rootScope->try_emplace(
        "IoC.Scope.Parent",
        [](std::vector<std::any> args) -> std::any
        {
            throw std::logic_error("The root scope has no a parent scope.");
        });

    rootScope->try_emplace(
        "IoC.Scope.Create.Empty",
        [](std::vector<std::any> args) -> std::any
        {
            return std::make_shared<Scope>();
        });

    rootScope->try_emplace(
        "IoC.Scope.Create",
        [](std::vector<std::any> args) -> std::any
        {
            auto creatingScope = Ioc::Resolve<std::shared_ptr<Scope>>("IoC.Scope.Create.Empty", std::vector<std::any>{});

            if (args.size() > 0)
            {
                auto parentScope = std::any_cast<std::shared_ptr<Scope>>(args[0]);
                creatingScope->emplace("IoC.Scope.Parent",
                                       [parentScope](std::vector<std::any> args) -> std::any
                                       {
                                           return parentScope;
                                       });
            }
            else
            {
                auto parentScope = Ioc::Resolve<std::shared_ptr<Scope>>("IoC.Scope.Current", std::vector<std::any>{});
                creatingScope->emplace("IoC.Scope.Parent",
                                       [parentScope](std::vector<std::any> args) -> std::any
                                       {
                                           return parentScope;
                                       });
            }
            return creatingScope;
        });

    rootScope->try_emplace(
        "Scopes.New",
        [](std::vector<std::any> args) -> std::any
        {
            std::string id = std::any_cast<std::string>(args[0]);
            return CommandPtr(std::make_shared<NewScopeCommand>(id));
        });

    rootScope->try_emplace(
        "Scopes.Current",
        [](std::vector<std::any> args) -> std::any
        {
            std::string id = std::any_cast<std::string>(args[0]);
            return CommandPtr(std::make_shared<SetCurrentScopeByIdCommand>(id));
        });

    rootScope->try_emplace(
        "IoC.Register",
        [](std::vector<std::any> args) -> std::any
        {
            CommandPtr command = std::make_shared<RegisterDependencyCommand>(std::any_cast<std::string>(args[0]), std::any_cast<DependencyStratgey>(args[1]));
            return command;
        });

    Ioc::Resolve<CommandPtr>(
        "Update Ioc Resolve Dependency Strategy",
        {StrategyUpdater(
            [](Strategy oldStrategy) -> Strategy
            {
                return [](std::string dependency,
                          std::vector<std::any> args) -> std::any
                {
                    auto scope = currentScope != nullptr ? currentScope : rootScope;
                    DependencyResolver dependencyResolver(scope);

                    return dependencyResolver.Resolve(dependency, args);
                };
            })})
        ->execute();

    m_alreadyExecutesScuccessfully = true;
};