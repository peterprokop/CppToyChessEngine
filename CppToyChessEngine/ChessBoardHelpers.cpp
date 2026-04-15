#include "ChessBoardHelpers.hpp"

bool didPieceMove(PieceValueType value) {
    return value & kHasMovedBit;
}

bool doesPieceBelongToPlayer(PieceValueType value, uint8_t player) {
    return (value & 0x10) / 0x10 == player;
}

bool isPawn(PieceValueType value) {
    return (value & kPieceTypeMask) == 0x1;
}

bool isRook(PieceValueType value) {
    return PieceType(value & kPieceTypeMask) == PieceType::Rook;
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
            value = std::to_underlying(Pawn);
            break;
        case 'n':
            value = std::to_underlying(Knight);
            break;
        case 'b':
            value = std::to_underlying(Bishop);
            break;
        case 'r':
            value = std::to_underlying(Rook);
            break;
        case 'q':
            value = std::to_underlying(Queen);
            break;
        case 'k':
            value = std::to_underlying(King);
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

std::vector<Coordinate> moveOffsetsForPieceType(PieceType pieceType) {
#define kRookOffsets {1, 0}, {-1, 0}, {0, 1}, {0, -1}
#define kBishopOffsets {1, 1}, {-1, 1}, {1, -1}, {-1, -1}
    
    using enum PieceType;
    switch (pieceType) {
        case Bishop:
            return {kBishopOffsets};
        case Rook:
            return {kRookOffsets};
        case Queen:
            return {kRookOffsets, kBishopOffsets};
        default:
            return {};
    }
}
