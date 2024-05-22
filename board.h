#pragma once
#include <vector>
#include <memory>
#include "pieces.h"
#include "player.h"

// Class representing the Board
class Board
{
private:
    // 8x8 matrix to represent the board, using smart pointers to Pieces
    std::vector<std::vector<std::unique_ptr<Piece>>> board;
    Player player1;
    Player player2;

public:
    // Constructor
    Board(const std::string &player1Name, const std::string &player2Name);

    // Method to display the board in the terminal
    void displayBoard() const;

    // Method to place a piece on the board
    void placePiece(std::unique_ptr<Piece> piece, int x, int y);

    // Method to check if a move is legal
    bool isLegalMove(Piece *piece, int newX, int newY) const;

    // Method to get a piece from the board
    Piece *getPiece(int x, int y) const;

    // Method to move a piece on the board
    void movePiece(Piece *piece, int x, int y);

    // Additional methods for handling board logic can be added here
};