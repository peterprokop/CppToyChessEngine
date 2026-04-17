#pragma once

#include "ChessBoard.hpp"

#include <vector>

class ChessGame {
public:
    ChessBoard chessBoard;
    
    ChessGame();
    ChessGame(const ChessGame& game);
    ChessGame(std::string boardString);
    
    std::string asString() const;
    bool makeMoveMaybe(Move move);    
    std::vector<Move> possibleMovesForPlayer(uint8_t player);
};
