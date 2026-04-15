#pragma once

#include "Coordinate.h"

#include <vector>
#include <sstream>

/*
 Every piece is encoded by 16 bits
 
 First 4 bits are for piece type:
 0x0 - Empty
 0x1 - Pawn
 0x2 - Knight
 0x3 - Bishop
 0x4 - Rook
 0x5 - Queen
 0x6 - King
 
 2nd 4 bits for color:
 0x0? - white
 0x1? - black
 
 3nd 4 bits are indicating if piece has moved or not:
 0x0?? - not moved
 0b0001'????'???? - moved
 */
typedef uint16_t PieceValueType;

constexpr uint16_t kPieceTypeMask = 0xF;
constexpr uint16_t kPlayerMask = 0xF0;
constexpr uint16_t kBlackPlayerBit = 0b1'0000;

constexpr uint16_t kHasMovedMask = 0xF00;
constexpr uint16_t kHasMovedBit = 0b1'0000'0000;

constexpr std::array<PieceType, 4> kPawnPromotionOptions {
    PieceType::Knight,
    PieceType::Bishop,
    PieceType::Rook,
    PieceType::Queen,
};

#define BOARD_SIZE 8
const std::vector<PieceValueType> firstRankPieces =
    {4, 2, 3, 5, 6, 3, 2, 4};

#define ITERATE_BOARD(CODE) \
for (int16_t RANK = BOARD_SIZE - 1; RANK >= 0; RANK--) {\
    for (int16_t FILE = 0; FILE < BOARD_SIZE; FILE++) {\
        const auto CURRENT_PIECE = board[FILE][RANK];\
        CODE\
    } \
}

bool didPieceMove(PieceValueType value);
bool doesPieceBelongToPlayer(PieceValueType value, uint8_t player);
bool isPawn(PieceValueType value);
bool isRook(PieceValueType value);
bool isLastRank(Coordinate destination);

char pieceFromValue(PieceValueType value);
PieceValueType pieceValueFromChar(char ch);
void trimLeadingNewline(std::string &string);
std::vector<Coordinate> possibleMoveTargetsForPieceValue(PieceValueType value);
std::vector<Coordinate> moveOffsetsForPieceType(PieceType pieceType);

class ChessBoard {
private:
    // 1st index - file, 2nd - rank
    PieceValueType board[BOARD_SIZE][BOARD_SIZE];
    
    ChessBoard(PieceValueType existingBoard[BOARD_SIZE][BOARD_SIZE], MoveSimple move);
    ChessBoard(PieceValueType existingBoard[BOARD_SIZE][BOARD_SIZE]);
    bool isWithinBoard(Coordinate coordinate);
    
public:
    ChessBoard();
    
    ChessBoard(std::string boardString);
    
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
