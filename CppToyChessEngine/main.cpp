#include <iostream>

#include "ChessGame.hpp"

int main(int argc, const char * argv[]) {
    ChessGame game = ChessGame();
        
    std::cout << game.asString() << "\n";
    
    game.makeMoveMaybe({6, 1}, {6, 2});
    game.makeMoveMaybe({6, 0}, {7, 2});
    game.makeMoveMaybe({5, 0}, {6, 1});
    std::cout << game.asString() << "\n";
    
    const std::vector<Move> moves = game.possibleMovesForPlayer(0);
    
    std::cout << "Possible moves for white:\n";
    for (auto move : moves) {
        std::cout << move << "\n";
    }
    
    
    for (int16_t i = 0; i < 8; i++) {
        game.makeMoveMaybe({i, 6}, {i, 5});
    }
    std::cout << game.asString() << "\n";
    std::cout << "Possible moves for black:\n";
    for (auto move : game.possibleMovesForPlayer(1)) {
        std::cout << move << "\n";
    }
    
    game.makeMoveMaybe({0, 5}, {0, 1});
    std::cout << game.asString() << "\n";
    std::cout << "Possible moves for black:\n";
    for (auto move : game.possibleMovesForPlayer(1)) {
        std::cout << move << "\n";
    }
    
    return 0;
}
