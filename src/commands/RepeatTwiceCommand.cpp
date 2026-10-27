#include "RepeatTwiceCommand.h"


RepeatTwiceCommand::RepeatTwiceCommand (CommandPtr cmd) : m_cmd(cmd) {}


void RepeatTwiceCommand::execute()
{
    m_cmd->execute();
}