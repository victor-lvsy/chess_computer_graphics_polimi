#include "headers/board.h"
#include <iostream>
#include <iomanip>

// Constructor

Board::Board() : whiteIndex(0), blackIndex(0)
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

    Piece* rawPiecePtr = board[x][y].get();
    
    if (rawPiecePtr->getColor() == Color::WHITE) {
        whitePieces.push_back(rawPiecePtr);
    } else {
        blackPieces.push_back(rawPiecePtr);
    }
}

void Board::resetHover(Color color) const
{
    if (color == Color::WHITE) {
        whiteIndex = 0;
    } else {
        blackIndex = 0;
    }
}

Piece* Board::getNextPiece(Color color) const
{
    if (color == Color::WHITE) {
        if (whitePieces.empty()) return nullptr;
        Piece* piece = whitePieces[whiteIndex];
        whiteIndex = (whiteIndex + 1) % whitePieces.size();
        return piece;
    } else {
        if (blackPieces.empty()) return nullptr;
        Piece* piece = blackPieces[blackIndex];
        blackIndex = (blackIndex + 1) % blackPieces.size();
        return piece;
    }
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

    // Additional check for pawn diagonal captures
    if (piece->getType() == PieceType::PAWN)
    {
        int dx = newX - x;
        int dy = std::abs(newY - y);

        // Ensure diagonal captures are only valid if an enemy piece is present
        if (dy == 1 && board[newX][newY] == nullptr)
        {
            return false;
        }
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

// Check if the given position is within the boundaries of the board
bool Board::checkBoundaries(int newX, int newY) const
{
    return (newX >= 0 && newX < 8 && newY >= 0 && newY < 8);
}


void Board::movePiece(Piece *piece, int newX, int newY)
{
    int x = piece->getPosition().first;
    int y = piece->getPosition().second;

    if (board[newX][newY])
    {
        // Capture enemy piece
        std::cout << "Eating a piece" << std::endl;

        board[newX][newY].get()->kill();
        // Move captured piece to the garbage bin
        capturedPieces.push_back(std::move(board[newX][newY]));

    }

    board[newX][newY] = std::move(board[x][y]);
    board[newX][newY]->setPosition(newX, newY);
}

// Find the position of a king of given color
std::pair<int, int> Board::findKingPosition(Color color) const
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            const Piece *piece = board[i][j].get();
            if (piece && piece->getType() == PieceType::KING && piece->getColor() == color)
            {
                return {i, j};
            }
        }
    }
    return {-1, -1}; // King not found (shouldn't happen in a valid game)
}

// Check if a given position is under attack by the enemy
bool Board::isPositionUnderAttack(int x, int y, Color enemyColor) const
{
    const std::vector<Piece*>& enemyPieces = (enemyColor == Color::WHITE) ? whitePieces : blackPieces;
    for (const Piece* enemyPiece : enemyPieces)
    {
        if (enemyPiece->isAlive() && isLegalMove(const_cast<Piece*>(enemyPiece), x, y))
        {
            return true;
        }
    }
    return false;
}

bool Board::isCheckmate(Color color)
{
    // Locate the king's position
    std::pair<int, int> kingPos = findKingPosition(color);
    int kingX = kingPos.first;
    int kingY = kingPos.second;

    // Determine the enemy color
    Color enemyColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;

    // Check if the king is in check
    if (!isPositionUnderAttack(kingX, kingY, enemyColor))
    {
        return false;
    }

    // Generate all potential moves for the king and check if any move is safe
    static const int directions[8][2] = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };

    for (const auto& dir : directions)
    {
        int newX = kingX + dir[0];
        int newY = kingY + dir[1];

        if (isLegalMove(board[kingX][kingY].get(),newX, newY)){
            // Temporarily move the piece to see if it resolves the check situation
            std::unique_ptr<Piece> originalPiece = std::move(board[newX][newY]);
            board[newX][newY] = std::move(board[kingX][kingY]);
            board[newX][newY]->setPosition(newX, newY);

            bool stillInCheck = isPositionUnderAttack(newX, newY, enemyColor);

            // Reset board
            board[kingX][kingY] = std::move(board[newX][newY]);
            board[kingX][kingY]->setPosition(kingX, kingY);
            board[newX][newY] = std::move(originalPiece);

            if (!stillInCheck)
            {
                return false; // Found a move that alleviates the check
            }
        }
        
        // if (isLegalMove(board[kingX][kingY].get(),newX, newY) && !isPositionUnderAttack(newX, newY, enemyColor))
        // {
        //     std::cout << newX << " " << newY<< std::endl;
        //     return false; // Found a safe square for the king
        // }
    }

    // Check if any piece of the same color can make a move to block or capture the threatening piece
    const std::vector<Piece*>& friendlyPieces = (color == Color::WHITE) ? whitePieces : blackPieces;
    for (const Piece* piece : friendlyPieces)
    {
        if (!piece->isAlive())
            continue;

        int curX = piece->getPosition().first;
        int curY = piece->getPosition().second;

        // Try every possible destination on the board for the piece
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                if (i == curX && j == curY)
                    continue;

                if (isLegalMove(const_cast<Piece*>(piece), i, j))
                {
                    // Temporarily move the piece to see if it resolves the check situation
                    std::unique_ptr<Piece> originalPiece = std::move(board[i][j]);
                    board[i][j] = std::move(board[curX][curY]);
                    board[i][j]->setPosition(i, j);

                    bool stillInCheck = isPositionUnderAttack(kingX, kingY, enemyColor);

                    // Reset board
                    board[curX][curY] = std::move(board[i][j]);
                    board[curX][curY]->setPosition(curX, curY);
                    board[i][j] = std::move(originalPiece);

                    if (!stillInCheck)
                    {
                        return false; // Found a move that alleviates the check
                    }
                }
            }
        }
    }

    // No moves found that can alleviate check
    return true;
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

void Board::printAllPieces(Color color) const
{
    const std::vector<Piece*>& pieces = (color == Color::WHITE) ? whitePieces : blackPieces;
    
    std::cout << "List of all " << (color == Color::WHITE ? "White" : "Black") << " pieces on the board:\n";
    for (const Piece* piece : pieces)
    {
        if (piece->isAlive()) // Make sure to print only pieces that are still on the board
        {
            piece->print();
        }
    }
}

void Board::printCapturedPieces() const
{
    std::cout << "List of all captured pieces:\n";
    for (const auto& capturedPiece : capturedPieces)
    {
        if (capturedPiece)
        {
            capturedPiece->print();
        }
    }
}