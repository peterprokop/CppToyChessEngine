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

struct Move {
    friend std::ostream& operator<<(std::ostream&, const Move&);
    
    Coordinate source;
    Coordinate destination;
};

std::ostream & operator << (std::ostream &outs, const Move &move) {
    return outs << move.source << " -> " << move.destination;
}
