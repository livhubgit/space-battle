#pragma once

class Angle {
private:
    int degrees_;

public:
    explicit Angle(int degrees = 0)
        : degrees_(((degrees % 360) + 360) % 360) {}


    int getDegrees() const {
        return degrees_;
    }

    Angle operator+(const Angle& other) const {
        return Angle(degrees_ + other.degrees_);
    }

    bool operator==(const Angle& other) const {
        return degrees_ == other.degrees_;
    }
};