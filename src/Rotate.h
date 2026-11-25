// Rotate.h
#pragma once
#include "Rotable.h"
#include "commands/Command.h"

class Rotate : public Command {
public:
    explicit Rotate(Rotable& rotable);
    void execute();
private:
    Rotable& m_rotable;
};