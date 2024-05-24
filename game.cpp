#include "header/game.h"

// Constructor
Game::Game(const std::string &player1Name, const std::string &player2Name)
    : player1(player1Name), player2(player2Name), board(player1Name, player2Name) {}

// Initialize the board with all pieces in starting positions
void Game::init()
{
    // Place pawns
    for (int i = 0; i < 8; ++i)
    {
        board.placePiece(std::make_unique<Pawn>(Color::WHITE, 1, i), 1, i);
        player1.addAlivePiece(std::make_unique<Pawn>(Color::WHITE, 1, i));

        board.placePiece(std::make_unique<Pawn>(Color::BLACK, 6, i), 6, i);
        player2.addAlivePiece(std::make_unique<Pawn>(Color::BLACK, 6, i));
    }

    // Place rooks
    board.placePiece(std::make_unique<Rook>(Color::WHITE, 0, 0), 0, 0);
    player1.addAlivePiece(std::make_unique<Rook>(Color::WHITE, 0, 0));

    board.placePiece(std::make_unique<Rook>(Color::WHITE, 0, 7), 0, 7);
    player1.addAlivePiece(std::make_unique<Rook>(Color::WHITE, 0, 7));

    board.placePiece(std::make_unique<Rook>(Color::BLACK, 7, 0), 7, 0);
    player2.addAlivePiece(std::make_unique<Rook>(Color::BLACK, 7, 0));

    board.placePiece(std::make_unique<Rook>(Color::BLACK, 7, 7), 7, 7);
    player2.addAlivePiece(std::make_unique<Rook>(Color::BLACK, 7, 7));

    // Place knights
    board.placePiece(std::make_unique<Knight>(Color::WHITE, 0, 1), 0, 1);
    player1.addAlivePiece(std::make_unique<Knight>(Color::WHITE, 0, 1));

    board.placePiece(std::make_unique<Knight>(Color::WHITE, 0, 6), 0, 6);
    player1.addAlivePiece(std::make_unique<Knight>(Color::WHITE, 0, 6));

    board.placePiece(std::make_unique<Knight>(Color::BLACK, 7, 1), 7, 1);
    player2.addAlivePiece(std::make_unique<Knight>(Color::BLACK, 7, 1));

    board.placePiece(std::make_unique<Knight>(Color::BLACK, 7, 6), 7, 6);
    player2.addAlivePiece(std::make_unique<Knight>(Color::BLACK, 7, 6));

    // Place bishops
    board.placePiece(std::make_unique<Bishop>(Color::WHITE, 0, 2), 0, 2);
    player1.addAlivePiece(std::make_unique<Bishop>(Color::WHITE, 0, 2));

    board.placePiece(std::make_unique<Bishop>(Color::WHITE, 0, 5), 0, 5);
    player1.addAlivePiece(std::make_unique<Bishop>(Color::WHITE, 0, 5));

    board.placePiece(std::make_unique<Bishop>(Color::BLACK, 7, 2), 7, 2);
    player2.addAlivePiece(std::make_unique<Bishop>(Color::BLACK, 7, 2));

    board.placePiece(std::make_unique<Bishop>(Color::BLACK, 7, 5), 7, 5);
    player2.addAlivePiece(std::make_unique<Bishop>(Color::BLACK, 7, 5));

    // Place queens
    board.placePiece(std::make_unique<Queen>(Color::WHITE, 0, 3), 0, 3);
    player1.addAlivePiece(std::make_unique<Queen>(Color::WHITE, 0, 3));

    board.placePiece(std::make_unique<Queen>(Color::BLACK, 7, 3), 7, 3);
    player2.addAlivePiece(std::make_unique<Queen>(Color::BLACK, 7, 3));

    // Place kings
    board.placePiece(std::make_unique<King>(Color::WHITE, 0, 4), 0, 4);
    player1.addAlivePiece(std::make_unique<King>(Color::WHITE, 0, 4));

    board.placePiece(std::make_unique<King>(Color::BLACK, 7, 4), 7, 4);
    player2.addAlivePiece(std::make_unique<King>(Color::BLACK, 7, 4));
}

// Display the current state of the board
void Game::displayBoard() const
{
    board.displayBoard();
}

// Get a reference to the board
Board &Game::getBoard()
{
    return board;
}