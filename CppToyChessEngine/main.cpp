#import <iostream>
#import "ChessGame.hpp"

int main(int argc, const char * argv[]) {
    ChessGame game = ChessGame();
        
    std::cout << game.asString();
    
    game.makeMoveMaybe({2, 1}, {2, 2});
    std::cout << game.asString();
    return 0;
}
