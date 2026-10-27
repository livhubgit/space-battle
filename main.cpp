#include <iostream>
#include <stdexcept>
#include <typeindex>
#include <memory>

#include "CommandProcessor.h"
#include "ExceptionHandlerStore.h"
#include "commands/Command.h"
#include "commands/ExceptionToLogCommand.h"
#include "commands/PutInQueueCommand.h"
#include "commands/RepeatCommand.h"

// Команда для пробного запуска: всегда бросает исключение
class TestThrowingCommand : public Command
{
public:
    void execute() override
    {
        throw std::runtime_error("test error");
    }
};

void registerPutInQueueHandler(std::type_index commandType,
                           std::type_index exceptionType,
                           std::queue<CommandPtr> &q)
{
    // Регистрируем лямбду-обработчик на пару «TestThrowingCommand бросила std::runtime_error».
    // Когда это случится, вызовется лямбда, которая  создаёт ExceptionToLogCommand  и PutInQueueCommand,
    // в которую вложен логгер, и возвращает PutInQueueCommand.
    // Дальше work() выполняет PutInQueueCommand, и та ставит логгер в очередь.
    // логгер выполнится на следующем обороте цикла и запишет текст исключения в лог.
    ExceptionHandlerStore::registerHandler(
        commandType,
        exceptionType,
        [&](CommandPtr, const std::exception &e)
        {
            auto logCmd = std::make_shared<ExceptionToLogCommand>(e, std::cout);
            return std::make_shared<PutInQueueCommand>(q, logCmd);
        });
}

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

int main()
{
    CommandProcessor processor;

    // Регистрируем лямбду-обработчик на пару «TestThrowingCommand бросила std::runtime_error».
    // Когда это случится, вызовется лямбда, которая  создаёт ExceptionToLogCommand  и PutInQueueCommand,
    // в которую вложен логгер, и возвращает PutInQueueCommand.
    // Дальше work() выполняет PutInQueueCommand, и та ставит логгер в очередь.
    // логгер выполнится на следующем обороте цикла и запишет текст исключения в лог.
    ExceptionHandlerStore::registerHandler(
        typeid(TestThrowingCommand),
        typeid(std::runtime_error),
        [&](CommandPtr, const std::exception &e)
        {
            auto logCmd = std::make_shared<ExceptionToLogCommand>(e, std::cout);
            return std::make_shared<PutInQueueCommand>(processor.queue(), logCmd);
        });


    ExceptionHandlerStore::registerHandler(
        typeid(TestThrowingCommand),
        typeid(std::runtime_error),
        [&](CommandPtr cmd, const std::exception &e)
        {
            auto repeatCmd = std::make_shared<RepeatCommand>(cmd);
            return std::make_shared<PutInQueueCommand>(processor.queue(), repeatCmd);
        });

    // 2. Кладём в очередь команду, которая упадёт
    processor.queue().push(std::make_shared<TestThrowingCommand>());

    // 3. Запускаем обработку
    processor.work();

    return 0;
}