// MacroCommand.cpp
#include "MacroCommand.h"
#include "CommandException.h"

MacroCommand::MacroCommand(std::vector<CommandPtr> comArray) : m_comArray(std::move(comArray)) {}

void MacroCommand::execute()
{
    for (const auto &cmd : m_comArray)
    {
        try
        {
            cmd->execute();
        }
        catch (const std::exception &e)
        {
            throw CommandException(std::string("Macrocommand execution failed, ") + e.what());
        }
        catch (...)
        {
            throw CommandException("Macrocommand execution failed: unknown exception");
        }
    }
}