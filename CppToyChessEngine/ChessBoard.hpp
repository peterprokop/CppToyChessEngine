#pragma once

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

#include "Coordinate.hpp"

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

bool didPieceMove(PieceValueType value) {
    return value & kHasMovedBit;
}

bool doesPieceBelongToPlayer(PieceValueType value, uint8_t player) {
    return (value & 0x10) / 0x10 == player;
}

bool isPawn(PieceValueType value) {
    return (value & kPieceTypeMask) == 0x1;
}

bool isLastRank(Coordinate destination) {
    return (destination.rank == 0) || (destination.rank == BOARD_SIZE - 1);
}

char pieceFromValue(PieceValueType value) {
    const bool isWhite = ((value & 0x10) == 0);
    char printablePiece = '_';
    switch (value % 0x10) {
        case 0:
            return ' ';
            break;
        case 1:
            printablePiece = 'p';
            break;
        case 2:
            printablePiece = 'n';
            break;
        case 3:
            printablePiece = 'b';
            break;
        case 4:
            printablePiece = 'r';
            break;
        case 5:
            printablePiece = 'q';
            break;
        case 6:
            printablePiece = 'k';
            break;
            
        default:
            return '?';
            break;
    }
    
    if (isWhite) {
        printablePiece += ('A' - 'a');
    }
    
    return printablePiece;
}

PieceValueType pieceValueFromChar(char ch) {
    if (ch == '_') {
        return 0;
    }
    bool isBlackPlayer = (ch >= 'a') && (ch <= 'z');
    PieceValueType value = 0;
    
    if (!isBlackPlayer) {
        ch = ch + 'a' - 'A';
    }
    
    using enum PieceType;
    switch (ch) {
        case 'p':
            value = std::__to_underlying(Pawn);
            break;
        case 'n':
            value = std::__to_underlying(Knight);
            break;
        case 'b':
            value = std::__to_underlying(Bishop);
            break;
        case 'r':
            value = std::__to_underlying(Rook);
            break;
        case 'q':
            value = std::__to_underlying(Queen);
            break;
        case 'k':
            value = std::__to_underlying(King);
            break;
        default:
            value = 0;
            break;
    }
    
    if (isBlackPlayer && value > 0) {
        value |= kBlackPlayerBit;
    }
    
    return value;
}

void trimLeadingNewline(std::string &string) {
    string.erase(0, string.find_first_not_of("\n\r"));
}

std::vector<Coordinate> possibleMoveTargetsForPieceValue(PieceValueType value) {
    switch (value % 0x10) {
        case 1:
            // Pawn
            if ((value & 0x10) == 0) {
                return didPieceMove(value)
                    ? std::vector<Coordinate>{Coordinate({0, 1})}
                    : std::vector<Coordinate>{Coordinate({0, 1}), (Coordinate{0, 2})};
            } else {
                return didPieceMove(value)
                    ? std::vector<Coordinate>{Coordinate{0, -1}}
                    : std::vector<Coordinate>{Coordinate{0, -1}, Coordinate{0, -2}};
            }
            break;
        case 2:
            // Knight
            return {
                {1, 2},
                {1, -2},
                {-1, 2},
                {-1, -2},
                {2, 1},
                {2, -1},
                {-2, 1},
                {-2, -1},
            };
        case 3:
            // TODO: bishop
            return {};
        case 4:
            // TODO: rook
            return {};
        case 5:
            // TODO: queen
            return {};
        case 6:
            // King
            return {
                {-1, -1},
                {-1, 0},
                {-1, 1},
                {0, -1},
                {0, 1},
                {1, -1},
                {1, 0},
                {1, 1},
            };
            break;
            
        default:
            return {};
    }
}

class ChessBoard {
private:
    // 1st index - file, 2nd - rank
    PieceValueType board[BOARD_SIZE][BOARD_SIZE];
    
    ChessBoard(PieceValueType existingBoard[BOARD_SIZE][BOARD_SIZE], MoveSimple move) {
        for (int i = 0; i < BOARD_SIZE; i++) {
            std::copy(std::begin(existingBoard[i]), std::end(existingBoard[i]), std::begin(board[i]));
        }
        board[move.destination.file][move.destination.rank] = board[move.source.file][move.source.rank] | kHasMovedBit;
        board[move.source.file][move.source.rank] = 0;
    }
    
    bool isWithinBoard(Coordinate coordinate) {
        return coordinate.file >= 0
            && coordinate.file < BOARD_SIZE
            && coordinate.rank >= 0
            && coordinate.rank < BOARD_SIZE;
    }
    
public:
    ChessBoard() {
        // Default board for now
        for (int i = 0; i < BOARD_SIZE; i++) {
            board[i][0] = firstRankPieces[i];
            board[i][1] = 0x01;
            for (int j = 2; j < BOARD_SIZE - 2; j++) {
                board[i][j] = 0;
            }
            
            board[i][BOARD_SIZE - 2] = 0x11;
            board[i][BOARD_SIZE - 1] = 0x10 | firstRankPieces[i];
        }
    }
    
    ChessBoard(std::string boardString) {
        ITERATE_BOARD({
            board[FILE][RANK] = 0;
        });
        trimLeadingNewline(boardString);
        
        int file = 0;
        int rank = BOARD_SIZE - 1;
        for(const char& ch : boardString) {
            if (rank < 0) {
                break;
            }
            if (ch == '\n') {
                rank--;
                file = 0;
                continue;
            } else if (file >= 8) {
                file = 0;
                rank--;
            }

            board[file][rank] = pieceValueFromChar(ch);
            file++;
        }
    }
    
    std::string asString() const {
        std::stringstream sstr{};
        for (int i = BOARD_SIZE - 1; i >= 0; i--) {
            sstr << (i + 1) << "|";
            for (int j = 0; j < BOARD_SIZE; j++) {
                sstr << pieceFromValue(board[j][i]) << " ";
            }
            
            sstr << "\n";
        }

        for (int j = 0; j <= BOARD_SIZE; j++) {
            sstr << "--";
        }
        
        sstr << "\n";
        sstr << "  ";
        
        for (int j = 0; j < BOARD_SIZE; j++) {
            sstr << (char)('a' + j) << " ";
        }
        
        sstr << "\n";
        return sstr.str();
    }
    
    ChessBoard boardByMoving(MoveSimple move) {
        return ChessBoard(board, move);
    }
    
    /**
     This will generate all possible moves by specific player (0 - white, 1 - black)
     Note that some moves need to be filtered at the higher level as board does not know
     if en passant is applicable or castling is allowed, it only holds current state.
     */
    std::vector<Move> possibleMovesForPlayer(uint8_t player) {
        std::vector<Move> possibleMoves{};
        ITERATE_BOARD({
            if (CURRENT_PIECE == 0) {
                continue;
            }
            if (doesPieceBelongToPlayer(CURRENT_PIECE, player)) {
                const Coordinate currentCoordinate({FILE, RANK});
                // Process regular pawn, knight and king moves
                for (const auto& offset : possibleMoveTargetsForPieceValue(CURRENT_PIECE)) {
                    const Coordinate destination = currentCoordinate + offset;
                    if (isWithinBoard(destination)) {
                        // TODO: check if destination is blocked (for all pieces)
                        // TODO: check if move generates check for own king
                        // TODO: add moves for pawn capture
                        // TODO: add moves for en passant capture
                        // TODO: add moves for castling
                        const PieceValueType destinationPiece = board[destination.file][destination.rank];
                        if (destinationPiece > 0 && doesPieceBelongToPlayer(destinationPiece, player)) {
                            continue;
                        }
                                                
                        // Pawn logic
                        if (isPawn(CURRENT_PIECE)) {
                            // Pawn can't move forward if square is not empty
                            if (destinationPiece != 0) {
                                continue;
                            }
                            // TODO: Handle pawn promotion when capturing piece
                            // Handle pawn promotion
                            if (isLastRank(destination)) {
                                std::for_each(
                                  kPawnPromotionOptions.begin(),
                                  kPawnPromotionOptions.end(),
                                  [&](PieceType pieceType)
                                  {
                                      possibleMoves.push_back(MovePawnPromotion({
                                          pieceType,
                                          MoveSimple(currentCoordinate, destination)
                                      }));
                                  }
                              );
                            }
                        } else {
                            // Non-pawn-promotion
                            possibleMoves.push_back(MoveSimple(currentCoordinate, destination));
                        }
                    }
                }
            }
        })
        
        // TODO: check if castling is possible
        // if !(king has moved) && !(rook1 has moved) && (rook, king on same rank)
        // && (space between king and rook is empty) && (space between kings source and destination is not attacked)
        return possibleMoves;
    }
};
