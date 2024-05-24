#include "header/game.h"

#include <memory> // For using smart pointers

int main()
{

    // Create a game with two players
    Game chessGame("Alice", "Bob");

    // Initialize the game (place all pieces)
    chessGame.init();

    // Display the initial state of the board
    chessGame.displayBoard();

    // Get references to the board and pieces for convenience
    Board &board = chessGame.getBoard();

    // Implement a set of moves
    // Move White pawn from (1, 0) to (2, 0)
    // Move White Knight from (0, 1) to (2, 2)
    Piece *whiteKnight1 = board.getPiece(0, 1);
    if (board.isLegalMove(whiteKnight1, 2, 2))
    {
        board.movePiece(whiteKnight1, 2, 2);
        std::cout << "Move White Knight from (0, 1) to (2, 2) is legal.\n";
    }
    else
    {
        std::cout << "Move White Knight from (0, 1) to (2, 2) is illegal.\n";
    }

    chessGame.displayBoard();

    // Move Black pawn from (6, 0) to (5, 0)
    Piece *blackPawn1 = board.getPiece(6, 0);

    if (board.isLegalMove(blackPawn1, 4, 0))
    {
        board.movePiece(blackPawn1, 4, 0);
        std::cout << "Move Black Pawn from (6, 0) to (4, 0) is legal.\n";
    }
    else
    {
        std::cout << "Move Black Pawn from (6, 0) to (4, 0) is illegal.\n";
    }

    chessGame.displayBoard();

    if (board.isLegalMove(whiteKnight1, 4, 3))
    {
        board.movePiece(whiteKnight1, 4, 3);
        std::cout << "Move White Knight to (4, 3) is legal.\n";
    }
    else
    {
        std::cout << "Move White Knight to (4, 3) is illegal.\n";
    }

    chessGame.displayBoard();

    if (board.isLegalMove(whiteKnight1, 6, 4))
    {
        board.movePiece(whiteKnight1, 6, 4);
        std::cout << "Move White Knight to (6, 4) is legal.\n";
    }
    else
    {
        std::cout << "Move White Knight to (6, 4) is illegal.\n";
    }

    chessGame.displayBoard();

    // Additional moves can be added here

    return 0;
}