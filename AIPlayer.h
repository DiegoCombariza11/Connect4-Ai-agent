#ifndef AIPLAYER_H
    #define AIPLAYER_H

    #include "Player.h"
    #include "EasyHeuristic.h"
    #include "MediumHeursitic.h"
    #include "HardHeuristic.h"
    #include <limits>
    #include <memory>

#include "MediumHeursitic.h"

enum class Difficulty {
        EASY,
        MEDIUM,
        HARD
    };

    enum class Algorithm {
        MINIMAX,
        ALPHA_BETA
    };

    class AIPlayer : public Player {
    public:
        AIPlayer(int playerId, Difficulty difficulty, Algorithm algorithm = Algorithm::ALPHA_BETA)
            : id(playerId), selectedAlgorithm(algorithm) {
            switch (difficulty) {
                case Difficulty::EASY:
                    heuristic = std::make_unique<EasyHeuristic>();
                    depthLimit = 2;
                    break;
                case Difficulty::MEDIUM:
                    heuristic = std::make_unique<MediumHeuristic>();
                    depthLimit = 4;
                    break;
                case Difficulty::HARD:
                    heuristic = std::make_unique<HardHeuristic>();
                    depthLimit = 6;
                    break;
            }
        }

        int getMove(const Board& board) override {
            int bestCol = -1;
            int bestScore = std::numeric_limits<int>::min();
            nodesEvaluated = 0;  // Reset contador

            for (int col : board.getValidMoves()) {
                Board temp = board;
                temp.dropPiece(col, id);

                int score;
                if (selectedAlgorithm == Algorithm::ALPHA_BETA) {
                    score = alphaBeta(temp, depthLimit - 1, false,
                                    std::numeric_limits<int>::min(),
                                    std::numeric_limits<int>::max());
                } else {
                    score = minimax(temp, depthLimit - 1, false);
                }

                if (score > bestScore) {
                    bestScore = score;
                    bestCol = col;
                }
            }
            return bestCol;
        }

        int getNodesEvaluated() const { return nodesEvaluated; }

    private:
        int id;
        int depthLimit;
        Algorithm selectedAlgorithm;
        std::unique_ptr<HeuristicBase> heuristic;
        mutable int nodesEvaluated = 0;

        // Minimax
        int minimax(Board board, int depth, bool maximizing) {
            nodesEvaluated++;

            if (depth == 0 || board.isFull() || board.checkWin(1) || board.checkWin(2)) {
                return heuristic->evaluate(board, id);
            }

            if (maximizing) {
                int maxEval = std::numeric_limits<int>::min();
                for (int col : board.getValidMoves()) {
                    Board temp = board;
                    temp.dropPiece(col, id);
                    int eval = minimax(temp, depth - 1, false);
                    maxEval = std::max(maxEval, eval);
                }
                return maxEval;
            } else {
                int minEval = std::numeric_limits<int>::max();
                int opponent = (id == 1 ? 2 : 1);
                for (int col : board.getValidMoves()) {
                    Board temp = board;
                    temp.dropPiece(col, opponent);
                    int eval = minimax(temp, depth - 1, true);
                    minEval = std::min(minEval, eval);
                }
                return minEval;
            }
        }

        // Alpha-Beta con poda
        int alphaBeta(Board board, int depth, bool maximizing, int alpha, int beta) {
            nodesEvaluated++;

            if (depth == 0 || board.isFull() || board.checkWin(1) || board.checkWin(2)) {
                return heuristic->evaluate(board, id);
            }

            if (maximizing) {
                int maxEval = std::numeric_limits<int>::min();
                for (int col : board.getValidMoves()) {
                    Board temp = board;
                    temp.dropPiece(col, id);
                    int eval = alphaBeta(temp, depth - 1, false, alpha, beta);
                    maxEval = std::max(maxEval, eval);
                    alpha = std::max(alpha, eval);

                    // Poda alfa-beta
                    if (beta <= alpha) {
                        break;  // Corte beta
                    }
                }
                return maxEval;
            } else {
                int minEval = std::numeric_limits<int>::max();
                int opponent = (id == 1 ? 2 : 1);
                for (int col : board.getValidMoves()) {
                    Board temp = board;
                    temp.dropPiece(col, opponent);
                    int eval = alphaBeta(temp, depth - 1, true, alpha, beta);
                    minEval = std::min(minEval, eval);
                    beta = std::min(beta, eval);

                    // Poda alfa-beta
                    if (beta <= alpha) {
                        break;  // Corte alfa
                    }
                }
                return minEval;
            }
        }
    };

    #endif