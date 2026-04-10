#include <variant>

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

typedef std::variant<MoveSimple, MoveCastling> Move;

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
        } else {
            static_assert(false, "non-exhaustive visitor");
        }
    }, move);
    
    return outs;
}
