#import <iostream>
#import <sstream>

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

char pieceFromValue(uint8_t value) {
    const bool isWhite = ((value & 0x10) == 0);
    char printablePiece = ' ';
    switch (value % 0x10) {
        case 0:
            printablePiece = ' ';
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
            printablePiece = '?';
            break;
    }
    
    if (isWhite) {
        printablePiece += ('A' - 'a');
    }
    
    return printablePiece;
}

class ChessBoard {
private:
    // 1st index - file, 2nd - rank
    uint8_t board[BOARD_SIZE][BOARD_SIZE];
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
            for (int j = 0; j < BOARD_SIZE; j++) {
                sstr << pieceFromValue(board[j][i]) << " ";
            }
            
            sstr << "\n";
        }
        return sstr.str();
    }
};
