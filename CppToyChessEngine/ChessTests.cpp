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
    
    // Promote to knight
    
    game.makeMoveMaybe(MovePawnPromotion{
                           PieceType::Knight,
                           MoveSimple({0,6}, {0,7}),
                       });
    
    std::vector<Move> whiteMovesAfterPromo = game.possibleMovesForPlayer(0);
    std::vector<Move> expectedWhiteMovesAfterPromo{
        // Knight
        MoveSimple({0,7}, {1,5}),
        MoveSimple({0,7}, {2,6}),
        // King
        MoveSimple({0,0}, {0,1}),
        MoveSimple({0,0}, {1,0}),
        MoveSimple({0,0}, {1,1}),
    };
    
    EXPECT_EQ(whiteMovesAfterPromo.size(), 5);
    
    EXPECT_EQ(whiteMovesAfterPromo, expectedWhiteMovesAfterPromo);
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
    
    game.makeMoveMaybe(MoveSimple({0,1}, {0,3}));
    
    // Check again
    std::vector<Move> whiteMovesAfterPawnMove = game.possibleMovesForPlayer(0);
    std::vector<Move> expectedWhiteMovesAfterPawnMove{
        MoveSimple({0,3}, {0,4}),
    };
    
    EXPECT_EQ(whiteMovesAfterPawnMove.size(), 1);
    
    EXPECT_EQ(whiteMovesAfterPawnMove, expectedWhiteMovesAfterPawnMove);
}

TEST(ChessGameTest, RookMoves) {
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
    
    ChessGame game = ChessGame(gameString);
    
    // Check moves
    std::vector<Move> whiteMoves = game.possibleMovesForPlayer(0);
    
    // Expected moves:
    //    b2 -> c2
    //    b2 -> d2
    //    b2 -> e2
    //    b2 -> f2
    //    b2 -> g2
    //    b2 -> a2
    //    b2 -> b3
    //    b2 -> b4
    //    b2 -> b1
    std::vector<Move> expectedWhiteMoves{
        MoveSimple({1,1}, {2,1}),
        MoveSimple({1,1}, {3,1}),
        MoveSimple({1,1}, {4,1}),
        MoveSimple({1,1}, {5,1}),
        MoveSimple({1,1}, {6,1}),
        MoveSimple({1,1}, {0,1}),
        MoveSimple({1,1}, {1,2}),
        MoveSimple({1,1}, {1,3}),
        MoveSimple({1,1}, {1,0}),
    };
    
    EXPECT_EQ(whiteMoves.size(), expectedWhiteMoves.size());
    EXPECT_EQ(whiteMoves, expectedWhiteMoves);
}

bool operator<(const Coordinate& left, const Coordinate& right) {
    if (left.file < right.file) {
        return true;
    } else if (left.file < right.file) {
        return left.rank < right.rank;
    }
    
    return false;
}

bool operator<(const MoveSimple& left, const MoveSimple& right) {
    if (left.source < right.source) {
        return true;
    } else if (left.source == right.source) {
        return left.destination < right.destination;
    }
    
    return false;
}

bool operator<(const MoveCastling& left, const MoveCastling& right) {
    if (left.type < right.type) {
        return true;
    } else if (left.type == right.type) {
        if (left.kingsMove < right.kingsMove) {
            return true;
        } else if (left.kingsMove == right.kingsMove) {
            return left.rooksMove < right.rooksMove;
        }
    }
    
    return false;
}

bool operator<(const MovePawnPromotion& left, const MovePawnPromotion& right) {
    if (left.promoteTo < right.promoteTo) {
        return true;
    } else if (left.promoteTo == right.promoteTo) {
        return left.pawnsMove < right.pawnsMove;
    }
    
    return false;
}

TEST(ChessGameTest, QueenMovesEmptyBoard) {
    std::string gameString = R"(
________
________
________
________
________
________
________
Q_______
)";
    
    ChessGame game = ChessGame(gameString);

    std::vector<Move> whiteMoves = game.possibleMovesForPlayer(0);
    Coordinate source{0, 0};
    std::set<Move> expectedWhiteMovesSet{
        MoveSimple(source, {0,1}),
        MoveSimple(source, {0,2}),
        MoveSimple(source, {0,3}),
        MoveSimple(source, {0,4}),
        MoveSimple(source, {0,5}),
        MoveSimple(source, {0,6}),
        MoveSimple(source, {0,7}),
        
        MoveSimple(source, {1,0}),
        MoveSimple(source, {2,0}),
        MoveSimple(source, {3,0}),
        MoveSimple(source, {4,0}),
        MoveSimple(source, {5,0}),
        MoveSimple(source, {6,0}),
        MoveSimple(source, {7,0}),
        
        MoveSimple(source, {1,1}),
        MoveSimple(source, {2,2}),
        MoveSimple(source, {3,3}),
        MoveSimple(source, {4,4}),
        MoveSimple(source, {5,5}),
        MoveSimple(source, {6,6}),
        MoveSimple(source, {7,7}),
    };
    
    std::set<Move> whiteMovesSet(whiteMoves.begin(), whiteMoves.end());
    // If board is empty, then number of possible moves is always 3 * (board_size - 1)
    EXPECT_EQ(whiteMoves.size(), 3 * 7);
    
    EXPECT_EQ(whiteMovesSet, expectedWhiteMovesSet);
}

TEST(ChessGameTest, QueenMovesBlockedByEnemy) {
    std::string gameString = R"(
________
________
________
________
________
________
nn______
Qn______
)";
    
    ChessGame game = ChessGame(gameString);

    std::vector<Move> whiteMoves = game.possibleMovesForPlayer(0);
    Coordinate source{0, 0};
    std::set<Move> expectedWhiteMovesSet{
        MoveSimple(source, {0,1}),
        MoveSimple(source, {1,0}),
        MoveSimple(source, {1,1}),
    };
    
    std::set<Move> whiteMovesSet(whiteMoves.begin(), whiteMoves.end());
    EXPECT_EQ(whiteMoves.size(), 3);
    
    EXPECT_EQ(whiteMovesSet, expectedWhiteMovesSet);
}
