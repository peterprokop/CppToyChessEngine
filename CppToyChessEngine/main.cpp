#import <iostream>
#import "ChessGame.hpp"

int main(int argc, const char * argv[]) {
    ChessGame game = ChessGame();
        
    std::cout << game.asString() << "\n";
    
    game.makeMoveMaybe({6, 1}, {6, 2});
    game.makeMoveMaybe({6, 0}, {7, 2});
    game.makeMoveMaybe({5, 0}, {6, 1});
    std::cout << game.asString() << "\n";
    
    const std::vector<Move> moves = game.possibleMovesForPlayer(0);
    
    std::cout << "Possible moves:\n";
    for (auto move : moves) {
        std::cout << move << "\n";
    }
    
    return 0;
}
