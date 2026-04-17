#include "ChessGame.hpp"
#include "ChessPlayer.hpp"

#include <iostream>

int main(int argc, const char * argv[]) {
    ChessGame game = ChessGame();
        
    std::cout << game.asString() << "\n";
    
    game.makeMoveMaybe(MoveSimple({6, 1}, {6, 2}));
    game.makeMoveMaybe(MoveSimple({6, 0}, {7, 2}));
    game.makeMoveMaybe(MoveSimple({5, 0}, {6, 1}));
    std::cout << game.asString() << "\n";
    
    const std::vector<Move> moves = game.possibleMovesForPlayer(0);
    
    std::cout << "Possible moves for white:\n";
    for (auto move : moves) {
        std::cout << move << "\n";
    }
    
    
    for (int16_t i = 0; i < 8; i++) {
        game.makeMoveMaybe(MoveSimple({i, 6}, {i, 5}));
    }
    std::cout << game.asString() << "\n";
    std::cout << "Possible moves for black:\n";
    for (auto move : game.possibleMovesForPlayer(1)) {
        std::cout << move << "\n";
    }
    
    // Show rook moves
    std::string gameString = R"(
________
________
________
________
_p______
________
pR____p_
_q______
)";
    
    ChessGame rookGame = ChessGame(gameString);
    
    // Check moves
    std::vector<Move> whiteMoves = rookGame.possibleMovesForPlayer(0);
    std::cout << rookGame.asString() << "\n";
    std::cout << "Possible moves for white:\n";
    for (auto move : whiteMoves) {
        std::cout << move << "\n";
    }
    
    // Dummy game with 2 players which make random moves    
    const std::array<std::unique_ptr<ChessPlayer>, 2> players = {
        std::make_unique<ChessPlayerRandom>(ChessPlayerRandom(0)),
        std::make_unique<ChessPlayerMaxOneLevel>(ChessPlayerMaxOneLevel(1)),
    };
    
    int currentPlayer = 0;
    ChessGame newGame = ChessGame();
    
    while (true) {
        auto moves = newGame.possibleMovesForPlayer(currentPlayer);
        if (moves.size() == 0) {
            break;
        }
        
        const auto move = players[currentPlayer]->makeAMove(moves, newGame);
        
        newGame.makeMoveMaybe(move);
        std::cout << "Player " << (currentPlayer + 1) << " move: " << move << "\n";
        std::cout << newGame.asString() << "\n";
        
        currentPlayer++;
        currentPlayer %= 2;
    }
    
    return 0;
}
