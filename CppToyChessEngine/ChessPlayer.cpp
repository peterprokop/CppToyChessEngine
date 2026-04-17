#include "ChessPlayer.hpp"
#include "MinMaxEval/MinMaxEval.hpp"

Move ChessPlayerRandom::makeAMove(std::vector<Move> moves, ChessGame game) {
    assert(moves.size() > 0);
    return moves[std::rand() % moves.size()];
}

Move ChessPlayerMaxOneLevel::makeAMove(std::vector<Move> moves, ChessGame game) {
    assert(moves.size() > 0);
    
    double currentMaxScore = std::numeric_limits<double>::lowest();
    Move bestMove;
    for (const auto& move : moves) {
        ChessGame evalGame = ChessGame(game);
        evalGame.makeMoveMaybe(move);
        
        const double score = evaluateTotalPieceScore(color, evalGame);
        if (score > currentMaxScore) {
            bestMove = move;
            currentMaxScore = score;
        }
    }
    
    return bestMove;
}
