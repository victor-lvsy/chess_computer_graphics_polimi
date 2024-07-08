#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <utility>
#include "pieces.h"

// Class representing the Board
class Board
{
private:
    // 8x8 matrix to represent the board, using smart pointers to Pieces
    std::vector<std::vector<std::unique_ptr<Piece>>> board;
    // Vectors to store all pieces of each color
    std::vector<Piece*> whitePieces;
    std::vector<Piece*> blackPieces;
    std::vector<Piece*> allPieces;
    // Iteration indices for each color
    mutable size_t whiteIndex;
    mutable size_t blackIndex;
    // Vector to store captured pieces
    std::vector<std::unique_ptr<Piece>> capturedPieces;

    std::pair<int, int> findKingPosition(Color color) const;
    bool isPositionUnderAttack(int x, int y, Color enemyColor) const;
    bool checkBoundaries(int newX, int newY) const;

public:
    // Constructor
    Board();

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

    void printAllPieces(Color color) const;
    void printCapturedPieces() const;

    void resetHover(Color color) const;
    Piece* getNextPiece(Color color) const;
    
    // Method to check if there is a checkmate situation
    bool isCheckmate(Color color);

    std::vector<Piece*> getAllPieces() const;

};