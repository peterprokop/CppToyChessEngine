#include <cstdint>
#include <vector>

#include "PieceType.hpp"
#include "Coordinate.hpp"

#include <span>

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

#define BOARD_SIZE 8

constexpr uint16_t kPieceTypeMask = 0xF;
constexpr uint16_t kPlayerMask = 0xF0;
constexpr uint16_t kBlackPlayerBit = 0b1'0000;

constexpr uint16_t kHasMovedMask = 0xF00;
constexpr uint16_t kHasMovedBit = 0b1'0000'0000;

bool didPieceMove(PieceValueType value);
bool doesPieceBelongToPlayer(PieceValueType value, uint8_t player);
bool isPawn(PieceValueType value);
bool isRook(PieceValueType value);
bool isLastRank(Coordinate destination);

char pieceFromValue(PieceValueType value);

PieceValueType pieceValueFromChar(char ch);

void trimLeadingNewline(std::string &string);

std::vector<Coordinate> possibleMoveTargetsForPieceValue(PieceValueType value);
std::span<const Coordinate> moveOffsetsForPieceType(PieceType pieceType);
