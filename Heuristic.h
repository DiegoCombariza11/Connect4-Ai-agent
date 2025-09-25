#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "Board.h"
#include <vector>
#include <algorithm>

class Heuristic {
public:
    static int evaluate(const Board& board, int player) {
        int score = 0;
        int opponent = (player == 1 ? 2 : 1);

        // 1. Ponderar el centro
        int centerCol = Board::COLS / 2;
        int centerCount = 0;
        for (int row = 0; row < Board::ROWS; row++) {
            // OJO: el Board usa grid privado, habría que agregar un getter.
            // Supongamos que agregas Board::getCell(row, col).
            // Aquí asumo que existe:
            // if (board.getCell(row, centerCol) == player) centerCount++;
        }
        score += centerCount * 3; // peso para centro

        // 2. Revisar todas las "ventanas" de 4
        // Horizontal
        for (int row = 0; row < Board::ROWS; row++) {
            for (int col = 0; col < Board::COLS - 3; col++) {
                std::vector<int> window = {
                    board.getCell(row, col),
                    board.getCell(row, col+1),
                    board.getCell(row, col+2),
                    board.getCell(row, col+3)
                };
                score += evaluateWindow(window, player, opponent);
            }
        }

        // Vertical
        for (int col = 0; col < Board::COLS; col++) {
            for (int row = 0; row < Board::ROWS - 3; row++) {
                std::vector<int> window = {
                    board.getCell(row, col),
                    board.getCell(row+1, col),
                    board.getCell(row+2, col),
                    board.getCell(row+3, col)
                };
                score += evaluateWindow(window, player, opponent);
            }
        }

        // Diagonal positiva
        for (int row = 0; row < Board::ROWS - 3; row++) {
            for (int col = 0; col < Board::COLS - 3; col++) {
                std::vector<int> window = {
                    board.getCell(row, col),
                    board.getCell(row+1, col+1),
                    board.getCell(row+2, col+2),
                    board.getCell(row+3, col+3)
                };
                score += evaluateWindow(window, player, opponent);
            }
        }

        // Diagonal negativa
        for (int row = 3; row < Board::ROWS; row++) {
            for (int col = 0; col < Board::COLS - 3; col++) {
                std::vector<int> window = {
                    board.getCell(row, col),
                    board.getCell(row-1, col+1),
                    board.getCell(row-2, col+2),
                    board.getCell(row-3, col+3)
                };
                score += evaluateWindow(window, player, opponent);
            }
        }

        return score;
    }

private:
    static int evaluateWindow(const std::vector<int>& window, int player, int opponent) {
        int score = 0;
        int playerCount = std::count(window.begin(), window.end(), player);
        int oppCount = std::count(window.begin(), window.end(), opponent);
        int emptyCount = std::count(window.begin(), window.end(), 0);

        if (playerCount == 4) score += 1000;
        else if (playerCount == 3 && emptyCount == 1) score += 100;
        else if (playerCount == 2 && emptyCount == 2) score += 10;

        if (oppCount == 3 && emptyCount == 1) score -= 80;

        return score;
    }
};

#endif
