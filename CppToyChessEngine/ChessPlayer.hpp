#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <vector>

#include "Move.hpp"

class ChessPlayer {
private:
    uint8_t color;
public:
    ChessPlayer(uint8_t color):color(color) {}
    virtual Move makeAMove(std::vector<Move> moves) = 0;
};

class ChessPlayerRandom: ChessPlayer {
public:
    ChessPlayerRandom(uint8_t color): ChessPlayer(color) {
        std::srand((unsigned int) std::time(nullptr));
    }
    
    Move makeAMove(std::vector<Move> moves) override;
};
