#pragma once
#include <string>
#include <vector>
#include <memory> // For using smart pointers
#include "pieces.h"
#include <algorithm>

// Class representing a Player in the game
class Player
{
private:
    std::string name;                                // Name of the player
    std::vector<std::unique_ptr<Piece>> alivePieces; // List of alive pieces
    std::vector<std::unique_ptr<Piece>> deadPieces;  // List of captured pieces

public:
    // Constructor
    Player(const std::string &name);

    // Get the player's name
    std::string getName() const;

    // Add a piece to the list of alive pieces
    void addAlivePiece(std::unique_ptr<Piece> piece);

    // Move a piece to the list of dead pieces
    void movePieceToDead(Piece *piece);

    // Get the list of alive pieces
    const std::vector<std::unique_ptr<Piece>> &getAlivePieces() const;

    // Get the list of dead pieces
    const std::vector<std::unique_ptr<Piece>> &getDeadPieces() const;

    // Print the details of the player's pieces
    void printPieces() const;
};