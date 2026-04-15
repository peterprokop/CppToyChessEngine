#pragma once

#include "PieceType.hpp"

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

struct MoveSimple {
    friend std::ostream& operator<<(std::ostream&, const MoveSimple&);
    friend bool operator==(const MoveSimple&, const MoveSimple&);
    
    Coordinate source;
    Coordinate destination;
};

std::ostream & operator << (std::ostream &outs, const MoveSimple &move) {
    return outs << move.source << " -> " << move.destination;
}

bool operator==(const MoveSimple& left, const MoveSimple& right) {
    return left.source == right.source
    && left.destination == right.destination;
}

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

bool operator==(const MoveCastling& left, const MoveCastling& right) {
    return left.type == right.type
    && left.kingsMove == right.kingsMove
    && left.rooksMove == right.rooksMove;
}

struct MovePawnPromotion {
    friend bool operator==(const MovePawnPromotion&, const MovePawnPromotion&);
    
    PieceType promoteTo;
    MoveSimple pawnsMove;
};

bool operator==(const MovePawnPromotion& left, const MovePawnPromotion& right) {
    return left.promoteTo == right.promoteTo
    && left.pawnsMove == right.pawnsMove;
}

typedef std::variant<MoveSimple, MoveCastling, MovePawnPromotion> Move;

std::ostream & operator << (std::ostream &outs, const Move &move) {
    std::visit([&](auto&& arg)
    {
        using T = std::decay_t<decltype(arg)>;
        
        if constexpr (std::is_same_v<T, MoveSimple>) {
            outs << arg.source << " -> " << arg.destination;
        } else if constexpr (std::is_same_v<T, MoveCastling>) {
            outs << ((arg.type == MoveCastlingType::KingSide)
                ? "0-0"
                : "0-0-0");
        } else if constexpr (std::is_same_v<T, MovePawnPromotion>) {
            outs << arg.pawnsMove.source << " -> " << arg.pawnsMove.destination
                << algebraicNotationForPieceType(arg.promoteTo);
        } else {
            static_assert(false, "non-exhaustive visitor");
        }
    }, move);
    
    return outs;
}
