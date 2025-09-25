
#ifndef CONNECT4_AI_AGENT_HEURISTICBASE_H
#define CONNECT4_AI_AGENT_HEURISTICBASE_H


#include "Board.h"
#include <vector>

class HeuristicBase {
public:
    virtual ~HeuristicBase() {}
    virtual int evaluate(const Board& board, int player) = 0;
};

#endif