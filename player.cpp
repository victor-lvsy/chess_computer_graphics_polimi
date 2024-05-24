#include "header/player.h"

// Constructor
Player::Player(const std::string &name) : name(name) {}

// Get the player's name
std::string Player::getName() const
{
    return name;
}

// Add a piece to the list of alive pieces
void Player::addAlivePiece(std::unique_ptr<Piece> piece)
{
    alivePieces.push_back(std::move(piece));
}

// Move a piece to the list of dead pieces
void Player::movePieceToDead(Piece *piece)
{
    // Find the piece in the list of alive pieces
    auto it = std::find_if(alivePieces.begin(), alivePieces.end(),
                           [piece](const std::unique_ptr<Piece> &p)
                           { return p.get() == piece; });

    // If the piece is found, move it to the list of dead pieces
    if (it != alivePieces.end())
    {
        deadPieces.push_back(std::move(*it));
        alivePieces.erase(it);
    }
}

// Get the list of alive pieces
const std::vector<std::unique_ptr<Piece>> &Player::getAlivePieces() const
{
    return alivePieces;
}

// Get the list of dead pieces
const std::vector<std::unique_ptr<Piece>> &Player::getDeadPieces() const
{
    return deadPieces;
}

// Print the details of the player's pieces
void Player::printPieces() const
{
    std::cout << "Player: " << name << "\n";
    std::cout << "Alive pieces:\n";
    for (const auto &piece : alivePieces)
    {
        piece->print();
    }
    std::cout << "Captured pieces:\n";
    for (const auto &piece : deadPieces)
    {
        piece->print();
    }
}