// ChangeVelocityCommand.h
//поворачивает вектор мгновенной скорости на угол поворота объекта, если у объекта есть скорост
#pragma once

#include "Command.h"
#include "VelocityChangeable.h"
#include "Rotable.h"

class ChangeVelocityCommand : public Command
{
    static constexpr double PI = 3.14159265358979323846;
public:
    ChangeVelocityCommand(Rotable &rotable, VelocityChangeable &velocitychangeable);

    void execute() override;

private:
    VelocityChangeable &m_velocitychangeable;
    Rotable &m_rotable;
};