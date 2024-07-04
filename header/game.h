#pragma once

#include "board.h"

class Game
{
private:
    Board board;

public:
    // Constructor
    Game();

    // Initialize the board with all pieces in starting positions
    void init();

    // Display the current state of the board
    void displayBoard() const;

    // Get a reference to the board
    Board &getBoard();
};