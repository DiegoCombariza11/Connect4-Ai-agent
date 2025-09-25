
#ifndef CONNECT4_AI_AGENT_EASYHEURISTIC_H
#define CONNECT4_AI_AGENT_EASYHEURISTIC_H



#include "HeuristicBase.h"

class EasyHeuristic : public HeuristicBase {
public:
    int evaluate(const Board& board, int player) override {
        int opponent = (player == 1 ? 2 : 1);

        // Verificar victoria/derrota inmediata
        if (board.checkWin(player)) return 1000;
        if (board.checkWin(opponent)) return -1000;

        // Evaluation board: mayor peso en el centro, menor en los bordes
        static const int evaluationBoard[Board::ROWS][Board::COLS] = {
            { 3, 4, 5, 7, 5, 4, 3 },
            { 4, 6, 8,10, 8, 6, 4 },
            { 5, 8,11,13,11, 8, 5 },
            { 5, 8,11,13,11, 8, 5 },
            { 4, 6, 8,10, 8, 6, 4 },
            { 3, 4, 5, 7, 5, 4, 3 }
        };

        int score = 0;

        // Evaluar cada posición usando la matriz de evaluación
        for (int row = 0; row < Board::ROWS; row++) {
            for (int col = 0; col < Board::COLS; col++) {
                int cell = board.getCell(row, col);
                if (cell == player) {
                    score += evaluationBoard[row][col];
                } else if (cell == opponent) {
                    score -= evaluationBoard[row][col];
                }
            }
        }

        return score;
    }
};

#endif