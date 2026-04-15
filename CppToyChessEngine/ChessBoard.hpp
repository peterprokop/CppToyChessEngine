#pragma once

#include "Coordinate.hpp"
#include "Move.hpp"
#include "ChessBoardHelpers.hpp"

#include <vector>
#include <sstream>

constexpr std::array<PieceType, 4> kPawnPromotionOptions {
    PieceType::Knight,
    PieceType::Bishop,
    PieceType::Rook,
    PieceType::Queen,
};

const std::vector<PieceValueType> firstRankPieces =
    {4, 2, 3, 5, 6, 3, 2, 4};

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
    
    ChessBoard(PieceValueType existingBoard[BOARD_SIZE][BOARD_SIZE]) {
        for (int i = 0; i < BOARD_SIZE; i++) {
            std::copy(std::begin(existingBoard[i]), std::end(existingBoard[i]), std::begin(board[i]));
        }
    }
    
    bool isWithinBoard(Coordinate coordinate) const {
        return coordinate.file >= 0
            && coordinate.file < BOARD_SIZE
            && coordinate.rank >= 0
            && coordinate.rank < BOARD_SIZE;
    }
    
    std::vector<Move> possibleMovesForRaycaster(Coordinate currentCoordinate,
                                                PieceType pieceType,
                                                uint8_t player) const {
        std::vector<Move> possibleMoves{};
        const std::span<const Coordinate> offsets = moveOffsetsForPieceType(pieceType);
        
        for (const auto& offset : offsets) {
            for (int i = 1; i < BOARD_SIZE; i++) {
                const Coordinate destination = currentCoordinate + offset * i;
                if (isWithinBoard(destination)) {
                    const PieceValueType destinationPiece = board[destination.file][destination.rank];
                    if (destinationPiece == 0) {
                        possibleMoves.push_back(
                            MoveSimple(currentCoordinate, destination)
                        );
                        // Continue iterating `i` (range)
                        continue;
                    }
                    
                    if (!doesPieceBelongToPlayer(destinationPiece, player)) {
                        // Piece belongs to opponent - move can be added
                        possibleMoves.push_back(
                            MoveSimple(currentCoordinate, destination)
                        );
                    }
                    // Stop iterating `i` (range)
                    break;
                } else {
                    // Stop iterating `i` (range)
                    break;
                }
            }
        }
        
        return possibleMoves;
    }
    
    std::vector<Move> possibleMovesForNonRaycaster(Coordinate currentCoordinate,
                                                   PieceValueType currentPiece,
                                                   uint8_t player) const {
        std::vector<Move> possibleMoves{};
        for (const auto& offset : possibleMoveTargetsForPieceValue(currentPiece)) {
            const Coordinate destination = currentCoordinate + offset;
            if (isWithinBoard(destination)) {
                // TODO: check if destination is blocked (for all pieces)
                // TODO: check if move generates check for own king
                // TODO: add moves for pawn capture
                // TODO: add moves for en passant capture
                const PieceValueType destinationPiece = board[destination.file][destination.rank];
                if (destinationPiece > 0 && doesPieceBelongToPlayer(destinationPiece, player)) {
                    continue;
                }
                                        
                if (isPawn(currentPiece)) {
                    // Pawn can't move forward if square is not empty
                    if (destinationPiece != 0) {
                        continue;
                    }
                    // TODO: Handle pawn promotion when capturing piece
                    // Handle pawn promotion
                    if (isLastRank(destination)) {
                        std::ranges::for_each(
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
                    } else {
                        // No promotion
                        possibleMoves.push_back(MoveSimple(currentCoordinate, destination));
                    }
                } else {
                    // Non-pawn logic
                    possibleMoves.push_back(MoveSimple(currentCoordinate, destination));
                }
            }
        }
        
        return possibleMoves;
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
    
    ChessBoard boardByChangingPieceType(PieceType pieceType, Coordinate coordinate) {
        ChessBoard newBoard = ChessBoard(board);
        PieceValueType pieceValue = newBoard.board[coordinate.file][coordinate.rank];
        
        newBoard.board[coordinate.file][coordinate.rank] =
            (pieceValue & ~kPieceTypeMask) | (std::to_underlying(pieceType) & kPieceTypeMask);
        
        return newBoard;
    }
    
    /**
     This will generate all possible moves by specific player (0 - white, 1 - black)
     Note that some moves need to be filtered at the higher level as board does not know
     if en passant is applicable or castling is allowed, it only holds current state.
     */
    std::vector<Move> possibleMovesForPlayer(uint8_t player) {
        std::vector<Move> possibleMoves{};
        
        for (int16_t RANK = BOARD_SIZE - 1; RANK >= 0; RANK--) {
            for (int16_t FILE = 0; FILE < BOARD_SIZE; FILE++) {
                const auto CURRENT_PIECE = board[FILE][RANK];

                if (CURRENT_PIECE == 0) {
                    continue;
                }
                
                if (!doesPieceBelongToPlayer(CURRENT_PIECE, player)) {
                    continue;
                }
                
                const Coordinate currentCoordinate({FILE, RANK});
                PieceType pieceType = PieceType(CURRENT_PIECE & kPieceTypeMask);
                using enum PieceType;
                if (pieceType == Bishop || pieceType == Rook || pieceType == Queen) {
                    const auto movesToAdd = possibleMovesForRaycaster(currentCoordinate, pieceType, player);
                    possibleMoves.append_range(movesToAdd);
                } else {
                    // Process regular pawn, knight and king moves
                    const auto movesToAdd = possibleMovesForNonRaycaster(currentCoordinate, CURRENT_PIECE, player);
                    possibleMoves.append_range(movesToAdd);
                }
            }
        }
        
        // TODO: check if castling is possible
        // if !(king has moved) && !(rook1 has moved) && (rook, king on same rank)
        // && (space between king and rook is empty) && (space between kings source and destination is not attacked)
        return possibleMoves;
    }
};
