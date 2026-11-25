#include "CommandProcessor.h"
#include "ExceptionHandlerStore.h"

void CommandProcessor::work()
{
    //Пока в очереди есть команды, берём первую, убираем её из очереди и выполняем. 
    //Если команда бросила исключение, catch его ловит и просит ExceptionHandler подобрать обработчик 
    //по паре «тип команды, тип исключения». 
    //Обработчик возвращает новую команду (например, «положить логгер в очередь»), 
    //и она тут же выполняется. Если обработчика нет, исключение просто проглатывается, и цикл идёт дальше.
    while (!m_commandsQueue.empty())
    {
        auto cmd = m_commandsQueue.front();
        m_commandsQueue.pop();
        try
        {
            cmd->execute();
        }
        catch (const std::exception &e)
        {
            CommandPtr ec = ExceptionHandlerStore::handle(cmd, e);
            if (ec)
                ec->execute();
        }
    }
}
