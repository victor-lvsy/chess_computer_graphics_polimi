#include "pieces.h"

// Piece constructor
Piece::Piece(PieceType type, Color color, int x, int y)
    : type(type), color(color), x(x), y(y) {}

Color Piece::getColor() const
{
    return color;
}

std::pair<int, int> Piece::getPosition() const
{
    return {x, y};
}

void Piece::setPosition(int newX, int newY)
{
    x = newX;
    y = newY;
}

void Piece::print() const
{
    static const char *pieceTypeNames[] = {"Pawn", "Knight", "Bishop", "Rook", "Queen", "King"};
    std::cout << "Type: " << pieceTypeNames[static_cast<int>(type)]
              << ", Color: " << (color == Color::WHITE ? "White" : "Black")
              << ", Position: (" << x << ", " << y << ")\n";
}

// Utility function to check if the move is within board boundaries
bool Piece::checkBoundaries(int newX, int newY) const
{
    return !(newX < 0 || newX > 7 || newY < 0 || newY > 7);
}

// Pawn constructor
Pawn::Pawn(Color color, int x, int y)
    : Piece(PieceType::PAWN, color, x, y) {}

bool Pawn::isLegalMove(int newX, int newY) const
{
    // Check if the move is within board boundaries
    if (!checkBoundaries(newX, newY))
    {
        return false;
    }

    int direction = (color == Color::WHITE) ? 1 : -1; // White pawns move up, Black pawns move down
    int startRow = (color == Color::WHITE) ? 1 : 6;

    // Check if move is straight forward by one square
    if (newX == x + direction && newY == y)
    {
        return true;
    }
    // Check if move is straight forward by two squares from the starting position
    if (x == startRow && newX == x + 2 * direction && newY == y)
    {
        return true;
    }
    // Additional rules like capturing moves (diagonal) can be added here
    return false;
}

// Knight constructor
Knight::Knight(Color color, int x, int y)
    : Piece(PieceType::KNIGHT, color, x, y) {}

bool Knight::isLegalMove(int newX, int newY) const
{
    // Check if the move is within board boundaries
    if (!checkBoundaries(newX, newY))
    {
        return false;
    }
    // Calculate the horizontal and vertical distances
    int dx = std::abs(newX - x);
    int dy = std::abs(newY - y);

    // Check for "L" shape movement: (2, 1) or (1, 2)
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

// Rook constructor
Rook::Rook(Color color, int x, int y)
    : Piece(PieceType::ROOK, color, x, y) {}

// Implementing the isLegalMove function for Rook
bool Rook::isLegalMove(int newX, int newY) const
{
    // Check if the move is within board boundaries
    if (!checkBoundaries(newX, newY))
    {
        return false;
    }
    // Rook moves either horizontally or vertically
    return (newX == x) || (newY == y);
}

// Bishop constructor
Bishop::Bishop(Color color, int x, int y)
    : Piece(PieceType::BISHOP, color, x, y) {}

// Implementing the isLegalMove function for the Bishop
bool Bishop::isLegalMove(int newX, int newY) const
{
    // Check if the move is within board boundaries
    if (!checkBoundaries(newX, newY))
    {
        return false;
    }

    // Bishop moves diagonally, so the absolute change in x and y must be the same
    int dx = std::abs(newX - x);
    int dy = std::abs(newY - y);

    return dx == dy;
}

// Queen constructor
Queen::Queen(Color color, int x, int y)
    : Piece(PieceType::QUEEN, color, x, y) {}

// Implementing the isLegalMove function for the Queen
bool Queen::isLegalMove(int newX, int newY) const
{
    // Check if the move is within board boundaries
    if (!checkBoundaries(newX, newY))
    {
        return false;
    }

    // Queen moves like both a Rook and a Bishop:
    // Horizontal, vertical, or diagonal movement
    int dx = std::abs(newX - x);
    int dy = std::abs(newY - y);

    return (newX == x || newY == y) || (dx == dy);
}

// King constructor
King::King(Color color, int x, int y)
    : Piece(PieceType::KING, color, x, y) {}

// Implementing the isLegalMove function for the King
bool King::isLegalMove(int newX, int newY) const
{
    // Check if the move is within board boundaries
    if (!checkBoundaries(newX, newY))
    {
        return false;
    }

    // King moves one square in any direction
    int dx = std::abs(newX - x);
    int dy = std::abs(newY - y);

    // Ensure the move is at most one square in any direction
    return (dx <= 1 && dy <= 1);
}