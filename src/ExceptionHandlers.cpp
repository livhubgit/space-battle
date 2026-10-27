
#include "ExceptionHandlerStore.h"
#include "commands/ExceptionToLogCommand.h"
#include "commands/PutInQueueCommand.h"
#include "commands/RepeatCommand.h"
#include "commands/RepeatTwiceCommand.h"

// обработчик: упавшая команда + исключение -> PutInQueueCommand (ExceptionToLogCommand)
void registerPutInQueueLogCommandHandler(std::type_index commandType,
                                         std::type_index exceptionType,
                                         std::queue<CommandPtr> &q,
                                         std::ostream &stream)
{

    ExceptionHandlerStore::registerHandler(
        commandType,
        exceptionType,
        [&](CommandPtr, const std::exception &e)
        {
            auto logCmd = std::make_shared<ExceptionToLogCommand>(e, stream);
            return std::make_shared<PutInQueueCommand>(q, logCmd);
        });
}

//обработчик: упавшая команда + исключение -> PutInQueueCommand(RepeatCommand)
void registerRepeatHandler(std::type_index commandType,
                           std::type_index exceptionType,
                           std::queue<CommandPtr> &q)
{
    ExceptionHandlerStore::registerHandler(
        commandType,
        exceptionType,
        [&](CommandPtr cmd, const std::exception &e)
        {
            auto repeatCmd = std::make_shared<RepeatCommand>(cmd);
            return std::make_shared<PutInQueueCommand>(q, repeatCmd);
        });
}

// Функция-стратегия - при первом выбросе исключения повторить команду, при повторном выбросе исключения записать информацию в лог.
void registerRepeatOnceThenLog(std::type_index commandType,
                               std::type_index exceptionType,
                               std::queue<CommandPtr> &q,
                               std::ostream &stream)
{
    registerRepeatHandler(commandType, exceptionType, q);
    registerPutInQueueLogCommandHandler(typeid(RepeatCommand), exceptionType, q, stream);
}

//обработчик: упавшая команда + исключение -> PutInQueueCommand(RepeatTwiceCommand)
//такой же как registerRepeatHandler но создаёт RepeatTwiceCommand вместо RepeatCommand
void registerRepeatTwiceHandler(std::type_index commandType,
                                std::type_index exceptionType,
                                std::queue<CommandPtr> &q)
{
    ExceptionHandlerStore::registerHandler(
        commandType,
        exceptionType,
        [&](CommandPtr cmd, const std::exception &)
        {
            auto repeatTwiceCmd = std::make_shared<RepeatTwiceCommand>(cmd);
            return std::make_shared<PutInQueueCommand>(q, repeatTwiceCmd);
        });
}

// Функция-стратегия - повторить два раза, потом записать в лог
void registerRepeatTwiceThenLog(std::type_index commandType,
                                std::type_index exceptionType,
                                std::queue<CommandPtr> &q,
                                std::ostream &stream)
{
    registerRepeatHandler(commandType, exceptionType, q);
    registerRepeatTwiceHandler(typeid(RepeatCommand), exceptionType, q);
    registerPutInQueueLogCommandHandler(typeid(RepeatTwiceCommand), exceptionType, q, stream);
}