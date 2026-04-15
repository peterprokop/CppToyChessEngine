#pragma once

#include "PieceType.hpp"

#include <iostream>

struct Coordinate {
    friend std::ostream& operator<<(std::ostream&, const Coordinate&);
    friend bool operator==(const Coordinate&, const Coordinate&);
    friend Coordinate operator*(const Coordinate&, const int);
    
    int16_t file;
    int16_t rank;
};


std::ostream& operator<<(std::ostream &outs, const Coordinate &coordinate);
bool operator==(const Coordinate& left, const Coordinate& right);

Coordinate operator*(const Coordinate& coordinate, const int multiplier);
Coordinate operator+(const Coordinate& x, const Coordinate& y);
