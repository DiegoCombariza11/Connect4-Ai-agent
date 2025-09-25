
#ifndef CONNECT4_AI_AGENT_HARDHEURISTIC_H
#define CONNECT4_AI_AGENT_HARDHEURISTIC_H


#include "HeuristicBase.h"
#include <algorithm>

class HardHeuristic : public HeuristicBase {
public:
    int evaluate(const Board& board, int player) override {
        int score = 0;
        int opponent = (player == 1 ? 2 : 1);

        // Verificar victorias inmediatas
        if (board.checkWin(player)) return 10000;
        if (board.checkWin(opponent)) return -10000;

        // Estrategia defensiva más agresiva
        score += evaluateThreats(board, player, opponent);

        // Control del centro mejorado
        score += evaluateCenter(board, player);

        // Evaluar todas las ventanas con pesos mejorados
        score += evaluateAllWindows(board, player, opponent);

        // Penalizar movimientos que den ventaja al oponente
        score += evaluateOpponentThreats(board, player, opponent);

        return score;
    }

private:
    int evaluateThreats(const Board& board, int player, int opponent) {
        int score = 0;

        // Buscar amenazas inmediatas del oponente (3 en raya)
        for (int col = 0; col < Board::COLS; col++) {
            if (board.isValidMove(col)) {
                Board temp = board;
                temp.dropPiece(col, opponent);
                if (temp.checkWin(opponent)) {
                    score -= 500; // Penalizar mucho si el oponente puede ganar
                }
            }
        }

        return score;
    }

    int evaluateCenter(const Board& board, int player) {
        int score = 0;
        int centerCol = Board::COLS / 2;

        // Puntuar más el control del centro
        for (int row = 0; row < Board::ROWS; row++) {
            if (board.getCell(row, centerCol) == player) score += 5;
        }

        // Puntuar columnas adyacentes al centro
        for (int col = centerCol - 1; col <= centerCol + 1; col += 2) {
            if (col >= 0 && col < Board::COLS) {
                for (int row = 0; row < Board::ROWS; row++) {
                    if (board.getCell(row, col) == player) score += 3;
                }
            }
        }

        return score;
    }

    int evaluateAllWindows(const Board& board, int player, int opponent) {
        int score = 0;

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

        // Vertical y diagonales (similar a MediumHeuristic pero con pesos diferentes)
        // ... (implementación similar pero con evaluateWindow mejorado)

        return score;
    }

    int evaluateOpponentThreats(const Board& board, int player, int opponent) {
        int score = 0;

        // Evaluar si nuestros movimientos crean oportunidades para el oponente
        for (int col = 0; col < Board::COLS; col++) {
            if (board.isValidMove(col)) {
                Board temp = board;
                temp.dropPiece(col, player);

                // Verificar si esto le da una oportunidad al oponente
                for (int oppCol = 0; oppCol < Board::COLS; oppCol++) {
                    if (temp.isValidMove(oppCol)) {
                        Board temp2 = temp;
                        temp2.dropPiece(oppCol, opponent);
                        if (temp2.checkWin(opponent)) {
                            score -= 200; // Penalizar movimientos que den ventaja
                        }
                    }
                }
            }
        }

        return score;
    }

    int evaluateWindow(const std::vector<int>& window, int player, int opponent) {
        int score = 0;
        int playerCount = std::count(window.begin(), window.end(), player);
        int oppCount = std::count(window.begin(), window.end(), opponent);
        int emptyCount = std::count(window.begin(), window.end(), 0);

        // Pesos más agresivos para la dificultad difícil
        if (playerCount == 4) score += 2000;
        else if (playerCount == 3 && emptyCount == 1) score += 200;
        else if (playerCount == 2 && emptyCount == 2) score += 20;

        // Más defensivo contra el oponente
        if (oppCount == 3 && emptyCount == 1) score -= 150;
        else if (oppCount == 2 && emptyCount == 2) score -= 15;

        return score;
    }
};

#endif