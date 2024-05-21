#include "pieces.h"

int main() {

    // Create a White Pawn at position (1, 0)
    Pawn whitePawn(Color::WHITE, 1, 0);
    // Attempt to move the pawn forward by one square
    int newX = 2, newY = 0;
    if (whitePawn.isLegalMove(newX, newY)) {
        whitePawn.setPosition(newX, newY);
        std::cout << "Move is legal.\n";
    } else {
        std::cout << "Move is illegal.\n";
    }

    whitePawn.print();

    // Create a White Knight at position (0, 1)
    Knight whiteKnight(Color::WHITE, 0, 1);

    // Attempt to move the knight in an "L" shape (2, 3)
    newX = 2, newY = 2;
    if (whiteKnight.isLegalMove(newX, newY)) {
        whiteKnight.setPosition(newX, newY);
        std::cout << "Move is legal.\n";
    } else {
        std::cout << "Move is illegal.\n";
    }
    whiteKnight.print();

    return 0;

}