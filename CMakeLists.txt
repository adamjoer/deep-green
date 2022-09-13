cmake_minimum_required(VERSION 3.16)
project(chess)

set(CMAKE_CXX_STANDARD 17)

add_executable(chess
    main.cpp
    chess/game.cpp chess/game.h
    chess/piece/piece.h
    chess/piece/king.cpp chess/piece/king.h
    chess/piece/queen.cpp chess/piece/queen.h
    chess/piece/rook.cpp chess/piece/rook.h
    chess/piece/bishop.cpp chess/piece/bishop.h
    chess/piece/knight.cpp chess/piece/knight.h
    chess/piece/pawn.cpp chess/piece/pawn.h
)
