#include "board.h"
#include <iostream>
#include <iomanip>

// Constructor

Board::Board(const std::string &player1Name, const std::string &player2Name)
    : player1(player1Name), player2(player2Name)
{
    // Resize the board to 8x8 with nullptr
    board.resize(8);
    for (auto &row : board)
    {
        row.resize(8);
    }
}

void Board::placePiece(std::unique_ptr<Piece> piece, int x, int y)
{
    board[x][y] = std::move(piece);
}

bool Board::isLegalMove(Piece *piece, int newX, int newY) const
{
    if (!piece || !piece->isLegalMove(newX, newY))
        return false;

    int x = piece->getPosition().first;
    int y = piece->getPosition().second;

    // Check if destination is occupied by an ally piece
    if (board[newX][newY] && board[newX][newY]->getColor() == piece->getColor())
    {
        return false;
    }

    // For non-knight pieces, check for obstacles in the path
    if (piece->getType() != PieceType::KNIGHT)
    {
        int dx = (newX - x) == 0 ? 0 : (newX - x) / std::abs(newX - x); // Direction of movement in X
        int dy = (newY - y) == 0 ? 0 : (newY - y) / std::abs(newY - y); // Direction of movement in Y
        int curX = x + dx;
        int curY = y + dy;
        while (curX != newX || curY != newY)
        {
            if (board[curX][curY] != nullptr)
            {
                return false; // There is an obstacle in the way
            }
            curX += dx;
            curY += dy;
        }
    }
    // Allow the move if destination is empty or occupied by an enemy piece
    return true;
}

Piece *Board::getPiece(int x, int y) const
{
    return board[x][y].get();
}

void Board::movePiece(Piece *piece, int newX, int newY)
{
    int x = piece->getPosition().first;
    int y = piece->getPosition().second;

    if (board[newX][newY])
    {
        // Capture enemy piece
        Color enemyColor = board[newX][newY]->getColor();

        std::cout << "Eating a piece" << std::endl;

        if (enemyColor == Color::WHITE)
            player1.movePieceToDead(board[newX][newY].get());
        else
            player2.movePieceToDead(board[newX][newY].get());
    }

    board[newX][newY] = std::move(board[x][y]);
    board[newX][newY]->setPosition(newX, newY);
}

void Board::displayBoard() const
{
    std::cout << "  a b c d e f g h\n";
    for (int i = 7; i >= 0; --i)
    {
        std::cout << i + 1 << " ";
        for (int j = 0; j < 8; ++j)
        {
            if (board[i][j])
            {
                char pieceChar;
                switch (board[i][j]->getType())
                {
                case PieceType::PAWN:
                    pieceChar = 'P';
                    break;
                case PieceType::KNIGHT:
                    pieceChar = 'N';
                    break;
                case PieceType::BISHOP:
                    pieceChar = 'B';
                    break;
                case PieceType::ROOK:
                    pieceChar = 'R';
                    break;
                case PieceType::QUEEN:
                    pieceChar = 'Q';
                    break;
                case PieceType::KING:
                    pieceChar = 'K';
                    break;
                default:
                    pieceChar = ' ';
                    break;
                }
                pieceChar = board[i][j]->getColor() == Color::WHITE ? pieceChar : tolower(pieceChar);
                std::cout << pieceChar << " ";
            }
            else
            {
                std::cout << ". ";
            }
        }
        std::cout << i + 1 << "\n";
    }
    std::cout << "  a b c d e f g h\n";
}