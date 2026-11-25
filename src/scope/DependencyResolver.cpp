
#include "DependencyResolver.h"


DependencyResolver::DependencyResolver(std::shared_ptr<Scope> scope) : m_dependencies(scope) {}

std::any DependencyResolver::Resolve(std::string dependency, std::vector<std::any> args)
{
    std::shared_ptr<Scope> dependencies = m_dependencies;

    while (true)
    {

        auto it = dependencies->find(dependency);
        if (it != dependencies->end())
        {
            // Элемент найден
            return it->second(args);
        }
        else
        {
            dependencies = std::any_cast<std::shared_ptr<Scope>>(dependencies->at("IoC.Scope.Parent")(args));
            // Элемент не найден
        }
    }
}