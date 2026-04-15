#include "PieceType.h"

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
