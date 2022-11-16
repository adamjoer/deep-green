#pragma once

#include "bitBoard.h"

namespace Chess {

    enum class Color {
        White = 0,
        Black = 1
    };

    enum class PieceType {
        King = 0,
        Queen = 1,
        Rook = 2,
        Bishop = 3,
        Knight = 4,
        Pawn = 5
    };

    enum class Castling {
        WhiteKing = 1,
        WhiteQueen = 2,
        BlackKing = 4,
        BlackQueen = 8
    };

    class Board {
    private:
        /*
         * Bitboards for current state of the game, indexed by enums (Color and PieceType).
         */
         BitBoard bitboards[2][6];
    };

}
