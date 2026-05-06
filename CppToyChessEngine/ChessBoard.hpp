#pragma once

#include "Coordinate.hpp"
#include "Move.hpp"
#include "ChessBoardHelpers.hpp"

#include <vector>
#include <sstream>
#include <functional>

constexpr std::array<PieceType, 4> kPawnPromotionOptions {
    PieceType::Knight,
    PieceType::Bishop,
    PieceType::Rook,
    PieceType::Queen,
};

constexpr std::array<PieceValueType, BOARD_SIZE> firstRankPieces =
    {4, 2, 3, 5, 6, 3, 2, 4};

class ChessBoard {
private:
    // 1st index - file, 2nd - rank
    PieceValueType board[BOARD_SIZE][BOARD_SIZE];
    
    ChessBoard(PieceValueType existingBoard[BOARD_SIZE][BOARD_SIZE], MoveSimple move);    
    ChessBoard(PieceValueType existingBoard[BOARD_SIZE][BOARD_SIZE]);
    
    bool isWithinBoard(Coordinate coordinate) const;
    
    std::vector<Move> possibleMovesForRaycaster(Coordinate currentCoordinate,
                                                PieceType pieceType,
                                                uint8_t player) const;
    
    std::vector<Move> possibleMovesForNonRaycaster(Coordinate currentCoordinate,
                                                   PieceValueType currentPiece,
                                                   uint8_t player) const;
    
public:
    ChessBoard();
    ChessBoard(std::string boardString);
    
    void iterateBoard(std::function<void(int16_t, int16_t, PieceValueType)> callback) const;
    std::string asString() const;
    ChessBoard boardByMoving(MoveSimple move);
    ChessBoard boardByChangingPieceType(PieceType pieceType, Coordinate coordinate);
    
    /**
     This will generate all possible moves by specific player (0 - white, 1 - black)
     Note that some moves need to be filtered at the higher level as board does not know
     if en passant is applicable or castling is allowed, it only holds current state.
     */
    std::vector<Move> possibleMovesForPlayer(uint8_t player);
};
