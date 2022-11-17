#pragma once

#include "bitboard.h"

#include <array>

namespace Chess {

    enum class Color {
        White,
        Black,
    };

    enum class PieceType {
        King,
        Queen,
        Rook,
        Bishop,
        Knight,
        Pawn,
    };

    enum class castlingBits {
        WhiteKing = 1,
        WhiteQueen = 2,
        BlackKing = 4,
        BlackQueen = 8,
    };

    class Board {
    public:
        explicit Board(std::string &fen);
        Bitboard bitboards[2][6];
        uint8_t castlingRights;
        Color playerTurn;
        Square enPassant;
        uint8_t fiftyMoveCounter;
        uint16_t fullMoveCounter;

        static Bitboard rookAttacks(Square square, Bitboard occupiedSquares);

        static Bitboard bishopAttacks(Square square, Bitboard occupiedSquares);

        static Bitboard knightAttacks(Square square);

        static Bitboard kingAttacks(Square square);

        static Bitboard queenAttacks(Square square, Bitboard occupiedSquares);

        static Bitboard pawnAttacks(Square square, Bitboard occupiedSquares, Color color);

        static void printAttackRays(Direction direction, Square square = Square::None);

    private:
        /**
         * Bitboards for current state of the game, indexed by enums (Color and PieceType).
         */

        /**
         * Bitboards with attack rays for sliding pieces, indexed by enums (Direction and Square)
         */
        static const std::array<std::array<Bitboard, 64>, 8> attackRayMasks;

        static const std::array<Bitboard, 64> knightAttackMasks;

        static const std::array<Bitboard, 64> kingAttackMasks;

        static const std::array<std::array<Bitboard, 64>, 2> pawnAttackMasks;

        static std::array<Bitboard, 64> generateAttackRayMasks(Direction direction);

        static Bitboard generateAttackRayMask(Direction direction, Square square);

        static std::array<Bitboard, 64> generateKnightAttackMasks();

        static Bitboard generateKnightAttackMask(Square square);

        static std::array<Bitboard, 64> generateKingAttackMasks();

        static Bitboard generateKingAttackMask(Square square);

        static std::array<std::array<Bitboard, 64>, 2> generatePawnAttackMasks();

        static Bitboard generatePawnAttackMask(Square square, Color color);

        static Bitboard slidingAttack(Square square, Direction direction,
                                      Bitboard occupiedSquares);

        static Bitboard positiveRayAttack(Square square, Direction direction,
                                          Bitboard occupiedSquares);

        static Bitboard negativeRayAttack(Square square, Direction direction,
                                          Bitboard occupiedSquares);
    };
}
