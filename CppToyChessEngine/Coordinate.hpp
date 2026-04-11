#pragma once

#include <variant>
#include <type_traits>

#include "PieceType.hpp"

struct Coordinate {
    friend std::ostream& operator<<(std::ostream&, const Coordinate&);
    
    int16_t file;
    int16_t rank;
};


std::ostream & operator << (std::ostream &outs, const Coordinate &coordinate) {
    return outs << (char)('a' + coordinate.file) << (coordinate.rank + 1);
}

Coordinate operator +(const Coordinate& x, const Coordinate& y) {
    return {
        static_cast<int16_t>(x.file + y.file),
        static_cast<int16_t>(x.rank + y.rank)
    };
}

struct MoveSimple {
    friend std::ostream& operator<<(std::ostream&, const MoveSimple&);
    
    Coordinate source;
    Coordinate destination;
};

std::ostream & operator << (std::ostream &outs, const MoveSimple &move) {
    return outs << move.source << " -> " << move.destination;
}

enum class MoveCastlingType {
    KingSide,
    QueenSide
};

struct MoveCastling {
    MoveCastlingType type;
    MoveSimple kingsMove;
    MoveSimple rooksMove;
};

struct MovePawnPromotion {
    PieceType promoteTo;
    MoveSimple pawnsMove;
};

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
