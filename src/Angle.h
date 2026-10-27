#pragma once

class Angle {
private:
    int m_degrees;

public:
    explicit Angle(int degrees = 0)
        : m_degrees(((degrees % 360) + 360) % 360) {}


    int getDegrees() const {
        return m_degrees;
    }

    Angle operator+(const Angle& other) const {
        return Angle(m_degrees + other.m_degrees);
    }

    bool operator==(const Angle& other) const {
        return m_degrees == other.m_degrees;
    }
};