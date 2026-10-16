// Move.cpp
#include "Move.h"

Move::Move(Movable& movable) : m_movable(movable) {}

void Move::execute() {
    m_movable.setPosition(m_movable.getPosition().moveTo(m_movable.getVelocity()));
}