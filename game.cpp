#include "headers/game.h"

// Constructor
Game::Game()
    : board() {}

// Initialize the board with all pieces in starting positions
void Game::init()
{
    
    // Black: {King, Queen, Bishop, Bishop, Knight, Knight, Rook, Rook, Pawn x8}
    board.placePiece(std::make_unique<King>(Color::BLACK, 7, 4), 7, 4);
    board.placePiece(std::make_unique<Queen>(Color::BLACK, 7, 3), 7, 3); 

    board.placePiece(std::make_unique<Bishop>(Color::BLACK, 7, 2), 7, 2);
    board.placePiece(std::make_unique<Bishop>(Color::BLACK, 7, 5), 7, 5);

    board.placePiece(std::make_unique<Knight>(Color::BLACK, 7, 1), 7, 1);
    board.placePiece(std::make_unique<Knight>(Color::BLACK, 7, 6), 7, 6); 

    board.placePiece(std::make_unique<Rook>(Color::BLACK, 7, 0), 7, 0);
    board.placePiece(std::make_unique<Rook>(Color::BLACK, 7, 7), 7, 7);          

    for (int i = 0; i < 8; ++i)
    {
        board.placePiece(std::make_unique<Pawn>(Color::BLACK, 6, i), 6, i);
    }

    // White: {King, Queen, Bishop, Bishop, Knight, Knight, Rook, Rook, Pawn x8}
    board.placePiece(std::make_unique<King>(Color::WHITE, 0, 4), 0, 4);
    board.placePiece(std::make_unique<Queen>(Color::WHITE, 0, 3), 0, 3);

    board.placePiece(std::make_unique<Bishop>(Color::WHITE, 0, 2), 0, 2);
    board.placePiece(std::make_unique<Bishop>(Color::WHITE, 0, 5), 0, 5);

    board.placePiece(std::make_unique<Knight>(Color::WHITE, 0, 1), 0, 1);
    board.placePiece(std::make_unique<Knight>(Color::WHITE, 0, 6), 0, 6);

    board.placePiece(std::make_unique<Rook>(Color::WHITE, 0, 0), 0, 0);
    board.placePiece(std::make_unique<Rook>(Color::WHITE, 0, 7), 0, 7);

    for (int i = 0; i < 8; ++i)
    {
        board.placePiece(std::make_unique<Pawn>(Color::WHITE, 1, i), 1, i);
    }

}

// Display the current state of the board
void Game::displayBoard() const
{
    board.displayBoard();
    board.printAllPieces(Color::WHITE);
    board.printAllPieces(Color::BLACK);
    board.printCapturedPieces();
}

// Get a reference to the board
Board &Game::getBoard()
{
    return board;
}