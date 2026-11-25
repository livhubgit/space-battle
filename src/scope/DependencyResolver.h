
#pragma once

#include <any>
#include <vector>
#include <string>
#include <memory>

#include "DependencyResolverInterface.h"
#include "InitCommand.h"

class DependencyResolver : public DependencyResolverInterface
{
public:
    DependencyResolver(std::shared_ptr<Scope> scope);
    std::any Resolve(std::string dependency, std::vector<std::any> args) override;

    private:
    std::shared_ptr<Scope> m_dependencies;
};