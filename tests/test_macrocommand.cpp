#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sstream>
#include <string>
#include <memory>

#include "commands/CommandException.h"
#include "commands/CheckFuelCommand.h"
#include "commands/BurnFuelCommand.h"

#include "commands/Command.h"
#include "commands/MacroCommand.h"

#include "Movable.h"
#include "commands/LineMoveCommand.h"

#include "commands/ChangeVelocityCommand.h"
#include "commands/RotateWithVelocityCommand.h"

using ::testing::Return;

using ::testing::_;
using ::testing::Throw;

class FuelConsumableMock : public FuelConsumable
{
public:
    MOCK_METHOD(int, getFuel, (), (const, override));
    MOCK_METHOD(int, getFuelBurnRate, (), (const, override));
    MOCK_METHOD(void, setFuel, (int newFuel), (override));
};

class CommandMock : public Command
{
public:
    MOCK_METHOD(void, execute, (), (override));
};

class MovableMock : public Movable
{
public:
    MOCK_METHOD(Vector, getPosition, (), (const, override));
    MOCK_METHOD(Vector, getVelocity, (), (const, override));
    MOCK_METHOD(void, setPosition, (Vector newPosition), (override));
};

class VelocityChangeableMock : public VelocityChangeable
{
public:
    MOCK_METHOD(bool, hasVelocity, (), (const, override));
    MOCK_METHOD(Vector, getVelocity, (), (const, override));
    MOCK_METHOD(void, setVelocity, (Vector newVelocity), (override));
};

class RotableMock : public Rotable
{
public:
    MOCK_METHOD(Angle, getAngle, (), (const, override));
    MOCK_METHOD(Angle, getAngularVelocity, (), (const, override));
    MOCK_METHOD(void, setAngle, (Angle newAngle), (override));
};

// 1. Реализовать класс CheckFuelCommand и тесты к нему.
TEST(CheckFuelCommandTest, ThrowsWhenNotEnoughFuel)
{
    // Arrange: условия
    FuelConsumableMock fuelconsumable;
    ON_CALL(fuelconsumable, getFuel()).WillByDefault(Return(1000));
    ON_CALL(fuelconsumable, getFuelBurnRate()).WillByDefault(Return(3000));

    CheckFuelCommand checkfuelcommand(fuelconsumable);

    // Act + Assert
    EXPECT_THROW(checkfuelcommand.execute(), CommandException);
}

// 1. Реализовать класс CheckFuelCommand и тесты к нему.
TEST(CheckFuelCommandTest, DoesNotThrowWhenEnoughFuel)
{
    // Arrange: условия
    FuelConsumableMock fuelconsumable;
    ON_CALL(fuelconsumable, getFuel()).WillByDefault(Return(1000));
    ON_CALL(fuelconsumable, getFuelBurnRate()).WillByDefault(Return(300));

    CheckFuelCommand checkfuelcommand(fuelconsumable);

    // Act + Assert
    EXPECT_NO_THROW(checkfuelcommand.execute());
}

// 1. Реализовать класс CheckFuelCommand и тесты к нему.
TEST(CheckFuelCommandTest, DoesNotThrowWhenFuelEqualsBurnRate)
{
    // Arrange: условия
    FuelConsumableMock fuelconsumable;
    ON_CALL(fuelconsumable, getFuel()).WillByDefault(Return(1000));
    ON_CALL(fuelconsumable, getFuelBurnRate()).WillByDefault(Return(1000));

    CheckFuelCommand checkfuelcommand(fuelconsumable);

    // Act + Assert
    EXPECT_NO_THROW(checkfuelcommand.execute());
}

// 2.Реализовать класс BurnFuelCommand и тесты к нему.
TEST(BurnFuelCommandTest, BurnsFuelByBurnRate)
{
    // Arrange: условия
    FuelConsumableMock fuelconsumable;
    ON_CALL(fuelconsumable, getFuel()).WillByDefault(Return(1000));
    ON_CALL(fuelconsumable, getFuelBurnRate()).WillByDefault(Return(900));

    // Arrange: что проверяем
    EXPECT_CALL(fuelconsumable, setFuel(100));

    BurnFuelCommand burnfuelcommand(fuelconsumable);

    // Act
    burnfuelcommand.execute();
}

// 3. Реализовать простейшую макрокоманду и тесты к ней. Здесь простейшая - это значит, что при выбросе исключения вся последовательность команд приостанавливает свое выполнение, а макрокоманда выбрасывает CommandException.
TEST(MacroCommandTest, AllCommandsExecuteInOrder)
{
    // Arrange
    auto command1 = std::make_shared<CommandMock>();
    auto command2 = std::make_shared<CommandMock>();
    auto command3 = std::make_shared<CommandMock>();

    MacroCommand macrocommand({command1, command2, command3});

    ::testing::InSequence seq;
    EXPECT_CALL(*command1, execute());
    EXPECT_CALL(*command2, execute());
    EXPECT_CALL(*command3, execute());

    // Act + Assert
    EXPECT_NO_THROW(macrocommand.execute());
}

TEST(MacroCommandTest, Commands2STDThrowThenStopAndMacrocommandThrow)
{
    // Arrange
    auto command1 = std::make_shared<CommandMock>();
    auto command2 = std::make_shared<CommandMock>();
    auto command3 = std::make_shared<CommandMock>();

    MacroCommand macrocommand({command1, command2, command3});

    ::testing::InSequence seq;
    EXPECT_CALL(*command1, execute());
    EXPECT_CALL(*command2, execute()).WillOnce(Throw(std::runtime_error("runtime_error")));
    EXPECT_CALL(*command3, execute()).Times(0);

    // Act + Assert
    EXPECT_THROW(macrocommand.execute(), CommandException);
}

TEST(MacroCommandTest, Commands2CommandExceptionThrowThenStopAndMacrocommandThrow)
{
    // Arrange
    auto command1 = std::make_shared<CommandMock>();
    auto command2 = std::make_shared<CommandMock>();
    auto command3 = std::make_shared<CommandMock>();

    MacroCommand macrocommand({command1, command2, command3});

    ::testing::InSequence seq;
    EXPECT_CALL(*command1, execute());
    EXPECT_CALL(*command2, execute()).WillOnce(Throw(CommandException("CommandException error")));
    EXPECT_CALL(*command3, execute()).Times(0);

    // Act + Assert
    EXPECT_THROW(macrocommand.execute(), CommandException);
}

TEST(MacroCommandTest, Commands2NoSTDThrowThenStopAndMacrocommandThrow)
{
    // Arrange
    auto command1 = std::make_shared<CommandMock>();
    auto command2 = std::make_shared<CommandMock>();
    auto command3 = std::make_shared<CommandMock>();

    MacroCommand macrocommand({command1, command2, command3});

    ::testing::InSequence seq;
    EXPECT_CALL(*command1, execute());
    EXPECT_CALL(*command2, execute()).WillOnce(Throw(42));
    EXPECT_CALL(*command3, execute()).Times(0);

    // Act + Assert
    EXPECT_THROW(macrocommand.execute(), CommandException);
}

// 4.Реализовать команду движения по прямой с расходом топлива, используя команды с предыдущих шагов.
TEST(LineMoveCommandTest, MovesAndBurnsFuelWhenEnoughFuel)
{
    // Arrange: условия
    FuelConsumableMock fuelconsumable;
    ON_CALL(fuelconsumable, getFuel()).WillByDefault(Return(1000));
    ON_CALL(fuelconsumable, getFuelBurnRate()).WillByDefault(Return(900));

    MovableMock movable;
    ON_CALL(movable, getPosition()).WillByDefault(Return(Vector{12, 5}));
    ON_CALL(movable, getVelocity()).WillByDefault(Return(Vector{-7, 3}));

    // Arrange: что проверяем
    EXPECT_CALL(movable, setPosition(Vector{5, 8}));
    EXPECT_CALL(fuelconsumable, setFuel(100));

    LineMoveCommand linemovecommand(fuelconsumable, movable);

    // Act
    linemovecommand.execute();
}

TEST(LineMoveCommandTest, NoFuelNoMovel)
{
    // Arrange: условия
    FuelConsumableMock fuelconsumable;
    ON_CALL(fuelconsumable, getFuel()).WillByDefault(Return(800));
    ON_CALL(fuelconsumable, getFuelBurnRate()).WillByDefault(Return(900));

    MovableMock movable;
    ON_CALL(movable, getPosition()).WillByDefault(Return(Vector{12, 5}));
    ON_CALL(movable, getVelocity()).WillByDefault(Return(Vector{-7, 3}));

    // Arrange: что проверяем
    EXPECT_CALL(movable, setPosition(_)).Times(0);
    EXPECT_CALL(fuelconsumable, setFuel(_)).Times(0);

    LineMoveCommand linemovecommand(fuelconsumable, movable);

    // Act +Assert
    EXPECT_THROW(linemovecommand.execute(), CommandException);
}

// 5.Реализовать команду для модификации вектора мгновенной скорости при повороте. Необходимо учесть, что не каждый разворачивающийся объект движется.
TEST(ChangeVelocityCommandTest, HasVelocityChangeVelocity)
{
    // Arrange: условия
    VelocityChangeableMock velocitychangeable;
    RotableMock rotable;
    ON_CALL(velocitychangeable, hasVelocity()).WillByDefault(Return(true));
    ON_CALL(velocitychangeable, getVelocity()).WillByDefault(Return(Vector{10, 0}));
    ON_CALL(rotable, getAngularVelocity()).WillByDefault(Return(Angle{90}));

    EXPECT_CALL(velocitychangeable, setVelocity(Vector{0, 10}));

    ChangeVelocityCommand changevelocitycommand(rotable, velocitychangeable);

    // Act
    changevelocitycommand.execute();
}

TEST(ChangeVelocityCommandTest, NoVelocityNoChangeVelocity)
{
    // Arrange: условия
    VelocityChangeableMock velocitychangeable;
    RotableMock rotable;
    ON_CALL(velocitychangeable, hasVelocity()).WillByDefault(Return(false));

    EXPECT_CALL(velocitychangeable, getVelocity()).Times(0);
    EXPECT_CALL(velocitychangeable, setVelocity(_)).Times(0);

    ChangeVelocityCommand changevelocitycommand(rotable, velocitychangeable);

    // Act
    changevelocitycommand.execute();
}

TEST(ChangeVelocityCommandTest, GetVelocityThrowThanChangeVelocityCommandThrow)
{
    // Arrange: условия
    VelocityChangeableMock velocitychangeable;
    RotableMock rotable;
    ON_CALL(velocitychangeable, hasVelocity()).WillByDefault(Return(true));

    EXPECT_CALL(velocitychangeable, getVelocity()).WillOnce(Throw(CommandException("error")));

    ChangeVelocityCommand changevelocitycommand(rotable, velocitychangeable);

    // Act + Assert.
    EXPECT_THROW(changevelocitycommand.execute(), CommandException);
}


// 6.Реализовать команду поворота, которая еще и меняет вектор мгновенной скорости, если есть.
TEST(RotateWithVelocityCommandTest, RotateAndVelocity)
{
    // Arrange: условия 
    RotableMock rotable;
    ON_CALL(rotable, getAngle()).WillByDefault(Return(Angle{90}));
    ON_CALL(rotable, getAngularVelocity()).WillByDefault(Return(Angle{90}));

    VelocityChangeableMock velocitychangeable;
    ON_CALL(velocitychangeable, hasVelocity()).WillByDefault(Return(true));
    ON_CALL(velocitychangeable, getVelocity()).WillByDefault(Return(Vector{10, 0}));


    // Arrange: что проверяем
    EXPECT_CALL(rotable, setAngle(Angle{180}));
    EXPECT_CALL(velocitychangeable, setVelocity(Vector{0, 10}));

    RotateWithVelocityCommand rotatewithvelocitycommand(rotable, velocitychangeable);

    // Act
    rotatewithvelocitycommand.execute();
}

TEST(RotateWithVelocityCommandTest, RotatesObjectAndSkipsVelocityWhenObjectHasNoVelocity)
{
    // Arrange: условия 
    RotableMock rotable;
    ON_CALL(rotable, getAngle()).WillByDefault(Return(Angle{90}));
    ON_CALL(rotable, getAngularVelocity()).WillByDefault(Return(Angle{90}));

    VelocityChangeableMock velocitychangeable;
    ON_CALL(velocitychangeable, hasVelocity()).WillByDefault(Return(false));


    // Arrange: что проверяем
    EXPECT_CALL(rotable, setAngle(Angle{180}));
    EXPECT_CALL(velocitychangeable, getVelocity()).Times(0);
    EXPECT_CALL(velocitychangeable, setVelocity(_)).Times(0);


    RotateWithVelocityCommand rotatewithvelocitycommand(rotable, velocitychangeable);

    // Act + Assert
    EXPECT_NO_THROW(rotatewithvelocitycommand.execute());
}

TEST(RotateWithVelocityCommandTest, StopsAndThrowsCommandExceptionWhenRotationFails)
{
    // Arrange: условия 
    RotableMock rotable;
    ON_CALL(rotable, getAngle()).WillByDefault(Return(Angle{90}));
    ON_CALL(rotable, getAngularVelocity()).WillByDefault(Return(Angle{90}));

    VelocityChangeableMock velocitychangeable;
    ON_CALL(velocitychangeable, hasVelocity()).WillByDefault(Return(true));


    // Arrange: что проверяем
    EXPECT_CALL(rotable, setAngle(_)).WillOnce(Throw(CommandException("test error")));
    EXPECT_CALL(velocitychangeable, getVelocity()).Times(0);
    EXPECT_CALL(velocitychangeable, setVelocity(_)).Times(0);


    RotateWithVelocityCommand rotatewithvelocitycommand(rotable, velocitychangeable);

    // Act + Assert
    EXPECT_THROW(rotatewithvelocitycommand.execute(), CommandException);
}