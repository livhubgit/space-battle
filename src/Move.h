// Move.h
#pragma once
#include "Movable.h"
#include "commands/Command.h"

class Move : public Command {
public:
    explicit Move(Movable& movable);
    void execute() override;
private:
    Movable& m_movable;
};