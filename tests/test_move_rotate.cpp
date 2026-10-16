#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <stdexcept>

#include "Move.h"
#include "Rotate.h"

using ::testing::Return;

using ::testing::Throw;
using ::testing::_;



class MovableMock : public Movable {
public:
    MOCK_METHOD(Vector, getPosition, (), (const, override));
    MOCK_METHOD(Vector, getVelocity, (), (const, override));
    MOCK_METHOD(void, setPosition, (Vector newPosition), (override));    
};


class RotableMock : public Rotable {
public:
    MOCK_METHOD(Angle, getAngle, (), (const, override));
    MOCK_METHOD(Angle, getAngularVelocity, (), (const, override));
    MOCK_METHOD(void, setAngle, (Angle newAngle), (override));
};

//Для объекта, находящегося в точке (12, 5) и движущегося со скоростью (-7, 3) движение меняет положение объекта на (5, 8)
TEST(MoveTest, MoveChangesPositionByVelocity) {
    // Arrange: условия
    MovableMock movable;    
    ON_CALL(movable, getPosition()).WillByDefault(Return(Vector{12, 5}));
    ON_CALL(movable, getVelocity()).WillByDefault(Return(Vector{-7, 3}));

    // Arrange: что проверяем
    EXPECT_CALL(movable, setPosition(Vector{5, 8}));

    Move move(movable);

    // Act
    move.execute();
}

//Попытка сдвинуть объект, у которого невозможно прочитать положение в пространстве, приводит к ошибке
TEST(MoveTest, ThrowsWhenPositionCannotBeRead) {
    // Arrange: условия
    MovableMock movable;    
    EXPECT_CALL(movable, getPosition()).WillOnce(Throw(std::runtime_error("cannot read position")));
    EXPECT_CALL(movable, setPosition(_)).Times(0);  // setPosition ни разу не вызвалась

    Move move(movable);

    // Act + Assert
    EXPECT_THROW(move.execute(), std::runtime_error);
}

//Попытка сдвинуть объект, у которого невозможно прочитать значение мгновенной скорости, приводит к ошибке
TEST(MoveTest, ThrowsWhenVelocityCannotBeRead) {
    // Arrange: условия
    MovableMock movable;
    EXPECT_CALL(movable, getVelocity()).WillOnce(Throw(std::runtime_error("cannot read velocity")));
    EXPECT_CALL(movable, setPosition(_)).Times(0);  // setPosition ни разу не вызвалась

    Move move(movable);

    // Act + Assert
    EXPECT_THROW(move.execute(), std::runtime_error);
}


// Попытка сдвинуть объект, у которого невозможно изменить положение в пространстве, приводит к ошибке
TEST(MoveTest, ThrowsWhenPositionCannotBeSet) {
    // Arrange
    MovableMock movable;
    EXPECT_CALL(movable, setPosition(_)).WillOnce(Throw(std::runtime_error("cannot set position")));

    Move move(movable);

    // Act + Assert
    EXPECT_THROW(move.execute(), std::runtime_error);
}



//поворот --------------------------------------------------------------------------------------------


// Поворот по часовой стрелке без перехода через границу
TEST(RotateTest, RotatesClockwise) {
    // Arrange
    RotableMock rotable;
    ON_CALL(rotable, getAngle()).WillByDefault(Return(Angle{90}));
    ON_CALL(rotable, getAngularVelocity()).WillByDefault(Return(Angle{45}));
    EXPECT_CALL(rotable, setAngle(Angle{135}));

    Rotate rotate(rotable);

    // Act
    rotate.execute();
}

// Переход через границу полного оборота
TEST(RotateTest, RotationWrapsAroundFullCircle) {
    // Arrange
    RotableMock rotable;
    ON_CALL(rotable, getAngle()).WillByDefault(Return(Angle{350}));
    ON_CALL(rotable, getAngularVelocity()).WillByDefault(Return(Angle{20}));
    EXPECT_CALL(rotable, setAngle(Angle{10}));

    Rotate rotate(rotable);

    // Act
    rotate.execute();
}


// Поворот на отрицательную угловую скорость (против часовой стрелки)
TEST(RotateTest, setAngleByAngleAndAngularVelocity) {
    // Arrange
    RotableMock rotable;    
    ON_CALL(rotable, getAngle()).WillByDefault(Return(Angle{10}));
    ON_CALL(rotable, getAngularVelocity()).WillByDefault(Return(Angle{-30}));    
    EXPECT_CALL(rotable, setAngle(Angle{340}));

    Rotate rotate(rotable);

    // Act
    rotate.execute();
}


//Попытка повернуть объект, у которого невозможно прочитать угол, приводит к ошибке
TEST(RotateTest, ThrowsWhenAngleCannotBeRead) {
    // Arrange: условия
    RotableMock rotable;    
    EXPECT_CALL(rotable, getAngle()).WillOnce(Throw(std::runtime_error("cannot read angle")));
    EXPECT_CALL(rotable, setAngle(_)).Times(0);  // setAngle ни разу не вызвалась

    Rotate rotate(rotable);

    // Act + Assert
    EXPECT_THROW(rotate.execute(), std::runtime_error);
}

//Попытка повернуть объект, у которого невозможно прочитать значение угловой скорости, приводит к ошибке
TEST(RotateTest, ThrowsWhenAngularVelocityCannotBeRead) {
    // Arrange: условия
    RotableMock rotable;
    EXPECT_CALL(rotable, getAngularVelocity()).WillOnce(Throw(std::runtime_error("cannot read AngularVelocity")));
    EXPECT_CALL(rotable, setAngle(_)).Times(0);  // setAngle ни разу не вызвалась

    Rotate rotate(rotable);

    // Act + Assert
    EXPECT_THROW(rotate.execute(), std::runtime_error);
}


// Попытка повернуть объект, у которого невозможно изменить угол в пространстве, приводит к ошибке
TEST(RotateTest, ThrowsWhenAngleCannotBeSet) {
    // Arrange
    RotableMock rotable;
    EXPECT_CALL(rotable, setAngle(_)).WillOnce(Throw(std::runtime_error("cannot set angle")));

    Rotate rotate(rotable);

    // Act + Assert
    EXPECT_THROW(rotate.execute(), std::runtime_error);
}


