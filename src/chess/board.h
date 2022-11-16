#pragma once

#include "bitboard.h"

#include <array>

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

    class Board {
    private:
        /*
         * Bitboards for current state of the game, indexed by enums (Color and PieceType).
         */
        Bitboard bitboards[2][6];

        /*
         * Bitboards with attack rays for sliding pieces, indexed by enums (Direction and Square)
         */
        static const std::array<std::array<Bitboard, 64>, 8> attackRays;

        static std::array<Bitboard, 64> generateAttackRays(Direction direction);

        static Bitboard generateAttackRay(Direction direction, Square square);
    };
}
