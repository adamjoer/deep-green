#pragma once

#include "bitboard.h"

#include <array>

namespace Chess {

    enum class Color: char {
        White,
        Black,
    };

    enum class PieceType: int {
        King,
        Queen,
        Rook,
        Bishop,
        Knight,
        Pawn,
    };

    class Board {
    public:
        static Bitboard rookAttacks(Square square, Bitboard occupiedSquares);

        static Bitboard bishopAttacks(Square square, Bitboard occupiedSquares);

        static Bitboard knightAttacks(Square square);

        static Bitboard kingAttacks(Square square);

        static void printAttackRays(Direction direction, Square square = Square::None);

    private:
        /**
         * Bitboards for current state of the game, indexed by enums (Color and PieceType).
         */
        Bitboard bitboards[2][6];

        /**
         * Bitboards with attack rays for sliding pieces, indexed by enums (Direction and Square)
         */
        static const std::array<std::array<Bitboard, 64>, 8> attackRayMasks;

        static const std::array<Bitboard, 64> knightAttackMasks;

        static const std::array<Bitboard, 64> kingAttackMasks;

        static std::array<Bitboard, 64> generateAttackRayMasks(Direction direction);

        static Bitboard generateAttackRayMask(Direction direction, Square square);

        static std::array<Bitboard, 64> generateKnightAttackMasks();

        static Bitboard generateKnightAttackMask(Square square);

        static std::array<Bitboard, 64> generateKingAttackMasks();

        static Bitboard generateKingAttackMask(Square square);

        static Bitboard slidingAttack(Square square, Direction direction, Bitboard occupiedSquares);

        static Bitboard positiveRayAttack(Square square, Direction direction, Bitboard occupiedSquares);

        static Bitboard negativeRayAttack(Square square, Direction direction, Bitboard occupiedSquares);
    };
}
