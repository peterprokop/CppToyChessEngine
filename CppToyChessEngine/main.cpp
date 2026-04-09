#import <iostream>
#import "ChessGame.hpp"

int main(int argc, const char * argv[]) {
    ChessGame game = ChessGame();
    
    // Just prints initial board for now
    std::cout << game.asString();
    return 0;
}
