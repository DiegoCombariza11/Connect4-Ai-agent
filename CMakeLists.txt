cmake_minimum_required(VERSION 4.0)
project(Connect4_Ai_agent)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Connect4_Ai_agent
        AIPlayer.cpp
        AIPlayer.h
        Board.cpp
        Board.h
        Heuristic.cpp
        Heuristic.h
        HumanPlayer.cpp
        HumanPlayer.h
        main.cpp
        Player.h
        Utils.cpp
        Utils.h
        HeuristicBase.cpp
        HeuristicBase.h
        EasyHeuristic.cpp
        EasyHeuristic.h
        MediumHeursitic.cpp
        MediumHeursitic.h
        HardHeuristic.cpp
        HardHeuristic.h)
