// Rotate.h
#pragma once
#include "Rotable.h"

class Rotate {
public:
    explicit Rotate(Rotable& rotable);
    void execute();
private:
    Rotable& m_rotable;
};