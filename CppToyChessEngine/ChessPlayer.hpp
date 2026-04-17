#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <vector>

#include "Move.hpp"
#include "ChessGame.hpp"

class ChessPlayer {
protected:
    uint8_t color;
public:
    ChessPlayer(uint8_t color):color(color) {}
    virtual Move makeAMove(std::vector<Move> moves, ChessGame game) = 0;
};

class ChessPlayerRandom: public ChessPlayer {
public:
    ChessPlayerRandom(uint8_t color): ChessPlayer(color) {
        std::srand((unsigned int) std::time(nullptr));
    }
    
    Move makeAMove(std::vector<Move> moves, ChessGame game) override;
};

class ChessPlayerMaxOneLevel: public ChessPlayer {
public:
    ChessPlayerMaxOneLevel(uint8_t color): ChessPlayer(color) {}
    
    Move makeAMove(std::vector<Move> moves, ChessGame game) override;
};
