#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "Player.h"
#include <iostream>

class HumanPlayer : public Player {
public:
    HumanPlayer(int playerId) : id(playerId) {}

    int getMove(const Board& board) override {
        int col;
        do {
            std::cout << "Jugador " << id << ", elige columna (0-"
                      << (Board::COLS - 1) << "): ";
            std::cin >> col;

            if (!board.isValidMove(col)) {
                std::cout << "Movimiento inválido. Intenta de nuevo." << std::endl;
            }
        } while (!board.isValidMove(col));

        return col;
    }

private:
    int id;
};

#endif