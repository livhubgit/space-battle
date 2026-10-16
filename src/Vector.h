#pragma once

struct Vector
{
    int x;
    int y;

    Vector operator+(const Vector &other) const
    {
        return {x + other.x, y + other.y};
    }

    Vector moveTo(const Vector &other) const
    {
        return {x + other.x, y + other.y};
    }

    bool operator==(const Vector &other) const
    {
        return x == other.x && y == other.y;
    }
};
