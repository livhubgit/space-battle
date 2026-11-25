#pragma once
#include <string> 
#include <iostream>
#include <exception>

#include "Command.h"

//записывает текст исключения в поток (лог)
class ExceptionToLogCommand : public Command
{
public:
    ExceptionToLogCommand(const std::exception &e, std::ostream &log);
    void execute() override;

private:
    std::string m_message;
    std::ostream &m_log;
};