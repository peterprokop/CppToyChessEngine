#include "ChessGame.hpp"

ChessGame::ChessGame() {
    chessBoard = ChessBoard();
}

ChessGame::ChessGame(const ChessGame& game) {
    chessBoard = game.chessBoard;
}

ChessGame::ChessGame(std::string boardString) {
    chessBoard = ChessBoard(boardString);
}

std::string ChessGame::asString() const {
    return chessBoard.asString();
}

// TODO: handle pawn promotion, castling
// TODO: add player arg, check if move is permitted
bool ChessGame::makeMoveMaybe(Move move) {
    std::visit([&](auto&& arg)
    {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, MoveSimple>) {
            this->chessBoard = this->chessBoard.boardByMoving(std::get<MoveSimple>(move));
        } else if constexpr (std::is_same_v<T, MoveCastling>) {
            MoveCastling moveCastling = std::get<MoveCastling>(move);
            this->chessBoard = this->chessBoard
                .boardByMoving(moveCastling.kingsMove)
                .boardByMoving(moveCastling.rooksMove);
        } else if constexpr (std::is_same_v<T, MovePawnPromotion>) {
            MovePawnPromotion movePawnPromo = std::get<MovePawnPromotion>(move);
            this->chessBoard = this->chessBoard
                .boardByMoving(movePawnPromo.pawnsMove)
                .boardByChangingPieceType(
                                          movePawnPromo.promoteTo,
                                          movePawnPromo.pawnsMove.destination
                                          );
        } else {
            static_assert(false, "non-exhaustive visitor");
        }
    }, move);

    return true;
}

std::vector<Move> ChessGame::possibleMovesForPlayer(uint8_t player) {
    // TODO: check en passant
    // TODO: check castling
    // TODO: check 3 move rule
    // TODO: check 50 move rule
    return chessBoard.possibleMovesForPlayer(player);
}
