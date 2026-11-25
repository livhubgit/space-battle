#include "PutInQueueCommand.h"


PutInQueueCommand::PutInQueueCommand (std::queue<CommandPtr>& q, CommandPtr cmd) : m_queue(q), m_cmd(cmd) {}


void PutInQueueCommand::execute()
{
    m_queue.push(m_cmd);
}