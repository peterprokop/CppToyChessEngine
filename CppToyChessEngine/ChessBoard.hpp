#import <iostream>
#import <sstream>
#include <algorithm>
#include <iterator>

#import "Coordinate.hpp"

/*
 Pieces:
 0x0 - Empty
 0x1 - Pawn
 0x2 - Knight
 0x3 - Bishop
 0x4 - Rook
 0x5 - Queen
 0x6 - King
 */

#define BOARD_SIZE 8
const std::vector<uint8_t> firstRankPieces =
    {4, 2, 3, 5, 6, 3, 2, 4};

#define ITERATE_BOARD(CODE) \
for (int16_t RANK = BOARD_SIZE - 1; RANK >= 0; RANK--) {\
    for (int16_t FILE = 0; FILE < BOARD_SIZE; FILE++) {\
        const auto CURRENT_PIECE = board[FILE][RANK];\
        CODE\
    } \
}

char pieceFromValue(uint8_t value) {
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

std::vector<Coordinate> possibleMoveTargetsForPieceValue(uint8_t value) {
    switch (value % 0x10) {
        case 1:
            // TODO: add double first move
            if ((value & 0x10) == 0) {
                return {{0, 1}};
            } else {
                return {{0, -1}};
            }
            break;
        case 2:
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
    uint8_t board[BOARD_SIZE][BOARD_SIZE];
    
    ChessBoard(uint8_t existingBoard[BOARD_SIZE][BOARD_SIZE], Move move) {
        for (int i = 0; i < BOARD_SIZE; i++) {
            std::copy(std::begin(existingBoard[i]), std::end(existingBoard[i]), std::begin(board[i]));
        }
        board[move.destination.file][move.destination.rank] = board[move.source.file][move.source.rank];
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
    
    ChessBoard boardByMoving(Move move) {
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
            if ((CURRENT_PIECE & 0x10) == player) {
                const Coordinate currentCoordinate({FILE, RANK});
                for (const auto& offset : possibleMoveTargetsForPieceValue(CURRENT_PIECE)) {
                    const Coordinate destination = currentCoordinate + offset;
                    if (isWithinBoard(destination)) {
                        // TODO: check if destination is blocked (for all pieces)
                        // TODO: check if move generates check for own king
                        // TODO: add moves for pawn capture
                        // TODO: add moves for en passant capture
                        // TODO: add moves for pawn promotion
                        // TODO: add moves for castling
                        const uint8_t destinationPiece = board[destination.file][destination.rank];
                        if (destinationPiece > 0 && ((destinationPiece & 0x10) == player)) {
                            continue;
                        }

                        possibleMoves.push_back({currentCoordinate, destination});
                    }
                }
            }
        })
        return possibleMoves;
    }
};
