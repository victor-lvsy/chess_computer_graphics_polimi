#pragma once

#include "board.h"

class Game
{
private:
    Board board;
    Color currPlayer;
    Piece* currPiece;
    std::pair<int, int> nextMoove;

public:
    // Constructor
    Game();

    // Initialize the board with all pieces in starting positions
    void init();

    // Display the current state of the board
    void displayBoard() const;

    // Get a reference to the board
    Board &getBoard();

    Color isPlaying() const;

    void swapColor();

    std::pair<int, int> getNextMoove();

    void setNextMoove(std::pair<int, int> xy);

    Piece* getCurrPiece();
    void setCurrPiece(Piece* piece);
};