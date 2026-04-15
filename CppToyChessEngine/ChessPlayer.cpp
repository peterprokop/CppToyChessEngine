#include "ChessPlayer.hpp"

Move ChessPlayerRandom::makeAMove(std::vector<Move> moves) {
    assert(moves.size() > 0);
    return moves[std::rand() % moves.size()];
}
