#pragma once

#include <cstdint>

enum class PieceType: uint8_t {
    Pawn = 0x1,
    Knight = 0x2,
    Bishop = 0x3,
    Rook = 0x4,
    Queen = 0x5,
    King = 0x6,
};

char algebraicNotationForPieceType(PieceType pieceType) {
    using enum PieceType;
    switch (pieceType) {
        case Pawn:
            return 'P';
        case Knight:
            return 'N';
        case Bishop:
            return 'B';
        case Rook:
            return 'R';
        case Queen:
            return 'Q';
        case King:
            return 'K';
    }
}
