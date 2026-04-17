#include "MinMaxEval.hpp"

double scoreForSinglePiece(PieceValueType value) {
    const auto type = PieceType(value & kPieceTypeMask);
    using enum PieceType;
    switch (type) {
        case Pawn:
            return 1;
            break;
        case Knight:
            return 2.5;
            break;
        case Bishop:
            return 3;
            break;
        case Rook:
            return 5;
            break;
        case Queen:
            return 9;
            break;
        case King:
            return 100;
            break;
    }
}

double evaluateTotalPieceScore(uint8_t player, ChessGame game) {
    double currentScore = 0;
    game.chessBoard.iterateBoard([&player, &currentScore](int16_t file, int16_t rank, PieceValueType pieceValue) {
        const auto score = scoreForSinglePiece(pieceValue);
        if (doesPieceBelongToPlayer(pieceValue, player)) {
            currentScore += score;
        } else {
            currentScore -= score;
        }
    });
    
    return currentScore;
}
