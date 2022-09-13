cmake_minimum_required(VERSION 3.16)
project(chess)

set(CMAKE_CXX_STANDARD 17)

add_executable(chess
    main.cpp
    chess/game.cpp chess/game.h
    chess/piece.h
    chess/king.cpp chess/king.h
    chess/queen.cpp chess/queen.h
    chess/rook.cpp chess/rook.h
    chess/bishop.cpp chess/bishop.h
    chess/knight.cpp chess/knight.h
    chess/pawn.cpp chess/pawn.h
)
