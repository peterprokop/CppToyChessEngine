#pragma once

#include "PieceType.h"

#include <variant>
#include <type_traits>
#include <iostream>

struct Coordinate {
    friend std::ostream& operator<<(std::ostream&, const Coordinate&);
    friend bool operator==(const Coordinate&, const Coordinate&);
    friend Coordinate operator*(const Coordinate&, const int);
    
    int16_t file;
    int16_t rank;
};


std::ostream & operator << (std::ostream &outs, const Coordinate &coordinate);
bool operator==(const Coordinate& left, const Coordinate& right);
Coordinate operator*(const Coordinate& coordinate, const int multiplier);
Coordinate operator +(const Coordinate& x, const Coordinate& y);

struct MoveSimple {
    friend std::ostream& operator<<(std::ostream&, const MoveSimple&);
    friend bool operator==(const MoveSimple&, const MoveSimple&);
    
    Coordinate source;
    Coordinate destination;
};

std::ostream & operator << (std::ostream &outs, const MoveSimple &move);
bool operator==(const MoveSimple& left, const MoveSimple& right);

enum class MoveCastlingType {
    KingSide,
    QueenSide
};

struct MoveCastling {
    friend bool operator==(const MoveCastling&, const MoveCastling&);
    
    MoveCastlingType type;
    MoveSimple kingsMove;
    MoveSimple rooksMove;
};

bool operator==(const MoveCastling& left, const MoveCastling& right);

struct MovePawnPromotion {
    friend bool operator==(const MovePawnPromotion&, const MovePawnPromotion&);
    
    PieceType promoteTo;
    MoveSimple pawnsMove;
};

bool operator==(const MovePawnPromotion& left, const MovePawnPromotion& right);

typedef std::variant<MoveSimple, MoveCastling, MovePawnPromotion> Move;

std::ostream & operator << (std::ostream &outs, const Move &move);
