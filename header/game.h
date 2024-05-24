#pragma once

#include "board.h"
#include "player.h"

class Game
{
private:
    Player player1;
    Player player2;
    Board board;

public:
    // Constructor
    Game(const std::string &player1Name, const std::string &player2Name);

    // Initialize the board with all pieces in starting positions
    void init();

    // Display the current state of the board
    void displayBoard() const;

    // Get a reference to the board
    Board &getBoard();
};