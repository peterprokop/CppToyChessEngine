#pragma once
#include <variant>

#include "Coordinate.hpp"

struct MoveSimple {
    friend std::ostream& operator<<(std::ostream&, const MoveSimple&);
    friend bool operator==(const MoveSimple&, const MoveSimple&);
    
    Coordinate source;
    Coordinate destination;
};

std::ostream& operator<<(std::ostream &outs, const MoveSimple &move);
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
