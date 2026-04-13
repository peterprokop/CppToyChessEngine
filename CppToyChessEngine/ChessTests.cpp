#include <gtest/gtest.h>
#include <vector>
#include <variant>

#include "ChessGame.hpp"

TEST(ChessGameTest, PawnPromotion) {
    std::string gameString = R"(
_______k
P_______
________
________
________
________
________
K_______
)";
    
    ChessGame game = ChessGame(gameString);
    
    // Check moves
    std::vector<Move> whiteMoves = game.possibleMovesForPlayer(0);
    std::vector<Move> expectedWhiteMoves{
        MovePawnPromotion{
            PieceType::Knight,
            MoveSimple({0,6}, {0,7}),
        },
        MovePawnPromotion{
            PieceType::Bishop,
            MoveSimple({0,6}, {0,7}),
        },
        MovePawnPromotion{
            PieceType::Rook,
            MoveSimple({0,6}, {0,7}),
        },
        MovePawnPromotion{
            PieceType::Queen,
            MoveSimple({0,6}, {0,7}),
        },
        MoveSimple({0,0}, {0,1}),
        MoveSimple({0,0}, {1,0}),
        MoveSimple({0,0}, {1,1}),
    };
    
    EXPECT_EQ(whiteMoves.size(), 7);
    
    EXPECT_EQ(whiteMoves, expectedWhiteMoves);
}

TEST(ChessGameTest, PawnMoves) {
    std::string gameString = R"(
________
________
________
________
________
________
P_______
________
)";
    
    ChessGame game = ChessGame(gameString);
    
    // Check moves
    std::vector<Move> whiteMoves = game.possibleMovesForPlayer(0);
    std::vector<Move> expectedWhiteMoves{
        MoveSimple({0,1}, {0,2}),
        MoveSimple({0,1}, {0,3}),
    };
    
    EXPECT_EQ(whiteMoves.size(), 2);
    
    EXPECT_EQ(whiteMoves, expectedWhiteMoves);
    
    game.makeMoveMaybe({0,1}, {0,3});
    
    // Check again
    std::vector<Move> whiteMovesAfterPawnMove = game.possibleMovesForPlayer(0);
    std::vector<Move> expectedWhiteMovesAfterPawnMove{
        MoveSimple({0,3}, {0,4}),
    };
    
    EXPECT_EQ(whiteMovesAfterPawnMove.size(), 1);
    
    EXPECT_EQ(whiteMovesAfterPawnMove, expectedWhiteMovesAfterPawnMove);
}
