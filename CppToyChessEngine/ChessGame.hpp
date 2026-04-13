#pragma once

#include <vector>

#include "ChessBoard.hpp"

class ChessGame {
private:
    ChessBoard chessBoard;
public:
    ChessGame() {
        chessBoard = ChessBoard();
    }
    
    ChessGame(std::string boardString) {
        chessBoard = ChessBoard(boardString);
    }
    
    std::string asString() const {
        return chessBoard.asString();
    }
    
    // TODO: handle pawn promotion, castling
    bool makeMoveMaybe(Coordinate source, Coordinate destination) {
        MoveSimple move = {source, destination};
        chessBoard = chessBoard.boardByMoving(move);
        return true;
    }
    
    std::vector<Move> possibleMovesForPlayer(uint8_t player) {
        // TODO: check en passant
        // TODO: check castling
        // TODO: check 3 move rule
        // TODO: check 50 move rule
        return chessBoard.possibleMovesForPlayer(player);
    }
};
