#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"

class Player {
public:
    virtual int getMove(const Board& board) = 0;
    virtual ~Player() {}
};

#endif
