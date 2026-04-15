#pragma once

#include "ChessBoard.hpp"

#include <vector>

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
    // TODO: add player arg, check if move is permitted
    bool makeMoveMaybe(Move move) {
        std::visit([&](auto&& arg)
        {
            using T = std::decay_t<decltype(arg)>;
            
            if constexpr (std::is_same_v<T, MoveSimple>) {
                this->chessBoard = this->chessBoard.boardByMoving(std::get<MoveSimple>(move));
            } else if constexpr (std::is_same_v<T, MoveCastling>) {
                // Not implemented yet
                std::abort();
            } else if constexpr (std::is_same_v<T, MovePawnPromotion>) {
                MovePawnPromotion movePawnPromo = std::get<MovePawnPromotion>(move);
                this->chessBoard = this->chessBoard
                    .boardByMoving(movePawnPromo.pawnsMove)
                    .boardByChangingPieceType(
                                              movePawnPromo.promoteTo,
                                              movePawnPromo.pawnsMove.destination
                                              );
            } else {
                static_assert(false, "non-exhaustive visitor");
            }
        }, move);
        
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
