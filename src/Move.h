// Move.h
#pragma once
#include "Movable.h"

class Move {
public:
    explicit Move(Movable& movable);
    void execute();
private:
    Movable& m_movable;
};