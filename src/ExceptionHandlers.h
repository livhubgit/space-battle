#pragma once

#include <stdexcept>
#include <typeindex>
#include <memory>
#include <queue>
#include <ostream>

#include "commands/Command.h"


//обработчик: упавшая команда + исключение -> PutInQueueCommand (ExceptionToLogCommand)
void registerPutInQueueLogCommandHandler(
    std::type_index commandType,
    std::type_index exceptionType,
    std::queue<CommandPtr> &q,
    std::ostream &stream);

//обработчик: упавшая команда + исключение -> PutInQueueCommand(RepeatCommand)
void registerRepeatHandler(
    std::type_index commandType,
    std::type_index exceptionType,
    std::queue<CommandPtr> &q);

//Функция-стратегия - при первом выбросе исключения повторить команду, при повторном выбросе исключения записать информацию в лог.
void registerRepeatOnceThenLog(std::type_index commandType,
                               std::type_index exceptionType,
                               std::queue<CommandPtr> &q,
                               std::ostream &stream);


//обработчик: упавшая команда + исключение -> PutInQueueCommand(RepeatTwiceCommand)
//такой же как registerRepeatHandler но создаёт RepeatTwiceCommand вместо RepeatCommand
void registerRepeatTwiceHandler(
    std::type_index commandType,
    std::type_index exceptionType,
    std::queue<CommandPtr> &q);


//Функция-стратегия - повторить два раза, потом записать в лог
void registerRepeatTwiceThenLog(std::type_index commandType,
                               std::type_index exceptionType,
                               std::queue<CommandPtr> &q,
                               std::ostream &stream);