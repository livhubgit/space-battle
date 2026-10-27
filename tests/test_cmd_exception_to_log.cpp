#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sstream>
#include <string>

#include "commands/ExceptionToLogCommand.h"
#include "commands/RepeatCommand.h"

#include "ExceptionHandlers.h"
#include "ExceptionHandlerStore.h"
#include "CommandProcessor.h"

using ::testing::Return;

using ::testing::_;
using ::testing::Throw;


// Команда всегда бросающая исключение и считающая вызовы
class TestIncrementAndThrowingCommand : public Command
{
public:
    int counter = 0;
    void execute() override
    {
        counter++;
        throw std::runtime_error("test error");
    }
};

// 4.Реализовать Команду, которая записывает информацию о выброшенном исключении в лог
TEST(ExceptionsTest, ExceptionToLogCommand)
{
    // Arrange
    std::ostringstream oss;
    ExceptionToLogCommand cmd(std::runtime_error("runtime_error text"), oss);

    // Act
    cmd.execute();

    // Assert
    EXPECT_THAT(oss.str(), testing::HasSubstr("runtime_error text"));
}

// 5. Реализовать обработчик исключения, который ставит Команду, пишущую в лог, в очередь Команд.
TEST(ExceptionsTest, registerPutInQueueLogCommandHandler)
{
    // Arrange

    // Команда всегда бросающая исключение
    class TestThrowingCommand : public Command
    {
    public:
        void execute() override
        {
            throw std::runtime_error("test error");
        }
    };

    // поток для лога
    std::ostringstream oss;

    // цикл
    CommandProcessor processor;

    ExceptionHandlerStore::clear();

    registerPutInQueueLogCommandHandler(
        typeid(TestThrowingCommand),
        typeid(std::runtime_error),
        processor.queue(),
        oss);

    // Кладём в очередь команду, которая упадёт
    processor.queue().push(std::make_shared<TestThrowingCommand>());

    // Act
    // Запускаем обработку
    processor.work();

    // Assert
    EXPECT_THAT(oss.str(), testing::HasSubstr("test error"));
    EXPECT_TRUE(processor.queue().empty());
}

// 6. Реализовать Команду, которая повторяет Команду, выбросившую исключение.
TEST(ExceptionsTest, RepeatCommand)
{
    // Arrange

    class CommandMock : public Command
    {
    public:
        MOCK_METHOD(void, execute, (), (override));
    };

    auto mockCmd = std::make_shared<CommandMock>();
    RepeatCommand repeatCmd(mockCmd);

    EXPECT_CALL(*mockCmd, execute()).Times(1);


    // Act
    repeatCmd.execute();

    // Assert
}

// 7. Реализовать обработчик исключения, который ставит в очередь Команду - повторитель команды, выбросившей исключение.
TEST(ExceptionsTest, RepeatHandlerRetriesFailedCommand)
{
    // Arrange

    // цикл
    CommandProcessor processor;

    ExceptionHandlerStore::clear();

    registerRepeatHandler(
        typeid(TestIncrementAndThrowingCommand),
        typeid(std::runtime_error),
        processor.queue());

    // Кладём в очередь команду, которая упадёт
    auto cmd =  std::make_shared<TestIncrementAndThrowingCommand>();
    processor.queue().push(cmd);

    // Act
    // Запускаем обработку
    processor.work();

    // Assert
    //количество запусков равно 2 
    EXPECT_EQ(cmd->counter, 2);
    EXPECT_TRUE(processor.queue().empty());
}



//8. С помощью Команд из пункта 4 и пункта 6 реализовать следующую обработку исключений:
//при первом выбросе исключения повторить команду, при повторном выбросе исключения записать информацию в лог.
TEST(ExceptionsTest, RepeatOnceThenLogOnSecondFailure)
{
    // Arrange

    // поток для лога
    std::ostringstream oss;
    // цикл
    CommandProcessor processor;

    ExceptionHandlerStore::clear();

    registerRepeatOnceThenLog(
        typeid(TestIncrementAndThrowingCommand),
        typeid(std::runtime_error),
        processor.queue(),
        oss
    );

    // Кладём в очередь команду, которая упадёт
    auto cmd =  std::make_shared<TestIncrementAndThrowingCommand>();
    processor.queue().push(cmd);

    // Act
    // Запускаем обработку
    processor.work();

    // Assert
    EXPECT_EQ(cmd->counter, 2); //команда в итоге выполнилась дважды
    EXPECT_THAT(oss.str(), testing::HasSubstr("test error"));   // в лог вышло сообщение об ошибке
    EXPECT_TRUE(processor.queue().empty());   // цикл закончися сам и очередь пуста
}


//9.повторить два раза, потом записать в лог
TEST(ExceptionsTest, RepeatTwiceThenLogAfterThirdFailure)
{
    // Arrange


    // поток для лога
    std::ostringstream oss;
    // цикл
    CommandProcessor processor;

    ExceptionHandlerStore::clear();

    registerRepeatTwiceThenLog(
        typeid(TestIncrementAndThrowingCommand),
        typeid(std::runtime_error),
        processor.queue(),
        oss
    );

    // Кладём в очередь команду, которая упадёт
    auto cmd =  std::make_shared<TestIncrementAndThrowingCommand>();
    processor.queue().push(cmd);

    // Act
    // Запускаем обработку
    processor.work();

    // Assert
    EXPECT_EQ(cmd->counter, 3); //команда в итоге выполнилась трижды
    EXPECT_THAT(oss.str(), testing::HasSubstr("test error"));   // в лог вышло сообщение об ошибке
    EXPECT_TRUE(processor.queue().empty());   // цикл закончися сам и очередь пуста
}