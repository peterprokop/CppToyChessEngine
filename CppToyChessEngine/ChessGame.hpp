#import "ChessBoard.hpp"

class ChessGame {
private:
    ChessBoard chessBoard;
public:
    ChessGame() {
        chessBoard = ChessBoard();
    }
    
    std::string asString() const {
        return chessBoard.asString();
    }
};
