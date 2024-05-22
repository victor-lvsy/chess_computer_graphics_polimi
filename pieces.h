#include <iostream>
#include <string>
#include <cmath>

enum class PieceType
{
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};
enum class Color
{
    WHITE,
    BLACK
};

// Abstract class for a Chess Piece
class Piece
{
protected:
    PieceType type; // Type of the piece
    Color color;    // Color of the piece
    int x, y;       // Position of the piece on the board (row, col)

public:
    Piece(PieceType type, Color color, int x, int y);
    virtual ~Piece() = default;

    Color getColor() const;
    std::pair<int, int> getPosition() const;
    void setPosition(int newX, int newY);

    // Pure virtual function to check if the move is legal
    virtual bool isLegalMove(int newX, int newY) const = 0;

    // Print the details of the piece
    virtual void print() const;

protected:
    // Utility function to check if the move is within board boundaries
    bool checkBoundaries(int newX, int newY) const;
};

// Class for Pawn, derived from Piece
class Pawn : public Piece
{
public:
    Pawn(Color color, int x, int y);
    bool isLegalMove(int newX, int newY) const override;
};

// Class for Knight, derived from Piece
class Knight : public Piece
{
public:
    Knight(Color color, int x, int y);
    bool isLegalMove(int newX, int newY) const override;
};

// Rook class derived from Piece
class Rook : public Piece
{
public:
    Rook(Color color, int x, int y);
    bool isLegalMove(int newX, int newY) const override;
};

// Bishop class derived from Piece
class Bishop : public Piece
{
public:
    Bishop(Color color, int x, int y);
    bool isLegalMove(int newX, int newY) const override;
};

// Queen class derived from Piece
class Queen : public Piece
{
public:
    Queen(Color color, int x, int y);
    bool isLegalMove(int newX, int newY) const override;
};

// King class derived from Piece
class King : public Piece
{
public:
    King(Color color, int x, int y);
    bool isLegalMove(int newX, int newY) const override;
};