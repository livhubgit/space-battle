// ChangeVelocityCommand.cpp

#include <cmath>

#include "ChangeVelocityCommand.h"

ChangeVelocityCommand::ChangeVelocityCommand(Rotable &rotable, VelocityChangeable &velocitychangeable) : m_velocitychangeable(velocitychangeable), m_rotable(rotable) {}
                
void ChangeVelocityCommand::execute()
{
    if (m_velocitychangeable.hasVelocity()) {
        Vector v = m_velocitychangeable.getVelocity();
        double rad = m_rotable.getAngularVelocity().getDegrees() * PI / 180.0;

        int newX = static_cast<int>(std::lround(v.x * std::cos(rad) - v.y * std::sin(rad)));
        int newY = static_cast<int>(std::lround(v.x * std::sin(rad) + v.y * std::cos(rad)));

        m_velocitychangeable.setVelocity(Vector{newX, newY});
    }
}