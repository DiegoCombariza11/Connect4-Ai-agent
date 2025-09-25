#include "Board.h"
    #include "AIPlayer.h"
    #include "HumanPlayer.h"
    #include <iostream>
    #include <chrono>
    #include <iomanip>
    #include <thread>
    #include <windows.h>  // Para configurar la consola en Windows

    int main() {
        // Configurar la consola para UTF-8 en Windows
        #ifdef _WIN32
            SetConsoleOutputCP(CP_UTF8);
            SetConsoleCP(CP_UTF8);
            std::locale::global(std::locale(""));
        #endif

        std::cout << "=== CONECTA 4 ===" << std::endl;

        // Selección de modo de juego
        int gameMode;
        std::cout << "Selecciona modo de juego:" << std::endl;
        std::cout << "1. Humano vs IA" << std::endl;
        std::cout << "2. IA vs IA" << std::endl;
        std::cout << "Opción: ";
        std::cin >> gameMode;

        Board board;
        std::unique_ptr<Player> player1;
        std::unique_ptr<Player> player2;

        // Variables para estadísticas
        double totalTimePlayer1 = 0.0;
        double totalTimePlayer2 = 0.0;
        int movesPlayer1 = 0;
        int movesPlayer2 = 0;
        int totalNodesPlayer1 = 0;
        int totalNodesPlayer2 = 0;

        auto getDifficultyChoice = []() -> Difficulty {
            int choice;
            std::cout << "1. Fácil  2. Media  3. Difícil" << std::endl;
            std::cout << "Dificultad: ";
            std::cin >> choice;
            return (choice == 1) ? Difficulty::EASY :
                   (choice == 3) ? Difficulty::HARD : Difficulty::MEDIUM;
        };

        auto getAlgorithmChoice = []() -> Algorithm {
            int choice;
            std::cout << "Selecciona algoritmo:" << std::endl;
            std::cout << "1. Minimax tradicional" << std::endl;
            std::cout << "2. Alpha-Beta con poda (recomendado)" << std::endl;
            std::cout << "Algoritmo: ";
            std::cin >> choice;
            return (choice == 1) ? Algorithm::MINIMAX : Algorithm::ALPHA_BETA;
        };

        if (gameMode == 1) {
            // Humano vs IA
            player1 = std::make_unique<HumanPlayer>(1);

            std::cout << "\nConfigurando IA:" << std::endl;
            Difficulty difficulty = getDifficultyChoice();
            Algorithm algorithm = getAlgorithmChoice();

            std::string diffName = (difficulty == Difficulty::EASY) ? "FÁCIL" :
                                  (difficulty == Difficulty::HARD) ? "DIFÍCIL" : "MEDIO";
            std::string algName = (algorithm == Algorithm::MINIMAX) ? "Minimax" : "Alpha-Beta";

            std::cout << "IA configurada: " << diffName << " con " << algName << std::endl;
            player2 = std::make_unique<AIPlayer>(2, difficulty, algorithm);

        } else if (gameMode == 2) {
            // IA vs IA
            std::cout << "\nConfigurando IA 1:" << std::endl;
            Difficulty difficulty1 = getDifficultyChoice();
            Algorithm algorithm1 = getAlgorithmChoice();

            std::cout << "\nConfigurando IA 2:" << std::endl;
            Difficulty difficulty2 = getDifficultyChoice();
            Algorithm algorithm2 = getAlgorithmChoice();

            auto getDifficultyName = [](Difficulty d) -> std::string {
                switch (d) {
                    case Difficulty::EASY: return "FÁCIL";
                    case Difficulty::MEDIUM: return "MEDIO";
                    case Difficulty::HARD: return "DIFÍCIL";
                }
                return "MEDIO";
            };

            auto getAlgorithmName = [](Algorithm a) -> std::string {
                return (a == Algorithm::MINIMAX) ? "Minimax" : "Alpha-Beta";
            };

            std::cout << "\nIA 1: " << getDifficultyName(difficulty1)
                      << " (" << getAlgorithmName(algorithm1) << ") vs ";
            std::cout << "IA 2: " << getDifficultyName(difficulty2)
                      << " (" << getAlgorithmName(algorithm2) << ")" << std::endl;

            player1 = std::make_unique<AIPlayer>(1, difficulty1, algorithm1);
            player2 = std::make_unique<AIPlayer>(2, difficulty2, algorithm2);

        } else {
            std::cout << "Opción inválida. Saliendo..." << std::endl;
            return 1;
        }

        // Bucle principal del juego
        int currentPlayer = 1;
        bool gameEnded = false;
        int moveNumber = 1;

        std::cout << "\nIniciando juego...\n" << std::endl;
        board.print();

        while (!gameEnded) {
            Player* current = (currentPlayer == 1) ? player1.get() : player2.get();
            AIPlayer* aiCurrent = dynamic_cast<AIPlayer*>(current);

            if (gameMode == 2) {
                std::cout << "\nMovimiento " << moveNumber << " - Jugador " << currentPlayer << std::endl;
            } else {
                std::cout << "\nTurno del Jugador " << currentPlayer << std::endl;
            }

            // Medir tiempo y nodos evaluados
            auto startTime = std::chrono::high_resolution_clock::now();
            int col = current->getMove(board);
            auto endTime = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
            double timeInMs = duration.count() / 1000.0;

            // Registrar estadísticas
            if (gameMode == 2 || (gameMode == 1 && aiCurrent)) {
                int nodesEvaluated = aiCurrent ? aiCurrent->getNodesEvaluated() : 0;

                if (currentPlayer == 1) {
                    totalTimePlayer1 += timeInMs;
                    totalNodesPlayer1 += nodesEvaluated;
                    movesPlayer1++;
                } else {
                    totalTimePlayer2 += timeInMs;
                    totalNodesPlayer2 += nodesEvaluated;
                    movesPlayer2++;
                }

                if (aiCurrent) {
                    std::cout << "Tiempo: " << std::fixed << std::setprecision(2)
                              << timeInMs << " ms, Nodos evaluados: " << nodesEvaluated << std::endl;
                }
            }

            // Realizar movimiento
            if (board.isValidMove(col) && board.dropPiece(col, currentPlayer)) {
                std::cout << "Jugador " << currentPlayer << " juega en columna " << col << std::endl;
                board.print();

                if (board.checkWin(currentPlayer)) {
                    std::cout << "\nJUGADOR " << currentPlayer << " GANA" << std::endl;
                    gameEnded = true;
                } else if (board.isFull()) {
                    std::cout << "\nEMPATE! El tablero está lleno." << std::endl;
                    gameEnded = true;
                } else {
                    currentPlayer = (currentPlayer == 1) ? 2 : 1;
                    moveNumber++;
                }
            } else {
                std::cout << "Movimiento inválido. Intenta de nuevo." << std::endl;
            }

            // Pausa para visualización
            if (gameMode == 2 && !gameEnded) {
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
            }
        }

        // Mostrar estadísticas detalladas
        if (gameMode == 2) {
            std::cout << "\n=== ESTADÍSTICAS DETALLADAS ===" << std::endl;
            std::cout << std::fixed << std::setprecision(2);

            auto printStats = [](const std::string& name, int moves, double totalTime, int totalNodes) {
                std::cout << name << ":" << std::endl;
                std::cout << "  - Movimientos: " << moves << std::endl;
                std::cout << "  - Tiempo total: " << totalTime << " ms" << std::endl;
                std::cout << "  - Nodos evaluados: " << totalNodes << std::endl;
                if (moves > 0) {
                    std::cout << "  - Tiempo promedio: " << (totalTime / moves) << " ms/movimiento" << std::endl;
                    std::cout << "  - Nodos promedio: " << (totalNodes / moves) << " nodos/movimiento" << std::endl;
                }
                std::cout << std::endl;
            };

            printStats("Jugador 1", movesPlayer1, totalTimePlayer1, totalNodesPlayer1);
            printStats("Jugador 2", movesPlayer2, totalTimePlayer2, totalNodesPlayer2);

            std::cout << "Tiempo total del juego: " << (totalTimePlayer1 + totalTimePlayer2) << " ms" << std::endl;
            std::cout << "Nodos totales evaluados: " << (totalNodesPlayer1 + totalNodesPlayer2) << std::endl;
        }

        std::cout << "\nGracias por jugar" << std::endl;
        return 0;
    }