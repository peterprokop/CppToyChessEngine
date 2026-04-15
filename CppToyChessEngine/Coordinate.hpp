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


std::ostream & operator << (std::ostream &outs, const Coordinate &coordinate) {
    return outs << (char)('a' + coordinate.file) << (coordinate.rank + 1);
}

bool operator==(const Coordinate& left, const Coordinate& right) {
    return left.file == right.file
        && left.rank == right.rank;
}

Coordinate operator*(const Coordinate& coordinate, const int multiplier) {
    return Coordinate{
        static_cast<int16_t>(coordinate.file * multiplier),
        static_cast<int16_t>(coordinate.rank * multiplier)
    };
}

Coordinate operator +(const Coordinate& x, const Coordinate& y) {
    return {
        static_cast<int16_t>(x.file + y.file),
        static_cast<int16_t>(x.rank + y.rank)
    };
}

