#include "RepeatCommand.h"


RepeatCommand::RepeatCommand (CommandPtr cmd) : m_cmd(cmd) {}


void RepeatCommand::execute()
{
    m_cmd->execute();
}