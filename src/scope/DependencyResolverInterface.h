
#pragma once

#include <any>
#include <vector>
#include <string>

class DependencyResolverInterface
{
public:    
    virtual std::any Resolve(std::string dependency, std::vector<std::any> args) = 0;
};