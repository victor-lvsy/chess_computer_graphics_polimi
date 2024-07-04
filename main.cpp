#include "headers/game.h"

#include <memory> // For using smart pointers


int main()
{

    // Create a game
    Game chessGame;

    // Get references to the board and pieces for convenience
    Board &board = chessGame.getBoard();

     // Initial positions of the White pieces
    board.placePiece(std::make_unique<King>(Color::WHITE, 0, 4), 0, 4);

    board.placePiece(std::make_unique<Pawn>(Color::BLACK, 1, 5), 1, 5);
    board.placePiece(std::make_unique<Pawn>(Color::BLACK, 1, 4), 1, 4);
    board.placePiece(std::make_unique<Pawn>(Color::BLACK, 1, 3), 1, 3);
    board.placePiece(std::make_unique<Pawn>(Color::BLACK, 2, 5), 2, 5);
    board.placePiece(std::make_unique<Pawn>(Color::BLACK, 2, 4), 2, 4);
    board.placePiece(std::make_unique<Pawn>(Color::BLACK, 2, 3), 2, 3);

    board.displayBoard();

    std::cout << "Checking for checkmate..." << std::endl;
    // Check for checkmate
    if (board.isCheckmate(Color::WHITE))
    {
        std::cout << "White is in checkmate!\n";
    }
    else
    {
        std::cout << "White is not in checkmate.\n";
    }



    return 0;
}