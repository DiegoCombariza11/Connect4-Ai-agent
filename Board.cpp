#include "Board.h"

Board::Board() {
    grid = std::vector<std::vector<int>>(ROWS, std::vector<int>(COLS, 0));
}

bool Board::dropPiece(int col, int player) {
    if (!isValidMove(col)) return false;
    for (int row = ROWS - 1; row >= 0; --row) {
        if (grid[row][col] == 0) {
            grid[row][col] = player;
            return true;
        }
    }
    return false;
}

bool Board::isValidMove(int col) const {
    return (col >= 0 && col < COLS && grid[0][col] == 0);
}

bool Board::isFull() const {
    for (int col = 0; col < COLS; col++) {
        if (grid[0][col] == 0) return false;
    }
    return true;
}

bool Board::checkWin(int player) const {
    // Revisar horizontal, vertical y diagonales
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (grid[row][col] == player) {
                if (checkDirection(row, col, 0, 1, player)) return true; // Horizontal
                if (checkDirection(row, col, 1, 0, player)) return true; // Vertical
                if (checkDirection(row, col, 1, 1, player)) return true; // Diagonal abajo-derecha
                if (checkDirection(row, col, 1, -1, player)) return true; // Diagonal abajo-izquierda
            }
        }
    }
    return false;
}

bool Board::checkDirection(int row, int col, int dRow, int dCol, int player) const {
    for (int i = 1; i < 4; i++) {
        int r = row + dRow * i;
        int c = col + dCol * i;
        if (r < 0 || r >= ROWS || c < 0 || c >= COLS || grid[r][c] != player) {
            return false;
        }
    }
    return true;
}

void Board::print() const {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            std::cout << grid[row][col] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "---------------" << std::endl;
    for (int col = 0; col < COLS; col++) {
        std::cout << col << " ";
    }
    std::cout << std::endl;
}

std::vector<int> Board::getValidMoves() const {
    std::vector<int> moves;
    for (int col = 0; col < COLS; col++) {
        if (isValidMove(col)) moves.push_back(col);
    }
    return moves;
}
