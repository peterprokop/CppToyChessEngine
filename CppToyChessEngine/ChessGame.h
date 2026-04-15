#pragma once

#include "ChessBoard.h"

#include <vector>

class ChessGame {
private:
    ChessBoard chessBoard;
public:
    ChessGame();
    
    ChessGame(std::string boardString);
    
    std::string asString() const;
    
    bool makeMoveMaybe(Move move);
    
    std::vector<Move> possibleMovesForPlayer(uint8_t player);
};
