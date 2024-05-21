#include "pieces.h"

// Piece constructor
Piece::Piece(PieceType type, Color color, int x, int y)
    : type(type), color(color), x(x), y(y) {}

Color Piece::getColor() const {
    return color;
}

std::pair<int, int> Piece::getPosition() const {
    return {x, y};
}

void Piece::setPosition(int newX, int newY) {x = newX; y = newY;}

void Piece::print() const {
    static const char* pieceTypeNames[] = { "Pawn", "Knight", "Bishop", "Rook", "Queen", "King" };
    std::cout << "Type: " << pieceTypeNames[static_cast<int>(type)]
              << ", Color: " << (color == Color::WHITE ? "White" : "Black")
              << ", Position: (" << x << ", " << y << ")\n";
}

// Pawn constructor
Pawn::Pawn(Color color, int x, int y)
    : Piece(PieceType::PAWN, color, x, y) {}

bool Pawn::isLegalMove(int newX, int newY) const {
    int direction = (color == Color::WHITE) ? 1 : -1; // White pawns move up, Black pawns move down
    int startRow = (color == Color::WHITE) ? 1 : 6;

    // Check if move is straight forward by one square
    if (newX == x + direction && newY == y) {
        return true;
    }
    // Check if move is straight forward by two squares from the starting position
    if (x == startRow && newX == x + 2 * direction && newY == y) {
        return true;
    }
    // Additional rules like capturing moves (diagonal) can be added here
    return false;

}

// Knight constructor
Knight::Knight(Color color, int x, int y)
    : Piece(PieceType::KNIGHT, color, x, y) {}

bool Knight::isLegalMove(int newX, int newY) const {
    // Calculate the horizontal and vertical distances
    int dx = std::abs(newX - x);
    int dy = std::abs(newY - y);

    // Check for "L" shape movement: (2, 1) or (1, 2)
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);

}