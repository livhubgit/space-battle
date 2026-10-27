// Rotate.cpp
#include "Rotate.h"

Rotate::Rotate(Rotable& rotable) : m_rotable(rotable) {}

void Rotate::execute() {
    m_rotable.setAngle(m_rotable.getAngle() + m_rotable.getAngularVelocity());
}