#ifndef BOARD_H
    #define BOARD_H

    #include <vector>
    #include <iostream>

    class Board {
    public:
        static const int ROWS = 6;
        static const int COLS = 7;

        Board();
        bool dropPiece(int col, int player);
        bool isValidMove(int col) const;
        bool isFull() const;
        bool checkWin(int player) const;
        void print() const;
        std::vector<int> getValidMoves() const;

        int getCell(int row, int col) const {
            return grid[row][col];
        }

    private:
        std::vector<std::vector<int>> grid;
        bool checkDirection(int row, int col, int dRow, int dCol, int player) const;
    };

    #endif


