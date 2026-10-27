#include "ExceptionToLogCommand.h"


ExceptionToLogCommand::ExceptionToLogCommand(const std::exception &e, std::ostream &log) : m_message(e.what()), m_log(log) {}


void ExceptionToLogCommand::execute()
{
    m_log << m_message << '\n'; 
}
